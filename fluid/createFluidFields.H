    // Initialise fluid field pointer lists
    PtrList<IOdictionary> chemistryProperties(fluidRegions.size());
    PtrList<psiChemistryModel> pChemistryFluid(fluidRegions.size());
//    PtrList<hsCombustionThermo> thermoFluid(fluidRegions.size());
    PtrList<DimensionedField<scalar, volMesh> > kappaFluid(fluidRegions.size());
    PtrList<volScalarField> rhoFluid(fluidRegions.size());
    PtrList<volScalarField> pFluid(fluidRegions.size());
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

    // Populate fluid field pointer lists
    forAll(fluidRegions, i)
    {
        Info<< "*** Reading fluid mesh thermophysical properties for region "
            << fluidRegions[i].name() << nl << endl;

        Info<< "    Adding to thermoFluid\n" << endl;
        pChemistryFluid.set
        (
            i,
            autoPtr<psiChemistryModel>
            (
                psiChemistryModel::New
                (
                    fluidRegions[i]
                )
            ).ptr()
        );
/*        thermoFluid.set
        (
            i,
            hsCombustionThermo::New(fluidRegions[i]).ptr()
        );
*/
        Info<< "*** Reading chemistry properties for region "
            << fluidRegions[i].name() << nl <<endl;
        chemistryProperties.set
        (
            i,
            new IOdictionary
            (
                IOobject
                (
                    "chemistryProperties",
                    runTime.constant(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE,
                    false
                )
            )
        );
        Info<< "    Adding to rhoFluid\n" << endl;
        rhoFluid.set
        (
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
                pChemistryFluid[i].thermo().rho()
            )
        );

        Info<< "    Adding to KFluid\n" << endl;

        KFluid.set
        (
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
                pChemistryFluid[i].thermo().Cp()*pChemistryFluid[i].thermo().alpha()
            )
        );

        Info<< "    Adding to UFluid\n" << endl;
        UFluid.set
        (
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
        pFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "p",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );

        Info<< "    Adding to phiFluid\n" << endl;
        phiFluid.set
        (
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
                linearInterpolate(rhoFluid[i]*UFluid[i])
                    & fluidRegions[i].Sf()
            )
        );

        Info<< "    Adding to gFluid\n" << endl;
        gFluid.set
        (
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
        Info<< "    Adding to turbulence\n" << endl;
        turbulence.set
        (
            i,
            autoPtr<compressible::turbulenceModel>
            (
                compressible::turbulenceModel::New
                (
                    rhoFluid[i],
                    UFluid[i],
                    phiFluid[i],
                    pChemistryFluid[i].thermo()
                )
            ).ptr()
        );
        radiationFluid.set
        (
            i,
            autoPtr<radiation::radiationModel>
            (
                radiation::radiationModel::New
                (
                    pChemistryFluid[i].thermo().T()
                )
            ).ptr()
        );
        GFluid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "G",
                    runTime.timeName(),
                    fluidRegions[i],
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                fluidRegions[i]
            )
        );
        parcelFluid.set
        (
            i,
            new thermoReactingCloud
            (
                "reactingCloud1",
                rhoFluid[i],
                UFluid[i],
                gFluid[i],
                pChemistryFluid[i].thermo()
            )
        );

        kappaFluid.set
        (
            i,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
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


        chemistryShFluid.set
        (
            i,
            new DimensionedField<scalar, volMesh>
            (
                IOobject
                (
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
