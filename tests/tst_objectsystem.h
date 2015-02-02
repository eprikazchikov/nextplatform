#ifndef TST_OBJECTSYSTEM_H
#define TST_OBJECTSYSTEM_H

#include <QObject>

class ObjectSystemTest : public QObject {
    Q_OBJECT

private slots:
    void Serialize_Desirialize_Object();

};

#endif // TST_OBJECTSYSTEM_H
