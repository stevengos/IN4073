
#define Q 14
#define K (1 << (Q - 1))
typedef  int q14;

q14 normal2q (double x);

q14 q_multiplication (q14 a, q14 b);

double q2normal(q14 a);

q14 q_division (q14 a, q14 b);

q14 q_add(q14 a, q14 b);

q14 q_subtract(q14 a, q14 b);
