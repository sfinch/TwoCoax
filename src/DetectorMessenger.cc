//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(
                                           DetectorConstruction* Det)
:Detector(Det)
{ 
  TwoCoaxDir = new G4UIdirectory("/TwoCoax/");
  TwoCoaxDir->SetGuidance("UI commands");
  
  detDir = new G4UIdirectory("/TwoCoax/det/");
  detDir->SetGuidance("detector control");
       
  DetectorDistanceCmd= new G4UIcmdWithADoubleAndUnit("/TwoCoax/det/setDetectorDistance",this);
  DetectorDistanceCmd->SetGuidance("Set the distance of the detector to the source.");
  DetectorDistanceCmd->SetParameterName("Size",false);
  DetectorDistanceCmd->SetRange("Size>0.");
  DetectorDistanceCmd->SetUnitCategory("Length");    
  DetectorDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //update
  UpdateCmd = new G4UIcmdWithoutParameter("/TwoCoax/det/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete DetectorDistanceCmd; 

  delete UpdateCmd;
  delete detDir;
  delete TwoCoaxDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == DetectorDistanceCmd )
   { Detector->SetDetectorDistance(DetectorDistanceCmd->GetNewDoubleValue(newValue));}

  //update
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......