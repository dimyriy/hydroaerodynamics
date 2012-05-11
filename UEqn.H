    fvVectorMatrix UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence->divDevRhoReff(U)
      - parcels.SU()
    );

    if (momentumPredictor)
    {
        resU = solve(UEqn == -fvc::grad(p)).initialResidual();
    }
