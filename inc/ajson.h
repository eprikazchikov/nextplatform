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
    AJson                       ();

    static AVariant             load                        (const string &data);
    static string               save                        (const AVariant &data, int depth = -1);

    static inline string        readString                  (const string &data, unsigned int &it);

protected:
    static inline void          skipSpaces                  (const char *data, unsigned int &it);

    static inline bool          isSpace                     (char c);
    static inline bool          isDigit                     (char c);

};

#endif // AJSON_H
