/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#include <stdio.h>

#include <String.h>

#include "ColorDialog.h"

extern char kOk[];
extern char kCancel[];

const BString kColorTitle = BString("Color selection");

ColorDialog::ColorDialog(BString title, float width, float height,
						 rgb_color color)
						:
						fColor(color),
						Dialog(title.String(), width, height)
{
	if (title.Length() == 0) {
		SetTitle(kColorTitle);
	}
	
	if (width < 1.0) {
		width = 200;
	}
	
	if (height < 1.0) {
		height = 110;
	}
	
	CreateViews();
	
	ResizeTo(width, height);
	
	InitControls();
}

void
ColorDialog::CreateViews()
{
	BButton* okayButton = new BButton(
		BRect(0, 1, 80, 1), "okayButton", kOk, new BMessage(MSG_OK_CLICKED),
		B_FOLLOW_RIGHT);
		
	okayButton->MakeDefault(true);
	
	BButton* cancelButton = new BButton(
		BRect(0, 1, 80, 1), "cancelButton", kCancel, new BMessage(MSG_CANCEL_CLICKED),
		B_FOLLOW_RIGHT);
		
	fColorControl = new BColorControl(
		BPoint(1, 1), B_CELLS_32x8, 4, "colorSelection", NULL, false);
		
	SetLayout(new BGroupLayout(B_HORIZONTAL));
	
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 10)
		.Add(fColorControl)
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
ColorDialog::InitControls()
{
	Lock();
	
	fColorControl->SetValue(fColor);
	
	CenterOnScreen();
	
	Unlock();
}

void ColorDialog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_OK_CLICKED:
			fprintf(stdout, "#%x%x%x%x%x%x\n",
				fColorControl->ValueAsColor().red,
				fColorControl->ValueAsColor().red,
				fColorControl->ValueAsColor().green,
				fColorControl->ValueAsColor().green,
				fColorControl->ValueAsColor().blue,
				fColorControl->ValueAsColor().blue);
				// zenity-compatible format
			
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
