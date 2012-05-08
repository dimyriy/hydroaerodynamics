fvSolution solutionDict(runTime);
const dictionary& simple = solutionDict.subDict("SIMPLE");
bool resPrint = simple.lookupOrDefault("resPrint", false);
int debugLevel = simple.lookupOrDefault("debugLevel", 1);
int nSolidCorrections(readInt(simple.lookup("nSolidCorrections")));
int nNonOrthCorr =
    simple.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);
bool momentumPredictor =
    simple.lookupOrDefault<Switch>("momentumPredictor", true);
bool transonic =
    simple.lookupOrDefault<Switch>("transonic", false);
