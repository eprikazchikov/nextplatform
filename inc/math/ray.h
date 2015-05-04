#ifndef RAY_H_HEADER_INCLUDED
#define RAY_H_HEADER_INCLUDED

struct ARay {
    ARay()                                                          {}
    ARay(const AVector3D &p, const AVector3D &d) : pos(p), dir(d)   {}

    inline bool intersect(const AVector3D &p, float r, AVector3D *pt) {
        AVector3D l = p - pos;
        float tca   = l.dot(dir);
        if(tca < 0)
            return false;

        float d2    = l.dot(l) - tca * tca;
        if(d2 > r * r)
            return false;

        if(pt) {
            float thc   = sqrt(r * r - d2);
            float t0    = tca - thc;
            float t1    = tca + thc;

            if(t0 < 0.001)
                t0      = t1;

            *pt         = pos + dir * t0;
        }

        return true;
    }

    inline bool intersect(APlane &p, AVector3D *pt, bool back = false) {
        float d = dir.dot(p.normal);
        if(d >= 0.0f) {
            if(back)
                p.normal    = -p.normal;
            else
                return false;
        }

        float t = p.normal.dot(p.normal * p.d - pos) / d;
        if(t <= 0.0)
            return false;

        if(pt)
            *pt = pos + dir * t;

        return true;
    }

    inline bool intersect(const AABox &b, AVector3D *pt) {
        AVector3D min, max;
        b.get_box(min, max);

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
            if(pt)
                *pt = pos;
             return true;
        }

        AVector3D maxT;
        for(int i = 0; i < 3; i++) {
            if(quadrant[i] != 2 && dir[i] != 0.0f)
                maxT[i] = (candidate[i] - pos[i]) / dir[i];
            else
                maxT[i] = -1.0f;
        }

        int whichPlane = 0;
        for(int i = 1; i < 3; i++) {
            if(maxT[whichPlane] < maxT[i])
                whichPlane = i;
        }

        if(maxT[whichPlane] < 0.0f)
            return false;

        AVector3D coord;
        for(int i = 0; i < 3; i++) {
            if(whichPlane != i) {
                coord[i] = pos[i] + maxT[whichPlane] * dir[i];
                if(coord[i] < min[i] || coord[i] > max[i])
                    return false;
            } else {
                coord[i] = candidate[i];
            }
        }
        if(pt)
            *pt = coord;

        return true;
    }

    inline bool intersect(const AVector3D &v1, const AVector3D &v2, const AVector3D &v3, AVector3D *pt, bool back = false) {
        AVector3D ip;
        if(!intersect(APlane(v1, v2, v3), &ip, back))
            return false;

        AVector3D ve0   = v3 - v1;
        AVector3D ve1   = v2 - v1;
        AVector3D ve2   = ip - v1;
        float dot00     = ve0.dot(ve0);
        float dot01     = ve0.dot(ve1);
        float dot02     = ve0.dot(ve2);
        float dot11     = ve1.dot(ve1);
        float dot12     = ve1.dot(ve2);
        float invDenom  = 1.0f / (dot00 * dot11 - dot01 * dot01);
        AVector2D b     = AVector2D((dot11 * dot02 - dot01 * dot12) * invDenom, (dot00 * dot12 - dot01 * dot02) * invDenom);

        if((b.x >= 0) && (b.y >= 0) && (b.x + b.y <= 1.0f)) {
            if(pt)
                *pt     = ip;
            return true;
        }

        return false;
    }

    inline ARay reflect(const AVector3D &n, const AVector3D &p) {
        ARay ret;

        ret.pos     = p;
        ret.dir     = dir - n * ((float)2.0 * dir.dot(n));
        ret.dir.normalize();

        return ret;
    }

    inline ARay refract(const AVector3D &n, const AVector3D &p, float c0, float c1) {
        ARay ret;

        float eta   = c0 / c1;
        float theta = n.dot(dir);
        float k     = (float)1.0 - eta * eta * ((float)1.0 - theta * theta);

        ret.pos     = p;
        ret.dir     = dir * eta - n * (eta * theta + sqrt(k));
        ret.dir.normalize();

        return ret;
    }
    /*
    inline ARay diffuse(const AVector3D &n, const AVector3D &p, ARange &seed) {
        ARay ret;

        float r1    = (float)2.0 * PI * seed.frand();
        float r2    = seed.frand();
        float r2s   = sqrt(r2);

        AVector3D u = (fabs(n.x) > .1 ? AVector3D(0, 1, 0) : AVector3D(1)) % n;
        u.normalize();
        AVector3D v = n % u;

        ret.pos     = p;
        ret.dir     = u * cos(r1) * r2s + v * sin(r1) * r2s + n * sqrt(1 - r2);
        ret.dir.normalize();

        return ret;
    }
    */
    AVector3D pos;      // Origin of ray
    AVector3D dir;      // Direction of ray
};

#endif // RAY_H_HEADER_INCLUDED
