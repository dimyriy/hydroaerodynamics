{
    resT = solve
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turbulence->alphaEff(), h)
     ==
        DpDt
      + parcels.Sh()
      + radiation->Sh(thermo)
    ).initialResidual();

    thermo.correct();
    radiation->correct();
}
