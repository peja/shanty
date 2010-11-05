const char* usage_text =
"Usage:\n"
"  shanty [OPTION...]\n"
"\n"
"Help options\n"
"  -h, --help                  Show help options\n"
"\n"
"General options\n"
"  --title=TITLE               Set the dialog title\n"
"  --width=WIDTH               Set the width\n"
"  --height=HEIGHT             Set the height\n"
"  --timeout=TIMEOUT           Set dialog timeout in seconds\n"
"\n"
"Calendar options\n"
"  --calendar                  Display calendar dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --day=DAY                   Set the calendar day\n"
"  --month=MONTH               Set the calendar month\n"
"  --year=YEAR                 Set the calendar year\n"
"  --date-format=PATTERN       Set the format for the returned date\n"
"\n"
"Text entry options\n"
"  --entry                     Display text entry dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --entry-text=TEXT           Set the entry text\n"
"  --hide-text                 Hide the entry text\n"
"\n"
"Error options\n"
"  --error                     Display error dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"\n"
"Info options\n"
"  --info                      Display info dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"\n"
"File selection options\n"
"  --file-selection            Display file selection dialog\n"
"  --filename=FILENAME         Set the filename\n"
"  --multiple                  Allow multiple files to be selected\n"
"  --directory                 Activate directory-only selection\n"
"  --save                      Activate save mode\n"
"  --separator=SEPARATOR       Set output separator character\n"
"\n"
"List options\n"
"  --list                      Display list dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --column=COLUMN             Set the column header\n"
"  --checklist                 Use check boxes for first column\n"
"  --radiolist                 Use radio buttons for first column\n"
"  --separator=SEPARATOR       Set output separator character\n"
"  --multiple                  Allow multiple rows to be selected\n"
"  --editable                  Allow changes to text\n"
"  --print-column=NUMBER       Print a specific column (Default is 1. 'ALL' can be used to print all columns)\n"
"  --hide-column=NUMBER        Hide a specific column\n"
"\n"
"Notification icon options\n"
"  --notification              Display notification\n"
"  --text=TEXT                 Set the notification text\n"
"  --listen                    Listen for commands on stdin\n"
"\n"
"Progress options\n"
"  --progress                  Display progress indication dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --percentage=PERCENTAGE     Set initial percentage\n"
"  --pulsate                   Pulsate progress bar\n"
"  --auto-close                Dismiss the dialog when 100%% has been reached\n"
"  --auto-kill                 Kill parent process if cancel button is pressed\n"
"  --no-cancel                 Hide the Cancel button\n"
"\n"
"Question options\n"
"  --question                  Display question dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --ok-label=TEXT             Set the label of the Ok button\n"
"  --cancel-label=TEXT         Set the label of the Cancel button\n"
"\n"
"Warning options\n"
"  --warning                   Display warning dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"\n"
"Scale options\n"
"  --scale                     Display scale dialog\n"
"  --text=TEXT                 Set the dialog text\n"
"  --value=VALUE               Set initial value\n"
"  --min-value=VALUE           Set minimum value\n"
"  --max-value=VALUE           Set maximum value\n"
"  --step=VALUE                Set step size\n"
"  --print-partial             Print partial values\n"
"  --hide-value                Hide value\n"
"\n"
"Text information options\n"
"  --text-info                 Display text information dialog\n"
"  --filename=FILENAME         Open file\n"
"  --editable                  Allow changes to text\n"
"\n"
"Miscellaneous options\n"
"  --about                     About shanty\n"
"  --version                   Print version\n"
"\n";
