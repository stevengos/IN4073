syms a1 a2 a3 a4
syms p r y l
syms kp kr ky kl

S = [p;r;y;l]
K = diag([kp,kr,ky,kl])
M = [1 0 -1 0; 0 -1 0 1; -1 1 -1 1; -1 -1 -1 -1]
A = [a1;a2;a3;a4]

C = K*M;

C_inv = inv(C)

K_inv = inv(K)
M_inv = inv(M)

C_inv