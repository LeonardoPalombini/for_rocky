//declare hists and graphs

#include "objs.hh"
#include "dataUtils.cpp"

void declareDataObjs(){
	
	double rng = 0.1; //variable hist range
	for(int i = 0; i< reco_pTkMc.size(); i++){
		
		rng = 0.09 * ( 4.e-6 * enSteps[i] * enSteps[i] + 1. );
		reco_pTkMc[i] = new TH1F( Form("reco_pTkMc_%i",i), Form("(p_{reco}-p_{mc})/p_{mc} (ACTS) %.0f GeV; #Deltap/p",enSteps[i]), 1000,-1,0.3);//50,-rng,rng
	}
	
	double eta;
	for(int i = 0; i< reco_pTkMc_th.size(); i++){

		reco_pTkMc_th[i] = new TH1F( Form("reco_pTkMc_th_%i",i), Form("(p_{reco}-p_{mc})/p_{mc} (ACTS) #theta=%.1f; #Deltap/p",thSteps[i]), 1000,-1,0.3);
	}
	
	resP_vsE = new TGraphErrors();
	resP_vsE->SetTitle("Momentum resolution vs True E; true E [GeV]");
	effP_vsE = new TGraphErrors();
	effP_vsE->SetTitle("Tracking efficiency vs True E; true E [GeV]");
	
	resP_vsTh = new TGraphErrors();
	resP_vsTh->SetTitle("Momentum resolution vs True #theta; true #theta");
	effP_vsTh = new TGraphErrors();
	effP_vsTh->SetTitle("Tracking efficiency vs True #theta; true #theta");

}
