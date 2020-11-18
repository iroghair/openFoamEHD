#!/usr/bin/python

import numpy as np  # NumPy (multidimensional arrays, linear algebra, ...)
import scipy as sp  # SciPy (signal and image processing library)
import matplotlib as mpl         # Matplotlib (2D/3D plotting library)
import matplotlib.pyplot as plt  # Matplotlib's pyplot: MATLAB-like syntax
from pylab import *              # Matplotlib's pylab interface
import os
import ast

# Fill out simulation parameters
a = 0.05                  # Initial bump size
eps = 2                   # Permittivity
K = 1                     # Conductivity
timesToLoad=[0, 2, 4, 6]  # Times to load and verify
# Run foamCalc magGrad Ue to obtain electric field strength
# Run 'sample'  utility to generate the files loaded here

dirtimes=[0]
  
fig=plt.figure()
fig.add_subplot(2,1,1)  

for t in range(0,len(timesToLoad)):
  fname='sets/'+ str(timesToLoad[t]) + '/rhoE_lineX_rhoE.xy'
  x,rhoE = loadtxt(fname, unpack=True)

  # Theoretical relation (needs x so we got this into the loop)
  initBump = exp(-x**2/(2.*a*a))/(a*sqrt(2*pi));

  plot(x, rhoE, 'bo')
  plot(x,initBump*exp(-K*timesToLoad[t]/eps), 'b-')

title('Charge bump relaxation');
xlabel('Position x/y');
ylabel('Rho_E');

fig.add_subplot(2,1,2)  

dirs=os.listdir("sets/")
for n in range(0,len(dirs)):
  dirtimes.append(ast.literal_eval(dirs[n]))
  
dirSorted=sorted(dirtimes)

rhoEMaxTot=zeros(len(dirSorted))
time=zeros(len(dirSorted))

for d in range(0,len(dirtimes)):
  fname='sets/' + str(dirSorted[d]) + '/somePoints_rhoE.xy'    
  x,y,z,rhoEMax = loadtxt(fname, unpack=True)
  rhoEMaxTot[d]=rhoEMax
  time[d]=dirSorted[d]

maxCharge=exp(-0/(2*a*a))/(a*sqrt(2*pi))
charge = maxCharge*exp(-K*time/eps)

plot(dirSorted,rhoEMaxTot, 'bo')
plot(dirSorted,charge, 'b-')
title('Transient relaxation of charge maximum');
xlabel('Time');
ylabel('Rho_{E,max}');

show()



