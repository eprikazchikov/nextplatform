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

    /*! \enum Type */
    enum Type {
        NONE                    = 0,
        BOOL,
        INT,
        FLOAT,
        STRING,
        MAP,
        LIST,

        VECTOR2D                = 10,
        VECTOR3D,
        VECTOR4D,
        QUATERNION,
        MATRIX3D,
        MATRIX4D,
        CURVE
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
    AVariant                    (const string &value);
    AVariant                    (const AVariantMap &value);
    AVariant                    (const AVariantList &value);

    AVariant                    (const AVector2D &value);
    AVariant                    (const AVector3D &value);
    AVariant                    (const AVector4D &value);
    AVariant                    (const AQuaternion &value);
    AVariant                    (const AMatrix3D &value);
    AVariant                    (const AMatrix4D &value);
    AVariant                    (const ACurve &value);

    AVariant                    (bool *value);
    AVariant                    (int *value);
    AVariant                    (float *value);
    AVariant                    (string *value);

    AVariant                    (AVector2D *value);
    AVariant                    (AVector3D *value);
    AVariant                    (AVector4D *value);
    AVariant                    (AQuaternion *value);
    AVariant                    (AMatrix3D *value);
    AVariant                    (AMatrix4D *value);
    AVariant                    (ACurve *value);

    ~AVariant                   ();

    AVariant                   &operator=                   (const AVariant &value);

    bool                        operator==                  (const AVariant &right) const;
    bool                        operator!=                  (const AVariant &right) const;

    Type                        type                        () const;
    bool                        isShared                    () const;

    // Conversion and getters
    const bool                  toBool                      () const;
    const int                   toInt                       () const;
    const float                 toFloat                     () const;
    const std::string           toString                    () const;

    const AVariantMap           toMap                       () const;
    const AVariantList          toList                      () const;

    const AVector2D             toVector2D                  () const;
    const AVector3D             toVector3D                  () const;
    const AVector4D             toVector4D                  () const;
    const AQuaternion           toQuaternion                () const;
    const AMatrix3D             toMatrix3D                  () const;
    const AMatrix4D             toMatrix4D                  () const;
    const ACurve                toCurve                     () const;

    void                        appendProperty              (const AVariant &value, const string &name = "");

protected:
    Data                        mData;

};

#endif // AVARIANT_H
