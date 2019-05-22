
{
    fvScalarMatrix hEqn(
        fvm::ddt(rho, hs)
      + mvConvection->fvmDiv(phi, hs)
      - fvm::laplacian(turb.alphaEff(), hs)
     ==
        DpDt
     //+  radiation.Shs(thermo)
     +  parcels.Sh()
    );

    hEqn.relax();

    hsres = hEqn.solve().initialResidual();

    thermo.correct();
    radiation.correct();
    Info<< "T gas min/max   = " << min(T).value() << ", "<< max(T).value() << endl;
}
