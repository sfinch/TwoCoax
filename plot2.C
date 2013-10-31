void plot2(){

	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);

	//TFile *sim = new TFile(Form("simulated.root"),"READ");
	TFile *sim = new TFile(Form("1Msim.root"),"READ");
	TTree *tree = (TTree*) sim->Get("tree2");

	TH1F *hEn[2];
	TH1F *hEnV[2];
	TH1F *hEnSmear[2];
	TH2F *h2DSmear = new TH2F("h2DSmear","h2DSmear",3000,0,3000,3000,0,3000);
	TH2F *h2DSmearV = new TH2F("h2DSmearV","h2DSmearV",3000,0,3000,3000,0,3000);

		
	for (Int_t i=0; i<2; i++){
		int det = i+1;
		
		hEn[i] = new TH1F(Form("hEn%i",i),Form("hEn%i",i),6000,0,3000);
		hEnV[i] = new TH1F(Form("hEnV%i",i),Form("hEnV%i",i),6000,0,3000);
		hEnSmear[i] = new TH1F(Form("hEnSmear%i",i),Form("hEnSmear%i",i),6000,0,3000);

		tree->Project(Form("hEn%i",i),Form("GE%iEN",det));
		tree->Project(Form("hEnSmear%i",i),Form("GE%iRes",det));

	}

	TCanvas *c1 = new TCanvas("c1","c1",800,600);
	c1->Divide(2,1);
	for (Int_t i=0; i<2; i++){
		c1->cd(i+1);
		gPad->SetLogy();
		hEn[i]->Draw();
		hEnSmear[i]->SetLineColor(2);
		hEnSmear[i]->Draw("same");
		
	}
	
}
