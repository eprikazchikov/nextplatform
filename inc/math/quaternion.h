/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef QUATERNION_H_HEADER_INCLUDED
#define QUATERNION_H_HEADER_INCLUDED

struct AQuaternion {
	
	AQuaternion() : x(0), y(0), z(0), w(1) { }
	AQuaternion(const AVector3D &dir, float angle) {
        set(dir, angle);
	}

	AQuaternion(float x, float y, float z, float angle) {
        set(x, y, z, angle);
	}

	AQuaternion(AMatrix3D &m) {
		float t		= m.mat[0] + m.mat[4] + m.mat[8];
		if(t > 0) {
            float s	= 0.5f / (float)sqrt(t);
            w	= 0.25f / s;
			x	= ( m.mat[7] - m.mat[5] ) * s;
			y	= ( m.mat[2] - m.mat[6] ) * s;
			z	= ( m.mat[3] - m.mat[1] ) * s;
		} else {
			if ( m.mat[0] > m.mat[4] && m.mat[0] > m.mat[8] ) {
              float s	= (float)2.0 * (float)sqrt( 1.0f + m.mat[0] - m.mat[4] - m.mat[8] );
			  w = (m.mat[7] - m.mat[5] ) / s;
              x = 0.25f * s;
			  y = (m.mat[1] + m.mat[3] ) / s;
			  z = (m.mat[2] + m.mat[6] ) / s;
			} else if (m.mat[4] > m.mat[8]) {
              float s	= (float)2.0 * (float)sqrt( 1.0f + m.mat[4] - m.mat[0] - m.mat[8] );
			  w = (m.mat[2] - m.mat[6] ) / s;
			  x = (m.mat[1] + m.mat[3] ) / s;
              y = 0.25f * s;
			  z = (m.mat[5] + m.mat[7] ) / s;
			} else {
              float s	= (float)2.0 * (float)sqrt( 1.0f + m.mat[8] - m.mat[0] - m.mat[4] );
			  w = (m.mat[3] - m.mat[1] ) / s;
			  x = (m.mat[2] + m.mat[6] ) / s;
			  y = (m.mat[5] + m.mat[7] ) / s;
              z = 0.25f * s;
			}
		}
	}

    /// Comparison operators
    bool operator==(const AQuaternion &v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
    bool operator!=(const AQuaternion &v) const { return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w); }
	
    operator float*()                   { return (float*)&x; }
    operator const float*() const       { return (float*)&x; }
	
    float &operator[](int i)            { return q[i]; }
    const float operator[](int i) const { return q[i]; }
	
	AQuaternion operator*(const AQuaternion &q) const {
		AQuaternion ret;
		ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
		ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
		ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
		ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return ret;
	}
	
    void set(const AVector3D &dir, float angle) {
		float length = dir.length();
		if(length != 0.0) {
			length = 1.0f / length;
			float sinangle = (float)sin(angle * DEG2RAD / 2.0f);
			x = dir[0] * length * sinangle;
			y = dir[1] * length * sinangle;
			z = dir[2] * length * sinangle;
			w = (float)cos(angle * DEG2RAD / 2.0f);
		} else {
			x = y = z = 0.0;
			w = 1.0;
		}
	}
    void set(float x, float y, float z, float angle) {
        set(AVector3D(x, y, z), angle);
	}
	
    void slerp(const AQuaternion &q0, const AQuaternion &q1, float t) {
		float k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
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
			float omega = (float)acos(cosomega);
			float sinomega = (float)sin(omega);
			k0 = (float)sin((1.0f - t) * omega) / sinomega;
			k1 = (float)sin(t * omega) / sinomega;
		} else {
			k0 = 1.0f - t;
			k1 = t;
		}
		x = q0.x * k0 + q.x * k1;
		y = q0.y * k0 + q.y * k1;
		z = q0.z * k0 + q.z * k1;
		w = q0.w * k0 + q.w * k1;
	}
	
    AMatrix3D toMatrix() const {
		AMatrix3D ret;
		float x2 = x + x;
		float y2 = y + y;
		float z2 = z + z;
		float xx = x * x2;
		float yy = y * y2;
		float zz = z * z2;
		float xy = x * y2;
		float yz = y * z2;
		float xz = z * x2;
		float wx = w * x2;
		float wy = w * y2;
		float wz = w * z2;

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
	
	union {
		struct {
            float x, y, z, w;
		};
		float q[4];
	};
};

#endif /* QUATERNION_H_HEADER_INCLUDED */
