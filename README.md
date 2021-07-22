# MilesTag_PRO_Remote
Pro remote for lasertag devices (based on MILES protocol)

(По-русски будет ниже)

This device is based on Arduino Nano.
There are 3 main modes of operation:
- prebuilt commands (i.e. New game, Change Team, Full Ammo and so on)
- command line mode - for testing commands
- preset changer - this special mode made for particular Hardware - Lasewar 8-9th gen
In Commandline mode and prebuilt commands you can use several commands divided by * sign

To make this project you need:
- Arduino Nano
- Keypad 4x4
- I2C Led 16x2
- I2C port extender based on PCF8574
- IR Led TSAL6100
- PNP transistor (BC556/BC557)
- Resistors 
- 4 buttons
- 1 switch button 



Русская секция

Устройство сделано на Arduino Nano
3 режима работы - пульт, консоль и выбор персонажей (для поколения 8-9 LW) 
В основное меню можно прошить последовательное исполнение команд (разделять команды *)
То же самое можно сделать в консоли.
