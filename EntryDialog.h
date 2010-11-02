/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#ifndef	ENTRY_DIALOG_H
#define	ENTRY_DIALOG_H

#include <interface/StringView.h>
#include <interface/TextControl.h>

#include "Dialog.h"


class EntryDialog : public Dialog
{
public:

    EntryDialog(char* title, float width, float height,
                char* text, char* entryText, bool hideText);
                
    
    void MessageReceived(BMessage* msg);
    void CreateViews();
    void InitControls();
    
private:
    
    const char* fText;
    const char* fEntryText;
    bool fHideText;
    
    BTextControl* fEntry;
};

#endif	// ENTRY_DIALOG_H
