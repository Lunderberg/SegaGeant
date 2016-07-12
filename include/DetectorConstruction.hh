#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "SeGA_Detector.hh"
#include "Doppler.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "SeGA_Detector.hh"
#include <vector>

class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
   
   DetectorConstruction(Doppler*);
   ~DetectorConstruction();
   G4VPhysicalVolume* Construct();

  private:

   Doppler* doppler;

   G4LogicalVolume* experimentalHall_log;
   G4LogicalVolume* BeamTube_log;
   G4LogicalVolume* Target_log;
   G4LogicalVolume* CD_log;
   G4LogicalVolume* frame_log;
   G4LogicalVolume* GV_log;
  
   G4VPhysicalVolume* experimentalHall_phys;
   G4VPhysicalVolume* BeamTube_phys;
   G4VPhysicalVolume* Target_phys;
   G4VPhysicalVolume* CD1_phys;
   G4VPhysicalVolume* CD2_phys;
   G4VPhysicalVolume* frame1_phys;
   G4VPhysicalVolume* frame2_phys;
   G4VPhysicalVolume* GV_phys;


   G4Element* C;
   G4Element* Co;
   G4Element* Fe;
   G4Material* Ar;
   G4Material* Al;
   G4Material* Au;
   G4Material* Si;
   G4Material* ssteel;
   G4Material* BeamTubeMaterial;
   G4Material* TargetMaterial;
   G4Material* CDMaterial;

   G4double BTrmin, BTrmax, BTDz, BTPhi0, BTdPhi;
   G4double TargetX, TargetY, TargetZ;
   G4double CDrmin,CDrmax,CDthickness,CDPhi0,CDdPhi,CDz1,CDz2;
   G4double R1,R2,Theta1,Theta2,z1,z2,Phi;
   G4double frameInnerRadius, frameOuterRadius,frameThickness,frameOffset;
   G4double GVinnerRadius,GVouterRadius,GVlength,GVoffset;
   G4int Spaces1,Spaces2,nDet1,nDet2,flip1,flip2,RingID1,RingID2;

   G4ThreeVector BTPos;
   G4ThreeVector TargetPos;

   std::vector<SeGA_Detector*> ring1;
   std::vector<SeGA_Detector*> ring2;

};

#endif

