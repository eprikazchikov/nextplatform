#ifndef TST_MATH_H
#define TST_MATH_H

#include <QObject>

class MathTest: public QObject {
    Q_OBJECT
private slots:
    void Rotate_to_Matrix();
    void Generate_Gaussian_Kernel();
};

#endif // TST_MATH_H
