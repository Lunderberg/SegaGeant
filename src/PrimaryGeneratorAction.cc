#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "RadioactiveDecayGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Nucleus.hh"
#include "DataOutput.hh"
#include "G4RandomDirection.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DataOutput* dat, Doppler* dop)
{
  data = dat;
  gunMessenger = new PrimaryGeneratorMessenger(this,dat);
  doppler = dop;
  Beam = 0;
  Beta = 0;
  E_cm = 1.0*MeV;
  EmLoc = G4ThreeVector(0.0,0.0,0.0);

  srcParticleGun = new RadioactiveDecayGun(data);

  G4int n_particle = 1;
  beamParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  beamParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
 delete gunMessenger;
 delete beamParticleGun;
 delete srcParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(!Beam)
  {
    srcParticleGun->GeneratePrimaryVertex(anEvent);
  }
  else {
    G4ThreeVector SourcePoint = G4RandomDirection();
    SourcePoint.setX(0.0);
    SourcePoint.setY(0.0);
    G4float NewBeta = Beta*(1-0.05*SourcePoint.z());
    SourcePoint.setZ(SourcePoint.z()*1*mm);

    //Gamma = 1.0/sqrt(1-Beta*Beta);
    Gamma = 1.0/sqrt(1-NewBeta*NewBeta);
    p_gamma = G4RandomDirection();
    u_cm = p_gamma.cosTheta();
    //u_lab = (u_cm + Beta)/(1+u_cm*Beta);
    u_lab = (u_cm + NewBeta)/(1+u_cm*NewBeta);
    theta_lab = acos(u_lab);
    p_gamma.setTheta(theta_lab);
    //E_lab = E_cm*Gamma*(1+Beta*u_cm);
    E_lab = E_cm*Gamma*(1+NewBeta*u_cm);

//    G4cout << "Original u_lab: " << u_lab << G4endl;


    //beamParticleGun->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));
    //beamParticleGun->SetParticlePosition(SourcePoint);
    beamParticleGun->SetParticleMomentumDirection(p_gamma);
    beamParticleGun->SetParticleEnergy(E_lab);
    beamParticleGun->GeneratePrimaryVertex(anEvent);
  }
}

void PrimaryGeneratorAction::SetBeta(G4double b)
{
 if (b>=0 && b<=1)
 {
  Beta = b;
  doppler->SetBeta(Beta);
  G4cout << "Setting Beta: " << Beta << G4endl;
}
 else {
  G4cout << "Beta must lie between 0 and 1" << G4endl;
 }
}

void PrimaryGeneratorAction::SetE(G4double e)
{
 E_cm = e;
}

void PrimaryGeneratorAction::SetBeam(G4bool b)
{
 Beam = b;
 if(Beam){G4cout << "Beam Mode" << G4endl;}
 if(!Beam){G4cout << "Calibration Source Mode" << G4endl;}
}

void PrimaryGeneratorAction::SetEmLoc(G4ThreeVector Loc)
{
 EmLoc = Loc;
 beamParticleGun->SetParticlePosition(EmLoc);
}
