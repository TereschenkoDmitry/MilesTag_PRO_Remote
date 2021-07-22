//-----------------------------------
// Miles Tag remote 
// ver 1.0 12/06/2021
//
// Published under Apache License 2.0
//
//----------------------------------
#include <Keypad_I2C.h> //https://github.com/joeyoung/arduino_keypads.git
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>  //https://github.com/thomasfredericks/Bounce2
#include <ctype.h>

#define lcd_addr 0x3F
#define keypad_addr 0x20
#define NUM_BUTTONS 4
#define pause_in_ms 1000 

#define MAX_MENU_ITEMS 9

const byte numRows= 4;
const byte numCols= 4;


const uint8_t BUTTON_PINS[NUM_BUTTONS] = {4,5,6,7};

int buttonIndex;
bool buttonPressed = false;
bool consoleEnable = false;
bool presetEnable = false;
bool menuEnable = true;
bool menuUpdate = false;

int menuItem = 0;
int menuItemCurrent = 0;
int menuItemMax = MAX_MENU_ITEMS;

int cursor_pos = 0;
String stringToSend = "";

char keymap1[numRows][numCols]= { 
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'} };

char keymap2[numRows][numCols]= { 
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'F','0','E','D'} };

char keymap3[numRows][numCols]= { 
{'1','2','3','*'},
{'4','5','6','*'},
{'7','8','9','*'},
{'*','0','*','*'} };

byte rowPins[numRows] = {7,6,5,4}; //Rows 0 to 3

byte colPins[numCols]= {3,2,1,0}; //Columns 0 to 3

String menuName [MAX_MENU_ITEMS] = {"New game","Full ammo","2x life","Red team","Blue team","Yellow team","Green team","Zombie","Wifi On/Off"};
String menuCommand[MAX_MENU_ITEMS] = {"8302E8*","8306E8*","8310E8*","A900E8*","A901E8*","A902E8*","A903E8*","A808E8*","83FAE8*"};  // you can send multiple commands - divide them by * sign

//initializes an instance of the Keypad class

boolean alpha = true;   // Start with the basic keypad.
char* keypadMap = (alpha == true) ? makeKeymap(keymap1) : makeKeymap(keymap2);


//create a new Keypad
Keypad_I2C keypad(keypadMap, rowPins, colPins, sizeof(rowPins), sizeof(colPins), keypad_addr);

LiquidCrystal_I2C lcd(lcd_addr,16,2);  // 
Bounce * buttons = new Bounce[NUM_BUTTONS];

String versionNumber = "ver-1.0";
String versionDate = "12/06/2021";

void setup() {
Wire.begin();
keypad.begin();
keypad.port_write( 0xff );
keypad.addEventListener(keypadEvent);                                      // Add an event listener.
keypad.setHoldTime(500);

Serial.begin(9600);

for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(25);              // interval in ms
  }

lcd.init();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print(versionNumber);
lcd.setCursor(0,1);
lcd.print(versionDate);
delay(1000);
lcd.clear();

}

//If key is pressed, this key is stored in 'keypressed' variable //If key is not equal to 'NO_KEY', then this key is printed out //if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process

void loop() {
  char key = keypad.getKey();
  if (consoleEnable) console();
  if (presetEnable) presets();
  if (menuEnable) {
    if (menuUpdate == false) mainMenu();
    
      for (int i = 0; i < NUM_BUTTONS; i++)  {
        // Update the Bounce instance :
         buttons[i].update();
        // If it fell, flag the need to toggle the LED
          if ( buttons[i].fell() ) {
             buttonIndex = i;
            buttonPressed = true;
          }
      }
    if (buttonPressed) {
      
          if (buttonIndex == 0) {
     //Serial.println("Send");
     stringToSend = menuCommand[menuItem];
          char commandStr[stringToSend.length()+1];
          stringToSend.toCharArray(commandStr, stringToSend.length()+1);
          char delim[]= "*";
          char *ptr = strtok(commandStr, delim);
          while(ptr != NULL)
              {
                unsigned long cmd = strtol(ptr, NULL, 16);
              sendSony(cmd, 24);
              if (sizeof(delim) > 0 ) delay(pause_in_ms);
              //Serial.println(cmd, HEX);
              ptr = strtok(NULL, delim);
              }
     
          }
     if (buttonIndex == 1) {
      //down     
      //Serial.println("Down");
      menuItem++;
      if (menuItem > MAX_MENU_ITEMS - 1) menuItem = 0;
      menuUpdate = false;
     }
     if (buttonIndex == 2) {
      //up     
      //Serial.println("Up");
      menuItem--;
      if (menuItem < 0) menuItem = MAX_MENU_ITEMS - 1;
      menuUpdate = false;
      
     }
     
buttonPressed = false;
    }
  } // end of menu
}

//-----------helpers

void mainMenu () {
  cursor_pos = 0;
stringToSend = "";

lcd.clear();
lcd.setCursor(0,0);
lcd.print(menuName[menuItem]);

lcd.setCursor(5,1);
lcd.print("^");

lcd.setCursor(10,1);
lcd.print("v");

lcd.setCursor(14,1);
lcd.print("E");
menuUpdate = true;

}


// Take care of some special events.
void keypadEvent(KeypadEvent key) {
   // static char virtKey = NO_KEY;      // Stores the last virtual key press. (Alpha keys only)
   // static char physKey = NO_KEY;      // Stores the last physical key press. (Alpha keys only)
    static char buildStr[12];
    static byte buildCount;
    static byte pressCount;

    switch (keypad.getState())
    {
    case PRESSED:

                if (consoleEnable) {
                  lcd.setCursor(cursor_pos,0);
                  lcd.print(key);
                  stringToSend += key;
                  cursor_pos++;
                  lcd.setCursor(cursor_pos,0);
                  lcd.print("_");
                } else 
                if (presetEnable) {
                  
                  if (key != '*') {
                    //Serial.print(key);
                    if (stringToSend.length()>1) {
                      cursor_pos = 9;
                      stringToSend = "";
                      lcd.setCursor(11,0);
                      lcd.print(" ");
                    }
                  lcd.setCursor(cursor_pos,0);
                  lcd.print(key);
                  
                  stringToSend += key;
                  cursor_pos++;
                  lcd.setCursor(cursor_pos,0);
                  lcd.print("_");
                    
                  }
                } else {
                lcd.setCursor(0,0);
                lcd.print(key);
                }
               
                if ((key == 'A') && (consoleEnable == false)) {consoleEnable = true; menuEnable = false;} //enable console
                if ((key == 'B') && (presetEnable == false)) {presetEnable = true; menuEnable = false;}//enable preset menu

        break;

    case HOLD:/*
        if (key == '#')  {                   // Toggle between keymaps.
            if (alpha == true)  {            // We are currently using a keymap with letters
                keypad.begin(*numberKeys);   // and want to change to numbers.
                alpha = false;
                digitalWrite(ledPin, LOW);
            }
            else  {                          // Or, we are currently using a keymap with numbers
                keypad.begin(*alphaKeys);    // and want to change to letters.
                alpha = true;
            }
        }
        else  {                             // Some key other than '#' was pressed.
            buildStr[buildCount++] = (isalpha(key)) ? virtKey : key;
            buildStr[buildCount] = '\0';
            Serial.println();
            Serial.println(buildStr);
        }
        */
        break;

    case RELEASED:
        //if (buildCount >= sizeof(buildStr))  buildCount = 0;    // Our string is full. Start fresh.
        break;

    }  // end switch-case
}  // end keypad events
