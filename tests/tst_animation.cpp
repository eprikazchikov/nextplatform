#include "tst_animation.h"

#include "tst_common.h"

#include "anim/propertyanimation.h"

#include <QTest>
#include <QDebug>

void AnimationTest::Base_animation() {
    Animation anim;
}

void AnimationTest::Variant_animation() {
    VariantAnimation anim;
    anim.setLoopDuration(1000);
    {
        anim.setKeyFrames({ KeyFrame(0.0f, 0), KeyFrame(1.0f, 10) });
        anim.setCurrentTime(200);
        QCOMPARE(anim.currentValue().toInt(), 2);
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, 1.0f), KeyFrame(1.0f, 2.0f) });
        anim.setCurrentTime(250);
        QCOMPARE(anim.currentValue().toFloat(), 1.25f);
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector2(1, 2)), KeyFrame(1.0f, Vector2(3, 4)) });
        anim.setCurrentTime(300);
        QCOMPARE(anim.currentValue().toVector2(), Vector2(1.6f, 2.6f));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector3(1, 2, 3)), KeyFrame(1.0f, Vector3(3, 4, 5)) });
        anim.setCurrentTime(300);
        QCOMPARE(anim.currentValue().toVector3(), Vector3(1.6f, 2.6f, 3.6f));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector4(1, 2, 3, 4)), KeyFrame(0.5f, Vector4(4, 5, 6, 7)), KeyFrame(1.0f, Vector4(3, 4, 5, 6)) });
        anim.setCurrentTime(500);
        Vector4 v   = anim.currentValue().toVector4();
        QCOMPARE(v, Vector4(4, 5, 6, 7));

        anim.setCurrentTime(750);
        QCOMPARE(anim.currentValue().toVector4(), Vector4(3.5, 4.5, 5.5, 6.5));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Quaternion(Vector3(0, 1, 0), 0)), KeyFrame(1.0f, Quaternion(Vector3(0, 1, 0), 90)) });
        anim.setCurrentTime(500);

        Quaternion result   = anim.currentValue().toQuaternion();
        Quaternion expected = Quaternion(Vector3(0, 1, 0), 45);
        //QCOMPARE(result, expected);
    }
}

void AnimationTest::Property_animation() {
    PropertyAnimation anim;
    anim.setLoopDuration(1000);

    TestObject object;
    anim.setTarget(&object, "vec");
    QCOMPARE((anim.target() != nullptr), true);

    anim.setKeyFrames({ KeyFrame(0.0f, Vector2(0, 0)), KeyFrame(1.0f, Vector2(1, 2)) });
    anim.setCurrentTime(500);

    QCOMPARE(object.getVector(), Vector2(0.5, 1));
}
