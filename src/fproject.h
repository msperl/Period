/*
 * Program:     Period98
 *
 * File:        fproject.h
 * Purpose:     definition of labels for the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "fgeneral.h"

// general definitions
#define CONFIG_MAX_FREQUENCIES      256
#define CONFIG_DEFAULT_ALIAS_STEP   (1./365.)
#define CONFIG_DEFAULT_FOURIER_NAME "No fourier calculations until now"
#define CONFIG_DEFAULT_FOURIER_LOWER_FREQUENCY 0.0
#define CONFIG_DEFAULT_FOURIER_UPPER_FREQUENCY 50.0

// the protocol part

#define PROTOCOL_DATE "\nOn: "

// the dialog part

// save project 
#define DIALOG_PROJECT_UNEDITED   "The project has not been changed,\n"\
                                  "so no need to save it..."
#define DIALOG_PROJECT_SAVE_TITLE "Save project"

// delete project
#define DIALOG_DELETE_OLD_PROJECT "Are you sure you want to delete\n"\
                                  "the old Project?" 

// save old project first
#define DIALOG_SAVE_PROJECT_FIRST "Do you want to save the project first?"

// load project
#define DIALOG_PROJECT_LOAD_TITLE "Load project"
#define DIALOG_UNKNOWN_PROJECT_KEYWORD "Onknown project keyword: %s"
// file extentions

#define DIALOG_PROJECT_EXTENTION  "p98"
#ifdef wx_msw
#define DIALOG_PROJECT_PATTERN    "Period98 Project files(*.p98)|*.p98|"ALL_FILE_PATTERN
#else
#define DIALOG_PROJECT_PATTERN    "*.p98"
#endif




