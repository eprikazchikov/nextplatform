#ifndef TST_JSON_H
#define TST_JSON_H

#include <QObject>

class JsonTest: public QObject {
    Q_OBJECT
private slots:
    void Serialize_Desirialize();

};

#endif // TST_JSON_H
