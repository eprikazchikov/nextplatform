#ifndef TST_JSON_H
#define TST_JSON_H

#include <QObject>

#define JSON_STUB2 " \
{ \
    \"Reference\": \"thor://factory/AObject\", \
    \"Name\": \"TestObject\", \
    \"ID\": 1, \
    \"Properties\": [ \
        { \
            \"Name\": \"TestProperty1\", \
            \"Flags\": 1, \
            \"Data\": true \
        },\
        { \
            \"Name\": \"TestProperty2\", \
            \"Flags\": 1, \
            \"Data\": 1 \
        },\
        { \
            \"Name\": \"TestProperty3\", \
            \"Flags\": 1, \
            \"Data\": 1.0 \
        },\
        { \
            \"Name\": \"TestProperty4\", \
            \"Flags\": 1, \
            \"Data\": \"1.0\" \
        } \
    ] \
}"

class JsonTest: public QObject {
    Q_OBJECT
private slots:
    void Serialize_Desirialize();

};

#endif // TST_JSON_H
