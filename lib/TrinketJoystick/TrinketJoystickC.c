/*
This is the part of the TrinketHidCombo code that is usually written in C
I didn't want to mix it with the object oriented C++ code

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
#include "TrinketJoystickC.h"
#include "usbconfig.h"
#include "usbdrv/usbdrv.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>
#include <stdint.h>

// preprocessor macro for compile time assert, see http://www.pixelbeat.org/programming/gcc/static_assert.html
#define ct_assert(e) extern char(*ct_assert(void))[sizeof(char[1 - 2 * !(e)])]

uint8_t report_buffer[DATA_SIZE];
char usb_hasCommed = 0;
uint8_t idle_rate = 500 / 4;  // see HID1_11.pdf sect 7.2.4
uint8_t protocol_version = 0; // see HID1_11.pdf sect 7.2.6
loopDelay = 5;				  // 5ms

void usbBegin()
{
	cli();

	// run at full speed, because Trinket defaults to 8MHz for low voltage compatibility reasons
	clock_prescale_set(clock_div_1);

	// fake a disconnect to force the computer to re-enumerate
	PORTB &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT));
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();

	// start the USB driver
	usbInit();
	sei();
}

void usbPollWrapper()
{
	usbPoll();
}

void usbReportSend(uint8_t len)
{
	// perform usb background tasks until the report can be sent, then send it
	while (1)
	{
		usbPoll(); // this needs to be called at least once every 10 ms
		if (usbInterruptIsReady())
		{
			usbSetInterrupt((uint8_t *)report_buffer, len); // send
			break;

			// see http://vusb.wikidot.com/driver-api
		}
	}

	/* 	// packet size is 8 bytes max, send two packets
	// see http://vusb.wikidot.com/driver-api
	while (!usbInterruptIsReady())
		usbPoll();
	usbSetInterrupt((uint8_t *)report_buffer, 8);

	while (!usbInterruptIsReady())
		usbPoll();
	usbSetInterrupt((uint8_t *)report_buffer + 8, 2); */
}

// USB HID report descriptor for boot protocol keyboard
// see HID1_11.pdf appendix B section 1
// USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH is defined in usbconfig (should be 173)
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
	// Joystick 8
	0x05, 0x01,			  // USAGE_PAGE (Generic Desktop)
	0x09, 0x04,			  // USAGE (Joystick)
	0xA1, 0x01,			  // COLLECTION (Application)
	0x85, REPID_THROTTLE, //   REPORT_ID (3)

	// 16 Buttons 20
	0x05, 0x09, //   USAGE_PAGE (Button)
	0x19, 0x01, //   USAGE_MINIMUM (Button 1)
	0x29, 0x10, //   USAGE_MAXIMUM (Button 16)
	0x15, 0x00, //   LOGICAL_MINIMUM (0)
	0x25, 0x01, //   LOGICAL_MAXIMUM (1)
	0x75, 0x01, //   REPORT_SIZE (1)
	0x95, 0x10, //   REPORT_COUNT (16)
	0x55, 0x00, //   UNIT_EXPONENT (0)
	0x65, 0x00, //   UNIT (None)
	0x81, 0x02, //   INPUT (Data,Var,Abs)

	// Throttle + Steering 20
	0x05, 0x02,		  //   USAGE_PAGE (Simulation Controls)
	0x15, 0x00,		  //   LOGICAL_MINIMUM (0)
	0x26, 0xFF, 0x00, //   LOGICAL_MAXIMUM (255)
	0xA1, 0x00,		  //   COLLECTION (Physical)
	0x09, 0xBB,		  //     USAGE (Throttle)
	0x09, 0xBA,		  //     USAGE (Steering)
	0x75, 0x08,		  //     REPORT_SIZE (8)
	0x95, 0x02,		  //     REPORT_COUNT (2)
	0x81, 0x02,		  //     INPUT (Data,Var,Abs)
	0xC0,			  //   END_COLLECTION

	// Two Hat switches (8 Positions) 40
	0x05, 0x01,		  //   USAGE_PAGE (Generic Desktop)
	0x09, 0x39,		  //   USAGE (Hat switch)
	0x15, 0x00,		  //   LOGICAL_MINIMUM (0)
	0x25, 0x07,		  //   LOGICAL_MAXIMUM (7)
	0x35, 0x00,		  //   PHYSICAL_MINIMUM (0)
	0x46, 0x3B, 0x01, //   PHYSICAL_MAXIMUM (315)
	0x65, 0x14,		  //   UNIT (Eng Rot:Angular Pos)
	0x75, 0x04,		  //   REPORT_SIZE (4)
	0x95, 0x01,		  //   REPORT_COUNT (1)
	0x81, 0x02,		  //   INPUT (Data,Var,Abs)

	0x09, 0x39,		  //   USAGE (Hat switch)
	0x15, 0x00,		  //   LOGICAL_MINIMUM (0)
	0x25, 0x07,		  //   LOGICAL_MAXIMUM (7)
	0x35, 0x00,		  //   PHYSICAL_MINIMUM (0)
	0x46, 0x3B, 0x01, //   PHYSICAL_MAXIMUM (315)
	0x65, 0x14,		  //   UNIT (Eng Rot:Angular Pos)
	0x75, 0x04,		  //   REPORT_SIZE (4)
	0x95, 0x01,		  //   REPORT_COUNT (1)
	0x81, 0x02,		  //   INPUT (Data,Var,Abs)

	/* 	// X, Y Axis 24
	0x15, 0x00,		  //   LOGICAL_MINIMUM (0)
	0x26, 0xFF, 0x00, //   LOGICAL_MAXIMUM (255)
	0x75, 0x08,		  //   REPORT_SIZE (8)
	0x09, 0x01,		  //   USAGE (Pointer)
	0xA1, 0x00,		  //   COLLECTION (Physical)
	0x09, 0x30,		  //     USAGE (x)
	0x09, 0x31,		  //     USAGE (y)
	0x09, 0x33,		  //     USAGE (rx)
	0x09, 0x34,		  //     USAGE (ry)
	0x95, 0x04,		  //     REPORT_COUNT (4)
	0x81, 0x02,		  //     INPUT (Data,Var,Abs)
	0xC0,	 */
	//   END_COLLECTION

	0xC0 // END COLLECTION
};

// if the sizes don't match, compile fails here
ct_assert(sizeof(usbHidReportDescriptor) == USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH);

// see http://vusb.wikidot.com/driver-api
// constants are found in usbdrv.h
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usb_hasCommed = 1;

	// see HID1_11.pdf sect 7.2 and http://vusb.wikidot.com/driver-api
	usbRequest_t *rq = (void *)data;

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS)
		return 0; // ignore request if it's not a class specific request

	// see HID1_11.pdf sect 7.2
	switch (rq->bRequest)
	{
	case USBRQ_HID_GET_IDLE:
		usbMsgPtr = &idle_rate; // send data starting from this byte
		return 1;				// send 1 byte
	case USBRQ_HID_SET_IDLE:
		idle_rate = rq->wValue.bytes[1]; // read in idle rate
		return 0;						 // send nothing
	case USBRQ_HID_GET_PROTOCOL:
		usbMsgPtr = &protocol_version; // send data starting from this byte
		return 1;					   // send 1 byte
	case USBRQ_HID_SET_PROTOCOL:
		protocol_version = rq->wValue.bytes[1];
		return 0; // send nothing
	case USBRQ_HID_GET_REPORT:
		usbMsgPtr = (uint8_t *)&report_buffer; // send the report data
		clearReportBuffer();
		report_buffer[0] = rq->wValue.bytes[0]; // set report ID
		// determine the return data length based on which report ID was requested
		if (rq->wValue.bytes[0] == REPID_MOUSE)
			return REPSIZE_MOUSE;
		if (rq->wValue.bytes[0] == REPID_KEYBOARD)
			return REPSIZE_KEYBOARD;
		if (rq->wValue.bytes[0] == REPID_MMKEY)
			return REPSIZE_MMKEY;
		if (rq->wValue.bytes[0] == REPID_SYSCTRLKEY)
			return REPSIZE_SYSCTRLKEY;
		if (rq->wValue.bytes[0] == REPID_THROTTLE)
		{
			return REPSIZE_THROTTLE;
		}
		return DATA_SIZE; // default
	case USBRQ_HID_SET_REPORT:
		if (rq->wLength.word == 2) // check data is available
		{
			// 1st is the report byte, data is the 2nd byte.
			// We don't check report type (it can only be output or feature)
			// we never implemented "feature" reports so it can't be feature
			// so assume "output" reports
			// this means set LED status
			// since it's the only one in the descriptor
			return USB_NO_MSG; // send nothing but call usbFunctionWrite
		}
		else // no data or do not understand data, ignore
		{
			return 0; // send nothing
		}
	default:	  // do not understand data, ignore
		return 0; // send nothing
	}
}

void clearReportBuffer()
{
	for (uint8_t i = 0; i < DATA_SIZE; i++)
	{
		report_buffer[i] = 0;
	}
}

// see http://vusb.wikidot.com/driver-api
usbMsgLen_t usbFunctionWrite(uint8_t *data, uchar len)
{
	return 1; // 1 byte read
}

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny25__)
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */
// section copied from EasyLogger
/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
void calibrateOscillator(void)
{
	uchar step = 128;
	uchar trialValue = 0, optimumValue;
	int x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

	/* do a binary search: */
	do
	{
		OSCCAL = trialValue + step;
		x = usbMeasureFrameLength(); /* proportional to current real frequency */
		if (x < targetValue)		 /* frequency still too low */
			trialValue += step;
		step >>= 1;
	} while (step > 0);
	/* We have a precision of +/- 1 for optimum OSCCAL here */
	/* now do a neighborhood search for optimum value */
	optimumValue = trialValue;
	optimumDev = x; /* this is certainly far away from optimum */
	for (OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++)
	{
		x = usbMeasureFrameLength() - targetValue;
		if (x < 0)
			x = -x;
		if (x < optimumDev)
		{
			optimumDev = x;
			optimumValue = OSCCAL;
		}
	}
	OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/
#endif
