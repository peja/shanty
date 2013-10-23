/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */
 
#ifndef	UTILS_H
#define	UTILS_H

#include <Window.h>

struct hsv_color {
	uint16 hue;
	uint16 saturation;
	uint16 value;
};

void		center_on_screen(BWindow* window);
rgb_color	hex_string_to_rgb_color(BString const &hex);
hsv_color	rgb_to_hsv_color(rgb_color const &color);
rgb_color	hsv_to_rgb_color(hsv_color const &color);
ssize_t		read_stdin_nonblocking(char* str, size_t size);

#endif	// UTILS_H
