#ifndef RadioactiveDecayGun_h
#define RadioactiveDecayGun_h 1

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "Nucleus.hh"
#include "DataOutput.hh"
#include "RadioactiveDecayGunmessenger.hh"

class RadioactiveDecayGunmessenger;
////////////////////////////////////////////////////////////////////////////////
//
class RadioactiveDecayGun : public G4ParticleGun
{
  // class description
  // The RadioactiveDecayGun is an inherited version of G4ParticleGun
  // to allow user to specify an isotope as the initial tracking particle.
  // class description - end

public:
  RadioactiveDecayGun(DataOutput*);
  ~RadioactiveDecayGun();

public: // with description

  void  SetNucleus(Nucleus theIon1);
  // Sets the isotope.
  //
    inline Nucleus GetNucleus() {return theIon;}
  // Returns the specified isotope.
  //
private:

  RadioactiveDecayGunmessenger  *theRadioactiveDecayGunMessenger;
  DataOutput* data;
  Nucleus theIon;

};
#endif
////////////////////////////////////////////////////////////////////////////////




