/*
Header file for TrinketHidComboC.c

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

#ifndef _TRINKETHIDCOMBOC_H_
#define _TRINKETHIDCOMBOC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define DATA_SIZE 8

#define REPID_MOUSE 1
#define REPID_KEYBOARD 2
#define REPID_MMKEY 3
#define REPID_SYSCTRLKEY 4
#define REPSIZE_MOUSE 4
#define REPSIZE_KEYBOARD 8
#define REPSIZE_MMKEY 3
#define REPSIZE_SYSCTRLKEY 2

#define REPID_THROTTLE 9
#define REPSIZE_THROTTLE 2

    extern char usb_hasCommed;
    extern uint8_t report_buffer[DATA_SIZE];
    extern uint8_t idle_rate;
    extern uint8_t protocol_version;

    void usbBegin();
    void usbPollWrapper();
    void usbReportSend(uint8_t len);
    void clearReportBuffer();

#ifdef __cplusplus
}
#endif

#endif