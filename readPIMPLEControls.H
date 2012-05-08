
fvSolution solutionDict(runTime);
const dictionary& pimple = solutionDict.subDict("PIMPLE");
int nSolidCorrections(readInt(pimple.lookup("nSolidCorrections")));
int debugLevel(readInt(pimple.lookup("debugLevel")));
bool resPrint = pimple.lookupOrDefault<Switch>("resPrint",false);
