#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "DataOutput.hh"
#include "G4ParticleGun.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class G4ParticleGun;
class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
public:
  PrimaryGeneratorMessenger(PrimaryGeneratorAction*, DataOutput*);
  virtual ~PrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  PrimaryGeneratorAction*        Action;
  DataOutput*                    data;
  G4ParticleGun*                 particleGun;
  G4UIdirectory*                 gunDir; 
  G4UIcmdWithADouble*            BetaCmd;
  G4UIcmdWithADoubleAndUnit*     EnerCmd;
  G4UIcmdWithABool*              BeamCmd;
};

#endif

