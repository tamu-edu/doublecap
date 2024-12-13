
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


HousingDetector::HousingDetector(
    G4double lm_face,
    G4double lm_thick,
    G4double hm_diameter,
    G4double hm_thick,
    G4double cu_thick,
    G4double spacing,
    G4double airgap_thick,
    G4LogicalVolume *motherVolume
    ) : 
    G4VUserDetectorConstruction(),
    motherLV(motherVolume), 
    lm_face(lm_face),
    lm_thick(lm_thick),
    hm_diameter(hm_diameter),
    hm_thick(hm_thick),
    cu_thick(cu_thick),
    spacing(spacing),
    airgap_thick(airgap_thick)
{
    Construct();
}


G4VPhysicalVolume* HousingDetector::Construct() {

    G4ThreeVector origin = G4ThreeVector(0.,0.,0.);

    G4Material *bulk = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    G4Material *cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
    G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"); // G4_AIR

    if (verbosity > 0) {
        G4cout << "got materials" << G4endl;
    }


    hm_offset = lm_thick/2. + hm_thick/2. + spacing;
    box_halfheight = hm_offset + hm_thick/2. + airgap_thick;
    box_halfwidth = hm_diameter/2. + airgap_thick;



    //highmass1 = new HousingDetector(G4ThreeVector(0.,0.,hm_offset), hm_diameter, hm_thick, cu_thick, airgap_thick, 'c', worldLogical, "highmass1");
    //highmass2 = new HousingDetector(G4ThreeVector(0.,0.,-hm_offset), hm_diameter, hm_thick, cu_thick, airgap_thick, 'c', worldLogical, "highmass2");

    // boxes delineating copper region
    G4Box *airgap = new G4Box("det_airgap",\
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
    airgapLV = new G4LogicalVolume(airgap, air, "det_airgap");
    lmLV = new G4LogicalVolume(lm, bulk, "lmLV");
    hmLV = new G4LogicalVolume(hm1, bulk, "hmLV");


    if (verbosity > 0) {
        G4cout << "defined all LVs" << G4endl;
    }

    
    new G4PVPlacement(0, origin, copperLV, "det_copper", motherLV, false, 0, overlaps);
    new G4PVPlacement(0, origin, airgapLV, "det_airgap", copperLV, false, 0, overlaps);

    new G4PVPlacement(0, G4ThreeVector(0., 0., hm_offset), hmLV, "highmass1", airgapLV, false, 0, overlaps);
    new G4PVPlacement(0, G4ThreeVector(0., 0., -hm_offset), hmLV, "highmass2", airgapLV, false, 0, overlaps);
    
    if (verbosity > 0) {
        G4cout << "placed almost all LVs" << G4endl;
    }
    return new G4PVPlacement(0, origin, lmLV, "lowmass", airgapLV, false, 0, overlaps);
}




////////////////////////////////////////////////////////////////////////////////


SourceBlock::SourceBlock(G4ThreeVector center, G4LogicalVolume *motherLogical) : 
    center(center),
    motherLogical(motherLogical)
{
    Construct();
}


G4VPhysicalVolume* SourceBlock::Construct() {
    G4Material *cf = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cf");
    G4Material *pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");

    G4Box *leadbox = new G4Box("lead",\ 
            swidth/2. + pbthickness, 
            swidth/2. + pbthickness, 
            swidth/2. + pbthickness);
    G4Box *sourcebox = new G4Box("source",\ 
            swidth/2., swidth/2., sthick/2.);


    if (verbosity > 0) {
        G4cout << "defined source boxes" << G4endl;
    }

    G4LogicalVolume *leadLogical = new G4LogicalVolume(leadbox, pb, "lead");
    sourceLogical = new G4LogicalVolume(sourcebox, cf, "source");


    if (verbosity > 0) {
        G4cout << "defined source LVs" << G4endl;
    }

    new G4PVPlacement(0, center, leadLogical, "lead",\ 
            motherLogical, false, 0, overlaps);

    if (verbosity > 0) {
        G4cout << "placed lead" << G4endl;
    }

    return new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), sourceLogical, "source",\ 
            leadLogical, false, 0, overlaps);
}

