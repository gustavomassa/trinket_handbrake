/*
This is the part of the TrinketHidCombo code that is exposed to the user

Copyright (c) 2013 Adafruit Industries
All rights reserved.

TrinketHidCombo is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

TrinketHidCombo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with TrinketHidCombo. If not, see
<http://www.gnu.org/licenses/>.
*/

#ifndef _TRINKETHIDCOMBO_H_
#define _TRINKETHIDCOMBO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

class Trinket_Joystick
{
private:
	int8_t xAxis;
	int8_t yAxis;
	int8_t zAxis;
	int16_t xAxisRotation;
	int16_t yAxisRotation;
	int16_t zAxisRotation;
	uint16_t buttons;
	uint8_t throttle;
	uint8_t rudder;
	int16_t hatSwitch[2];

	void initState();
	void sendState();

public:
	Trinket_Joystick(); // empty constructor, ignore me
	void begin();		// starts the USB driver, causes re-enumeration
	void poll();		// this (or "press" something) must be called at least once every 10ms
	char isConnected(); // checks if USB is connected, 0 if not connected

	void setThrottle(uint8_t value);

	void mouseMove(signed char x, signed char y, uint8_t buttonMask);
	void pressSystemCtrlKey(uint8_t key);
};

// an instance that the user can use
extern Trinket_Joystick TrinketJoystick;

#endif