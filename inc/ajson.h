#ifndef AJSON_H
#define AJSON_H

#include <string>
#include <list>
#include <map>
#include <stack>

#include "avariant.h"

using namespace std;

class AObject;

class AJson {
public:
    typedef list<AJson>         jsonArray;
    typedef map<string, AJson>  jsonMap;

    enum types {
        VARIANT,
        ARRAY,
        OBJECT
    };

protected:
    types                       mType;
    AVariant                    mValue;
    jsonArray                   mArray;
    jsonMap                     mMap;

public:
    AJson                       ();

    bool                        load                        (const string &data);
    string                      save                        (int depth = -1);

protected:
    inline void                 skipSpaces                  (const char *data, unsigned int &p);

    inline bool                 isSpace                     (char c);
    inline bool                 isDigit                     (char c);

    void                        setValue                    (const string &name, const AVariant &value);
    AJson                      &setValue                    (const string &name, AJson &value);
};

#endif // AJSON_H
