//void energy_res(int num){
void energy_res(){

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	TRandom3 r(0);

	double m[2] = {0.000417863,0.000423296};
	double b[2] = {0.555814,0.568549};
	double nt_Ge[2], nt_NaI;
	double smear_Ge[2], smear_NaI;
	TH1F *hEnSmear[2];
	TH2F *h2DSmear = new TH2F("h2DSmear","h2DSmear",3000,0,3000,3000,0,3000);
	TH2F *h2DSmearV = new TH2F("h2DSmearV","h2DSmearV",3000,0,3000,3000,0,3000);

	//TFile *sim = new TFile(Form("simulated.root"),"UPDATE");
	TFile *sim = new TFile(Form("1Msim.root"),"UPDATE");
	TTree *tree = (TTree*) sim->Get("tree2");
		
	for (Int_t i=0; i<2; i++){
		int det = i+1;

		tree->SetBranchAddress(Form("GE%iEN",det),&nt_Ge[i]);
		tree->Branch(Form("GE%iRes",det),&smear_Ge[i],Form("GE%iRes/D",det));

		hEnSmear[i] = new TH1F(Form("hEnSmear%i",i),Form("hEnSmear%i",i),6000,0,3000);
	}
	tree->SetBranchAddress("NaI",&nt_NaI);
	//tree->Branch("NaIRes",&smear_NaI,"NaIRes/D");

	Int_t nentries = (Int_t) tree->GetEntries();
	//nentries = 100;

	for (Int_t k = 0; k<nentries; k++) {
		if (k % 100000==0){
			printf("%i\n",k);
		}
		tree->GetEntry(k);

		double sigma[2];
		for (int i=0; i<2; i++){
			sigma[i] = nt_Ge[i]*m[i]+b[i];
			smear_Ge[i] = nt_Ge[i];// + r.Gaus(0,sigma[i]);
			hEnSmear[i]->Fill(smear_Ge[i]);
		}
		h2DSmear->Fill(smear_Ge[0],smear_Ge[1]);
		if (nt_NaI<50){
			h2DSmearV->Fill(smear_Ge[0],smear_Ge[1]);
		}
		
		tree2->Fill();
/*		if (smear_Ge[0]>230 && smear_Ge[0]<232){
			cout << "------ " << k << " -------" << endl;
			cout << sigma[0] << "  " << sigma[1] << endl;
			cout << nt_Ge[0] << "  " << smear_Ge[0] << "  ||  " <<  nt_Ge[1] << "  " << smear_Ge[1] << endl;
		}*/
	}
	tree->Write("", TObject::kOverwrite);
	sim->Write();

//	TCanvas *c1 = new TCanvas("c1","c1",800,600);
//	c1->Divide(2,1);
	for (int i=0; i<2; i++){
//		c1->cd(i+1);
//		hEnSmear[i]->Draw();

		hEnSmear[i]->Write();
	}

	TCanvas *c2 = new TCanvas("c2","c2",900,600);
	c2->Divide(2,1);
	c2->cd(1);
	h2DSmear->Draw("col");
	h2DSmear->Write();
	
	c2->cd(2);
	h2DSmearV->Draw("col");
	h2DSmearV->Write();
	

	sim->Close();

	

}
