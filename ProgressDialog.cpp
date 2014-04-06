/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic 
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <stdlib.h>
#include <ctype.h>
#include "ProgressDialog.h"
#include "utils.h"

extern char kOk[];
extern char kCancel[];

const BString kProgressTitle = "Progress";
const BString kProgressText = "Running...";

// BarberPole colors
const rgb_color kHighColor = {50, 150, 255, 255};
const rgb_color kLowColor = {255, 255, 255, 255};

ProgressDialog*
ProgressDialog::fInstance = NULL;

thread_id
ProgressDialog::fThreadId = 0;


ProgressDialog::ProgressDialog(BString const & title, float width, float height,
	BString const & text, int percentage, bool pulsate, bool autoClose, bool noCancel,
	BString const & windowIcon)
	:
	Dialog(title, windowIcon, width, height)
{
    if (title.Length() == 0)
        SetTitle(kProgressTitle);
    
    fText = (text.Length() > 0 ? text : kProgressText);
    
	fPercentage = percentage;
        
    fPulsate = pulsate;
    fAutoClose = autoClose;
    fNoCancel = noCancel;
    
    if (width < 1.0)
        width = 200;
    
    if (height < 1.0)
        height = 110;
    
    
    CreateViews();
    
    ResizeTo(width, height);
    
    InitControls();
    
    fThreadId = spawn_thread(_ReadInputThread, "readInput", B_NORMAL_PRIORITY, NULL);
    
    if (fThreadId >= B_OK) {
    	resume_thread(fThreadId);
    }
    
    fInstance = this;
}

void
ProgressDialog::CreateViews()
{	
    fOkayButton = new BButton(
        BRect(0, 1, 80, 1), "okayButton", kOk, new BMessage(MSG_OK_CLICKED),
        B_FOLLOW_RIGHT);
        
    fOkayButton->SetEnabled(false);
    
    BButton* cancelButton = new BButton(
        BRect(0, 1, 80, 1), "cancelButton", kCancel, new BMessage(MSG_CANCEL_CLICKED),
        B_FOLLOW_RIGHT);
    
    if (fNoCancel) {
    	
    	// Disable closing of the dialog
		SetFlags(Flags() | B_NOT_CLOSABLE);
		
		cancelButton->Hide();
	} else {
    	cancelButton->MakeDefault(true);
	}
    
    
    fTextLabel = new BStringView(
                    BRect(0, 0, 1, 1), NULL, fText);
    fTextLabel->SetAlignment(B_ALIGN_LEFT);
    fTextLabel->SetExplicitMaxSize(
        	BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
        
    if (!fPulsate) {
        
    	fStatusBar = new BStatusBar(
    			 BRect(0, 0, 1, 1), "statusBar", NULL, NULL);
    } else {
    	fBarberPole = new BarberPole(
    		BRect(0, 0, 1, 16), "barberPole", B_FOLLOW_LEFT, 0);
    		
    	fBarberPole->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, 16));
    	
    	fBarberPole->SetHighColor(kHighColor);
    	fBarberPole->SetLowColor(kLowColor);
    }

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 5)
        .Add(fTextLabel)
        .Add(fPulsate ? (BView*)fBarberPole : (BView*)fStatusBar)
        .AddGlue()
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
            .AddGlue()
            .Add(cancelButton)
            .Add(fOkayButton)
        )
        .SetInsets(5, 5, 5, 5)
    );
}


void
ProgressDialog::InitControls()
{
    Lock();
    
    if (!fPulsate)
    	fStatusBar->Update(fPercentage, NULL, NULL);
    else {
    	SetPulseRate(0);
    }
    
    CenterOnScreen();
    
    Unlock();
}

void
ProgressDialog::FrameResized(float width, float height)
{
	if (fPulsate)
		fBarberPole->Invalidate();
	
	Dialog::FrameResized(width, height);
}

int32
ProgressDialog::_ReadInputThread(void* data)
{
	const size_t len = 8192;
	char buffer[len];
	
	ssize_t n;
	
	BString line;
	
	for(;;) {
		n = read_stdin_nonblocking(buffer, len - 1);
		
		if (fInstance->fPulsate)
			fInstance->SetPulseRate(100000);
		
		if (n != -1) {
   		
   			for (int i = 0; i < n; i++) {
   				if (buffer[i] != '\n') {
   					line += buffer[i];
   					
   				} else {
   					fInstance->_ProcessLine(line);
   					line.Truncate(0);
   				}
   			}
		} else {
			fInstance->_ProcessLine(line);
			break;
		}
    	
	}
	
	fInstance->_EndInput();
	
	return 0;
}

void
ProgressDialog::_ProcessLine(const BString& line)
{
	switch (line.ByteAt(0)) {
		
	case '\0': return;
	
	case '#':
	{
		// Change the text label
		
		// Trim the input line
		int32 start, end;
		
		for (start = 1; start < line.Length() && isspace(line[start]); start++) ;
		
		for (int i = start; i < line.Length(); i++) {
			if (!isspace(line[i]))
				end = i;
		}
		
		BString label;
		
		line.CopyInto(label, start, end - start + 1);
		
		Lock();
		
		fInstance->fTextLabel->SetText(label.String());
		
		Unlock();
		
		break;
	}
	
	default:
	{
		if (fPulsate)
			return;
		
		// Update progress

		const char *line_ptr = line.String();
		char *end_ptr;
		
		int p = strtoul(line_ptr, &end_ptr, 10);
		
		if (end_ptr == line_ptr) {
			// Unsuccessful conversion
			return;
		}
		
		Lock();
		
		fStatusBar->Update(p - fPercentage, NULL, NULL);
		
		Unlock();
		
		fPercentage = p;
		
		if (fPercentage >= 100) {
			_EndInput();
		}
	}
		
	}
}

void
ProgressDialog::_EndInput()
{
	if (fAutoClose) {
				
		_EndThread();
      
        be_app->PostMessage(MSG_OK_CLICKED);
        be_app->PostMessage(B_QUIT_REQUESTED);
		
	} else {
		Lock();
		
		fOkayButton->SetEnabled(true);
		fOkayButton->MakeDefault(true);
		
		if (fPulsate) {
			SetPulseRate(0);
			
		} else {
			fStatusBar->Update(100 - fPercentage, NULL, NULL);
		}
		
		Unlock();
	}
}

void
ProgressDialog::_EndThread()
{
    if (fThreadId >= B_OK)
    	send_data(fThreadId, 0, NULL, 0);
}

bool
ProgressDialog::QuitRequested()
{
    _EndThread();
    
    return Dialog::QuitRequested();
}

void
ProgressDialog::MessageReceived(BMessage* message)
{
    switch (message->what) {
        case MSG_OK_CLICKED:
        
        	_EndThread();
        
            be_app->PostMessage(MSG_OK_CLICKED);
            Quit();
        
            break;
        
        case MSG_CANCEL_CLICKED:
        
        	_EndThread();
        
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            Quit();
            
            break;
            
        default:
            Dialog::MessageReceived(message);
            break;
    }
}

