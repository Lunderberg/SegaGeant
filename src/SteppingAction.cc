#include "SteppingAction.hh"
#include "DataOutputFormat.hh"

#include "G4ios.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "Randomize.hh"

SteppingAction::SteppingAction(EventAction* evt)
  :eventaction(evt)
{
// gaussianRand = new CLHEP::RandGauss((new CLHEP::RanecuEngine));
  resolution = 0.003;
}

SteppingAction::~SteppingAction()
{
//  delete gaussianRand;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
   G4VPhysicalVolume* volume
   = aStep->GetPreStepPoint()->GetPhysicalVolume();//GetTouchableHandle()->GetVolume();
   G4int copy;
   G4double edep = aStep->GetTotalEnergyDeposit();
   G4double stepl = 0;
   G4String particle=aStep->GetTrack()->GetDefinition()->GetParticleName();
   if(particle != "anti_nu_e" && particle != "nu_e")
   {
     stepl = aStep->GetStepLength();
   }
   if(volume->GetName() == "detectorcell" || volume->GetName() == "JANUS_Phys")
   {
     copy = volume->GetCopyNo();
     G4double X,Y,Z;
     X = aStep->GetTrack()->GetPosition().x();
     Y = aStep->GetTrack()->GetPosition().y();
     Z = aStep->GetTrack()->GetPosition().z();
     mean = edep;
     Type det;

     if (edep != 0.0) {

      if(volume->GetName() == "detectorcell") {
       resolution = 0.003;
       det = SeGA; 
      }

      if (volume->GetName() == "JANUS_Phys") {
       resolution = 0.003;
       det = JANUS;
      }

     stdDev = resolution*edep/2.35;
     randEnergy = eventaction->runAct->gaussianRand->fire(mean,stdDev);
     eventaction->AddCrys(randEnergy,stepl,copy,X,Y,Z,det);
     }
   } 
}


