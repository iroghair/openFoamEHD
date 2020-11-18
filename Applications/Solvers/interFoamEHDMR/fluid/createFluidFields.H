    // Initialise fluid field pointer lists
    PtrList<volScalarField> rhoFluid(fluidRegions.size());
    PtrList<volScalarField> pFluid(fluidRegions.size());
    PtrList<volScalarField> UeFluid(fluidRegions.size());
    PtrList<volTensorField> MSTFluid(fluidRegions.size());
    PtrList<volScalarField> rhoEFluid(fluidRegions.size());
    PtrList<volVectorField> UFluid(fluidRegions.size());
    PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
    PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
    PtrList<volScalarField> p_rghFluid(fluidRegions.size());
    PtrList<volScalarField> alpha1Fluid(fluidRegions.size());
    PtrList<surfaceScalarField> rhoPhiFluid(fluidRegions.size());
    PtrList<volScalarField> ghFluid(fluidRegions.size());
    PtrList<surfaceScalarField> ghfFluid(fluidRegions.size());
    PtrList<dimensionedScalar> rho1Fluid(fluidRegions.size());
    PtrList<dimensionedScalar> rho2Fluid(fluidRegions.size());
    PtrList<dimensionedScalar> nuFluid(fluidRegions.size());
    // IR: Added twoPhaseMixture
    PtrList<twoPhaseMixture> twoPhasePropertiesFluid(fluidRegions.size());
    PtrList<incompressible::turbulenceModel> turbulenceFluid(fluidRegions.size());
    // IR: Added interfaceProperties
    PtrList<interfaceProperties> interfaceFluid(fluidRegions.size());
    //MM for incompressible we need to add reference pressures
    PtrList<label> pRefCellFluid(fluidRegions.size());
    PtrList<scalar> pRefValueFluid(fluidRegions.size());

    // IR: Adding several more variables for electric properties
    PtrList<dimensionedScalar> eps1Fluid(fluidRegions.size());
    PtrList<dimensionedScalar> eps2Fluid(fluidRegions.size());
    PtrList<dimensionedScalar> sgm1Fluid(fluidRegions.size());
    PtrList<dimensionedScalar> sgm2Fluid(fluidRegions.size());
    PtrList<volScalarField> epsFluid(fluidRegions.size());
    PtrList<volScalarField> sgmFluid(fluidRegions.size());

    // IR:  Create a persistent variable to store the maximum time step
    //      regarding the electrical properties. The entire simulation is
    //      based on the minimum time step of any fluid region.
    scalar deltaTElectricMax = GREAT;

    // Populate fluid field pointer lists
    forAll(fluidRegions, i)
    {
        Info<< "    Adding to p_rghFluid\n" << endl;
        p_rghFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "p_rgh",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );
	
        Info<< "    Adding to alpha1Fluid\n" << endl;
        alpha1Fluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "alpha1",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );

        Info<< "    Adding to UFluid\n" << endl;
        UFluid.set
        (
            i,
            new volVectorField
            (
                IOobject
                (
                    "U",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );

        Info<< "    Adding to phiFluid\n" << endl;
        phiFluid.set
        (
            i,
            new surfaceScalarField
            (
                IOobject
                (
                    "phi",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                linearInterpolate(UFluid[i]) & fluidRegions[i].Sf()
            )
        );
        
        Info << "Reading transportProperties\n" << endl;
        twoPhasePropertiesFluid.set
        (
            i,
            // IR:  I got an error about this pointer, and I suspect it has to do with the fact
            //      that we are making a pointer of a library instead of a variable type. For
            //      the turbulence model the autoPtr keyword was used, so I directly translated
            //      that to the twoPhaseMixture here, and interfaceProperties below.
            // IR:  NOTE: The comment above refers to an autoPtr construction, but
            //      now it seems to work without that... Leaving the comment for 
            //      future reference in case the world blows up: it was an accident and we have nothing to do with it.
            new twoPhaseMixture
            (   
                UFluid[i], 
                phiFluid[i]
            )
        );
      
        // IR:  Even simple dimensionedScalars need to be set with the multiRegion
        //      approach, of course...
        rho1Fluid.set(i,new dimensionedScalar(twoPhasePropertiesFluid[i].rho1()));
        rho2Fluid.set(i,new dimensionedScalar(twoPhasePropertiesFluid[i].rho2()));

        Info<< "    Reading electrical properties\n" << nl << endl;
        IOdictionary elPropDict
        (
            IOobject
            (
                "electricalProperties",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE
            )
        );
        
        // IR:  Read electrical properties from electricalProperties dictionary file
        eps1Fluid.set(i,new dimensionedScalar(elPropDict.subDict("phase1").lookup("eps")));
        eps2Fluid.set(i,new dimensionedScalar(elPropDict.subDict("phase2").lookup("eps")));
        sgm1Fluid.set(i,new dimensionedScalar(elPropDict.subDict("phase1").lookup("sgm")));
        sgm2Fluid.set(i,new dimensionedScalar(elPropDict.subDict("phase2").lookup("sgm")));

        // IR:  Set up the conductivity and permittivity fields
        sgmFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "sgm",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                //scalar(1) / (alpha1/sgm1 + (scalar(1) - alpha1)/sgm2),
                alpha1Fluid[i]*sgm1Fluid[i] + (scalar(1) - alpha1Fluid[i])*sgm2Fluid[i],
                alpha1Fluid[i].boundaryField().types()
            )
        );
        // IR: weg: sgm.oldTime();

        epsFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "eps",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                //alpha1*eps1 + (scalar(1) - alpha1)*eps2,
                scalar(1) / (alpha1Fluid[i]/eps1Fluid[i] + (scalar(1) - alpha1Fluid[i])/eps2Fluid[i]),
                alpha1Fluid[i].boundaryField().types()
            )
        );
        // IR: weg: eps.oldTime();

        deltaTElectricMax = min
        (
            deltaTElectricMax, 
            min
            (
                eps1Fluid[i].value()/(sgm1Fluid[i].value()+SMALL), 
                eps2Fluid[i].value()/(sgm2Fluid[i].value()+SMALL)
            )
        );

        Info<< "    Adding to rhoFluid\n" << endl;
        rhoFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "rho",
                    runTime.timeName(),
                    fluidRegions[i],
                    // IR:  Commented to match with interFoam 
                    //IOobject::NO_READ,
                    //IOobject::AUTO_WRITE
                    // IR:  New line to match with interFoam
                   IOobject::READ_IF_PRESENT
                ),
                alpha1Fluid[i]*rho1Fluid[i] + (scalar(1)-alpha1Fluid[i])*rho2Fluid[i],
                //MM interfoam to add?! 
                // IR: uncommented the line below
                alpha1Fluid[i].boundaryField().types()
	          )
        );
  	    //MM interfoam to add?!
  	    // IR: uncommented the line below 
  	    rhoFluid[i].oldTime();

        Info<< "    Adding to rhoPhiFluid\n" << endl;
        //MM initialisation doesn't matter, rhoPhi is reset after alpha1 solution
      	rhoPhiFluid.set
        (
            i,
            new surfaceScalarField
            (
                IOobject
                (
                    "rho*phi",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                rho1Fluid[i]*phiFluid[i]
            )
        );


        /* * * * * * * * * * * * * * * * * * * * * * * * * 
          IR: The lines below are added (compared to interFoam), but
          have been here in the example multiRegion code that we got. */

        Info<< "    Adding to UeFluid\n" << endl;
        UeFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "Ue",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );         

        Info<< "    Adding to rhoEFluid\n" << endl;
        rhoEFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "rhoE",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );

        Info<< "    Adding to MSTFluid\n" << endl;
        MSTFluid.set
        (
            i,
            new volTensorField
            (
                IOobject
                (
                    "MST",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                epsFluid[i]*(fvc::grad(UeFluid[i])*fvc::grad(UeFluid[i]) 
              - scalar(0.5)*magSqr(fvc::grad(UeFluid[i]))*I)
            )
        );


         
        // IR: We need to add interfaceProperties to each fluidRegion individually;
        //     Therefore I am commenting out the following line, and setting a new PtrList.
        //	interfaceProperties interface(alpha1, U, twoPhaseProperties);

        Info<< "    Adding to interface\n" << endl;
        interfaceFluid.set
        (
            i,
            // IR:  Note that for the interfaceProperties, we need an 'autoPtr' kind 
            //      of implementation as also seen for the turbulence part.
                new interfaceProperties
                (
                    alpha1Fluid[i],
                    UFluid[i],
                    twoPhasePropertiesFluid[i]
                )
        );
	
        Info<< "    Adding to turbulenceModel\n" << endl;
        turbulenceFluid.set
        (
            i,
            autoPtr<incompressible::turbulenceModel>
            (
                incompressible::turbulenceModel::New
                (
                    UFluid[i], 
                    phiFluid[i], 
                    twoPhasePropertiesFluid[i]
                )
            )
        );


        Info<< "    Adding to gFluid\n" << endl;
        gFluid.set
        (
            i,
            new uniformDimensionedVectorField
            (
                IOobject
                (
                    "g",
                    runTime.constant(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                )
            )
        );
        
        //MM this is not needed because we have gFluid: #include "readGravitationalAcceleration.H"

        // The following lines have been copied from interFoam, do we need them?
        // IR:  I guess not, because they're commented out in the 
        /* //Read the data file and initialise the interpolation table
        interpolationTable<vector> timeSeriesAcceleration
        (
            runTime.path()/runTime.caseConstant()/"acceleration.dat"
        );*/


        Info<< "    Adding to ghFluid\n" << endl;
        ghFluid.set
        (   
            i,  
            new volScalarField("gh", gFluid[i] & fluidRegions[i].C())
        );  

        Info<< "    Adding to ghfFluid\n" << endl;
        ghfFluid.set
        (   
            i,  
            new surfaceScalarField("ghf", gFluid[i] & fluidRegions[i].Cf())
        );  

        Info << " Adding to pFluid\n" << endl;
        pFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "p",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                p_rghFluid[i]+rhoFluid[i]*ghFluid[i]
            )
        );

        Info << "Setting reference pressures in refCells\n" << endl;
        pRefCellFluid.set
        (
	        i,
	         new label(0)
	    );
	      
        pRefValueFluid.set
        (
            i,
            new scalar(0.0)
        );
	      
	      // IR:  Checked that we do not need a multiRegion setRefCell here, as it seems
	      //      to give any reference cell value to the fourth argument.
        setRefCell
        (   
            pFluid[i],
            p_rghFluid[i], //MM interfoam added, probably needed
            fluidRegions[i].solutionDict().subDict("PIMPLE"),
            pRefCellFluid[i],
            pRefValueFluid[i]
        );

	    if (p_rghFluid[i].needReference())
	    {
		    pFluid[i] += dimensionedScalar
		    (
			    "p",
			     pFluid[i].dimensions(),
			     pRefValueFluid[i] - getRefCellValue(pFluid[i],pRefCellFluid[i])
		    );
		    p_rghFluid[i]=pFluid[i]-rhoFluid[i]*ghFluid[i]; 
        }
    }
    
    Info<< "Time step limitation due to electrical properties:" << deltaTElectricMax <<" s\n"<<endl;
