/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

 
#ifndef	PROGRESS_DIALOG_H
#define	PROGRESS_DIALOG_H

#include <StatusBar.h>
#include <StringView.h>

#include "BarberPole/BarberPole.h"
#include "Dialog.h"


class ProgressDialog : public Dialog
{
public:
							ProgressDialog(BString const & title, float width, float height,
								BString const & text, int percentage, bool pulsate, bool autoClose,
								bool noCancel, BString const & windowIcon);

    		void			MessageReceived(BMessage* msg);
    		void			CreateViews();
    		void			InitControls();
    
    		void			FrameResized(float width, float height);
			bool			QuitRequested();
    
private:
	static	int32			_ReadInputThread(void* data);
	static	void			_EndThread();
	
			void			_ProcessLine(const BString& line);
			void			_EndInput();

	static	ProgressDialog*	fInstance;
    static	thread_id		fThreadId;
    
    		BString			fText;
    		int				fPercentage;
    		bool			fPulsate;
    		bool			fAutoClose;
    		bool			fNoCancel;
    
    		BStringView*	fTextLabel;
    		BStatusBar*		fStatusBar;
			BButton*		fOkayButton;
			BarberPole*		fBarberPole;
};

#endif	// PROGRESS_DIALOG_H
