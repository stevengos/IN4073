#ifndef FIXH
#define FIXH
#define Q 14
#define K (1 << (Q - 1))
typedef  short q14;

q14 normal2q (int x);

q14 q_multiplication (q14 a, q14 b);

int q2normal(q14 a);

q14 q_division (q14 a, q14 b);

q14 q_add(q14 a, q14 b);

q14 q_subtract(q14 a, q14 b);
#endif
