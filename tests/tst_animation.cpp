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
        anim.setKeyFrames({ KeyFrame(0.0f, 0.0f),
                            KeyFrame(1.0f, 10.0f) });
        anim.setCurrentTime(200);
        QCOMPARE(anim.currentValue().toInt(), 2);
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, 1.0f),
                            KeyFrame(1.0f, 2.0f) });
        anim.setCurrentTime(250);
        QCOMPARE(anim.currentValue().toFloat(), 1.25f);
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector2(1.0f, 2.0f)),
                            KeyFrame(1.0f, Vector2(3.0f, 4.0f)) });
        anim.setCurrentTime(300);
        QCOMPARE(anim.currentValue().toVector2(), Vector2(1.6f, 2.6f));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector3(1.0f, 2.0f, 3.0f)),
                            KeyFrame(1.0f, Vector3(3.0f, 4.0f, 5.0f)) });
        anim.setCurrentTime(300);
        QCOMPARE(anim.currentValue().toVector3(), Vector3(1.6f, 2.6f, 3.6f));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Vector4(1.0f, 2.0f, 3.0f, 4.0f)),
                            KeyFrame(0.5f, Vector4(4.0f, 5.0f, 6.0f, 7.0f)),
                            KeyFrame(1.0f, Vector4(3.0f, 4.0f, 5.0f, 6.0f)) });
        anim.setCurrentTime(500);
        Vector4 v   = anim.currentValue().toVector4();
        QCOMPARE(v, Vector4(4.0f, 5.0f, 6.0f, 7.0f));

        anim.setCurrentTime(750);
        QCOMPARE(anim.currentValue().toVector4(), Vector4(3.5f, 4.5f, 5.5f, 6.5f));
    }
    {
        anim.setKeyFrames({ KeyFrame(0.0f, Quaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f)),
                            KeyFrame(1.0f, Quaternion(Vector3(0.0f, 1.0f, 0.0f), 90.0f)) });
        anim.setCurrentTime(500);

        Quaternion result   = anim.currentValue().toQuaternion();
        Quaternion expected = Quaternion(Vector3(0.0f, 1.0f, 0.0f), 45.0f);
        //QCOMPARE(result, expected);
    }
}

void AnimationTest::Property_animation() {
    PropertyAnimation anim;
    anim.setLoopDuration(1000);

    TestObject object;
    anim.setTarget(&object, "vec");
    QCOMPARE((anim.target() != nullptr), true);

    anim.setKeyFrames({ KeyFrame(0.0f, Vector2(0.0f, 0.0f)),
                        KeyFrame(1.0f, Vector2(1.0f, 2.0f)) });
    anim.setCurrentTime(500);

    QCOMPARE(object.getVector(), Vector2(0.5, 1.0f));
}
