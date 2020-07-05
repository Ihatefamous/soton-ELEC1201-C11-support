/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/about/licenses/
 */
 
#include <util/delay.h>
#include "ili934x.h"

void init_display_controller()
{
	uint16_t x, y;
	RESET_lo();
	_delay_ms(100);
	RESET_hi();
	_delay_ms(100);
	RS_hi();
	WR_hi();
	RD_hi(); 
	CS_lo();
	BLC_lo();
	VSYNC_hi();
	write_cmd(DISPLAY_OFF);
	write_cmd(SLEEP_OUT);
	_delay_ms(60);
	write_cmd_data(INTERNAL_IC_SETTING,			 1, "\x01");
	write_cmd_data(POWER_CONTROL_1,				 2, "\x26\x08");
    write_cmd_data(POWER_CONTROL_2,				 1, "\x10");
    write_cmd_data(VCOM_CONTROL_1,				 2, "\x35\x3E");
    write_cmd_data(MEMORY_ACCESS_CONTROL,		 1, "\x48");
    write_cmd_data(RGB_INTERFACE_SIGNAL_CONTROL, 1, "\x4A");  // Set the DE/Hsync/Vsync/Dotclk polarity
    write_cmd_data(FRAME_CONTROL_IN_NORMAL_MODE, 2, "\x00\x1B"); // 70Hz
    write_cmd_data(DISPLAY_FUNCTION_CONTROL,	 4, "\x0A\x82\x27\x00");
    write_cmd_data(VCOM_CONTROL_2,			     1, "\xB5");
    write_cmd_data(INTERFACE_CONTROL,			 3, "\x01\x00\x00"); // System interface
    write_cmd_data(GAMMA_DISABLE,				 1, "\x00"); 
    write_cmd_data(GAMMA_SET,					 1, "\x01"); // Select Gamma curve 1
    write_cmd_data(PIXEL_FORMAT_SET,			 1, "\x55"); // 0x66 - 18bit /pixel,  0x55 - 16bit/pixel
    write_cmd_data(POSITIVE_GAMMA_CORRECTION,	15, "\x1F\x1A\x18\x0A\x0F\x06\x45\x87\x32\x0A\x07\x02\x07\x05\x00");
    write_cmd_data(NEGATIVE_GAMMA_CORRECTION,	15, "\x00\x25\x27\x05\x10\x09\x3A\x78\x4D\x05\x18\x0D\x38\x3A\x1F");
    write_cmd_data(COLUMN_ADDRESS_SET,			 4, "\x00\x00\x00\xEF");
    write_cmd_data(PAGE_ADDRESS_SET,			 4, "\x00\x00\x01\x3F");
    write_cmd(TEARING_EFFECT_LINE_OFF);
    write_cmd_data(DISPLAY_INVERSION_CONTROL,	 1, "\x00");
    write_cmd_data(ENTRY_MODE_SET,				 1, "\x07");
    /* Clear display */
	write_cmd(MEMORY_WRITE);
	for(x=0; x<240; x++)
		for(y=0; y<320; y++)
			write_data16(0x0000);
	write_cmd(DISPLAY_ON);
	_delay_ms(50);
	BLC_hi();
};

