#!/usr/bin/python
import numpy as np  # NumPy (multidimensional arrays, linear algebra, ...)
import scipy as sp  # SciPy (signal and image processing library)
import matplotlib as mpl         # Matplotlib (2D/3D plotting library)
import matplotlib.pyplot as plt  # Matplotlib's pyplot: MATLAB-like syntax
from pylab import *              # Matplotlib's pylab interface

# Fill out simulation parameters
time=10
L=1;
eps1=2.0;
eps2=1.0;
sgm1=1.0;
sgm2=4.0;

# Calculate some base parameters
R=sgm1/sgm2
Q=eps1/eps2

# Run foamCalc magGrad Ue to obtain electric field strength
# Run 'sample'  utility to generate the files loaded here
fname='sets/'+ str(time) + '/lineY_Ue_alpha1_magGradUe_p_rgh.xy'
y, Ue,  alpha1, EE,  p = loadtxt(fname, unpack=True)

# Theoretical relations: conducting-conducting
phi1_ex = (-2.*y+R)/(1.+R)
phi2_ex = R*(-2.*y+1.)/(1.+R)
E_ex=zeros(size(y))
deltap_ex = -(2.*(R**2.-Q)/(1.+R)**2.)

# Get the indices for the two different phases
i1=nonzero(alpha1>=0.5)
i2=nonzero(alpha1<0.5)

# Combined data for the two phases
phi_ex=hstack([phi1_ex[i1],phi2_ex[i2]])
E_ex[i1]=2./(1.+R)
E_ex[i2]=2.*R/(1.+R)

plot(y,phi_ex, 'r-')
plot(y, Ue, 'ro')
xlabel('y')
ylabel('phi [V]')
show()

#plot(y,E_ex, 'b-')
#plot(y, EE,  'bo')
#xlabel('y')
#ylabel('E [V/m]')
#show()

