set PATH=C:\Users\werewolf\.vscode\HusarionTools\bin\;%PATH%
cd c:\Users\werewolf\Documents\GitHub\Delta_robot\Driver\Husarion_delta || exit 1
start /wait st-flash write myproject.bin 0x08010000 || exit 1
start st-util
arm-none-eabi-gdb %*