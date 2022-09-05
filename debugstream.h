/**
 * @file debugstream.h
 * @author gxt_kt (gxt_kt@163.com)
 * @brief  If you have use the qDebuf() function of "Qt", you must use this module easily.
 * And the qDebug is change name to gDebug here. The detail see the "@attention".
 * The github address is https://github.com/gxt-kt/gDebugV2.0
 * @version 0.1
 * @date 2022-09-05
 *
 * @copyright Copyright (c) 2022
 *
 * @attention You can use the default gDebug/gDebug() function to output the debug stream.
 * Such as gDebug("hello world"); , Complexly you can write like gDebug("hello") << "world"; and so on.
 * And the default gDebug/gDebug() has enable the space and newline.
 * If you use the class DebugStream create a new instantiation.The space funciton is exist but the
 * auto newline is invalid.
 */

#ifndef DEBUGSTREAM_H__
#define DEBUGSTREAM_H__

#include <string>
#include <cstdarg>
#include <cstring>
#include <cstdio>

// There will no debug stream output if define the NO_DEBUG_OUTPUT
//#define NO_DEBUG_OUTPUT

// Define the CallBackFunction type
typedef void (*DebugSendStringCallBack)(const char *str, int num);

// The default call back function : use the printf to send the stream
void DebugSendStringCallBack_Default_(const char *str, int num);

// Set the endl compatible with std::endl;
class DebugStreamEndl {};
extern class DebugStreamEndl endl;

class DebugStream {
 public:
  //======================================
  explicit DebugStream \
  (DebugSendStringCallBack fun=DebugSendStringCallBack_Default_, int buf_len = 256);
  DebugStream(const DebugStream &obj);
  DebugStream& operator=(const DebugStream &obj);
  ~DebugStream();
  //===============================================================
  inline DebugStream &OutEn(bool en)    {out_en=en;   return *this;}
  inline DebugStream &NoNewLine()       {newline=false;   return *this;}
  inline DebugStream &Space()           {space=true;  return *this;}
  inline DebugStream &NoSpace()         {space=false; return *this;}
  //===============================================================
  //=========================================

  //=========================================
  inline DebugStream &print()                                    {return *this;}
  inline DebugStream &operator<<(const DebugStream& stream)      {newline=false;return MayBeSpace();}
  inline DebugStream &operator()()                        {return *this;}
  inline DebugStream &operator<<(DebugStreamEndl struct_) {(*this)<<"\n";
    return *this;}

  inline DebugStream &operator<<(const char *str)         {(*this)("%s",str);return MayBeSpace();}
  inline DebugStream &operator<<(const std::string& str)  {(*this)("%s",str.c_str());return MayBeSpace();}
  inline DebugStream &operator<<(bool t)                  {(*this)(t?"true":"false");return MayBeSpace();}
  inline DebugStream &operator<<(char t)                  {(*this)("%c",t);return MayBeSpace();}
  inline DebugStream &operator<<(signed short t)          {(*this)("%d",t);return MayBeSpace();}
  inline DebugStream &operator<<(unsigned short t)        {(*this)("%d",t);return MayBeSpace();}
  inline DebugStream &operator<<(signed int t)            {(*this)("%d",t);return MayBeSpace();}
  inline DebugStream &operator<<(unsigned int t)          {(*this)("%d",t);return MayBeSpace();}
  inline DebugStream &operator<<(signed long t)           {(*this)("%ld",t);return MayBeSpace();}
  inline DebugStream &operator<<(unsigned long t)         {(*this)("%ld",t);return MayBeSpace();}
  inline DebugStream &operator<<(signed long long t)      {(*this)("%ld",t);return MayBeSpace();}
  inline DebugStream &operator<<(unsigned long long t)    {(*this)("%ld",t);return MayBeSpace();}
  inline DebugStream &operator<<(float t)                 {(*this)("%f",t);return MayBeSpace();}
  inline DebugStream &operator<<(double t)                {(*this)("%lf",t);return MayBeSpace();}
  inline DebugStream &operator<<(const void * t)          {return *this;}
  inline DebugStream &operator<<(std::nullptr_t)          {(*this)("(nullptr)");return *this;}
  inline DebugStream &operator<<(const char16_t* t)       {(*this)("%s",t);return MayBeSpace();}
  inline DebugStream &operator<<(char16_t t)              {(*this)("%c",t);return MayBeSpace();}
  inline DebugStream &operator<<(char32_t t)              {(*this)("%c",t);return MayBeSpace();}

  //======================================
 public:
  DebugStream &operator()(const char *fmt, ...);

 private:
  inline DebugStream &MayBeSpace() {
    if (!newline_&&space) (*this)(" ");
    newline_ = false;
    return *this;
  }

 private:
  DebugSendStringCallBack fun;
  char *DebugStreamBuf;
  int buf_len{256};
  bool out_en{true};
  bool space{true};
  bool newline{true};
  bool newline_{false}; // solve the bug that add newline still add space
};



#define gDebug DebugStream(DebugSendStringCallBack_Default_).print()


#endif //DEBUGSTREAM_H__
