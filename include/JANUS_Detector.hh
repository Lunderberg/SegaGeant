#ifndef JANUS_Detector_H
#define JANUS_Detector_H

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

class JANUS_Detector {
public:

  JANUS_Detector();
  ~JANUS_Detector(); 
  void Construct();
  G4LogicalVolume* GetLogical() {
    return detector_Log;
  }

private:

  G4Material* Si;

  G4double innerRadius, outerRadius, thickness;
  //G4double rSegs, phiSegs; 
  //G4double segInnerRad, segOuterRad, segStartAngle, segStopAngle;
  //G4int i, j;
  //G4ThreeVector detSegVec;

  //G4Tubs* detSeg_Tubs;
  //G4LogicalVolume* detSeg_Log;
  //G4VPhysicalVolume* detSeg_Phys;

  G4Tubs* detector_Tubs;
  G4LogicalVolume* detector_Log;

};

#endif
