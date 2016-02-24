#include "xgmainfr.h"
#include "xgmenu.h"
#include "ldialtxt.h"

void CMainFrame::OnMenuCommand(int id)
{
  // Are we Calculating?
  if (IsCalculating)
    {
      // Do nothing...
      return;
    }
  if (!(
      FileMenu(id)
      ||
      TimestringMenu(id)
      ||
      FitMenu(id)
      ||
      FourierMenu(id)
      ||
      LogMenu(id)
      ||
      HelpMenu(id)
      ))
    {
      (void)wxMessageBox(MENU_NOTDEFINED,MENU_WARNING,wxOK|wxCENTRE);
    }
  // clean Status-message
  if (id != M_FILE_QUIT) 
    {
      SetStatusText("",0);
    }
}
