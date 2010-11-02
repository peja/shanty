/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#ifndef	PROGRESS_DIALOG_H
#define	PROGRESS_DIALOG_H

#include <support/String.h>
#include <interface/StatusBar.h>
#include <interface/StringView.h>

#include "BarberPole/BarberPole.h"
#include "Dialog.h"


class ProgressDialog : public Dialog
{
public:

    ProgressDialog(char* title, float width, float height, char* text,
                   int percentage, bool pulsate, bool autoClose, bool noCancel);
                
    
    void MessageReceived(BMessage* msg);
    void CreateViews();
    void InitControls();
    
    void FrameResized(float width, float height);
	bool QuitRequested();
    
private:

	static int32 _ReadInputThread(void* data);
	static void _EndThread();
	
	void _ProcessLine(const BString& line);
	void _EndInput();
    
private:

	static ProgressDialog* fInstance;
    static thread_id fThreadId;
    
    const char* fText;
    int fPercentage;
    bool fPulsate;
    bool fAutoClose;
    bool fNoCancel;
    
    BStringView* fTextLabel;
    BStatusBar* fStatusBar;
    BButton* fOkayButton;
    BarberPole* fBarberPole;
};

#endif	// PROGRESS_DIALOG_H
