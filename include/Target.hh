#ifndef TARGET_H_
#define TARGET_H_

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "Materials.hh"

class Target_Messenger;

class Target {
public:
  Target(G4LogicalVolume* parent_volume);
  ~Target();

  G4VPhysicalVolume *Construct();
  void setX(G4double);
  void setY(G4double);
  void setZ(G4double);
  void setMaterial(G4String);
  void setNStep(G4int);
  void setSourceFrame(G4String);
  void Report();
  G4LogicalVolume *GetTargetLog() {
    return Target_log;
  }
  G4Box *GetTarget() {
    return aTarget;
  }
  G4VPhysicalVolume *GetTargetPlacement() {
    return Target_phys;
  }
  void setTargetReactionDepth(G4double);
  void ScaleDensity(G4double);
  void SetPositionZ(G4double);
  G4double GetTargetThickness() {
    return Target_thickness;
  }
  G4ThreeVector *GetPos() {
    return Pos;
  }

private:
  G4LogicalVolume* parent_volume;

  Target_Messenger* messenger;

  // dimensions
  G4double Target_side_x;
  G4double Target_side_y;
  G4double Target_thickness;
  G4double frameThickness;
  G4double frameInnerRadius;
  G4double frameOuterRadius;
  G4double tapeThickness;

  Materials *materials;
  G4Material *TargetMaterial;
  G4Material *frameMaterial;
  G4Material *tapeMaterial;

  //default position
  G4RotationMatrix NoRot;
  G4ThreeVector *Pos;

  G4Box *aTarget;
  G4Tubs *euFrame;
  G4Tubs *euTape;
  G4Tubs *csFrame;
  G4Tubs *csRing;
  G4Tubs *csTape;

  G4LogicalVolume *Target_log;
  G4LogicalVolume *euFrame_log;
  G4LogicalVolume *euTape_log;
  G4LogicalVolume *csFrame_log;
  G4LogicalVolume *csRing_log;
  G4LogicalVolume *csTape_log;

  G4VPhysicalVolume *Target_phys;
  G4VPhysicalVolume *euFrame_phys;
  G4VPhysicalVolume *euTape_phys;
  G4VPhysicalVolume *csFrame_phys;
  G4VPhysicalVolume *csRing_phys;
  G4VPhysicalVolume *csTape_phys;

  G4UserLimits *target_limits;

  //Number of simulation steps
  G4int NStep;

  G4String sourceFrame;

};

#endif
