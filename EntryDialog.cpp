/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include "EntryDialog.h"

extern char kOk[];
extern char kCancel[];

const BString kEntryTitle = "Add a new entry";
const BString kEntryInitialText = "Enter new text:";

EntryDialog::EntryDialog(BString const & title, float width, float height,
	BString const & text, BString const & entryText,
	bool hideText, BString const & windowIcon)
	:
	Dialog(title, windowIcon, width, height)
{
    if (title.Length() == 0)
        SetTitle(kEntryTitle);
    
    fText = text;
    
    if (fText.Length() == 0)
        fText = kEntryInitialText;
        
    fEntryText = entryText;
    fHideText = hideText;
    
    if (width < 1.0)
        width = 200;
    
    if (height < 1.0)
        height = 110;
    
    //AddShortcut(B_ESCAPE, 0, new BMessage(MSG_CANCEL_CLICKED));
    
    CreateViews();
    
    ResizeTo(width, height);
    
    InitControls();
}

void
EntryDialog::CreateViews()
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
        
    fEntry = new BTextControl(
    			 BRect(0, 0, 1, 1), "textEntry", NULL, NULL, NULL);
    			 
    fEntry->SetExplicitMaxSize(
        BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 5)
        .Add(textLabel)
        .Add(fEntry)
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
EntryDialog::InitControls()
{
    Lock();
    
    fEntry->TextView()->HideTyping(fHideText);
    fEntry->SetText(fEntryText);
    fEntry->MakeFocus(true);
    
    CenterOnScreen();
    
    Unlock();
}

void
EntryDialog::MessageReceived(BMessage* message)
{
    switch (message->what) {
        case MSG_OK_CLICKED:
        
        	fprintf(stdout, "%s\n", fEntry->Text());

            be_app->PostMessage(MSG_OK_CLICKED);
            Quit();
        
            break;
        
        case MSG_CANCEL_CLICKED:
        
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            Quit();
            
            break;
            
        default:
            Dialog::MessageReceived(message);
            break;
    }
}


