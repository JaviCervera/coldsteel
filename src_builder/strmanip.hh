#pragma once

#include "string.hh"
#include "vector.hh"
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
  std::string fromnumber(const T &val, const char *format)
  {
    char buf[32];
    sprintf(buf, format, val);
    return std::string(buf);
  }

  inline std::string fromint(int val)
  {
    return fromnumber(val, "%i");
  }

  inline std::string fromdouble(double val)
  {
    return fromnumber(val, "%f");
  }

  template <typename T>
  T tonumber(const std::string &str, const char *format)
  {
    T ret = 0;
    sscanf(str.c_str(), format, &ret);
    return ret;
  }

  inline int toint(const std::string &str)
  {
    return tonumber<int>(str, "%i");
  }

  inline float tofloat(const std::string &str)
  {
    return tonumber<float>(str, "%f");
  }

  inline double todouble(const std::string &str)
  {
    return tonumber<double>(str, "%lf");
  }

  inline std::string replaceall(const std::string &str, const std::string &find, const std::string &rep)
  {
    std::string strcopy = str;
    size_t find_pos = strcopy.find(find);
    while (find_pos != std::string::npos)
    {
      strcopy.replace(find_pos, find.length(), rep);
      find_pos = strcopy.find(find, find_pos + rep.length());
    }
    return strcopy;
  }

  inline std::string stripext(const std::string &filename)
  {
    return filename.substr(0, filename.rfind('.'));
  }

  inline std::string stripdir(const std::string &filename)
  {
    size_t find_pos = filename.rfind('\\');
    if (find_pos == std::string::npos)
      find_pos = filename.rfind('/');
    return (find_pos != std::string::npos)
               ? filename.substr(find_pos + 1, filename.length() - find_pos - 1)
               : filename;
  }

  inline std::string extractext(const std::string &filename)
  {
    size_t find_pos = filename.rfind('.');
    return (find_pos != std::string::npos)
               ? filename.substr(find_pos + 1, filename.length() - find_pos - 1)
               : "";
  }

  inline std::string extractdir(const std::string &filename)
  {
    size_t find_pos = filename.rfind('\\');
    if (find_pos == std::string::npos)
      find_pos = filename.rfind('/');
    return filename.substr(0, find_pos);
  }

  inline std::string read(const std::string &filename)
  {
    FILE *f = fopen(filename.c_str(), "rb");
    if (!f)
      return "";
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char *)malloc(size + 1);
    fread(buf, sizeof(char), size, f);
    buf[size] = '\0';
    std::string str(buf);
    free(buf);
    return str;
  }

  inline void write(const std::string &str, const std::string &filename, bool append = true)
  {
    FILE *f = fopen(filename.c_str(), append ? "ab" : "wb");
    if (!f)
      return;
    fwrite(str.c_str(), sizeof(char), str.length(), f);
    fclose(f);
  }

  inline std::vector<std::string> split(const std::string &str, char delim)
  {
    std::vector<std::string> arr;
    if (str.length() == 0 || delim == 0)
      return arr;
    std::string out;
    for (size_t i = 0; i < str.length(); ++i)
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

  // todo: optimize this
  inline std::string upper(const std::string &str)
  {
    std::string out;
    for (size_t i = 0; i < str.length(); ++i)
      out += toupper(str[i]);
    return out;
  }

  // todo optimize this
  inline std::string lower(const std::string &str)
  {
    std::string out;
    for (size_t i = 0; i < str.length(); ++i)
      out += tolower(str[i]);
    return out;
  }

  inline std::string ltrim(const std::string &str)
  {
    // count spaces at the beginning
    size_t i = 0;
    while (i < str.length() && isspace(str[i]))
      ++i;

    // return trimmed string
    return str.substr(i);
  }

  inline std::string rtrim(const std::string &str)
  {
    // look for first non space on the right
    int i = str.length() - 1;
    size_t pos = std::string::npos;
    while (i > 0 && pos == std::string::npos)
    {
      if (!isspace(str[i]))
        pos = i + 1;
      --i;
    }

    if (pos == std::string::npos)
      pos = str.length();

    // return trimmed string
    return str.substr(0, pos);
  }

  inline std::string trim(const std::string &str)
  {
    return rtrim(ltrim(str));
  }

  inline std::string lset(const std::string &str, size_t length, char c)
  {
    std::string out = str;
    if (out.length() > length)
    {
      out.resize(length);
    }
    else if (out.length() < length)
    {
      out = std::string(length - out.length(), c) + out;
    }
    return out;
  }

  inline std::string rset(const std::string &str, size_t length, char c)
  {
    std::string out = str;
    out.resize(length, c);
    return out;
  }
} // namespace strmanip
