//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
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

   	 void SetName(G4String);
     
     void BuildSample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot);
     void PrintSampleParameters();
     
  public:
  
     G4String GetName()					{return name;};
      
     const G4VPhysicalVolume* GetSample()   {return physSample;};
     const G4VPhysicalVolume* GetShell()    {return physShell;};
     const G4VPhysicalVolume* GetBag()      {return physBag;};

  private:

     G4String	 	    name;
     G4LogicalVolume*   logicWorld;
   	 G4ThreeVector		SamplePos;
   	 G4RotationMatrix*	SampleRot;
     
     G4Material*        sampleMaterial;
     G4Material*        shellMaterial;
     G4Material*        bagMaterial;
     G4Material*        defaultMaterial;
     
     G4double           samplePhi;
     G4double           sampleHeight;
     G4double           containerWallThick;
     G4double           containerOuterPhi;

     G4double           bagThick;

	 G4VSolid *sample;					//Nd disk
     G4LogicalVolume *logSample;		//pointer to the logical inner disk Nd
     G4VPhysicalVolume *physSample;	//pointer to the physical inner disk Nd

	 G4VSolid *shell;					//shell holding the sample
     G4LogicalVolume *logShell;			//pointer to the logical shell
     G4VPhysicalVolume *physShell;		//pointer to the physical shell

	 G4VSolid *bag;						//shell holding the sample
     G4LogicalVolume *logBag;			//pointer to the logical shell
     G4VPhysicalVolume *physBag;		//pointer to the physical shell

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

