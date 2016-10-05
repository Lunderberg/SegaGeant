#include "RunMessenger.hh"
#include "G4UIdirectory.hh"
#include "RunAction.hh"

RunMessenger::RunMessenger(RunAction* act)
{
 action = act;
 runDir = new G4UIdirectory("/runDir/");
 binFileCmd = new G4UIcmdWithAString("/runDir/binFile", this);
 binFileCmd->SetGuidance("Set name of binary output file.");
 binDirCmd = new G4UIcmdWithAString("/runDir/binDir", this);
 binDirCmd->SetGuidance("Set directory for binary output file");
}

RunMessenger::~RunMessenger()
{
 delete runDir;
 delete binFileCmd;
 delete binDirCmd;
}

void RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
 if (command == binFileCmd){
  action->SetBinFile(newValue);
 }

 if (command == binDirCmd){
  action->SetBinDir(newValue);
 }
}
