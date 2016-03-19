#ifndef TST_BSON_H
#define TST_BSON_H

#include <QObject>

class BsonTest: public QObject {
    Q_OBJECT
private slots:
    void Serialize_Desirialize();

};

#endif // TST_BSON_H

