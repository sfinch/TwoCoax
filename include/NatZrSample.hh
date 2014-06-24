//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/NatZrSample.hh
// Creates a natZr metal sample
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef NatZrSample_h
#define NatZrSample_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NatZrSample 
{
  public:
  
    NatZrSample(G4String name);
    ~NatZrSample();

  public:

    // setters
    void SetName(G4String);
     
    void BuildSample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot);
    void PrintSampleParameters();
     
  public:
  
    G4String GetName()                 {return name;};

    const G4VPhysicalVolume* GetSample()   {return physSample;};

  private:

    //object properties
    G4String           name;
    G4LogicalVolume*   logicWorld;
    G4ThreeVector      SamplePos;
    G4RotationMatrix*  SampleRot;
    
    G4Material*        sampleMaterial;
    
    G4double           sample1Thick;
    G4double           sample1RadIn;
    G4double           sample1RadOut;

    G4double           sample2Thick;
    G4double           sample2RadIn;
    G4double           sample2RadOut;

    
    G4VSolid *sample;                  //sample
    G4LogicalVolume *logSample;        //pointer to the logical sample
    G4VPhysicalVolume *physSample;     //pointer to the physical sample

  private:
    
     void ComputeNatZrSampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NatZrSample::ComputeNatZrSampleParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

