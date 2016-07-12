
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "RunAction.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(EventAction*);

  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  EventAction*          eventaction;
  CLHEP::RandGauss*     gaussianRand;
  G4double              mean, stdDev;
  G4double              resolution;
  G4double              randEnergy;
};


#endif
