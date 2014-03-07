//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/PrimaryGeneratorMessenger.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  //Directory
  gunDir = new G4UIdirectory("/TwoCoax/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  //switch between GPS and custom gun enent generator
  RndmCmd = new G4UIcmdWithAString("/TwoCoax/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot the using GPS (on - defualt) or custom event generator (off)");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //change the number of gammas
  numGammaCmd = new G4UIcmdWithAnInteger("/TwoCoax/gun/numGamma",this);
  numGammaCmd->SetGuidance("Set the number of Gammas shot");
  numGammaCmd->SetParameterName("Number",false);
  numGammaCmd->SetRange("Number>0&&Number<5");
  numGammaCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //change the r position of the source
  PositionRCmd = new G4UIcmdWithADoubleAndUnit("/TwoCoax/gun/positionR",this);
  PositionRCmd->SetGuidance("Set the position of the source along R");
  PositionRCmd->SetParameterName("Size",false);
  PositionRCmd->SetUnitCategory("Length");
  PositionRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // change the energy of the gamma (up to 4)
  for (int i=0; i<4;i++){
    G4String dir = "/TwoCoax/gun/energy"+G4UIcommand::ConvertToString(i+1);
    G4String guid = "Set energy of gamma "+G4UIcommand::ConvertToString(i+1);

    G4String dir2 = "/TwoCoax/gun/spin"+G4UIcommand::ConvertToString(i+1);
    G4String guid2 = "Set spin of gamma "+G4UIcommand::ConvertToString(i+1);

    energyCmd[i] = new G4UIcmdWithADoubleAndUnit(dir.c_str(),this);
    energyCmd[i]->SetGuidance(guid.c_str());
    energyCmd[i]->SetParameterName("Size",false);
    energyCmd[i]->SetRange("Size>0.");
    energyCmd[i]->SetUnitCategory("Energy");
    energyCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);

    spinCmd[i] = new G4UIcmdWithAnInteger(dir.c_str(),this);
    spinCmd[i]->SetGuidance(guid.c_str());
    spinCmd[i]->SetParameterName("Number",false);
    spinCmd[i]->SetRange("Number>=0&&Number<6");
    spinCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  //print parameters
  PrintCmd = new G4UIcmdWithoutParameter("/TwoCoax/gun/print",this);
  PrintCmd->SetGuidance("Print gun parameters");
  PrintCmd->AvailableForStates(G4State_Idle);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete numGammaCmd;
  delete PositionRCmd;
  for (int i=0; i<4;i++){
    delete energyCmd[i];
    delete spinCmd[i];
  }
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == RndmCmd){
    Action->SetRndmFlag(newValue);
  }
  if (command == numGammaCmd){
    Action->SetNumGamma(numGammaCmd->GetNewIntValue(newValue));
  }
  if (command == PositionRCmd){
    Action->SetPositionR(PositionRCmd->GetNewDoubleValue(newValue));
  }
  for (int i=0; i<4;i++){
    if (command == energyCmd[i]){
      Action->SetEnergy(i, energyCmd[i]->GetNewDoubleValue(newValue));
      Action->SetSpin(i, spinCmd[i]->GetNewIntValue(newValue));
    }
  }
  if (command == PrintCmd){
    Action->PrintGunParameters();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
