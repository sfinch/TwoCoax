//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/ZrSample.hh
// Makes the enriched 96Zr sample
//
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
  
     G4String GetName()					 {return name;};
     G4double GetActive1Thick()          {return active1Thick;};
     G4double GetActive2Thick()          {return active2Thick;};
     G4double GetActive1OutR()           {return active1OutR;};
     G4double GetActive2InR()            {return active2InR;};
     G4double GetActive2OutR()           {return active2OutR;};
      
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
     
     G4double           sample1WallThickZ;
     G4double           sample1WallThickPhi;
     G4double           sample1Height;
     G4double           sample1OuterR;

     G4double           sample2WallThickZ;
     G4double           sample2WallThickPhi;
     G4double           sample2Height;
     G4double           sample2OuterR;

     G4double           active1Thick;
     G4double           active2Thick;
     G4double           active1OutR;
     G4double           active2InR;
     G4double           active2OutR;
     
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
    active1Thick = sample1Height - 2*sample1WallThickZ;
    active2Thick = sample2Height - 2*sample2WallThickZ;

    active1OutR = sample1OuterR - sample1WallThickPhi;
    active2InR  = sample1OuterR + sample2WallThickPhi;
    active2OutR = sample2OuterR - sample2WallThickPhi;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

