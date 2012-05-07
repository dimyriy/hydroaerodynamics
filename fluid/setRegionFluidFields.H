    const fvMesh& mesh = fluidRegions[i];
    basicPsiThermo& thermo = thermoFluid[i];
    volScalarField& rho = rhoFluid[i];
    volScalarField& K = KFluid[i];
    volVectorField& U = UFluid[i];
    surfaceScalarField& phi = phiFluid[i];
    compressible::turbulenceModel& turbulence = turbulenceFluid[i];
    volScalarField& DpDt = DpDtFluid[i];
    volScalarField& p = thermo.p();
    const volScalarField& psi = thermo.psi();
    volScalarField& h = thermo.h();
    basicThermoCloud& parcels = parcelsFluid[i];
    const dimensionedScalar initialMass(
        "initialMass",
        dimMass,
        initialMassFluid[i]
    );