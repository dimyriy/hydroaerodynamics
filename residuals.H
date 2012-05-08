
void printHeader(bool resPrint, double dt, double t){
    if(resPrint){
        cout.precision(2);
    	std::cout.setf(std::ios::scientific,std::ios::floatfield);
        std::cout<<"; Flow Time: "<<t<<"s; "<<"dt: "<<dt<<"s";
        std::cout<<"\n-----------------------------------------------------------\n"
        	     <<  "|  Iter   |    Resid. P   |    Resid. U   |    Resid. T   |"
        	     <<"\n-----------------------------------------------------------\n";
        std::cout.unsetf(std::ios::floatfield);
        cout.precision(5);
    }
}

void printResiduals(bool resPrint, int solidCorrections, double resP, double resU, double resT){
    if(resPrint)
        printf("|   %5i |   %5.4le  |   %5.4le  |   %5.4le  |\n",solidCorrections,resP,resU,resT);
}

void printFooter(bool resPrint, double elapsedCpuTime, double elapsedClockTime, double continuityEr){
    if(resPrint){
        cout.precision(2);
        std::cout.setf(std::ios::scientific,std::ios::floatfield);
        cout<<"-----------------------------------------------------------"
    		<<"\nCont. err: "<<continuityEr;
        std::cout.unsetf(std::ios::floatfield);
        cout<<"; Ex. Time: " << elapsedClockTime << "s";
        cout.precision(5);
    }
}
