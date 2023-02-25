/*
 * Master Code.c
 *
 * Created: 13/01/2023 01:23:59 م
 * Author : Ahmed Hassan 
 */ 
#include <avr/io.h>
#include "LCD.h"
#include "LED.h"
#include "Keypad.h"
#include "timers.h"
#include "App_config.h"
#include "menu.h"
#include "SPI.h"
#include "EEPROM.h"
volatile uint16 session_counter = 0;//used to calculate session time
uint8 timeout_flag = FALSE;//used to know if the session is still valid or timed out 

int main(void)
{
	uint8 temperature = 0;//The average temperature of the room
	uint8 temp_ones = NOT_SELECTED;//The entered right number of the temperature
	uint8 temp_tens = NOT_SELECTED;//The entered left number of the temperature
	uint8 key_pressed = NOT_PRESSED;	
	uint8 login_mode = NO_MODE; //Store the current user mode admin or guest or not logged in
	uint8 block_mode_flag = FALSE;//is true if the login is blocked or false if not blocked
	/**********************  Initialization  ********************************************/
    LCD_intial();//initializes the LCD screen
	keypad_vInit();//initializes the keypad
	SPI_vInitMaster();//initializes the spi protocol
	LED_vInit(ADMIN_LED_PORT,ADMIN_LED_PIN) ;//initializes the admin led 
	LED_vInit(GUEST_LED_PORT,GUEST_LED_PIN) ;//initializes the admin led 
	LED_vInit(BLOCK_LED_PORT,BLOCK_LED_PIN) ;//initializes the admin led 
	/**********************  Printing welcome message ********************************************/
	LCD_send_string("welcome to smart");
	LCD_move_cursor(2,1); 
	LCD_send_string("home system!");
	_delay_ms(1000);
	LCD_clear_screen();
	/**********************  Setting Admin and Geust password if not set ********************************************/
	if ((EEPROM_read(ADMIN_PASS_STATUS_ADDRESS)!=PASS_SET)||(EEPROM_read(GUEST_PASS_STATUS_ADDRESS)!=PASS_SET))
	{
		LCD_send_string("Login for");
		LCD_move_cursor(2,1);
		LCD_send_string("first time");
		_delay_ms(1000);
		LCD_clear_screen();
		LCD_send_string("set admin pass");
		LCD_move_cursor(2,1);
		LCD_send_string("admin pass:");
	///////////////////////Setting Admin Password /////////////////////////////////////////////////////////////
	uint8 pass_counter = 0;
	for(pass_counter=0;pass_counter<PASS_SIZE;pass_counter++)
	{
		do
		{
			key_pressed = keypad_u8check_press();
		} while (key_pressed==NOT_PRESSED);
		LCD_send_char(key_pressed);
		_delay_ms(CHARACTER_PREVIEW_TIME);
		LCD_move_cursor(2,12+pass_counter);
		LCD_send_char(PASS_SYMPOL);
		EEPROM_write(EEPROM_ADMIN_ADDRESS+pass_counter,key_pressed);
	}
	EEPROM_write(ADMIN_PASS_STATUS_ADDRESS,PASS_SET);
	LCD_clear_screen();
	LCD_send_string("password set");
	LCD_move_cursor(2,1);
	LCD_send_string("successfully");
	_delay_ms(500);
	LCD_clear_screen();
	////////////////////////Setting Guest Password/////////////////////////////////////////////////////////
	LCD_clear_screen();
	LCD_send_string("set Guest pass");
	LCD_move_cursor(2,1);
	LCD_send_string("Guest pass:");
	for(pass_counter=0;pass_counter<PASS_SIZE;pass_counter++)
	{
		do
		{
			key_pressed = keypad_u8check_press();
		} while (key_pressed==NOT_PRESSED);
		LCD_send_char(key_pressed);
		_delay_ms(CHARACTER_PREVIEW_TIME);
		LCD_move_cursor(2,12+pass_counter);
		LCD_send_char(PASS_SYMPOL);
		EEPROM_write(EEPROM_GUEST_ADDRESS+pass_counter,key_pressed);
	}
	EEPROM_write(GUEST_PASS_STATUS_ADDRESS,PASS_SET);
	LCD_clear_screen();
	LCD_send_string("password set");
	LCD_move_cursor(2,1);
	LCD_send_string("successfully");
	_delay_ms(500);
	LCD_clear_screen();
	EEPROM_write(LOGIN_BLOCKED_ADDRESS,FALSE);
	}
	/********************************************************************************/
	while (1)
	{
		key_pressed = NOT_PRESSED;//return the variable that holds the pressed key from keypad to its initial value
		uint8 pass_tries_count=0;//stores how many times the user tried to log in to the system and failed
			/*******************handling Time out ********************************************************/
		if ( timeout_flag==TRUE )//check for timeout
		{//if timeout flag was raised
			timer0_stop();//stop the timer that increase the session counter
			session_counter = 0;//clear session counter
			timeout_flag=FALSE;//clear time out flag
			login_mode=NO_MODE;//log the user out
			key_pressed = NOT_PRESSED;//clear the key_pressed to avoid unwanted selection in the menu switch
			LED_vTurnOff(GUEST_LED_PORT,GUEST_LED_PIN);//turnoff the led of the guest
			LED_vTurnOff(ADMIN_LED_PORT,ADMIN_LED_PIN);//turnoff the led of the admin
			LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			LCD_send_string("Session Timeout");//print session timeout message
			_delay_ms(1000);//Halt the system for the given time in (ms)
		}
		while (login_mode==NO_MODE)//The user can only leave the loop only in case of he was logged in as guest or admin
		{
			/*******************handling block mode********************************************************/
			if(block_mode_flag==TRUE)//if the login process was blocked wait till the end of the block period
			{
				LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
				LCD_send_string("Login blocked");
				LCD_move_cursor(2,1);
				LCD_send_string("wait 20 seconds");
				LED_vTurnOn(BLOCK_LED_PORT,BLOCK_LED_PIN);//Turn on the led of Blocked
				_delay_ms(BLOCK_MODE_TIME);//Halt the system for the given time in (ms)
				pass_tries_count = 0; //Clear the count on number of wrong tries
				block_mode_flag = FALSE;//Disable block of runtime
				LED_vTurnOff(BLOCK_LED_PORT,BLOCK_LED_PIN);//Turn off the led of Blocked
				EEPROM_write(LOGIN_BLOCKED_ADDRESS,FALSE);//write false at blocked location in EEPROM
			}
			/**************************************************************************************************/
			LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
			LCD_send_string("Select mode:");
			LCD_move_cursor(2,1);
			LCD_send_string("0:Admin 1:Guest");
			/********************Handling wrong choice of keys***************************************************/
			do
			{
				key_pressed = keypad_u8check_press();
			} while (key_pressed==NOT_PRESSED);
			if ((key_pressed!=CHECK_ADMIN_MODE)&&(key_pressed!=CHECK_GUEST_MODE))
			{
				LCD_clear_screen();
				LCD_send_string("wrong input");
				key_pressed = NOT_PRESSED;
				_delay_ms(1000);
				continue;
			}
			/*************************************************************************************************/
			uint8 pass_counter=0;//counts the entered key of the password from the keypad
			uint8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};//temporarily hold the entire password that will be entered by the user to be check
			switch (key_pressed)
			{
				/********************************* Admin login **********************************************/
				case CHECK_ADMIN_MODE :
				while (login_mode!=ADMIN)
				{
					LCD_clear_screen();
					LCD_send_string("Admin mode");
					LCD_move_cursor(2,1);
					LCD_send_string("Enter pass:");
					_delay_ms(200);//Halt the system for the given time in (ms)
					for(pass_counter=0;pass_counter<PASS_SIZE;pass_counter++)
					{
						do
						{
							pass[pass_counter] = keypad_u8check_press();
						} while (pass[pass_counter]==NOT_STORED);
						LCD_send_char(pass[pass_counter]);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_move_cursor(2,12+pass_counter);
						LCD_send_char(PASS_SYMPOL);
					}//end of for loop 
					/***********compare passwords**************/
					if (EEPROM_read(EEPROM_ADMIN_ADDRESS)==pass[0]&&EEPROM_read(EEPROM_ADMIN_ADDRESS+1)==pass[1]&&EEPROM_read(EEPROM_ADMIN_ADDRESS+2)==pass[2]&&EEPROM_read(EEPROM_ADMIN_ADDRESS+3)==pass[3])
					{
						login_mode = ADMIN;
						pass_tries_count = 0;
						LCD_clear_screen();
						LCD_send_string("Right password");
						LCD_move_cursor(2,1);
						LCD_send_string("Admin mode");
						_delay_ms(500);
						LED_vTurnOn(ADMIN_LED_PORT,ADMIN_LED_PIN);
						timer_CTC_init_interrupt();///////////////////////////////////
						LCD_clear_screen();
					}
					else/*********in case of wrong password*****************/
					{
						pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
						login_mode = NO_MODE;//set the mode as not logged in
						LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_send_string("Wrong Pass");
						LCD_move_cursor(2,1);
						LCD_send_string("Tries left:");
						LCD_send_char(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);//print the number of tries left before block mode to be activated
						_delay_ms(1000);//Halt the system for the given time in (ms)
						LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
						{
							EEPROM_write(LOGIN_BLOCKED_ADDRESS,TRUE);//write to the EEPROM TRUE to the the block mode address
							block_mode_flag = TRUE;//turn on block mode
							break;//break the loop of admin login #while(login_mode!=ADMIN)# at line 
						}
					}//end of else 
					
				}//end of while loop
				
				break;//break of switch case
				/********************************* Guest login **********************************************/
				case CHECK_GUEST_MODE:
				while (login_mode!=GUEST)
				{
					LCD_clear_screen();
					LCD_send_string("Guest mode");
					LCD_move_cursor(2,1);
					LCD_send_string("Enter pass:");
					_delay_ms(200);//Halt the system for the given time in (ms)
					for(pass_counter=0;pass_counter<PASS_SIZE;pass_counter++)
					{
						do
						{
							pass[pass_counter] = keypad_u8check_press();
						} while (pass[pass_counter]==NOT_STORED);
						LCD_send_char(pass[pass_counter]);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_move_cursor(2,12+pass_counter);
						LCD_send_char(PASS_SYMPOL);
					}//end of for loop
					/****************compare passwords*/
					if (EEPROM_read(EEPROM_GUEST_ADDRESS)==pass[0]&&EEPROM_read(EEPROM_GUEST_ADDRESS+1)==pass[1]&&EEPROM_read(EEPROM_GUEST_ADDRESS+2)==pass[2]&&EEPROM_read(EEPROM_GUEST_ADDRESS+3)==pass[3])
					{
						login_mode = GUEST;
						pass_tries_count = 0;
						LCD_clear_screen();
						LCD_send_string("Right password");
						LCD_move_cursor(2,1);
						LCD_send_string("Guest mode");
						_delay_ms(500);
						LED_vTurnOn(GUEST_LED_PORT,GUEST_LED_PIN);
						timer_CTC_init_interrupt();//////////////////////////////////
						LCD_clear_screen();
					}
					else//in case of wrong password
					{
						pass_tries_count++;//increase the number of wrong tries to block login if it exceeds the allowed tries
						login_mode = NO_MODE;//set the mode as not logged in
						LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						LCD_send_string("Wrong Pass");
						LCD_move_cursor(2,1);
						LCD_send_string("Tries left:");
						LCD_send_char(TRIES_ALLOWED-pass_tries_count+ASCII_ZERO);//print the number of tries left before block mode to be activated
						_delay_ms(1000);//Halt the system for the given time in (ms)
						LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
						if (pass_tries_count>=TRIES_ALLOWED)//if the condition of the block mode is true
						{
							EEPROM_write(LOGIN_BLOCKED_ADDRESS,TRUE);//write to the EEPROM TRUE to the the block mode address
							block_mode_flag = TRUE;//turn on block mode
							break;//break the loop of admin login #while(login_mode!=ADMIN)# at line
						}
					}//end of else				
				}//end of while loop
				break;//break of switch case				
			}//end of switch 			
		}//end of while (login_mode==NO_MODE)
		/**********************************************************************************************/
		/**********************************************************************************************/
		/**********************************************************************************************/
		/**********************************************************************************************/
				uint8 show_menu = MAIN_MENU;
				
					while(timeout_flag!=TRUE)//Show the menu in case of the time is not out
					{
						key_pressed = NOT_PRESSED;//Set the key pressed by the user to its default value
						switch(show_menu)
						{
							
							case MAIN_MENU:
							while(((key_pressed < '1') || (key_pressed > '4')) && (timeout_flag == FALSE))
							{
								/******************** print main Menu ******************/
								LCD_clear_screen();
								LCD_send_string("1:Room1 2:Room2");
								LCD_move_cursor(2,1);
								if(login_mode==ADMIN)//check login mode
								{
									LCD_send_string("3:Room3 4:More ");//this menu options only printed if the logged in user is an admin
								}
								else if(login_mode==GUEST)//check login mode
								{
									LCD_send_string("3:Room3 4:Room4");//this menu options only printed if the logged in user is a guest
								}
								/*******************************************************/
								
								key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
								_delay_ms(100);//to avoid the duplication of the pressed key
								
								if (key_pressed == SELECT_ROOM1)//If key pressed is 1
								{
									show_menu = ROOM1_MENU;//Set the next menu to be shown to room1 menu
								}
								else if (key_pressed == SELECT_ROOM2)//If key pressed is 2
								{
									show_menu = ROOM2_MENU;//Set the next menu to be shown to room2 menu
								}
								else if (key_pressed == SELECT_ROOM3)//If key pressed is 3
								{
									show_menu = ROOM3_MENU;//Set the next menu to be shown to room3 menu
								}
								else if (key_pressed == SELECT_ROOM4 && login_mode == GUEST)//If key pressed is 4 and the logged in user is guest
								{
									show_menu = ROOM4_MENU;//Set the next menu to be shown to room4 menu
								}
								else if (key_pressed == ADMIN_MORE_OPTION && login_mode == ADMIN)//If key pressed is 4 and the logged in user is admin
								{
									show_menu = MORE_MENU;//Set the next menu to be shown to more menu
								}
								else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
								{
									LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
									LCD_send_string("Wrong input");//print error message
									_delay_ms(500);//Halt the system for the given time in (ms)
								}
							}
							break;//End of main menu case
							
							case MORE_MENU:
							while (((key_pressed < '1') || (key_pressed > '4')) && (timeout_flag == FALSE))
							{
								/******************** print more Menu ******************/
								LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
								LCD_send_string("1:Room4    2:TV   ");
								LCD_move_cursor(2,1);
								LCD_send_string("3:Air Cond.4:RET");
								/*******************************************************/
								key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
								_delay_ms(100);//to avoid the duplication of the pressed key
								
								if (key_pressed == SELECT_ROOM4_ADMIN)//If key pressed is 1
								{
									show_menu = ROOM4_MENU;//Set the next menu to be shown to room4 menu
								}
								else if (key_pressed == SELECT_TV)//If key pressed is 2
								{
									show_menu = TV_MENU;//Set the next menu to be shown to TV menu
								}
								else if (key_pressed == SELECT_AIR_CONDITIONING)//If key pressed is 3
								{
									show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to Air conditioning menu
								}
								else if (key_pressed == ADMIN_RET_OPTION)//If key pressed is 4 (RET)
								{
									show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
								}
								else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
								{
									LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
									LCD_send_string("Wrong input");//print error message
									_delay_ms(500);//Halt the system for the given time in (ms)
								}
							}
							break;//End of more menu case	
							case AIRCONDITIONING_MENU:	
							while (((key_pressed < '0') || (key_pressed > '2'))&& (timeout_flag == FALSE))	
							{
								/******************** print more Menu ******************/
								LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
								LCD_send_string("1:Set Temperature");
								LCD_move_cursor(2,1);
								LCD_send_string("2:control 0:RET");
								/*******************************************************/
								key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
								_delay_ms(100);//to avoid the duplication of the pressed key
								
								if (key_pressed == SELECT_SET_TEMPERATURE)//If key pressed is 1
								{
									show_menu = TEMPERATURE_MENU;//Set the next menu to be shown to set temperature menu
								}
								else if (key_pressed == SELECT_AIR_COND_CTRL)//If key pressed is 2
								{
									show_menu = AIRCOND_CTRL_MENU;//Set the next menu to be shown to air conditioning control menu
								}
								else if (key_pressed == SELECT_AIR_COND_RET)//If key pressed is 0
								{
									show_menu = MORE_MENU;//Set the next menu to be shown to more menu
								}
								else if(key_pressed != NOT_PRESSED)//show wrong input message if the user pressed wrong key
								{
									LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
									LCD_send_string("Wrong input");//print error message
									_delay_ms(500);//Halt the system for the given time in (ms)
								}
							}
							
							break;//End of more menu case	
							case ROOM1_MENU :
							vMenuOption(ROOM1_MENU,login_mode);//call the function that show the menu of room 1
							show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
							break;	//End of room1 menu case
							
							case ROOM2_MENU :
							vMenuOption(ROOM2_MENU,login_mode);//call the function that show the menu of room 1
							show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
							break;//End of room2 menu case
							
							case ROOM3_MENU :
							vMenuOption(ROOM3_MENU,login_mode);//call the function that show the menu of room 1
							show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
							break;//End of room3 menu case
							
							case ROOM4_MENU :
							vMenuOption(ROOM4_MENU,login_mode);//call the function that show the menu of room 1
							if (login_mode == GUEST)//in case of guest is logged in
							{
								show_menu = MAIN_MENU;//Set the next menu to be shown to main menu
							}
							else//in case of admin is logged in
							{
								show_menu = MORE_MENU;//Set the next menu to be shown to more menu
							}
							break;//End of room4 menu case
							case TV_MENU:
							vMenuOption(TV_MENU,login_mode);//call the function that show the menu of tv
							show_menu = MORE_MENU;//Set the next menu to be shown to more menu
							break;//End of TV menu case
							case AIRCOND_CTRL_MENU:
							vMenuOption(AIRCOND_CTRL_MENU,login_mode);//call the function that show the menu of Air conditioning control
							show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
							break;//End of air conditioning control menu case
							/**********************************************************************************************/
							case TEMPERATURE_MENU:
							temperature = 0;//clear the value of temperature
							while (temperature==0 && timeout_flag == FALSE)//start the loop that asks for the temperature from the user in case of the time is not out
							{
								key_pressed = NOT_PRESSED;//set the key pressed to the default value
								LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
								LCD_send_string("Set temp.:__ ");//print the format of inserting temperature
								LCD_send_char(DEGREES_SYMBOL); // print the symbol of degree
								LCD_send_char('C'); // print the C character
								LCD_move_cursor(1,11);//move the cursor to the place to write the entered temperature
								_delay_ms(200);//Halt the system for the given time in (ms)
							/******************************************************************************/
								key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
								_delay_ms(250);//to avoid the duplication of the pressed key

								if (timeout_flag == TRUE) //in case of the time is out before the user press a key
										{
											break;//break the loop that ask for temperature
										}
								if (key_pressed <'0' || key_pressed >'9')//show wrong input message if the user entered non numeric value
										{
											LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
											LCD_send_string("Wrong input");//print error message
											_delay_ms(500);//Halt the system for the given time in (ms)
											continue;//return to #while (temperature==0)# line 672
										}
								else//if the value is valid
									{
										LCD_send_char(key_pressed);//print the value on the lcd
										temp_tens = key_pressed-ASCII_ZERO;//save the entered value
										key_pressed = NOT_PRESSED;//set the key pressed to the default value
									}
									/*******************************************************************************/
									key_pressed = u8GetKeyPressed(login_mode);//wait for the user till key is pressed or the time is out
									_delay_ms(250);//to avoid the duplication of the pressed key
												
									if (timeout_flag == TRUE)//if the user session is timeout
										{
											break;//break the loop that ask for temperature
										}
									if ((key_pressed <'0' || key_pressed >'9'))//show wrong input message if the user entered non numeric value
										{
											LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
											LCD_send_string("Wrong input");//print error message
											_delay_ms(500);//Halt the system for the given time in (ms)
											continue;//repeat the loop that ask for the temperature
										}
									else//if the value is valid
										{
											LCD_send_char(key_pressed);//print the value on the lcd
											temp_ones = key_pressed-ASCII_ZERO;//save the entered value
											key_pressed = NOT_PRESSED;//set the key pressed to the default value
										}
									temperature = temp_tens*10 + temp_ones;//set the value of the temperature from the given separated values
									SPI_MasterTransmit_Data(SET_TEMPERATURE);//Send the code of set temperature
									_delay_ms(200);//Halt the system to prevent write collision
									SPI_MasterTransmit_Data(temperature);//send the entered temperature
									LCD_clear_screen();//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
									LCD_send_string("Temperature Sent");//show the message
									_delay_ms(500);//Halt the system for the given time in (ms)
										}
									show_menu = AIRCONDITIONING_MENU;//Set the next menu to be shown to air conditioning menu
									break;//break from switch	
						}//End of the switch
					}//End of while that repeats the menu after each successful action till session timeout
	}//end of while(1)
}//end of main
ISR(TIMER0_COMP_vect)
{
	session_counter++;//increase the indicator of session time for every tick
}