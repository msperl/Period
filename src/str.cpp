/*
 * Program:     Period98
 *
 * File:        str.cpp
 * Purpose:     implementation-file for
 *              ?
 * Author:      Martin Sperl
 * Created:     1996
 * Copyright:   (c) 1996-1998, Martin Sperl
 */

#include "str.h"
#include <string.h>

//
// constructors
//

char * myString::createcopy(char const * txt) const
{
  char * tmp;
  if (txt==0)
    {
      tmp=new char(0);
    }
  else
    {
      int i,j,len;
      len=length(txt);
      tmp=new char[len+1];
      j=0;
      for (i=0;i<=len;i++)
	{
#ifndef wx_msw
	  if (txt[i]!=13)
#endif
	    {
	      tmp[j]=txt[i];
	      j++;
	    }
	}
    }
  return tmp;
}

myString::myString()
{
  data=createcopy("");
}

myString::myString(myString const & str)
{
  data=createcopy(str.chars());
}

myString::myString(char const * txt)
{
  data=createcopy(txt);
}

myString::myString(char ch)
{
  char tmp[2]={ch,0};
  data=createcopy(tmp);
}

//
// Destructor code
//

myString::~myString()
{
  RemoveData();
}

void myString::RemoveData()
{
  if (data!=0)
    {
      delete data;
    }
  data=0;
}

//
// assignment code
//

myString& myString::operator=(myString const & str)
{
  RemoveData();
  data=createcopy(str.chars());
  return *this;
}

myString& myString::operator=(char const * psz)
{
  RemoveData();
  data=createcopy(psz);
  return *this;
}

myString& myString::operator=(char ch)
{
  return operator=(myString(ch));
}

//
// length code
//

int myString::length() const
{
  return length(data);
}

int myString::length(char const * txt) const
{
  if (txt==0)
    {
      return 0;
    }
  int i;
  for(i=0;txt[i]!=0;i++)
    {;}
  return i;
}

char * myString::chars() const
{
  return data;
}

//
// concentration code
//

char * myString::concentrate(char const *txt1, char const *txt2) const
{
  char * tmp;
  if (txt1==0)
    {
      return createcopy(txt2);
    }
  if (txt2==0)
    {
      return createcopy(txt1);
    }
  int i,len1,len2;
  len1=length(txt1);
  len2=length(txt2);
  tmp=new char[len1+len2+1];
  for (i=0;i<len1;i++)
    {
      tmp[i]=txt1[i];
    }
  for (i=0;i<=len2;i++)
    {
      tmp[len1+i]=txt2[i];
    }
  return tmp;
}

myString& myString::operator+=(myString const& str)
{
  char * tmp=concentrate(data,str.chars());
  RemoveData();
  data=tmp;
  return *this;
}

myString& myString::operator+=(char const * psz)
{
  char * tmp=concentrate(data,psz);
  RemoveData();
  data=tmp;
  return *this;
}

myString & myString::operator+=(char ch)
{
  return operator+=(myString(ch));
}

myString operator+(myString const & str1, const myString& str2)
{
  myString tmp(str1);
  tmp+=str2;
  return tmp;
}

myString operator+(myString const & str, char const * psz)
{
  myString tmp(str);
  tmp+=psz;
  return tmp;
}

myString operator+(char const * psz, myString const & str)
{
  myString tmp(psz);
  tmp+=str;
  return tmp;
}

myString operator+(char ch, myString const& str)
{
  myString tmp(ch);
  tmp+=str;
  return tmp;
}

myString operator+(myString const& str,char ch)
{
  myString tmp(str);
  tmp+=ch;
  return tmp;
}

//
// compare code
//

int myString::Compare(char const *psz1, char const *psz2) const
{
  if ((psz1==NULL)&&(psz2==NULL))
    { return 0; }
  if ((psz1==NULL)&&(psz2!=NULL))
    { return 1; }
  if ((psz1!=NULL)&&(psz2==NULL))
    { return -1; }
  return strcmp(psz1,psz2);
}

int myString::Compare(const char * psz) const
{
  return Compare(data,psz);
}

int myString::Compare(myString const & str) const
{
  return Compare(data,str.chars());
}

int myString::CompareNoCase(const char * psz) const
{
#ifdef wx_msw
	return _stricmp(data,psz);
#else
	return strcasecmp(data,psz);
#endif
}

int myString::CompareNoCase(myString const & str) const
{
  return CompareNoCase(str.chars());
}

//
// case changer code
//

void myString::MakeLower(char * tmp) const
{
  if (tmp==0)
    {
      return;
    }
  int len=length(tmp);
  int i;
  for(i=0;i<len;i++)
    {
      if ( (tmp[i]>64) && (tmp[i]<93) )
	{
	  tmp[i]+=32;
	}
    }
}

void myString::MakeUpper(char * tmp) const
{
  if (tmp==0)
    {
      return;
    }
  int len=length(tmp);
  int i;
  for(i=0;i<len;i++)
    {
      if ( (tmp[i]>96) && (tmp[i]<125) )
	{
	  tmp[i]-=32;
	}
    }
}

myString &myString::MakeLower()
{
  MakeLower(data);
  return *this;
}

myString &myString::MakeUpper()
{
  MakeUpper(data);
  return *this;
}

//
// substring code
//

myString myString::Mid(int from,int count) const
{
  int len=length(data);
  if ( from>len )
    {
      return "";
    }
  if (from<0)
    {
      count+=from;
      from=0;
    }
  if (count<0)
    {
      count=len;
    }
  int to=from+count;
  if ( to > len )
    {
      to=len;
    }
  char* copy=new char[to-from+1];
  int i;
  for(i=0;from<to;from++,i++)
    {
      copy[i]=data[from];
    }
  copy[i]=0;
  return copy;
}

myString myString::Left(int to) const
{
  return Mid(0,to);
}

myString myString::Right(int len) const
{
  int from=length(data)-len;
  if (from<0)
    {
      return data;
    }
  return Mid(from,len);
}

int myString::Find(char ch, int start) const
{
  if(start>=length(data))
    {
      return -1;
    }
  while(data[start]!=0)
    {
      if (data[start]==ch)
	{
	  return start;
	}
      start++;
    }
  return -1;
}

int myString::Find(char const * psz, int start) const
{
  if (psz==0)
    {
      return start;
    }
  if(start>=length(data))
    {
      return -1;
    }
  int pos;
  int result;
  while((pos=Find(psz[0],start))!=-1)
    {
      result=Compare(&data[pos],psz);
      if ((result==2)||(result==0))
	{
	  return pos;
	}
      start++;
    }
  return -1;
}

int myString::Find(myString const & str, int start) const
{
  return Find(str.chars(), start);
}

int myString::FindOneOf(char const * psz, int start) const
{
  if (psz==0)
    {
      return -1;
    }
  int i, leni=length(data);
  if (start>=leni)
    {
      return -1;
    }
  int j, lenj=length(psz);
  if (lenj==0)
    {
      return start;
    }
  for(i=start;i<leni;i++)
    {
      for(j=0;j<lenj;j++)
	{
	  if (data[i]==psz[j])
	    {
	      return i;
	    }
	}
    }
  return -1;
}

int myString::FindOneOf(myString const &str, int start) const
{
  return FindOneOf(str.chars(),start);
}

ostream & operator<<(ostream & st,myString const &str)
{
  return (st<<str.chars());
}

char const &myString::operator[](int i) const
{
  if (i<0)
    {
      return data[0];
    }
  if (i>length())
    {
      i=length();
    }
  return data[i];
}















