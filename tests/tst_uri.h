#ifndef TST_URI_H
#define TST_URI_H

#include <QObject>

class UriTest : public QObject {
    Q_OBJECT

private slots:
    void Parse_URI();

    void Parse_Path();

};

#endif // TST_URI_H
