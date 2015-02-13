/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef AVARIANT_H
#define AVARIANT_H

#include <string>
#include <map>
#include <list>

#include <amath.h>

using namespace std;

class AVariant {
public:
    typedef map<string, AVariant>   AVariantMap;
    typedef list<AVariant>          AVariantList;

    typedef AVector4D           Color;

    /*! \enum Type */
    enum Type {
        ANY                     = 0,
        BOOL                    = 1,
        INT                     = 2,
        FLOAT                   = 3,
        STRING                  = 4,
        VECTOR                  = 5,
        COLOR                   = 6,
        MAP                     = 7,
        LIST                    = 8
    };

    struct Data {
        Data                    ();
        Type                    type;

        bool                    shared;

        union Union {
            bool                b;
            int                 i;
            float               f;
            void               *so;
        } base;

        std::string             s;
        AVector3D               v;
        Color                   c;
        AVariantMap             m;
        AVariantList            l;
    };

public:
    AVariant                    ();
    AVariant                    (const AVariant &copy);
    AVariant                    (Type type);
    AVariant                    (bool value);
    AVariant                    (int value);
    AVariant                    (float value);
    AVariant                    (const char *value);
    AVariant                    (const std::string &value);
    AVariant                    (const AVariantMap &value);
    AVariant                    (const AVariantList &value);
    /// \todo: Are they realy needed?
    AVariant                    (const AVector3D &value);
    AVariant                    (const Color &value);

    AVariant                    (bool *value);
    AVariant                    (int *value);
    AVariant                    (float *value);
    AVariant                    (std::string *value);
    /// \todo: Are they realy needed?
    AVariant                    (AVector3D *value);
    AVariant                    (Color *value);

    ~AVariant                   ();

    AVariant                   &operator=                   (const AVariant &value);

    bool                        operator==                  (const AVariant &right) const;
    bool                        operator!=                  (const AVariant &right) const;

    Type                        type                        () const;

    // Conversion and getters
    const bool                  toBool                      () const;
    const int                   toInt                       () const;
    const float                 toFloat                     () const;
    const std::string           toString                    () const;
    const AVector3D             toVector                    () const;
    const Color                 toColor                     () const;
    const AVariantMap           toMap                       () const;
    const AVariantList          toList                      () const;

    void                        appendProperty              (const AVariant &value, const string &name = "");

protected:
    inline bool                 toFloatArray                (float *v, const AVariantList &list);

    Data                        mData;

};

#endif // AVARIANT_H
