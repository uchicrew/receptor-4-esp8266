DIY Client Widget for NodeMCU v2 | Receptor-4
====

This is [Receptor-4](http://receptor-4.appspot.com) client program for NodeMCU v2.

![do-it-yourself](https://raw.githubusercontent.com/uchicrew/receptor-4-esp8266/master/image.jpg)


## Require components

* NodeMCU v2 - ESP8266 Development Board
* SSD1306 - 0.96inch 128x64 OLED I2C

Build your own device?
Please refer to [Guide](http://receptor-4.appspot.com/doc/diy-esp8266-widget-guide.en.html).


## How to build firmware

This is [PlatformIO](http://platformio.org/) based project.
Please refer to the document and install [PlatformIO Core](http://docs.platformio.org/en/latest/core.html).

This project build on espressif8266 platform.

    $ pio platform install espressif8266

There are two dependencies for this, ArduinoJson and ESP8266_SSD1306.

    $ pio lib install 64
    $ pio lib install 562

Configure wifi settings in `settings.h`.

    const char* SSID = "YOUR SSID";
    const char* PASSWORD = "YOUR PASSWORD";

Then build it and upload firmware.

    $ pio run --target upload


## Licence

MIT

## Author

UCHICREW
