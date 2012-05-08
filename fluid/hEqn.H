{
    fvScalarMatrix hEqn(
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turbulence.alphaEff(), h)
     ==
        DpDt
      + parcels.Sh()
    );
    hEqn.relax();
    resT = hEqn.solve().initialResidual();
    thermo.correct();
    Info<< "Min/max T:" << min(thermo.T()).value() << ' '
        << max(thermo.T()).value() << endl;
}
