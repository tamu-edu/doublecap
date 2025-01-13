
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VisAttributes.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4SDManager.hh"

#include "DCGeometry.hh"
#include "DCSensitiveDetector.hh"

DCGeometry::DCGeometry(G4double lm_face, G4double lm_thick, G4double hm_diameter, G4double hm_thick, G4double cu_thick, G4double spacing, G4double airgap, G4double pethickness, G4double leadthickness, G4double leadradius, G4double sourcesize) : 
    G4VUserDetectorConstruction(),
    lm_face(lm_face),
    lm_thick(lm_thick),
    hm_diameter(hm_diameter),
    hm_thick(hm_thick),
    cu_thick(cu_thick),
    spacing(spacing),
    airgap(airgap),
    pethickness(pethickness),
    leadthickness(leadthickness),
    leadradius(leadradius),
    sourcesize(sourcesize)
{
    detector = new DCDetector(lm_face, lm_thick, hm_diameter, hm_thick, cu_thick, spacing, airgap, worldLV);

    ComputeParams();
}


G4VPhysicalVolume* DCGeometry::Construct() {


    ConstructWorldBox();

    detector->PlaceVolumes(worldLV);

    source = new SourceBlock(sourcesize, sourcez, worldLV);

    ConstructShield();

    SetVisAttributes();

    return new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), worldLV, "World", nullptr, false, 0, overlaps);
}


void DCGeometry::ComputeParams() {

    if (verbosity > 0) {
        G4cout << "Starting DCGeometry::ComputeParams()" << G4endl;
    }

    box_halfheight = detector->GetBoxHalfHeight();
    box_halfwidth = detector->GetBoxHalfWidth();

    leadz = box_halfheight + cu_thick + pethickness + leadthickness/2.;
    sourcez = leadz + leadthickness/2. + sourcesize/2. + 100.*nm;

    world_halfheight = sourcez + sourcesize/2. + 100.*nm;

    if (verbosity > 0) {
        G4cout << "DCGeometry: parameters computed." << G4endl;
        if (verbosity > 1) {
            G4cout << "    box_halfheight: " << box_halfheight/cm << " cm" << G4endl;
            G4cout << "    box_halfwidth: " << box_halfwidth/cm << " cm" << G4endl;
            G4cout << "    leadz: " << leadz/cm << " cm" << G4endl;
            G4cout << "    sourcez: " << sourcez/cm << " cm" << G4endl;
            G4cout << "    world_halfheight: " << world_halfheight/cm << " cm" << G4endl << G4endl;
        }
    }
}


void DCGeometry::ConstructWorldBox() {
    G4Material *hdpe = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Box *worldBox = new G4Box("WorldBox", world_halfwidth, world_halfwidth, world_halfheight);
    worldLV = new G4LogicalVolume(worldBox, hdpe, "World");
    if (verbosity > 1) {
        G4cout << "defined world LV" << G4endl;
    }
}


void DCGeometry::SetVisAttributes() {

    G4Colour lowmassColor = G4Color(1., 0.1, 1.);
    G4Colour highmassColor = G4Color(0.1, 1., 1.);
    G4Colour sourceColor = G4Color(0.1, 1., 0.1);
    G4Colour copperColor = G4Color(0.88, 0.42, 0.09);
    G4Colour leadColor = G4Color(0.8, 0.8, 0.8);

    worldLV->SetVisAttributes(G4VisAttributes(false));

    detector->GetLowMass()->SetVisAttributes(G4VisAttributes(lowmassColor));
    detector->GetHighMass()->SetVisAttributes(G4VisAttributes(highmassColor));
    detector->GetCopper()->SetVisAttributes(G4VisAttributes(copperColor));
    detector->GetAirgap()->SetVisAttributes(G4VisAttributes(copperColor));
    source->GetBlock()->SetVisAttributes(G4VisAttributes(sourceColor));
    leadLV->SetVisAttributes(G4VisAttributes(leadColor));
}

void DCGeometry::ConstructShield() {
    G4Material *pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    G4Tubs *leadCylinder = Cylinder(leadthickness, leadradius, "lead");
    leadLV = new G4LogicalVolume(leadCylinder, pb, "lead");
    new G4PVPlacement(0, G4ThreeVector(0.,0.,leadz), leadLV, "lead",\
                worldLV, false, 0, overlaps);
    if (verbosity > 1) {
        G4cout << "placed shield volume" << G4endl;
    }
}


void DCGeometry::ConstructSDandField() {

    G4SDManager::GetSDMpointer()->SetVerboseLevel(verbosity);

    auto dcdet_hm = new DCSensitiveDetector("hmdchits","DCHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(dcdet_hm);
    SetSensitiveDetector("hmLV", dcdet_hm);

    auto dcdet_lm = new DCSensitiveDetector("lmdchits", "DCHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(dcdet_lm);
    SetSensitiveDetector("lmLV", dcdet_lm);

    if (verbosity > 1) {
        G4cout << "defined DCSensitiveDetectors" << G4endl;
    }
}

