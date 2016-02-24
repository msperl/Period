#include "xgmainfr.h"
#include "xpgraph.h"
#include "xgmenu.h"
#include "xppredic.h"

int CMainFrame::FitMenu(int id)
{
  switch (id)
    {    
    case M_FIT_LOAD:
      myProject.LoadPeriod();
      break;
    case M_FIT_SAVE:
      myProject.SavePeriod();
      break;
    case M_FIT_CLEAN:
      myProject.CleanPeriod();
      break;
    case M_FIT_PREDICT:
      // Update data from original data
      PredictAmplitude(this);
      break;
    case M_FIT_REFIT:
      {
	// now get zeropoint
	myString tmp=myProject.GetZeropoint();
	char *result=wxGetTextFromUser(REFIT_MSG,REFIT_CAP,tmp.chars());
	if (result)
	  {
	    myProject.Refit(result);
	  }
	break;
      }

    case M_FIT_ARTIF:
      {
	// Now create data
	CreateArtificialData(this);
	break;
      }

    case M_FIT_FREQADJ:
      {
	myString str=myProject.GetFrequencyAdjustment();
	char *user=::wxGetTextFromUser(FREQ_ADJUST_HEAD,
				       FREQ_ADJUST_QUEST,
				       str.chars());
	if (user!=NULL)
	  {
	    myProject.SetFrequencyAdjustment(user);
	  }
	break;
      }

    case M_FIT_EPOCH:
      EpochDialog(this);
      break;
    default:
      return FALSE;
    }
  return TRUE;
}

void p_Graph_Frame::OnMenuCommand(int id)
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
      else
        {
	  switch (id)
	    {
	    case M_ERROR_DISPLAY:
	      DrawError=GetMenuBar()->Checked(id);
	      UpdateDisplay(0);
	      break;
	    case M_FILE_SAVEPHA:
	      myProject.SaveTimeStringPhase(Frequency);
	      break;
	    case M_FILE_SAVEPHABIN:
	      myProject.SaveTimeStringPhaseBinned(
						  Frequency,
						  BinValue(),
						  (DataMode)UseData,
						  binsize,
						  binphase,
						  binampl,
						  binampl2
						  );
	      break;
	    case M_FREQ_BIN:
	      SetUseBinning(!UseBinning);
	      break;
	    case M_FREQ_EDIT:
	      ChangeFrequency();
	      break;
	    case M_FREQ_BINVAL:
	      ChangeBinSpacing();
	      break;
	    default:
	      CMyGraph::OnMenuCommand(id);
	      break;
	    }
	}
    }
}
