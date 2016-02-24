/*
 * Program:     Period98
 *
 * File:        genptr.h
 * Purpose:     header-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
//
// a generic Pointer class
//@ManMemo:

template <class Type>
class TPtr
{
private:
  ///
  Type * p;
public:
  ///
  static const Type *const nil;
  ///
  TPtr(Type *pI=nil)
    :p(pI) {}
  ///
  Type * operator -> () const {return  p; }
  ///
  Type operator* () const { return *p; }
};

