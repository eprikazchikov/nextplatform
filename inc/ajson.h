#ifndef AJSON_H
#define AJSON_H

#include <string>
#include <list>
#include <map>
#include <stack>

#include "avariant.h"

using namespace std;

class AObject;

class AJsonValue {
public:
    enum types {
        VARIANT,
        ARRAY,
        OBJECT
    };

    types                       type                        ();
    void                        setType                     (const types type);

    void                        setValue                    (const string &name, const AVariant &value);
    void                        setValue                    (const string &name, AJsonValue *value);

protected:
    types                       mType;
    AVariant                    mValue;
    list<AJsonValue *>          mArray;
    map<string, AJsonValue *>   mMap;
};

class AJson {
public:
    static string               objectSave                  (const AObject &object);
    static AObject             *objectLoad                  (const string &data);

    bool                        parse                       (const string &data);

protected:
    void                        skipSpaces                  (const char *data, unsigned int &p);

    inline bool                 isSpace                     (char c);
    inline bool                 isDigit                     (char c);

};

#endif // AJSON_H
