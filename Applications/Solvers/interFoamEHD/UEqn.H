    surfaceScalarField muEff
    (
        "muEff",
        twoPhaseProperties.muf()
      + fvc::interpolate(rho*turbulence->nut())
    );

    MST = eps*(fvc::grad(Ue)*fvc::grad(Ue) - scalar(0.5)*magSqr(fvc::grad(Ue))*I);
    volVectorField temptemp = fvc::div(MST);

    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(rhoPhi, U)
      - fvm::laplacian(muEff, U)
      - (fvc::grad(U) & fvc::grad(muEff))
      - temptemp 
    );

    UEqn.relax();

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                    fvc::interpolate(interface.sigmaK())*fvc::snGrad(alpha1)
                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
//                  + (fvc::interpolate(temptemp) & mesh.Sf() / mesh.magSf())
//                  + fvc::interpolate(rhoE)*fvc::snGrad(Ue)
//                  - scalar(0.5)* fvc::interpolate(EE)*fvc::snGrad(eps)
                ) * mesh.magSf()
            )
//            - temptemp
        );
    }
