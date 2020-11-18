#!/usr/bin/python
import numpy as np  # NumPy (multidimensional arrays, linear algebra, ...)
import scipy as sp  # SciPy (signal and image processing library)
import matplotlib as mpl         # Matplotlib (2D/3D plotting library)
import matplotlib.pyplot as plt  # Matplotlib's pyplot: MATLAB-like syntax
from pylab import *              # Matplotlib's pylab interface

# Fill out simulation parameters
time       = 30             # Time to display in verification plot
R          = 0.05           # Radius of cylinder
totRhoE    = 2.83334e-05    # Total free charge in the domain (hint: use foamCalcEx or 'integrate variables' filter in paraFoam to obtain this number)
D          = 0.02           # Depth of 2D domain
eps2       = 2.             # Permittivity of dielectric
Q          = totRhoE/D      # Charge per unit length of cylinder

# Run foamCalc magGrad Ue to obtain electric field strength
# Run 'sample'  utility to generate the files loaded here
fname='sets/'+ str(time) + '/lineX1_Ue_alpha1_magGradUe_p.xy'
x, Ue,  alpha1, EE,  p = loadtxt(fname, unpack=True)

i1=nonzero(x>=0)
xpos=x[i1]
EE=EE[i1]

# Theoretical relation
exact = zeros(len(xpos))
for i in range(0,len(xpos)):
    if (xpos[i] < R):
        exact[i] = 0.
    else:
        exact[i] = Q/(2.*pi*eps2)*(1./xpos[i]);

plot(xpos,exact, 'r-')
plot(xpos,EE, 'ro')
xlabel('Position [m]')
ylabel('E [V/m]')
show()
