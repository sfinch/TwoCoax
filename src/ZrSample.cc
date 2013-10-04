//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ZrSample.hh"

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

ZrSample::ZrSample(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  sample1ThickZ	  = 1.41*mm;
  sample1ThickPhi = 1.44*mm;
  sample1Height   = 10.22*mm;
  sample1OuterPhi = 0.5*31.75*mm;

  sample2ThickZ   = 1.57*mm;
  sample2ThickPhi = 1.31*mm;
  sample2Height   = 10.07*mm;
  sample2OuterPhi = 0.5*69.95*mm;
  
  ComputeZrSampleParameters();
  
  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();
  shellMaterial = manager->FindOrBuildMaterial("G4_POLYCARBONATE");

  //sampleMaterial1 = manager->FindOrBuildMaterial("G4_Zr");
  //sampleMaterial2 = manager->FindOrBuildMaterial("G4_Zr");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ZrSample::~ZrSample(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ZrSample::DefineMaterials()
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
  G4Isotope* Zr90 = new G4Isotope("Zr90", iz=40, n=90, a=90.*g/mole);
  G4Isotope* Zr91 = new G4Isotope("Zr91", iz=40, n=91, a=91.*g/mole);
  G4Isotope* Zr92 = new G4Isotope("Zr92", iz=40, n=92, a=92.*g/mole);
  G4Isotope* Zr94 = new G4Isotope("Zr94", iz=40, n=94, a=94.*g/mole);
  G4Isotope* Zr96 = new G4Isotope("Zr96", iz=40, n=96, a=96.*g/mole);

  G4Element* sample1  = new G4Element("Zr Sample 1",symbol="Zr1",ncomponents=5);
  sample1->AddIsotope(Zr90, abundance=3.95*perCent);
  sample1->AddIsotope(Zr91, abundance=0.88*perCent);
  sample1->AddIsotope(Zr92, abundance=1.39*perCent);
  sample1->AddIsotope(Zr94, abundance=2.38*perCent);
  sample1->AddIsotope(Zr96, abundance=91.39*perCent);

  G4Element* sample2  = new G4Element("Zr Sample 2",symbol="Zr2",ncomponents=5);
  sample2->AddIsotope(Zr90, abundance=12.21*perCent);
  sample2->AddIsotope(Zr91, abundance=2.3*perCent);
  sample2->AddIsotope(Zr92, abundance=16.25*perCent);
  sample2->AddIsotope(Zr94, abundance=5.07*perCent);
  sample2->AddIsotope(Zr96, abundance=64.18*perCent);

  // define chemical molecule
  density = 7.2832/4.84412*g/cm3;
  sampleMaterial1 = new G4Material("ZrO2_sample1", density, ncomponents=2);
  sampleMaterial1 ->AddElement(sample1, natoms=1);
  sampleMaterial1 ->AddElement(O, natoms=2);

  density = 26.9685/18.2445*g/cm3;
  sampleMaterial2 = new G4Material("ZrO2_sample2", density, ncomponents=2);
  sampleMaterial2 ->AddElement(sample2, natoms=1);
  sampleMaterial2 ->AddElement(O, natoms=2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ZrSample::BuildSample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  SamplePos = G4ThreeVector(pos->x(),pos->y(),pos->z());

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*SampleRot);

  // complete the ZrSample parameters definition
  ComputeZrSampleParameters();
  
  // Sample 1
  sample1 = new G4Tubs("sample1", //name
  			0,	//inner radius
			sample1OuterPhi-sample1ThickPhi, 	//outer radius
			(sample1Height*.5)-sample1ThickZ, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  
  logSample1 = new G4LogicalVolume(sample1,
  			sampleMaterial1,	//material
			"logSample1");

  physSample1 = new G4PVPlacement(rm,	//rotation
					SamplePos,	//placement
					logSample1,	//its logical volume
					"physSample1",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number

  // Sample 2
  sample2 = new G4Tubs("sample2", //name
  			sample1OuterPhi+sample2ThickPhi,	//inner radius
			sample2OuterPhi-sample2ThickPhi, 	//outer radius
			(sample2Height*.5)-sample2ThickZ, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  
  logSample2 = new G4LogicalVolume(sample2,
  			sampleMaterial2,	//material
			"logSample2");

  physSample2 = new G4PVPlacement(rm,	//rotation
					SamplePos,	//placement
					logSample2,	//its logical volume
					"physSample2",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number

  //shell
  G4VSolid *shell1 = new G4Tubs("shell1", //name
  			0,	//inner radius
			sample1OuterPhi, 	//outer radius
			sample1Height*.5, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *shell2 = new G4Tubs("shell2", //name
  			0,	//inner radius
			sample2OuterPhi, 	//outer radius
			sample2Height*.5, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *shellSum = new G4UnionSolid("shellSum",			//add the two shells
  			shell1, shell2,0,G4ThreeVector(0,0,0));
  G4VSolid *shellSum1 = new G4SubtractionSolid("shellSum1",	//subtract sample1
  			shellSum, sample1,0,G4ThreeVector(0,0,0));
  shell = new G4SubtractionSolid("shell",					//subtract sample2
  			shellSum1, sample2,0,G4ThreeVector(0,0,0));

  logShell = new G4LogicalVolume(shell,
  			shellMaterial,	//material
			"logShell");

  physShell = new G4PVPlacement(rm,	//rotation
					SamplePos,	//placement
					logShell,	//its logical volume
					"physShell",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  
  PrintSampleParameters();     

  // Visualization attributes
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.1,0.1,0.1,1));
  atb->SetForceSolid(true);
  logSample1->SetVisAttributes(atb);
  logSample2->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,1.0,0.0,0.3));
  //atb->SetForceWireframe(true);
  atb->SetForceSolid(true);
  logShell->SetVisAttributes(atb);}
  

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ZrSample::PrintSampleParameters(){
	
  
  G4double Samp1Vol = sample1->GetCubicVolume();
  G4double Samp2Vol = sample2->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
		 << "\n---> Shell is " << shellMaterial->GetName() << "\n" 
		 << "---> Sample 1 \n"
         << sample1ThickZ/mm << "mm Z thickness \n" 
         << sample1ThickPhi/mm << "mm phi thickness \n" 
         << sample1Height/mm << "mm height\n" 
         << sample1OuterPhi/mm << "mm outer radius\n" 
		 << "---> Sample 1 \n"
         << sample2ThickZ/mm << "mm Z thickness \n" 
         << sample2ThickPhi/mm << "mm phi thickness \n" 
         << sample2Height/mm << "mm height\n" 
         << sample2OuterPhi/mm << "mm outer radius\n" 
		 << "--->Calculated quantities \n"
		 << Samp1Vol/(cm*cm*cm) << " cm^3 sample 1 volume \n"
		 << Samp2Vol/(cm*cm*cm) << " cm^3 sample 2 volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ZrSample::SetName(G4String giveName){
	name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
