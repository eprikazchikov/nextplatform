#include <QtTest>

#include "tst_variant.h"
#include "tst_object.h"
#include "tst_objectsystem.h"

int main(int argc, char *argv[]) {
    int status = 0;
    {
        VariantTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }
    {
        ObjectTest tc;
        //status |= QTest::qExec(&tc, argc, argv);
    }
    {
        ObjectSystemTest tc;
        //status |= QTest::qExec(&tc, argc, argv);
    }
    return status;
}
