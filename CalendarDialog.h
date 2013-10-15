/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef	CALENDAR_DIALOG_H
#define	CALENDAR_DIALOG_H

#include "Dialog.h"


class MonthWindowView;

class CalendarDialog : public Dialog
{
public:
						CalendarDialog(BString const & title, float width, float height,
							BString const & text, int day, int month, int year, BString const & format,
							BString const & windowIcon);
    
    void				MessageReceived(BMessage* msg);
    void				CreateViews();
    void				InitControls();
    void				SetDate(int day, int month, int year, bool initial = false);
    
private:
    BString				fText;
    struct tm*			timeinfo;
    int					fDay;
    int					fMonth;
    int					fYear;
    BString				fFormat;
    
    MonthWindowView*	fmonthView;
};

#endif	// CALENDAR_DIALOG_H
