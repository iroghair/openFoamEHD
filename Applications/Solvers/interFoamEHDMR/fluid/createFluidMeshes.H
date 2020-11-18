    PtrList<fvMesh> fluidRegions(rp.fluidRegionNames().size());
    // IR: added a pimpleControl list
    PtrList<pimpleControl> pimpleFluid(rp.fluidRegionNames().size());

    forAll(rp.fluidRegionNames(), i)
    {
        Info<< "Create fluid mesh for region " << rp.fluidRegionNames()[i]
            << " for time = " << runTime.timeName() << nl << endl;

        fluidRegions.set
        (
            i,
            new fvMesh
            (
                IOobject
                (
                    rp.fluidRegionNames()[i],
                    runTime.timeName(),
                    runTime,
                    IOobject::MUST_READ
                )
            )
        );

    // IR:  We need a PIMPLE control variable for each mesh that we solve
    //      so this line is taken from the original interFoam and adapted
    //      according to the multiRegion format
        
        // pimpleControl pimple(mesh);
        pimpleFluid.set
        (
            i,
            new pimpleControl
            (
                fluidRegions[i]
            )
        );
    }
