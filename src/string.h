#pragma once

#include "common.h"

int Val(const char *str);
float ValF(const char *str);
const char *Str(int number);
const char *StrF(float number);
const char *Left(const char *str, int num);
const char *Right(const char *str, int num);
const char *Mid(const char *str, int pos, int num);
const char *Replace(const char *str, const char *find, const char *replace);
int Find(const char *str, const char *find, int offset);
const char *Upper(const char *str);
const char *Lower(const char *str);
const char *Trim(const char *str);
const char *Char(int code);
int Code(const char *str);
int Len(const char *str);
int SplitCount(const char *str, const char *delimiter);
const char *SplitIndex(const char *str, const char *delimiter, int index);
const char *StripExt(const char *filename);
const char *StripDir(const char *filename);
const char *ExtractExt(const char *filename);
const char *ExtractDir(const char *filename);
const char *LoadString(const char *filename);
void SaveString(const char *str, const char *filename, bool_t append);
