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
eps == scalar(1) / (alpha1/eps1 + (scalar(1) - alpha1)/eps2);

// To prevent small (e.g. ~ 1e-15) alpha1 fractions from changing the 
// conductivity we need a lower-level averaging procedure that prevents
// such small alpha1 values to participate in the averaging procedure.
// We still need harmonic averaging to get the right conductivity near 
// the interface. Note that if using dielectric-conductive liquids, the
// liquid containing > 0.1% of dielectric will still be very low. This
// has the advantage that the charges are restricted near the interface.

// Determine if we are using dielectric and conductive liquids
if(
    max(sgm1.value(),sgm2.value()) / min(sgm1.value(),sgm2.value()) > 1E10
  )
  {
    forAll( alpha1, celli)
    {
      sgm[celli] = (alpha1[celli] < 0.01) ? 
          sgm2.value() : 
          ((alpha1[celli] > 0.99) ? 
              sgm1.value() : 
              (scalar(1)/((alpha1[celli]/sgm1.value())+((scalar(1)-alpha1[celli])/sgm2.value()))));
    }
  }
  else
  {
    Info << "Two conductive liquids" << endl;
    sgm == scalar(1) / (alpha1/sgm1 + (scalar(1) - alpha1)/sgm2);
  }
    
