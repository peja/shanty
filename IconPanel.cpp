/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "IconPanel.h"

#include <new>
#include <stdexcept>

#include "utils.h"

IconPanel::IconPanel(BString const &icon)
	:
	BView(BRect(0, 0, kBorderWidth + kBorderMargin, 0), "iconPanel",
		B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_SUPPORTS_LAYOUT),
	fIconName(icon)
{
	fIcon = new(std::nothrow) BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);
	if (fIcon == NULL)
		throw std::runtime_error("BBitmap: No memory for bitmap");
	
	get_vector_icon(icon, fIcon);
}

void
IconPanel::Draw(BRect updateRect)
{
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	FillRect(Bounds());
	
	BRect drawBounds(Bounds());
	drawBounds.right = kBorderWidth;
	SetHighColor(kDarkBorderColor);
	FillRect(drawBounds);
	
	SetDrawingMode(B_OP_OVER);
	DrawBitmap(fIcon, BPoint(kIconHorizOffset, kIconVertOffset));
	SetDrawingMode(B_OP_COPY);
}
