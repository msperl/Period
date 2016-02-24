#include "xgtable.h"
#include "xgmenu.h"

void CMyTable::OnMenuCommand(int id)
{
  switch (id)
    {
    case M_FILE_CLOSE:
      if (OnClose())
	{
	  delete this;
	}
      break;
    case M_FILE_PRINT:
      OnPrint();
      break;
    default:
      (void) wxMessageBox(MENU_NOTDEFINED,
			  MENU_WARNING,wxOK|wxCENTRE);
      break;
    }
}

