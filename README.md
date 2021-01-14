## Trinket Analog Handbrake

### Adafruit docs about Trinket
- [](https://www.adafruit.com/product/1500)
- [](https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide)
- [](https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide)
- [](https://learn.adafruit.com/introducing-trinket/16mhz-vs-8mhz-clock)
- [](https://learn.adafruit.com/trinket-usb-keyboard/code)
- [](https://learn.adafruit.com/trinket-usb-volume-knob/code)

### Adafruit repos for Trinket
- [](https://github.com/adafruit/Adafruit_Arduino_Boards/)
- [](https://github.com/adafruit/Adafruit-Trinket-USB/)
- [](https://github.com/adafruit/Adafruit_Windows_Drivers/releases/tag/2.5.0.0)

### USB docs
- [](https://www.usb.org/hid)
- [](http://vusb.wikidot.com/driver-api)
- [](https://www.usb.org/sites/default/files/hid1_11.pdf)

### Board config
- Board: Adafruit Trinket (ATtiny85 @ 8MHZ)(Adafruit AVR Boards)
- Programmer: USBtinyISP
- Low speed USB is only able to accept data packets 8 bytes in length or less

### Features
- Based on the Adafruit HID Combo lib, it uses bit-bang USB technique
- Joystick based on the lib created by Matthew Heironimus for Arduino
- Throttling info is working fine, receiving data from 10K potentiometer using 10 bit ADC

### TODO
- Add a complete gamepad structure on the hid report descriptor, so the games can recognized the gamepad on the settings
