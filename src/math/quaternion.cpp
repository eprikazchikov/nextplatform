#include "amath.h"

AQuaternion::AQuaternion() :
    x(0),
    y(0),
    z(0),
    w(1) {
}
AQuaternion::AQuaternion(const AVector3D &dir, areal angle) {
    areal length = dir.length();
    if(length != 0.0) {
        length = 1.0f / length;
        areal sinangle = (areal)sin(angle * DEG2RAD / 2.0f);
        x = dir[0] * length * sinangle;
        y = dir[1] * length * sinangle;
        z = dir[2] * length * sinangle;
        w = (areal)cos(angle * DEG2RAD / 2.0f);
    } else {
        x = y = z = 0.0;
        w = 1.0;
    }
}

AQuaternion::AQuaternion(const AVector3D &euler) {
    AVector3D rad2(euler.x * DEG2RAD * 0.5f, euler.y * DEG2RAD * 0.5f, euler.z * DEG2RAD * 0.5f);
    AVector3D c(cos(rad2.x), cos(rad2.y), cos(rad2.z));
    AVector3D s(sin(rad2.x), sin(rad2.y), sin(rad2.z));

    w = c.x * c.y * c.z + s.x * s.y * s.z;
    x = s.x * c.y * c.z - c.x * s.y * s.z;
    y = c.x * s.y * c.z + s.x * c.y * s.z;
    z = c.x * c.y * s.z - s.x * s.y * c.z;
}

AQuaternion::AQuaternion(const AMatrix3D &m) {
    areal W = m[0] + m[4] + m[8];
    areal X = m[0] - m[4] - m[8];
    areal Y = m[4] - m[0] - m[8];
    areal Z = m[8] - m[0] - m[4];

    int index = 0;
    areal four = W;
    if(X > four) {
        four    = X;
        index   = 1;
    }
    if(Y > four) {
        four    = Y;
        index   = 2;
    }
    if(Z > four) {
        four    = Z;
        index   = 3;
    }

    areal biggest   = sqrt(four + 1) * 0.5;
    areal mult = 0.25 / biggest;

    switch(index) {
        case 0: {
            w = biggest;
            x = (m[5] - m[7]) * mult; // m[1][2] - m[2][1]
            y = (m[6] - m[2]) * mult; // m[2][0] - m[0][2]
            z = (m[1] - m[3]) * mult; // m[0][1] - m[1][0]
        } break;
        case 1: {
            w = (m[5] - m[7]) * mult; // m[1][2] - m[2][1]
            x = biggest;
            y = (m[1] + m[3]) * mult; // m[0][1] + m[1][0]
            z = (m[6] + m[2]) * mult; // m[2][0] + m[0][2]
        } break;
        case 2: {
            w = (m[6] - m[2]) * mult; // m[2][0] - m[0][2]
            x = (m[1] + m[3]) * mult; // m[0][1] + m[1][0]
            y = biggest;
            z = (m[5] + m[7]) * mult; // m[1][2] + m[2][1]
        } break;
        case 3: {
            w = (m[1] - m[3]) * mult; // m[0][1] - m[1][0]
            x = (m[6] + m[2]) * mult; // m[2][0] + m[0][2]
            y = (m[5] + m[7]) * mult; // m[1][2] + m[2][1]
            z = biggest;
        } break;
        default: break;
    }
}

/// Comparison operators
bool AQuaternion::operator==(const AQuaternion &v) const {
    return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
}
bool AQuaternion::operator!=(const AQuaternion &v) const {
    return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
}

areal &AQuaternion::operator[](int i) {
    return q[i];
}
const areal AQuaternion::operator[](int i) const {
    return q[i];
}

AQuaternion AQuaternion::operator*(const AQuaternion &q) const {
    AQuaternion ret;
    ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
    ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
    ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
    ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
    return ret;
}

AVector3D AQuaternion::operator*(const AVector3D &v) const {
    AVector3D vec(x, y, z);

    AVector3D uv    = vec.cross(v);
    AVector3D uuv   = vec.cross(uv);

    return v + ((uv * w) + uuv) * 2;
}

AQuaternion AQuaternion::inverse() const {
    AQuaternion ret;
    ret.w = w; ret.x =-x; ret.y =-y; ret.z =-z;
    return ret;
}

void AQuaternion::mix(const AQuaternion &q0, const AQuaternion &q1, areal t) {
    areal k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
    AQuaternion q;
    if(cosomega < 0.0) {
        cosomega = -cosomega;
        q.x = -q1.x;
        q.y = -q1.y;
        q.z = -q1.z;
        q.w = -q1.w;
    } else {
        q.x = q1.x;
        q.y = q1.y;
        q.z = q1.z;
        q.w = q1.w;
    }
    if(1.0 - cosomega > 1e-6) {
        areal omega = (areal)acos(cosomega);
        areal sinomega = (areal)sin(omega);
        k0 = (areal)sin((1.0f - t) * omega) / sinomega;
        k1 = (areal)sin(t * omega) / sinomega;
    } else {
        k0 = 1.0f - t;
        k1 = t;
    }
    x = q0.x * k0 + q.x * k1;
    y = q0.y * k0 + q.y * k1;
    z = q0.z * k0 + q.z * k1;
    w = q0.w * k0 + q.w * k1;
}

AMatrix3D AQuaternion::toMatrix() const {
    AMatrix3D ret;
    areal qxx(x * x);
    areal qyy(y * y);
    areal qzz(z * z);
    areal qxz(x * z);
    areal qxy(x * y);
    areal qyz(y * z);
    areal qwx(w * x);
    areal qwy(w * y);
    areal qwz(w * z);

    ret[0] = 1 - 2 * (qyy +  qzz);
    ret[1] = 2 * (qxy + qwz);
    ret[2] = 2 * (qxz - qwy);

    ret[3] = 2 * (qxy - qwz);
    ret[4] = 1 - 2 * (qxx +  qzz);
    ret[5] = 2 * (qyz + qwx);

    ret[6] = 2 * (qxz + qwy);
    ret[7] = 2 * (qyz - qwx);
    ret[8] = 1 - 2 * (qxx +  qyy);
    return ret;
}

AVector3D AQuaternion::euler() const {
    return toMatrix().euler();
}
