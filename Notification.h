/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <String.h>
#include <SupportDefs.h>

class Notification {
public:
			Notification(BString const & title, BString const & text,
				BString const & windowIcon, int32 timeout);
	
	void	Notify();
private:
	BString	fTitle;
	BString	fText;
	BString	fWindowIcon;
	int32	fTimeout;
};

#endif // NOTIFICATION_H
