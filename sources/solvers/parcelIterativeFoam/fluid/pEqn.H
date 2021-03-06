rho = thermo.rho();

volScalarField rUA = 1.0/UEqn.A();
U = rUA*UEqn.H();
phi =
    fvc::interpolate(rho)*((fvc::interpolate(U) & mesh.Sf())
      + fvc::ddtPhiCorr(rUA, rho, U, phi)
    );

for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++){
    fvScalarMatrix pEqn(
        fvm::ddt(psi, p)
      + fvc::div(phi)
      - fvm::laplacian(rho*rUA, p)
     ==
        parcels.Srho()
    );
    pEqn.relax();
    pres = pEqn.solve().initialResidual();
    if (nonOrth == nNonOrthCorr){
        phi += pEqn.flux();
    }
}

#include "rhoEqn.H"
#include "compressibleContinuityErrors.H"

U -= rUA*fvc::grad(p);
U.correctBoundaryConditions();
DpDt = fvc::DDt(surfaceScalarField("phiU", phi/fvc::interpolate(rho)), p);
//DpDt = fvc::ddt(p);
