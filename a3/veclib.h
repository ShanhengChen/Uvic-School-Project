#pragma once

#include <math.h>

class Vec3
{
public:

    Vec3() : vals{ 0,0,0 } {}
    Vec3(double e0, double e1, double e2) : vals{ e0, e1, e2 } {}

    Vec3 operator-() const { return Vec3(-vals[0], -vals[1], -vals[2]); }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return vals[0] * vals[0] + vals[1] * vals[1] + vals[2] * vals[2];
    }

public:
    union
    {
        struct { double x, y, z; };
        double vals[3];
    };
};

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.vals[0] + v.vals[0], u.vals[1] + v.vals[1], u.vals[2] + v.vals[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.vals[0] - v.vals[0], u.vals[1] - v.vals[1], u.vals[2] - v.vals[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.vals[0] * v.vals[0], u.vals[1] * v.vals[1], u.vals[2] * v.vals[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.vals[0], t * v.vals[1], t * v.vals[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/ (Vec3 u, Vec3 v) {
    return Vec3(u.vals[0] / v.vals[0], u.vals[1] / v.vals[1], u.vals[2] / v.vals[2]);
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1 / t) * v;
}

inline Vec3 unit_vector(Vec3 v) {
    return v / v.length();
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.vals[0] * v.vals[0]
        + u.vals[1] * v.vals[1]
        + u.vals[2] * v.vals[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.vals[1] * v.vals[2] - u.vals[2] * v.vals[1],
        u.vals[2] * v.vals[0] - u.vals[0] * v.vals[2],
        u.vals[0] * v.vals[1] - u.vals[1] * v.vals[0]);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 multiply4(const double A[4][4], const Vec3& v, bool isDir = false) {
    double v4[4] = { v.x, v.y, v.z, isDir ? 0.0 : 1.0 };
    double res[4];
    for (int i = 0; i < 4; i++) {
        res[i] = 0;
        for (int j = 0; j < 4; j++) {
            res[i] += A[i][j] * v4[j];
        }
    }
    return { res[0], res[1], res[2] };
}

inline void translate(const double A[4][4], double Atra[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Atra[i][j] = A[j][i];
        }
    }
}
