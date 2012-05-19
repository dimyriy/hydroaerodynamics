{
    for (int nonOrth=0; nonOrth<=nNonOrthCorr; nonOrth++)
    {
        tmp<fvScalarMatrix> TEqn
        (
            fvm::laplacian(K, T)
        );
        TEqn().relax();
        TEqn().solve();
    }

    Info<< "Min/max T:" << min(T) << ' ' << max(T) << endl;
    if( (min(T).value()<280) || (max(T).value() > 320) )
        cout<<"Min/max T:" << min(T).value() << ' ' << max(T).value()<<"\n";
}
