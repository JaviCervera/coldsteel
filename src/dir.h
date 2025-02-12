#pragma once

#include "common.h"

bool_t AddZip(const char *filename);
const char *DirContents(const char *dir);
void ChangeDir(const char *dir);
const char *CurrentDir();
