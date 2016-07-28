
#ifndef RunAction_h
#define RunAction_h 1

#include "DataOutput.hh"
#include "Doppler.hh"
#include "G4UserRunAction.hh"
#include "globals.hh"
#include "PrimaryGeneratorAction.hh"
#include "Randomize.hh"

#define CHANNELS 4000

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class G4Run;
class PrimaryGeneratorAction;
class DataOutput;
class RunAction : public G4UserRunAction
{
public:
  RunAction(DataOutput*,Doppler*);
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double, G4int*, G4double*, G4double*, G4double*, G4double*); 
  Doppler* doppler;
  CLHEP::RandGauss* gaussianRand;
  void SetRres(G4double r) {rRes = r;};
  void SetZres(G4double r) {zRes = r;};

private:
  FILE* binOutput;
  DataOutput* data;
  G4double sumECrys, sum2ECrys;
  G4double sumLCrys, sum2LCrys;
  G4int totalSpectrum[CHANNELS];
  G4int segmentSpectrum[16][CHANNELS];
  G4int segmentCounts[16];
  G4int totalEbySeg[16][CHANNELS];
  G4int E_vs_R[CHANNELS][10];
  G4int E_vs_Phi[CHANNELS][10];
  G4double maxSegE, E_Doppler;
  G4double E_per_channel,x,y,z,zRand,rRand,zRes,rRes;
  G4double mm_per_channel, rad_per_channel;
  G4int totalCounts;
  G4int totalCountsSeg[16];
  G4int segment, maxSeg, tagDetector;
  G4int i,j;
  G4String name;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

