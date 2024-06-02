// header with global objects

#ifndef OBJS_HH
#define OBJS_HH

#include "../include/structs.hh"

//reco tree from acts+pandora
	TTree* recoTuple;

//trees digi hits
	TTree* vdxBarrelTuple;
	TTree* vdxEndcapTuple;
	TTree* itBarrelTuple;
	TTree* itEndcapTuple;
	TTree* otBarrelTuple;
	TTree* otEndcapTuple;
	TTree* ecBarrelTuple;
	TTree* ecEndcapTuple;
	TTree* hcBarrelTuple;
	TTree* hcEndcapTuple;
	TTree* hcRingTuple;
	
//trees sim hits
	TTree* vdxBarrelSimTuple;
	TTree* vdxEndcapSimTuple;
	TTree* itBarrelSimTuple;
	TTree* itEndcapSimTuple;
	TTree* otBarrelSimTuple;
	TTree* otEndcapSimTuple;
	TTree* ecBarrelSimTuple;
	TTree* ecEndcapSimTuple;
	TTree* hcBarrelSimTuple;
	TTree* hcEndcapSimTuple;
	TTree* hcRingSimTuple;

//branches' addresses MC RC
	particleMC partMC;
	particleRC partRC;
	trackRC trkRC;
	jetRC jtRC;
	clusterRC cluRC;
//branches' addresses Hits	
	trackerData vxBarHits;
	trackerData vxEndHits;
	trackerData itBarHits;
	trackerData itEndHits;
	trackerData otBarHits;
	trackerData otEndHits;
	caloData ecBarHits;
	caloData ecEndHits;
	caloData hcBarHits;
	caloData hcEndHits;
	caloData hcRinHits;
//branches' addresses SimHits		
	trackerData vxBarSimHits;
	trackerData vxEndSimHits;
	trackerData itBarSimHits;
	trackerData itEndSimHits;
	trackerData otBarSimHits;
	trackerData otEndSimHits;
	caloData ecBarSimHits;
	caloData ecEndSimHits;
	caloData hcBarSimHits;
	caloData hcEndSimHits;	
	caloData hcRinSimHits;	
	

//data containers
//	vector<double> enSteps{20, 60, 100, 140, 180, 220, 260, 300, 340, 380, 420, 460, 500, 540, 580, 620, 660, 700, 740, 780, 820, 860, 900, 940, 980};
	vector<double> enSteps{5, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215, 225, 235, 245, 255, 265, 275, 285, 295, 305, 315, 325, 335, 345, 355, 365, 375, 385, 395, 405, 415, 425, 435, 445, 455, 465, 475, 485, 495, 505, 515, 525, 535, 545, 555, 565, 575, 585, 595, 605, 615, 625, 635, 645, 655, 665, 675, 685, 695, 705, 715, 725, 735, 745, 755, 765, 775, 785, 795, 805, 815, 825, 835, 845, 855, 865, 875, 885, 895, 905, 915, 925, 935, 945, 955, 965, 975, 985, 995};
	vector<double> thSteps{11.,13.,15.,17.,19.,21.,23.,25.,27.,29.,31.,33.,35.,37.,39.,41.,43.,45.,47.,49.,51.,53.,55.,57.,59.,61.,63.,65.,67.,69.,
						   71.,73.,75.,77.,79.,81.,83.,85.,87.,89.};

	vector<TH1F*> reco_pTkMc(enSteps.size());

	vector<TH1F*> reco_pTkMc_th(thSteps.size());
		
	vector<int> evtBinsE(enSteps.size(),0);	//evts for effic
	vector<int> evtBinsTh(thSteps.size(),0);
	
	TGraphErrors* resP_vsE;	//resol P vs E
	TGraphErrors* effP_vsE;		//eff P cut vs E

	TGraphErrors* resP_vsTh;	//resol P vs th
	TGraphErrors* effP_vsTh;	//eff P cut vs th
	
	vector<int> trStates;
	vector<double> enTrack;
	
#endif
