/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef COLOR_DIALOG_H
#define COLOR_DIALOG_H

#include <ColorControl.h>
#include <SupportDefs.h>

#include "Dialog.h"

class ColorDialog : public Dialog {
public:
					ColorDialog(BString title, float width, float height,
								rgb_color color);
	
	void			MessageReceived(BMessage* msg);
	void 			CreateViews();
	void 			InitControls();
	
private:
	const rgb_color	fColor;

	BColorControl* 	fColorControl;
};

#endif // COLOR_DIALOG_H
