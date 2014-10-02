//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/NatZrSample.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NatZrSample.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Transform3D.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NatZrSample::NatZrSample(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  sample1Thick    = 10.*mm;
  sample1RadIn    = 35.*mm;
  sample1RadOut   = 50.*mm;

  sample2Thick    = 110.*mm;
  sample2RadIn    = 50.*mm;
  sample2RadOut   = 60.*mm;

  ComputeNatZrSampleParameters();
  
  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();
  sampleMaterial = manager->FindOrBuildMaterial("G4_Zr");

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NatZrSample::~NatZrSample(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NatZrSample::DefineMaterials()
{ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NatZrSample::BuildSample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  SamplePos = G4ThreeVector(pos->x(),pos->y(),pos->z());

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*SampleRot);

  // complete the NatZr Sample parameters definition
  ComputeNatZrSampleParameters();
  
  // Sample1
  sample1 = new G4Tubs("sample1", //name
            sample1RadIn,  //inner radius
            sample1RadOut,  //outer radius
            sample1Thick/2., //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logSample1 = new G4LogicalVolume(sample1,
            sampleMaterial, //material
            "logSample1");

  physSample1 = new G4PVPlacement(rm,    //rotation
                    SamplePos,  //placement
                    logSample1,  //its logical volume
                    "physSample1",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  // Sample2
  sample2 = new G4Tubs("sample2", //name
            sample2RadIn,  //inner radius
            sample2RadOut,  //outer radius
            sample2Thick/2., //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logSample2 = new G4LogicalVolume(sample2,
            sampleMaterial, //material
            "logSample2");

  physSample2 = new G4PVPlacement(rm,    //rotation
                    SamplePos,  //placement
                    logSample2,  //its logical volume
                    "physSample2",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  PrintSampleParameters();     

  // Visualization attributes
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.5,0.5,0.5,1));
  atb->SetForceSolid(true);
  logSample1->SetVisAttributes(atb);}
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.5,0.5,0.5,1));
  atb->SetForceSolid(true);
  logSample2->SetVisAttributes(atb);}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NatZrSample::PrintSampleParameters(){
  
  G4double SampVol1 = sample1->GetCubicVolume();
  G4double SampVol2 = sample1->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
         << "\n---> Sample is " << sampleMaterial->GetName() << "\n" 
         << "---> Sample 1 \n"
         << sample1Thick/mm << "mm Z thickness \n" 
         << sample1RadIn/mm << "mm inner radius\n" 
         << sample1RadOut/mm << "mm outer radius\n" 
         << "--->Calculated quantities \n"
         << SampVol1/(cm*cm*cm) << " cm^3 sample 1 volume \n"
         << "---> Sample 2 \n"
         << sample2Thick/mm << "mm Z thickness \n" 
         << sample2RadIn/mm << "mm inner radius\n" 
         << sample2RadOut/mm << "mm outer radius\n" 
         << "--->Calculated quantities \n"
         << SampVol2/(cm*cm*cm) << " cm^3 sample 2 volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NatZrSample::SetName(G4String giveName){
    name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
