//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/NdSample.hh
// makes the enriched 150Nd sample
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef NdSample_h
#define NdSample_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NdSample 
{
  public:
  
    NdSample(G4String name);
    ~NdSample();

  public:

    // setters
    void SetName(G4String);
     
    // functions
    void BuildSample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot);
    void PrintSampleParameters();
     
  public:
  
    // getters
    G4String GetName()                     {return name;};
     
    const G4VPhysicalVolume* GetSample()   {return physSample;};
    const G4VPhysicalVolume* GetShell()    {return physShell;};
    const G4VPhysicalVolume* GetBag()      {return physBag;};

  private:

    // obect properties
    G4String           name;
    G4LogicalVolume*   logicWorld;
    G4ThreeVector      SamplePos;
    G4RotationMatrix*  SampleRot;
    
    G4Material*        sampleMaterial;
    G4Material*        shellMaterial;
    G4Material*        bagMaterial;
    
    G4double           samplePhi;          //diameter of sample
    G4double           sampleHeight;       //height of sample
    G4double           containerWallThick; //thickness of sample holder walls
    G4double           containerOuterPhi;  //diameter of sample holder

    G4double           bagThick;

    G4VSolid *sample;                  //Nd disk
    G4LogicalVolume *logSample;        //pointer to the logical inner disk Nd
    G4VPhysicalVolume *physSample;     //pointer to the physical inner disk Nd

    G4VSolid *shell;                   //shell holding the sample
    G4LogicalVolume *logShell;         //pointer to the logical shell
    G4VPhysicalVolume *physShell;      //pointer to the physical shell

    G4VSolid *bag;                     //shell holding the sample
    G4LogicalVolume *logBag;           //pointer to the logical shell
    G4VPhysicalVolume *physBag;        //pointer to the physical shell

  private:
    
    void ComputeNdSampleParameters();
    void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NdSample::ComputeNdSampleParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

