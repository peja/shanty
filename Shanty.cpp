/*
 * Copyright 2010, Milos Pejovic. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <app/Application.h>
#include <interface/Alert.h>
#include <storage/Path.h>
#include <storage/FilePanel.h>
#include <storage/File.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <sys/select.h>


#include "shanty_help.h"

#define SHANTY_VERSION "0.1"

const char* kSignature = "application/x-vnd.Shanty";

char* kOk = "OK";
char* kCancel = "Cancel";
char* kYes = "Yes";
char* kNo = "No";
char* kClose = "Close";
char* kInfoTitle = "Information";
char* kWarningTitle = "Warning";
char* kErrorTitle = "Error";
char* kQuestionTitle = "Question";
char* kInfoText = "All updates are complete.";
char* kWarningText = "Are you sure you want to proceed?";
char* kErrorText = "An error has occurred.";

//
// Read the input in a non-blocking way.
//
// Returns -1 when there is no more data to read, on error, and if
// the thread receives a message, which indicates that the reading must stop.
//
// Otherwise returns number of read bytes.
ssize_t
read_stdin_nonblocking(char* str, size_t size)
{
	static fd_set rfds, rfdsCopy;
	static struct timeval tv, tvCopy;

	// Read from stdin (fd 0).
	
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Wait up to a second.
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	
	for (;;) {
		// Quit if there is a message in the cache.
		if (has_data(find_thread(NULL)))
			break;
			
		// Make copies of structures, because 'select' modifies them.
		rfdsCopy = rfds;
		tvCopy = tv;
		
		if (select(1, &rfdsCopy, NULL, NULL, &tvCopy) == -1)
        	break;
        	
    	if (FD_ISSET(0, &rfds)) {
    		ssize_t n = read(0, str, size);
    		
    		if (n <= 0) break;
    		
    		return n;
    	}
	}
	
	return -1;
}



#include "Dialog.h"
#include "EntryDialog.h"
#include "CalendarDialog.h"
#include "TextInfoDialog.h"
#include "ScaleDialog.h"
#include "ProgressDialog.h"

enum ResponseTypes { kNone, kInfo, kCalendar, kEntry, kError, kFileSelection,
                     kList, kNotification, kProgress, kQuestion, kWarning,
                     kScale, kTextInfo, kHelp, kAbout, kVersion };
                     
enum Parameters { kDummy=2, kTitle, kWidth, kHeight, kTimeout, kText, kDay,
                  kMonth, kYear, kDateFormat, kEntryText, kFilename,
                  kSeparator, kColumn, kPrintColumn, kHideColumn,
                  kPercentage, kValue, kMinValue, kMaxValue, kStep,
                  kOkLabel, kCancelLabel };

class Shanty : public BApplication {
    public:
        Shanty();
        virtual ~Shanty() { }

        virtual void ReadyToRun();
        virtual void ArgvReceived(int32 argc, char** argv);

		bool AutoKill() const {
			return fAutoKill && fResponseType == kProgress && fReturnValue == 1;
		}
        int32 ReturnValue() const { return fReturnValue; }
        
        void MessageReceived(BMessage* message);
        void RefsReceived(BMessage* message);

    private:
        void _Usage() const;
        void _Version() const;
        void _About() const;

    private:
        int32 fReturnValue;
        Dialog* fDialog;
        
        char* fTitle;
        char* fText;
        char* fFilename;
        char* fSeparator;
        char* fEntryText;
        char* fDateFormat;
        char* fOkLabel;
        char* fCancelLabel;
        
        int fResponseType;
        int fNoWrap;
        int fMultiple;
        int fDirectory;
        int fSave;
        int fChecklist;
        int fRadiolist;
        int fEditable;
        int fListen;
        int fPulsate;
        int fAutoClose;
        int fAutoKill;
        int fNoCancel;
        int fPrintPartial;
        int fHideValue;
        int fHideText;
        float fWidth;
        float fHeight;
        int fDay;
        int fMonth;
        int fYear;
    	int32 fValue;
    	int32 fMinValue;
    	int32 fMaxValue;
	    int32 fStep;
	    int fPercentage;
};

Shanty::Shanty()
    :
    BApplication(kSignature),
    
    fReturnValue(0),
    fDialog(NULL),
    
    fText(NULL),
    fTitle(NULL),
    fFilename(NULL),
    fSeparator(NULL),
    fEntryText(NULL),
    fDateFormat(NULL),
    fOkLabel(kYes),
    fCancelLabel(kNo),
    
    fResponseType(kNone),
    fNoWrap(0),
    fMultiple(0),
    fDirectory(0),
    fSave(0),
    fChecklist(0),
    fRadiolist(0),
    fEditable(0),
    fListen(0),
    fPulsate(0),
    fAutoClose(0),
    fAutoKill(0),
    fNoCancel(0),
    fPrintPartial(0),
    fHideValue(0),
    fHideText(0),
    fWidth(0.0),
    fHeight(0.0),
    fDay(0),
    fMonth(0),
    fYear(0),
    fValue(0),
    fMinValue(0),
    fMaxValue(0),
    fStep(0),
	fPercentage(0)
{
}


/*
    Invoked when the application receives a B_ARGV_RECEIVED message.
    The message is sent when command line arguments are used in launching the
    app from the shell. The function isn't called if there were no command
    line arguments.
*/
void
Shanty::ArgvReceived(int32 argc, char** argv)
{
    int c;
    
  
  while (1) {
      static struct option long_options[] =
        {
          // These options do not require arguments
          
          {"info",             no_argument,       &fResponseType, kInfo},
          {"calendar",         no_argument,       &fResponseType, kCalendar},
          {"entry",            no_argument,       &fResponseType, kEntry},
          {"file-selection",   no_argument,       &fResponseType, kFileSelection},
          {"list",             no_argument,       &fResponseType, kList},
          {"notification",     no_argument,       &fResponseType, kNotification},
          {"progress",         no_argument,       &fResponseType, kProgress},
          {"question",         no_argument,       &fResponseType, kQuestion},
          {"warning",          no_argument,       &fResponseType, kWarning},
          {"error",            no_argument,       &fResponseType, kError},
          {"scale",            no_argument,       &fResponseType, kScale},
          {"text-info",        no_argument,       &fResponseType, kTextInfo},
          
          {"about",            no_argument,       &fResponseType, kAbout},
          {"version",          no_argument,       &fResponseType, kVersion},
          
          {"no-wrap",          no_argument,       &fNoWrap, 1},
          {"multiple",         no_argument,       &fMultiple, 1},
          {"directory",        no_argument,       &fDirectory, 1},
          {"save",             no_argument,       &fSave, 1},
          {"checklist",        no_argument,       &fChecklist, 1},
          {"radiolist",        no_argument,       &fRadiolist, 1},
          {"editable",         no_argument,       &fEditable, 1},
          {"listen",           no_argument,       &fListen, 1},
          {"pulsate",          no_argument,       &fPulsate, 1},
          {"auto-close",       no_argument,       &fAutoClose, 1},
          {"auto-kill",        no_argument,       &fAutoKill, 1},
          {"no-cancel",        no_argument,       &fNoCancel, 1},
          {"print-partial",    no_argument,       &fPrintPartial, 1},
          {"hide-value",       no_argument,       &fHideValue, 1},
          {"hide-text",        no_argument,       &fHideText, 1},
          
          {"help",             no_argument,       0, 'h'},
          
          
          // These options require arguments          
          
          {"title",           required_argument, 0, kTitle},
          {"width",           required_argument, 0, kWidth},
          {"height",          required_argument, 0, kHeight},
          {"timeout",         required_argument, 0, kTimeout},
          {"text",            required_argument, 0, kText},
          {"day",             required_argument, 0, kDay},
          {"month",           required_argument, 0, kMonth},
          {"year",            required_argument, 0, kYear},
          {"date-format",     required_argument, 0, kDateFormat},
          {"entry-text",      required_argument, 0, kEntryText},
          {"filename",        required_argument, 0, kFilename},
          {"separator",       required_argument, 0, kSeparator},
          {"column",          required_argument, 0, kColumn},
          {"print-column",    required_argument, 0, kPrintColumn},
          {"hide-column",     required_argument, 0, kHideColumn},
          {"percentage",      required_argument, 0, kPercentage},
          {"value",           required_argument, 0, kValue},
          {"min-value",       required_argument, 0, kMinValue},
          {"max-value",       required_argument, 0, kMaxValue},
          {"step",            required_argument, 0, kStep},
          {"ok-label",        required_argument, 0, kOkLabel},
          {"cancel-label",    required_argument, 0, kCancelLabel},
          
          
          // Dummy options
          
          {"confirm-overwrite",    no_argument, 0, kDummy},
          {"file-filter",    required_argument, 0, kDummy},
          {"window-icon",    required_argument, 0, kDummy},
          {"class",          required_argument, 0, kDummy},
          {"name",           required_argument, 0, kDummy},
          {"screen",         required_argument, 0, kDummy},
          {"gtk-module",     required_argument, 0, kDummy},
          {"sync",                 no_argument, 0, kDummy},
          {"g-fatal-warnings",     no_argument, 0, kDummy},
          {"display",        required_argument, 0, kDummy},
          
          
          {0, 0, 0, 0}
        };
        
      // getopt_long stores the option index here.
      int option_index = 0;

      c = getopt_long (argc, argv, "h",
                       long_options, &option_index);

      // Detect the end of the options.
      if (c == -1)
        break;

      switch (c) {
        case 0:
         
          break;

        case kTitle:
                fTitle = strdup(optarg);
                break;
                
        case kText:
                fText = strdup(optarg);
                break;
        
        case kFilename:
        		fFilename = strdup(optarg);
                break;
                
        case kSeparator:
        		fSeparator = strdup(optarg);
                break;
                
        case kEntryText:
        		fEntryText = strdup(optarg);
        		break;
        
        case kWidth:
        		fWidth = atof(optarg);
        			
        		break;
        	
        case kHeight:
        		fHeight = atof(optarg);
        		
        		break;
        		
        case kDateFormat:
        		fDateFormat = strdup(optarg);
        		
        		break;
        		
        case kDay:
        		fDay = atoi(optarg);
        		
        		break;
        		
        case kMonth:
        		fMonth = atoi(optarg);
        		
        		break;
        		
        case kYear:
        		fYear = atoi(optarg);
        		
        		break;
        		
        case kValue:
        		fValue = atoi(optarg);
        		
        		break;
        		
        case kMinValue:
        		fMinValue = atoi(optarg);
        		
        		break;
        		
        case kMaxValue:
        		fMaxValue = atoi(optarg);
        		
        		break;
        		
        case kStep:
        		fStep = atoi(optarg);
        		
        		break;
        		
        case kPercentage:
        		fPercentage = atoi(optarg);
        		
        		break;
        		
       	case kOkLabel:
       			fOkLabel = strdup(optarg);
       			
       			break;
       			
       	case kCancelLabel:
       			fCancelLabel = strdup(optarg);
       			
       			break;
        
        case kTimeout:
        case kColumn:
        case kPrintColumn:
        case kHideColumn:
        
        
        case kDummy: // Do nothing
            break;

        case 'h':
          fResponseType = kHelp;
          break;
          
        default:
          // get_opts_long displays the error message
        
          fReturnValue = -1;
          break;

        }
    }

}


void
Shanty::_Usage() const
{
    printf(usage_text);
}

void
Shanty::_Version() const
{
    printf("%s\n", SHANTY_VERSION);
}

void
Shanty::_About() const
{
    printf("Shanty %s\n", SHANTY_VERSION);
    printf("\nDisplay dialog boxes from scripts\n");
}


/*
    Is called when the app receives a B_READY_TO_RUN message. The message
    is sent automatically during the Run() function, and is sent after the
    initial B_REFS_RECEIVED and B_ARGV_RECEIVED messages (if any) have been
    handled.
*/
void
Shanty::ReadyToRun()
{ 
    if (fReturnValue != -1) {
        
        bool doAlert = false;
        alert_type fIcon = B_INFO_ALERT;
        
        switch (fResponseType) {
            
            case kInfo:
                if (fTitle == NULL)
                    fTitle = kInfoTitle;
                    
                if (fText == NULL)
                    fText = kInfoText;
                
                doAlert = true;
                
                break;
                    
            case kWarning:
                if (fTitle == NULL)
                    fTitle = kWarningTitle;
                    
                if (fText == NULL)
                    fText = kWarningText;
                
                fIcon = B_WARNING_ALERT;
                doAlert = true;
                
                break;
            
            case kError:
                if (fTitle == NULL)
                    fTitle = kErrorTitle;
                    
                if (fText == NULL)
                    fText = kErrorText;
                
                fIcon = B_STOP_ALERT;
                doAlert = true;
                
                break;
            
            case kQuestion:
            {    
                if (fTitle == NULL)
                    fTitle = kQuestionTitle;
                
                if (fText == NULL)
                    fText = kWarningText;
                
                BAlert* alert = new BAlert(fTitle, fText,
                                           fCancelLabel, fOkLabel, NULL,
                                           B_WIDTH_AS_USUAL, B_OFFSET_SPACING,
                                           B_WARNING_ALERT);
                                           
                alert->SetShortcut(0, B_ESCAPE);
                
                fReturnValue = !alert->Go();
                
                Quit();
                
                break;
            }
            
            case kHelp:
                    _Usage();
                    
                    Quit();
                    
                    break;
            
            case kAbout:
            		_About();
            		
            		Quit();
            		
            		break;
            		
            case kVersion:
            		_Version();
            		
            		Quit();
            		
            		break;
            		
            case kFileSelection:
            {
            		bool multiple = !fSave && fMultiple;
            		uint32 flavor = (fDirectory ? B_DIRECTORY_NODE : B_FILE_NODE );
            		file_panel_mode mode = (fSave ? B_SAVE_PANEL : B_OPEN_PANEL);
            		
            		BFilePanel* panel = new BFilePanel(mode, NULL, NULL, flavor, multiple);        
    
    				BEntry entry;
    
    				if (entry.SetTo(fFilename) == B_OK) {
    
        				if (!entry.IsDirectory()) {
        					if (fSave) {
        						char name[B_FILE_NAME_LENGTH];
        						entry.GetName(name);
        						panel->SetSaveText(name);
        					}
        					
            				entry.GetParent(&entry);
        				}
        
        				panel->SetPanelDirectory(&entry);
    				}
    				
    				if (fTitle != NULL)
    					panel->Window()->SetTitle(fTitle);
    
    				panel->Show();
            	
            		break;
            }
            
            case kEntry:
            {
            	fDialog = new EntryDialog(fTitle, fWidth, fHeight, fText, fEntryText, fHideText);
            	
            	fDialog->Show();
            	
            	break;
            }
            
            case kCalendar:
            {
            	fDialog = new CalendarDialog(fTitle, fWidth, fHeight, fText,
            										fDay, fMonth, fYear, fDateFormat);
            	
            	fDialog->Show();
            	
            	break;
            }
            
            case kTextInfo:
        	{
        		fDialog = new TextInfoDialog(fTitle, fWidth, fHeight, fFilename, fEditable);
            	
            	fDialog->Show();
            	
            	break;
        	}
        	
        	case kScale:
        	{
        		fDialog = new ScaleDialog(fTitle, fWidth, fHeight, fText,
        								  fValue, fMinValue, fMaxValue, fStep,
        								  fPrintPartial, fHideValue);
        		
            	fDialog->Show();
            	
            	break;
        	}
        	
        	case kProgress:
        	{
        		fDialog = new ProgressDialog(fTitle, fWidth, fHeight, fText,
        									 fPercentage, fPulsate, fAutoClose, fNoCancel);
        									 
        		fDialog->Show();
        		
        		break;
        	}
        	
            case kList:
            case kNotification:
            {	
            	fprintf(stderr, "Bummer! Option is not yet implemented...\n");
            	
            	fReturnValue = -1;
            	
            	Quit();
            	
            	break;
            }
            
            case kNone: //Error
            default:
            	fprintf(stderr, "You need to specify a dialog type. Check --help\n");
            
            	fReturnValue = -1;
            	
            	Quit();
            
            break;
        }
        
        if (doAlert) {
            BAlert* alert = new BAlert(fTitle, fText,
                kOk, NULL, NULL,
                B_WIDTH_AS_USUAL, fIcon);

            alert->Go();
            
            Quit();
        }
    } else {
    
    	Quit();
    }
}



void
Shanty::RefsReceived(BMessage* message)
{
	int32        refNum;
	entry_ref    ref;
	BPath        path;
    BEntry       entry;
    const char*  separator;
	
	separator = (fSeparator != NULL ? fSeparator : "|");
	
	for (refNum = 0 ;; refNum++) {
		if (message->FindRef("refs", refNum, &ref) != B_OK)
			break;
		else {
			if (entry.SetTo(&ref) != B_OK) {
				fReturnValue = -1;
				return;
			}
			
    		entry.GetPath(&path);
    		
    		if (refNum == 0) {
    			fprintf(stdout, "%s", path.Path());
    			
    			if (!fMultiple)
    				break;
    				
    		} else {
    			fprintf(stdout, "%s%s", separator, path.Path());
    		}
		}
	}
	
	fprintf(stdout, "\n");
    		
    Quit();
}


void
Shanty::MessageReceived(BMessage* message)
{
    switch (message->what) {
    	
    	case B_SAVE_REQUESTED:
    	{
    		entry_ref    ref;
    		const char*  name;
    		BPath        path;
    		BEntry       entry;
    		
    		if (message->FindRef("directory", &ref) == B_OK &&
    			message->FindString("name", &name) == B_OK &&
    		    entry.SetTo(&ref) == B_OK) {
    		    	
    		    entry.GetPath(&path);
    		    path.Append(name);
    		    
    			fprintf(stdout, "%s\n", path.Path());    
    		    
    		} else {
    			fReturnValue = -1;
    		}
    		
    		Quit();
    		break;
    	}
    	
    	case B_CANCEL: case MSG_CANCEL_CLICKED:
    		fReturnValue = 1;
    		
    		if (fResponseType != kFileSelection) {
    			fDialog->Lock();
    			fDialog->Quit();
    		}
    	
    		Quit();
    		
    		break;
    		
    	case MSG_OK_CLICKED:
    		fDialog->Lock();
	    	fDialog->Quit();
	    	
    		Quit();
    		
    		break;
    		
    	default:
    		BApplication::MessageReceived(message);
    }
}


int
main(int argc, char** argv)
{
    Shanty app;
    if (app.InitCheck() != B_OK)
        return -1;

    app.Run();

	if (app.AutoKill())
		kill(getppid(), SIGHUP);

    return app.ReturnValue();
}
