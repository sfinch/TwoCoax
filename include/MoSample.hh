//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef MoSample_h
#define MoSample_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MoSample 
{
  public:
  
    MoSample(G4String name);
   ~MoSample();

  public:

   	 void SetName(G4String);
     
     void BuildSample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot);
     void PrintSampleParameters();
     
  public:
  
     G4String GetName()					{return name;};
      
     const G4VPhysicalVolume* GetSample()   {return physSample;};

  private:

     G4String	 	    name;
     G4LogicalVolume*   logicWorld;
   	 G4ThreeVector		SamplePos;
   	 G4RotationMatrix*	SampleRot;
     
     G4Material*        sampleMaterial;
     
     G4double           sampleThick;
     G4double           sampleRad;

     
	 G4VSolid *sample;					//sample
     G4LogicalVolume *logSample;		//pointer to the logical sample
     G4VPhysicalVolume *physSample;		//pointer to the physical sample

  private:
    
     void ComputeMoSampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MoSample::ComputeMoSampleParameters()
{
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

