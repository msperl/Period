/*
 * Program:     Period98
 *
 * File:        lmenu.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#define T_FILE        "File"

#define T_FILE_NEW    "New Project"
#define C_FILE_NEW    "Clear all and restart"
#define T_FILE_LOAD   "Load Project"
#define C_FILE_LOAD   "Load a project into memory"
#define T_FILE_SAVE   "Save Project"
#define C_FILE_SAVE    "Saves the current project"
#define T_FILE_SAVEAS "Save Project As"
#define C_FILE_SAVEAS "Saves the current project with a different name"
#define T_FILE_PRINT  "Print"
#define C_FILE_PRINT  "Makes a printout"

#define T_FILE_CLOSE  "Close"
#define C_FILE_CLOSE  "Close this window"

#define T_FILE_QUIT   "Quit"
#define C_FILE_QUIT    "Exit Period98"

#define T_FILE_SAVEPHA "Export phases"
#define C_FILE_SAVEPHA "Exports the phase-data to the current plot"

#define T_FILE_SAVEPHABIN "Export binned phases"
#define C_FILE_SAVEPHABIN "Exports the binned phase-data to the current plot"

////////////////////////////////////////////////

#define T_TIMESTRING         "Timestring"

#define T_TIMESTRING_LOAD   "Import time-string"
#define C_TIMESTRING_LOAD   "Loads a time-string removing the old one"
#define T_TIMESTRING_APPEND   "Append time-string"
#define C_TIMESTRING_APPEND   "Appends a time-string to the current one and selects all points"
#define T_TIMESTRING_SAVE   "Export time-string"
#define C_TIMESTRING_SAVE   "Saves the timestring"

#define T_TIMESTRING_SUBDIV  "Subdivide time-string"
#define C_TIMESTRING_SUBDIV  "Subdivides into substrings"
#define T_TIMESTRING_ADJUST  "Adjust time-string"
#define C_TIMESTRING_ADJUST  "Adjust zero-points of substrings"
#define T_TIMESTRING_MAKEADJ "Restore zero-point adjustments"
#define C_TIMESTRING_MAKEADJ "Cancels previous zero-point adjustments"
#define T_TIMESTRING_RENAME  "Combine substrings"
#define C_TIMESTRING_RENAME  "Combines the selected substrings"
#define T_TIMESTRING_DELDEF  "Delete default label"
#define C_TIMESTRING_DELDEF  "Sets the delete default-label"

#define T_TIMESTRING_NAME_WEIGHT "Use weights from attribute"
#define C_TIMESTRING_NAME_WEIGHT "Calculate weight using the weights from the attributes"

#define T_TIMESTRING_POINT_WEIGHT "Use weights from points"
#define C_TIMESTRING_POINT_WEIGHT "Calculate weight using the individual weight for each point"

#define T_TIMESTRING_ERROR_SCALE "Set error scale"
#define C_TIMESTRING_ERROR_SCALE "Defines a new error scale used with plots"

////////////////////////////////////////////////

#define T_FIT         "Fit"

#define T_FIT_LOAD   "Import frequencies"
#define C_FIT_LOAD   "Loads a list of frequencies removing the old ones"
#define T_FIT_SAVE   "Export frequencies as"
#define C_FIT_SAVE   "Saves all frequencies"
#define T_FIT_EPOCH   "Epochs"
#define C_FIT_EPOCH   "Calculate epochs for the active frequencies"
#define T_FIT_REFIT   "Recalculate residuals"
#define C_FIT_REFIT   "Recalculates the residuals for the current fit"
#define T_FIT_PREDICT "Predict signal"
#define C_FIT_PREDICT "Predict a signal for a given time"
#define T_FIT_ARTIF   "Create artificial data"
#define C_FIT_ARTIF   "Creates artificial data with the currently selected frequencies"
#define T_FIT_FREQADJ "Set alias-gap"
#define C_FIT_FREQADJ "Sets the frequency-gap between two aliasing"

////////////////////////////////////////////////

#define T_FOURIER      "Fourier"
#define T_FOURIER_NOISE "Calculate noise at frequency"
#define C_FOURIER_NOISE "Calculates noise from the current residuals at a given frequency"
#define T_FOURIER_NOISE_SPECTRA "Calculate noise spectrum"
#define C_FOURIER_NOISE_SPECTRA "Calculates a noise-spectra from the current residuals in a given range"

////////////////////////////////////////////////

#define T_LOG      "Log"
#define T_LOG_SAVE "Save logfile"
#define C_LOG_SAVE "Saves the logfile to an extra file"
#define T_LOG_PRINT  "Print Logfile"
#define C_LOG_PRINT  "Makes a printout of the logfile"


////////////////////////////////////////////////

#define T_DISPLAY        "Display"
#define T_DISPLAY_POWER  "Display power"
#define C_DISPLAY_POWER  "Displays spectra in power"

#define T_FREQ           "Frequency"
#define T_FREQ_EDIT      "Edit Frequency"
#define C_FREQ_EDIT      "Changes the frequency for which the phase is displayed"
#define T_FREQ_BIN       "Use Binning"
#define C_FREQ_BIN       "Display binned data"
#define T_FREQ_BINVAL    "Binning spacing"
#define C_FREQ_BINVAL    "Changes the binning spacing"

////////////////////////////////////////////////

#define T_COLORS         "Colors"
#define T_COLORS_USEC   "%s"
#define C_COLORS_USEC   "Use %s as indicator for colors"

////////////////////////////////////////////////

#define T_DATA           "Data"
#define T_DATA_OBS       "Observed"
#define C_DATA_OBS       "Displays observed values"
#define T_DATA_ADJ       "Adjusted"
#define C_DATA_ADJ       "Displays zero-point adjusted values"
#define T_DATA_ROB       "Residuals - Observed"
#define C_DATA_ROB       "Displays Residuals corresponding to non-zero-point adjusted values"
#define T_DATA_RAD       "Residuals - Adjusted"
#define C_DATA_RAD       "Displays residuals corresponding to zero-point adjusted values"

////////////////////////////////////////////////

#define T_ZOOM         "Zoom"
#define T_ZOOM_DISPALL "Display all"
#define C_ZOOM_DISPALL "Displays the whole data-set"
#define T_ZOOM_DISPREG "Select viewport"
#define C_ZOOM_DISPREG "Changes the viewport of the graph"

////////////////////////////////////////////////

#define T_ERROR         "Error"
#define T_ERROR_DISPLAY "Draw error bars"
#define C_ERROR_DISPLAY "Draw error bars assuming that weights corresponds to errors"

////////////////////////////////////////////////

#define T_HELP       "Help"
#define T_HELP_ABOUT "About"
#define C_HELP_ABOUT "Gives basic information about Period98"
#define T_HELP_INTRO "Introduction"
#define C_HELP_INTRO "Gives you a basic introduction to Period98"
#define T_HELP_CONTENTS "Contents"
#define C_HELP_CONTENTS "Lists all possible topics in help"
#define T_HELP_HOWTOUSE "How to use"
#define C_HELP_HOWTOUSE "Gives you a brief tour around the features of Period98"
#define T_HELP_KEY "Keyword search"
#define C_HELP_KEY "Let's you search for keywords"
#define T_HELP_COPYRIGHT "Copyright"
#define C_HELP_COPYRIGHT "Shows you the Copyright of Period98"

#define T_HELP_BUGS "Report bug"
#define C_HELP_BUGS "Shows information on where to report bugs."






