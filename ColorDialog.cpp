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

const BString kColorTitle = "Color selection";

enum
{
	MSG_COLOR_CHANGED = 'clrc',
	MSG_HEX_COLOR_CHANGED = 'hclc',
	MSG_HUE_CHANGED = 'hchn',
	MSG_SAT_CHANGED = 'schn',
	MSG_VAL_CHANGED = 'vchn',
};

ColorDialog::ColorDialog(BString const & title, float width, float height,
						 rgb_color color, BString const & windowIcon)
						:
						Dialog(title, windowIcon, width, height)
{
	if (title.Length() == 0) {
		SetTitle(kColorTitle);
	}
	
	fColor = color;
	
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
		BPoint(1, 1), B_CELLS_32x8, 4, "colorSelection", new BMessage(MSG_COLOR_CHANGED),
		false);
	
	fColorHex = new BTextControl(
		BRect(0, 1, 80, 1), "colorHex", "Hex:", "", new BMessage(MSG_HEX_COLOR_CHANGED),
		B_FOLLOW_RIGHT);

	fColorHue = new BTextControl(
		BRect(0, 1, 80, 1), "colorHue", "H:", "", new BMessage(MSG_HUE_CHANGED),
		B_FOLLOW_RIGHT);

	fColorSaturation = new BTextControl(
		BRect(0, 1, 80, 1), "colorSaturation", "S:", "", new BMessage(MSG_SAT_CHANGED),
		B_FOLLOW_RIGHT);

	fColorValue = new BTextControl(
		BRect(0, 1, 80, 1), "colorValue", "V:", "", new BMessage(MSG_VAL_CHANGED),
		B_FOLLOW_RIGHT);
	
	AddChild(BGroupLayoutBuilder(B_VERTICAL, 5)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
			.Add(fColorControl)
			.Add(BGroupLayoutBuilder(B_VERTICAL, 0)
				.Add(fColorHue)
				.Add(fColorSaturation)
				.Add(fColorValue)
			)
		)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
			.Add(fColorView)
			.Add(fColorHex)
		)
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
ColorDialog::InitControls()
{
	Lock();
	
	fColorControl->SetValue(fColor);
	fColorControl->Looper()->PostMessage(MSG_COLOR_CHANGED);
		// update ColorView and ColorHex
	
	fColorHex->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	
	fColorHue->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	
	fColorSaturation->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	
	fColorValue->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
	
	// Disable all characters first
	for (uint32 i = 0; i < 256; i++) {
		fColorHex->TextView()->DisallowChar(i);
		fColorHue->TextView()->DisallowChar(i);
		fColorSaturation->TextView()->DisallowChar(i);
		fColorValue->TextView()->DisallowChar(i);
	}
	
	// Allow only numbers
	for (uint32 i = '0'; i <= '9'; i++) {
		fColorHex->TextView()->AllowChar(i);
		fColorHue->TextView()->AllowChar(i);
		fColorSaturation->TextView()->AllowChar(i);
		fColorValue->TextView()->AllowChar(i);
	}
	
	// Allow [a-fA-F] for fColorHex
	for (uint32 i = 'A'; i <= 'F'; i++) {
		fColorHex->TextView()->AllowChar(i);
		fColorHex->TextView()->AllowChar(i + 32); // lowercase
	}
	
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
			hsv_color hsv = rgb_to_hsv_color(color);
			
			fColorView->SetViewColor(color);
			fColorView->Invalidate();
			
			BString temp = "";
			temp << hsv.hue;
			fColorHue->SetText(temp);
			temp = "";
			temp << hsv.saturation;
			fColorSaturation->SetText(temp);
			temp = "";
			temp << hsv.value;
			fColorValue->SetText(temp);

			char hex_color[7];
			sprintf(hex_color, "%02X%02X%02X", color.red, color.green, color.blue);
			
			fColorHex->SetText(hex_color);
			
			break;
		}
		
		case MSG_HEX_COLOR_CHANGED:
		{
			rgb_color color = hex_string_to_rgb_color(fColorHex->Text());
			hsv_color hsv = rgb_to_hsv_color(color);

			fColorControl->SetValue(color);

			BString temp = "";
			temp << hsv.hue;
			fColorHue->SetText(temp);
			temp = "";
			temp << hsv.saturation;
			fColorSaturation->SetText(temp);
			temp = "";
			temp << hsv.value;
			fColorValue->SetText(temp);
			
			fColorView->SetViewColor(color);
			fColorView->Invalidate();
			
			break;
		}
		
		case MSG_HUE_CHANGED:
		case MSG_SAT_CHANGED:
		case MSG_VAL_CHANGED:
		{
			hsv_color hsv;
			hsv.hue = atoi(fColorHue->Text());
			hsv.saturation = atoi(fColorSaturation->Text());
			hsv.value = atoi(fColorValue->Text());
			
			if (hsv.hue < 0 || hsv.hue > 240) {
				if (hsv.hue < 0)
					hsv.hue = 0;
				else if (hsv.hue > 240)
					hsv.hue = 240;
				
				BString temp = "";
				temp << hsv.hue;
				fColorHue->SetText(temp);
			}

			if (hsv.saturation < 0 || hsv.saturation > 100) {
				if (hsv.saturation < 0)
					hsv.saturation = 0;
				else if (hsv.saturation > 100)
					hsv.saturation = 100;

				BString temp = "";
				temp << hsv.saturation;
				fColorSaturation->SetText(temp);	
			}
			
			if (hsv.value < 0 || hsv.value > 100) {
				if (hsv.value < 0)
					hsv.value = 0;
				else if (hsv.value > 100)
					hsv.value = 100;

				BString temp = "";
				temp << hsv.value;
				fColorValue->SetText(temp);
			}
			
			rgb_color color = hsv_to_rgb_color(hsv);
			
			fColorControl->SetValue(color);
			
			fColorView->SetViewColor(color);
			fColorView->Invalidate();
			
			char hex_color[7];
			sprintf(hex_color, "%02X%02X%02X", color.red, color.green, color.blue);
			
			fColorHex->SetText(hex_color);
			
			break;
		}
			
		default:
			Dialog::MessageReceived(message);
			break;	
	}
}
