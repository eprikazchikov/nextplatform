#ifndef TST_ANIMATION_H
#define TST_ANIMATION_H

#include <QObject>

class AnimationTest : public QObject {
    Q_OBJECT
private slots:
    void Base_animation();
    void Variant_animation();
    void Property_animation();
};

#endif // TST_ANIMATION_H
