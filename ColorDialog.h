/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COLOR_DIALOG_H
#define COLOR_DIALOG_H

#include <ColorControl.h>
#include <SupportDefs.h>
#include <TextControl.h>
#include <String.h>

#include "Dialog.h"

class ColorDialog : public Dialog {
public:
					ColorDialog(BString const & title, float width, float height,
						rgb_color color, BString const & windowIcon);
	
	void			MessageReceived(BMessage* msg);
	void 			CreateViews();
	void 			InitControls();
	
private:
	rgb_color		fColor;

	BView*			fColorView;
	BColorControl* 	fColorControl;
	BTextControl*	fColorHex;
	BTextControl*	fColorHue;
	BTextControl*	fColorSaturation;
	BTextControl*	fColorValue;
};

#endif // COLOR_DIALOG_H
