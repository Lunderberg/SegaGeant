#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

#include "G4Ions.hh"
#include "G4ionIonisation.hh"
#include "G4IonTable.hh"
#include "G4IonConstructor.hh"
#include "G4StepLimiter.hh"

#include "G4GenericIon.hh"
#include "G4RadioactiveDecay.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

class PhysicsList: public G4VUserPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

  protected:
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

  protected:
   void ConstructEM();
   void constructIon();

};

#endif

