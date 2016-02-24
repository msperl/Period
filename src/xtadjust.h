/*
 * Program:     Period98
 *
 * File:        xtadjust.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

//@ManMemo: ADJUST DATA

void AdjustData(wxWindow * Frame);

///
void AdjustQUIT(wxButton& calc,wxEvent& event);
///
void AdjustAdjust(wxButton& calc,wxEvent& event);
///
void AdjustSelection(wxButton &Butt, wxEvent &);

///
void AdjustFillBox(int id);

//@ManMemo: SUBDIVIDE DATA

void SubDivideData(wxWindow * Frame);

///
void SubDivideQUIT(wxButton& calc,wxEvent& event);
///
void SubDivideDoIt(wxButton& calc,wxEvent& event);

//@ManMemo: RENAME SELECTION

void RenameSelection(wxWindow * Frame);
///
void RenameQUIT(wxButton& calc,wxEvent& event);
///
void RenameDoIt(wxButton& calc,wxEvent& event);

