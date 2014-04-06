/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "Notification.h"

#include <Application.h>
#include <Notification.h>
#include <Roster.h>

#include <new>
#include <stdexcept>

#include "utils.h"

const BString kNotificationTitle = "Notification";
const BString kNotificationText = "Notification";
const BString kNotificationIcon = "info";


Notification::Notification(BString const & title, BString const & text,
	BString const & windowIcon, int32 timeout)
{
	if (title.Length() == 0)
		fTitle = kNotificationTitle;
		
	fText = (text.Length() > 0 ? text : kNotificationText);
	
	fWindowIcon = (windowIcon.Length() > 0 ? windowIcon : kNotificationIcon);
	
	fTimeout = timeout;
}


void
Notification::Notify()
{
	notification_type type;
	if (fWindowIcon == "warning" || fWindowIcon == "question")
		type = B_IMPORTANT_NOTIFICATION;
	else if (fWindowIcon == "error")
		type = B_ERROR_NOTIFICATION;
	else
		type = B_INFORMATION_NOTIFICATION;
		
	fIcon = new(std::nothrow) BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);
	if (fIcon == NULL)
		throw std::runtime_error("BBitmap: No memory for bitmap");
	
	get_vector_icon(fWindowIcon, fIcon);
	
	BNotification notify(type);
	notify.SetTitle(fTitle);
	notify.SetContent(fText);
	notify.SetIcon(fIcon);
	
	notify.Send(fTimeout * 1000000);
}
