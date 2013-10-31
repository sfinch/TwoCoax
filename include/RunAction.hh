//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/RunAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);
    
    void fillPerEvent(double[]); 
    void SetFileName(G4String val) {fileName = val;}
    G4String GetFileName()        {return fileName;}

  private:
    double enDep[3]; //energy deposited in {HPGe1, HPGe2, NaI}
    TH1F *hEn[2];    //HPGe energy spectra
    TH1F *hEnV[2];   //HPGe energy spectra with NaI anticoincidence
    TH1F *hNaI;      //NaI energy Spectra
    TH2F *h2D;       //2D coincidence spectra
    TH2F *h2DV;      //2D coincidence spectra with NaI cut
    TTree *tMC;
    TFile *fMC;

    G4String fileName;

    RunActionMessenger *runMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

