#include "xgmainfr.h"
#include "xfgraph.h"
#include "xgmenu.h"
#include "xfnoise.h"

int CMainFrame::FourierMenu(int id)
{
  switch (id)
    {
    case M_FOURIER_NOISE:
      {
	// here goes the Noise-dialog
	NoiseCalculate(this);
	break;
      }
    case M_FOURIER_NOISE_SPECTRA:
      {
	// here goes the Noise-dialog
	NoiseSpectraCalculate(this);
	break;
      }
    default:
      return FALSE;
    }
  return TRUE;
}

void f_Graph_Frame::OnMenuCommand(int id)
{
  switch (id)
    {
    case M_DISPLAY_POWER:
      SetUsePower(!UsePower);
      break;
    default:
      CMyGraph::OnMenuCommand(id);
      break;
    }
}
