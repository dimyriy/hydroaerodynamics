{
    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, h)
      + fvm::div(phi, h)
      - fvm::laplacian(turb.alphaEff(), h)
     ==
        DpDt
      + parcels.Sh()
      + radiation.Sh(thermo)
    );
    if (oCorr == nOuterCorr-1)
    {
        hEqn.relax();
        hEqn.solve(mesh.solver("hFinal"));
    }
    else
    {
        hEqn.relax();
        hEqn.solve();
    }

    thermo.correct();
    radiation.correct();

    Info<< "Min/max T:" << min(thermo.T()).value() << ' '
        << max(thermo.T()).value() << endl;
}
