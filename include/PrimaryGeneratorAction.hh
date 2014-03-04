//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/PrimaryGeneratorAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "TF1.h"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
    virtual ~PrimaryGeneratorAction();
  
    // setters
    void SetRndmFlag(G4String val)   {rndmFlag = val;}
    void SetPositionR(G4double R)    {positionR = R;}
    void SetNumGamma(int num);
    void SetEnergy(int num, G4double En);
    void SetSpin(int num, int j);
    void SetEnSpin(int num, G4double En, int j);
  
    // function
    void GeneratePrimaries(G4Event*);
    G4ThreeVector randP();
    G4ThreeVector randMultipole(G4ThreeVector, int, int, int);

  private:
    double pi;

    // object properties
    G4ParticleGun*           particleGun;  //pointer a to G4  class
    DetectorConstruction*    Detector;     //pointer to the geometry
    G4double                 sampleWidth;  // width of the active sample
      
    PrimaryGeneratorMessenger* gunMessenger;   //messenger for this class
    G4String                   rndmFlag;       //flag for switching between gps and gun

    G4int          numGamma;       // max = 4
    G4double       positionR;
    G4double       energy[4];
    G4double       spin[4];
  
    TF1 *fPDF[6][6][6];

    G4GeneralParticleSource* particleSource;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetEnergy(int num, G4double En)
{ 
  if ((num>=0)&&(num<4)){
    energy[num] = En;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetSpin(int num, int j)
{ 
  if ((num>=0)&&(num<4)){
    spin[num] = j;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetEnSpin(int num, G4double En, int j)
{ 
  if ((num>=0)&&(num<4)){
    energy[num] = En;
    spin[num] = j;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetNumGamma(int num)
{
  if (num<1)
    num=1;
  else if (num>4)
    num = 4;
  
  numGamma = num;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


