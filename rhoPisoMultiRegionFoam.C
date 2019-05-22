/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2010 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    rhoPisoFoam

Description
    Transient PISO solver for conjugate fluid-solid analis of compressible,
    laminar or turbulent flow with lagrangian parcels.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "basicPsiThermo.H"
#include "turbulenceModel.H"
#include "fixedGradientFvPatchFields.H"
#include "regionProperties.H"
#include "compressibleCourantNo.H"
#include "solidRegionDiffNo.H"
#include "radiationModel.H"
#include "basicThermoCloud.H"
#include "thermophysicalFunction.H"
#include "residuals.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    double resU, resP, resT, continuityEr;
    #include "setRootCase.H"
    #include "createTime.H"

    regionProperties rp(runTime);

    #include "createFluidMeshes.H"
    #include "createSolidMeshes.H"

    #include "createFluidFields.H"
    #include "createSolidFields.H"

    #include "initContinuityErrs.H"

    #include "readTimeControls.H"
    #include "readSolidTimeControls.H"


    #include "compressibleMultiRegionCourantNo.H"
    #include "solidRegionDiffusionNo.H"
    #include "setInitialMultiRegionDeltaT.H"
    cout.precision(5);

    while (runTime.run()){
        #include "readTimeControls.H"
        #include "readSolidTimeControls.H"
        #include "readPIMPLEControls.H"


        #include "compressibleMultiRegionCourantNo.H"
        #include "solidRegionDiffusionNo.H"
        #include "setMultiRegionDeltaT.H"
        lduMatrix::debug = debugLevel;
        Info.level = debugLevel;
        runTime++;
        if(Pstream::master())
            printHeader(resPrint, runTime.value(), runTime.deltaT().value());
        Info<< "Time = " << runTime.timeName() << nl << endl;
        forAll(fluidRegions, i){
                #include "setRegionFluidFields.H"
                rho.storePrevIter();
                p.storePrevIter();
                #include "readFluidMultiRegionPISOControls.H"
                #include "solveFluid.H"
            }
        for(int solidCorrections=0; solidCorrections<nSolidCorrections; solidCorrections++){
            forAll(solidRegions, i){
                #include "setRegionSolidFields.H"
                #include "readSolidMultiRegionPISOControls.H"
                #include "solveSolid.H"
            }
            forAll(fluidRegions, i){
                #include "setRegionFluidFields.H"
                #include "readFluidMultiRegionPISOControls.H"
                #include "solidCorrections.H"
            }
            if(Pstream::master())
                printResiduals(resPrint, solidCorrections, resP, resU, resT);
        }
        runTime.write();
        if(Pstream::master())
            printFooter(resPrint, runTime.elapsedCpuTime(),runTime.elapsedClockTime(),continuityEr);
        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
