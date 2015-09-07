# The basic Physics
The summarized important physic concepts for QR needed for control. 


The Start;
 - QR is alligned with earth frame , φ, θ, ψ = 0. and X,Y,Z=0 .
 
 - Start to raise in negatieve z direction to above. X,Y=0, z= height.
 
The main forces (X,Y,Z) and momentum (L,M,N)
 - X=0, Y=0
 - Z = -b(w1^2 + w2^2 + w3^2 + w4^2)
 - L = b(w4^2 - w2^2), 
 - M=b(w1^2 - w3^2), 
 - N = d(w2 ^ 2 + w4 ^ 2 - w1 ^ 2 - w3^2)
 
W(i) = rotor RPM proportional to the duty cycle c(i) controlled by ae(i), result in;
  - X=0, Y=0
 - Z = -b(ae(1)^2 + ae(2)^2 + ae(3)^2 + ae(4)^2)
 - L = b(ae(4)^2 - ae(2)^2), 
 - M=b(ae(1)^2 - ae(3)^2), 
 - N = d(ae(2) ^ 2 + ae(4) ^ 2 - ae(1) ^ 2 - ae(3)^2)
 
b and d are specified for QR.
We need to compute the ae(i)'s needed for the ;
- desired lift Z
- desired roll L
- desired pitch M
- desired yaw N

The order of movement ;
- first rotation along z-axis an angle ψ.
- Second rotation alnog y-axis an angle θ.
- Third rotation along x-axis an angle  φ.

The following varaibles are used by standard dynamical and kinemtic equations;(Find example in the example code!)
- u = speed in x-direction  and p = angulair speed with x =axis
- v = speed in y-direction and q = angulair speed with y = axis
- w = speed in z-direction and r = angulair speed with z = axis




 

 






















