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

    Copyright: 2008-2014 Evgeniy Prikazchikov
*/

#ifndef AVARIANT_H
#define AVARIANT_H

#include <map>
#include <list>
#include <string>

#include <acommon.h>

#include <amath.h>
#include <ametatype.h>

#define STRUCTURE "__S__"
#define DATA "__D__"

using namespace std;

class NEXT_LIBRARY_EXPORT AVariant {
public:
    typedef map<string, AVariant>  AVariantMap;
    typedef list<AVariant>         AVariantList;


    struct Data {
        Data                    ();

        uint32_t                type;
        void                   *so;
    };

public:
    AVariant                    ();
    AVariant                    (AMetaType::Type type);
    AVariant                    (bool value);
    AVariant                    (int value);
    AVariant                    (double value);
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

    AVariant                    (uint32_t type, void *copy);

    ~AVariant                   ();

    AVariant                   &operator=                   (const AVariant &value);

    bool                        operator==                  (const AVariant &right) const;
    bool                        operator!=                  (const AVariant &right) const;

    uint32_t                    type                        () const;
    void                       *data                        () const;

    bool                        canConvert                  (uint32_t type) const;

    template<typename T>
    bool                        canConvert                  () const {
        return AVariant::canConvert(AMetaType::type<T>());
    }

    template<typename T>
    T                           value                       () const {
        uint32_t type   = AMetaType::type<T>();
        if(mData.so) {
            if(mData.type == type) {
                return *(reinterpret_cast<const T *>(mData.so));
            } else if(canConvert(type)) {
                T result;

                AMetaType::convert(mData.so, mData.type, &result, type);

                return result;
            }
        }
        return T();
    }

    template<typename T>
    static AVariant             fromValue                   (const T &value) {
        AVariant result;
        result.mData.type   = AMetaType::type<T>();
        if(result.mData.type != AMetaType::Invalid) {
            result.mData.so = AMetaType::create(result.mData.type, reinterpret_cast<const void *>(&value));
        }
        return result;
    }

    // Conversion and getters
    bool                        toBool                      () const;
    int                         toInt                       () const;
    double                      toDouble                    () const;
    const string                toString                    () const;

    const AVariantMap           toMap                       () const;
    const AVariantList          toList                      () const;

    const AVector2D             toVector2D                  () const;
    const AVector3D             toVector3D                  () const;
    const AVector4D             toVector4D                  () const;
    const AQuaternion           toQuaternion                () const;
    const AMatrix3D             toMatrix3D                  () const;
    const AMatrix4D             toMatrix4D                  () const;
    const ACurve                toCurve                     () const;

protected:
    Data                        mData;

};

#endif // AVARIANT_H
