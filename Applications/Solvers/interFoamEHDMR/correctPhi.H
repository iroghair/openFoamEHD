// IR: Added loop over fluidRegions
forAll(fluidRegions, i)
{
    // IR: Note that in this entire file, I changed variables to the
    //     multiRegion type (e.g. p_rgh --> p_rghFluid[i])
    #include "continuityErrs.H"

    // IR: This variable is local only, no list
    wordList pcorrTypes 
    (
        p_rghFluid[i].boundaryField().size(),
        zeroGradientFvPatchScalarField::typeName
    );

    // IR: Changed 'i' in this local forAll loop to 'p' because 
    //     'i' is used to indicate the current fluid domain.
    forAll (p_rghFluid[i].boundaryField(), p)
    {
        if (p_rghFluid[i].boundaryField()[p].fixesValue())
        {
            pcorrTypes[p] = fixedValueFvPatchScalarField::typeName;
        }
    }
  
    // IR: This variable is local only, no list but we got to make
    //     sure that it is defined on the current fluidRegion only...
    volScalarField pcorr
    (
        IOobject
        (
            "pcorr",
            runTime.timeName(),
            // IR: weg mesh,
            // IR: Added region mesh
            fluidRegions[i],
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        // IR: weg mesh,
        // IR: Added region mesh
        fluidRegions[i],
        dimensionedScalar("pcorr", p_rghFluid[i].dimensions(), 0.0),
        pcorrTypes
    );
    
    // IR: I think this is local only, no list
    // IR: Changed rho to its corresponding fluidRegions type
    // TODO: Do these variables indeed get cleared? Otherwise we keep on
    //        declaring variables and thus leaking memory.
    dimensionedScalar rAUf("(1|A(U))", dimTime/rhoFluid[i].dimensions(), 1.0);

    // IR: Changed variables to fluidRegion type, note that pcorr is 
    //     initialised for each fluidRegion just above
    adjustPhi(phiFluid[i], UFluid[i], pcorr);

    // IR: Changed pimple to list type
    while (pimpleFluid[i].correctNonOrthogonal())
    {
        fvScalarMatrix pcorrEqn
        (
            // IR: Changed phi to its corresponding fluidRegion variable
            fvm::laplacian(rAUf, pcorr) == fvc::div(phiFluid[i])
        );

        // IR: Changed pRefCell and pRefValue to list types
        pcorrEqn.setReference(pRefCellFluid[i], pRefValueFluid[i]);
        pcorrEqn.solve();

        // Changed pimple to list type
        if (pimpleFluid[i].finalNonOrthogonalIter())
        {
            // IR: Changed phi to its corresponding fluidRegion variable
            phiFluid[i] -= pcorrEqn.flux();
        }
    }

    #include "continuityErrs.H"
}
