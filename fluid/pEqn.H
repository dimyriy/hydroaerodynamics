rho = thermo.rho();

volScalarField rUA = 1.0/UEqn().A();
U = rUA*UEqn().H();

if (transonic){
    surfaceScalarField phid(
        "phid",
        fvc::interpolate(psi)
       *(
            (fvc::interpolate(U) & mesh.Sf())
          + fvc::ddtPhiCorr(rUA, rho, U, phi)
        )
    );

    for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++){
        fvScalarMatrix pEqn(
            fvm::ddt(psi, p)
          + fvm::div(phid, p)
          - fvm::laplacian(rho*rUA, p)
        );

        pEqn.solve();

        if (nonOrth == nNonOrthCorr){
            phi == pEqn.flux();
        }
    }
}
else{
    phi =
        fvc::interpolate(rho)*(
            (fvc::interpolate(U) & mesh.Sf())
          + fvc::ddtPhiCorr(rUA, rho, U, phi)
        );

    for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++){
        fvScalarMatrix pEqn(
            fvm::ddt(psi, p)
          + fvc::div(phi)
          - fvm::laplacian(rho*rUA, p)
        );

        pEqn.solve();

        if (nonOrth == nNonOrthCorr){ phi += pEqn.flux(); }
    }
}

rho = thermo.rho();
rho = max(rho, rhoMin);
rho = min(rho, rhoMax);
rho.relax();
#include "rhoEqn.H"
thermo.correct();
Info<<"rho max/min :    "<<max(rho).value()<<" "<<min(rho).value()<<endl;
#include "compressibleContinuityErrors.H"

U -= rUA*fvc::grad(p);
U.correctBoundaryConditions();

DpDt = fvc::DDt(surfaceScalarField("phiU", phi/fvc::interpolate(rho)), p);
K = thermo.Cp()*turbulence.alphaEff();
