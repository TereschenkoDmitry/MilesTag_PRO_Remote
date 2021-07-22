//console micro programm

void console() {

keypad.begin(*keymap2);

bool exitconsole = true;


console_menu();

while (exitconsole) {


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
    Serial.println(buttonIndex);


  if (buttonIndex == 0) {
     Serial.println("Send");
          //sendSony(0x8302E8, 24);
          stringToSend +="*";
          char commandStr[stringToSend.length()+1];
          stringToSend.toCharArray(commandStr, stringToSend.length()+1);
          char delim[]= "*";
          char *ptr = strtok(commandStr, delim);
          while(ptr != NULL)
              {
                unsigned long cmd = strtol(ptr, NULL, 16);
              sendSony(cmd, 24);
              delay(pause_in_ms);
              //Serial.println(cmd, HEX);
              ptr = strtok(NULL, delim);
              }
          
          //if (stringToSend.length() <= 6) stringToSend +="*";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Ok");
          //Serial.println(stringToSend);
          
          delay(500);
          console_menu();
    }
    
if (buttonIndex == 3) {  
    //Serial.println("exit");
    exitconsole = false; 
    }
         
if (buttonIndex == 1) {
    //Serial.println("pause");
    stringToSend +="*";
    lcd.setCursor(cursor_pos,0);
    lcd.print("*");
    cursor_pos++;
    lcd.setCursor(cursor_pos,0);
    lcd.print("_");
    }


buttonPressed = false;
    
  }


char key = keypad.getKey();

  
}



//end of console  
lcd.clear();
keypad.begin(*keymap1);
consoleEnable = false;
menuEnable = true;
menuUpdate = false;
}

//-----console helpers----
void console_menu() {
cursor_pos = 0;
stringToSend = "";

lcd.clear();
lcd.setCursor(0,0);
lcd.print("_");

lcd.setCursor(1,1);
lcd.print("B");

lcd.setCursor(5,1);
lcd.print("<");

lcd.setCursor(10,1);
lcd.print("*");

lcd.setCursor(14,1);
lcd.print("E");


  
}
