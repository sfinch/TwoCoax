//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DetectorMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* Detector;
    
    //Directories
    G4UIdirectory*             TwoCoaxDir;
    G4UIdirectory*             detDir;

    //Set separation distance between detectors
    G4UIcmdWithADoubleAndUnit* DetectorDistanceCmd;

    G4UIcmdWithAString*        HPGeCmd[2];
    G4UIcmdWithAString*        NaICmd;
    G4UIcmdWithAString*        ZrCmd;
    G4UIcmdWithAString*        MoCmd;
    G4UIcmdWithAString*        NdCmd;

    //update
    G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

