#ifndef DCGEOMETRY_H
#define DCGEOMETRY_H

// DCGeometry.hh
// 
// overall geometry specification and fridge build
//
//     DCGeometry         - geometry manager (consists of 3 HousingDetectors, a FridgeBody, and a SourceBlock)
//

#include "DCDetectors.hh"
#include "DCFridge.hh"


class DCGeometry : public G4VUserDetectorConstruction {
public:
    DCGeometry(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    virtual ~DCGeometry() {;}
    G4VPhysicalVolume* Construct() override;
    void ConstructWorldBox();
    void ConstructSDandField() override;
    void verbose(G4int v) {verbosity = v;}
private:
    
    G4int verbosity = 0;
    G4bool overlaps = false;

    // geometry component objects
    HousingDetector *detector; // detector stack
    SourceBlock *source; // source block and lead box
    FridgeBlock *fridge; // copper and HDPE layers
    
    // detector dimensions and specifications
    G4double lm_face; // half-width of square low mass detector
    G4double lm_thick; // thickness of low mass detector
    G4double hm_diameter; // diameter of cylindrical high mass detectors
    G4double hm_thick; // thickness of high mass detectors
    G4double cu_thick; // thickness of copper layers
    G4double spacing; // spacing between copper boxes
    G4double hm_offset; // offset between centers of detectors
    G4double airgap_thick; // air gap between detectors and copper boxes

    G4double cuspace1; // spacing between copper plates in fridge
    G4double cuspace2;

    G4double platethickness; // plate thickness

    G4ThreeVector source_position;

    G4double box_halfheight;
    G4double box_halfwidth;

    G4LogicalVolume *worldLV;
    G4LogicalVolume *fridgeLV;
};

#endif