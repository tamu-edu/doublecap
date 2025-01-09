
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "DCDetectors.hh"


DCDetector::DCDetector(
    G4double lm_face,
    G4double lm_thick,
    G4double hm_diameter,
    G4double hm_thick,
    G4double cu_thick,
    G4double spacing,
    G4double airgap,
    G4LogicalVolume *motherVolume
    ) :
    lm_face(lm_face),
    lm_thick(lm_thick),
    hm_diameter(hm_diameter),
    hm_thick(hm_thick),
    cu_thick(cu_thick),
    spacing(spacing),
    airgap(airgap)
{
    Construct();
}


void DCDetector::Construct() {


    G4Material *silicon = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    G4Material *germanium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ge");
    G4Material *cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
    G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"); // G4_AIR

    if (verbosity > 1) {
        G4cout << "got materials" << G4endl;
    }

    hm_offset = lm_thick/2. + hm_thick/2. + spacing;
    box_halfheight = hm_offset + hm_thick/2. + airgap;
    box_halfwidth = hm_diameter/2. + airgap;

    if (verbosity > 0) {
        G4cout << "box_halfheight = " << box_halfheight/cm << " cm" << G4endl;
        G4cout << "box_halfwidth = " << box_halfwidth/cm << " cm" << G4endl;
    }

    //highmass1 = new DCDetector(G4ThreeVector(0.,0.,hm_offset), hm_diameter, hm_thick, cu_thick, airgap, 'c', worldLogical, "highmass1");
    //highmass2 = new DCDetector(G4ThreeVector(0.,0.,-hm_offset), hm_diameter, hm_thick, cu_thick, airgap, 'c', worldLogical, "highmass2");

    // boxes delineating copper region
    G4Box *airgapBox = new G4Box("det_airgap",\
            box_halfwidth, 
            box_halfwidth, 
            box_halfheight);

    G4Box *copper = new G4Box("det_copper",\
            box_halfwidth + cu_thick, 
            box_halfwidth + cu_thick, 
            box_halfheight + cu_thick);

    //case 'b': // x1 = width, x2 = thickness
    G4Box *lm = new G4Box("lowmass", lm_face/2., lm_face/2., lm_thick/2.);

    //case 'c': // x1 = diameter, x2 = thickness
    G4Tubs *hm1 = new G4Tubs("highmass", 0., hm_diameter/2., hm_thick/2., 0.*deg, 360.*deg);

    copperLV = new G4LogicalVolume(copper, cu, "det_copper");
    airgapLV = new G4LogicalVolume(airgapBox, air, "det_airgap");
    lmLV = new G4LogicalVolume(lm, silicon, "lmLV");
    hmLV = new G4LogicalVolume(hm1, germanium, "hmLV");


    if (verbosity > 1) {
        G4cout << "defined all LVs" << G4endl;
    }
}


void DCDetector::PlaceVolumes(G4LogicalVolume *motherLV) {

    G4ThreeVector origin = G4ThreeVector(0.,0.,0.);
    
    new G4PVPlacement(0, origin, copperLV, "det_copper", motherLV, false, 0, overlaps);
    new G4PVPlacement(0, origin, airgapLV, "det_airgap", copperLV, false, 0, overlaps);

    new G4PVPlacement(0, G4ThreeVector(0., 0., hm_offset), hmLV, "highmass1", airgapLV, false, 0, overlaps);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -hm_offset), hmLV, "highmass2", airgapLV, false, 0, overlaps);
    
    if (verbosity > 1) {
        G4cout << "placed almost all LVs" << G4endl;
    }
    new G4PVPlacement(0, origin, lmLV, "lowmass", airgapLV, false, 0, overlaps);
}




////////////////////////////////////////////////////////////////////////////////


SourceBlock::SourceBlock(G4double sourcesize, G4double sourcez, G4LogicalVolume *motherLV) : 
    sourcesize(sourcesize),
    sourcez(sourcez),
    motherLV(motherLV)
{
    Construct();
}


void SourceBlock::Construct() {
    G4Material *cf = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cf");

    G4Box *sourcebox = new G4Box("source",\
            sourcesize/2., sourcesize/2., sourcesize/2.);

    if (verbosity > 1) {
        G4cout << "defined source box" << G4endl;
    }

    sourceLV = new G4LogicalVolume(sourcebox, cf, "source");

    if (verbosity > 1) {
        G4cout << "defined source LV" << G4endl;
    }

    new G4PVPlacement(0, G4ThreeVector(0.,0.,sourcez), sourceLV, "source",\
            motherLV, false, 0, overlaps);

    if (verbosity > 1) {
        G4cout << "placed source" << G4endl;
    }
}

