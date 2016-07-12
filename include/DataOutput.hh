#ifndef DataOutput_h
#define DataOutput_h 1
#include "globals.hh"
#include "Nucleus.hh"
#include "DataOutputMessenger.hh"

class DataOutput
{
 public:
   DataOutput();
   ~DataOutput();
   void WriteSpectrum(G4int*, G4int, G4int, G4double, G4String);
   void Write2DSpectrum(G4int[4000][10], G4int, G4int, G4String);
   void SetNucleus(G4String N) {nucleus = N;};
   void SetBeta(G4String b) {beta = b;};
   void SetBeam(G4bool b){ Beam = b;};
   void SetDir(G4String);
   void SetFile(G4String);

   void SetSegVerbose(G4bool b){SegVerbose = b;};
   G4bool GetSegVerbose() {return SegVerbose;};
   void SetTotalEVerbose(G4bool b){TotalEVerbose = b;};
   G4bool GetTotalEVerbose() {return TotalEVerbose;};

 private:
   DataOutputMessenger* datamessenger;
   G4int i,j;
   G4bool Beam;
   G4String tag;
   G4String beta;
   G4String nucleus;
   G4String FileName;
   G4String outputdir;
   G4bool SegVerbose, TotalEVerbose;
};

#endif
