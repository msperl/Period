/*
 * Program:     Period98
 *
 * File:        complex.h
 * Purpose:     header-file for
 *              a complex class
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */
#ifndef __complex__
#define __complex__

#include "iostream.h"
#include <math.h>

///

/** This class is a simple implementation of a complex data-type
 */
class mycomplex
{
public:
  //@Man: methodes
  //@{
  
  //@Man: member-methodes
  //@{
  /// constructor
  inline mycomplex(double re=0,double im=0):r(re),i(im) {;}
  /// gives the real part of the complex value
  inline double real() const {return r;}
  /// gives the imaginary part of the complex value
  inline double imag() const {return i;}
  //@}


  //@Man: math-methodes
  //@{
  /// norm of the complex
  friend inline double norm(mycomplex const &t )
  { return sqrt(t.r*t.r+t.i*t.i);}
  
  /// add a complex and assign
  inline mycomplex& operator+=(const mycomplex &c)
  {r+=c.r;i+=c.i;return *this;}
  /// add two complex values
  friend inline mycomplex operator+(const mycomplex &c1,const mycomplex &c2)
  {mycomplex tmp(c1);tmp+=c2;return tmp;}

  /// subtract a complex and assign
  inline mycomplex& operator-=(const mycomplex &c)
  {r-=c.r;i-=c.i;return *this;}
  /// subtract a complex from another
  friend inline mycomplex operator-(const mycomplex &c1,const mycomplex &c2)
  {mycomplex tmp(c1);tmp-=c2;return tmp;}
  
  /// multiply with a real and assign
  inline mycomplex& operator*=(double d)
  {r*=d;i*=d;return *this;}
  /// multiply a real with a complex 
  friend inline mycomplex operator*(double d, const mycomplex &c)
  {mycomplex tmp(c);tmp*=d;return tmp;}
  /// multiply a complex with a real
  friend inline mycomplex operator*(const mycomplex &c, double d)
  {mycomplex tmp(c);tmp*=d;return tmp;}
  
  /// divide with a real and assign
  inline mycomplex& operator/=(double d)
  {r/=d;i/=d;return *this;}
  /// divide a real with a complex
  friend inline mycomplex operator/(double d, const mycomplex &c)
  {mycomplex tmp(c);tmp/=d;return tmp;}
  /// divide a complex with a real
  friend inline mycomplex operator/(const mycomplex &c, double d)
  {mycomplex tmp(c);tmp/=d;return tmp;}

  //@}

  //@Man: IO-methodes
  //@{
  /// outputs the data to a stream
  friend ostream& operator<<(ostream& st, const mycomplex &c);
  /// inputs data from a stream
  friend istream& operator>>(istream& st, mycomplex &c);
  //@}
  
  //@}

  //@Man: parameters
  //@{
private:
  /// Real part of the complex
  double r;
  /// Imaginary part of the complex
  double i;
  //@}


};

#endif
