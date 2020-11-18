label nAlphaCorr(readLabel(pimple.dict().lookup("nAlphaCorr")));

label nAlphaSubCycles(readLabel(pimple.dict().lookup("nAlphaSubCycles")));

if (nAlphaSubCycles > 1)
{
    dimensionedScalar totalDeltaT = runTime.deltaT();
    surfaceScalarField rhoPhiSum(0.0*rhoPhi);

    for
    (
        subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
        !(++alphaSubCycle).end();
    )
    {
        #include "alphaEqn.H"
        rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
    }

    rhoPhi = rhoPhiSum;
}
else
{
    #include "alphaEqn.H"
}

interface.correct();

rho == alpha1*rho1 + (scalar(1) - alpha1)*rho2;
eps == alpha1*eps1 + (scalar(1) - alpha1)*eps2;
//eps = scalar(1)/((alpha1/eps1.value())+((scalar(1)-alpha1)/eps2.value()));

forAll( alpha1, celli){
    if (alpha1[celli] < scalar(0.01))
    {
        sgm[celli] = sgm2.value();
    }
    else
    {
          if(alpha1[celli] > scalar(0.99)) 
          {
            sgm[celli] = sgm1.value();
          }
          else
          {
            sgm[celli] = scalar(1)/((alpha1[celli]/(sgm1.value()+SMALL))+((scalar(1)-alpha1[celli])/(sgm2.value()+SMALL)));
            //sgm[celli] = alpha1[celli]*sgm1.value()+(scalar(1)-alpha1[celli])*sgm2.value();
          }
    }
}

