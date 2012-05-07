    tmp<fvVectorMatrix> UEqn
    (
        fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turbulence.divDevRhoReff(U)
    );

    UEqn().relax();

    if (momentumPredictor)
    {
        solve(UEqn() == -fvc::grad(p));
    }
