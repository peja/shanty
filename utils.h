/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */
 
#ifndef	UTILS_H
#define	UTILS_H

#include <Window.h>

void		center_on_screen(BWindow* window);
rgb_color	hex_string_to_rgb_color(BString const &hex);
ssize_t		read_stdin_nonblocking(char* str, size_t size);

#endif	// UTILS_H
