
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

DCGeometry::DCGeometry(G4double lm_face, G4double lm_thick, G4double hm_diameter, G4double hm_thick, G4double cu_thick, G4double spacing, G4double airgap_thick, G4double sourcex, G4double sourcey, G4double sourcez, G4double cuspace1, G4double cuspace2, G4double platethickness) : 
    G4VUserDetectorConstruction(),
    lm_face(lm_face),
    lm_thick(lm_thick),
    hm_diameter(hm_diameter),
    hm_thick(hm_thick),
    cu_thick(cu_thick),
    spacing(spacing),
    airgap_thick(airgap_thick),
    cuspace1(cuspace1), cuspace2(cuspace2), platethickness(platethickness),
    source_position(G4ThreeVector(sourcex, sourcey, sourcez))
{;}


G4VPhysicalVolume* DCGeometry::Construct() {

    ConstructWorldBox();

    detector = new HousingDetector(lm_face, lm_thick, hm_diameter, hm_thick, cu_thick, spacing, airgap_thick, worldLV);

    G4double box_halfheight = detector->GetBoxHalfHeight();
    G4double box_halfwidth = detector->GetBoxHalfWidth();

    source = new SourceBlock(source_position, worldLV);

    // shift of top of MXC plate from origin
    G4double plateshift = lm_thick/2. + spacing + hm_thick + airgap_thick + cu_thick + 1000.*nm;
    fridge = new FridgeBlock(cuspace1, cuspace2, platethickness, plateshift, box_halfheight + cu_thick, box_halfwidth + cu_thick, worldLV);

    if (verbosity > 0) {
        G4cout << "did the fridge definition" << G4endl;
    }

    G4Colour lowmassColor = G4Color(1., 0.1, 1.);
    G4Colour highmassColor = G4Color(0.1, 1., 1.);
    G4Colour sourceColor = G4Color(1., 1., 1.);
    G4Colour copperColor = G4Color(0.88, 0.42, 0.09);
    G4Colour shieldColor = G4Color(0.7, 0.7, 0.7);


    worldLV->SetVisAttributes(G4VisAttributes(false));

    detector->GetLowMass()->SetVisAttributes(G4VisAttributes(lowmassColor));
    detector->GetHighMass()->SetVisAttributes(G4VisAttributes(highmassColor));
    detector->GetCopper()->SetVisAttributes(G4VisAttributes(copperColor));
    detector->GetAirgap()->SetVisAttributes(G4VisAttributes(copperColor));
    source->GetBlock()->SetVisAttributes(G4VisAttributes(sourceColor));
    if (verbosity > 0) {
        G4cout << "doing fridge plate color" << G4endl;
    }
    fridge->GetPlate()->SetVisAttributes(G4VisAttributes(copperColor));
    if (verbosity > 0) {
        G4cout << "doing for " << fridge->GetNumShields() << " shields" << G4endl;
    }
    for (int i = 0; i < fridge->GetNumShields(); ++i) {
        fridge->GetShield(i)->SetVisAttributes(G4VisAttributes(shieldColor));
    }


    return new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), worldLV, "World", nullptr, false, 0, overlaps);
}



void DCGeometry::ConstructWorldBox() {
    G4Material *galactic = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"); //G4_AIR
    G4Box *worldBox = new G4Box("WorldBox", 30.*cm, 30.*cm, 50.*cm);
    worldLV = new G4LogicalVolume(worldBox, galactic, "World");
}




void DCGeometry::ConstructSDandField() {

    G4SDManager::GetSDMpointer()->SetVerboseLevel(2);
        
    auto lm = new G4MultiFunctionalDetector("lm");
    G4SDManager::GetSDMpointer()->AddNewDetector(lm);
    G4VPrimitiveScorer *lmprim = new G4PSEnergyDeposit("edep");
    lm->RegisterPrimitive(lmprim);
    SetSensitiveDetector("lmLV", lm);

    auto hm = new G4MultiFunctionalDetector("hm");
    G4SDManager::GetSDMpointer()->AddNewDetector(hm);
    G4VPrimitiveScorer *hmprim = new G4PSEnergyDeposit("edep");
    hm->RegisterPrimitive(hmprim);
    SetSensitiveDetector("hmLV", hm);

    
    auto dcdet = new DCSensitiveDetector("dchits","DCHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(dcdet);
    SetSensitiveDetector("hmLV", dcdet);
    return;
}

