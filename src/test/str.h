/*
 * Program:     Period98
 *
 * File:        str.h
 * Purpose:     header-file for
 *              a string class
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#ifndef __str_h__
#define __str_h__

#include <iostream.h>

///

/** This class is a simple implementation of a string datatype
 */
class myString
{
 public:
  //@Man: methodes
  //@{

  //@Man: member-methodes
  //@{

  /// creates an empty string
  myString();
  /// creates a string from another string
  myString(myString const & str);
  /// creates a string from antother string
  myString(char const * psz);
  ///creates a string from a character
  myString(char ch);
  
  //
  /// destructor
  ~myString();

  /// clears string
  void RemoveData();
  //
  // get Length
  //@ManMemo:

  int length() const;

  //
  // get pointer
  //@ManMemo:

  char * chars() const;

  //@}

  //
  //@Man: Assignment
  //@{
  myString& operator=(myString const & str);
  ///
  myString& operator=(char const * psz);
  ///
  myString& operator=(char ch);
  ///
  myString& operator+=(myString const& str);
  ///
  myString& operator+=(char const* psz);
  ///
  myString& operator+=(char ch);
  ///
  friend int operator==(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)==0);}
  ///
  friend int operator==(const myString& str, const char* psz)
  { return (str.Compare(psz)==0);}
  ///
  friend int operator==(const char* psz, const myString& str)
  { return (str.Compare(psz)==0);}
  
  ///
  friend int operator!=(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)!=0);}
  ///
  friend int operator!=(const myString& str, const char* psz)
  { return (str.Compare(psz)!=0);}
  ///
  friend int operator!=(const char* psz, const myString& str)
  { return (str.Compare(psz)!=0);}
  
  ///
  friend int operator<(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)<0); }
  ///
  friend int operator<(const myString& str, const char* psz)
  { return (str.Compare(psz)<0); }
  ///
  friend int operator<(const char* psz, const myString& str)
  { return (str.Compare(psz)>0); }
  
  ///
  friend int operator>(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)>0); }
  ///
  friend int operator>(const myString& str, const char* psz)
  { return (str.Compare(psz)>0); }
  ///
  friend int operator>(const char* psz, const myString& str)
  { return (str.Compare(psz)<0); }
  
  ///
  friend int operator<=(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)<=0); }
  ///
  friend int operator<=(const myString& str, const char* psz)
  { return (str.Compare(psz)<=0); }
  ///
  friend int operator<=(const char* psz, const myString& str)
  { return (str.Compare(psz)>=0); }
  
  ///
  friend int operator>=(const myString& str1, const myString& str2)
  { return (str1.Compare(str2)>=0); }
  ///
  friend int operator>=(const myString& str, const char* psz)
  { return (str.Compare(psz)>=0); }
  ///
  friend int operator>=(const char* psz, const myString& str)
  { return (str.Compare(psz)<=0); }
  //@}


  //
  // comparison
  //@Man:
  //@{  
  ///
  int Compare(const char * psz) const;
  ///
  int Compare(myString const & str) const;
  ///
  int CompareNoCase(const char * psz) const;
  ///
  int CompareNoCase(myString const & str) const;
  //@}

  //
  // case changers
  //@Man:

  myString& MakeUpper();
  ///
  myString& MakeLower();

  //
  // substrings
  ///
  myString Mid(int from, int count=-1) const;
  ///
  myString Left(int to) const;
  ///
  myString Right(int len) const;

  //
  // finds
  //
  ///
  int Find(char ch, int start=0) const;
  ///
  int Find(char const *psz, int start=0) const;
  ///
  int Find(myString const &str, int start=0) const;
  ///
  int FindOneOf(char const * psz, int start=0) const;
  ///
  int FindOneOf(myString const & str, int start=0) const;

  //
  // operator []
  /// 
  char const &operator[](int i) const;

  //
  // protected functions
  //@ManMemo:

 protected:
  char* createcopy(char const *tXt) const;
  ///
  char* concentrate(char const *txtl, char const *txt2) const;
  ///
  int length(char const * txt) const;
  ///
  int Compare(char const *pszl, char const *psz2) const;
  ///
  void MakeUpper(char * psz) const;
  ///
  void MakeLower(char * psz) const;
  ///@}
 protected:
  ///
  char* data;

  
};

///
myString operator+(myString const & strl, const myString& str2);
///
myString operator+(myString const & str, char const * psz);
///
myString operator+(char const * psz, myString const& str);
///
myString operator+(char ch, myString const& str);
///
myString operator+(myString const& str,char ch);
///
ostream & operator<<(ostream & st,myString const &str);



#endif














