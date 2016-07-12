
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "EventMessenger.hh"
#define N_SCAT 100

class RunAction;
class EventMessenger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*);
  virtual ~EventAction();
  void setNScat() {n_scat = N_SCAT;};
  void  BeginOfEventAction(const G4Event*);
  void  EndOfEventAction(const G4Event*);
    
  void AddCrys(G4double de,G4double dl,G4int detNo,
	       G4double x,G4double y,G4double z);
                     
  void SetPrintModulo(G4int    val)  {printModulo = val;};
  void SetResolution(G4double res) {resolution = res; G4cout << "Resolution :" << res << G4endl;};
  RunAction*  runAct;

private:
  EventMessenger*     eventMessenger;
  G4int               printModulo;
  G4int               n_scat;
  G4int               maxE[N_SCAT];
  G4int               hits[N_SCAT];
  G4double            energy[N_SCAT];
  G4double            dE;
  G4double            X[N_SCAT];
  G4double            Y[N_SCAT];
  G4double            Z[N_SCAT];
  G4double            EnergyCrys, TrackLCrys;
  CLHEP::RandGauss*   gaussianRand;
  G4double            mean;
  G4double            stdDev;
  G4double            resolution;
  G4double            randEnergy;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
