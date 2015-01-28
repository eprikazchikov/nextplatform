#ifndef AJSON_H
#define AJSON_H

#include <string>
#include <list>
#include <map>

#include "avariant.h"

using namespace std;

class AObject;

class AJsonNode {
public:
    types                       type                        ();
    void                        setType                     (const types type);

    void                        setValue                    (const AVariant &value);
    void                        setValue                    (const string &name, const AVariant &value);

protected:
    enum types {
        VARIANT,
        ARRAY,
        OBJECT
    };

    types                       mType;
    AVariant                    mValue;
    list<AJsonNode>             mArray;
    map<string, AJsonNode>      mMap;
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
