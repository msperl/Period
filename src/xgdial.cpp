/*
 * Program:     Period98
 *
 * File:        xgdial.h
 * Purpose:     Implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "xgdial.h"

Bool myDialogBox::OnClose(void)
{
  int result=wxDialogBox::OnClose();
  Show(FALSE);
  return result;
}
