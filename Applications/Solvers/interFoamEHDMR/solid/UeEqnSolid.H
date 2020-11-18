//Info << "  Solve the electric potential equation " << endl;
{
/*    surfaceScalarField rhoEFlux = -fvc::interpolate(sgm)*mesh.magSf()*fvc::snGrad(Ue);
    fvScalarMatrix rhoEEqn
    (
        fvm::ddt(rhoE)
      + fvm::div(phi, rhoE)
      + fvc::div(rhoEFlux)
    );
    //rhoEEqn.relax();
    rhoEEqn.solve();
*/            
    fvScalarMatrix UeEqn
    (
        fvm::laplacian(eps,Ue)
    );
    UeEqn.relax();
    UeEqn.solve();
}
Info<< "End of UeEqnSolid.H - Ue max/min : " << max(Ue).value() << " " << min(Ue).value() << endl;
