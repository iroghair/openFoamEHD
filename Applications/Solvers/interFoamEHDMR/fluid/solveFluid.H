// IR: Changed a few things to make this work with pimple.
// --- Pressure-velocity PIMPLE corrector loop
while (pimple.loop())
{
    // Info<<" Solving UEqn"<<endl;
    #include "UEqn.H"
    
    // --- Pressure corrector loop
    while (pimple.correct())
    {
        // Info<<" Solving pEqn"<<endl;
        #include "pEqn.H"
    }
    
    if (pimple.turbCorr())
    {
        turbulence.correct();
    }
}
