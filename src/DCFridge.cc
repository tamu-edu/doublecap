

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
    shields = std::vector<G4LogicalVolume*>(); // upper and lower shields may have different thicknesses

    std::vector<G4double> shieldThicknesses = {5.*cm, 2.*cm};

    std::vector<G4double> shieldPositions = {
        -plateshift + cuspace1 + platethickness + cuspace2/2., // center upper shield between plates
        -plateshift + box_halfheight + cuspace1/2. // center lower shield between middle plate and top of detector boxplates
    };
    if (verbosity > 0) {
        G4cout << "defined shield positions" << G4endl;
    }

    G4Material *cu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
    G4Material *hdpe = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");


    G4Tubs *plateCylinder = Cylinder(platethickness, "plate");
    plateLV = new G4LogicalVolume(plateCylinder, cu, "plate");
    if (verbosity > 0) {
        G4cout << "got plateLV" << G4endl;
    }

    G4Tubs *uppershield = Cylinder(shieldThicknesses[0], "uppershield");
    G4Tubs *lowershield = Cylinder(shieldThicknesses[1], "lowershield");
    G4LogicalVolume *uppershieldLV = new G4LogicalVolume(uppershield, hdpe, "uppershield");
    G4LogicalVolume *lowershieldLV = new G4LogicalVolume(lowershield, hdpe, "lowershield");
    if (verbosity > 1) {
        G4cout << "created shield LVs" << G4endl;
    }

    shields.push_back(uppershieldLV);
    shields.push_back(lowershieldLV);
    if (verbosity > 0) {
        G4cout << "added shield LVs to vector" << G4endl;
        if (verbosity > 1) {
            G4cout << "shields.size() = " << shields.size() << G4endl;
            G4cout << "shieldPositions[0] = " << shieldPositions[0]/cm << " cm" << G4endl;
            G4cout << "shieldPositions[1] = " << shieldPositions[1]/cm << " cm" << G4endl;
        }
    }

    for (int i = 0; i < shields.size(); ++i) {
        new G4PVPlacement(0, G4ThreeVector(0., 0., shieldPositions[i]), shields[i], "shield", motherLV, false, i+1, overlaps);
    }
    if (verbosity > 0) {
        G4cout << "placed shields in world" << G4endl;
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




