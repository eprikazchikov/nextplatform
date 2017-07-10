#ifndef TST_OBJECTSYSTEM_H
#define TST_OBJECTSYSTEM_H

#include <QObject>

class AObjectSystem;

class ObjectSystemTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void Object_Instansing();
    void Serialize_Desirialize_Object();
    void RegisterUnregister_Object();
    void Override_Object();

private:
};

#endif // TST_OBJECTSYSTEM_H
