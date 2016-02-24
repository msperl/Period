/*
 * Program:     Period98
 *
 * File:        xgmenuac.cpp
 * Purpose:     implementation-file for
 *              menu handling
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgmenu.h"
#include "xgmainfr.h"
#include "fproject.h"
#include "ldialtxt.h"
#include "helpref.h"

int HelpMenu(int id)
{
  switch (id)
    {
      
    // HELP MENUS
    case M_HELP_ABOUT:
      {
	char msg[4096];
	sprintf(msg,ABOUT_MSG,
		VERSION,VERSIONDATE,
		COMPILEDBY,COMPILEDDATE);
	(void)wxMessageBox(msg,ABOUT_HEAD,wxOK|wxCENTRE);
      }
      break;
    case M_HELP_COPYRIGHT:
      myProject.HelpKeyword(HELP_COPYRIGHT);
	  break;
    case M_HELP_INTRO:
      myProject.HelpKeyword(HELP_INTRODUCT);
      break;
    case M_HELP_CONTENTS:
      myProject.HelpContents();
      break;
    case M_HELP_HOWTOUSE:
      myProject.HelpKeyword(HELP_HOWTOUSE);
      break;
    case M_HELP_BUGS:
      myProject.HelpKeyword(HELP_BUGS);
      break;
    case M_HELP_KEY:
      myProject.HelpKeyword();
      break;
    default:
      return FALSE;
    }
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////

int CMainFrame::FileMenu(int id)
{
  switch (id)
    {
    case M_FILE_SAVE:
      myProject.SaveProject();
      break;
    case M_FILE_SAVEAS:
      myProject.SaveProjectAs();
      break;
    case M_FILE_LOAD:
      myProject.LoadProject();
      break;
      
    case M_FILE_NEW:
      myProject.NewProject();
      break;
      
    case M_FILE_QUIT:
      if (myProject.HasChanged())
	{
	  if (myProject.Confirm(DIALOG_SAVE_PROJECT_FIRST)!=0)
	    {
	      // save ...
	      myProject.SaveProject();
	      // possibly not saved?
	      if (myProject.HasChanged())
		{
		  // not saved...
		  return TRUE;
		}
	    }
	  }
	  // everything is up to date
	  myProject.NoChanges();
	  // now clean everything...
	  myProject.NewProject();
      delete this;
      break;

    default:
      return FALSE;
    }
  return TRUE;
}


