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

AQuaternion::AQuaternion(const AMatrix3D &m) {
    areal t = m.mat[0] + m.mat[4] + m.mat[8];
    if(t > 0) {
        areal s = 0.5f / (areal)sqrt(t);
        w   = 0.25f / s;
        x   = ( m.mat[7] - m.mat[5] ) * s;
        y   = ( m.mat[2] - m.mat[6] ) * s;
        z   = ( m.mat[3] - m.mat[1] ) * s;
    } else {
        if ( m.mat[0] > m.mat[4] && m.mat[0] > m.mat[8] ) {
            areal s = (areal)2.0 * (areal)sqrt( 1.0f + m.mat[0] - m.mat[4] - m.mat[8] );
            w = (m.mat[7] - m.mat[5] ) / s;
            x = 0.25f * s;
            y = (m.mat[1] + m.mat[3] ) / s;
            z = (m.mat[2] + m.mat[6] ) / s;
        } else if (m.mat[4] > m.mat[8]) {
            areal s = (areal)2.0 * (areal)sqrt( 1.0f + m.mat[4] - m.mat[0] - m.mat[8] );
            w = (m.mat[2] - m.mat[6] ) / s;
            x = (m.mat[1] + m.mat[3] ) / s;
            y = 0.25f * s;
            z = (m.mat[5] + m.mat[7] ) / s;
        } else {
            areal s = (areal)2.0 * (areal)sqrt( 1.0f + m.mat[8] - m.mat[0] - m.mat[4] );
            w = (m.mat[3] - m.mat[1] ) / s;
            x = (m.mat[2] + m.mat[6] ) / s;
            y = (m.mat[5] + m.mat[7] ) / s;
            z = 0.25f * s;
        }
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
    areal x2 = x + x;
    areal y2 = y + y;
    areal z2 = z + z;
    areal xx = x * x2;
    areal yy = y * y2;
    areal zz = z * z2;
    areal xy = x * y2;
    areal yz = y * z2;
    areal xz = z * x2;
    areal wx = w * x2;
    areal wy = w * y2;
    areal wz = w * z2;

    ret[0] = 1.0f - (yy + zz);
    ret[3] = xy - wz;
    ret[6] = xz + wy;

    ret[1] = xy + wz;
    ret[4] = 1.0f - (xx + zz);
    ret[7] = yz - wx;

    ret[2] = xz - wy;
    ret[5] = yz + wx;
    ret[8] = 1.0f - (xx + yy);
    return ret;
}
