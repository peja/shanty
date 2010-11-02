/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <stdlib.h>
#include <ctype.h>
#include "ProgressDialog.h"

extern char* kOk;
extern char* kCancel;

const char* kProgressTitle = "Progress";
const char* kProgressText  = "Running...";

// BarberPole colors
const rgb_color kHighColor = {50, 150, 255, 255};
const rgb_color kLowColor = {255, 255, 255, 255};

ssize_t
read_stdin_nonblocking(char* str, size_t size);


ProgressDialog*
ProgressDialog::fInstance = NULL;

thread_id
ProgressDialog::fThreadId = 0;


ProgressDialog::ProgressDialog(char* title, float width, float height, char* text,
                               int percentage, bool pulsate, bool autoClose, bool noCancel)
                         : Dialog(title, width, height)
{
    if (title == NULL)
        SetTitle(kProgressTitle);
    
    fText = (text != NULL ? text : kProgressText);
    
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
    			 
        
    // Build the layout
    SetLayout(new BGroupLayout(B_HORIZONTAL));

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10)
        .Add(fTextLabel)
        .Add(fPulsate ? fBarberPole : fStatusBar)
        .AddGlue()
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
            .AddGlue()
            .Add(cancelButton)
            .AddGlue()
            .Add(fOkayButton)
            .AddGlue()
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
      
        Hide();
        be_app->PostMessage(MSG_OK_CLICKED);
		
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
        
            Hide();
            be_app->PostMessage(MSG_OK_CLICKED);
        
            break;
        
        case MSG_CANCEL_CLICKED:
        
        	_EndThread();
        
            Hide();
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            
            break;
            
        default:
            Dialog::MessageReceived(message);
            break;
    }
}
