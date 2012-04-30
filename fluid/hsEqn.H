
{
    fvScalarMatrix hEqn(
        fvm::ddt(rho, hs)
      + mvConvection->fvmDiv(phi, hs)
      - fvm::laplacian(turb.alphaEff(), hs)
     ==
        DpDt
     +  parcels.Sh()
    );

    hEqn.relax();

    hsres = hEqn.solve().initialResidual();

    thermo.correct();
}
