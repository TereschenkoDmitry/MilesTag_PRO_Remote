//preset micro program

void presets() {

keypad.begin(*keymap3);

bool exitpresets = true;

presets_menu();

while (exitpresets) {

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
    //Serial.println(buttonIndex);


  if (buttonIndex == 0) {
    //0xA8 [num of preset in hex] E8
    int i = stringToSend.toInt();
   String presetNumStr = String (stringToSend.toInt(), HEX);
   if (presetNumStr.length()==1) {presetNumStr = String ("0"+presetNumStr);}
   stringToSend = String("A8"+ presetNumStr + "E8*");
   //Serial.println(stringToSend);
          char commandStr[stringToSend.length()+1];
          stringToSend.toCharArray(commandStr, stringToSend.length()+1);
          char delim[]= "*";
          char *ptr = strtok(commandStr, delim);
          while(ptr != NULL)
              {
                unsigned long cmd = strtol(ptr, NULL, 16);
              sendSony(cmd, 24);
              delay(100);
              ptr = strtok(NULL, delim);
              }
          //delay(100);
          presets_menu();
    }
    
if (buttonIndex == 3) {  
    exitpresets = false; 
    }
         
if (buttonIndex == 1) {

    }


buttonPressed = false;
    
  }


char key = keypad.getKey();

  
}
//end of console  
lcd.clear();
keypad.begin(*keymap1);
presetEnable = false;
menuEnable = true;
menuUpdate = false;
}



//-----console helpers----
void presets_menu() {
cursor_pos = 9;
stringToSend = "";

lcd.clear();
lcd.setCursor(0,0);
lcd.print("Preset N: ");

lcd.setCursor(0,1);
lcd.print("Back");

lcd.setCursor(13,1);
lcd.print("Ent");
}
