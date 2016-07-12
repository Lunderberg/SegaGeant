
#include "EventMessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

EventMessenger::EventMessenger(EventAction* evt)
{
 event = evt;
 IntrensicResCmd = new G4UIcmdWithADouble("/event/intrensicRes",this);
 IntrensicResCmd->SetGuidance("define intrensic detector energy resolution");
 zResCmd = new G4UIcmdWithADoubleAndUnit("/event/zRes",this);
 zResCmd->SetGuidance("define spacial resolution in z direction");
 rResCmd = new G4UIcmdWithADoubleAndUnit("/event/rRes",this);
 rResCmd->SetGuidance("define spacial resolution in r direction");
}

EventMessenger::~EventMessenger()
{
 delete IntrensicResCmd;
 delete zResCmd;
 delete rResCmd;
}

void EventMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
 if (command==IntrensicResCmd)
 {
   event->SetResolution(IntrensicResCmd->GetNewDoubleValue(newValue));
   G4cout << "Setting intrensic energy resolution to " << newValue << G4endl;
 }
 if (command==zResCmd)
 {
   event->runAct->SetZres(zResCmd->GetNewDoubleValue(newValue));
   G4cout << "Setting Z resolution to " << newValue << G4endl;
 }
 if (command==rResCmd)
 {
   event->runAct->SetRres(rResCmd->GetNewDoubleValue(newValue));
   G4cout << "Setting R resolution to " << newValue << G4endl;
 }

}
