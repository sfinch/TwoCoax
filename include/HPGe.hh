//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/HPGe.hh
// Produces a p-type HPGe 
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HPGe_h
#define HPGe_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class HPGeMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HPGe
{
  public:
  
    HPGe(G4String name);
    ~HPGe();

  public:

    // Setters
    void SetName(G4String);

    void SetCrystalMaterial (G4String);     
    void SetWindowMaterial (G4String);     
    void SetWallMaterial (G4String);     

    void SetCrystalHalfLength(G4double);
    void SetCrystalRad(G4double);
    void SetCrystalEndRad(G4double);
    void SetHoleRad(G4double);
    void SetHoleDepth(G4double);
    void SetDeadLayerThick(G4double);

    void SetShellHalfLength(G4double);
    void SetEndGap(G4double);
    void SetWindowThickness(G4double);
    void SetWallThickness(G4double);

    // Functions
    void BuildHPGe(G4LogicalVolume *logWorld,
                   G4ThreeVector *pos,
                   G4RotationMatrix *rot);
    void PrintHPGeParameters();
     
  public:
  
    // Getters
    G4String GetName()                 {return name;};

    G4double GetCrystalHalfLength()    {return crystalHalfLength;};
    G4double GetCrystalRad()           {return crystalRad;}; 
    G4double GetCrystalEndRad()        {return crystalEndRad;};
    G4double GetHoleRad()              {return holeRad;};
    G4double GetHoleDepthRad()         {return holeDepth;};
    G4double GetDeadLayerThick()       {return deadLayerThick;};
     
    G4double GetShellHalfLength()      {return shellHalfLength;}; 
    G4double GetEndGap()               {return endGap;};
    G4double GetWindowThickness()      {return windowThickness;};
    G4double GetWallThickness()        {return wallThickness;};
     
    G4Material* GetCrystalMaterial()   {return crystalMaterial;};
    G4Material* GetWindowMaterial()    {return windowMaterial;};
    G4Material* GetWallMaterial()      {return wallMaterial;};
    
    const G4VPhysicalVolume* GetCrystal()   {return physiCrystal;};
                 
  private:

    // Detector properties
    G4String           name;
    G4LogicalVolume*   logicWorld;
    G4ThreeVector*     DetPos;
    G4RotationMatrix*  DetRot;
    
    G4Material*        crystalMaterial;
    G4Material*        windowMaterial;
    G4Material*        wallMaterial;
     
    G4double           crystalHalfLength;
    G4double           crystalRad;
    G4double           crystalEndRad; // radius of curvature on detector
    G4double           holeRad;
    G4double           holeDepth;
    G4double           deadLayerThick;

    G4double           shellHalfLength; // outer shell
    G4double           endGap;          // gap between detector face and faceplate
    G4double           windowThickness;
    G4double           wallThickness;

    G4double           theta;           // angle subtended by detector face
    
    G4VSolid *activeCrystal;
    G4VSolid *deadLayer;
    G4VSolid *hole;

    G4LogicalVolume *logCrystal;        //pointer to the logical crystal 
    G4LogicalVolume *logDeadLayer;      //pointer to the logical dead layer
    G4VPhysicalVolume *physiCrystal;    //pointer to the physical crystal 
    G4VPhysicalVolume *physiDeadLayer;  //pointer to the physical dead layer

    G4LogicalVolume *logShell;          //pointer to the logical shell 
    G4VPhysicalVolume *physiShell;      //pointer to the physical shell 
    
    G4LogicalVolume *logWindow;         //pointer to the logical shell 
    G4VPhysicalVolume *physiWindow;     //pointer to the physical shell 

    HPGeMessenger* hpgeMessenger;
     
  private:
    
    void ComputeHPGeParameters();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void HPGe::ComputeHPGeParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

