# openFoamEHD

Electrohydrodynamic models based on OpenFOAM
============================================

Introduction
------------
In 2012, I've created a few electrohydrodynamic solvers based on OpenFOAM as part of my postdoc project with Prof. Frieder Mugele at the Physics of Complex Fluids group at the University of Twente. While I've shared a zipped version of the source code and validation cases on the cfd-online forums, I am now also sharing the code here on GitHub hoping that it can be of value to other users.

This repository holds solvers, utilities and a few cases (mostly validation cases with scripts that compare to the analytical solution). These solvers were used for our publication Roghair et al. [1].

The solvers, utilities and cases are provided as is, in the hope that it is useful but no warranty of any kind is given. We assume no liability or responsibility for any errors or omissions in these files. The license is GNU General Public License, which is included in the source files.

The solvers are based on OpenFOAM 2.1.1, any translation to newer versions will be appreciated, as well as other improvements or corrections.

Directory structure
-------------------
* Applications
  * Solvers
    * **interFoamEHD:** Multiphase electrohydrodynamic solver without multi-region
    * **interFoamEHDMR:** Multiphase electrohydrodynamic solver with multi-region, coupling the electric potential solution across fluid and solid domains
  * Utilities
    * **multiRegionSetFields:** A setFields application specifically for multi-region simulations. The source was obtained from user karlvirgil on cfd-online forums:  http://www.cfd-online.com/Forums/openfoam/105491-how-use-setfields-multiregionsolver.html
  * Validation: validation cases are described in the paper mentioned above, inspired from the work by Lopez-Herrera et al. [2]
    * **chargeBumpRelaxation:** 
    * **conductingCylinder:**
    * **planarLayer:**

References
----------

[1] Roghair, Musterd, Van den Ende, Kleijn, Kreutzer and Mugele: A numerical technique to simulate display pixels based on electrowetting. Microfluidics and Nanofluidics, 2015, DOI 10.1007/s10404-015-1581-5
[2] López-Herrera, Popinet and Herrada: A charge-conservative approach for simulating electrohydrodynamic two-phase flows using Volume-Of-Fluid. J. Comp. Phys. 230(5), 2011. DOI: 10.1016/j.jcp.2010.11.042
