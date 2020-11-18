    // IR: The CoNum is defined for all fluidRegions at once; 
    //     we do want our time step to be chosen based on the
    //     highest Courant number that we find in any region.
    scalar CoNum = -GREAT;

    forAll(fluidRegions, regionI)
    {
        CoNum = max
        (
            icoCourantNo
            (
                fluidRegions[regionI],
                runTime,
                phiFluid[regionI]
            ),
            CoNum
        );
    }
