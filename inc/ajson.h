#ifndef AJSON_H
#define AJSON_H

#include <string>
#include <list>
#include <map>

#include "avariant.h"

using namespace std;

class AObject;

class AJson {
public:
    static string               objectSave                  (const AObject &object);
    static AObject             *objectLoad                  (const string &data);

    bool                        parse                       (const string &data);

protected:
    enum States {
        objectBegin = 1,
        objectEnd,
        arrayBegin,
        arrayEnd,
        propertyNext,
        propertyName,
        propertyValue
    };

    typedef map<string, AVariant>   objectMap;

    void                        skipSpaces                  (const char *data, unsigned int &p);

    inline bool                 isSpace                     (char c);
    inline bool                 isDigit                     (char c);

};

#endif // AJSON_H
