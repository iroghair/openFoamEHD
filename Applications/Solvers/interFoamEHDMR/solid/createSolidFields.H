    // Initialise solid field pointer lists
    PtrList<volScalarField> UeSolid(solidRegions.size());
    // IR: Added some electrical variables
    PtrList<volScalarField> epsSolid(solidRegions.size());
    PtrList<volScalarField> sgmSolid(solidRegions.size());
    // IR: I removed many comments from the 'hacked-' version

    // Populate solid field pointer lists
    forAll(solidRegions, i)
    {
    
       Info<< "    Reading electrical properties" << nl << endl;
        IOdictionary elPropDict
        (
            IOobject
            (
                "electricalProperties",
                runTime.constant(),
                solidRegions[i],
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE
            )
        );
        
        // IR:  Read electrical properties from electricalProperties dictionary file
        //      Note that we are constructing a volScalarField of both eps and sgm,
        //      even though we know it is constant throughout the region. This is 
        //      because we want to use the variables in the solver and couple to the
        //      fluid regions, in which these fields _can_ vary with phase fraction.
        //eps1Solid.set(i,new dimensionedScalar(elPropDict.lookup("eps")));
        //sgm1Solid.set(i,new dimensionedScalar(elPropDict.lookup("sgm")));
        
        epsSolid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "eps",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i],
                dimensionedScalar
                (
                    elPropDict.lookup("eps")
                )
            )
        );
        
        sgmSolid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "sgm",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i],
                dimensionedScalar
                (
                    elPropDict.lookup("sgm")
                )
            )
        );

        
        Info<< "    Adding to UeSolid\n" << endl;
        UeSolid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Ue",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i]
            )
        );
    
 /*       Info<< "    Adding to sigmaMags\n" << endl;
        sigmaMags.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "sigmaMag",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i]
            )
        );*/
    }
