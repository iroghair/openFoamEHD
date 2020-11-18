/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2009 OpenCFD Ltd.
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

Original Application
    chtMultiRegionFoam-

Application
    interFoamEHDMR
    
Description:
    A multi-region solver where solid and fluid zones can be combined, where
    the fluid zone is solved using the interFoam method including electro-
    hydrodynamic effects (interFoamEHD). Starting point were the chtMultiRegionFoam
    and (obviously) the interFoam codes.
    
    In the solid phase, only Gauss's Law is solved (yielding electric potential
    distribution), which is coupled via mixed-boundary condition to other 
    regions (solid or fluid). The fluid region solves interFoam equations (VOF
    method for multiphase flow), Gauss's Law, an electric charge transport eq
    and couples the electric force to the interface via the Maxwell stress 
    tensor (see UEqn.H).
        
    Please refer to Roghair, Musterd, Ende, Kleijn, Kreutzer and Mugele, 
    Microfluid Nanofluid (2015) for details and verification cases.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "turbulenceModel.H"
#include "fixedGradientFvPatchFields.H"
#include "regionProperties.H"
//MM added
#include "singlePhaseTransportModel.H"
#include "icoCourantNo.H"
// IR: Added header
#include "alphaLocalRegionCourantNo.H"
//MM interFoam added
#include "MULES.H"
#include "subCycle.H"
#include "interfaceProperties.H"
#include "twoPhaseMixture.H"
#include "interpolationTable.H"
#include "pimpleControl.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"

    regionProperties rp(runTime);

    #include "createFluidMeshes.H"
    #include "createSolidMeshes.H"

    #include "createFluidFields.H"
    #include "createSolidFields.H"

    // IR: Removed single-line include #include "initContinuityErrs.H"
    List<scalar> cumulativeContErr(fluidRegions.size(), 0.0);

    #include "readTimeControls.H"

    //MM interFoam added
    #include "correctPhi.H"

    #include "icoMultiRegionCourantNo.H"
    #include "setInitialMultiRegionDeltaT.H"

    Info << "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        // IR: Do we need the following line? This is not in interFoam!
        #include "readPIMPLEControls.H"
        #include "icoMultiRegionCourantNo.H"
        //MM interfoam added
        #include "alphaMultiRegionCourantNo.H"
        #include "setMultiRegionDeltaT.H"

        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;

//        if (nOuterCorr != 1)
//        {
            forAll(fluidRegions, i)
            {
                // TODO:    Some variables here are not used in this particular
                //          loop; this generates warnings. Also, we need to check
                //          which 'oldFluidFields' to store...
                #include "setRegionFluidFieldsAlpha.H"
                //#include "storeOldFluidFields.H"

                Info << "Voor twoPhaseProps"<<endl;
                //MM interfoam added IR: changed to list type
                twoPhaseProperties.correct();
                Info << "Na twoPhaseProps"<<endl;
                //MM interfoam added
                #include "alphaEqnSubCycle.H"
            }
//        }

        // --- PIMPLE loop
        // First solve the electric equations so the E-field is 
        // consistent in all regions.
        Info<<" Solving UeEqns for all regions"<<endl;
        for (int oCorr=0; oCorr<4; oCorr++)
        {
            forAll(fluidRegions, i)
            {   
                // Set variables fluidRegions
                #include "setRegionFluidFieldsElectric.H"
                #include "UeEqnFluid.H"
            }
            forAll(solidRegions, i)
            {
                #include "setRegionSolidFieldsElectric.H"
                #include "UeEqnSolid.H"
            }
        }
        
        Info<<" Solving flow for fluidRegions"<<endl;
        for (int oCorr=0; oCorr<nOuterCorr; oCorr++)
        {
            forAll(fluidRegions, i)
            {
                Info<< "\nSolving for fluid region "
                    << fluidRegions[i].name() << endl;
                #include "setRegionFluidFields.H"
                // IR:  I guess we do not need the following line since
                //      the pimple controls have already been read from
                //      the pimpleControl variables, created for each
                //      fluidRegion separately.
                //      #include "readFluidMultiRegionPIMPLEControls.H"
                #include "solveFluid.H"
            }
        }

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info << "End\n" << endl;

    return 0;
}


// ************************************************************************* //
