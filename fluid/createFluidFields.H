PtrList<IOdictionary> chemistryProperties(fluidRegions.size());
PtrList<psiChemistryModel> pChemistryFluid(fluidRegions.size());
PtrList<DimensionedField<scalar, volMesh> > kappaFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());
PtrList<volScalarField> KFluid(fluidRegions.size());
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<volScalarField> GFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<DimensionedField<scalar, volMesh> > chemistryShFluid(fluidRegions.size());
PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
PtrList<compressible::turbulenceModel> turbulence(fluidRegions.size());
PtrList<thermoReactingCloud> parcelFluid(fluidRegions.size());
PtrList<radiation::radiationModel> radiationFluid(fluidRegions.size());
List<scalar> initialMassFluid(fluidRegions.size());
forAll(fluidRegions, i){
    Info<< "*** Reading fluid mesh thermophysical properties for region "
        << fluidRegions[i].name() << nl << endl;
    pChemistryFluid.set(
        i,
        autoPtr<psiChemistryModel>(
            psiChemistryModel::New(
                fluidRegions[i]
            )
        ).ptr()
    );
    chemistryProperties.set(
        i,
        new IOdictionary(
            IOobject(
                "chemistryProperties",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            )
        )
    );
    rhoFluid.set(
        i,
        new volScalarField(
            IOobject(
                "rho",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            pChemistryFluid[i].thermo().rho()
        )
    );
    KFluid.set(
        i,
        new volScalarField(
            IOobject(
                "K",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            pChemistryFluid[i].thermo().Cp()*pChemistryFluid[i].thermo().alpha()
        )
    );
    UFluid.set(
        i,
        new volVectorField(
            IOobject(
                "U",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    phiFluid.set(
        i,
        new surfaceScalarField(
            IOobject(
                "phi",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            linearInterpolate(rhoFluid[i]*UFluid[i])
                & fluidRegions[i].Sf()
        )
    );
    gFluid.set(
        i,
        new uniformDimensionedVectorField(
            IOobject(
                "g",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        )
    );
    turbulence.set(
        i,
        autoPtr<compressible::turbulenceModel>(
            compressible::turbulenceModel::New(
                rhoFluid[i],
                UFluid[i],
                phiFluid[i],
                pChemistryFluid[i].thermo()
            )
        ).ptr()
    );
    radiationFluid.set(
        i,
        autoPtr<radiation::radiationModel>(
            radiation::radiationModel::New(
                pChemistryFluid[i].thermo().T()
            )
        ).ptr()
    );
    GFluid.set(
        i,
        new volScalarField(
            IOobject(
                "G",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    parcelFluid.set(
        i,
        new thermoReactingCloud(
            "reactingCloud1",
            rhoFluid[i],
            UFluid[i],
            gFluid[i],
            pChemistryFluid[i].thermo()
        )
    );
    kappaFluid.set(
        i,
        new DimensionedField<scalar, volMesh>(
            IOobject(
                "kappa",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i],
            dimensionedScalar("zero",dimless,0.0)
        )
    );
    chemistryShFluid.set(
        i,
        new DimensionedField<scalar, volMesh>(
            IOobject(
                "chemistry::Sh",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[i],
            dimensionedScalar("chemistrySh",dimEnergy/dimTime/dimVolume, 0.0)
        )
    );
    initialMassFluid[i] = fvc::domainIntegrate(rhoFluid[i]).value();
}