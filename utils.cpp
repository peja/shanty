/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "utils.h"

#include <Screen.h>
#include <String.h>

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
    
    if (hex.Length() != 7 || hex[0] != '#')
    	return color;
            	
    hex.CopyInto(red, 1, 2);
    hex.CopyInto(green, 3, 2);
    hex.CopyInto(blue, 5, 2);
            	
    color.red = strtol(red.String(), NULL, 16);
    color.green = strtol(green.String(), NULL, 16);
    color.blue = strtol(blue.String(), NULL, 16);
    color.alpha = 255;
    
    return color;
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

