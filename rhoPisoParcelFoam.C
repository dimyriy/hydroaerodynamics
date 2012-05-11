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
    Transient PISO solver for compressible, laminar or turbulent flow.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "basicPsiThermo.H"
#include "turbulenceModel.H"
#include "radiationModel.H"
#include "basicThermoCloud.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"

    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "createFields.H"
    #include "createRadiationModel.H"
    #include "initContinuityErrs.H"
    #include "readTimeControls.H"
    #include "compressibleCourantNo.H"
    #include "setInitialDeltaT.H"


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;
    double resU=0, resP=0, resT=0;
    int nIter = 0;
    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "readPISOControls.H"
        dictionary residualProperties = mesh.solutionDict().subDict("residuals");
        bool residualsPrint = Switch(residualProperties.lookupOrDefault("residualsPrint", false));
        if(residualsPrint)
            Info.level = 0;
        if(residualsPrint && Pstream::master()&&(nIter == 0 ||(nIter>30&&nIter%30==0)))
            cout<<"-----------------------------------------------------------------\n|	Time	|	ResP	|	ResU	|	ResT	|\n-----------------------------------------------------------------\n";
            nIter++;
        #include "compressibleCourantNo.H"
        #include "setDeltaT.H"
        runTime++;

        Info<< "Time = " << runTime.timeName() << nl << endl;
        #include "rhoEqn.H"
        parcels.evolve();
        Info<<"Parcels evolved"<<endl;
        #include "UEqn.H"

        // --- PISO loop
        for (int corr=1; corr<=nCorr; corr++)
        {
            #include "hEqn.H"
            #include "pEqn.H"
        }

        turbulence->correct();

        rho = thermo.rho();
        if(Pstream::master()&&residualsPrint)
            printf("| %4.3e	|	%2.1e	|	%2.1e	|	%2.1e	|\n", runTime.value(), resP, resU, resT);

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
