//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/DetectorMessenger.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:Detector(Det)
{ 
  TwoCoaxDir = new G4UIdirectory("/TwoCoax/");
  TwoCoaxDir->SetGuidance("UI commands");
  
  //directory
  detDir = new G4UIdirectory("/TwoCoax/det/");
  detDir->SetGuidance("detector control");
       
  // set the detector separation distance command
  DetectorDistanceCmd= new G4UIcmdWithADoubleAndUnit("/TwoCoax/det/setDetectorDistance",this);
  DetectorDistanceCmd->SetGuidance("Set the distance of the detector to the source.");
  DetectorDistanceCmd->SetParameterName("Size",false);
  DetectorDistanceCmd->SetRange("Size>0.");
  DetectorDistanceCmd->SetUnitCategory("Length");    
  DetectorDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // switch the two HPGes on/off
  for (int i=0; i<2; i++){
    G4String dir = "/TwoCoax/det/HPGe"+G4UIcommand::ConvertToString(i+1);
    G4String guid = "Tun on (default) or off HPGe"+G4UIcommand::ConvertToString(i+1);

    HPGeCmd[i] = new G4UIcmdWithAString(dir.c_str(),this);
    HPGeCmd[i]->SetGuidance(guid.c_str());
    HPGeCmd[i]->SetParameterName("choice",true);
    HPGeCmd[i]->SetDefaultValue("on");
    HPGeCmd[i]->SetCandidates("on off");
    HPGeCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  //switch the NaI detector on/off
  NaICmd = new G4UIcmdWithAString("/TwoCoax/det/NaI",this);
  NaICmd->SetGuidance("Turn the NaI detector on or off (default)");
  NaICmd->SetParameterName("choice",true);
  NaICmd->SetDefaultValue("off");
  NaICmd->SetCandidates("on off");
  NaICmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //switch the Zr sample on/off
  ZrCmd = new G4UIcmdWithAString("/TwoCoax/det/Zr",this);
  ZrCmd->SetGuidance("Turn the Zr sample on (default) or off");
  ZrCmd->SetParameterName("choice",true);
  ZrCmd->SetDefaultValue("on");
  ZrCmd->SetCandidates("on off");
  ZrCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //switch the Mo sample on/off
  MoCmd = new G4UIcmdWithAString("/TwoCoax/det/Mo",this);
  MoCmd->SetGuidance("Turn the Mo sample on or off (default)");
  MoCmd->SetParameterName("choice",true);
  MoCmd->SetDefaultValue("off");
  MoCmd->SetCandidates("on off");
  MoCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //switch the Nd sample on/off
  NdCmd = new G4UIcmdWithAString("/TwoCoax/det/Nd",this);
  NdCmd->SetGuidance("Turn the Nd sample on or off (default)");
  NdCmd->SetParameterName("choice",true);
  NdCmd->SetDefaultValue("off");
  NdCmd->SetCandidates("on off");
  NdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

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

  delete HPGeCmd[0];
  delete HPGeCmd[1];
  delete NaICmd;
  delete ZrCmd;
  delete MoCmd;
  delete NdCmd;

  delete UpdateCmd;
  delete detDir;
  delete TwoCoaxDir;  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //distance
  if (command == DetectorDistanceCmd){
    Detector->SetDetectorDistance(DetectorDistanceCmd->GetNewDoubleValue(newValue));
  }
  //flags to tun det components on/off
  for (int i=0; i<2; i++){
    if (command == HPGeCmd[i]){
      Detector->SetHPGeFlag(i, newValue);
    }
  }
  if (command == NaICmd){
    Detector->SetNaIFlag(newValue);
  }
  if (command == ZrCmd){
    Detector->SetZrFlag(newValue);
  }
  if (command == MoCmd){
    Detector->SetMoFlag(newValue);
  }
  if (command == NdCmd){
    Detector->SetNdFlag(newValue);
  }

  //update
  if (command == UpdateCmd){
    Detector->UpdateGeometry();
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
