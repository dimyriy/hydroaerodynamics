
{
    chemistry.solve
    (
        runTime.value() - runTime.deltaT().value(),
        runTime.deltaT().value()
    );

    // turbulent time scale
    turbulentReaction = false;
    if (turbulentReaction)
    {
        DimensionedField<scalar, volMesh> tk =
            Cmix*sqrt(turb.muEff()/rho/turb.epsilon());
        DimensionedField<scalar, volMesh> tc =
            chemistry.tc()().dimensionedInternalField();

        // Chalmers PaSR model
        kappa = (runTime.deltaT() + tc)/(runTime.deltaT() + tc + tk);
    }
    else
    {
        kappa = 1.0;
    }

    chemistrySh = kappa*chemistry.Sh()();
}
