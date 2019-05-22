    // We do not have a top-level mesh. Construct the fvSolution for
    // the runTime instead.
    fvSolution solutionDict(runTime);

    const dictionary& pimple = solutionDict.subDict("PIMPLE");

    int nOuterCorr(readInt(pimple.lookup("nOuterCorrectors")));
    bool onScreen(Switch(pimple.lookupOrDefault("printResidualsOnScreen", false)));
    bool onFile(Switch(pimple.lookupOrDefault("writeResidualsToFile", false)));
    int dbgLvl(readInt(pimple.lookup("debugLevel")));