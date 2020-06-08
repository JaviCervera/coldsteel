#include "../common.h"
#include "string.h"


class StringFactory : public asIStringFactory {
public:
    static StringFactory* Instance() {
        if (!instance) instance = new StringFactory;
        return instance;
    }

    const void* GetStringConstant(const char* data, asUINT length) {
        asAcquireExclusiveLock();
        stringc str(data, length);
        map<stringc, int>::Node* node = stringCache.find(str);
        if (node) {
            node->setValue(node->getValue() + 1);
        } else {
            stringCache.insert(str, 1);
            node = stringCache.find(str);
        }
        asReleaseExclusiveLock();
        return reinterpret_cast<const void*>(&node->getKey());
    }

    int ReleaseStringConstant(const void* str) {
        if (!str) return asERROR;
        int ret = asSUCCESS;
        asAcquireExclusiveLock();
        map<stringc, int>::Node* node = stringCache.find((const char*)str);
        if (node) {
            node->setValue(node->getValue() - 1);
            if (node->getValue() == 0) stringCache.remove(node);
        } else {
            ret = asERROR;
        }
        asReleaseExclusiveLock();
        return ret;
    }

    int GetRawStringData(const void *str, char *data, asUINT *length) const {
        if (!str) return asERROR;
        if (length) *length = (asUINT)reinterpret_cast<const stringc*>(str)->size();
        if (data) memcpy(data, reinterpret_cast<const stringc*>(str)->c_str(), reinterpret_cast<const stringc*>(str)->size());
        return asSUCCESS;
    }

    map<stringc, int> stringCache;
private:
    static StringFactory* instance;

    StringFactory() {}
};


StringFactory* StringFactory::instance = NULL;


static void ConstructString(asIScriptGeneric* gen) {
    new (gen->GetObject())stringc();
}


static void CopyConstructString(asIScriptGeneric* gen) {
    stringc* a = static_cast<stringc*>(gen->GetArgObject(0));
    new (gen->GetObject())stringc(*a);
}


static void DestructString(asIScriptGeneric* gen) {
    stringc* ptr = static_cast<stringc*>(gen->GetObject());
    ptr->~stringc();
}


static void AssignString(asIScriptGeneric* gen) {
    stringc* a = static_cast<stringc*>(gen->GetArgObject(0));
    stringc* self = static_cast<stringc*>(gen->GetObject());
    *self = *a;
    gen->SetReturnAddress(self);
}


static void AddAssignString(asIScriptGeneric* gen) {
    stringc* a = static_cast<stringc*>(gen->GetArgObject(0));
    stringc* self = static_cast<stringc*>(gen->GetObject());
    *self += *a;
    gen->SetReturnAddress(self);
}


static void StringEquals(asIScriptGeneric* gen) {
    stringc* a = static_cast<stringc*>(gen->GetObject());
    stringc* b = static_cast<stringc*>(gen->GetArgAddress(0));
    *(bool*)gen->GetAddressOfReturnLocation() = (*a == *b);
}


static void StringCmp(asIScriptGeneric * gen) {
    stringc* a = static_cast<stringc*>(gen->GetObject());
    stringc* b = static_cast<stringc*>(gen->GetArgAddress(0));
    int cmp = 0;
    if (*a < *b) cmp = -1;
    else if(*b < *a) cmp = 1;
    *(int*)gen->GetAddressOfReturnLocation() = cmp;
}


static void StringAdd(asIScriptGeneric * gen) {
    stringc* a = static_cast<stringc*>(gen->GetObject());
    stringc* b = static_cast<stringc*>(gen->GetArgAddress(0));
    stringc ret = *a + *b;
    gen->SetReturnObject(&ret);
}


void RegisterString(asIScriptEngine *engine) {
    engine->RegisterObjectType("string", sizeof(stringc), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    engine->RegisterStringFactory("string", StringFactory::Instance());
    engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructString), asCALL_GENERIC);
    engine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const string &in)", asFUNCTION(CopyConstructString), asCALL_GENERIC);
    engine->RegisterObjectBehaviour("string", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructString), asCALL_GENERIC);
    engine->RegisterObjectMethod("string", "string &opAssign(const string &in)", asFUNCTION(AssignString), asCALL_GENERIC);
    engine->RegisterObjectMethod("string", "string &opAddAssign(const string &in)", asFUNCTION(AddAssignString), asCALL_GENERIC);
    engine->RegisterObjectMethod("string", "bool opEquals(const string &in) const", asFUNCTION(StringEquals), asCALL_GENERIC);
    engine->RegisterObjectMethod("string", "int opCmp(const string &in) const", asFUNCTION(StringCmp), asCALL_GENERIC);
    engine->RegisterObjectMethod("string", "string opAdd(const string &in) const", asFUNCTION(StringAdd), asCALL_GENERIC);
}
