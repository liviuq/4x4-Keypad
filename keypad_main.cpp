#include <Arduino.h>
#include <Keypad.h>
#include "keypad_uno.h"

const byte rows = 4;               //4 rows
const byte cols = 4;               //4 columns
//map the keypad's layout:
char hexaKeys[rows][cols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
byte rowPins[rows] = {2, 3, 4, 5}; //Arduino's digital pins for the rows
byte colPins[cols] = {6, 7, 8, 9}; //Arduino's digital pins for the columns

char password[] = "2648C789C";     //Master password. Change it to your needs
const short passwordlen = 9;       //Lenght of the master password to compare with the len. of the entered password
char entered[15];

char key;                          //It is going to hold the current key press
short i,j;
bool ok;                           //ok == true -> Enter; ok == false -> Retype the password

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, rows, cols);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Greetings();                              //What you see on the display outside

  key = customKeypad.waitForKey();          //Waits for the user to press the '*' to start entereing the password

  if( customKeypad.isPressed('*'))          //Checks if user started to enter password
  {
    while( '#' != key)                      //The entered password must end with a '#'
    {
      Serial.print("Enter the character nr ");
      Serial.println(i + 1);
      key = customKeypad.waitForKey();      //Program waits for the user to input another character
      entered[i++] = key;                   //Adding the pressed key to the entered password array
    }
    entered[i - 1 ] = '\0';                 //Removes the '#' from the end of the entered password

    if( (i - 1) == passwordlen)             //Checks if the lengts are equal
    {                                       //If not, it will promt you to enter again the password
      for( j = 0; j < ( i - 1); j++)
      {
        if( entered[j] == password[j])      //Actual checking of the passwords
        {
          ok = true;
        }
        else
        {
          ok = false;                       //If a single character doesn't matches, it breaks
          break;
        }
      }
    }
    else
    {
      ok = false;
    }

    if( ok == true)
    {
      Serial.println("Come in");
      delay(500);
    }
    else
    {
      Serial.println("Try again");
    } 
  }
  else
  {
    Serial.println("Press '*' to begin auth");
    delay(50);
  }

  if( i > 0)                                  //If the user entered something, clear the enteres string
  {
    for( i=0; entered[i] != '\0' ; i++)       
    { 
      entered[i] = '\0';                      //Overwrites all the cells with the string terminator
    }
    i = 0;                                    //resets the position to 0 to overwrite if incorrect password entered
  }
  delay(1000);
  
}