#!/usr/bin/python
import numpy as np  # NumPy (multidimensional arrays, linear algebra, ...)
import scipy as sp  # SciPy (signal and image processing library)
import matplotlib as mpl         # Matplotlib (2D/3D plotting library)
import matplotlib.pyplot as plt  # Matplotlib's pyplot: MATLAB-like syntax
from pylab import *              # Matplotlib's pylab interface

# Fill out simulation parameters
time=2e-08
#eps1=2;
#eps2=1;
#sgm1=2;
#sgm2=1;

# Calculate some base parameters
#R=sgm1/sgm2
#Q=eps1/eps2

# Run foamCalc magGrad Ue to obtain electric field strength
# Run 'sample'  utility to generate the files loaded here
fname='sets/'+ str(time) + '/lineY_Ue_alpha1_magGradUe_p_rgh.xy'
y, Ue,  alpha1, EE,  p = loadtxt(fname, unpack=True)

# Get the indices for the two different phases
i1=nonzero(alpha1>0.5)  # conductive phase
i2=nonzero(alpha1<=0.5) # dielectric phase

# Theoretical relations: conducting-conducting
phi1_ex=zeros(size(y))
phi2_ex=zeros(size(y))
phi1_ex[i1] = 1.
phi2_ex[i2] = 1.-2.*y[i2]
E_ex=zeros(size(y))
deltap_ex = -2.
E_ex[i1]=0.0
E_ex[i2]=2.0

# Combined data for the two phases
phi_ex=hstack([phi1_ex[i1],phi2_ex[i2]])

plot(y,phi_ex, 'r-')
plot(y, Ue, 'ro')
xlabel('y')
ylabel('phi [V]')
show()

plot(y,E_ex, 'b-')
plot(y, EE,  'bo')
xlabel('y')
ylabel('E [V/m]')
show()

#plot(r,E)
#show()
#plot(dist,ux, '-o')
#show()

#print 'hoi2'
#hist(x, 100)



#print x, y, z, alpha1

