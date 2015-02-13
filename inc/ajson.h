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

    types                       type                        () const;
    void                        setType                     (types type);

    AJson                      &property                    (const string &key);

    void                        appendProperty              (const AVariant &value, const string &name = "");
    AJson                      &appendProperty              (const AJson &value, const string &name = "");

protected:
    inline void                 skipSpaces                  (const char *data, unsigned int &p);

    inline bool                 isSpace                     (char c);
    inline bool                 isDigit                     (char c);
};

#endif // AJSON_H
