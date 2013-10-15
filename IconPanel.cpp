/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "IconPanel.h"

#include <Bitmap.h>
#include <File.h>
#include <FindDirectory.h>
#include <IconUtils.h>
#include <Path.h>
#include <Resources.h>

#include <new>
#include <stdexcept>

IconPanel::IconPanel(BString const &icon)
	:
	BView(BRect(0, 0, kBorderWidth + kBorderMargin, 0), "iconPanel",
		B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_SUPPORTS_LAYOUT),
	fIconName(icon)
{
	fIcon = new(std::nothrow) BBitmap(BRect(0, 0, 32, 32), 0, B_RGBA32);
	if (fIcon == NULL)
		throw std::runtime_error("BBitmap: No memory for bitmap");
	
	if (fIconName == "info" || fIconName == "warning"
		|| fIconName == "question" || fIconName == "error") {
		// Extract icons from app_server
		status_t status;
		
		BPath path;
		status = find_directory(B_BEOS_SERVERS_DIRECTORY, &path);
		if (status < B_OK)
			throw std::runtime_error("find_directory() failed");
		
		path.Append("app_server");
		BFile file;
		status = file.SetTo(path.Path(), B_READ_ONLY);
		if (status < B_OK)
			throw std::runtime_error("BFile.SetTo() failed");
		
		BResources resources;
		status = resources.SetTo(&file);
		if (status < B_OK)
			throw std::runtime_error("BResources.SetTo() failed");
	
		BString iconName;
		
		if (fIconName == "info")
			iconName = "info";
		else if (fIconName == "warning" || fIconName == "question")
			iconName = "warn";
		else if (fIconName == "error")
			iconName = "stop";
	
		size_t size;
		const uint8* rawIcon;
	
		rawIcon = (const uint8*) resources.LoadResource(B_VECTOR_ICON_TYPE, iconName.String(), &size);
		if (rawIcon == NULL)
			throw std::runtime_error("Icon resource not found");

		BIconUtils::GetVectorIcon(rawIcon, size, fIcon);
	} else {
		status_t status;
		
		BFile file;
		status = file.SetTo(fIconName, B_READ_ONLY);
		if (status >= B_OK)
		{
			size_t size;
			off_t file_size;
			uint8* rawIcon;
			
			file.GetSize(&file_size);
			rawIcon = new uint8[file_size];
			size = file.Read(rawIcon, file_size);
			
			BIconUtils::GetVectorIcon(rawIcon, size, fIcon);
			
			delete rawIcon;
		
			// does not throw an exception so user can run script
			// even if he did not get icons with it
		}
	}
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
