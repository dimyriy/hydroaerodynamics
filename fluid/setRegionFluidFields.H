    const fvMesh& mesh = fluidRegions[i];
    psiChemistryModel& chemistry = pChemistryFluid[i];
    hsCombustionThermo& thermo = chemistry.thermo();
    volScalarField& rho = rhoFluid[i];
    volScalarField& K = KFluid[i];
    volVectorField& U = UFluid[i];
    surfaceScalarField& phi = phiFluid[i];
    compressible::turbulenceModel& turb = turbulence[i];
    volScalarField& DpDt = DpDtFluid[i];
    volScalarField& p = thermo.p();
    const volScalarField& T = thermo.T();
    const volScalarField& psi = thermo.psi();
    volScalarField& hs = thermo.hs();
    thermoReactingCloud& parcels = parcelFluid[i];
    Switch turbulentReaction(chemistryProperties[i].lookup("turbulentReaction"));
    dimensionedScalar Cmix("Cmix",dimless,1.0);
    if(turbulentReaction){ chemistryProperties[i].lookup("Cmix") >> Cmix; }
    volScalarField& G = GFluid[i];
    radiation::radiationModel& radiation = radiationFluid[i];
    DimensionedField<scalar,volMesh>& kappa = kappaFluid[i];
    basicMultiComponentMixture& composition = thermo.composition();
    word inertSpecie(thermo.lookup("inertSpecie"));
    if(!composition.contains(inertSpecie)){
        FatalErrorIn(args.executable())
            << "Specified inert specie '" << inertSpecie << "' not found in "
            << "species list. Available species:" << composition.species()
            <<exit(FatalError);
    }
    PtrList<volScalarField>& Y = composition.Y();
    multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
    forAll(Y,speciei){ fields.add(Y[speciei]); }
    fields.add(hs);
    DimensionedField<scalar,volMesh>& chemistrySh = chemistryShFluid[i];
    const dimensionedScalar initialMass(
        "initialMass",
        dimMass,
        initialMassFluid[i]
    );