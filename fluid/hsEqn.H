{
    fvScalarMatrix hEqn
    (
        fvm::ddt(rho, hs)
      + mvConvection->fvmDiv(phi, hs)
      - fvm::laplacian(turb.alphaEff(), hs)
     ==
        DpDt
     +  parcels.Sh()
     +  radiation.Shs(thermo)
     +  chemistrySh
    );

    hEqn.relax();

    hsres = hEqn.solve().initialResidual();

    thermo.correct();

    radiation.correct();
}
