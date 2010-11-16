/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <support/String.h>
#include <stdio.h>
#include "ScaleDialog.h"

extern char kOk[];
extern char kCancel[];

const char kScaleTitle[] = "Adjust the scale value";


ScaleDialog::ScaleDialog(char* title, float width, float height,
                         char* text, int32 value, int32 minValue, int32 maxValue,
                         int32 step, bool printPartial, bool hideValue)
                         : Dialog(title, width, height)
{
    if (title == NULL)
        SetTitle(kScaleTitle);
    
    fText = (text != NULL ? text : kScaleTitle);
    
    fMinValue = (minValue >= 0 ? minValue : 0);
        			
    fMaxValue = (maxValue > 0 ? maxValue : 100);
        			
    fStep = (fStep > 0 ? step : 1);
        			
    fValue = (fValue >= fMinValue && fValue <= fMaxValue ? value : 0);
    
    fPrintPartial = printPartial;
    fHideValue = hideValue;
    
    if (width < 1.0)
        width = 200;
    
    if (height < 1.0)
        height = 110;
    
    CreateViews();
    
    ResizeTo(width, height);
    
    InitControls();
}

void
ScaleDialog::CreateViews()
{
    BButton* okayButton = new BButton(
        BRect(0, 1, 80, 1), "okayButton", kOk, new BMessage(MSG_OK_CLICKED),
        B_FOLLOW_RIGHT);
        
    okayButton->MakeDefault(true);
    
    BButton* cancelButton = new BButton(
        BRect(0, 1, 80, 1), "cancelButton", kCancel, new BMessage(MSG_CANCEL_CLICKED),
        B_FOLLOW_RIGHT);
    
    BStringView* textLabel = new BStringView(
                    BRect(0, 0, 1, 1), NULL, fText);
    textLabel->SetAlignment(B_ALIGN_LEFT);
    textLabel->SetExplicitMaxSize(
        BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
        
    fSlider = new BSlider(
    			 BRect(0, 0, 1, 1), "slider", NULL, NULL, fMinValue, fMaxValue);
    			 
    fSlider->SetModificationMessage(new BMessage('SlDE'));
    			 
    fSlider->SetExplicitMaxSize(
        BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
        
    
    if (!fHideValue) {
    	BString val;
    	
    	val << fValue;
    	
    	fValueLabel = new BStringView(
                    BRect(0, 0, 1, 1), NULL, val.String());               
    }
        
        
    // Build the layout
    SetLayout(new BGroupLayout(B_HORIZONTAL));
    
    BGroupLayoutBuilder builder = BGroupLayoutBuilder(B_VERTICAL, 10).Add(textLabel);

	if (!fHideValue)
		builder.Add(BGroupLayoutBuilder(B_HORIZONTAL, 5)
        	.Add(fSlider)
        	.Add(fValueLabel)
        );
	else
		builder.Add(fSlider);


    AddChild(
        builder.AddGlue()
        .Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
            .AddGlue()
            .Add(cancelButton)
            .AddGlue()
            .Add(okayButton)
            .AddGlue()
        )
        .SetInsets(5, 5, 5, 5)
    );
}


void
ScaleDialog::InitControls()
{
    Lock();
    
    fSlider->SetValue(fValue);
    fSlider->SetKeyIncrementValue(fStep);
    fSlider->MakeFocus(true);
    
    CenterOnScreen();
    
    Unlock();
}

void
ScaleDialog::MessageReceived(BMessage* message)
{
    switch (message->what) {
        case MSG_OK_CLICKED:
        {
        	BString val;
        	val << fSlider->Value();
        	
        	fprintf(stdout, "%s\n", val.String());
        
            be_app->PostMessage(MSG_OK_CLICKED);
            Quit();
        
            break;
        }
        
        case MSG_CANCEL_CLICKED:
        
            be_app->PostMessage(MSG_CANCEL_CLICKED);
            Quit();
            
            break;
            
        case 'SlDE':
    	{
    		int32 newValue = fSlider->Value();
    		
    		// Avoid repeating same numbers
    		if (fValue != newValue) {
    			fValue = newValue;
    		} else {
    			return;
    		}
    		
    		BString val;
    	
    		val << fValue;
    		
    		
        	if (!fHideValue)
    			fValueLabel->SetText(val.String());
        	
        	if (fPrintPartial)
        		fprintf(stdout, "%s\n", val.String());
        	
        	break;
		}    
		
        default:
            Dialog::MessageReceived(message);
            break;
    }
}


