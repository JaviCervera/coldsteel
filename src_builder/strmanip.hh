#pragma once

#include <irrString.h>
#include <irrArray.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#ifndef realpath
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#endif
#endif

namespace strmanip
{
  template <typename T>
  irr::core::stringc fromnumber(const T &val, const char *format)
  {
    char buf[32];
    sprintf(buf, format, val);
    return irr::core::stringc(buf);
  }

  inline irr::core::stringc fromint(int val)
  {
    return fromnumber(val, "%i");
  }

  inline irr::core::stringc fromdouble(double val)
  {
    return fromnumber(val, "%f");
  }

  template <typename T>
  T tonumber(const irr::core::stringc &str, const char *format)
  {
    T ret = 0;
    sscanf(str.c_str(), format, &ret);
    return ret;
  }

  inline int toint(const irr::core::stringc &str)
  {
    return tonumber<int>(str, "%i");
  }

  inline float tofloat(const irr::core::stringc &str)
  {
    return tonumber<float>(str, "%f");
  }

  inline double todouble(const irr::core::stringc &str)
  {
    return tonumber<double>(str, "%lf");
  }

  inline irr::core::stringc replaceall(const irr::core::stringc &str, const irr::core::stringc &find, const irr::core::stringc &rep)
  {
    irr::core::stringc result = str;
    result.replace(find, rep);
    return result;
  }

  inline irr::core::stringc stripext(const irr::core::stringc &filename)
  {
    irr::s32 pos = filename.findLast('.');
    if (pos != -1)
      return filename.subString(0, pos);
    return filename;
  }

  inline irr::core::stringc stripdir(const irr::core::stringc &filename)
  {
    irr::s32 pos = filename.findLast('\\');
    if (pos == -1)
      pos = filename.findLast('/');
    if (pos != -1)
      return filename.subString(pos + 1, filename.size() - pos - 1);
    return filename;
  }

  inline irr::core::stringc extractext(const irr::core::stringc &filename)
  {
    irr::s32 pos = filename.findLast('.');
    if (pos != -1)
      return filename.subString(pos + 1, filename.size() - pos - 1);
    return "";
  }

  inline irr::core::stringc extractdir(const irr::core::stringc &filename)
  {
    irr::s32 pos = filename.findLast('\\');
    if (pos == -1)
      pos = filename.findLast('/');
    if (pos != -1)
      return filename.subString(0, pos);
    return filename;
  }

  inline irr::core::stringc read(const irr::core::stringc &filename)
  {
    FILE *f = fopen(filename.c_str(), "rb");
    if (!f)
      return irr::core::stringc();
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)malloc(size + 1);
    fread(buf, sizeof(char), size, f);
    buf[size] = '\0';
    irr::core::stringc str(buf);
    free(buf);
    return str;
  }

  inline void write(const irr::core::stringc &str, const irr::core::stringc &filename, bool append = true)
  {
    FILE *f = fopen(filename.c_str(), append ? "ab" : "wb");
    if (!f)
      return;
    fwrite(str.c_str(), sizeof(char), str.size(), f);
    fclose(f);
  }

  inline irr::core::array<irr::core::stringc> split(const irr::core::stringc &str, char delim)
  {
    irr::core::array<irr::core::stringc> arr;
    if (str.size() == 0 || delim == 0)
      return arr;
    irr::core::stringc out;
    for (irr::u32 i = 0; i < str.size(); ++i)
    {
      const char c = str[i];
      if (c == delim)
      {
        arr.push_back(out);
        out = "";
      }
      else
      {
        out += c;
      }
    }
    arr.push_back(out);
    return arr;
  }

  inline irr::core::stringc upper(const irr::core::stringc &str)
  {
    irr::core::stringc out;
    for (irr::u32 i = 0; i < str.size(); ++i)
      out.append(toupper(str[i]));
    return out;
  }

  inline irr::core::stringc lower(const irr::core::stringc &str)
  {
    irr::core::stringc out;
    for (irr::u32 i = 0; i < str.size(); ++i)
      out.append(tolower(str[i]));
    return out;
  }

  inline irr::core::stringc ltrim(const irr::core::stringc &str)
  {
    irr::u32 i = 0;
    while (i < str.size() && isspace(str[i]))
      ++i;
    return str.subString(i, str.size() - i);
  }

  inline irr::core::stringc rtrim(const irr::core::stringc &str)
  {
    irr::s32 i = (irr::s32)str.size() - 1;
    irr::s32 pos = -1;
    while (i >= 0 && pos == -1)
    {
      if (!isspace(str[(irr::u32)i]))
        pos = i + 1;
      --i;
    }
    if (pos == -1)
      pos = (irr::s32)str.size();
    return str.subString(0, pos);
  }

  inline irr::core::stringc trim(const irr::core::stringc &str)
  {
    return rtrim(ltrim(str));
  }

  inline irr::core::stringc lset(const irr::core::stringc &str, irr::u32 length, char c)
  {
    if (str.size() > length)
    {
      return str.subString(0, (irr::s32)length);
    }
    else if (str.size() < length)
    {
      irr::core::stringc prefix;
      for (irr::u32 i = 0; i < length - str.size(); ++i)
        prefix.append(c);
      return prefix + str;
    }
    return str;
  }

  inline irr::core::stringc rset(const irr::core::stringc &str, irr::u32 length, char c)
  {
    irr::core::stringc out = str;
    if (out.size() > length)
      out = out.subString(0, (irr::s32)length);
    while (out.size() < length)
      out.append(c);
    return out;
  }
} // namespace strmanip
