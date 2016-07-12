
#include "RadioactiveDecayGun.hh"
#include "RadioactiveDecayGunmessenger.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4UImanager.hh"
#include "G4IonTable.hh"
#include "G4Ions.hh"

RadioactiveDecayGun::RadioactiveDecayGun(DataOutput* dat)
{
 data = dat;
 theRadioactiveDecayGunMessenger = new RadioactiveDecayGunmessenger(this, data);
}

RadioactiveDecayGun::~RadioactiveDecayGun()
{delete theRadioactiveDecayGunMessenger;}

void RadioactiveDecayGun::SetNucleus (Nucleus theIon1)
{
 theIon = theIon1;

 G4IonTable* theIonTable = (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());

 G4ParticleDefinition *anIon = NULL;

 G4int A = theIon.GetA();
 G4int Z = theIon.GetZ();
 G4double E = theIon.GetE();

 anIon = theIonTable->GetIon(Z,A,E);

 SetParticleDefinition(anIon);

 G4UImanager* UI = G4UImanager::GetUIpointer();

 UI->ApplyCommand("/gun/energy 0.0 MeV");
 UI->ApplyCommand("/gun/position 0.0 0.0 0.24 cm");
}


