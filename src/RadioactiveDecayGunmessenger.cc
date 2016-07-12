
#include "RadioactiveDecayGunmessenger.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

//-----------------------------------------------------------------------------
RadioactiveDecayGunmessenger::RadioactiveDecayGunmessenger
(RadioactiveDecayGun* theRadioactiveDecayGun1, DataOutput* dat) :
theRadioactiveDecayGun(theRadioactiveDecayGun1) {
  data = dat;
  ionCmd = new UIcmdWithNucleusAndUnit("/source/ion",this);
  ionCmd->SetGuidance("define the primary ion (a,z,e)");
  ionCmd->SetParameterName("A","Z","E",true);

  ionCmd->SetDefaultUnit("keV");
  ionCmd->SetUnitCandidates("keV MeV");
}
//-----------------------------------------------------------------------------
RadioactiveDecayGunmessenger::~RadioactiveDecayGunmessenger () {
  delete ionCmd;
}
//-----------------------------------------------------------------------------
void RadioactiveDecayGunmessenger::SetNewValue
  (G4UIcommand *command, G4String newValues) {

  if (command==ionCmd)
  {
    G4IonTable* ionTable =
          (G4IonTable*)(G4ParticleTable::GetParticleTable()->GetIonTable());
    G4int a;
    G4int z;
    G4double e;
    char unit[30];
    sscanf(newValues,"%i %i %lf %s",&a,&z,&e,unit);
    G4String nucstr = ionTable->GetIonName(z,a,e);
    theRadioactiveDecayGun->SetNucleus(ionCmd->GetNewNucleusValue(newValues));
    data->SetNucleus(nucstr);
    G4cout << "Setting Nucleus: " << nucstr << G4endl;
  }
}
//-----------------------------------------------------------------------------










