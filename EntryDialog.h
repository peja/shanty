/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef	ENTRY_DIALOG_H
#define	ENTRY_DIALOG_H

#include <StringView.h>
#include <TextControl.h>

#include "Dialog.h"


class EntryDialog : public Dialog
{
public:

    				EntryDialog(BString const & title, float width, float height,
						BString const & text, BString const & entryText, bool hideText,
						BString const & windowIcon);        
    
    void			MessageReceived(BMessage* msg);
    void			CreateViews();
    void			InitControls();
    
private:
    BString			fText;
    BString			fEntryText;
    bool			fHideText;
    
    BTextControl*	fEntry;
};

#endif	// ENTRY_DIALOG_H
