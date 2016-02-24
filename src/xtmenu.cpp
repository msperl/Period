#include "xtgraph.h"
#include "xgmainfr.h"
#include "xgmenu.h"
#include "xtadjust.h"
#include "xtrename.h"
#include "fgeneral.h"

int CMainFrame::TimestringMenu(int id)
{
  switch (id)
    {
    case M_TIMESTRING_LOAD:
      myProject.LoadTimeString();
      break;
    case M_TIMESTRING_APPEND:
      myProject.AppendTimeString();
      break;
    case M_TIMESTRING_SAVE:
      myProject.SaveTimeString();
      break;
    case M_TIMESTRING_ADJUST:
      AdjustData(this);
      break;
    case M_TIMESTRING_MAKEADJ:
      myProject.MakeObservedAdjusted();
      break;
    case M_TIMESTRING_RENAME:
      {
        RenameSelection(this);
        break;
      }
    case M_TIMESTRING_SUBDIV:
      {
        SubDivideData(this);
        break;
      }

    case M_TIMESTRING_DELDEF:
      DialogDeletePointInfo(this);
      break;
    case M_TIMESTRING_NAME_WEIGHT:
      {
	int opt=GetMenuBar()->Checked(id);
	myProject.SetUseNameWeight(opt);
        break;
      }
    case M_TIMESTRING_POINT_WEIGHT:
      {
	int opt=GetMenuBar()->Checked(id);
	myProject.SetUsePointWeight(opt);	
        break;
      }
    case M_TIMESTRING_ERROR_SCALE:
      {
	char *c,def[256];
	sprintf(def,FORMAT_AMPLITUDE,myProject.GetErrorScale());
	c=wxGetTextFromUser(ERROR_MSG,
			    ERROR_CAP,
			    def);
	if (c)
	  {
	    myProject.SetErrorScale(c);
	  }
	break;
      }
    default:
      return FALSE;
    }
  return TRUE;
}


void t_Graph_Frame::OnMenuCommand(int id)
{
  if ((id>400)&&(id<410))
    {
      id=id-401;
      SetUseName(id);
    }
  else
    {
      if ((id>500)&&(id<510))
	{
	  id=id-501;
	  SetUseData(id);
	}
      else if (id==M_ERROR_DISPLAY)
	{
	  DrawError=GetMenuBar()->Checked(id);
	  UpdateDisplay(0);
	}
      else
	{
	  CMyGraph::OnMenuCommand(id);
	}
    }
}
