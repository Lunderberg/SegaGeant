#include "SeGA_Detector.hh"
#include "G4Transform3D.hh"
#include "DetectorConstruction.hh"
#include "G4ios.hh"
#include <cmath>
#include "G4AssemblyVolume.hh"

#include "G4VisAttributes.hh"

SeGA_Detector::SeGA_Detector(G4LogicalVolume* experimentalHall_log)
 
{
  expHall_log=experimentalHall_log;
  HpGe = new G4Material("HpGe", 32., 72.61*g/mole, 5.323*g/cm3);
  Al =   new G4Material("Al", 13., 26.982*g/mole, 2.70*g/cm3);   //LR
  preampMat = new G4Material("preampMat", 13., 26.982*g/mole, 1.35*g/cm3);   //LR  (Air, copper, and aluminum?)
  vacuum = new G4Material("vacuum",1,1*g/mole,1e-5*g/cm3);

  // crystal dimensions
  Length=4.025*cm;               //LR
  innerRadius =0.*cm;
  outerRadius =3.165*cm;     //LR (Lew)
  fingerRadius=0.5*cm;

  // dead layer dimensions
  DLinnerRadius = fingerRadius;                  //LR
  DLouterRadius = DLinnerRadius+0.03*cm;         //LR (Dirk)

  // can dimensions
  iCanOuterRadius = 3.73*cm;                     //LR
  iCanInnerRadius = iCanOuterRadius-0.05*cm;     //LR
  iCanLength      = Length;                      //LR

  oCanThickness   = 0.05*cm;
  oCanOuterRadius = 4.325*cm;                      //LR (Dirk)
  oCanInnerRadius = oCanOuterRadius-oCanThickness; //LR 
  oCanLength      = 10.0*cm;
  oCanOffset.setZ(oCanLength - Length - oCanThickness);  //LR (0.7*cm gives overlaps in the 37 degree ring)

  preampRadius = oCanInnerRadius-50*um;            //LR
  preampLength = oCanLength - Length - 0.6/2.0*cm; //LR
  preampOffset.setZ(oCanOffset.z()+Length);

  neckRadius = 3.33/2.0*cm;   //LR (approx. from drawing)
  neckLength = 14.47/2.0*cm;  //LR (approx. from drawing)
  neckOffset.setX((oCanOuterRadius + neckLength*sin(45.0*deg) + neckRadius*cos(45.0*deg))); //LR 
 // neckOffset.setZ(9.32*cm + neckLength*cos(45.0*deg));                         //LR (approx. from drawing)
  neckOffset.setZ(oCanLength + neckLength*cos(45.0*deg));                         //LR (approx. from drawing)

  cryoThickness = 0.4*cm;                                                                           //LR (guess)
  cryoOuterRadius = 23.06/2.0*cm;                                                                   //LR (approx. from drawing)
  cryoInnerRadius = cryoOuterRadius - cryoThickness;                                                //LR (approx. from drawing)
  cryoBaseThickness = 1.5/2.0*cm;                                                                   //LR (guess)
  cryoBaseOffset.setX(neckOffset.x() + neckLength*sin(45.0*deg) + cryoBaseThickness*sin(45.0*deg)); //LR
  cryoBaseOffset.setZ(neckOffset.z() + neckLength*cos(45.0*deg) + cryoBaseThickness*cos(45.0*deg)); //LR

  cryoLength = 34.60/2.0*cm;                                                                        //LR (approx. from drawing)
  cryoOffset.setX(cryoBaseOffset.x() + cryoBaseThickness*sin(45.0*deg) + cryoLength*sin(45.0*deg)); //LR
  cryoOffset.setZ(cryoBaseOffset.z() + cryoBaseThickness*cos(45.0*deg) + cryoLength*cos(45.0*deg)); //LR

  startAngle      = 45.*deg;
  spanningAngle   = 360.*deg;
  Orientation     = 1;

  rd=24*cm;
  thetad=37*deg;
  phid=0*deg;
  ZShift.setX(0);
  ZShift.setY(0);
  ZShift.setZ(0);
  F=3.;
  G=2.;
  H=0.;

  thr_center=50*keV;
  thr_width=50*keV;
}


SeGA_Detector::~SeGA_Detector()
{
}

void SeGA_Detector::Construct(G4int num,G4int ring,G4int orient, G4double th,G4double ph,G4double r,G4double z, Doppler* dop)
{

  RingID=ring;
  Number=num;
  Orientation=orient;
  thetad=th;
  phid=ph;
  rd=r;
  doppler = dop;
  ZShift.setZ(Length + 2*oCanThickness + z);

  // Material surrounding the crystal

  iCan = new G4Tubs("iCan",iCanInnerRadius,iCanOuterRadius,iCanLength,startAngle,spanningAngle); //LR
  iCan_log = new G4LogicalVolume(iCan,Al,"iCan_log",0,0,0);                                      //LR


  oCanFull   = new G4Tubs("oCanFull",  0.0,oCanOuterRadius,oCanLength,startAngle,spanningAngle); //LR
  oCanHollow = new G4Tubs("oCanHollow",0.0,oCanInnerRadius,oCanLength,startAngle,spanningAngle); //LR
  oCan = new G4SubtractionSolid("oCan",oCanFull,oCanHollow,G4Transform3D(G4RotationMatrix(),G4ThreeVector(0.*cm, 0.*cm, oCanThickness)));
  oCan_log = new G4LogicalVolume(oCan,Al,"oCan_log",0,0,0);                                      //LR


  DL = new G4Tubs("DL",DLinnerRadius,DLouterRadius,Length*3.5/4.,startAngle,spanningAngle);      //LR
  DL_log = new G4LogicalVolume(DL,HpGe,"DL_log",0,0,0);                                          //LR


  preamp = new G4Tubs("preamp",0.,preampRadius,preampLength,startAngle,spanningAngle);           //LR
  preamp_log = new G4LogicalVolume(preamp,Al,"preamp_log",0,0,0);                                //LR


  neck = new G4Tubs("neck",0.,neckRadius,neckLength,startAngle,spanningAngle);                   //LR
  neck_log = new G4LogicalVolume(neck,Al,"neck_log",0,0,0);                                      //LR


  cryoBase = new G4Tubs("cryoBase",0.,cryoOuterRadius,cryoBaseThickness,startAngle,spanningAngle); //LR
  cryoBase_log = new G4LogicalVolume(cryoBase,Al,"cryoBase_log",0,0,0);                       //LR


  cryoFull   = new G4Tubs("cryoFull",  0.,cryoOuterRadius,cryoLength,startAngle,spanningAngle);                                        //LR
  cryoHollow = new G4Tubs("cryoHollow",0.,cryoInnerRadius,cryoLength-cryoThickness,startAngle,spanningAngle);                          //LR
  cryo = new G4SubtractionSolid("cryo",cryoFull,cryoHollow,G4Transform3D(G4RotationMatrix(),G4ThreeVector(0.*cm, 0.*cm, 0.*cm))); //LR
  cryo_log = new G4LogicalVolume(cryo,Al,"cryo_log",0,0,0);                                                                       //LR


  // crystal

  full = new G4Tubs("full",innerRadius,outerRadius,Length,startAngle,spanningAngle);
  hollow = new G4Tubs("hollow",innerRadius,DLouterRadius+50*um,Length*3.5/4.+50*um,startAngle,spanningAngle); //LR

  //LR This is the volume that is quartered and sliced with G4PVReplicas below. 
  //LR The transformation applies to the second solid (hollow here).
  detector = new G4SubtractionSolid("detector",full,hollow,G4Transform3D(G4RotationMatrix(),G4ThreeVector(0.*cm, 0.*cm, Length/8.)));
  detector_log = new G4LogicalVolume(detector,HpGe,"detector_log",0,0,0);

 // Detector readout division
 G4ThreeVector detectorcellShift;
 G4RotationMatrix detectorcellRot;

 rmax = fingerRadius;
 G4int rsegs = 1;
 G4int phisegs = 4;
 G4int zsegs = 8;
 for(G4int k=0;k<rsegs;k++) //Loop over radial segments
 {
   // evenly spaced radial segments
    dr = (outerRadius-fingerRadius)/rsegs;
    rmin = fingerRadius + k*dr;
    rmax = rmin + dr;
   
   // area preserving radial segments
   // rmin=rmax;
   // rmax = sqrt((outerRadius*outerRadius - fingerRadius*fingerRadius)/3 + rmin*rmin);

   for(G4int i=0;i<phisegs;i++) //Loop over phi segments
    {
      G4Tubs* detectorcell_Tub = new G4Tubs("detectorcellTub",rmin,rmax,Length/zsegs, 360/phisegs*i*deg, 360/phisegs*deg);
      detectorcell_log = new G4LogicalVolume(detectorcell_Tub,HpGe,"detectorcell",0,0,0);

      for(G4int j=0;j<zsegs;j++) //Loop over z segments
      {
        detectorcellShift.setZ(j*2*Length/zsegs - double(zsegs-1)/double(zsegs)*Length);
        G4Transform3D detectorcellTrans = G4Transform3D(detectorcellRot,detectorcellShift);
        G4int Copy = RingID*1000000 + Number*100000 + j*1000 + i*10 + k; // 2 digits for the phi and z info
        //G4int Copy = RingID*100000 + Number*10000 + j*1000 + i*10 + k; // 2 digits for the phi info
        detectorcell_phys = new G4PVPlacement(detectorcellTrans,detectorcell_log,"detectorcell",detector_log,false,Copy);
      }
    }
  }

  // Visualization Attributes

  G4Colour dgreen (0.0,0.75, 0.0, 1.0); 
  G4VisAttributes* Vis_5 = new G4VisAttributes(dgreen);
  Vis_5->SetVisibility(true);
  Vis_5->SetForceSolid(true); //LR

  G4Colour transGrey (0.8, 0.8, 0.8, 0.3);                  //LR
  G4VisAttributes* Vis_6 = new G4VisAttributes(transGrey);  //LR
  Vis_6->SetVisibility(true);                               //LR
  Vis_6->SetForceSolid(false);                              //LR

  G4Colour greyGreen (0.5, 0.6, 0.65, 0.1);
  G4VisAttributes* Vis_7 = new G4VisAttributes(greyGreen);
  Vis_7->SetVisibility(true);
  Vis_7->SetForceSolid(false);

  detector_log->SetVisAttributes(Vis_5);
  detectorcell_log->SetVisAttributes(Vis_6);

  oCan_log->SetVisAttributes(Vis_6);   //LR
  iCan_log->SetVisAttributes(Vis_6);   //LR
  preamp_log->SetVisAttributes(Vis_6); //LR
  neck_log->SetVisAttributes(Vis_6);   //LR
  cryoBase_log->SetVisAttributes(Vis_7);   //LR
  cryo_log->SetVisAttributes(Vis_7);   //LR

  // Place Detector

  DetRot=G4RotationMatrix::IDENTITY;
  cryoRot=G4RotationMatrix::IDENTITY; //LR
  cryoRot.rotateY(45.*deg);         //LR

  G4AssemblyVolume* SeGA_assembly = new G4AssemblyVolume();

  G4Transform3D detTrans = G4Transform3D(DetRot,DetPos);
  G4Transform3D oCanTrans = G4Transform3D(DetRot,oCanOffset);
  G4Transform3D preampTrans = G4Transform3D(DetRot,preampOffset);
  G4Transform3D neckTrans = G4Transform3D(cryoRot,neckOffset);
  G4Transform3D cryoBaseTrans = G4Transform3D(cryoRot,cryoBaseOffset);
  G4Transform3D cryoTrans = G4Transform3D(cryoRot,cryoOffset);


  SeGA_assembly->AddPlacedVolume(detector_log,detTrans);
  SeGA_assembly->AddPlacedVolume(DL_log,detTrans);
  SeGA_assembly->AddPlacedVolume(iCan_log,detTrans);
  SeGA_assembly->AddPlacedVolume(oCan_log,oCanTrans);
  SeGA_assembly->AddPlacedVolume(preamp_log,preampTrans);
  SeGA_assembly->AddPlacedVolume(neck_log,neckTrans);
  SeGA_assembly->AddPlacedVolume(cryoBase_log,cryoBaseTrans);
  SeGA_assembly->AddPlacedVolume(cryo_log,cryoTrans);

  if (Orientation<0) 
   { DetRot.rotateX(180*deg);
     ZShift.setZ(-ZShift.z());
   }
  DetRot.rotateY(thetad);
  DetRot.rotateZ(phid);
  DetPos.setX(rd*cos(phid));
  DetPos.setY(rd*sin(phid));
  DetPos.setZ(ZShift.z());
  G4Transform3D SeGATrans = G4Transform3D(DetRot,DetPos);

  SeGA_assembly->MakeImprint(expHall_log,SeGATrans);
  doppler->SetCrysSize(Length,outerRadius,fingerRadius);
  doppler->SetDetPos(RingID, Number, rd, DetPos.z(),Orientation); 

}
