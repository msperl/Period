#include "xgmainfr.h"
#include "xgmenu.h"
#include "flog.h"
#include "wxtxtprint.h"

int CMainFrame::LogMenu(int id)
{
  switch (id)
    {
    case M_LOG_PRINT:
      {
	char *text=TextDisp->GetContents();
	wxPrintText(this,"Logfile",text,50,12,wxMODERN);
      }
      break;
    case M_LOG_SAVE:
      {
	char* name=wxFileSelector(LOG_SAVE,
				  NULL,NULL,
				  DIALOG_LOG_EXTENTION,
				  DIALOG_LOG_PATTERN,
				  wxSAVE|wxOVERWRITE_PROMPT);
	if (name)
	  {
	    TextDisp->SaveFile(name);
	  }
      }
      break;
    default:
      return FALSE;
    }
  return TRUE;
}
