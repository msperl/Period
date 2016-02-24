/*
 * Program:     Period98
 *
 * File:        fgeneral.h
 * Purpose:     definition of labels of general interest
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __fgeneral_h
#define __fgeneral_h

// Formats

#define FORMAT_NUMBER         "F%-3i"
#define FORMAT_FREQUENCY      "%.9g"
#define FORMAT_AMPLITUDE      "%.9g"
#define FORMAT_PHASE          "%.6g"
#define FORMAT_ZEROPOINT      "%.9g"
#define FORMAT_RESIDUALS      "%.9g"
#define FORMAT_WEIGHTS        "%.6g"
#define FORMAT_COLOR          "%s"
#define FORMAT_TIME           "%.12g"

#define FORMAT_ACTIVE_FREQUENCIES "%i"

#define FIXEDFORMAT_NUMBER    "F%-3i"
#define FIXEDFORMAT_FREQUENCY "%15.9g"
#define FIXEDFORMAT_COMPO     "%15s"
#define FIXEDFORMAT_AMPLITUDE "%12.9g"
#define FIXEDFORMAT_PHASE     "%8.6g"
#define FIXEDFORMAT_ZEROPOINT "%12.9g"
#define FIXEDFORMAT_RESIDUALS "%12.9g"
#define FIXEDFORMAT_WEIGHTS   "%8.6g"
#define FIXEDFORMAT_TIME      "%15.12g"

#define FORMAT_PERIOD_PROTOCOL_COMPO \
                  FIXEDFORMAT_NUMBER\
                  " %c"FIXEDFORMAT_COMPO    "%c" \
                  " %c"FIXEDFORMAT_AMPLITUDE"%c" \
                  " %c"FIXEDFORMAT_PHASE    "%c" \
                  " "  FIXEDFORMAT_FREQUENCY
#define FORMAT_PERIOD_PROTOCOL \
                  FIXEDFORMAT_NUMBER\
                  " %c"FIXEDFORMAT_FREQUENCY"%c " \
                  " %c"FIXEDFORMAT_AMPLITUDE"%c " \
                  " %c"FIXEDFORMAT_PHASE    "%c"

#define PROTOCOL_ZEROPOINT    "Zeropoint:  "FORMAT_AMPLITUDE

// Other definition

#define ALL_FILE_PATTERN       "All files(*.*)|*.*"

#define DIALOG_CANT_WRITE_FILE     "Cannot write to file %s"
#define DIALOG_FILE_DOES_NOT_EXIST "File %s does not exist"
#define DIALOG_FILE_READ_ERROR     "Can not read from file %s" 
#define DIALOG_UNKNOWN_KEYWORD     "Unknown Project-keyword in %s-part:\n%s"

#define DIALOG_USE_WEIGHTS         "Should I use the given weights for calculation?"

#endif


