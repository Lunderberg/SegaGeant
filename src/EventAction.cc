#include "EventAction.hh"

#include "RunAction.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
//#include "Randomize.hh"

EventAction::EventAction(RunAction* run)
  :runAct(run),printModulo(1)
{
  setNScat();
  eventMessenger = new EventMessenger(this);
}


EventAction::~EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event* evt)
{ 
  crystalInfo.clear(); 
  G4int evtNb = evt->GetEventID();
  // do this just not to get warnings at compile
  evtNb = 1;
  
  // initialisation per event
  EnergyCrys = 0.;
  TrackLCrys = 0.;
  for(int i=0;i<n_scat;i++)
  {
    hits[i]   = 0;
    energy[i] = 0;
    X[i]      = 0;
    Y[i]      = 0;
    Z[i]      = 0;
    maxE[i]   = 0;
  }
}


void EventAction::EndOfEventAction(const G4Event* evt)
{
  // do this to avoid unused parameter warnings at compile
  int i = evt->GetEventID();
  i = 0;
  
  if(EnergyCrys!=0)
  { 
    //runAct->fillPerEvent(EnergyCrys,TrackLCrys,hits,energy,X,Y,Z);
    runAct->fillPerEvent(crystalInfo);
 } 
}  

void EventAction::AddCrys(G4double de,G4double dl,G4int detNo,
			       G4double x,G4double y,G4double z) 
{
  detNo = detNo/100000;

  if (!crystalInfo.count (detNo)) {
    crystalInfo[detNo].energy = 0;

    crystalInfo[detNo].maxEnergy = 0;

    crystalInfo[detNo].x = 0;

    crystalInfo[detNo].y = 0;

    crystalInfo[detNo].z = 0;
  }

  if (crystalInfo[detNo].maxEnergy <= de) {

   crystalInfo[detNo].x = x;
   crystalInfo[detNo].y = y;
   crystalInfo[detNo].z = z;
   crystalInfo[detNo].maxEnergy = de;
  }
 crystalInfo[detNo].energy += de; 

 G4int i=0;
  EnergyCrys += de;
  TrackLCrys += dl;
  if(hits[n_scat-1] != 0)
  {
    G4cout<<"********************\nWARNING! INCREASE 'N_SCAT' DEFINED\n";
    G4cout<<"IN EventAction.cc IF YOU DON'T WANT TO LOSE DATA!\n";
    G4cout<<"********************\n";
  }

  while(1)
  {
    if(de>maxE[i])
    {
      X[i]       = x;
      Y[i]       = y;
      Z[i]       = z;
    }
    if(hits[i] == 0)
    {
      hits[i]    = detNo;
      energy[i] += de;
      break;
    }
    else if(hits[i] == detNo)
    {
      energy[i] += de;
      break;
    }
    i++;
  }
}

