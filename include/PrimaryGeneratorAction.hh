#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "RadioactiveDecayGun.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "DataOutput.hh"
#include "Doppler.hh"

class G4ParticleGun;
class G4Event;
class G4PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
   public:
     PrimaryGeneratorAction(DataOutput*,Doppler*);
     ~PrimaryGeneratorAction();

   public:
     void GeneratePrimaries(G4Event* anEvent);
     void SetBeta(G4double);
     void SetE(G4double E);
     void SetBeam(G4bool b);
     void SetEmLoc(G4ThreeVector Loc);

   private:
     RadioactiveDecayGun* srcParticleGun;
     G4ParticleGun* beamParticleGun;
     PrimaryGeneratorMessenger* gunMessenger;
     DataOutput* data;
     Doppler* doppler;
     G4bool Beam;
     G4double Beta, Gamma, u_cm, u_lab, E_cm, E_lab, theta_lab;
     G4ThreeVector p_gamma, EmLoc;
};

#endif

