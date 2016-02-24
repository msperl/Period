/*
 * Program:     Period98
 *
 * File:        dglistfo.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
//
//
//

#ifndef __list_fw_h_
#define __list_fw_h_

#include "lists.h"
#include "genptr.h"
;
///
template <class type>
class TBase: public type
{
public:
  ///
  typedef TPtr<type> myPtr;

  ///
  TBase()
  :CFourier("None active"),mActiveIs(-1)
  {}
  ///
  ~TBase() {}

  ///
  int GetListEntries() const
  { return mList.entries(); }
  ///
  char const *GetTitle(int i) const
  { return (*mList[i]).GetTitle(); }
  ///
  void MakeActive(int i)
  { (type)*this=(*mList[i]); mActiveIs=i; }
  ///
  type *PrepareCalculations()
  {
    type * tmp= new type(*this);
    mList.addtail(myPtr(tmp));
    return tmp;
  }
  
protected:
  ///
  TList<myPtr> mList;
  ///
  int mActiveIs;
};

#endif




