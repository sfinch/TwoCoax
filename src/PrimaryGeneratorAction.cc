//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/PrimaryGeneratorAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include <map>
#include <stdexcept> 

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4GeneralParticleSource.hh"

#include "TF1.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
:Detector(DC),rndmFlag("on")
{

  pi = 3.141592;
  sampleWidth = 0;
  sampleWidth1 = (10.22-(2*1.41))*mm;
  sampleWidth2 = (10.07-(2*1.57))*mm;
  sample2InR  = (0.5*31.75 + 1.31)*mm;
  if (DC->GetZrFlag() == "on"){
    sampleWidth1 = DC->GetZrSample()->GetActive1Thick();
    sampleWidth2 = DC->GetZrSample()->GetActive2Thick();
    sample2InR  = DC->GetZrSample()->GetActive2InR();
    //G4double sample1OutR = DC->GetZrSample()->GetActive1OutR();
    //G4double sample2OutR = DC->GetZrSample()->GetActive2OutR();
  }

  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);
  for (int i=0; i<4; i++){
    energy[i] = 100*keV;
  }
  for (int i=0; i<5; i++){
    spin[i] = 0;
  } 
  // Particle gun
  numGamma = 2;
  positionR = 0*cm;
  positionX = 0*cm;
  if (numGamma == 2){
    //spin[0] = 2;
    spin[0] = 0;
    spin[1] = 2;
    // 102Ru
    //energy[0] = 468.58*keV;
    //energy[1] = 475.06*keV;
    // 96Zr
    //energy[0] = 369.8*keV; // 0^+_1
    //energy[0] = 551.8*keV; // 0^+_2
    //energy[0] = 719.6*keV; // 2^+_2
    //energy[0] = 847.7*keV; // 2^+_3
    energy[0] = 1844.25*keV; // 0^+_3
    energy[1] = 778.23*keV;
    // 150Nd 
    //energy[0] = 406.51*keV;
    //energy[1] = 333.96*keV;
  }
  else if (numGamma == 3){
    // 96Nb
    // BR = 51.5%
    /*
    spin[0] = 5;
    spin[1] = 4;
    spin[2] = 2;
    energy[0] = 568.9*keV;  //5 -> 4
    energy[1] = 1091.3*keV; //4 -> 2
    energy[2] = 778.2*keV;  //2 -> 0
    */
    // BR = 11.3%
    /*
    spin[0] = 5;
    spin[1] = 4;
    spin[2] = 2;
    energy[0] = 810.8*keV;  //5 -> 4
    energy[1] = 849.9*keV;  //4 -> 2
    energy[2] = 778.2*keV;  //2 -> 0
    */
    // BR = 18.8%
    spin[0] = 5;
    spin[1] = 3;
    spin[2] = 2;
    energy[0] = 460.0*keV;  //5 -> 4
    energy[1] = 1200.*keV;  //4 -> 2
    energy[2] = 778.2*keV;  //2 -> 0
  }
  else if (numGamma == 4){
    // 96Nb
    spin[0] = 5;
    spin[1] = 4;
    spin[2] = 4;
    spin[3] = 2;
    energy[0] = 568.9*keV;  //5 -> 4
    energy[1] = 241.4*keV;  //4 -> 4
    energy[2] = 850*keV;    //4 -> 2
    energy[3] = 778.2*keV;  //2 -> 0
  }

  fPDF[20] = new TF1("fPDF020","1.-3.*cos(x)**2+4.*cos(x)**4",0.,pi);
  fPDF[10] = new TF1("fPDF010","1.+cos(x)**2",0.,pi);

  fPDF[120] = new TF1("fPDF120","1.-(1./3.)*cos(x)**2",0.,pi);

  fPDF[220] = new TF1("fPDF220","1.+0.42856*cos(x)**2",0.,pi);

  fPDF[342] = new TF1("fPDF342","1.-0.19696*cos(x)**2",0.,pi);
  fPDF[322] = new TF1("fPDF320","1.-0.07314*cos(x)**2",0.,pi);
  fPDF[320] = new TF1("fPDF320","1.-0.10318*cos(x)**2",0.,pi);

  fPDF[444] = new TF1("fPDF444","1.+0.31965*cos(x)**2",0.,pi);
  fPDF[442] = new TF1("fPDF542","1.+(1./3.)*cos(x)**2",0.,pi);
  fPDF[434] = new TF1("fPDF434","1.+0.02906*cos(x)**2+0.00261*cos(x)**4",0.,pi);
  fPDF[432] = new TF1("fPDF432","1.+0.0769*cos(x)**2",0.,pi);
  fPDF[422] = new TF1("fPDF422","1.+0.1107*cos(x)**2",0.,pi);
  fPDF[420] = new TF1("fPDF420","1.-(1./8.)*cos(x)**2+(1./24.)*cos(x)**4",0.,pi);

  fPDF[544] = new TF1("fPDF544","1.-0.1019*cos(x)**2",0.,pi);
  fPDF[543] = new TF1("fPDF543","1.+0.07683*cos(x)**2",0.,pi);
  fPDF[542] = new TF1("fPDF542","1.-0.10319*cos(x)**2",0.,pi);
  fPDF[534] = new TF1("fPDF534","1.-0.04454*cos(x)**2",0.,pi);
  fPDF[532] = new TF1("fPDF532","1.-0.10318*cos(x)**2",0.,pi);

  particleGun  = new G4ParticleGun(1);
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(energy[0]);
  particleGun->SetParticlePosition(G4ThreeVector(positionX,positionR,0.*cm));

  // Particle source
  particleSource = new G4GeneralParticleSource();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { 
  delete particleGun;
  delete gunMessenger;
  delete particleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  
  // custom event generator
  if (rndmFlag == "off"){
    G4ThreeVector p[5];
    p[0] = randP();

    //randomize position in x (if wanted)
    if (sampleWidth/mm > 0){
        if (positionR > sample2InR){
            sampleWidth = sampleWidth2;
        }
        else{
            sampleWidth = sampleWidth1;
        }

        G4double posX = sampleWidth*(G4UniformRand()-0.5);
        particleGun->SetParticlePosition(G4ThreeVector(posX,positionR,0.*cm));
    }
    else{
        particleGun->SetParticlePosition(G4ThreeVector(positionX,positionR,0.*cm));
    }

    //calculate multipole radiation angular distribution
    for (int i=1; i<numGamma; i++){
      p[i] = randMultipole(p[i-1],spin[i-1], spin[i], spin[i+1]);
    }

    //start the gun
    for (int i=0; i<numGamma; i++){
      particleGun->SetParticleMomentumDirection(p[i]);
      particleGun->SetParticleEnergy(energy[i]);
      particleGun->GeneratePrimaryVertex(anEvent);
    }
  }

  // GPS
  if (rndmFlag == "on"){
    particleSource->GeneratePrimaryVertex(anEvent);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::randP(){

  //isotropic source 
  double P = 0.;
  double px, py, pz;
  do{
    px = (2*G4UniformRand())-1;
    py = (2*G4UniformRand())-1;
    pz = (2*G4UniformRand())-1;
    P = px*px+py*py+pz*pz;
  }while(P>1);
  P = sqrt(P);
  px = px/P;
  py = py/P;
  pz = pz/P;

  return G4ThreeVector(px,py,pz);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::randMultipole(G4ThreeVector P, int m1, int m2, int m3)
{

  G4double theta = P.getTheta();
  G4double phi = P.getPhi();

  double theta2 = 0;
  double phi2 = 2*3.141592*G4UniformRand();
  int decay = m1*100 + m2*10 + m3;
  try{
    theta2 = fPDF.at(decay)->GetRandom();
  }
  catch(const std::out_of_range& oor) {
    G4cout << "ERROR: W(" << decay << ") is not included! Using isotropic distribution." << G4endl;
    return randP();
  }

  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateY(theta);
  rot->rotateZ(phi);

  G4ThreeVector p2 (1,0,0);
  p2.setTheta(theta2);
  p2.setPhi(phi2);
  p2.setMag(1.);

  p2.transform(*rot);
  delete rot;

  //G4cout << "Theta should be " << std::cos(theta2) << ": " << e2.dot(P) << G4endl;

  return p2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::PrintGunParameters(){
  G4cout << "\n------------------------------------------------------------"
         << "\n---> Rndm flag (on is GPS, off is custom): " <<  rndmFlag  
         << "\n---> R Position:       " << positionR/cm << " cm"
         << "\n---> X Position:       " << positionX/cm << " cm"
         << "\n---> Sample width:     " << sampleWidth/cm << " cm"
         << "\n---> Number of gammas: " << numGamma << ""
         << "\n---> Level scheme: " << "\n";
  for (int i=0; i<numGamma; i++){
    G4cout << "       ----- " << spin[i] << "\n"
           << "        | " << energy[i]/keV << " keV\n";
  }
  G4cout << "       ----- " << spin[numGamma]
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
