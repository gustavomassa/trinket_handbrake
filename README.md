## Trinket Analog Handbrake

### Features
- Using PlatformIO with vscode
- Created a TrinketJoystick lib based on the Adafruit HID Combo lib, it uses bit-bang USB technique
- Joystick logic is based on the ArduinoJoystickLibrary created by Matthew Heironimus [https://github.com/MHeironimus/ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary)
- Throttling info is working fine, receiving data from 10K potentiometer using 10 bit ADC

### Structe
- main folder contains the .ino file
- hardware folder is the Arduino's hardware folder
- libraries folder is the Arduino's libraries folder

### Board config
- Board: Adafruit Trinket (ATtiny85 @ 8MHZ)(Adafruit AVR Boards)
- Programmer: USBtinyISP
- Low speed USB is only able to accept data packets 8 bytes in length or less

### TODO
- Add a complete gamepad structure on the hid report descriptor, so the games can recognized the gamepad on the settings

### Adafruit docs about Trinket
- [https://www.adafruit.com/product/1500](https://www.adafruit.com/product/1500)
- [https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide](https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide)
- [https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide](https://learn.adafruit.com/adafruit-arduino-ide-setup/arduino-1-dot-6-x-ide)
- [https://learn.adafruit.com/introducing-trinket/16mhz-vs-8mhz-clock](https://learn.adafruit.com/introducing-trinket/16mhz-vs-8mhz-clock)
- [https://learn.adafruit.com/trinket-usb-keyboard/code](https://learn.adafruit.com/trinket-usb-keyboard/code)
- [https://learn.adafruit.com/trinket-usb-volume-knob/code](https://learn.adafruit.com/trinket-usb-volume-knob/code)

### Adafruit repos for Trinket
- [https://github.com/adafruit/Adafruit_Arduino_Boards](https://github.com/adafruit/Adafruit_Arduino_Boards/)
- [https://github.com/adafruit/Adafruit-Trinket-USB/](https://github.com/adafruit/Adafruit-Trinket-USB/)
- [https://github.com/adafruit/Adafruit_Windows_Drivers/releases/tag/2.5.0.0](https://github.com/adafruit/Adafruit_Windows_Drivers/releases/tag/2.5.0.0)

### USB docs
- [https://www.usb.org/hid](https://www.usb.org/hid)
- [http://vusb.wikidot.com/driver-api](http://vusb.wikidot.com/driver-api)
- [https://www.usb.org/sites/default/files/hid1_11.pd](https://www.usb.org/sites/default/files/hid1_11.pdf)

### DIView
- DIView is the software used for testing the USB data and calibration [https://heusinkveld.com/download-diview/?q=%2Fdownload-diview%2F&v=19d3326f3137](https://heusinkveld.com/download-diview/?q=%2Fdownload-diview%2F&v=19d3326f3137)
