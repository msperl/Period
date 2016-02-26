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

// Load frequencies
#define PROTOCOL_LOADED_FREQUENCIES_FROM "Loaded a new set of frequencies\n"\
                                         "from file: %s\n"\
                                         "The loaded frequencies are:"
// Save frequencies
#define PROTOCOL_SAVED_FREQUENCIES_TO    "Saved frequencies in file %s"

// Epoch
#define PROTOCOL_EPOCH_FORMAT_HEADER "Freq. Frequency         Epoch"
#define PROTOCOL_EPOCH_FORMAT "%5s %17s "FIXEDFORMAT_EPOCH
#define PROTOCOL_EPOCH_INTENSITY "Calculating the epoch in intensity "\
                                 "for the active frequencies\n"\
                                 "for the following time: "FORMAT_TIME
#define PROTOCOL_EPOCH_MAGNITUDE "Calculating the epoch in magnitudes "\
                                 "for the active frequencies\n"\
                                 "for the following time: "FORMAT_TIME

// Change frequency
#define PROTOCOL_CHANGE_FREQUENCY "Changed frequency %i by %i steps of %g\n"\
                                  "from "FORMAT_FREQUENCY" to "FORMAT_FREQUENCY
// Predict
#define PROTOCOL_PREDICT "Predicted signal for time "FORMAT_TIME" is "FORMAT_AMPLITUDE

// Create artificial data

#define PROTOCOL_ARTIFICIAL_DATA_APPEND "Appending artificial data\nto file %s\n"\
                     "from: "FORMAT_TIME"\n"\
                     "to: "FORMAT_TIME"\n"\
                     "Stepsize: "FORMAT_TIME"\n"\
                     "Leading/trailing: "FORMAT_TIME

#define PROTOCOL_ARTIFICIAL_DATA_CREATE "Creating artificial data\nin file %s\n"\
                     "from: "FORMAT_TIME"\n"\
                     "to: "FORMAT_TIME"\n"\
                     "Stepsize: "FORMAT_TIME"\n"\
                     "Leading/trailing: "FORMAT_TIME

// calculate
#define PROTOCOL_PERIOD_CALCULATE_HEADER "Period calculation\n"\
                                         "Input:"
#define PROTOCOL_AMPVAR_CALCULATE_HEADER "Period amplitude/phase calculation\n"\
                                         "Input:"
#define PROTOCOL_PERIOD_USE_WEIGHTS      "Using weighted data."
#define PROTOCOL_PERIOD_USE_OBSERVED     "Using original data."
#define PROTOCOL_PERIOD_USE_ADJUSTED     "Using adjusted data."

#define PROTOCOL_PERIOD_CALCULATION_STARTED "Calculations started on:"

#define PROTOCOL_PERIOD_OUTPUT           "Output:"

#define PROTOCOL_PERIOD_RESULT           PROTOCOL_ZEROPOINT"\n"\
                                         "Residuals:  "FORMAT_AMPLITUDE"\n"\
                                         "Iterations: %i"

#define PROTOCOL_AMPVAR_WIDTH 72
#define PROTOCOL_AMPVAR_NAMES_SELECTED   "Names are selected from:"

#define PROTOCOL_AMPVAR_NOVAR         "%7s Fre = %12s Amp = %12.6g Pha = %12.6g"

#define PROTOCOL_AMPVAR_AMPVAR_HEAD   "%7s Fre = %12s                    Pha = %12.6g"
#define PROTOCOL_AMPVAR_AMPVAR        "        Name= %12s Amp = %12.6g"

#define PROTOCOL_AMPVAR_PHASEVAR_HEAD "%7s Fre = %12s Amp = %12.6g"
#define PROTOCOL_AMPVAR_PHASEVAR      "        Name= %12s                    Pha = %12.6g"

#define PROTOCOL_AMPVAR_ALLVAR_HEAD   "%7s Fre = %12s"
#define PROTOCOL_AMPVAR_ALLVAR        "        Name= %12s Amp = %12.6g Pha = %12.6g"


//
// the dialog part
//

// Load frequencies
#define DIALOG_LOAD_FREQUENCIES_TITLE "Load frequencies"

// Save frequencies
#define DIALOG_SAVE_FREQUENCIES_TITLE "Save frequencies"

// Ask user if he wants to use the weights
#define DIALOG_PERIOD_USE_WEIGHTS "Shall I use the given weights for the calculation?"

#define DIALOG_PERIOD_MAXIMUM_ITERATIONS "The maximum number of iterations\n"\
                                         "has been reached during calculations.\n"\
                                         "The result may not be reliable.\n"\
                                         "Please retry the calculations."
#define DIALOG_PERIOD_MATRIX_INVERSION   "Problem calculating\n"\
                                         "matrix cannot be inverted."
#define DIALOG_PERIOD_CALCULATION_INTERRUPTED \
                                         "Calculation has been interrupted\n"\
                                         "The calculated values may not be reliable."

#define DIALOG_PERIOD_NOT_ENOUGH_POINTS "The number of selected points\n"\
                                        "in the timestring is to low..."

#define DIALOG_PERIOD_NO_FREQUENCIES    "There are no active frequencies"

// file extentions

#define DIALOG_PERIOD_EXTENTION  "per"
#ifdef wx_msw
#define DIALOG_PERIOD_PATTERN    "Period files(*.per)|*.per|"ALL_FILE_PATTERN
#else
#define DIALOG_PERIOD_PATTERN    "*.per"
#endif
