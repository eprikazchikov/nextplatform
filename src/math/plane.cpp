#include "amath.h"

APlane::APlane() {

}
APlane::APlane(const AVector3D &v1, const AVector3D &v2, const AVector3D &v3)  {
    AVector3D aux1, aux2;
    aux1    = v2 - v1;
    aux2    = v3 - v1;
    normal  = aux1.cross(aux2);
    //normal.normalize();
    point   = v1;
    d       = normal.dot(point);
}

areal APlane::distance(const AVector3D &p) {
    return normal.dot(p) - d;
}
