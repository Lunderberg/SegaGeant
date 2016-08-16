#include "Target.hh"

#include <iostream>

#include "Target_Messenger.hh"

Target::Target(G4LogicalVolume * parent_volume)
  : parent_volume(parent_volume)
{
    Target_side_x = 50 * mm;
    Target_side_y = 50 * mm;
    Target_thickness = 0.1 * mm;
    Pos = new G4ThreeVector(0., 0., 0.);
    TargetMaterial = Materials::FindMaterial("Au");
    NStep = 20;

    sourceFrame = "";
    messenger = new Target_Messenger(this);
}

Target::~Target()
{
    delete target_limits;
    delete messenger;
}

//-----------------------------------------------------------------------------
G4VPhysicalVolume *Target::Construct()
{

    aTarget =
        new G4Box("target", Target_side_x / 2., Target_side_y / 2.,
                  Target_thickness / 2.);

    Target_log =
        new G4LogicalVolume(aTarget, TargetMaterial, "target_log", 0, 0,
                            0);
    target_limits = new G4UserLimits();
    target_limits->SetMaxAllowedStep(Target_thickness / NStep);
    Target_log->SetUserLimits(target_limits);

    Target_phys =
        new G4PVPlacement(G4Transform3D(NoRot, *Pos), Target_log, "target",
                          parent_volume, false, 0);

    G4Colour red(1.0, 0.0, 0.0);
    G4VisAttributes *Vis_6 = new G4VisAttributes(red);
    Vis_6->SetVisibility(true);
    Vis_6->SetForceSolid(false);

    Target_log->SetVisAttributes(Vis_6);

    return Target_phys;
}

//-----------------------------------------------------------------------------
void Target::setX(G4double X)
{
    Target_side_x = X;
    aTarget->SetXHalfLength(Target_side_x / 2.);
    G4cout << "----> Target side X is set to " << G4BestUnit(2. *
                                                             aTarget->GetXHalfLength
                                                             (),
                                                             "Length") <<
        G4endl;
}

//-----------------------------------------------------------------------------
void Target::setY(G4double Y)
{
    Target_side_y = Y;
    aTarget->SetYHalfLength(Target_side_y / 2.);
    G4cout << "----> Target side Y is set to " << G4BestUnit(2. *
                                                             aTarget->GetYHalfLength
                                                             (),
                                                             "Length") <<
        G4endl;
}

//-----------------------------------------------------------------------------
void Target::setZ(G4double Z)
{
    Target_thickness = Z;
    aTarget->SetZHalfLength(Target_thickness / 2.);
    target_limits->SetMaxAllowedStep(Target_thickness / NStep);
    Target_log->SetUserLimits(target_limits);
    G4cout << "----> Target thickness is set to " << G4BestUnit(2. *
                                                                aTarget->GetZHalfLength
                                                                (),
                                                                "Length")
        << G4endl;
}

//-----------------------------------------------------------------------------
void Target::setNStep(G4int n)
{
    NStep = n;
    target_limits->SetMaxAllowedStep(Target_thickness / NStep);
    Target_log->SetUserLimits(target_limits);
    G4cout << "----> Number of simulation steps in the target is set to "
        << NStep << G4endl;
}

//-----------------------------------------------------------------------------
void Target::Report()
{
    G4cout << "----> Target material set to  " <<
        Target_log->GetMaterial()->GetName() << G4endl;
    G4cout << "----> Target density:         " <<
        Target_log->GetMaterial()->GetDensity() << G4endl;
    G4cout << "----> Target side X is set to " << G4BestUnit(2. *
                                                             aTarget->GetXHalfLength
                                                             (),
                                                             "Length") <<
        G4endl;
    G4cout << "----> Target side Y is set to " << G4BestUnit(2. *
                                                             aTarget->GetYHalfLength
                                                             (),
                                                             "Length") <<
        G4endl;
    G4cout << "----> Target thickness is set to " << G4BestUnit(2. *
                                                                aTarget->GetZHalfLength
                                                                (),
                                                                "Length")
        << G4endl;
    G4cout << "----> Number of simulation steps in the target is set to "
        << NStep << G4endl;
}

//---------------------------------------------------------------------
void Target::setMaterial(G4String materialName)
{
    // search the material by its name
    TargetMaterial = Materials::FindMaterial(materialName);
    Target_log->SetMaterial(TargetMaterial);
    G4cout << "----> Target material set to     " <<
        Target_log->GetMaterial()->GetName() << G4endl;
}

//-------------------------------------------------------------------
void Target::setTargetReactionDepth(G4double depth)
{
    //  G4cout<<"\n----> The depth is "<<G4BestUnit(depth,"Length")<< G4endl;;
    target_limits->SetUserMinRange(depth);
}

//-----------------------------------------------------------------------------
void Target::SetPositionZ(G4double d)
{
    Pos->setZ(d);
    G4ThreeVector Sep;
    Sep.setX(Pos->getX());
    Sep.setY(Pos->getY());
    Sep.setZ(Pos->getZ());
    Target_phys->SetTranslation(Sep);
    d = Target_phys->GetTranslation().getZ();
    G4cout << "----> Target position is set to " << G4BestUnit(d,
                                                               "Length") <<
        G4endl;
}

//---------------------------------------------------------------------
void Target::ScaleDensity(G4double scale)
{
    // search the material by its name
    G4String name = TargetMaterial->GetName();
    G4double Z = TargetMaterial->GetZ();
    G4double A = TargetMaterial->GetA();
    G4double density = TargetMaterial->GetDensity();
    density *= scale;
    TargetMaterial = new G4Material(name, Z, A, density);
    Target_log->SetMaterial(TargetMaterial);
    G4cout << "----> Target material set to     " <<
        Target_log->GetMaterial()->GetName() << G4endl;
    G4cout << "----> Target Z set to            " <<
        Target_log->GetMaterial()->GetZ() << G4endl;
    G4cout << "----> Target mole mass set to       " <<
        Target_log->GetMaterial()->GetA() / g *
        mole << " g/mole" << G4endl;
    G4cout << "----> Target density set to         " <<
        Target_log->GetMaterial()->GetDensity() / g *
        cm3 << " g/cm3" << G4endl;

}

//---------------------------------------------------------------------
void Target::setSourceFrame(G4String sF)
{
    sourceFrame = sF;

    if (sourceFrame == "eu152_Z2707") {

        frameMaterial = Materials::FindMaterial("Al");
        frameThickness = 2.9 * mm;
        frameInnerRadius = 3.8 * cm / 2.0;
        frameOuterRadius = 5.4 * cm / 2.0;
        tapeMaterial = Materials::FindMaterial("G4_POLYETHYLENE");
        tapeThickness = 0.012 * cm;

        euFrame =
            new G4Tubs("euFrame", frameInnerRadius, frameOuterRadius,
                       frameThickness / 2., 0., 360. * deg);
        euFrame_log =
            new G4LogicalVolume(euFrame, frameMaterial, "euFrame_log", 0,
                                0, 0);
        euFrame_phys =
            new G4PVPlacement(G4Transform3D(NoRot, *Pos), euFrame_log,
                              "euFrame", parent_volume, false, 0);

        euTape =
            new G4Tubs("euTape", 0., frameInnerRadius, tapeThickness / 2.,
                       0., 360. * deg);
        euTape_log =
            new G4LogicalVolume(euTape, tapeMaterial, "euTape_log", 0, 0,
                                0);
        euTape_phys =
            new G4PVPlacement(G4Transform3D(NoRot, *Pos), euTape_log,
                              "euTape", parent_volume, false, 0);

    } else if (sourceFrame == "cs137_E2879") {

        frameMaterial = Materials::FindMaterial("Al");
        frameThickness = 0.7 * mm;
        frameInnerRadius = 2.54 * 13. / 16. * cm / 2.0;
        frameOuterRadius = 2.54 * cm / 2.0;
        tapeMaterial = Materials::FindMaterial("G4_POLYETHYLENE");
        tapeThickness = 0.016 * cm;

        csFrame =
            new G4Tubs("csFrame", frameInnerRadius, frameOuterRadius,
                       frameThickness / 2., 0., 360. * deg);
        csFrame_log =
            new G4LogicalVolume(csFrame, frameMaterial, "csFrame_log", 0,
                                0, 0);
        csFrame_phys =
            new G4PVPlacement(G4Transform3D(NoRot, *Pos), csFrame_log,
                              "csFrame", parent_volume, false, 0);

        G4ThreeVector ringPos(0., 0.,
                              frameOuterRadius - frameInnerRadius +
                              frameThickness / 2.0);

        csRing =
            new G4Tubs("csRing", frameOuterRadius - frameThickness,
                       frameOuterRadius,
                       frameOuterRadius - frameInnerRadius, 0.,
                       360. * deg);
        csRing_log =
            new G4LogicalVolume(csRing, frameMaterial, "csRing_log", 0, 0,
                                0);
        csRing_phys =
            new G4PVPlacement(G4Transform3D(NoRot, ringPos), csRing_log,
                              "csRing", parent_volume, false, 0);

        // Let's assume Mylar = Kapton = polyethylene is good enough.
        csTape =
            new G4Tubs("csTape", 0., frameInnerRadius, tapeThickness / 2.,
                       0., 360. * deg);
        csTape_log =
            new G4LogicalVolume(csTape, tapeMaterial, "csTape_log", 0, 0,
                                0);
        csTape_phys =
            new G4PVPlacement(G4Transform3D(NoRot, *Pos), csTape_log,
                              "csTape", parent_volume, false, 0);

    }

    G4cout << "----> Source frame is set to " << sourceFrame << G4endl;
}
