/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Global
    continuityErrs

Description
    Calculates and prints the continuity errors.

\*---------------------------------------------------------------------------*/

{
    // IR: Change phi to its corresponding fluidRegion variable
    volScalarField contErr(fvc::div(phiFluid[i]));

    scalar sumLocalContErr = runTime.deltaTValue()*
        // IR weg mag(contErr)().weightedAverage(mesh.V()).value();
        mag(contErr)().weightedAverage(fluidRegions[i].V()).value();

    scalar globalContErr = runTime.deltaTValue()*
        // IR weg contErr.weightedAverage(mesh.V()).value();
        contErr.weightedAverage(fluidRegions[i].V()).value();
    cumulativeContErr[i] += globalContErr;

    Info<< "time step continuity errors : sum local = " << sumLocalContErr
        << ", global = " << globalContErr
        << ", cumulative = " << cumulativeContErr
        << endl;
}

// ************************************************************************* //
