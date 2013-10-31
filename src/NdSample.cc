//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/NdSample.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NdSample.hh"

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

NdSample::NdSample(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  samplePhi       = 5.72*cm;
  sampleHeight    = 0.78*cm;
  containerWallThick = 1.59*mm;
  containerOuterPhi  = 3.*2.54*cm;

  bagThick   = 1.*mm;
  
  ComputeNdSampleParameters();
  
  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();
  shellMaterial = manager->FindOrBuildMaterial("G4_POLYCARBONATE");
  bagMaterial = manager->FindOrBuildMaterial("G4_POLYVINYLIDENE_FLUORIDE");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NdSample::~NdSample(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NdSample::DefineMaterials()
{ 
 
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4int iz, n;                 //iz=number of protons  in an isotope; 
                               // n=number of nucleons in an isotope;

  G4int ncomponents, natoms;
  G4double abundance;

  // define Elements
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);

  // define isotopes by relative abundance 
  G4Isotope* Nd142 = new G4Isotope("Nd142", iz=60, n=142, a=142.*g/mole);
  G4Isotope* Nd143 = new G4Isotope("Nd143", iz=60, n=143, a=143.*g/mole);
  G4Isotope* Nd144 = new G4Isotope("Nd144", iz=60, n=144, a=144.*g/mole);
  G4Isotope* Nd145 = new G4Isotope("Nd145", iz=60, n=145, a=145.*g/mole);
  G4Isotope* Nd146 = new G4Isotope("Nd146", iz=60, n=146, a=146.*g/mole);
  G4Isotope* Nd148 = new G4Isotope("Nd148", iz=60, n=148, a=148.*g/mole);
  G4Isotope* Nd150 = new G4Isotope("Nd150", iz=60, n=150, a=150.*g/mole);

  G4Element* sample  = new G4Element("Nd Sample",symbol="Nd1",ncomponents=7);
  sample->AddIsotope(Nd142, abundance=1.26*perCent);
  sample->AddIsotope(Nd143, abundance=0.82*perCent);
  sample->AddIsotope(Nd144, abundance=1.34*perCent);
  sample->AddIsotope(Nd145, abundance=0.74*perCent);
  sample->AddIsotope(Nd146, abundance=1.31*perCent);
  sample->AddIsotope(Nd148, abundance=0.94*perCent);
  sample->AddIsotope(Nd150, abundance=93.60*perCent);

  // define chemical molecule
  density = 50.*g/(3.14159*samplePhi*samplePhi*sampleHeight);
  sampleMaterial = new G4Material("NdO2_sample", density, ncomponents=2);
  sampleMaterial->AddElement(sample, natoms=2);
  sampleMaterial->AddElement(O, natoms=3);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NdSample::BuildSample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  SamplePos = G4ThreeVector(pos->x(),pos->y(),pos->z());

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*SampleRot);

  // complete the NdSample parameters definition
  ComputeNdSampleParameters();
  
  // Sample 
  sample = new G4Tubs("sample", //name
            0,  //inner radius
            samplePhi,  //outer radius
            sampleHeight*.5, //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logSample = new G4LogicalVolume(sample,
            sampleMaterial, //material
            "logSample");

  physSample = new G4PVPlacement(rm,    //rotation
                    SamplePos,  //placement
                    logSample,  //its logical volume
                    "physSample",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  //shell
  G4VSolid *shell1 = new G4Tubs("shell1", //name
            0,  //inner radius
            containerOuterPhi,  //outer radius
            containerWallThick+(sampleHeight*.5), //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  shell = new G4SubtractionSolid("shell",                   //subtract sample2
            shell1, sample,0,G4ThreeVector(0,0,0));

  logShell = new G4LogicalVolume(shell,
            shellMaterial,  //material
            "logShell");

  physShell = new G4PVPlacement(rm, //rotation
                    SamplePos,  //placement
                    logShell,   //its logical volume
                    "physShell",    //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number
  
  PrintSampleParameters();     

  // Visualization attributes
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.1,0.1,0.1,1));
  atb->SetForceSolid(true);
  logSample->SetVisAttributes(atb);}
  
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.3));
  atb->SetForceSolid(true);
  logShell->SetVisAttributes(atb);}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NdSample::PrintSampleParameters(){
  
  G4double SampVol = sample->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
         << "\n---> Shell is " << shellMaterial->GetName() << "\n" 
         << "---> Sample \n"
         << sampleHeight/mm << "mm height\n" 
         << samplePhi/mm << "mm phi \n" 
         << containerWallThick/mm << "mm container wall thickness \n" 
         << containerOuterPhi/mm << "mm container outer radius\n" 
         << "---> Bag \n"
         << bagThick/mm << "mm Z thickness \n" 
         << "--->Calculated quantities \n"
         << SampVol/(cm*cm*cm) << " cm^3 sample volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NdSample::SetName(G4String giveName){
    name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
