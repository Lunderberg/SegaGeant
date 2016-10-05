#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction(Doppler* dop)
 :  experimentalHall_log(0), experimentalHall_phys(0)
{
  materials = new Materials;

 doppler = dop;
 Ar = new G4Material("ArgonGas",18,39.85*g/mole,1.782*mg/cm3);
 Al = new G4Material("Aluminum",13,26.98*g/mole,2.7*g/cm3);
 Au = new G4Material("Gold",79,196.9*g/mole,19.32*g/cm3);
 Si = new G4Material("Silicon",14,28.08*g/mole,2.33*g/cm3);
 C  = new G4Element("Carbon","C",6,12.011*g/mole);
 Co = new G4Element("Cobalt","Co",27,58.9332*g/mole);
 Fe = new G4Element("Iron","Fe",26,55.85*g/mole);
 ssteel = new G4Material("ssteel",7.7*g/cm3,3);
 ssteel->AddElement(C,0.04);
 ssteel->AddElement(Fe,0.88);
 ssteel->AddElement(Co,0.08);

 target = new Target(experimentalHall_log);
}

DetectorConstruction::~DetectorConstruction() {
  delete target;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

 // Construct Experimental Hall
 //
 G4double expHall_x = 5.0*m;
 G4double expHall_y = 5.0*m;
 G4double expHall_z = 5.0*m;

 G4Box* experimentalHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
 experimentalHall_log = new G4LogicalVolume(experimentalHall_box, Ar, "expHal_log",0,0,0);
 experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);


 // Construct Beam Tube
 //
 BTrmin=7.366*cm;
 BTrmax=7.62*cm;
 BTDz=44*cm;
 BTPhi0=0*deg;
 BTdPhi=360*deg;
 BTPos.setX(0);
 BTPos.setY(0);
 BTPos.setZ(0);
 BeamTubeMaterial = Al;

 G4Tubs* BeamTube_solid = new G4Tubs("BeamTube",BTrmin,BTrmax,BTDz,BTPhi0,BTdPhi);
 BeamTube_log = new G4LogicalVolume(BeamTube_solid,BeamTubeMaterial,"BeamTube_log",0,0,0);
 BeamTube_phys = new G4PVPlacement(0,BTPos, // Rotation and position
                                   BeamTube_log,                // logical volume
				   "BeamTube",                  // name
				   experimentalHall_log,        // mother logical volume
				   false,                       // boolean operations
				   0);                          // copy number


 // Construct Target
 //
 target->Construct();


 // Construct Silicon Telescope
 //
 /*
 CDrmin=2*cm;
 CDrmax=6*cm;
 CDthickness=1*mm;
 CDPhi0=0*deg;
 CDdPhi=360*deg;
 CDz1=2*cm;
 CDz2=4*cm;
 CDMaterial = Si;

 G4Tubs* CD_solid = new G4Tubs("CD",CDrmin,CDrmax,CDthickness,CDPhi0,CDdPhi);
 CD_log = new G4LogicalVolume(CD_solid,CDMaterial,"CD_log",0,0,0);
 CD1_phys = new G4PVPlacement(0,G4ThreeVector(0,0,CDz1),CD_log,"CD1",experimentalHall_log,false,0);
 CD2_phys = new G4PVPlacement(0,G4ThreeVector(0,0,CDz2),CD_log,"CD2",experimentalHall_log,false,0);
 */

 // Construct SeGA Array
 // Barrel SeGA
 R1 = 12.975*cm;
 R2 = 12.975*cm;
 Theta1 = 0*deg;
 Theta2 = 0*deg;
 z1 = .0*cm;
 z2 = .0*cm;
 Spaces1 = 8;
 Spaces2 = 8;
 nDet1 = 8;
 nDet2 = 6; //should be 8, lowered temporarily
 flip1 = 1;
 flip2 = -1;
 RingID1 = 1;
 RingID2 = 2;

 std::vector<SeGA_Detector*>::iterator itPos;
 for(itPos = ring1.begin();itPos<ring1.end();itPos++)
  {delete *itPos;}
 ring1.clear();

 for(itPos = ring2.begin();itPos<ring2.end();itPos++)
  {delete *itPos;}
 ring2.clear();

 G4int i;
 for(i=0;i<nDet1;i++) {
  ring1.push_back(new SeGA_Detector(experimentalHall_log));
 }
 for(i=0;i<nDet2;i++) {
  ring2.push_back(new SeGA_Detector(experimentalHall_log));
 }

 Phi=0;
 i=0;
 for(itPos=ring1.begin();itPos<ring1.end();itPos++) {
   (*itPos)->Construct(i,RingID1,flip1,Theta1,Phi,R1,z1,doppler);
   i++;
   Phi+=360*deg/Spaces1;
 }

 Phi=0;
 i=0;
 for(itPos=ring2.begin();itPos<ring2.end();itPos++) {
   (*itPos)->Construct(i,RingID2,flip2,Theta2,Phi,R2,z2,doppler);
   i++;
   Phi+=360*deg/Spaces2;
 }

 //Construt JANUS Detectors
 JANUS_Offset1 = {0.0,0.0,3.0*cm};
 JANUS_Offset2 = {0.0,0.0,-3.0*cm};

 JANUS = new JANUS_Detector;
 JANUS->Construct();
 JANUS_Phys1 = new G4PVPlacement(0, JANUS_Offset1, JANUS->GetLogical(), "JANUS_Phys", experimentalHall_log,0,0);
 JANUS_Phys2 = new G4PVPlacement(0, JANUS_Offset2, JANUS->GetLogical(), "JANUS_Phys", experimentalHall_log,0,0);

 // Construct Frame & Gate Valve
 //
 frameInnerRadius = 47.6*cm;
 frameOuterRadius = 65.7*cm;
 frameThickness = 0.635*cm;
 frameOffset = 16*cm;

 GVinnerRadius = 9.3*cm;
 GVouterRadius = 17*cm;
 GVlength = 5.9*cm;
 GVoffset = 50*cm;

 G4Tubs* frame = new G4Tubs("frame",frameInnerRadius,frameOuterRadius,frameThickness,45*deg,360*deg);
 frame_log = new G4LogicalVolume(frame,Al,"frame_log",0,0,0);
 frame1_phys = new G4PVPlacement(0,G4ThreeVector(0,0,frameOffset),frame_log,"frame",experimentalHall_log,false,0,true);
 frame2_phys = new G4PVPlacement(0,G4ThreeVector(0,0,-frameOffset),frame_log,"frame",experimentalHall_log,false,0,true);

 G4Tubs* GV = new G4Tubs("GV",GVinnerRadius,GVouterRadius,GVlength,45.*deg,360.*deg);
 GV_log = new G4LogicalVolume(GV,ssteel,"GV_log",0,0,0);
 GV_phys = new G4PVPlacement(0,G4ThreeVector(0,0,GVoffset),GV_log,"GV",experimentalHall_log,false,0,true);


 // Vis Attributes
 //
 experimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);

 G4Colour lblue (0.1,0.3,0.4);
 G4VisAttributes* BTvis = new G4VisAttributes(lblue);
 BTvis->SetVisibility(true);
 BTvis->SetForceSolid(false);
 BeamTube_log->SetVisAttributes(BTvis);

 /*G4Colour green (0.0,1.0,0.0);
 G4VisAttributes* CDvis = new G4VisAttributes(green);
 CDvis->SetVisibility(true);
 CDvis->SetForceSolid(false);
 CD_log->SetVisAttributes(CDvis);
 */

 G4Colour grey (0.8,0.8,0.8,0.3);
 G4VisAttributes* frameVis = new G4VisAttributes(grey);
 frameVis->SetVisibility(true);
 frameVis->SetForceSolid(false);
 frame_log->SetVisAttributes(frameVis);
 GV_log->SetVisAttributes(frameVis);


 return experimentalHall_phys;
}
