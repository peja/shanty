/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include <storage/File.h>
#include "TextInfoDialog.h"


extern char* kClose;

const char* kTextInfoTitle = "Text View";

ssize_t
read_stdin_nonblocking(char* str, size_t size);


EditBox::EditBox(BRect viewFrame, bool editable)
	        : BTextView(viewFrame, "editBox", viewFrame, 
	                B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW)
{
	MakeEditable(editable);
}

void
EditBox::FrameResized(float width, float height)
{
	BTextView::FrameResized(width, height);
	
	const float TEXT_INSET = 2.0;
	
	BRect textRect;
	textRect = Bounds();
	textRect.OffsetTo(B_ORIGIN);
	textRect.InsetBy(TEXT_INSET, TEXT_INSET);
	SetTextRect(textRect);
}         


TextInfoDialog*
TextInfoDialog::fInstance = NULL;

thread_id
TextInfoDialog::fThreadId = 0;


TextInfoDialog::TextInfoDialog(char* title, float width, float height,
                         	   char* fileName, bool editable)
                         : Dialog(title, width, height)
{
    if (title == NULL)
        SetTitle(kTextInfoTitle);
        
    fFileName = fileName;
    fEditable = editable;
    
    if (width < 1.0)
        width = 300;
    
    if (height < 1.0)
        height = 400;
    
    //AddShortcut(B_ESCAPE, 0, new BMessage(MSG_CANCEL_CLICKED));
    
    CreateViews();
    
    ResizeTo(width, height);
    
    InitControls();
    
    // If filename is not given, get input from stdin
    if (fFileName == NULL) {
    	fThreadId = spawn_thread(TextInfoDialog::_ReadInputThread, "readInput", B_NORMAL_PRIORITY, NULL);
    
    	if (fThreadId >= B_OK) {
    		resume_thread(fThreadId);
    	}
    } else
    	fThreadId = B_OK - 1;
    	
    fInstance = this;
}

void
TextInfoDialog::CreateViews()
{
    BButton* closeButton = new BButton(
        BRect(0, 1, 80, 1), "closeButton", kClose, new BMessage(MSG_OK_CLICKED),
        B_FOLLOW_RIGHT);
        
    fEditBox = new EditBox(BRect(0, 0, 1, 1), fEditable);
    			 
        
    BScrollView* scrollView = new BScrollView("scrollview", fEditBox, B_FOLLOW_ALL, 0, false, true);
        
    scrollView->SetExplicitMaxSize(
        BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
        
    // Build the layout
    SetLayout(new BGroupLayout(B_HORIZONTAL));

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10)
        .Add(scrollView)
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
            .AddGlue()
            .Add(closeButton)
        )
        .SetInsets(5, 5, 5, 5)
    );
}


void
TextInfoDialog::InitControls()
{
    Lock();
    
    if (fFileName != NULL) {
    	BFile file;
    	
    	if (file.SetTo(fFileName, B_READ_ONLY) == B_OK) {
    		off_t len;
    		file.GetSize(&len);
    		char* text = new char[len];
    		
    		if (file.Read(text, len) >= B_OK) {
    			fEditBox->SetText(text);
    		}
    		
    		delete text;
    	}
    }
    
    fEditBox->MakeFocus(true);
    
    CenterOnScreen();
    
    Unlock();
}

int32
TextInfoDialog::_ReadInputThread(void* data)
{
	const size_t len = 8192;
	char buffer[len];
	
	// Read the input in a non-blocking way.
	
	ssize_t n;
	
	while ((n = read_stdin_nonblocking(buffer, len - 1)) != -1) {
   		
   		buffer[n] = '\0';
   			
		fInstance->Lock();
	
		fInstance->fEditBox->Insert(buffer);
	
		fInstance->Unlock();
    	
	}
	
	return 0;
}

void
TextInfoDialog::_EndThread()
{
    if (fThreadId >= B_OK)
    	send_data(fThreadId, 0, NULL, 0);
}

bool
TextInfoDialog::QuitRequested()
{
    _EndThread();
    
    return Dialog::QuitRequested();
}

void
TextInfoDialog::MessageReceived(BMessage* message)
{
    switch (message->what) {
        case MSG_OK_CLICKED:
        
        	if (fEditable) {
        		fprintf(stdout, "%s\n", fEditBox->Text());
        	}
        	
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


