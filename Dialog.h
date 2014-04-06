/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */
 
#ifndef	DIALOG_H
#define	DIALOG_H

#include <Application.h>
#include <Window.h>
#include <Button.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <SpaceLayoutItem.h>
#include <String.h>

enum
{
	MSG_OK_CLICKED = 'okcl',
	MSG_CANCEL_CLICKED = 'cacl',
};

class Dialog : public BWindow {
public:
    				Dialog(BString const & title, BString const & windowIcon,
    					float width, float height);
    virtual bool	QuitRequested();
    
    void			CenterOnScreen();
};

#endif	// DIALOG_H
