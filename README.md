# TheWall
The wall from Stranger Things using an addressable LED strip. 

Parts:
- Adafruit NeoPixel Digital RGB LED Strip
- Adafruit Feather M0 Bluefruit LE
- 3-pin JST SM Plug + Receptacle Cable Set
- 470 Ohm resistor (Anything in the neighbourhood should do)
- 3.3 LiPo Battery with 2-pin JST connector

# 1. Hardware Assembly 
- Solder the 3 pin JST connector to GND, VBAT, and a GPIO (I used pin 6).  Connection to the GPIO should be through the 470 Ohm resistor.
- Solder matching 3 pin JST corresponding connector to the neopixel strip.  The signalling on the strips is directional, make sure connector is put on the input end.  Look for "DIN".
- Plug battery and USB into the Feather M0.

# 2. Code
- Download arduino IDE and install support for "Adafruit SAMD Boards" and libraries for "Adafruit NeoPixel by Adafruit"
- Upload this sketch.
- "charmap" may need to be altered depending of physical configuration of the LED strip.

# 3. Running
- This sketch works with the Adafruit Bluefruit LE Connect app.  Connect and send strings over UART to see them blinked out on the LED strip.

See Adafruit Guides for Details:
https://learn.adafruit.com/adafruit-neopixel-uberguide/
https://learn.adafruit.com/adafruit-feather-m0-bluefruit-le/

![alt tag](https://raw.githubusercontent.com/YuchenLuo/TheWall/master/pictures/IMG_20161104_033112.jpg)
