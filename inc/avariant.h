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

#include <amath.h>

class AObject;

class AVariant {
public:
    typedef AVector4D           Color;

    /*! \enum Type */
    enum Type {
        ANY                     = 0,
        BOOL                    = 1,
        INT                     = 2,
        FLOAT                   = 3,
        STRING                  = 4,
        OBJECT                  = 5,
        VECTOR                  = 6,
        COLOR                   = 8
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
    };

public:
    AVariant                    ();
    AVariant                    (Type type);
    AVariant                    (bool value);
    AVariant                    (int value);
    AVariant                    (float value);
    AVariant                    (const char *value);
    AVariant                    (const std::string &value);
    AVariant                    (const AVector3D &value);
    AVariant                    (const Color &value);
    AVariant                    (bool *value);
    AVariant                    (int *value);
    AVariant                    (float *value);
    AVariant                    (std::string *value);
    AVariant                    (AVector3D *value);
    AVariant                    (Color *value);
    AVariant                    (AObject *value);

    ~AVariant                   ();

    AVariant                    operator =                  (const AVariant &value);

    Type                        type                        ();

    bool                        isConvertible               ();

    // Conversion and getters
    const bool                  toBool                      ();
    const int                   toInt                       ();
    const float                 toFloat                     ();
    const std::string           toString                    ();
    const AVector3D             toVector                    ();
    const Color                 toColor                     ();

protected:
    Data                        mData;

};

#endif // AVARIANT_H
