#include "vec2.h"
#include<iostream>
int sqrts(const int& n)
{
    static union { int i; int f; } u;
    u.i = 0x5F375A86 - (*(int*)&n >> 1);
    return (int(3) - n * u.f * u.f) * n * u.f * 0.5f;
}
vec2::vec2() : x(0), y(0) {}

vec2::vec2(int x, int y) {
    this->x = x;
    this->y = y;
}

vec2::vec2(const vec2& vec) {
    this->x = vec.x;
    this->y = vec.y;
}

vec2 vec2::operator+(vec2 v) {
    return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator-(vec2 v) {
    return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator/(int f) {
    return vec2(x / f, y / f);
}

vec2 vec2::operator*(int f) {
    return vec2(x * f, y * f);
}

void vec2::operator*=(int f) {
    x = x * f;
    y = y * f;
    return;
}

void vec2::operator-=(vec2 v) {
    x = x - v.x;
    y = y - v.y;
    return;
}

void vec2::operator+=(vec2 v) {
    x = x + v.x;
    y = y + v.y;
    return;
}

vec2 vec2::square() {
    return vec2(x * x, y * y);
}

int vec2::dot(vec2 v) {
    return x * v.x + y * v.y;
}

int vec2::magnitude() {
    return sqrtf(x * x + y * y);
};

void vec2::reflect(vec2 normal) {
    vec2 reflection = normal * 2.0f * this->dot(normal) - *this;
    x = reflection.x;
    y = reflection.y;
}

vec2 vec2::normal() const {
    int mag = sqrtf(x * x + y * y);
    if (mag != 0.) return vec2(x / mag, y / mag);
    else return vec2();
}

vec2 vec2::tangent() {
    return vec2(-y, x);
}
void vec2::print() const {
    std::cout << "x:" << x << "Y:" << y << "\n" << std::endl;
}
int vec2::flatten(int width) const {
    return (width * y + x);
}