/*
 * Program:     Period98
 *
 * File:        ldialtxt.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __ldialtxt_h__
#define __ldialtxt_h__

#include "version.h"
#include "lmenu.h"

////////////////////////////////////////////////////
// The Application name
////////////////////////////////////////////////////
#define APPNAME    "Period98"
#define VERSION    versiontxt
#define VERSIONDATE versiondatetxt
#define COMPILEDBY compiledbytxt
#define COMPILEDDATE compileddatetxt
#define AUTHOR     "Martin Sperl"
#define EMAIL      "sperl@dsn.astro.univie.ac.at"
#define COPYRIGHT  "(c) 1996-1998"

////////////////////////////////////////////////////
// General Settings
////////////////////////////////////////////////////

#define WARNING    "Warning:"
#define CALCULATE  "Calculate"
#define QUIT       "Quit"
#define CANCEL     "Cancel"
#define OKAY       "OK"

#define SIGNAL_PROG "Programm error encountered:\n"
#define SIGNAL_SAVE "\n"\
                    "Please save the project\n"\
                    "with a different name.\n"\
                    "Because while saving\n"\
                    "an error might occure,\n"\
                    "and the file may not contain\n"\
                    "correct data."
#define SIGNAL_UKN  SIGNAL_PROG\
                    "Unknown signal\n"\
                    SIGNAL_SAVE
#define SIGNAL_SEGV SIGNAL_PROG\
                    "Segmentation fault\n"\
                    SIGNAL_SAVE
#define SIGNAL_FPE  SIGNAL_PROG\
                    "Floating point error\n"\
                    SIGNAL_SAVE

////////////////////////////////////////////////////
// The mainframe settings
////////////////////////////////////////////////////

#define TIMESTRING "Time-string"
#define PERIODS    "Fit"
#define FOURIER    "Fourier"
#define PROTOCOL   "Log"
#define CALCULATING "Working, please wait..."

// things for menu handling

#define FREQ_ADJUST_HEAD  "Frequency adjustment:"
#define FREQ_ADJUST_QUEST "Change frequency adjustment"

#define ABOUT_HEAD "About "APPNAME
#define ABOUT_MSG  APPNAME" "\
                   "%s\n"\
                   "%s\n"\
                   "Author: "AUTHOR"\n"\
                   "("EMAIL")\n"\
                   COPYRIGHT"\n"\
                   "compiled by:\n"\
                   "%s\n"\
                   "%s"

#define MENU_NOTDEFINED "Sorry,\nthis menu-option has not\nbeen defined yet!!"
#define MENU_WARNING    "Menu warning"

////////////////////////////////////////////////////
// The wxproject settings
////////////////////////////////////////////////////

#define WXPROJ_WARNING WARNING

#define WXPROJ_FILEFOR "Please enter fileformat"
#define WXPROJ_FORMAT  "Format"
#define WXPROJ_FILEFORMAT "Please set the type of the columns you want to read in"
#define WXPROJ_FILESELECT "Select fields you want to write out"

#define WXPROJ_KEYWORD "Please enter keyword:"

////////////////////////////////////////////////////
// The myfile settings
////////////////////////////////////////////////////

#define MYFILE_DATA   "Display data"
#define MYFILE_GRAPH  "Display graph"
#define MYFILE_LOAD   "Import"
#define MYFILE_SAVE   "Export"

#define MYFILE_UKNBUT "No handler for\nspecial Buttons defined"
#define MYFILE_NOLOAD "There is no action \nimplemented for the\n"MYFILE_LOAD"-button"
#define MYFILE_NOSAVE "There is no action \nimplemented for the\n"MYFILE_SAVE"-button"
#define MYFILE_NOGRAP "There is no action \nimplemented for the\n"MYFILE_GRAPH"-button"
#define MYFILE_NODATA "There is no action \nimplemented for the\n"MYFILE_DATA"-button"

////////////////////////////////////////////////////
// The predict settings
////////////////////////////////////////////////////

#define PREDICT_DATE  "Predict signal at a given time"
#define PREDICT_START "First time in time-string: %g"
#define PREDICT_TIME  "enter time:"
#define PREDICT_AMPL  "The signal is:"
#define PREDICT_NOTAV "not calculated"
#define PREDICT_CALC  CALCULATE
#define PREDICT_QUIT  QUIT

#define PREDICT_ART   "Create artificial data"
#define PREDICT_BEG   "Start-time:"
#define PREDICT_END   "End-time:"
#define PREDICT_STEP  "Step:"
#define PREDICT_LEAD  "Leading/trailing time:"

#define PREDICT_NEW   "Create new file"
#define PREDICT_APP   "Append to file"
#define PREDICT_CANC  CANCEL

#define PREDICT_NOFRE "There are no frequencies selected!!!"
#define PREDICT_SAVE  "Save artificial data as:"
#define PREDICT_SAVAP "Append artificial data to:"

// here go the file-extentions - could be changed for Windows

#define PREDICT_EXT  "dat"
#define PREDICT_PAT  "*."PREDICT_EXT

////////////////////////////////////////////////////
// Use Data Section
////////////////////////////////////////////////////

#define USE          "Calculations based on:"

#define USE_WEIGHTS  "Use weights"

#define USE_ORIGDAT  "Original data"
#define USE_SHIFDAT  "Adjusted data"
#define USE_SPECTRAL "Spectral window"
#define USE_ORIGRES  "Residuals at original"
#define USE_SHIFRES  "Residuals at adjusted"

////////////////////////////////////////////////////
// The Noise settings
////////////////////////////////////////////////////

#define NOISE_CALCNOISE "Calculate noise"
#define NOISE_AT        "at what frequency:"
#define NOISE_AT_DEF    "10"
#define NOISE_BOX       "Box size:"
#define NOISE_BOX_DEF   "5"
#define NOISE_STEP      "Step rate:"
#define NOISE_STEP_DEF  "20"

#define NOISE_RES       "The noise is"
#define NOISE_NOTCOMP   "not computed yet..."

#define NOISE_FROM      "Frequencies from:"
#define NOISE_FROM_DEF  "0"
#define NOISE_TO        "to:"
#define NOISE_TO_DEF    "50"
#define NOISE_SPACE     "spacing:"
#define NOISE_SPACE_DEF "10"

#define NOISE_CAPTION "The noise calculation yields:"
#define NOISE_MESSAGE "Frequency   Noise"

#define NOISE_NOISE    FIXEDFORMAT_FREQUENCY"  "FIXEDFORMAT_AMPLITUDE

#define ERROR_MSG "Please enter the error scale:"
#define ERROR_CAP "Error scale:"

////////////////////////////////////////////////////
// The mygraph settings
////////////////////////////////////////////////////

#define MYGRAPH_NAME  "CMyGraph"
#define MYGRAPH_TITLE MYGRAPH_NAME" - unnamed"

////////////////////////////////////////////////////
// The mytable settings
////////////////////////////////////////////////////

#define MYTABLE_NAME  "CMyTable"
#define MYTABLE_TITLE MYTABLE_NAME" - unnamed"
#define MYTABLE_DUMMY "DUMMY"

////////////////////////////////////////////////////
// The FOURIER settings
////////////////////////////////////////////////////

#define FOU_TITLE      "Title:"
#define FOU_TITLE_DEF  "No Title yet ..."
#define FOU_FROM       "From:"
#define FOU_FROM_DEF   "0.0"
#define FOU_TO         "To:"
#define FOU_TO_DEF     "0.0"
#define FOU_STEP       "Step rate:"
#define FOU_STEP_DEF   "20"

#define FOU_HIGH       "High"
#define FOU_MEDIUM     "Medium"
#define FOU_LOW        "Low"
#define FOU_CUSTOM     "Custom"

#define FOU_NYQUIST    "Nyquist: 000.00      "
#define FOU_NYQUIST_P  "Nyquist: %g"
#define FOU_HIGHEST    "Highest peak at:"
#define FOU_HIGH_F     "F="
#define FOU_HIGH_A     "A="
#define FOU_HIGH_P     "P="

#define FOU_COMPACT    "Compact:"
#define FOU_ALL        "All"
#define FOU_PEAK       "Peaks only"

#define FOU_DELETE     "Delete"
#define FOU_RENAME     "Rename"

#define FOU_PREV       "Previous calculations:"

#define FOU_NOSAVE     "There is no fourier-spectrum to be saved..."

#define FOU_ALLOPEN    "There is already a window open\nfor this spectrum..."
#define FOU_PLOTNAME   "Fourier plot"
#define FOU_TABLENAME  "Fourier table"

#define FOU_RELABEL     "New label for this Fourier-calculation?"
#define FOU_NOTENOUGHPOINTS "Not enough points for fourier-calculation!" 

////////////////////////////////////////////////////
// The FOURIER-graph settings
////////////////////////////////////////////////////

#define FOUG_NAME      "fourier graph"

////////////////////////////////////////////////////
// The FOURIER-table settings
////////////////////////////////////////////////////

#define FOUT_NAME      "fourier table"

#define FOUT_FRE       "Frequency"
#define FOUT_AMP       "Amplitude"
#define FOUT_POW       "Power"
#define FOUT_UKN       "???"

////////////////////////////////////////////////////
// The TIMESTRING settings
////////////////////////////////////////////////////

#define TIME_CURRENT      "Current data file"
#define TIME_CURRENT_DEF  "No Data file__________________________________"
#define TIME_SELECTED     "Points selected:"
#define TIME_SELECTED_DEF "000000"
#define TIME_TOTAL        "Total points:"
#define TIME_TOTAL_DEF    "000000"
#define TIME_START        "Start time: "
#define TIME_START_DEF    "00000000000000"
#define TIME_END        "End time:"
#define TIME_END_DEF    "00000000000000"
#define TIME_NO_POINTS    "No points"

#define TIME_REVERSE      "Time string is in magnitudes"

#define TIME_EDIT         "Edit"
#define TIME_SELECT       "     Change Selection     "

#define TIME_ALLOPEN      "There is allready a window open"
#define TIME_PLOTNAME     "Time string plot"
#define TIME_TABLENAME    "Time string table"

#define TIME_TWOPOINT     "There are less than 2 points to display,\nWe cannot support this..."
#define TIME_NOSELECT     "Nothing selected..."

#define TIME_CHANGE       "Change names"
#define TIME_NAME         "Name:"
#define TIME_WEIGHT       "Weight:"
#define TIME_COLOR        "Color:"

#define TIME_CHANGE_DOUBLE "The attribute %s does allready exist.\n"\
                           "Please use the "T_TIMESTRING_RENAME" menu\n"\
                           "to join subsets..."

#define TIME_DO           "Change settings"
#define TIME_KEEP         "Keep"

#define TIME_NODATA       "No data-file"

////////////////////////////////////////////////////
// The ADJUST settings
////////////////////////////////////////////////////

#define ADJ_DAT_TITLE   "Adjust Data"
#define ADJ_DAT_HEADING_ORI " " \
                            "Label                   "\
		            "<Original>   "\
		            "Sigma Obs.   "\
		            "Points "\
                            "Adj"
#define ADJ_DAT_HEADING_ADJ " " \
                            "Label                   "\
		            "<Adjusted>   "\
		            "Sigma Adj.   "\
		            "Points "\
                            "Adj"
#define ADJ_DAT_INFO_ORI "Adjusting original values"
#define ADJ_DAT_INFO_ADJ "Adjusting already adjusted values"

#define ADJ_DAT_SAVE     "Save"
#define ADJ_DAT_PRINT    "Print"

#define ADJ_DAT_ADJUST  "Adjust"

#define ADJ_DAT_MSG_ORI "Original data..."
#define ADJ_DAT_MSG_ADJ "Adjusted data..."

#define ADJ_SUB_TITLE   "Subdivide data"
#define ADJ_SUB_GAP     "Size of the gap:"
#define ADJ_SUB_GAP_DEF "0.5"

#define ADJ_SUB_COLUMN  "Put result as what attribute?"
#define ADJ_SUB_COUNTER "Use running counter"
#define ADJ_SUB_PREFIX  "Label prefix"
#define ADJ_SUB_PREFIX_DEF "JD"

#define ADJ_SUB_SUBDIV  "Subdivide"

#define ADJ_REN_TITLE   "Rename data"
#define ADJ_REN_WHAT    "What label"
#define ADJ_REN_WHAT_DEF "JD"

#define ADJ_REN_RENAME  "Rename"

////////////////////////////////////////////////////
// The TIMESTRING-graph settings
////////////////////////////////////////////////////

#define TIMEG_NAME      "Time string graph"
#define TIMEG_TITLE     "Time string graph"

////////////////////////////////////////////////////
// The Timestring-table settings
////////////////////////////////////////////////////

#define TIMET_NAME      "Time string table"

#define TIMET_TIME      "Time"
#define TIMET_PHASE     "Phase"
#define TIMET_OBS       "Observed"
#define TIMET_ADJ       "Adjusted"
#define TIMET_PNT_WEIGHT "Pnt.weight"
#define TIMET_CALC      "Calculated"
#define TIMET_RESOBS    "Residuals(Obs)"
#define TIMET_RESADJ    "Residuals(Adj)"
#define TIMET_WEIGHT    "Weight"
#define TIMET_NONE      "Ignore"

#define TIMET_UKN       "???"

#define TIMET_REMOVEOLDTS_H  "Remove old timestring?"
#define TIMET_REMOVEOLDTS    "Are you sure, you want to remove the old timestring?"


////////////////////////////////////////////////////
// The Period settings
////////////////////////////////////////////////////

#define PERG_BINNING "Number of bins"
#define PERG_BINS    "Please enter the number of bins"

#define PERG_OTHER_VALUE "Choose other value"
#define PERG_FREQUENCY "Frequency:"
#define PERG_CHOOSE_FREQ "Please choose the appropriate frequency"
#define PERG_NEW_FREQ "Please enter new frequency-value"

#define PER_NOTENOUGHPOINTS "Not enough points for period-calculation!" 

#define PER_ACTIVE     "Selected Frequencies"
#define PER_LOOPS      "Iterations:"
#define PER_ZERO       "Zero-point:"
#define PER_RESIDUALS  "Residuals:"
#define PER_IMPROVE    "Improve all"
#define PER_SPECIAL    "Improve Special"

#define PER_PREV       "Prev"
#define PER_PREVX      "Prev %i"
#define PER_NEXTX      "Next %i"
#define PER_NEXT       "Next"

#define PER_USE        "Use Freq#"
#define PER_FRE        " Frequency"
#define PER_AMP        " Amplitude"
#define PER_PHA        " Phase"

#define PER_PLOTNAME   "Phase diagram"
#define PER_PHASEDIA   "Phase diagram"

// the special dialog

#define SPE_SEL        "Special selection"
#define SPE_MSG        "Please select what you want to improve:"
#define SPE_FRE        "Frequency"
#define SPE_AMP        "Amplitude"
#define SPE_PHA        "Phase"
#define SPE_AMPVAR     CALCULATE" amplitude/phase variations"

// the ampvar dialog

#define AMP_TITLE      "Amplitude variations"
#define AMP_USE        "Attribute to use:"
#define AMP_MSG_1      "All the frequencies shown below"
#define AMP_MSG_2      "will be used in the next calculation."
#define AMP_MODE       "Now select those with "
#define AMP_RESULT     "Amplitude variations result"
#define AMP_FREQ       ": frequency="
#define AMP_AMPVAR     "amplitude variations"
#define AMP_PHAVAR     "phase variations"
#define AMP_ALLVAR     "ampl.and phase var."

// the Epoch_dialog

#define EPO_TITLE      "Calculate epoch"
#define EPO_TIME       "Time of epoch:"
#define EPO_HEADING    "Freq Frequency Epoch"
#define EPO_INT        "Data is in intensity"

// the headings-dialog

#define HEADING_MESSAGE "Change heading:"
#define HEADING_CAPTION "New name:"

// the ZOOM-dialog

#define ZOOM_TITLE     "Set viewport for Plot"
#define ZOOM_MIN       "%s min:"
#define ZOOM_MAX       "%s max:"

// no points

#define NO_POINTS      "No points to plot"
// the Relabel Dialog
#define DIAL_RELABEL_TIT "Relabel data point"
#define DIAL_RELABEL_TIME "Time="
#define DIAL_DELPNT_TIT   "Change settings for deleted points"
#define DIAL_DELPNT_2     "Label to use for deleted points:"
#define DIAL_DELPNT_3     "Put deleted points as attribute:"

// the Refit messages
#define REFIT_MSG      "Zeropoint for refit:"
#define REFIT_CAP      "Refit data"

// Graph-defines

#define TIME              "Time"
#define PHASE             "Amplitude"
#define FREQUENCY         "Frequency"
#define SIGNAL            "Signal"
#define POWER             "Power"
#define AMPLITUDE         "Amplitude"

#endif
