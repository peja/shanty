/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <support/String.h>
#include <stdio.h>
#include "CalendarDialog.h"

#define __LANG_ENGLISH
#include "CalendarControl/MonthWindowView.cpp"


extern char kOk[];
extern char kCancel[];

const BString kCalendarTitle = "Calendar selection";
const BString kCalendarText  = "Select a date from below.";


void
CalendarDialog::SetDate(int day, int month, int year, bool initial)
{
	time_t rawtime;
	
  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );
  	
  	// It is possible to set only some of the options (--year --month --day)
  	// and still get a valid date (missing values are taken from the current date)
  	if (day > 0)
  		timeinfo->tm_mday = day;
  		
  	if (month > 0)
  		timeinfo->tm_mon = month - 1;
  		
  	if (year > 0)
  		timeinfo->tm_year = year - 1900;
  	
  	
  	if (mktime(timeinfo) == -1) {
  		if (initial) {
  			timeinfo = localtime ( &rawtime );
  			mktime(timeinfo);
  		
  			fDay = timeinfo->tm_mday;
  			fMonth = timeinfo->tm_mon + 1;
  			fYear = timeinfo->tm_year + 1900;
  		}
  	} else {
  		fDay = timeinfo->tm_mday;
  		fMonth = timeinfo->tm_mon + 1;
  		fYear = timeinfo->tm_year + 1900;
  	}
}


CalendarDialog::CalendarDialog(BString const & title, float width, float height,
	BString const & text, int day, int month, int year, BString const & format,
	BString const & windowIcon)
    :
    Dialog(title, windowIcon, width, height)
{
    if (title.Length() == 0)
        SetTitle(kCalendarTitle);
    
    fText = (text > 0 ? text : kCalendarText);
    
    if (width < 1.0)
        width = 200;
    
    if (height < 1.0)
        height = 210;
        
    SetDate(day, month, year, true);
    
    fFormat = (format != NULL ? format : BString("%x"));
    
    CreateViews();
    
    ResizeTo(width, height);
    
    InitControls();
}

void
CalendarDialog::CreateViews()
{
    BButton* okayButton = new BButton(
        BRect(0, 1, 80, 1), "okayButton", kOk, new BMessage(MSG_OK_CLICKED),
        B_FOLLOW_RIGHT);
        
    okayButton->MakeDefault(true);
    
    BButton* cancelButton = new BButton(
        BRect(0, 1, 80, 1), "cancelButton", kCancel, new BMessage(MSG_CANCEL_CLICKED),
        B_FOLLOW_RIGHT);
    
    BStringView* textLabel = new BStringView(
                    BRect(0, 0, 1, 1), NULL, fText);
    textLabel->SetAlignment(B_ALIGN_LEFT);
    textLabel->SetExplicitMaxSize(
        BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
        
    fmonthView = new MonthWindowView(fDay, fMonth, fYear, 0, 10000);

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 5)
        .Add(textLabel)
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
            .AddGlue()
            .Add(fmonthView)
            .AddGlue()
         )
        .AddGlue()
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
            .AddGlue()
            .Add(cancelButton)
            .Add(okayButton)
        )
        .SetInsets(5, 5, 5, 5)
    );
}


void
CalendarDialog::InitControls()
{
    Lock();
    
    fmonthView->MakeFocus(true);
    CenterOnScreen();
    
    Unlock();
}

void
CalendarDialog::MessageReceived(BMessage* message)
{
    switch (message->what) {
        case MSG_OK_CLICKED:
        {
   			const size_t len = 256;
        	char buffer[len];

			SetDate(fDay, fMonth, fYear);
			strftime (buffer, len - 1, fFormat.String(), timeinfo);
        	fprintf(stdout, "%s\n", buffer);
        
            be_app->PostMessage(MSG_OK_CLICKED);
            Quit();
        
            break;
        }
        
        case MSG_CANCEL_CLICKED:
        
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            Quit();
            
            break;
            
        case 'MVME':
    	{ 
			  // Date selected
			  int32 day, month, year;
			  if(message->FindInt32("day",&day)!=B_OK) return;
			  if(message->FindInt32("month",&month)!=B_OK) return;
			  if(message->FindInt32("year",&year)!=B_OK) return;
			  
			  SetDate(day, month, year);
			  
			  break;
    	}
        	
            
        default:
            Dialog::MessageReceived(message);
            break;
    }
}
