/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
 
#include <stdio.h>
#include <stdlib.h>

#include <String.h>

#include "ColorDialog.h"
#include "IconPanel.h"
#include "utils.h"

extern char kOk[];
extern char kCancel[];

const BString kColorTitle = BString("Color selection");

const uint32 MSG_COLOR_CHANGED = 'clrc';
const uint32 MSG_HEX_COLOR_CHANGED = 'hclc';

ColorDialog::ColorDialog(BString title, float width, float height,
						 rgb_color color, BString windowIcon)
						:
						fColor(color),
						Dialog(title, windowIcon, width, height)
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
		
	fColorView = new BView(
		BRect(0, 1, 80, 20), "colorView", B_FOLLOW_LEFT, B_SUPPORTS_LAYOUT);
		
	fColorControl = new BColorControl(
		BPoint(1, 1), B_CELLS_32x8, 4, "colorSelection", new BMessage(MSG_COLOR_CHANGED), false);
	
	fColorHex = new BTextControl(
		BRect(0, 1, 80, 1), "colorHex", "Hex:", "", new BMessage(MSG_HEX_COLOR_CHANGED),
		B_FOLLOW_RIGHT);
	
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 5)
		.Add(fColorControl)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
			.Add(fColorView)
			.Add(fColorHex)
		)
		.AddGlue()
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
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
	fColorControl->Looper()->PostMessage(MSG_COLOR_CHANGED);
		// update ColorView and ColorHex
	
	fColorHex->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	fColorHex->SetModificationMessage(new BMessage(MSG_HEX_COLOR_CHANGED));
	
	CenterOnScreen();
	
	Unlock();
}

void ColorDialog::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_OK_CLICKED:
		{
			rgb_color color = fColorControl->ValueAsColor();
			fprintf(stdout, "#%02x%02x%02x%02x%02x%02x\n", color.red, color.red,
				color.green, color.green, color.blue, color.blue);
				// zenity-compatible format
			
			be_app->PostMessage(MSG_OK_CLICKED);
			Quit();
			
			break;
		}
		
		case MSG_CANCEL_CLICKED:
			be_app->PostMessage(MSG_CANCEL_CLICKED);
			Quit();
			
			break;
			
		case MSG_COLOR_CHANGED:
		{
			rgb_color color = fColorControl->ValueAsColor();
			
			fColorView->SetViewColor(color);
			fColorView->Invalidate();
			
			char hex_color[8];
			sprintf(hex_color, "#%02X%02X%02X", color.red, color.green, color.blue);
			
			fColorHex->SetText(hex_color);
			
			break;
		}
		
		case MSG_HEX_COLOR_CHANGED:
		{   	
            rgb_color color = hex_string_to_rgb_color(fColorHex->Text());
            
            fColorControl->SetValue(color);
            
            fColorView->SetViewColor(color);
            fColorView->Invalidate();
		}
			
		default:
			Dialog::MessageReceived(message);
			break;	
	}
}
