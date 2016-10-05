#include "JANUS_Detector.hh"

JANUS_Detector::JANUS_Detector()

{
 Si = new G4Material("Si", 14, 28.0855*g/mole, 2.329*g/cm3);
 
 innerRadius = 1.1*cm;
 outerRadius = 3.5*cm;
 thickness = (140*0.001)*mm;

 //phiSegs = 32.0;
 //rSegs = 24.0;
 //detSegVec = {0.0,0.0,0.0}; 
}

JANUS_Detector::~JANUS_Detector()
{
}

void JANUS_Detector::Construct()
{
 //Create the shape and material of a JANUS Detector
 detector_Tubs = new G4Tubs("detector_Tubs", innerRadius, outerRadius, thickness/2, 0.0*deg, 360.0*deg);
 detector_Log = new G4LogicalVolume(detector_Tubs, Si, "detector_Log");

/* 
 //Slice the JANUS detector into its segments
 //loop over radial segments
 for (i=0; i < rSegs; i++)
 {
  segInnerRad = innerRadius +     i*(outerRadius-innerRadius)/rSegs;
  segOuterRad = innerRadius + (i+1)*(outerRadius-innerRadius)/rSegs;

  //loop over phi segments
  for (j=0; j<phiSegs; j++)
  {
   segStartAngle = j*(360*deg)/phiSegs; 
   segStopAngle = (j+1)*(360*deg)/phiSegs;

   //make the detector segements, place them in detector_Log
   detSeg_Tubs = new G4Tubs("detSeg_Tubs", segInnerRad, segOuterRad, thickness/2, segStartAngle, segStopAngle); 
   detSeg_Log = new G4LogicalVolume(detSeg_Tubs, Si, "detSeg_Log", 0, 0, 0);
   detSeg_Phys = new G4PVPlacement(0,detSegVec, detSeg_Log, "detSeg_Phys", detector_Log,0,0); 
  }
 }
*/ 

 //Visualization Attributes
 G4Colour yellow(1.0,1.0,0.0, 1.0);
 G4VisAttributes* vis = new G4VisAttributes(yellow);
 vis->SetVisibility(true);
 vis->SetForceSolid(true);

 detector_Log->SetVisAttributes(vis); 

 return;
}

