/*
This is the part of the TrinketHidCombo code that is exposed to the user
See the header file for more comments on the functions

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

#include "cmdline_defs.h"
#include "TrinketJoystick.h"
#include "TrinketJoystickC.h"

#include <stdint.h>
#include <avr/interrupt.h>

// create an instance that the user can use
Trinket_Joystick TrinketJoystick;

// empty constructor
Trinket_Joystick::Trinket_Joystick()
{
	initState();
}

void Trinket_Joystick::initState()
{
	// Initalize State
	xAxis = 0;
	yAxis = 0;
	zAxis = 0;
	xAxisRotation = 0;
	yAxisRotation = 0;
	zAxisRotation = 0;
	buttons = 0;
	throttle = 0;
	rudder = 0;
	hatSwitch[0] = -1;
	hatSwitch[1] = -1;
}

void Trinket_Joystick::sendState()
{
	//initReportBuffer();
	// ID
	//report_buffer[0] = JOYSTICK_REPORT_ID;

	uint32_t buttonTmp = buttons;

	// Split 32 bit button-state into 4 bytes
	report_buffer[0] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[2] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[3] = buttonTmp & 0xFF;

	report_buffer[4] = throttle;
	report_buffer[5] = rudder;

	/* 	uint32_t buttonTmp = buttons;

	// Split 32 bit button-state into 4 bytes
	report_buffer[0] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[2] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[3] = buttonTmp & 0xFF;

	report_buffer[4] = throttle;
	report_buffer[5] = rudder; */

	/* // Calculate hat-switch values
	uint8_t convertedHatSwitch[2];
	for (int hatSwitchIndex = 0; hatSwitchIndex < 2; hatSwitchIndex++)
	{
		if (hatSwitch[hatSwitchIndex] < 0)
		{
			convertedHatSwitch[hatSwitchIndex] = 8;
		}
		else
		{
			convertedHatSwitch[hatSwitchIndex] = (hatSwitch[hatSwitchIndex] % 360) / 45;
		}
	}

	// Pack hat-switch states into a single byte
	report_buffer[6] = (convertedHatSwitch[1] << 4) | (0b00001111 & convertedHatSwitch[0]);

	report_buffer[7] = xAxis + 127;
	report_buffer[8] = yAxis + 127;
	report_buffer[9] = zAxis + 127;

	report_buffer[10] = (xAxisRotation % 360) * 0.708;
	report_buffer[11] = (yAxisRotation % 360) * 0.708;
	report_buffer[12] = (zAxisRotation % 360) * 0.708; */

	//usbReportSend();
}

// starts the USB driver, causes re-enumeration
void Trinket_Joystick::begin()
{
	usbBegin();
}

// this must be called at least once every 10ms
void Trinket_Joystick::poll()
{
	usbPollWrapper();
}

// checks if USB is connected, 0 if not connected
char Trinket_Joystick::isConnected()
{
	return usb_hasCommed;
}

void Trinket_Joystick::setThrottle(uint8_t value)
{
	/* 	throttle = value;
	sendState(); */

	/* 	report_buffer[0] = REPID_THROTTLE;
	// 1 Byte Buttons 8
	//report_buffer[1] = 0x00;
	// 1 byte Throttle
	report_buffer[1] = value;
	// 1 byte steering
	report_buffer[2] = 0x00;
	// 1 byte Two Hat switches (8 Positions)
	report_buffer[3] = 0x00; */

	report_buffer[0] = REPID_THROTTLE;

	// Split 16 bit button-state into 2 bytes
	uint16_t buttonTmp = buttons;
	report_buffer[1] = buttonTmp & 0xFF;
	buttonTmp >>= 8;
	report_buffer[2] = buttonTmp & 0xFF;

	// 1 byte Throttle
	report_buffer[3] = value;
	// 1 byte steering
	report_buffer[4] = 0x00;

	// 1 byte Two Hat switches (8 Positions)
	report_buffer[5] = 0x00;

	usbReportSend(REPSIZE_THROTTLE);
}

// makes a mouse movement
void Trinket_Joystick::mouseMove(signed char x, signed char y, uint8_t buttonMask)
{
	signed char *signed_ptr = (signed char *)report_buffer; // this converts signed to unsigned

	// format the report structure
	signed_ptr[2] = x;
	signed_ptr[3] = y;
	report_buffer[1] = buttonMask;
	report_buffer[0] = REPID_MOUSE;

	usbReportSend(REPSIZE_MOUSE);
}

void Trinket_Joystick::pressSystemCtrlKey(uint8_t key)
{
	report_buffer[0] = REPID_SYSCTRLKEY;
	report_buffer[1] = key;
	usbReportSend(REPSIZE_SYSCTRLKEY);
	// immediate release
	report_buffer[0] = REPID_SYSCTRLKEY;
	report_buffer[1] = key;
	usbReportSend(REPSIZE_SYSCTRLKEY);
}