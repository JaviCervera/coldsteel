#include <ctype.h>
#include <stdlib.h>
#include "core.h"
#include "string.h"

extern "C" {


EXPORT int CALL csVal(const char* str) {
    return atoi(str);
}


EXPORT float CALL csValF(const char* str) {
    return atof(str);
}


EXPORT const char* CALL csStr(int number) {
    static char str[32];
    sprintf(str, "%i", number);
    return str;
}


EXPORT const char* CALL csStrF(float number) {
    static char str[32];
    sprintf(str, "%f", number);
    return str;
}


EXPORT const char* CALL csLeft(const char* str, int num) {
    static stringc retstr;
    retstr = stringc(str).subString(0, num);
    return retstr.c_str();
}


EXPORT const char* CALL csRight(const char* str, int num) {
    static stringc retstr;
    retstr = str;
    retstr = retstr.subString(retstr.size() - num, num);
    return retstr.c_str();
}


EXPORT const char* CALL csMid(const char* str, int pos, int num) {
    static stringc retstr;
    retstr = stringc(str).subString(pos, num);
    return retstr.c_str();
}


EXPORT const char* CALL csReplace(const char* str, const char* find, const char* replace) {
    static stringc retstr;
    retstr = stringc(str).replace(find, replace);
    return retstr.c_str();
}


EXPORT int CALL csFind(const char* str, const char* find, int offset) {
    return stringc(str).find(find, offset);
}


EXPORT const char* CALL csUpper(const char* str) {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i) retstr[i] = toupper(retstr[i]);
    return retstr.c_str();
}


EXPORT const char* CALL csLower(const char* str) {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i) retstr[i] = tolower(retstr[i]);
    return retstr.c_str();
}


EXPORT const char* CALL csTrim(const char* str) {
    static stringc retstr;
    retstr = stringc(str).trim();
    return retstr.c_str();
}


EXPORT const char* CALL csChar(int code) {
    static stringc retstr;
    retstr = " ";
    retstr[0] = (char)code;
    return retstr.c_str();
}


EXPORT int CALL csCode(const char* str) {
    return (int)str[0];
}


EXPORT int CALL csLen(const char* str) {
    return (int)strlen(str);
}


EXPORT const char* CALL csField(const char* str, const char* delimiter, int index) {
    static stringc retstr;
    array<stringc> split;
    stringc(str).split(split, delimiter, 1, false);
    retstr = (split.size() >= index) ? split[index] : "";
    return retstr.c_str();
}


EXPORT const char* CALL csLoadString(const char* filename) {
    static stringc retstr;
    IReadFile* file = _csDevice()->getFileSystem()->createAndOpenFile(filename);
    if (file) {
        long size = file->getSize();
        char* str = (char*)malloc(size + 1);
        str[size] = 0;
        file->read(str, size);
        retstr = str;
        free(str);
        file->drop();
    } else {
        retstr = "";
    }
    return retstr.c_str();
}


EXPORT void CALL csSaveString(const char* str, const char* filename, bool_t append) {
    IWriteFile* file = _csDevice()->getFileSystem()->createAndWriteFile(filename, append);
    file->write(str, strlen(str));
    file->drop();
}


} // extern "C"
