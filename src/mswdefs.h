#ifndef __mswdefs_h
#define __mswdefs_h

#ifdef wx_msw
inline int strcasecmp(const char * t1, const char * t2)
{
  return (_stricmp(t1,t2));
}

inline int strncasecmp(const char * t1, const char * t2, size_t n)
{
  return (_strnicmp(t1,t2,n));
}
#endif // wx_msw

#endif // __mswdefs_h
