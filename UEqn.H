    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divDevRhoReff(U)
      - parcels.SU()
    );

    if (momentumPredictor)
    {
        solve(UEqn == -fvc::grad(p));
    }
