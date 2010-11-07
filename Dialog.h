/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
 
#ifndef	DIALOG_H
#define	DIALOG_H

#include <app/Application.h>
#include <interface/Window.h>
#include <interface/Button.h>
#include <interface/GroupLayout.h>
#include <interface/GroupLayoutBuilder.h>
#include <interface/SpaceLayoutItem.h>

enum { MSG_OK_CLICKED=1000, MSG_CANCEL_CLICKED };

class Dialog : public BWindow {
public:

    Dialog(char* title, float width, float height);
    virtual bool QuitRequested();
    
    void CenterOnScreen();

};

#endif	// DIALOG_H
