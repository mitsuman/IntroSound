/* optical circuit by 0x4015
   sound track

   How to play: g++ optical_circuit.cpp && ./a.out | aplay -f dat
*/
 
#include <cmath>
#include <stdio.h>
#include "glm/glm/glm.hpp"

using namespace std;
using namespace glm;

vec2 A;
int gl_VertexID;

int z(int a, int b) {
     int t = (a * 3 >> 2) + (1 << 13);
     return ((t ^ -(t >> 14 & 1)) & b) - (b >> 1);
}
int F(int a, int b) {
     int t = a * 3 >> 2;
     return ((t + int(sin(t * 3.11 / (1 << 24) + b * 991) * (1 << 14)) | t + int(sin(t * 5.93 / (1 << 24) + b * 997) * (1 << 14))) & (1 << 15) - 1) - (1 << 14);
}
int C(int t) {
     float a = exp(-.000025 * t);
     return int(clamp(sin(sin(a * a - 1) * a * a * 127), -.5f, .5f) * a * 5e5);
}
int B(int t) {
     return (t * t % 18486 - 18486 / 2) * 2 >> min(t >> 14, 16);
}
int L(int t) {
     return (t * (t ^ (t + (t >> 10) + (t >> 15) | 1)) & 127) - 64 << 7;
}
int H(int a) {
     int t = a & (1 << 18) - 1, v = C(t) - ((z(t * 255 >> 4, (1 << 14) - 1) >> (t * 255 >> 24)) - (z(t << 5, (1 << 14) - 1) >> (t >> 16)) + (z(t * 257 >> 4, (1 << 14) - 1) >> (t * 257 >> 24))) * 8;
     t = a - (5 << 15) & (1 << 18) - 1;
     v -= C(t) - (z(t << 4, (1 << 14) - 1 - t % 555) >> (t >> 14)) * 16;
     t = a - (7 << 15) & (1 << 18) - 1;
     v -= C(t) - (z(t << 4, (1 << 14) - 1 - t % 555) >> (t >> 14)) * 16;
     if ((a >> 21 & 3) > 0) {
          int b = a + (1 << 16) & (1 << 17) - 1;
          for (int i = 0; i < 8; ++i) {
               t = b + i;
               v += (t * t * t % 94519 & z(t * 2, (1 << 14) - 1) + (1 << 13)) - (1 << 12) >> min(t >> 14, 16);
          }
          v += B(a - (3 << 15) & (1 << 18) - 1);
     }
     t = a * 2;
     if ((a >> 21 & 3) > 1) v += B(t & (1 << 19) - 1) + B(t - (1 << 17) & (1 << 19) - 1) + B(t - (5 << 16) & (1 << 19) - 1) + B(t - (7 << 16) & (1 << 19) - 1) * 2;
     return v;
}
int T(int a) {
     int t = a & (1 << 22) - 1, v = 0;
     if ((a >> 21 & 3) > 1) v += L(t * 3 >> 6) * ((1 << 22) - t >> 13) >> 8;
     t = a & (1 << 21) - 1;
     if ((a >> 21 & 7) == 3) v += ((z(t * 48, (1 << 14) - 1 - t % 555) & z(t * 5 * 255 >> 4, (1 << 14) - 1 - t % 555) & z(t * 7 * 257 >> 4, (1 << 14) - 1 - t % 555)) * ((1 << 21) - t >> 12) + (L(t * 3 >> 6) + L(t * 5 >> 6) + L(t * 7 >> 6)) * (t >> 14)) * 3 >> 8;
     t = a & (1 << 23) - 1;
     if ((a >> 21 & 7) > 3) v += (L(t * 3 >> 5) * ((1 << 23) - t >> 14) >> (t >> 14 & 1) + 8) + F(a << 4 & (1 << 24) - 1, 0) * 2;
     if ((a >> 21 & 7) < 6) t = (a & (1 << 19) - 1) + (1 << 19);
     if ((a >> 21 & 7) > 3) {
          int b = t + (1 << 24);
          t = b % (3 << 20);
          for (int i = (b * 3 >> 16) % 3; i < 10; i += 2) v += ((i & 2) - 1) * F(b * ((t >> 14 + i) % 3 + 2 + (t >> 15 + i & 3) / 3) * (i % 3 + 2) << 2 + (t >> 15 + i) % 3, i) * ((a & (1 << 23) - 1) >> 15) >> 7;
     }
     return v;
}

void shader_main() {
     int t = gl_VertexID * 3, a = max(t - (1 << 16), 0), b = max(a - 500, 0);
     
     //A = clamp((vec3(H(t), H(a), H(b)) * mat3(7, 1, 0, 5, 0, -3, 0, 0, 0) + vec3(T(t), T(a), T(b)) * mat3(5, 0, -3, 7, 1, 0, 0, 0, 0)).xy * 3 / (1 << 23), -.95f, .95f);
     //rewrite for glm
     vec3 c = (vec3(H(t), H(a), H(b)) * mat3(7, 1, 0, 5, 0, -3, 0, 0, 0) + vec3(T(t), T(a), T(b)) * mat3(5, 0, -3, 7, 1, 0, 0, 0, 0)) * (3.f / (1 << 23));
     A.x = clamp(c.x, -.95f, .95f);
     A.y = clamp(c.y, -.95f, .95f);
}

int main() {
    for (gl_VertexID = 0; gl_VertexID < 48000 * (132); gl_VertexID++) {
            shader_main();
            short out[2] = {A.x * 32768, A.y * 32768};
            fwrite(&out, sizeof out, 1, stdout);
    }
}
