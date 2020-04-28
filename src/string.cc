#include <ctype.h>
#include <stdlib.h>
#include "core.h"
#include "string.h"

extern "C" {


EXPORT int CALL asVal(const char* str) {
    return atoi(str);
}


EXPORT float CALL asValF(const char* str) {
    return atof(str);
}


EXPORT const char* CALL asStr(int number) {
    static char str[32];
    sprintf(str, "%i", number);
    return str;
}


EXPORT const char* CALL asStrF(float number) {
    static char str[32];
    sprintf(str, "%f", number);
    return str;
}


EXPORT const char* CALL asLeft(const char* str, int num) {
    static stringc retstr;
    retstr = stringc(str).subString(0, num);
    return retstr.c_str();
}


EXPORT const char* CALL asRight(const char* str, int num) {
    static stringc retstr;
    retstr = str;
    retstr = retstr.subString(retstr.size() - num - 1, num);
    return retstr.c_str();
}


EXPORT const char* CALL asMid(const char* str, int pos, int num) {
    static stringc retstr;
    retstr = stringc(str).subString(pos, num);
    return retstr.c_str();
}


EXPORT const char* CALL asReplace(const char* str, const char* find, const char* replace) {
    static stringc retstr;
    retstr = stringc(str).replace(find, replace);
    return retstr.c_str();
}


EXPORT int CALL asFind(const char* str, const char* find, int offset) {
    return stringc(str).find(find, offset);
}


EXPORT const char* CALL asUpper(const char* str) {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i) retstr[i] = toupper(retstr[i]);
    return retstr.c_str();
}


EXPORT const char* CALL asLower(const char* str) {
    static stringc retstr;
    retstr = str;
    for (u32 i = 0; i < retstr.size(); ++i) retstr[i] = tolower(retstr[i]);
    return retstr.c_str();
}


EXPORT const char* CALL asSTrim(const char* str) {
    static stringc retstr;
    retstr = stringc(str).trim();
    return retstr.c_str();
}


EXPORT const char* CALL asChar(int code) {
    static stringc retstr;
    retstr = " ";
    retstr[0] = (char)code;
    return retstr.c_str();
}


EXPORT int CALL asCode(const char* str) {
    return (int)str[0];
}


EXPORT int CALL asLen(const char* str) {
    return (int)strlen(str);
}


EXPORT const char* CALL asField(const char* str, const char* delimiter, int index) {
    static stringc retstr;
    array<stringc> split;
    stringc(str).split(split, delimiter, 1, false);
    retstr = (split.size() >= index) ? split[index] : "";
    return retstr.c_str();
}


EXPORT const char* CALL asLoadString(const char* filename) {
    static stringc retstr;
    IReadFile* file = _asDevice()->getFileSystem()->createAndOpenFile(filename);
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


EXPORT void CALL asSaveString(const char* str, const char* filename, bool_t append) {
    IWriteFile* file = _asDevice()->getFileSystem()->createAndWriteFile(filename, append);
    file->write(str, strlen(str));
    file->drop();
}


} // extern "C"
