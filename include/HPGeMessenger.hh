//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HPGeMessenger_h
#define HPGeMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class HPGe;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HPGeMessenger: public G4UImessenger
{
  public:
    HPGeMessenger(HPGe* );
   ~HPGeMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    HPGe* Detector;
    
    G4UIdirectory*             HPGeDir;
	//materials
    G4UIcmdWithAString*        CrystalMaterCmd;
    G4UIcmdWithAString*        WindowMaterCmd;
    G4UIcmdWithAString*        WallMaterCmd;
	//Crystal Dimensions
    G4UIcmdWithADoubleAndUnit* CrystalHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* CrystalRadCmd;
    G4UIcmdWithADoubleAndUnit* CrystalEndRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleDepthCmd;
    G4UIcmdWithADoubleAndUnit* DeadLayerCmd;
	//Shell dimensions
    G4UIcmdWithADoubleAndUnit* ShellHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* EndGapCmd;
    G4UIcmdWithADoubleAndUnit* WindowThickCmd;
    G4UIcmdWithADoubleAndUnit* WallThickCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

