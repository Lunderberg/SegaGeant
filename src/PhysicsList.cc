#include "PhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "globals.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

PhysicsList::PhysicsList()
{
// defaultCutValue = 1*mm;
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
 G4Gamma::GammaDefinition();
 G4Proton::ProtonDefinition();
 G4Electron::ElectronDefinition();
 G4Positron::PositronDefinition();
 G4Neutron::NeutronDefinition();
 G4GenericIon::GenericIonDefinition();
 G4NeutrinoE::NeutrinoEDefinition();
 G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

void PhysicsList::ConstructProcess()
{
 AddTransportation();
 ConstructEM();
 constructIon();
}

void PhysicsList::SetCuts()
{
 G4int temp = GetVerboseLevel();
 SetVerboseLevel(0);
 SetCutsWithDefault();
// SetCutValue(100*um,"Gamma");
// SetCutValue(1*um,"e-");
// SetCutValue(1*um,"e+");
 SetVerboseLevel(temp);
}

void PhysicsList::ConstructEM()
{
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

      if (particleName == "gamma") {
         pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
	 pmanager->AddDiscreteProcess(new G4ComptonScattering);
	 pmanager->AddDiscreteProcess(new G4GammaConversion);

      } else if (particleName == "e-") {
         pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
	 pmanager->AddProcess(new G4eIonisation,-1,2,2);
	 pmanager->AddProcess(new G4eBremsstrahlung,-1,3,3);

      } else if (particleName == "e+") {
         pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
	 pmanager->AddProcess(new G4eIonisation,-1,2,2);
	 pmanager->AddProcess(new G4eBremsstrahlung,-1,3,3);
         pmanager->AddProcess(new G4eplusAnnihilation,0,-1,4);
      }
  }
}

void PhysicsList::constructIon() {
  G4IonTable *theIonTable=
    (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
  G4RadioactiveDecay *theRadioactiveDecay = new G4RadioactiveDecay();
  for(G4int j=0;j<theIonTable->Entries();j++) {
  G4String particleName = theIonTable->GetParticle(j)->GetParticleName();
    if(particleName == "GenericIon") {
       G4ProcessManager* pmanager = 
         theIonTable->GetParticle(j)->GetProcessManager();

       pmanager->AddProcess(theRadioactiveDecay);
       pmanager->SetProcessOrdering(theRadioactiveDecay,idxPostStep);
       pmanager->SetProcessOrdering(theRadioactiveDecay,idxAtRest);
       pmanager->AddProcess(new G4eMultipleScattering, -1 ,1 ,1);
       pmanager->AddProcess(new G4ionIonisation, -1, 2, 2);
       pmanager->AddProcess(new G4StepLimiter, -1,-1, 4);
    }
  }
}

