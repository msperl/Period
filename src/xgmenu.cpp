/*
 * Program:     Period98
 *
 * File:        xgmenu.cpp
 * Purpose:     implementation-file for
 *              menu-creation
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "xgmenu.h"
#include "lmenu.h"
#include "xgprojec.h"
#include "advanced.h"

int IsAdvanced=1;

wxMenuBar* CreateMenuBar()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_File(),T_FILE);
  // Timestring-Menu
  bar->Append(CreateMenu_Timestring(),T_TIMESTRING);
  // Fit-Menu
  bar->Append(CreateMenu_Fit(),T_FIT);
  // Noise-Menu
  bar->Append(CreateMenu_Fourier(),T_FOURIER);
  // Noise-Menu
  bar->Append(CreateMenu_Log(),T_LOG);
  // Help-Menu
  bar->Append(CreateMenu_Help(),T_HELP);
  // return the menu
  return bar;
}

wxMenu* CreateMenu_File()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_FILE_NEW,
	       T_FILE_NEW,
	       C_FILE_NEW);
  file->Append(M_FILE_LOAD,
	       T_FILE_LOAD,
	       C_FILE_LOAD);
  file->Append(M_FILE_SAVE,
	       T_FILE_SAVE,
	       C_FILE_SAVE);
  file->Append(M_FILE_SAVEAS,
	       T_FILE_SAVEAS,
	       C_FILE_SAVEAS);
  
  file->AppendSeparator();

  //  file->Append(M_FILE_,T_FILE_,C_FILE_);
  file->Append(M_FILE_QUIT,T_FILE_QUIT,C_FILE_QUIT);

  return file;
}
wxMenu* CreateMenu_Timestring()
{
  // create this menu
  wxMenu *special=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  special->Append(M_TIMESTRING_LOAD,
		  T_TIMESTRING_LOAD,
		  C_TIMESTRING_LOAD);
  special->Append(M_TIMESTRING_APPEND,
		  T_TIMESTRING_APPEND,
		  C_TIMESTRING_APPEND);
  special->Append(M_TIMESTRING_SAVE,
		  T_TIMESTRING_SAVE,
		  C_TIMESTRING_SAVE);

  special->AppendSeparator();

  special->Append(M_TIMESTRING_POINT_WEIGHT,
		  T_TIMESTRING_POINT_WEIGHT,
		  C_TIMESTRING_POINT_WEIGHT,
		  TRUE);

  special->Append(M_TIMESTRING_NAME_WEIGHT,
		  T_TIMESTRING_NAME_WEIGHT,
		  C_TIMESTRING_NAME_WEIGHT,
		  TRUE);
  if (IsAdvanced) 
    {
      special->AppendSeparator();
      special->Append(M_TIMESTRING_ERROR_SCALE,
		      T_TIMESTRING_ERROR_SCALE,
		      C_TIMESTRING_ERROR_SCALE);
    }

  special->AppendSeparator();

  special->Append(M_TIMESTRING_SUBDIV,
		  T_TIMESTRING_SUBDIV,
		  C_TIMESTRING_SUBDIV);
  special->Append(M_TIMESTRING_ADJUST,
		  T_TIMESTRING_ADJUST,
		  C_TIMESTRING_ADJUST);
  special->Append(M_TIMESTRING_MAKEADJ,
		  T_TIMESTRING_MAKEADJ,
		  C_TIMESTRING_MAKEADJ);
  special->Append(M_TIMESTRING_RENAME,
		  T_TIMESTRING_RENAME,
		  C_TIMESTRING_RENAME);
  special->Append(M_TIMESTRING_DELDEF,
		  T_TIMESTRING_DELDEF,
		  C_TIMESTRING_DELDEF);

  // return the menu
  return special;
}

wxMenu* CreateMenu_Fit()
{
  // create this menu
  wxMenu *special=new wxMenu;
  //
  // create and add the relevant menu-entries
  //

  special->Append(M_FIT_LOAD,
		  T_FIT_LOAD,
		  C_FIT_LOAD);
  special->Append(M_FIT_SAVE,
		  T_FIT_SAVE,
		  C_FIT_SAVE);
  special->AppendSeparator();

  special->Append(M_FIT_EPOCH,
		  T_FIT_EPOCH,
		  C_FIT_EPOCH);
  special->Append(M_FIT_REFIT,
		  T_FIT_REFIT,
		  C_FIT_REFIT);
  special->Append(M_FIT_PREDICT,
		  T_FIT_PREDICT,
		  C_FIT_PREDICT);
  special->Append(M_FIT_ARTIF,
		  T_FIT_ARTIF,
		  C_FIT_ARTIF);
  special->Append(M_FIT_FREQADJ,
		  T_FIT_FREQADJ,
		  C_FIT_FREQADJ);

  // return the menu
  return special;
}

wxMenu* CreateMenu_Fourier()
{
  // create this menu
  wxMenu *noise=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  noise->Append(M_FOURIER_NOISE,
		T_FOURIER_NOISE,
		C_FOURIER_NOISE);
  noise->Append(M_FOURIER_NOISE_SPECTRA,
		T_FOURIER_NOISE_SPECTRA,
		C_FOURIER_NOISE_SPECTRA);

  // return the menu
  return noise;
}

wxMenu* CreateMenu_Log()
{
  // create this menu
  wxMenu *noise=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  noise->Append(M_LOG_SAVE,
		T_LOG_SAVE,
		C_LOG_SAVE);
  noise->AppendSeparator();
  noise->Append(M_LOG_PRINT,
		T_LOG_PRINT,
		C_LOG_PRINT);

  // return the menu
  return noise;
}

wxMenu* CreateMenu_Help()
{
  // create this menu
  wxMenu *help=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  help->Append(M_HELP_ABOUT,
	       T_HELP_ABOUT,
	       C_HELP_ABOUT);
  help->Append(M_HELP_COPYRIGHT,
	       T_HELP_COPYRIGHT,
	       C_HELP_COPYRIGHT);

  help->AppendSeparator();

  help->Append(M_HELP_INTRO,
	       T_HELP_INTRO,
	       C_HELP_INTRO);
  help->Append(M_HELP_CONTENTS,
	       T_HELP_CONTENTS,
	       C_HELP_CONTENTS);
  help->Append(M_HELP_HOWTOUSE,
	       T_HELP_HOWTOUSE,
	       C_HELP_HOWTOUSE);
  help->Append(M_HELP_BUGS,
	       T_HELP_BUGS,
	       C_HELP_BUGS);

  help->AppendSeparator();

  help->Append(M_HELP_KEY,
	       T_HELP_KEY,
	       C_HELP_KEY);
  return help;
}

wxMenuBar* CreateMenuBar_d_Data()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_d_File(),T_FILE);
  return bar;
}

wxMenu* CreateMenu_d_File()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_FILE_PRINT,
	       T_FILE_PRINT,
	       C_FILE_PRINT);
  file->Append(M_FILE_CLOSE,
	       T_FILE_CLOSE,
	       C_FILE_CLOSE);

  return file;
}

wxMenuBar* CreateMenuBar_p_Graph()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_p_File(),T_FILE);
  bar->Append(CreateMenu_t_Color(),T_COLORS);
  bar->Append(CreateMenu_t_Data(),T_DATA);
  bar->Append(CreateMenu_p_Frequency(),T_FREQ);
  bar->Append(CreateMenu_t_Zoom(),T_ZOOM);
  if (IsAdvanced)
    { 
      bar->Append(CreateMenu_t_Error(),T_ERROR);
    }
  bar->Append(CreateMenu_Help(),T_HELP);
  return bar;
}

wxMenu* CreateMenu_p_File()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_FILE_SAVEPHA,
	       T_FILE_SAVEPHA,
	       C_FILE_SAVEPHA);
  file->Append(M_FILE_SAVEPHABIN,
	       T_FILE_SAVEPHABIN,
	       C_FILE_SAVEPHABIN);

  file->AppendSeparator();

  file->Append(M_FILE_PRINT,
	       T_FILE_PRINT,
	       C_FILE_PRINT);

  file->AppendSeparator();

  file->Append(M_FILE_CLOSE,
	       T_FILE_CLOSE,
	       C_FILE_CLOSE);

  return file;
}

wxMenu* CreateMenu_p_Frequency()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_FREQ_EDIT,
	       T_FREQ_EDIT,
	       C_FREQ_EDIT);
  file->Append(M_FREQ_BIN,
	       T_FREQ_BIN,
	       C_FREQ_BIN,
	       TRUE);
  file->Append(M_FREQ_BINVAL,
	       T_FREQ_BINVAL,
	       C_FREQ_BINVAL);
  return file;
}

wxMenuBar* CreateMenuBar_f_Graph()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_d_File(),T_FILE);
  bar->Append(CreateMenu_f_Display(),T_DISPLAY);
  bar->Append(CreateMenu_t_Zoom(),T_ZOOM);
  bar->Append(CreateMenu_Help(),T_HELP);
  return bar;
}

wxMenu* CreateMenu_f_Display()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_DISPLAY_POWER,
	       T_DISPLAY_POWER,
	       C_DISPLAY_POWER,
	       TRUE);
  return file;
}


wxMenu* CreateMenu_t_Graph()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_FILE_PRINT,
	       T_FILE_PRINT,
	       C_FILE_PRINT);
  file->Append(M_FILE_CLOSE,
	       T_FILE_CLOSE,
	       C_FILE_CLOSE);

  return file;
}

wxMenuBar* CreateMenuBar_t_Graph()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_d_File(),T_FILE);
  bar->Append(CreateMenu_t_Color(),T_COLORS);
  bar->Append(CreateMenu_t_Data(),T_DATA);
  bar->Append(CreateMenu_t_Zoom(),T_ZOOM);
  if (IsAdvanced)
    { 
      bar->Append(CreateMenu_t_Error(),T_ERROR);
    }
  bar->Append(CreateMenu_Help(),T_HELP);
  return bar;
}

wxMenu* CreateMenu_t_Color()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  char txt1[256],txt2[256];
  myString name;
  // first
  name=myProject.NameSet(0);
  sprintf(txt1,T_COLORS_USEC,name.chars());
  sprintf(txt2,C_COLORS_USEC,name.chars());
  file->Append(M_COLORS_USEC1,
	       txt1,
	       txt2,
	       TRUE);
  // second
  name=myProject.NameSet(1);
  sprintf(txt1,T_COLORS_USEC,name.chars());
  sprintf(txt2,C_COLORS_USEC,name.chars());
  file->Append(M_COLORS_USEC2,
	       txt1,
	       txt2,
	       TRUE);
  // third
  name=myProject.NameSet(2);
  sprintf(txt1,T_COLORS_USEC,name.chars());
  sprintf(txt2,C_COLORS_USEC,name.chars());
  file->Append(M_COLORS_USEC3,
	       txt1,
	       txt2,
	       TRUE);
  // forth
  name=myProject.NameSet(3);
  sprintf(txt1,T_COLORS_USEC,name.chars());
  sprintf(txt2,C_COLORS_USEC,name.chars());
  file->Append(M_COLORS_USEC4,
	       txt1,
	       txt2,
	       TRUE);
  return file;
}

wxMenu* CreateMenu_t_Data()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_DATA_OBS,
	       T_DATA_OBS,
	       C_DATA_OBS,
	       TRUE);
  file->Append(M_DATA_ADJ,
	       T_DATA_ADJ,
	       C_DATA_ADJ,
	       TRUE);
  file->Append(M_DATA_ROB,
	       T_DATA_ROB,
	       C_DATA_ROB,
	       TRUE);
  file->Append(M_DATA_RAD,
	       T_DATA_RAD,
	       C_DATA_RAD,
	       TRUE);
  return file;
}

wxMenu* CreateMenu_t_Zoom()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_ZOOM_DISPALL,
	       T_ZOOM_DISPALL,
	       C_ZOOM_DISPALL);
  file->Append(M_ZOOM_DISPREG,
	       T_ZOOM_DISPREG,
	       C_ZOOM_DISPREG);

  return file;
}

wxMenu* CreateMenu_t_Error()
{
  // create this menu
  wxMenu *file=new wxMenu;
  //
  // create and add the relevant menu-entries
  //
  file->Append(M_ERROR_DISPLAY,
	       T_ERROR_DISPLAY,
	       C_ERROR_DISPLAY,
	       TRUE);

  return file;
}

wxMenuBar* CreateMenuBar_Simple()
{
  // create the Menubar
  wxMenuBar *bar=new wxMenuBar;
  //
  // create and add the relevant menus
  //
  // File-Menu
  bar->Append(CreateMenu_d_File(),T_FILE);
  bar->Append(CreateMenu_Help(),T_HELP);
  return bar;
}








