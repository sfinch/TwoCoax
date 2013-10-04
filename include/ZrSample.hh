//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ZrSample_h
#define ZrSample_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ZrSample 
{
  public:
  
    ZrSample(G4String name);
   ~ZrSample();

  public:

   	 void SetName(G4String);
     
     void BuildSample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot);
     void PrintSampleParameters();
     
  public:
  
     G4String GetName()					{return name;};
      
     const G4VPhysicalVolume* GetInnerSample()   {return physSample1;};
     const G4VPhysicalVolume* GetOuterSample()   {return physSample2;};
     const G4VPhysicalVolume* GetShell()  		 {return physShell;};

  private:

     G4String	 	    name;
     G4LogicalVolume*   logicWorld;
   	 G4ThreeVector		SamplePos;
   	 G4RotationMatrix*	SampleRot;
     
     G4Material*        sampleMaterial1;
     G4Material*        sampleMaterial2;
     G4Material*        shellMaterial;
     G4Material*        defaultMaterial;
     
     G4double           sample1ThickZ;
     G4double           sample1ThickPhi;
     G4double           sample1Height;
     G4double           sample1OuterPhi;

     G4double           sample2ThickZ;
     G4double           sample2ThickPhi;
     G4double           sample2Height;
     G4double           sample2OuterPhi;
     
	 G4VSolid *sample1;					//Inner Zr disk
     G4LogicalVolume *logSample1;		//pointer to the logical inner disk Zr
     G4VPhysicalVolume *physSample1;	//pointer to the physical inner disk Zr

	 G4VSolid *sample2;					//Outer Zr annulus
     G4LogicalVolume *logSample2;		//pointer to the logical outer annulus Zr
     G4VPhysicalVolume *physSample2;	//pointer to the physical outer annulus Zr

	 G4VSolid *shell;					//shell holding the sample
     G4LogicalVolume *logShell;			//pointer to the logical shell
     G4VPhysicalVolume *physShell;		//pointer to the physical shell

  private:
    
     void ComputeZrSampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void ZrSample::ComputeZrSampleParameters()
{
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

