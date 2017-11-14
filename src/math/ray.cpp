#include "math/amath.h"

ARay::ARay() {
}

ARay::ARay(const AVector3D &p, const AVector3D &d) :
        pos(p),
        dir(d) {
}

bool ARay::intersect(const AVector3D &p, areal r, AVector3D *pt) {
    AVector3D l = p - pos;
    areal tca   = l.dot(dir);
    if(tca < 0) {
        return false;
    }

    areal d2    = l.dot(l) - tca * tca;
    if(d2 > r * r) {
        return false;
    }

    if(pt) {
        areal thc   = sqrt(r * r - d2);
        areal t0    = tca - thc;
        areal t1    = tca + thc;

        if(t0 < 0.001) {
            t0      = t1;
        }

        *pt         = pos + dir * t0;
    }

    return true;
}

bool ARay::intersect(const APlane &p, AVector3D *pt, bool back) {
    AVector3D n = p.normal;
    areal d     = dir.dot(n);
    if(d >= 0.0f) {
        if(back) {
            n   = -n;
            d   = dir.dot(n);
        } else {
            return false;
        }
    }

    areal t = -n.dot(pos - p.point) / d;
    if(t <= 0.0) {
        return false;
    }

    if(pt) {
        *pt = pos + dir * t;
    }

    return true;
}

bool ARay::intersect(const AABox &b, AVector3D *pt) {
    AVector3D min, max;
    b.box(min, max);

    bool inside = true;
    char quadrant[3];
    AVector3D candidate;

    for(int i = 0; i < 3; i++) {
        if(pos[i] < min[i]) {
            quadrant[i]     = 1;
            candidate[i]    = min[i];
            inside          = false;
        } else if(pos[i] > max[i]) {
            quadrant[i]     = 0;
            candidate[i]    = max[i];
            inside          = false;
        } else	{
            quadrant[i]     = 2;
        }
    }

    if(inside)	{
        if(pt) {
            *pt = pos;
        }
         return true;
    }

    AVector3D maxT;
    for(int i = 0; i < 3; i++) {
        if(quadrant[i] != 2 && dir[i] != 0.0f) {
            maxT[i] = (candidate[i] - pos[i]) / dir[i];
        } else {
            maxT[i] = -1.0f;
        }
    }

    int whichPlane = 0;
    for(int i = 1; i < 3; i++) {
        if(maxT[whichPlane] < maxT[i]) {
            whichPlane = i;
        }
    }

    if(maxT[whichPlane] < 0.0f) {
        return false;
    }

    AVector3D coord;
    for(int i = 0; i < 3; i++) {
        if(whichPlane != i) {
            coord[i] = pos[i] + maxT[whichPlane] * dir[i];
            if(coord[i] < min[i] || coord[i] > max[i]) {
                return false;
            }
        } else {
            coord[i] = candidate[i];
        }
    }
    if(pt) {
        *pt = coord;
    }

    return true;
}

bool ARay::intersect(const AVector3D &v1, const AVector3D &v2, const AVector3D &v3, AVector3D *pt, bool back) {
    AVector3D ip;
    if(!intersect(APlane(v1, v2, v3), &ip, back)) {
        return false;
    }

    AVector3D ve0   = v3 - v1;
    AVector3D ve1   = v2 - v1;
    AVector3D ve2   = ip - v1;
    areal dot00     = ve0.dot(ve0);
    areal dot01     = ve0.dot(ve1);
    areal dot02     = ve0.dot(ve2);
    areal dot11     = ve1.dot(ve1);
    areal dot12     = ve1.dot(ve2);
    areal invDenom  = 1.0f / (dot00 * dot11 - dot01 * dot01);
    AVector2D b     = AVector2D((dot11 * dot02 - dot01 * dot12) * invDenom, (dot00 * dot12 - dot01 * dot02) * invDenom);

    if((b.x >= 0) && (b.y >= 0) && (b.x + b.y <= 1.0f)) {
        if(pt) {
            *pt     = ip;
        }
        return true;
    }
    return false;
}

ARay ARay::reflect(const AVector3D &n, const AVector3D &p) {
    ARay ret;

    ret.pos     = p;
    ret.dir     = dir - n * ((areal)2.0 * dir.dot(n));
    ret.dir.normalize();

    return ret;
}

ARay ARay::refract(const AVector3D &n, const AVector3D &p, areal c0, areal c1) {
    ARay ret;

    areal eta   = c0 / c1;
    areal theta = n.dot(dir);
    areal k     = (areal)1.0 - eta * eta * ((areal)1.0 - theta * theta);

    ret.pos     = p;
    ret.dir     = dir * eta - n * (eta * theta + sqrt(k));
    ret.dir.normalize();

    return ret;
}

ARay ARay::diffuse(const AVector3D &n, const AVector3D &p, areal min, areal max) {
    ARay ret;

    areal r1    = (areal)2.0 * PI * RANGE(min, max);
    areal r2    = RANGE(min, max);
    areal r2s   = sqrt(r2);

    AVector3D u = (fabs(n.x) > .1 ? AVector3D(0, 1, 0) : AVector3D(1)).cross(n);
    u.normalize();
    AVector3D v = n.cross(u);

    ret.pos     = p;
    ret.dir     = u * cos(r1) * r2s + v * sin(r1) * r2s + n * sqrt(1 - r2);
    ret.dir.normalize();

    return ret;
}
