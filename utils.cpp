/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "utils.h"

#include <Screen.h>
#include <String.h>
#include <SupportDefs.h>

#include <math.h>
#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void
center_on_screen(BWindow* window)
{
    BRect screenRect = BScreen(window).Frame();
    BRect windowRect = window->Frame();

    window->MoveTo(
        (screenRect.Width()  - windowRect.Width())  / 2,
        (screenRect.Height() - windowRect.Height()) / 2);
}

//
// Convert string containing color in hex ("#xxxxxx")
// to rgb_color structure.
rgb_color
hex_string_to_rgb_color(BString const &hex)
{
	BString red, green, blue;
    rgb_color color = { 0, 0, 0, 0 };
    
    if (hex.Length() != 6)
    	return color;
            	
    hex.CopyInto(red, 0, 2);
    hex.CopyInto(green, 2, 2);
    hex.CopyInto(blue, 4, 2);
            	
    color.red = strtol(red.String(), NULL, 16);
    color.green = strtol(green.String(), NULL, 16);
    color.blue = strtol(blue.String(), NULL, 16);
    color.alpha = 255;
    
    return color;
}

hsv_color
rgb_to_hsv_color(rgb_color const &color)
{
	uint16 temp;
	hsv_color result;
	
	temp = min_c(min_c(color.red, color.green), color.blue);
	
	result.value = max_c(max_c(color.red, color.green), color.blue);
	
	// Calculate hue
	if (temp == result.value)
		result.hue = 0;
	else {
		if (color.red == result.value)
			result.hue = 0 + ((color.green - color.blue) * 60 / 
				(uint16) (result.value - temp));
		if (color.green == result.value)
			result.hue = 120 + ((color.blue - color.red) * 60 /
				(uint16) (result.value - temp));
		if (color.blue == result.value)
			result.hue = 240 + ((color.red - color.green) * 60 /
				(uint16) (result.value - temp));
	}
	
	if (result.hue < 0)
		result.hue = result.hue + 360;
	
	// Calculate saturation
	if (result.value == 0)
		result.saturation = 0;
	else
		result.saturation = (result.value - temp) * 100 / (uint16) result.value;
	
	// Calculate value
	result.value = (100 * result.value) / 255;
	
	return result;
}

rgb_color
hsv_to_rgb_color(hsv_color const &color)
{
	rgb_color result;
	
	if (color.value == 0) {
		result.red = 0;
		result.green = 0;
		result.blue = 0;
		return result;
	} else if (color.hue == 0 && color.saturation == 0) {
		result.red = color.value * 255 / 100;
		result.green = color.value * 255 / 100;
		result.blue = color.value * 255 / 100;
	} else {
		uint16 region, remainder, p, q, t;
		region = color.hue / 43;
		remainder = (color.hue - (region * 43)) * 6;
		
		p = (color.value * (255 - color.saturation)) >> 8;
		q = (color.value * (255 - ((color.saturation * remainder) >> 8))) >> 8;
		t = (color.value * (255 - ((color.saturation * (255 - remainder)) >> 8))) >> 8;
		
		switch (region)
		{
			case 0:
				result.red = color.value;
				result.green = t;
				result.blue = p;
				break;
			case 1:
				result.red = q;
				result.green = color.value;
				result.blue = p;
				break;
			case 2:
				result.red = p;
				result.green = color.value;
				result.blue = t;
				break;
			case 3:
				result.red = p;
				result.green = q;
				result.blue = color.value;
				break;
			case 4:
				result.red = t;
				result.green = p;
				result.blue = color.value;
				break;
			default:
				result.red = color.value;
				result.green = p;
				result.blue = q;
				break;
		}
	}
	
	return result;
}

//
// Read the input in a non-blocking way.
//
// Returns -1 when there is no more data to read, on error, and if
// the thread receives a message, which indicates that the reading must stop.
//
// Otherwise returns number of read bytes.
ssize_t
read_stdin_nonblocking(char* str, size_t size)
{
	static fd_set rfds, rfdsCopy;
	static struct timeval tv, tvCopy;

	// Read from stdin (fd 0).
	
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Wait up to a second.
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	
	for (;;) {
		// Quit if there is a message in the cache.
		if (has_data(find_thread(NULL)))
			break;
			
		// Make copies of structures, because 'select' modifies them.
		rfdsCopy = rfds;
		tvCopy = tv;
		
		if (select(1, &rfdsCopy, NULL, NULL, &tvCopy) == -1)
        	break;
        	
    	if (FD_ISSET(0, &rfds)) {
    		ssize_t n = read(0, str, size);
    		
    		if (n <= 0) break;
    		
    		return n;
    	}
	}
	
	return -1;
}

