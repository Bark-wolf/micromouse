#ifndef vec2_
#define vec2_
struct vec2 {
    int x; int y;

    vec2();
    vec2(int x, int y);
    vec2(const vec2& vec);

    vec2 operator+(vec2 v);
    vec2 operator-(vec2 v);
    vec2 operator/(int f);
    vec2 operator*(int f);
    void operator*=(int f);
    void operator-=(vec2 v);
    void operator+=(vec2 v);

    vec2 square();
    int dot(vec2 v);
    int magnitude();
    void reflect(vec2 normal);
    vec2 normal() const;
    vec2 tangent();
    void print() const;
    int flatten(int width) const;
};
#endif