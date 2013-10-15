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

    CalendarDialog(char* title, float width, float height,
                   char* text, int day, int month, int year, char* format, BString windowIcon);
                
    
    void MessageReceived(BMessage* msg);
    void CreateViews();
    void InitControls();
    void SetDate(int day, int month, int year, bool initial = false);
    
private:
    
    const char* fText;
    struct tm * timeinfo;
    int fDay, fMonth, fYear;
    const char* fFormat;
    
    MonthWindowView* fmonthView;
};

#endif	// CALENDAR_DIALOG_H
