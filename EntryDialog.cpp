/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <stdio.h>
#include "EntryDialog.h"

extern char* kOk;
extern char* kCancel;

const char* kEntryTitle = "Add a new entry";
const char* kEntryInitialText  = "Enter new text:";

EntryDialog::EntryDialog(char* title, float width, float height,
                         char* text, char* entryText, bool hideText)
                         : Dialog(title, width, height)
{
    if (title == NULL)
        SetTitle(kEntryTitle);
    
    fText = text;
    
    if (fText == NULL)
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
        
    // Build the layout
    SetLayout(new BGroupLayout(B_HORIZONTAL));

    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10)
        .Add(textLabel)
        .Add(fEntry)
        .AddGlue()
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
            .AddGlue()
            .Add(cancelButton)
            .AddGlue()
            .Add(okayButton)
            .AddGlue()
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
        
            Hide();
            be_app->PostMessage(MSG_OK_CLICKED);
        
            break;
        
        case MSG_CANCEL_CLICKED:
        
            Hide();
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            
            break;
            
        default:
            Dialog::MessageReceived(message);
            break;
    }
}

