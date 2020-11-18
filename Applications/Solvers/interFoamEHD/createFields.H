    Info<< "Reading field p_rgh\n" << endl;
    volScalarField p_rgh
    (
        IOobject
        (
            "p_rgh",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field alpha1\n" << endl;
    volScalarField alpha1
    (
        IOobject
        (
            "alpha1",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    Info<< "Reading field U\n" << endl;
    volVectorField U
    (
        IOobject
        (
            "U",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    #include "createPhi.H"

    Info<< "Reading transportProperties\n" << endl;
    twoPhaseMixture twoPhaseProperties(U, phi);

    const dimensionedScalar& rho1 = twoPhaseProperties.rho1();
    const dimensionedScalar& rho2 = twoPhaseProperties.rho2();

    Info<< "    Reading electrical properties\n" << nl << endl;
    IOdictionary elPropDict
    (
        IOobject
        (
            "electricalProperties",
            runTime.constant(),
            mesh,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE
        )
    );
    
    // IR:  Read electrical properties from electricalProperties dictionary file
    dimensionedScalar eps1 = elPropDict.subDict("phase1").lookup("eps");
    dimensionedScalar eps2 = elPropDict.subDict("phase2").lookup("eps");
    dimensionedScalar sgm1 = elPropDict.subDict("phase1").lookup("sgm");
    dimensionedScalar sgm2 = elPropDict.subDict("phase2").lookup("sgm");

    
    Info<<"Found: eps1=" << eps1 << ",eps2=" << eps2 << ",sgm1=" << sgm1 << ",sgm2=" << sgm2 << endl;

    // Need to store rho for ddt(rho, U)
    volScalarField rho
    (
        IOobject
        (
            "rho",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT
        ),
        alpha1*rho1 + (scalar(1) - alpha1)*rho2,
        alpha1.boundaryField().types()
    );
    rho.oldTime();

    volScalarField sgm
    (
        IOobject
        (
            "sgm",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        scalar(1) / (alpha1/sgm1 + (scalar(1) - alpha1)/sgm2),
        alpha1.boundaryField().types()
    );
    sgm.oldTime();

    volScalarField eps
    (
        IOobject
        (
            "eps",
            runTime.timeName(),
            mesh,
            IOobject::READ_IF_PRESENT,
            IOobject::AUTO_WRITE
        ),
        scalar(1) / (alpha1/eps1 + (scalar(1) - alpha1)/eps2),
        alpha1.boundaryField().types()
    );
    eps.oldTime();

    // Mass flux
    // Initialisation does not matter because rhoPhi is reset after the
    // alpha1 solution before it is used in the U equation.
    surfaceScalarField rhoPhi
    (
        IOobject
        (
            "rho*phi",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rho1*phi
    );

    Info<< "Reading field Electric Potential Ue\n" << endl;
    volScalarField Ue
    (
        IOobject
        (
            "Ue",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );


    Info<< "Reading field electric density rhoE\n" << endl;
    volScalarField rhoE
    (
        IOobject
        (
            "rhoE",
            runTime.timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    volTensorField MST
    (
        IOobject
        (
            "MST",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        eps*(fvc::grad(Ue)*fvc::grad(Ue) - scalar(0.5)*magSqr(fvc::grad(Ue))*I)
    );

/*    volScalarField tmp1
    (
        IOobject
        (
            "tmp1",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        //fvc::div(MST)
        fvc::div(fvc::snGrad(Ue)*mesh.magSf(), sgm)
    );

    surfaceScalarField tmp2
    (
        IOobject
        (
            "tmp2",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        //rhoE*fvc::grad(Ue)
        -fvc::interpolate(sgm)*mesh.magSf()*fvc::snGrad(Ue)
    );

    volVectorField FElectric2
    (
        IOobject
        (
            "FElectric2",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        -scalar(0.5)*magSqr(fvc::grad(Ue))*fvc::grad(eps)
    );*/
    // Construct interface from alpha1 distribution
    interfaceProperties interface(alpha1, U, twoPhaseProperties);

    // Construct incompressible turbulence model
    autoPtr<incompressible::turbulenceModel> turbulence
    (
        incompressible::turbulenceModel::New(U, phi, twoPhaseProperties)
    );

    #include "readGravitationalAcceleration.H"

    /*
    dimensionedVector g0(g);

    // Read the data file and initialise the interpolation table
    interpolationTable<vector> timeSeriesAcceleration
    (
        runTime.path()/runTime.caseConstant()/"acceleration.dat"
    );
    */

/*
    Info<< "Calculating field rhoEFlux\n" << endl;
    surfaceScalarField rhoEFlux
    (
        IOobject
        (
            "rhoEFlux",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
      -fvc::interpolate(sgm)*mesh.magSf()*fvc::snGrad(Ue)
    );
*/
    Info<< "Calculating field g.h\n" << endl;
    volScalarField gh("gh", g & mesh.C());
    surfaceScalarField ghf("ghf", g & mesh.Cf());

    volScalarField p
    (
        IOobject
        (
            "p",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        p_rgh + rho*gh
    );


    label pRefCell = 0;
    scalar pRefValue = 0.0;
    setRefCell
    (
        p,
        p_rgh,
        mesh.solutionDict().subDict("PIMPLE"),
        pRefCell,
        pRefValue
    );


    // IR 20120229: I'm not sure what to do with these lines
    // since they are not here in OF2.1.0: commenting them out now,
    // perhaps the interfaceProperties is accounted for elsewhere...
    // Construct interface from gamma distribution
    //interfaceProperties interface(gamma, U, EHDtwoPhaseProperties);

    if (p_rgh.needReference())
    {
        p += dimensionedScalar
        (
            "p",
            p.dimensions(),
            pRefValue - getRefCellValue(p, pRefCell)
        );
        p_rgh = p - rho*gh;
    }
