#include <QtTest>

#include "tst_variant.h"
#include "tst_object.h"
#include "tst_objectsystem.h"

#define addReport(args, tpl, ts) { \
    if(!tpl.isEmpty()) { \
        QString arg = tpl; \
        arg.replace('*', ts.metaObject()->className()); \
        args.append("-o"); \
        args.append(arg); \
    } \
} \

#define Cleanup(args, tpl) { \
    if(!tpl.isEmpty()) { \
        args.removeLast(); \
        args.removeLast(); \
    } \
} \

int main(int argc, char *argv[]) {
    QStringList args;
    for(int i = 0; i < argc; i++) {
        args.append(argv[i]);
    }

    QString tpl;
    int index   = args.indexOf("-t");
    if(index != -1) {
        tpl = args.at(index + 1);
        args.removeAt(index);
        args.removeAt(index);
    }

    int status = 0;
    {
        VariantTest ts;
        addReport(args, tpl, ts);
        status |= QTest::qExec(&ts, args);
        Cleanup(args, tpl);
    }
    {
        ObjectTest ts;
        addReport(args, tpl, ts);
        status |= QTest::qExec(&ts, args);
        Cleanup(args, tpl);
    }
    {
        ObjectSystemTest ts;
        addReport(args, tpl, ts);
        status |= QTest::qExec(&ts, args);
        Cleanup(args, tpl);
    }
    return status;
}
