/*
 * Program:     Period98
 *
 * File:        xgmenu.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */


#define M_FILE_NEW    100
#define M_FILE_LOAD   101
#define M_FILE_SAVE   102
#define M_FILE_SAVEAS 103
#define M_FILE_PRINT  110
#define M_FILE_SAVEPHA 140
#define M_FILE_SAVEPHABIN 141

#define M_FILE_CLOSE  198

#define M_FILE_QUIT   199

////////////////////////////////////////////////

#define M_TIMESTRING_LOAD   121
#define M_TIMESTRING_APPEND 122
#define M_TIMESTRING_SAVE   123

#define M_TIMESTRING_SUBDIV  200 
#define M_TIMESTRING_ADJUST  201
#define M_TIMESTRING_MAKEADJ 202
#define M_TIMESTRING_RENAME  203
#define M_TIMESTRING_DELDEF  204

#define M_TIMESTRING_NAME_WEIGHT 210
#define M_TIMESTRING_POINT_WEIGHT 211
#define M_TIMESTRING_ERROR_SCALE 212

////////////////////////////////////////////////

#define M_FIT_LOAD   256
#define M_FIT_SAVE   257
#define M_FIT_CLEAN   255

#define M_FIT_EPOCH   250
#define M_FIT_REFIT   251
#define M_FIT_PREDICT 252
#define M_FIT_ARTIF   253
#define M_FIT_FREQADJ 254

////////////////////////////////////////////////

#define M_FOURIER_NOISE 701
#define M_FOURIER_NOISE_SPECTRA 702

////////////////////////////////////////////////
#define M_LOG_SAVE  270
#define M_LOG_PRINT 271

////////////////////////////////////////////////

#define M_DISPLAY_POWER  301

#define M_FREQ_EDIT   321
#define M_FREQ_BIN    322
#define M_FREQ_BINVAL 323

////////////////////////////////////////////////
//
// this is very hairy !!! I should change this soon
// to acomedate changes of titles
//

#define M_COLORS_USEC1   401
#define M_COLORS_USEC2   402
#define M_COLORS_USEC3   403
#define M_COLORS_USEC4   404

////////////////////////////////////////////////

#define M_DATA_OBS       501
#define M_DATA_ADJ       502
#define M_DATA_ROB       503
#define M_DATA_RAD       504

////////////////////////////////////////////////

#define M_ZOOM_DISPALL 601
#define M_ZOOM_DISPREG 602

////////////////////////////////////////////////

#define M_ERROR_DISPLAY 701

////////////////////////////////////////////////

#define M_HELP_ABOUT 900
#define M_HELP_INTRO 901
#define M_HELP_CONTENTS 902
#define M_HELP_HOWTOUSE 903
#define M_HELP_KEY 904
#define M_HELP_COPYRIGHT 905
#define M_HELP_BUGS 906

////////////////////////////////////////////////

#include <wx.h>

///
wxMenuBar* CreateMenuBar();
///
wxMenu* CreateMenu_File();
///
wxMenu* CreateMenu_Timestring();
///
wxMenu* CreateMenu_Fit();
///
wxMenu* CreateMenu_Fourier();
///
wxMenu* CreateMenu_Log();
///
wxMenu* CreateMenu_Help();

///
wxMenuBar* CreateMenuBar_d_Data();
///
wxMenu* CreateMenu_d_File();

///
wxMenuBar* CreateMenuBar_f_Graph();
///
wxMenu* CreateMenu_f_Graph();
///
wxMenu* CreateMenu_f_Display();

//
wxMenuBar* CreateMenuBar_p_Graph();
///
wxMenu* CreateMenu_p_Frequency();
//
wxMenu* CreateMenu_p_File();

///
wxMenuBar* CreateMenuBar_t_Graph();
///
wxMenu* CreateMenu_t_Graph();
///
wxMenu* CreateMenu_t_Color();
///
wxMenu* CreateMenu_t_Data();
///
wxMenu* CreateMenu_t_Zoom();
///
wxMenu* CreateMenu_t_Error();

///
wxMenuBar* CreateMenuBar_Simple();

///
int HelpMenu(int id);
