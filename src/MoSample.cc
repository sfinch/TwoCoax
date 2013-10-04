//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "MoSample.hh"

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

MoSample::MoSample(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  sampleThick 	 = 1.*cm;
  sampleRad      = (10./2.)*cm;

  ComputeMoSampleParameters();
  
  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();
  sampleMaterial = manager->FindOrBuildMaterial("G4_Mo");

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MoSample::~MoSample(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MoSample::DefineMaterials()
{ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MoSample::BuildSample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  SamplePos = G4ThreeVector(pos->x(),pos->y(),pos->z());

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*SampleRot);

  // complete the Mo Sample parameters definition
  ComputeMoSampleParameters();
  
  // Sample 
  sample = new G4Tubs("sample", //name
  			0,	//inner radius
			sampleRad, 	//outer radius
			sampleThick/2., //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  
  logSample = new G4LogicalVolume(sample,
  			sampleMaterial,	//material
			"logSample");

  physSample = new G4PVPlacement(rm,	//rotation
					SamplePos,	//placement
					logSample,	//its logical volume
					"physSample",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number

  PrintSampleParameters();     

  // Visualization attributes
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.5,0.5,0.5,1));
  atb->SetForceSolid(true);
  logSample->SetVisAttributes(atb);}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MoSample::PrintSampleParameters(){
	
  
  G4double SampVol = sample->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
		 << "\n---> Sample is " << sampleMaterial->GetName() << "\n" 
		 << "---> Sample 1 \n"
         << sampleThick/mm << "mm Z thickness \n" 
         << sampleRad/mm << "mm height\n" 
		 << "--->Calculated quantities \n"
		 << SampVol/(cm*cm*cm) << " cm^3 sample 1 volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MoSample::SetName(G4String giveName){
	name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
