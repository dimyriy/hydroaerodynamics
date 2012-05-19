    const fvMesh& mesh = fluidRegions[i];
    dimensionedScalar rhoMax(mesh.solutionDict().subDict("PISO").lookup("rhoMax"));
    dimensionedScalar rhoMin(mesh.solutionDict().subDict("PISO").lookup("rhoMin"));
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
    const volScalarField& T = thermo.T();
    basicThermoCloud& parcels = parcelsFluid[i];
    const dimensionedScalar initialMass(
        "initialMass",
        dimMass,
        initialMassFluid[i]
    );