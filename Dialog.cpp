/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "Dialog.h"
#include "utils.h"


void
Dialog::CenterOnScreen()
{
    center_on_screen(this);
}


Dialog::Dialog(const char* title, float width, float height)
    : BWindow(BRect(0, 0, width, height), title, B_TITLED_WINDOW, 
    B_ASYNCHRONOUS_CONTROLS | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{}


bool
Dialog::QuitRequested()
{
    be_app->PostMessage(MSG_CANCEL_CLICKED);
    return true;
}
