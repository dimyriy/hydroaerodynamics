                if(hsres <= max(mag(hsRES - thermo.T())/max(mag(thermo.T()),dimensionedScalar("1e-10",dimensionSet(0,0,0,1,0,0,0),1e-10))).value())
                {
                    hsres=max(mag(hsRES - thermo.T())/max(mag(thermo.T()),dimensionedScalar("1e-10",dimensionSet(0,0,0,1,0,0,0),1e-10))).value();
                }