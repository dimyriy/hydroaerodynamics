void printResiduals(bool onScreen, bool onFile, int nIter, double timeValue, double resP, double resU, double resT, ofstream &resFile){
	if(onScreen)
		printf("|	%5i	|	%2.1e	|	%2.1e	|	%2.1e	|\n", nIter, resP, resU, resT);
	if(onFile){
		resFile.open("residuals.plt",std::ios::app);
		resFile<<"\n"<<nIter<<"	"<<timeValue<<"	"<<resP<<"	"<<resU<<"	"<<resT;
		resFile.close();
	}
}
void printResHeaders(){
	printf("-----------------------------------------------------------------\n|	nIter 	|	Res P	|	res U	|	resT	|\n-----------------------------------------------------------------\n");
}