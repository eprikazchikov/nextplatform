#ifndef TST_SERIALIZATION_H
#define TST_SERIALIZATION_H

#include <QObject>

#include "core/variant.h"

class SerializationTest: public QObject {
    Q_OBJECT
private slots:
    void initTestCase();

    void Json_Serialize_Desirialize();
    void Bson_Serialize_Desirialize();

private:
    VariantMap  var1;
};

#endif // TST_SERIALIZATION_H

