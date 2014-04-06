/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "Dialog.h"
#include "IconPanel.h"
#include "utils.h"

void
Dialog::CenterOnScreen()
{
    center_on_screen(this);
}


Dialog::Dialog(BString const & title, BString const & windowIcon,
	float width, float height)
	:
    BWindow(BRect(0, 0, width, height), title.String(), B_TITLED_WINDOW, 
		B_ASYNCHRONOUS_CONTROLS | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	SetLayout(new BGroupLayout(B_HORIZONTAL, 0));
	
	if (windowIcon.Length() > 0) {
		IconPanel *iconPanel = new IconPanel(windowIcon);
	
		AddChild(iconPanel);
	
		GetLayout()->ItemAt(0)->SetExplicitMaxSize(
			BSize(kBorderWidth + kBorderMargin, B_SIZE_UNLIMITED));
	}
}


bool
Dialog::QuitRequested()
{
    be_app->PostMessage(MSG_CANCEL_CLICKED);
    return true;
}
