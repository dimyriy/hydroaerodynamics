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
    porousIterativeMultiregionFoam

Description
    Combination of heatConductionFoam and buoyantFoam for conjugate heat
    transfer between a solid region and fluid region

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "hCombustionThermo.H"
#include "turbulenceModel.H"
#include "BasicReactingCloud.H"
#include "psiChemistryModel.H"
#include "chemistrySolver.H"
#include "radiationModel.H"
#include "fixedGradientFvPatchFields.H"
#include "regionProperties.H"
#include "compressibleCourantNo.H"
#include "solidRegionDiffNo.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    ofstream resid;
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
    double uxres,spres,hsres,pres,specieres;
    int niter=0;
    bool wrt;
    cout.precision(5);
    while (runTime.run()){
        #include "readTimeControls.H"
        #include "readSolidTimeControls.H"
        #include "readPIMPLEControls.H"
        lduMatrix::debug = debugLevel;
        Info.level = infoLevel;
        #include "compressibleMultiRegionCourantNo.H"
        #include "solidRegionDiffusionNo.H"
        #include "setMultiRegionDeltaT.H"
        wrt=fileWrite;
        if((niter==0)&&(Pstream::master())&&(fileWrite)){//write residual file headers
            resid.open("residuals.plt");
            if(solveSpecies) resid<<"Variables=\"Iterations\",\"Velocity\",\"Energy\",\"Species\",\"Pressure\"";
            else resid<<"Variables=\"Iterations\",\"Velocity\",\"Energy\",\"Pressure\"";
        }
        runTime++;
        Info<< "Time = " << runTime.timeName() << nl << endl;
        if (nOuterCorr != 1){
            forAll(fluidRegions, i){//set fluid fields
                #include "setRegionFluidFields.H"
                #include "storeOldFluidFields.H"
            }
        }
        if((Pstream::master())&&((stepIterLevel)||(fileWrite))){//write header
            cout<<"\nStep: "<<runTime.deltaT().value()<<"s; Time: "<<runTime.value()<<"s; CoNum: "<<CoNum<<"; Exec. Time: "<< runTime.elapsedCpuTime()<<"s";
            if(!fileWrite){
                if(solveSpecies) cout<<"\n-------------------------------------------------------------------"<<nl;
                else cout<<"\n---------------------------------------------------"<<nl;
            }
        }
        if((Pstream::master())&&(stepIterLevel)){//write header
            if(solveSpecies) printf("Iter	Residuals U	Residuals T	Residuals Y	Residuals P\n-------------------------------------------------------------------\n");
            else printf("Iter	Residuals U	Residuals T	Residuals P\n---------------------------------------------------\n");
        }

        // --- MAIN LOCAL ITERATIONS

        for (int oCorr=0; oCorr<nOuterCorr; oCorr++){//local timestep iterations
            niter++;
            if((Pstream::master())&&(stepIterLevel)&&(oCorr%30==0)&&(oCorr>=30)){//write header
                if(solveSpecies) printf("-------------------------------------------------------------------\nIter	Residuals U	Residuals T	Residuals Y	Residuals P\n-------------------------------------------------------------------\n");
                else printf("---------------------------------------------------\nIter	Residuals U	Residuals T	Residuals P\n---------------------------------------------------\n");
            }
            pres=0;uxres=0;spres=0;hsres=0,pres=0,specieres=0;
            forAll(fluidRegions, i){//solve for fluid region
                Info<< "\nSolving for fluid region "<< fluidRegions[i].name() << nl;
                #include "setRegionFluidFields.H"
                #include "readFluidMultiRegionPIMPLEControls.H"
                #include "solveFluid.H"
            }
            forAll(solidRegions, i){//solve for solid region
                Info<< "\nSolving for solid region "<< solidRegions[i].name() << endl;
                #include "setRegionSolidFields.H"
                #include "readSolidMultiRegionPIMPLEControls.H"
                #include "solveSolid.H"
            }
            if(stepIterLevel){//print residuals on screen
                if(Pstream::master()){
                    if(solveSpecies) printf("%d	%6.5e	%6.5e	%6.5e	%6.5e\n",oCorr,uxres,hsres,spres,pres);
                    else printf("%d	%6.5e	%6.5e	%6.5e\n",oCorr,uxres,hsres,pres);
                }
            }
            if(Pstream::master()&&(fileWrite)){//write residuals to file
                    if(solveSpecies) resid<<nl<<niter<<"	"<<uxres<<"	"<<hsres<<"	"<<spres<<"`	"<<pres;
                    else resid<<nl<<niter<<"	"<<uxres<<"	"<<hsres<<"	"<<pres;
            }
            if((fabs(uxres)<=UxConvergence)&&(fabs(pres)<=pConvergence)&&(fabs(hsres)<=hsConvergence)){
                if(solveSpecies){ if(spres<=specieConvergence) break; }
                else break;
            }
        }
        /*
        forAll(fluidRegions, i){
            turbulence[i].correct();
            rhoFluid[i]=pChemistryFluid[i].thermo().rho();
        }
        */
        runTime.write();
        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }
    if((Pstream::master())&&(wrt)){ //close file and write "End"
        resid.close();
        if(Info.level==0) cout<<"\nEnd.\n";
    }
    Info<<"\nEnd.\n"<<endl;
    return 0;
}


// ************************************************************************* //
