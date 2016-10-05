#ifndef RunMessenger_h
#define RunMessenger_h

#include "G4UIcmdWithAString.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class RunAction;
class RunMessenger: public G4UImessenger
{
 public:
  RunMessenger(RunAction*);
  ~RunMessenger();
  void SetNewValue(G4UIcommand*, G4String);

 private:
  RunAction* action;
  G4UIcmdWithAString* binFileCmd;
  G4UIcmdWithAString* binDirCmd;
  G4UIdirectory* runDir;
};

#endif
