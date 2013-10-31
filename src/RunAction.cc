//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/RunAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
:fileName("simulated")
{
  runMessenger = new RunActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  // Make root file
  G4String name = fileName+".root";
  fMC = new TFile(name.c_str(),"RECREATE");
  tMC = new TTree("tree2","MonteCarlo Data");

  // Add branches to tree
  tMC->Branch("NaI",&enDep[2],"NaI/D");
  for (int j=0; j<2; j++){
    int det = j+1;
    tMC->Branch(Form("GE%iEN",det),&enDep[j],Form("GE%iEN/D",det));
  }

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
  //initialize cumulative quantities (histograms)
  hNaI = new TH1F("hNaI","hNaI",3000,0,3000);
  h2D = new TH2F("h2D","h2D",3000,0,3000,3000,0,3000);
  h2DV = new TH2F("h2DV","h2DV",3000,0,3000,3000,0,3000);
  for (int j=0;j<2;j++){
    hEn[j] = new TH1F(Form("hEn%i",j),Form("hEn%i",j),6000,0,3000);
    hEnV[j] = new TH1F(Form("hEnV%i",j),Form("hEnV%i",j),6000,0,3000);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::fillPerEvent(double E[])
{
  for (int j=0;j<3;j++){
    enDep[j] = E[j];
  }
  // Find maximum energy deposited in a HPGe
  double maxE = enDep[0];
  if (enDep[1]>enDep[0]){
    maxE = enDep[1];
  }

  
  //accumulate statistic
  if (maxE>200){    // trigger
    //Fill histograms
    h2D->Fill(enDep[0],enDep[1]);
    if (enDep[2]<50){
      h2DV->Fill(enDep[0],enDep[1]);
    }
    for (int j=0;j<2;j++){
      hEn[j]->Fill(enDep[j]);
      if (enDep[2]<50){
        hEnV[j]->Fill(enDep[j]);
      }
    }
    hNaI->Fill(enDep[2]);
    //Fill tree
    tMC->Fill();
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  //Write histos to TFile
  hNaI->Write();
  for (int j=0; j<2; j++){
    hEn[j]->Write();
    hEnV[j]->Write();
  }
  //Write TTree
  tMC->Write("", TObject::kOverwrite);
  //Write and close file
  fMC->Write();
  fMC->Close();

  //print
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "   Data saved to file " << fileName << ".root"
     << "\n------------------------------------------------------------\n"
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
