    // IR: The CoNum is defined for all fluidRegions at once; 
    //     we do want our time step to be chosen based on the
    //     highest Courant number that we find in any region.

    scalar maxAlphaCo
    (
        readScalar(runTime.controlDict().lookup("maxAlphaCo"))
    );

    scalar alphaCoNum = -GREAT;

    forAll(fluidRegions, regionI)
    {
        alphaCoNum = max
        (
            alphaLocalRegionCourantNo
            (
                fluidRegions[regionI],
                runTime,
                phiFluid[regionI],
                alpha1Fluid[regionI]
            ),
            alphaCoNum
        );
    }
