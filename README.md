#AVR USB RGB LED Controller

While working on an update for my <a href="http://blog.zakkemble.co.uk/cpu-usage-leds/" title="CPU Usage LEDs">CPU Usage LEDs project</a>, I thought why not just make it into a universal RGB LED controller? The CPU Usage LEDs controller took a value between 0 and 255, worked out what colour it should be and then fade to that colour. This was very limiting; changing what colours it used and how it fades required a firmware update. With this universal RGB LED controller the host software does all the work and the controller is simply told what brightness the red, green and blue LEDs should be.

To make it as easy as possible to interface with the controller I created a library which deals with all the LibUSB stuff.

The library also has support for reading and writing to the EEPROM of the controller.


##Documentation
<a href="http://zkemble.github.io/AVR-USB-RGB-LED-Controller/">Doxygen pages</a>

##Getting it working
###Microcontroller
- The firmware in ./firmware/usbLedController/ is written for ATtiny45 and ATtiny85.
- The provided .hex and .eep files are for ATtiny85.
- Run this to upload to the chip:

		avrdude.exe -c usbasp -p t85 -U flash:w:usbLedController.hex -U eeprom:w:usbLedController.eep -U lfuse:w:0xf1:m -U hfuse:w:0xdc:m -U efuse:w:0xff:m

###Windows
####To run the example programs:
- You will need to have the LibUSB-win32 driver installed (./driver/).
- Each program needs a copy of ./lib/win/librgbledctrl.dll in the same directoy, or you can just copy the .dll to your Windows folder (usually C:/Windows).

####To compile the example programs and library:
#####For C and C++:
- Copy ./lib/win/librgbledctrl.a to your compilers lib directory.
- Copy ./include/rgbledctrl.h to your compilers include directory.
- You will also need LibUSB-win32 libusb.a and lusb0_usb.h files in the same places.
- Eclipse CDT with MinGW were used to compile.

#####For C#:
- Add ./include/Rgbledctrl.cs to your project.
- Add 'using LibRgbLedCtrl;' with the rest of the 'using' lines.

###Linux
####To run the example programs:
- Copy ./other/99-RGBLEDController.rules to /etc/udev/rules.d/ so you don't have to run the programs as root.
- Copy ./lib/unix/librgbledctrl.so to /usr/lib/
- The binaries were compiled on Debian 7.1 x64.

####To compile the example programs and library:
- Copy ./lib/unix/librgbledctrl.a to /usr/lib/
- Copy ./include/rgbledctrl.h to /usr/include/
- Run makefiles

--------

./driver/ contains parts of libusb-win32 available at http://sourceforge.net/apps/trac/libusb-win32/wiki

./firmware/ contains the V-USB library available at http://www.obdev.at/products/vusb/index.html

Third party contents are copyrighted by their respective authors.

My productions are published under GNU GPL v3 (see License.txt).

--------

    http://blog.zakkemble.co.uk/avr-usb-rgb-led-controller/

--------

Zak Kemble

contact@zakkemble.co.uk