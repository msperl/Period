/*
 * Program:     Period98
 *
 * File:        xmain.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xmain.h"
#include "xgmainfr.h"
#include "dgtimein.h"
#include <stdlib.h>
#ifndef wx_msw
#include <unistd.h>
#endif
#include "advanced.h"
#include "mswdefs.h"

// 
// this is equivalent to the application main
//

wxPeriod myApp;
//
// Just in case for some compilers
IMPLEMENT_WXWIN_MAIN

// the Message Queue handler...

int ProcessMessageQueue()
{
  wxYield();
  return MainFrame->CancelCalculation();
}

// Constructor- for the main-class

extern char *optarg;
extern int optind,opterr;

wxPeriod::wxPeriod()
{
  SetClassName("Period98");
  FrequencyRows=-1;
}

//
// main-application-initialisation
//

wxFrame *wxPeriod::OnInit()
{
#ifndef wx_msw
	// set the AFM-font-path
  char *afmpath=getenv("PERIOD98_AFM_PATH");
  if (afmpath) {
    wxSetAFMPath(afmpath);
  }
#endif
  // parse commandline
  char *Filename=NULL;
  // check for arguments
  IsAdvanced=0;
#ifdef wx_msw
  for (int i=1;i<argc;i++)
    {
      if (strcasecmp(argv[i],"-a")==0)
	{
	  IsAdvanced=1;
	}
      else if (strncasecmp(argv[i],"-r",2)==0)
	{
	  FrequencyRows=-1;
	  sscanf(&((argv[i])[2]),"%i",&FrequencyRows);
	  if ((FrequencyRows<-1)||(FrequencyRows>32))
	    {
	      FrequencyRows=-1;
	    }
	}
      else if (Filename==NULL)
	{
	  Filename=argv[i];
	}
      else
	{
	  cerr<<argv[0]<<"Wrong argument:"<<argv[i]<<endl;
	}
    }
#else
  int c;
  int lastpos=1;
  while ( ( c=getopt(argc,argv,"ar:")) != EOF )
    {
      switch (c)
	{
	case 'r':
	  FrequencyRows=-1;
	  sscanf(optarg,"%i",&FrequencyRows);
	  if ((FrequencyRows<-1)||(FrequencyRows>32))
	    {
	      FrequencyRows=-1;
	    }
	  break;
	case 'a':
	  IsAdvanced=1;
	  break;
	}
    }
  lastpos=optind;
  // now check out the filename
  if (argc > lastpos)
    {
      Filename=argv[lastpos];
    }
#endif
  // create the Mainframe
  CMainFrame* tmp= new CMainFrame();
  // read in files if  neccessary
  if (Filename!=NULL)
    {
      // lets load file 
      myProject.LoadProject(Filename);
    }
  // update The display
  myProject.UpdateAllDisplays();
  // no changes have been made up to now
  myProject.NoChanges();
  // set the printer to Landscape as default
#ifndef wx_msw
  wxSetPrinterOrientation(PS_LANDSCAPE);
#endif
  // return a pointer to the window
  return tmp;
}










