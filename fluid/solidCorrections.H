tmp<fvVectorMatrix> UEqn
(
    fvm::ddt(rho, U)
  + fvm::div(phi, U)
  + turbulence.divDevRhoReff(U)
);
#include "hEqn.H"
#include "pEqn.H"
rho = thermo.rho();