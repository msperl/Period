/*
 * Program:     Period98
 *
 * File:        fproject.h
 * Purpose:     definition of labels 
 *              for the period part 
 *              of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "fgeneral.h"

// the protocol part

// Save Logfile
#define LOG_SAVE "Save logfile as:"

// file extentions

#define DIALOG_LOG_EXTENTION  "txt"
#ifdef wx_msw
#define DIALOG_LOG_PATTERN    "log files(*.txt)|*.txt|"ALL_FILE_PATTERN
#else
#define DIALOG_LOG_PATTERN    "*.txt"
#endif
