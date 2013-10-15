/*
 * Copyright 2013, Kacper Kasper, kacperkasper@gmail.com
 * Copyright 2010, Milos Pejovic
 * All rights reserved. Distributed under the terms of the MIT License.
 */

 
#ifndef	SCALE_DIALOG_H
#define	SCALE_DIALOG_H

#include <interface/Slider.h>
#include <interface/StringView.h>
#include "Dialog.h"


class ScaleDialog : public Dialog
{
public:

    ScaleDialog(char* title, float width, float height,
                char* text, int32 value, int32 minValue, int32 maxValue,
                int32 step, bool printPartial, bool hideValue, BString windowIcon);
                
    
    void MessageReceived(BMessage* msg);
    void CreateViews();
    void InitControls();
    
private:
    
    const char* fText;
    const char* fEntryText;
    int32 fValue, fMinValue, fMaxValue, fStep;
    bool fPrintPartial, fHideValue;
    
    BSlider* fSlider;
    BStringView* fValueLabel;
};

#endif	// SCALE_DIALOG_H
