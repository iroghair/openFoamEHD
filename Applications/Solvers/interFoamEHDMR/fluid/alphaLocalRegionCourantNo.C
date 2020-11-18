/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "alphaLocalRegionCourantNo.H"
#include "fvc.H"

Foam::scalar Foam::alphaLocalRegionCourantNo
(
    const fvMesh& mesh,
    const Time& runTime,
    const surfaceScalarField& phi,
    const volScalarField& alpha1
)
{
    scalar alphaCoNum = 0.0;
    scalar meanAlphaCoNum = 0.0;

    if (mesh.nInternalFaces())
    {
        scalarField sumPhi
        (
            pos(alpha1 - 0.01)*pos(0.99 - alpha1)
           *fvc::surfaceSum(mag(phi))().internalField()
        );
    
        alphaCoNum = 0.5*gMax(sumPhi/mesh.V().field())*runTime.deltaTValue();

        meanAlphaCoNum =
            0.5*(gSum(sumPhi)/gSum(mesh.V().field()))*runTime.deltaTValue();
    }


    Info<< "Region: " << mesh.name() << " Alpha Courant Number mean: " << meanAlphaCoNum
        << " max: " << alphaCoNum << endl;

    return alphaCoNum;
}


// ************************************************************************* //
