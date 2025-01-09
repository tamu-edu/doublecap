

#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "DCFridge.hh"

FridgeBlock::FridgeBlock(G4double cuspace1, G4double cuspace2, G4double platethickness, G4double plateshift, G4double box_halfheight, G4double box_halfwidth, G4LogicalVolume *motherLV) : 
    cuspace1(cuspace1),
    cuspace2(cuspace2), 
    platethickness(platethickness),
    plateshift(plateshift),
    box_halfheight(box_halfheight),
    box_halfwidth(box_halfwidth),
    motherLV(motherLV)
{
    G4Material *cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");

    G4Tubs *plateCylinder = Cylinder(platethickness, "plate");
    plateLV = new G4LogicalVolume(plateCylinder, cu, "plate");
    if (verbosity > 0) {
        G4cout << "got plateLV" << G4endl;
    }

    std::vector<G4double> plate_zs = {
        -plateshift - platethickness/2., 
        -plateshift + cuspace1 + platethickness/2.,
        -plateshift + cuspace1 + platethickness + cuspace2 + platethickness/2.
    };

    G4int i = 0;
    for (G4double z : plate_zs) {
        new G4PVPlacement(0, G4ThreeVector(0., 0., z), plateLV, "plate", motherLV, false, ++i, overlaps);
    }
}




