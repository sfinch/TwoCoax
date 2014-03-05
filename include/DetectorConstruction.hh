//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DetectorConstruction.hh
// Makes the detector, inluding HPGes, sources, and the NaI veto
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "HPGe.hh"
#include "NaIAnnulus.hh"
#include "ZrSample.hh"
#include "MoSample.hh"
#include "NdSample.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class HPGe;
class NaIAnnulus;
class ZrSample;
class MoSample;
class NdSample;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
    ~DetectorConstruction();

  public:
     
    G4VPhysicalVolume* Construct();
    void UpdateGeometry();
     
  public:
  
    void PrintTwoCoaxParameters(); 

    // Setters
    void SetDetectorDistance (G4double);     

    void SetHPGeFlag(int det, G4String val);
    void SetNaIFlag(G4String val)      {NaIFlag = val;};
    void SetZrFlag(G4String val)       {ZrFlag  = val;};
    void SetMoFlag(G4String val)       {MoFlag  = val;};
    void SetNdFlag(G4String val)       {NdFlag  = val;};

    // Getters
    G4double GetWorldSizeX()           {return WorldSizeX;}; 
    G4double GetWorldSizeYZ()          {return WorldSizeYZ;};
    G4double GetDetectorDistance()     {return detectorDistance;};

    G4String GetHPGeFlag(int det);
    G4String GetNaIFlag()              {return NaIFlag;};
    G4String GetZrFlag()               {return ZrFlag;};
    G4String GetMoFlag()               {return MoFlag;};
    G4String GetNdFlag()               {return NdFlag;};

    HPGe* GetHPGe(G4int det);
    NaIAnnulus* GetNaIAnnulus()        {return NaIDet;};
    ZrSample* GetZrSample()            {return ZrSamp;};
    MoSample* GetMoSample()            {return MoSamp;};
    NdSample* GetNdSample()            {return NdSamp;};

    const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           

  private:
     
    // Objects placed in the detector
    G4double           detectorDistance;
    HPGe*              HPGeDet[2];
    NaIAnnulus*        NaIDet;
    ZrSample*          ZrSamp;
    MoSample*          MoSamp;
    NdSample*          NdSamp;

    // flags to build det objects
    G4String           HPGeFlag[2];
    G4String           NaIFlag;
    G4String           ZrFlag;
    G4String           MoFlag;
    G4String           NdFlag;

    // World properties
    G4Material*        defaultMaterial;
    G4double           WorldSizeYZ;
    G4double           WorldSizeX;
            
    G4Box*             solidWorld;    //pointer to the solid World 
    G4LogicalVolume*   logicWorld;    //pointer to the logical World
    G4VPhysicalVolume* physiWorld;    //pointer to the physical World

    // Messenger
    DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
    void DefineMaterials();
    void ComputeTwoCoaxParameters();
    G4VPhysicalVolume* ConstructTwoCoax();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::ComputeTwoCoaxParameters()
{
  G4double maxRad = NaIDet->GetCrystalOutRad() + NaIDet->GetWallOutThickness();
  G4double maxLength = NaIDet->GetCrystalHalfLength();

  WorldSizeX  = 2.2*maxLength;
  WorldSizeYZ = 2.2*maxRad;

  //WorldSizeX  = 55.*cm;
  //WorldSizeYZ = 40.*cm;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline HPGe* DetectorConstruction::GetHPGe(G4int det)
{
  if (det==0 || det==1){ 
    return HPGeDet[det];
  }
  else
    return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4String DetectorConstruction::GetHPGeFlag(G4int det)
{
  if (det==0 || det==1){ 
    return HPGeFlag[det];
  }
  else
    return "";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::SetHPGeFlag(int det, G4String val)
{
  if (det==0 || det==1){ 
    HPGeFlag[det] = val;
  }
}

#endif

