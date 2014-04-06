/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */
 
#ifndef	TEXT_INFO_DIALOG_H
#define	TEXT_INFO_DIALOG_H

#include <interface/ScrollView.h>
#include <interface/TextView.h>
#include "Dialog.h"


class EditBox : public BTextView
{
public:
			EditBox(BRect viewFrame, bool editable, bool fixedFont);
	void	FrameResized(float width, float height);
};


class TextInfoDialog : public Dialog
{
public:
							TextInfoDialog(BString const & title, float width, float height,
								BString const & fileName, bool editable, bool fixedFont,
								BString const & windowIcon);
    
    		void			MessageReceived(BMessage* msg);
    		void			CreateViews();
    		void			InitControls();
    		bool			QuitRequested();

private:
	static	int32			_ReadInputThread(void* data);
	static	void			_EndThread();
    
    static	TextInfoDialog*	fInstance;
    static	thread_id		fThreadId;
    
    		BString			fFileName;
    		bool			fEditable;
    		bool			fFixedFont;

			BTextView*		fEditBox;
};

#endif	// TEXT_INFO_DIALOG_H
