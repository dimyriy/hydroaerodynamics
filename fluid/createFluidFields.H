PtrList<basicPsiThermo> thermoFluid(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());
PtrList<volScalarField> KFluid(fluidRegions.size());
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<uniformDimensionedVectorField> gFluid(fluidRegions.size());
PtrList<compressible::turbulenceModel> turbulenceFluid(fluidRegions.size());
PtrList<volScalarField> DpDtFluid(fluidRegions.size());
PtrList<basicThermoCloud> parcelsFluid(fluidRegions.size());
List<scalar> initialMassFluid(fluidRegions.size());
forAll(fluidRegions, i){
    Info<< "\nReading thermophysical properties\n" << endl;
    thermoFluid.set(
        i,
        basicPsiThermo::New(fluidRegions[i]).ptr()
    );
    Info<< "\nAdding to rhoFluid\n" << endl;
    rhoFluid.set(
        i,
        new volScalarField
        (
            IOobject
            (
                "rho",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            thermoFluid[i].rho()
        )
    );
    Info<< "\nAdding to KFluid\n" << endl;
    KFluid.set(
        i,
        new volScalarField
        (
            IOobject
            (
                "K",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            thermoFluid[i].Cp()*thermoFluid[i].alpha()
        )
    );
    Info<< "\nReading field U\n" << endl;
    UFluid.set(
        i,
        new volVectorField
        (
            IOobject
            (
                "U",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );
    Info<< "    Adding to gFluid\n" << endl;
    gFluid.set(
        i,
        new uniformDimensionedVectorField
        (
            IOobject
            (
                "g",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::NO_WRITE
            )
        )
    );
    Info<< "    Adding to phiFluid\n" << endl;
    phiFluid.set(
        i,
        new surfaceScalarField
        (
            IOobject
            (
                "phi",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
            linearInterpolate(rhoFluid[i]*UFluid[i])&fluidRegions[i].Sf()
        )
    );
    Info<< "    Adding to turbulenceFluid\n" << endl;
    turbulenceFluid.set(
        i,
        autoPtr<compressible::turbulenceModel>(
            compressible::turbulenceModel::New
            (
                rhoFluid[i],
                UFluid[i],
                phiFluid[i],
                thermoFluid[i]
            )
        ).ptr()
    );
    Info<< "Creating field DpDt\n" << endl;
    DpDtFluid.set(
        i,
        new volScalarField
        (
            "DpDt",
            fvc::DDt
            (
                surfaceScalarField
                (
                    "phiU",
                    phiFluid[i]/fvc::interpolate(rhoFluid[i])
                ),
                thermoFluid[i].p()
            )
        )
    );
    parcelsFluid.set(
        i,
        new basicThermoCloud(
            "basicThermoCloud1",
            rhoFluid[i],
            UFluid[i],
            gFluid[i],
            thermoFluid[i]
        )
    );
}