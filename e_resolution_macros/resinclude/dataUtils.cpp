//data analysis routines

#ifndef DATAUTILS_CPP
#define DATAUTILS_CPP


#include "../include/structs.hh"
#include "objs.hh"

// conversions

double getTheta(double x, double y, double z){
	double th = atan( sqrt( x*x + y*y ) / z );
	th = th > 0. ? th : 3.1416+th;
	return th;
}

double getEta(double x, double y, double z){
	double th = atan( sqrt( x*x + y*y ) / z );
	th = th > 0. ? th : 3.1416+th;
	return - log( tan(th/2.) );
}

double getEtafromTheta(double th){ 	//conv from degrees
	th = th * 3.14159 / 180.;
	return - log( tan(th/2.) );
}

double getPhi(double x, double y){
	double phi;
	if(x > 0) phi = atan( y / x );
	else{
		if(y > 0) phi = atan( y / x ) + 3.14159;
		else phi = atan( y / x ) - 3.14159;
	}
	return phi;
}

double getTotP(double x, double y, double z){
	return sqrt(x*x+y*y+z*z);
}

void calculateRMS90(TH1F* h, double &r, double &e){
		
	int end;
	int sum;
	double nentries90 = 0.9 * (double)h->Integral();
	double rms = 1000, err_rms = 1000;
						
	for(int i = 1; i < h->GetNbinsX()+1; i++){
								
		sum = 0;
		for(int j = i; j < h->GetNbinsX()+1; j++){
													
			sum += h->GetBinContent(j);
			end = j;
			if((double)sum > nentries90) break;
		}
											
		if(sum < nentries90-1) break;  
														
		h->GetXaxis()->SetRangeUser(h->GetBinCenter(i), h->GetBinCenter(end));
		if(h->GetRMS() < rms){
			rms = h->GetRMS();
			err_rms = h->GetRMSError();																				}
	}
							
	r = rms;
	e = err_rms;
	h->GetXaxis()->SetRangeUser(h->GetBinCenter(1), h->GetBinCenter(h->GetNbinsX()));
}



//=======================================================================

//find track associated to mc
int bestTrack(double mcE, double mcEta, double mcPhi, double &mom0, double &mom1, double cutRtrk = 0.05){
	
//exclude trackless events
	if(trkRC.ntr == 0) return -100;

//find track associated to primary e	
	int firstTS, lastTS;
	int bestTS = -10;
	double trkTheta, trkEta, trkPhi, dR;
	double pre_dR = 1.;

	int bestI;
	
	double testMom;
	mom0 = -1000.;
	mom1 = -1000.;
	for(int iTr = 0; iTr < trkRC.ntr; iTr++){
		
		firstTS = trkRC.idxVt[iTr];
		lastTS = trkRC.idxCalo[iTr];
		trkEta = log( tan( (atan(trkRC.tlambda[firstTS]) +0.5*3.14159) / 2. ) );	// no (-) to adapt coordinates
		trkPhi = trkRC.phi[firstTS];
		
		dR = sqrt(pow(mcEta-trkEta,2)+pow(mcPhi-trkPhi,2));
		testMom = 0.3 * 4. / ( abs(trkRC.r1[firstTS]) * 1000. * cos( atan( trkRC.tlambda[firstTS] ) ) );
		
		if(dR < cutRtrk && dR < pre_dR){
			bestTS = lastTS;
			bestI = iTr;
			pre_dR = dR;
			if(trkRC.r1[firstTS] < 0){
				mom0 = testMom;
				mom1 = 0.3 * 4. / ( abs(trkRC.r1[lastTS]) * 1000. * cos( atan( trkRC.tlambda[lastTS] ) ) );
			} 
		}
	}
	
	if(bestTS < -1) return -200;
	else return bestTS;
}



// generic fit procedure for reco and match spectra
// minG=-0.015, maxG=0.01, minC=-0.04, maxC=0.02, optT=e(energy),t(angle), optF=f(fit resolution), n(no fit resolution), rms=r(use rms90), n(default fit)
void fitResolEff(vector<TH1F*> v, TGraphErrors* gr, TGraphErrors* ge, double minG, double maxG, double minC, double maxC, char optT='e', char optRms='n'){
	
	vector<TF1*> fsk(v.size());
	TF1* gauss = new TF1("gauss", "gaus", -0.1, 0.1);

	double minEta, maxEta;
	int minx, manx;
	
	if(gr!=NULL) gr->Print();
	if(ge!=NULL) ge->Print();

	double rms, err_rms;
	double k,n;

	for(int i = 0; i < fsk.size(); i++){
		if(optRms != 'r'){
			fsk[i] = new TF1("fsk", "crystalball", -0.1, 0.1);
			fsk[i]->SetParNames("k","#mu","#sigma","#alpha","n");

			v[i]->Fit(gauss, "NQ0", "", minG, maxG);
			fsk[i]->SetParameters(gauss->GetParameter(0),gauss->GetParameter(1),gauss->GetParameter(2),1.,10.);

			v[i]->Fit(fsk[i], "SMQ+", "", minC, maxC);

			minx = v[i]->FindBin(fsk[i]->GetParameter(1)-5*fsk[i]->GetParameter(2));	
			manx = v[i]->FindBin(fsk[i]->GetParameter(1)+5*fsk[i]->GetParameter(2));
			//minx = v[i]->FindBin(-.05);
			//manx = v[i]->FindBin(.05);
		}	
		
		if(optT == 'e'){
			if(gr != NULL){
				if(optRms == 'r'){
					calculateRMS90(v[i], rms, err_rms);
					gr->SetPoint(i, enSteps[i], rms);
					gr->SetPointError(i, .5*(enSteps[1]-enSteps[0]), err_rms);	
				}
				else{
					gr->SetPoint(i, enSteps[i], fsk[i]->GetParameter(2)*100);
					gr->SetPointError(i, .5*(enSteps[1]-enSteps[0]), fsk[i]->GetParError(2)*100);
				}
				
			}
			if(ge != NULL){
//==Choose k=GetEntries if inclusive efficiency (=a track exists), k=Integral if good reco efficiency (=a track exists with a |pReco-pMc|<5*resolution)
			//	k = v[i]->Integral(minx, manx);
				k = v[i]->GetEntries();
				n = evtBinsE[i];
				ge->SetPoint(i, enSteps[i], (k+1) / (n+2));	//bayesian treatment of efficiency, exp val = (k+1)/(n+2), variance=(k+1)(k+2)/(n+2)(n+3)-(k+1)^2/(n+2)^2
				ge->SetPointError(i, .5*(enSteps[1]-enSteps[0]), sqrt( ((k+1)*(k+2))/((n+2)*(n+3)) - pow((k+1)/(n+2),2) ) );		
			}
		}
		else{
//			minEta = getEtafromTheta(thSteps[i]+2.5);
//			maxEta = getEtafromTheta(thSteps[i]-2.5);
			if(gr != NULL){
				if(optRms == 'r'){
					calculateRMS90(v[i], rms, err_rms);
					gr->SetPoint(i, thSteps[i], rms);
					gr->SetPointError(i, 1., err_rms);
				}
				else{
					gr->SetPoint(i, thSteps[i], fsk[i]->GetParameter(2)*100);
					gr->SetPointError(i, 1., fsk[i]->GetParError(2)*100);
				}
				
			//	trackCutsTh[i] = 5*fsk[i]->GetParameter(2);
			}
			if(ge != NULL){
//==Choose k=GetEntries if inclusive efficiency (=a track exists), k=Integral if good reco efficiency (=a track exists, with a |pReco-pMc|<some quantity)
			//  k = v[i]->Integral(minx, manx);
			  	k = v[i]->GetEntries();
				n = evtBinsTh[i];
				ge->SetPoint(i, thSteps[i], (k+1) / (n+2));     //bayesian treatment of efficiency, exp val = (k+1)/(n+2), variance=(k+1)(k+2)/(n+2)(n+3)-(k+1)^2/(n+2)^2
				ge->SetPointError(i, 1., sqrt( ((k+1)*(k+2))/((n+2)*(n+3)) - pow((k+1)/(n+2),2) ) );	
			}
		}
	}
	
	return;
}

#endif
