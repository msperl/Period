/*
 * Program:     Period98
 *
 * File:        complex.cpp
 * Purpose:     implementation-file for
 *              a complex class
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#include "complex.h"

ostream& operator<<(ostream& st, const mycomplex &c)
{
  return st<<'('<<c.r<<','<<c.i<<')';
}

istream& operator>>(istream& st, mycomplex &)
{
  return st;
}
