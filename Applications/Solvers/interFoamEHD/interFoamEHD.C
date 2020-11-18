/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Application
    interFoamEHD

Description
    interFoamEHD is an electrohydrodynamics solver, solving the electric 
    field and its  interaction with a  fluid--fluid interface.  We  have 
    extensively used the work of López-Herrera, Popinet & Herrada (Jour-
    nal of Computational Physics,  2011) for the further implementation.
    The starting point was EHDFoam, taken from the openfoamwiki.net, but
    that was developed for  OpenFOAM  1.5,  this version is working with 
    2.1.x.  The original version was also  using an external library for
    the  electrical  properties, but  this  feature  was  removed  and a
    separate electricalProperties dictionary is used.
    
    The model solves the fluid  flow equations with an additional forcing
    term to account for electrical stresses acting on the interface.  The
    electric field is described using Gauss's Law,  relating the electric 
    potential to the space charge.   The space charges are solved via the
    conservation equation of the bulk free charge.
    
(original author of EHDFoam):
wayne
email:waynezw0618@163.com

Development resumed by:
Ivo Roghair, Physics of Complex Fluids Group, 
  University of Twente, Enschede, The Netherlands

Currently at Eindhoven University of Technology, i.roghair@tue.nl


\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "MULES.H"
#include "subCycle.H"
#include "interfaceProperties.H"
#include "twoPhaseMixture.H"
#include "turbulenceModel.H"
#include "interpolationTable.H"
#include "pimpleControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

	 pimpleControl pimple(mesh);

    #include "initContinuityErrs.H"
    #include "createFields.H"
    #include "readTimeControls.H"
    #include "correctPhi.H"
    #include "CourantNo.H"
    #include "setInitialDeltaT.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "alphaCourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        twoPhaseProperties.correct();

        #include "alphaEqnSubCycle.H"
        #include "ElectricEqn.H"
         //Info<< "ElectricEqn over " << nl << endl;
        // --- Pressure-velocity PIMPLE corrector loop
        while (pimple.loop())
        {
            #include "UEqn.H"

            // --- Pressure corrector loop
            while (pimple.correct())
            {
                #include "pEqn.H"
            }

            if (pimple.turbCorr())
            {
                turbulence->correct();
            }
        }

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
