/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ICON_PANEL_H
#define ICON_PANEL_H

#include <String.h>
#include <SupportDefs.h>
#include <View.h>

// Taken from /src/kits/tracker/InfoWindow.cpp (line 192)
const float kBorderMargin = 15.0f;
const float kBorderWidth = 32.0f;

const float kIconHorizOffset = 18.0f;
const float kIconVertOffset = 6.0f;

const rgb_color kDarkBorderColor = { 184, 184, 184, 255 };

class IconPanel : public BView {
public:
	IconPanel(BString const &icon);
	
	void Draw(BRect updateRect);						
private:
	BString fIconName;
	BBitmap *fIcon;
};

#endif // ICON_PANEL_H
