#ifndef ABSON
#define ABSON

#include <vector>
#include <cstdint>

#include "avariant.h"

typedef vector<uint8_t>         AByteVector;

class ABson {
public:
    ABson                       ();

    static AVariant             load                        (const AByteVector &data, AMetaType::Type type = AMetaType::VariantMap);
    static AByteVector          save                        (const AVariant &data);

protected:
    enum BsonDataTypes {
        DOUBLE                  = 1,
        STRING,
        OBJECT,
        ARRAY,
        BYNARY,
        UNDEFINED,
        OBJECTID,
        BOOL,
        DATETYME,
        NONE,
        REGEXP,
        DBPOINTER,
        JAVASCRIPT,
        DEPRECATED,
        JAVASCRIPTWS,
        INT32,
        TIMESTAMP,
        INT64,
        MINKEY                  = -1,
        MAXKEY                  = 127
    };

protected:
    static inline uint8_t       type                        (const AVariant &data);

};

#endif // ABSON

