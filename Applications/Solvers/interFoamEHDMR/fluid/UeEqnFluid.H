//Info << "  Solve the electric potential equation " << endl;
{
    surfaceScalarField rhoEFlux = -fvc::interpolate(sgm)*mesh.magSf()*fvc::snGrad(Ue);
    fvScalarMatrix rhoEEqn
    (
        fvm::ddt(rhoE)
      + fvm::div(phi, rhoE)
      + fvc::div(rhoEFlux)
    );
    rhoEEqn.solve();

    fvScalarMatrix UeEqn
    (
        fvm::laplacian(eps,Ue) == -rhoE
    );
    UeEqn.relax();
    UeEqn.solve();
}

