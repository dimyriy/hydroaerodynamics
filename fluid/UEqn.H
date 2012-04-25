    fvVectorMatrix UEqn
    (
      fvm::ddt(rho, U)
      + fvm::div(phi, U)
      + turb.divDevRhoReff(U)
     ==
        rho.dimensionedInternalField()*gFluid[i]
      + parcels.SU()
    );

    if (momentumPredictor)
    {
        uxres=solve(UEqn == -fvc::grad(p)).initialResidual();
    }

