#include "DataOutputMessenger.hh"
#include "DataOutput.hh"
#include "G4UIdirectory.hh"

DataOutputMessenger::DataOutputMessenger(DataOutput* dat)
{
 data = dat;
 dataDir = new G4UIdirectory("/data/");
 dataDir->SetGuidance("Data Output commands");
 dirCmd = new G4UIcmdWithAString("/data/directory",this);
 dirCmd->SetGuidance("Choose directory for data output");
 fileCmd = new G4UIcmdWithAString("/data/file",this);
 fileCmd->SetGuidance("Choose root file name for data output");
 SegVerboseCmd = new G4UIcmdWithABool("/data/segverbose",this);
 SegVerboseCmd->SetGuidance("Output energy spectra for each segment");
 TotalEVerboseCmd = new G4UIcmdWithABool("/data/totaleverbose",this);
 TotalEVerboseCmd->SetGuidance("Output energy spectrum for all segments combined");
}

DataOutputMessenger::~DataOutputMessenger()
{
  delete dirCmd;
  delete fileCmd;
}

void DataOutputMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
 if(command == dirCmd){
   data->SetDir(newValue);
 }
 if(command == fileCmd){
   data->SetFile(newValue);
 }
 if(command == SegVerboseCmd){
   data->SetSegVerbose(SegVerboseCmd->GetNewBoolValue(newValue));
 }
 if(command == TotalEVerboseCmd){
   data->SetTotalEVerbose(TotalEVerboseCmd->GetNewBoolValue(newValue));
 }
}
