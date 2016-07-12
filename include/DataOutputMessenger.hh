#ifndef DataOutputMessenger_h
#define DataOutputMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "globals.hh"

class G4UIdirectory;
class DataOutput;
class DataOutputMessenger: public G4UImessenger
{
  public:
    DataOutputMessenger(DataOutput*);
    ~DataOutputMessenger();
    void SetNewValue(G4UIcommand*, G4String);

  private:
    DataOutput* data;
    G4UIcmdWithAString* dirCmd;
    G4UIcmdWithAString* fileCmd;
    G4UIcmdWithABool*   SegVerboseCmd;
    G4UIcmdWithABool*   TotalEVerboseCmd;
    G4UIdirectory*      dataDir;
};

#endif

