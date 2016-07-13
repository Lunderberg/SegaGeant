#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(
                                          PrimaryGeneratorAction* Gun, DataOutput* dat)
{
  data = dat;
  Action = Gun;
  gunDir = new G4UIdirectory("/source/");
  BetaCmd = new G4UIcmdWithADouble("/source/beta",this);
  EnerCmd = new G4UIcmdWithADoubleAndUnit("/source/energy",this);
  BeamCmd  = new G4UIcmdWithABool("/source/beam",this);
  EmLocCmd = new G4UIcmdWith3VectorAndUnit("/source/emLoc",this);

  gunDir->SetGuidance("source control");
  BetaCmd->SetGuidance("Set source speed v/c");
  EnerCmd->SetGuidance("Set source energy");
  BeamCmd->SetGuidance("Choose Beam mode (1) or source mode (0)");
  EmLocCmd->SetGuidance("Set point of emission");


}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete gunDir;
  delete BetaCmd;
  delete EnerCmd;
  delete BeamCmd;
  delete EmLocCmd;
}


void PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
 if(command == BetaCmd){
   Action->SetBeta(BetaCmd->GetNewDoubleValue(newValue));
   data->SetBeta(newValue);
   }
 if(command == EnerCmd){
   Action->SetE(EnerCmd->GetNewDoubleValue(newValue));
   G4cout << "Setting CM Energy: " << newValue << G4endl;
   }
 if(command == BeamCmd){
   Action->SetBeam(BeamCmd->GetNewBoolValue(newValue));
   data->SetBeam(BeamCmd->GetNewBoolValue(newValue));
   }
 if (command == EmLocCmd){
   Action->SetEmLoc(EmLocCmd->GetNew3VectorValue(newValue));
   G4cout << "Setting Emission Location: " << newValue << G4endl;
   }
}

