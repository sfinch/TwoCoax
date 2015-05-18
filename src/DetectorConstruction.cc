//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
//
// src/DetectorConstruction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4RotationMatrix.hh"

#include "G4Box.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
DetectorConstruction::DetectorConstruction()
:defaultMaterial(0),solidWorld(0),logicWorld(0),physiWorld(0)
{
  //detectorDistance = (1.1/2.)*cm; //Mo detector distance
  detectorDistance = (1.022/2)*cm; //Zr detector distance
  //detectorDistance = (24.9)*cm; //source detector distance

  HPGeFlag[0] = "on";
  HPGeFlag[1] = "on";
  NaIFlag = "off";
  ZrFlag = "off";
  MoFlag = "off";
  NdFlag = "off";
  NatZrFlag = "off";

  // materials
  DefineMaterials();

  //initialize detectors and samples
  HPGeDet[0] = new HPGe("HPGe1");
  HPGeDet[1] = new HPGe("HPGe2");

  NaIDet = new NaIAnnulus("NaI");

  ZrSamp = new ZrSample("Zr");
  MoSamp = new MoSample("Mo");
  NdSamp = new NdSample("Nd");
  NatZrSamp = new NatZrSample("NatZr");
  
  // Calculate the world size
  ComputeTwoCoaxParameters();
  
  // create commands for interactive definition 
  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){ 
    delete detectorMessenger;

    delete HPGeDet[0];
    delete HPGeDet[1];
    delete NaIDet;
    delete MoSamp;
    delete ZrSamp;
    delete NdSamp;
    delete NatZrSamp;

    delete defaultMaterial;
    delete solidWorld;
    delete logicWorld;
    delete physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){
  return ConstructTwoCoax();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){ 
 
    G4double a, z, density;      //z=mean number of protons;  
                                 //a=mass of a mole;

    // examples of vacuum
    G4Material* Vacuum = new G4Material("Galactic", z=1., a=1.01*g/mole, 
               density=universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
    //default materials of the World
    defaultMaterial  = Vacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructTwoCoax(){

  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the TwoCoax parameters definition
  ComputeTwoCoaxParameters();
   
  // World
  solidWorld = new G4Box("World",               //its name
                   WorldSizeX/2,WorldSizeYZ/2,WorldSizeYZ/2);   //its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,      //its solid
                                   defaultMaterial, //its material
                                   "World");        //its name
                                   
  physiWorld = new G4PVPlacement(0,         //no rotation
                                 G4ThreeVector(),   //at (0,0,0)
                                 logicWorld,        //its logical volume                 
                                 "World",       //its name
                                 0,         //its mother  volume
                                 false,         //no boolean operation
                                 0);            //copy number

  // HPGe
  // HPGe placement
  G4ThreeVector *DetPos[2];
  G4RotationMatrix *DetRot[2];
  DetPos[0] = new G4ThreeVector(-detectorDistance,0,0);
  DetPos[1] = new G4ThreeVector(detectorDistance,0,0);
  DetRot[0] = new G4RotationMatrix();
  DetRot[0]->rotateX(180*deg);
  DetRot[1] = new G4RotationMatrix();

  // build HPGes
  if (HPGeFlag[1]=="on"){
    HPGeDet[1]->BuildHPGe(logicWorld, DetPos[1], DetRot[1]);
  }
  if (HPGeFlag[0]=="on"){
    HPGeDet[0]->BuildHPGe(logicWorld, DetPos[0], DetRot[0]);
  }

  // NaI Annulus
  //placement
  G4ThreeVector *NaIPos = new G4ThreeVector(0,0,0);
  G4RotationMatrix *NaIRot = new G4RotationMatrix();
  //build
  if (NaIFlag=="on"){
    NaIDet->BuildNaIAnnulus(logicWorld, NaIPos, NaIRot);
  }

  //Sample
  //placement
  G4ThreeVector *sampPos = new G4ThreeVector(0,0,0);
  G4RotationMatrix *sampRot = new G4RotationMatrix();

  //build sample
  if (ZrFlag=="on"){
    ZrSamp->BuildSample(logicWorld, sampPos, sampRot);
  }
  else if (MoFlag=="on"){
    MoSamp->BuildSample(logicWorld, sampPos, sampRot);
  }
  else if (NdFlag=="on"){
    NdSamp->BuildSample(logicWorld, sampPos, sampRot);
  }
  if (NatZrFlag=="on"){
    NatZrSamp->BuildSample(logicWorld, sampPos, sampRot);
  }

  PrintTwoCoaxParameters();     
  
  // Visualization attributes
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  //always return physical world
  return physiWorld;    //always return the physical World
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintTwoCoaxParameters(){
  G4cout << "\n------------------------------------------------------------"
         << "\n---> Detector Distance  " << detectorDistance/cm << "cm\n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorDistance(G4double val){
  detectorDistance = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructTwoCoax());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
