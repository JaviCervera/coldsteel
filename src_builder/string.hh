#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace std
{
  class string
  {
  public:
    static const size_t npos = -1;

    string(const char *s = "");
    string(char c);
    string(size_t n, char c);
    string(const string &str);
    ~string();

    bool operator==(const string &other) const;
    bool operator!=(const string &other) const;
    bool operator<(const string &other) const;
    bool operator>(const string &other) const;
    string operator+(const string &other) const;
    string operator+(char c) const;
    string &operator=(const string &other);
    string &operator=(char c);
    string &operator+=(const string &other);
    string &operator+=(char c);
    char &operator[](size_t index);
    const char &operator[](size_t index) const;

    size_t length() const { return len; }
    size_t size() const { return len; }
    const char *c_str() const { return buffer; }

    size_t find(const string &str, size_t pos = 0) const;
    size_t rfind(const string &str, size_t pos = npos) const;
    string &replace(size_t pos, size_t len, const string &str);
    void resize(size_t n, char c = 0);
    string substr(size_t pos, size_t len = npos) const;

  private:
    char *buffer;
    size_t buffer_size;
    size_t len;

    static const char *strrstr(const char *str, const char *find, size_t endofs = npos);
  };

  inline string::string(const char *s)
      : buffer(0), buffer_size(0), len(0)
  {
    resize(strlen(s));
    strcpy(buffer, s);
  }

  inline string::string(char c)
      : buffer(0), buffer_size(0), len(0)
  {
    resize(1, c);
  }

  inline string::string(size_t n, char c)
      : buffer(0), buffer_size(0), len(0)
  {
    resize(n, c);
  }

  inline string::string(const string &str)
      : buffer(0), buffer_size(0), len(0)
  {
    resize(str.length());
    strcpy(buffer, str.buffer);
  }

  inline string::~string()
  {
    free(buffer);
  }

  inline bool string::operator==(const string &other) const
  {
    return strcmp(buffer, other.buffer) == 0;
  }

  inline bool string::operator!=(const string &other) const
  {
    return strcmp(buffer, other.buffer) != 0;
  }

  inline bool string::operator<(const string &other) const
  {
    return strcmp(buffer, other.buffer) < 0;
  }

  inline bool string::operator>(const string &other) const
  {
    return strcmp(buffer, other.buffer) > 0;
  }

  inline string string::operator+(const string &other) const
  {
    char *buf = (char *)malloc(len + other.len + 1);
    strcpy(buf, buffer);
    strcat(buf, other.buffer);
    string str(buf);
    free(buf);
    return str;
  }

  inline string string::operator+(char c) const
  {
    char s[2] = {c, '\0'};
    return *this + s;
  }

  inline string &string::operator=(const string &other)
  {
    if (buffer_size < other.len + 1)
    {
      buffer_size = sizeof(char) * (other.len + 1);
      free(buffer);
      buffer = (char *)malloc(buffer_size);
    }
    len = other.len;
    strcpy(buffer, other.buffer);
    return *this;
  }

  inline string &string::operator=(char c)
  {
    char s[2] = {c, '\0'};
    return *this = s;
  }

  inline string &string::operator+=(const string &other)
  {
    return *this = *this + other;
  }

  inline string &string::operator+=(char c)
  {
    char s[2] = {c, '\0'};
    return *this += s;
  }

  inline char &string::operator[](size_t index)
  {
    return buffer[index];
  }

  inline const char &string::operator[](size_t index) const
  {
    return buffer[index];
  }

  inline size_t string::find(const string &str, size_t pos) const
  {
    const char *p = strstr(&buffer[pos], str.buffer);
    if (p == NULL)
      return npos;
    else
      return (p - buffer);
  }

  inline size_t string::rfind(const string &str, size_t pos) const
  {
    const char *p = strrstr(buffer, str.buffer, pos);
    if (p == NULL)
      return npos;
    else
      return (p - buffer);
  }

  inline string &string::replace(size_t pos, size_t len, const string &str)
  {
    size_t len2 = str.length();
    if (len == npos)
      len = length() - pos;

    // increase buffer if needed
    if (buffer_size <= length() - len + len2)
    {
      buffer_size = length() - len + len2 + 1;
      buffer = (char *)realloc(buffer, buffer_size);
    }

    // move trailing chars if needed
    if (len != len2)
    {
      memmove(&this->buffer[pos + len2], &this->buffer[pos + len], this->len - pos - len);
    }

    // copy string chars
    strncpy(&buffer[pos], str.c_str(), len2);

    // update length
    this->len += len2 - len;
    buffer[this->len] = 0;

    return *this;
  }

  inline void string::resize(size_t n, char c)
  {
    const size_t prev_len = len;
    if (buffer_size <= n)
    {
      buffer_size = n + 1;
      buffer = (char *)realloc(buffer, buffer_size);
    }
    len = n;
    if (len > prev_len)
      memset(&buffer[prev_len], c, len - prev_len);
    buffer[len] = 0;
  }

  // todo: optimize this
  inline string string::substr(size_t pos, size_t len) const
  {
    if (len == npos)
      len = size() - pos;
    char *buf = (char *)malloc(len + 1);
    strncpy(buf, &buffer[pos], len);
    buf[len] = '\0';
    string str(buf);
    free(buf);
    return str;
  }

  inline const char *string::strrstr(const char *str, const char *find, size_t endofs)
  {
    if (endofs == npos)
      endofs = strlen(str);
    size_t len = strlen(find);
    for (const char *cp = str + endofs - len; cp >= str; --cp)
    {
      if (strncmp(cp, find, len) == 0)
        return cp;
    }
    return NULL;
  }

  inline string operator+(const char *cstr, const string &str)
  {
    return string(cstr) + str;
  }
} // namespace std
