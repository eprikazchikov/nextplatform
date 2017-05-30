#ifndef TST_OBJECTSYSTEM_H
#define TST_OBJECTSYSTEM_H

#include <QObject>

class AObjectSystem;

class ObjectSystemTest : public QObject {
    Q_OBJECT

private slots:
    void Object_Instansing();
    void Serialize_Desirialize_Object();
    void Override_object();
    void Delta_Serialize_Desirialize_Object();
    void Virtual_Inheritance();

private:
};

#endif // TST_OBJECTSYSTEM_H
