// get data from trees
#include "dataUtils.cpp"


// find best tracks and momentum
int anaTracks(double cutR = 0.05){

	ifstream file("dirs.txt");

	bool check1, check2;
	
	double mcE, mcTheta, mcEta, mcPhi, mcP;
	int ascTrk;
	double trackPi, trackPf;
	double dE, midE, dEta, minEta, maxEta, theta, midTh, dTh;
	
	int entries = recoTuple->GetEntries();
	cout << "Analyzing tracks, events: " << entries << endl;
//	entries = 10000;
	for(int i = 0; i < entries; i++){
		
		if(10*i % entries == 0) cout << 100.*(int)i / (int)entries << "%" << endl;
		
		recoTuple->GetEntry(i);
		
		mcE = partMC.e[0];
		mcTheta = getTheta( partMC.mox[0], partMC.moy[0], partMC.moz[0] );
		mcEta = getEta( partMC.mox[0], partMC.moy[0], partMC.moz[0] );
		mcPhi = getPhi( partMC.mox[0], partMC.moy[0] );
		mcP = getTotP( partMC.mox[0], partMC.moy[0], partMC.moz[0] );
		
		ascTrk = bestTrack( mcE, mcEta, mcPhi, trackPi, trackPf, cutR );
		
		trStates.push_back(ascTrk);
		enTrack.push_back(trackPf);
		
		for(int est = 0; est < enSteps.size(); est++){
			midE = enSteps[est];
			dE = enSteps[1]-enSteps[0]; //generic
			
			if(mcE <= (midE-0.5*dE) || mcE > (midE+0.5*dE)) continue;

			if(ascTrk > -1) reco_pTkMc[est]->Fill( (trackPi - mcP)/mcP );
			evtBinsE[est]++;
		}
				
		for(int tst = 0; tst < thSteps.size(); tst++){
			midTh = thSteps[tst];
			dTh = thSteps[1]-thSteps[0]; //generic	

			theta = mcTheta * 180 / 3.14159;
			theta = (theta<90.) ? theta : 180.-theta;
			if(theta <= (midTh-0.5*dTh) || theta > (midTh+0.5*dTh)) continue;
			
			if(ascTrk > -1) reco_pTkMc_th[tst]->Fill( (trackPi - mcP)/mcP );
			evtBinsTh[tst]++;
		}
	}	
	return 0;	
}

