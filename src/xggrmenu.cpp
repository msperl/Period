#include "xggraph.h"
#include "xgmenu.h"
#include "xgeneral.h"
#include "project.h"

void CMyGraph::OnMenuCommand(int id)
{
  switch (id)
    {
    case M_ZOOM_DISPALL:
      // Set Extrems
      CalcExtrems();
      // set values
      SetVisible(ExtXMin,ExtXMax,ExtYMin,ExtYMax);
      // Plot page anew
      myPlotArea->Refresh();
      break;
    case M_ZOOM_DISPREG:
      {
	wxDialogBox dialog(this,ZOOM_TITLE,TRUE);
	// now the lines:
	char txt[256];
	int x,y,xmax;
	// start with XMin
	sprintf(txt,ZOOM_MIN,GetXLabel());
	(void) new wxMessage(&dialog,txt,10);
	sprintf(txt,"%g",XMin);
	wxText *dxmin= new wxText(&dialog,NULL,(char*)NULL,txt,-1,-1,100);
	dxmin->GetPosition(&xmax,&y);
	dialog.NewLine();
	// XMax
	sprintf(txt,ZOOM_MAX,GetXLabel());
	(void) new wxMessage(&dialog,txt,10);
	sprintf(txt,"  %g",XMax);
	wxText *dxmax= new wxText(&dialog,NULL,(char*)NULL,txt,-1,-1,100);
	dxmax->GetPosition(&x,&y);
	if (x>xmax) { xmax=x; }
	dialog.NewLine();
	// YMin
	sprintf(txt,ZOOM_MIN,GetYLabel());
	(void) new wxMessage(&dialog,txt,10);
	sprintf(txt,"  %g",YMin);
	wxText *dymin= new wxText(&dialog,NULL,(char*)NULL,txt,-1,-1,100);
	dymin->GetPosition(&x,&y);
	if (x>xmax) { xmax=x; }
	dialog.NewLine();
	// YMax
	sprintf(txt,ZOOM_MAX,GetYLabel());
	(void) new wxMessage(&dialog,txt,10);
	sprintf(txt,"  %g",YMax);
	wxText *dymax= new wxText(&dialog,NULL,(char*)NULL,txt,-1,-1,100);
	dymax->GetPosition(&x,&y);
	if (x>xmax) { xmax=x; }
	dialog.NewLine();
	// now realign all
	dxmin->SetSize(xmax,-1,-1,-1);
	dxmax->SetSize(xmax,-1,-1,-1);
	dymin->SetSize(xmax,-1,-1,-1);
	dymax->SetSize(xmax,-1,-1,-1);
	// now add OK anc CANCEL Button
	wxButton *Ok=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,OKAY);
	Ok->SetDefault();
	wxButton *Cancel=new wxButton(&dialog,GENERAL_BUTTON_HANDLER,CANCEL);
	// set buttons inactive
	Ok->SetClientData(0);
	Cancel->SetClientData(0);
	// now fit
	dialog.Fit();
	// fit the buttons
	{
	  int w,h,x,y;
	  dialog.GetSize(&w,&h);
	  Ok->GetPosition(&x,&y);
	  int spacing=dialog.GetHorizontalSpacing();
	  int width=(w-x-2*spacing)/2;
	  Ok->SetSize(x,y,width,-1);
	  Cancel->SetSize(x+width+spacing,y,width,-1);
	}

	// now centre it
	dialog.Centre();
	// now show it
	dialog.Show(TRUE);
	// OK pressed?
	if (Ok->GetClientData())
	  {
	    // Ok, set values;
	    sscanf(dxmin->GetValue(),"  %g",&XMin);
	    sscanf(dxmax->GetValue(),"  %g",&XMax);
	    sscanf(dymin->GetValue(),"  %g",&YMin);
	    sscanf(dymax->GetValue(),"  %g",&YMax);
	    // refresh Plot
	    myPlotArea->Refresh();
	  }
	break;
      }
    case M_FILE_CLOSE:
      if (OnClose())
	{
	  delete this;
	}
      break;
    case M_FILE_PRINT:
      OnPrint();
      break;
    // help pages
    default:
      if (HelpMenu(id)==FALSE)
	{
	  (void) wxMessageBox(MENU_NOTDEFINED,
			      MENU_WARNING,wxOK|wxCENTRE);
	}
      break;
    }
}
