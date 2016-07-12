#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4ios.hh"

#include "Randomize.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "DataOutput.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "Doppler.hh"
#include "G4VisExecutive.hh"

int main(int argc,char* argv[])
{
  //Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Set Data Output
  DataOutput* dataOutput = new DataOutput;

  // Set Doppler reconstruction
  Doppler* doppler = new Doppler;

  // Set the Random Engine
//  CLHEP::RandGauss* RandGaussObj =
                   new CLHEP::RandGauss((new CLHEP::RanecuEngine)/*, mean, std*/); 
//  G4double rand;
//  for(G4int k=0;k<10;k++)
//  {
//  rand = RandGaussObj->fire(10.0,2.0);
//  G4cout << "Random Number: " << rand << G4endl;
//  }

  //Set mandatory initialization classes
  DetectorConstruction* detector = new DetectorConstruction(doppler);
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);


// set user action classes
  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(dataOutput,doppler);
  runManager->SetUserAction(gen_action);
  //
  RunAction* run_action = new RunAction(dataOutput,doppler);
  runManager->SetUserAction(run_action);
  //
  EventAction* event_action = new EventAction(run_action);
  runManager->SetUserAction(event_action);
  //
  G4UserSteppingAction* stepping_action = new SteppingAction(event_action);
  runManager->SetUserAction(stepping_action);

//Initialize Geant4 kernel
  runManager->Initialize();

// Get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4VisManager* visManager = new G4VisExecutive;
  G4cout << "Setting default source" << G4endl;
     UI->ApplyCommand("/source/ion 137 55 0");
     UI->ApplyCommand("/event/zRes -999 cm");

  if(argc!=1)  // Batch Mode
  {
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    G4cout << "Batch Mode\nOpening " << filename << G4endl;
    UI->ApplyCommand(command+filename);
  }
  else       // Interactive Mode
  {
  visManager->Initialize();
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/event/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");
//  UI->ApplyCommand("/control/execute vis.mac");
  UI->ApplyCommand("/vis/open OGLSX");
  UI->ApplyCommand("/vis/drawVolume");
  UI->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 70 20");
  G4UIsession* session = new G4UIterminal(new G4UItcsh);
  session->SessionStart();
  delete session;
  }

  // Job termination
  delete visManager;
  delete runManager;
  delete dataOutput;
  //delete RandGaussObj;
  return 0;
}
