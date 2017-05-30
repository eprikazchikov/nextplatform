#ifndef PLANE_H_HEADER_INCLUDED
#define PLANE_H_HEADER_INCLUDED

struct APlane {
    APlane ()                                                               {}
    APlane (const AVector3D &v1, const AVector3D &v2, const AVector3D &v3)  {
        setPoints(v1, v2, v3);
    }

    float distance(const AVector3D &p) {
        return dot(normal, p) - d;
    }

    void setPoints(const AVector3D &v1, const AVector3D &v2, const AVector3D &v3) {
        AVector3D aux1, aux2;
        aux1    = v2 - v1;
        aux2    = v3 - v1;
        normal  = cross(aux1, aux2);
        point   = v1;
        d       = dot(normal, point);
    }

    AVector3D normal;
    AVector3D point;
    float d;
};

#endif /* PLANE_H_HEADER_INCLUDED */
