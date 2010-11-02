/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#ifndef	TEXT_INFO_DIALOG_H
#define	TEXT_INFO_DIALOG_H

#include <interface/ScrollView.h>
#include <interface/TextView.h>
#include "Dialog.h"


class EditBox : public BTextView
{
public:
	
	EditBox(BRect viewFrame, bool editable);
	void FrameResized(float width, float height);
	
};


class TextInfoDialog : public Dialog
{
public:

    TextInfoDialog(char* title, float width, float height,
                   char* fileName, bool editable);
                
    
    void MessageReceived(BMessage* msg);
    void CreateViews();
    void InitControls();
    bool QuitRequested();

private:
	static int32 _ReadInputThread(void* data);
	static void _EndThread();
    
private:
    static TextInfoDialog* fInstance;
    static thread_id fThreadId;
    
    const char* fFileName;
    bool fEditable;
    
    BTextView* fEditBox;
};

#endif	// TEXT_INFO_DIALOG_H
