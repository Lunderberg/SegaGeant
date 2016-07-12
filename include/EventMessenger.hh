#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"
#include "EventAction.hh"
#include "RunAction.hh"

class EventAction;
class RunAction;
class G4UIcmdWithADouble;
class EventMessenger: public G4UImessenger
{
 public:
   EventMessenger(EventAction*);
   ~EventMessenger();

 private:
   void                 SetNewValue(G4UIcommand*, G4String);
   EventAction*         event;
   G4UIcmdWithADouble*  IntrensicResCmd;
   G4UIcmdWithADoubleAndUnit*  zResCmd;
   G4UIcmdWithADoubleAndUnit*  rResCmd;
};

#endif

