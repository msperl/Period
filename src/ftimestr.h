/*
 * Program:     Period98
 *
 * File:        ftimestr.h
 * Purpose:     definition of labels 
 *              for the time string part 
 *              of the project
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "fgeneral.h"

//
// the protocol part
//

// Load time string
#define PROTOCOL_APPEND_TIMESTRING \
                          "Appended %i points of data from file: %s\n"\
                          "in the format: %s\n"
#define PROTOCOL_LOAD_TIMESTRING \
                          "Loaded %i points of data from file: %s\n"\
                          "in the format: %s\n"
// Save time string
#define PROTOCOL_SAVE_TIMESTRING \
                          "Saved %i points to file: %s\n"\
                          "in the format: %s"

#define PROTOCOL_SAVE_PHASESTRING \
                          "Saved %i points to file: %s\n"\
                          "with a frequency of "FORMAT_FREQUENCY\
                          "used to calculate the phases\n"\
                          "in the format: %s"

#define PROTOCOL_SAVE_PHASESTRINGBINNED \
                          "Saved %i binned points to file: %s\n"\
                          "with a frequency of "FORMAT_FREQUENCY\
                          "used to calculate the phases\n"\
                          "and a binned spacing of:"FORMAT_FREQUENCY"\n"\
                          "using: %i as amplitudes"

// Selection
#define PROTOCOL_SELECTED_FROM   "Selected from %s:"
#define PROTOCOL_SELECTED_POINTS "%i out of %i points selected"

// Subdivide
#define PROTOCOL_SUBDIVIDE_CURRENT_SELECTION \
                                 "Subdivided the current selection\n"\
                                 "In column:        %s\n"\
                                 "By gaps of:       "FORMAT_TIME"\n"\
                                 "using the prefix: %s"
#define PROTOCOL_SUBDIVIDE_COUNTER "with a running counter."
#define PROTOCOL_SUBDIVIDE_TIME    "with time."

// Rename selection

#define PROTOCOL_RENAME "Renamed current selection to %s in column %s"

// Refit

#define PROTOCOL_REFIT "Recalculating  the residuals for the\n"\
                       "currently selected data points\n"\
                       "according to the following fit:\n"

#define PROTOCOL_RESIDUALS "Residuals: "FORMAT_AMPLITUDE

#define PROTOCOL_SCALE_REVERSE "The time string is in magnitudes"
#define PROTOCOL_SCALE_NORMAL "The time string is in intensity"

// make observed adjusted

#define PROTOCOL_RESTORE_ORIGINAL_VALUES \
                             "Restoring original data for %i points."

// Set name sets

#define PROTOCOL_CHANGE_HEADING \
                       "Renamed the attribute heading:\n"\
                       "from %20s to %20s\n"

// Adjust

#define PROTOCOL_ADJUST_ORIGINAL \
                   "Adjust the selected data of column %s\n"\
                   "with the adjustments to the original data."
#define PROTOCOL_ADJUST_WEIGHTED_ORIGINAL \
                   "Adjust the selected data of column %s\n"\
                   "with the weighted adjustments to the original data."
#define PROTOCOL_ADJUST_ADJUSTED \
                   "Adjust the selected data of column %s\n"\
                   "with the adjustments to the adjusted data."
#define PROTOCOL_ADJUST_WEIGHTED_ADJUSTED \
                   "Adjust the selected data of column %s\n"\
                   "with the weighted adjustments to the adjusted data."
#define PROTOCOL_ADJUST_POINTS_ADJUSTED \
                   "Adjusted the %i points of %s by "FORMAT_AMPLITUDE

// change name and weights
#define PROTOCOL_CHANGE_NAME \
                   "Changed name and weights in %s\n"\
                   "from %s ("FORMAT_WEIGHTS","FORMAT_COLOR") "\
                   "to %s ("FORMAT_WEIGHTS","FORMAT_COLOR")"

// relabel datapoint
#define PROTOCOL_RELABEL_POINT "Relabel datapoint:\n"\
                               "Time:     "FORMAT_TIME"\n"\
                               "Observed: "FORMAT_AMPLITUDE\
                               "Adjusted: "FORMAT_AMPLITUDE
#define PROTOCOL_RELABEL_FROM_TO "from %s to %s"

#define PROTOCOL_WEIGHT_NAME_ON  "Weights are calculated using the weights from the attributes"
#define PROTOCOL_WEIGHT_NAME_OFF "Weights are calculated ignoring the weights from the attributes"

#define PROTOCOL_WEIGHT_POINT_ON  "Weights are calculated using the individual weight for each point"
#define PROTOCOL_WEIGHT_POINT_OFF "Weights are calculated ignoring the individual weight for each point"

#define PROTOCOL_ERROR_SCALE "The error scale has been set to: "FORMAT_AMPLITUDE

//
// the dialog part
//

// Load frequencies
#define DIALOG_LOAD_TIMESTRING_TITLE   "Load time string"
#define DIALOG_APPEND_TIMESTRING_TITLE "Append time string"

// Save frequencies
#define DIALOG_SAVE_TIMESTRING_TITLE "Save time string"

// Save frequencies
#define DIALOG_SAVE_PHASESTRING_TITLE "Save phase string"

// Adjust
#define DIALOG_ADJUST_NEWFIT "Please calculate a new frequency fit"
#define DIALOG_ADJUST_ADJUSTED_NEW_FIT DIALOG_ADJUST_NEWFIT"." 
#define DIALOG_ADJUST_ORIGINAL_NEW_FIT DIALOG_ADJUST_NEWFIT\
                             " using the adjusted values." 

#define DIALOG_ADJUST_SAVE_STAT "Export subset statistics"
#define DIALOG_ADJUST_FORMAT "%-20s%13f%13f%10i %3s"
#define DIALOG_ADJUST_HEADER "Name                      Average        Sigma    Points  Adj"
#define DIALOG_ADJUST_YES  "yes"
#define DIALOG_ADJUST_NO   "no"

//
// file extentions
//
#define DIALOG_TIMESTRING_EXTENTION  "dat"
#ifdef wx_msw
#define DIALOG_TIMESTRING_PATTERN    "Time string files(*.dat)|*.dat|"\
                                     ALL_FILE_PATTERN
#else
#define DIALOG_TIMESTRING_PATTERN    "*.dat"
#endif

#define DIALOG_PHASESTRING_EXTENTION  "pha"
#ifdef wx_msw
#define DIALOG_PHASESTRING_PATTERN    "Phase string files(*.pha)|*.pha|"\
                                     ALL_FILE_PATTERN
#else
#define DIALOG_PHASESTRING_PATTERN    "*.pha"
#endif
