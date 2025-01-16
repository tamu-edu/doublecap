#ifndef DCGEOMETRY_H
#define DCGEOMETRY_H

// DCGeometry.hh
// 
// overall geometry specification and fridge build
//
//     DCGeometry         - geometry manager (consists of 3 HousingDetectors, a FridgeBody, and a SourceBlock)
//

#include "DCDetectors.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"


class DCGeometry : public G4VUserDetectorConstruction {
public:
    DCGeometry(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double);
    virtual ~DCGeometry() {;}
    G4VPhysicalVolume* Construct() override;
    void ComputeParams();
    void ConstructWorldBox();
    void ConstructShield();
    void SetVisAttributes();
    void ConstructSDandField() override;
    G4double GetSourceZ() const {return sourcez;}
    G4double GetSourceSize() const {return sourcesize;}
    G4double GetLMFace() const {return lm_face;}
    G4double GetLMThickness() const {return lm_thick;}
    void verbose(G4int v) {verbosity = v;}
private:
    
    G4int verbosity = 0;
    G4bool overlaps = false;

    // geometry component objects
    DCDetector *detector; // detector stack
    SourceBlock *source; // source block and lead box
    G4LogicalVolume *shield; // lead shield
    
    // detector dimensions and specifications
    G4double lm_face; // half-width of square low mass detector
    G4double lm_thick; // thickness of low mass detector
    G4double hm_diameter; // diameter of cylindrical high mass detectors
    G4double hm_thick; // thickness of high mass detectors
    G4double cu_thick; // thickness of copper layers
    G4double spacing; // spacing between copper boxes
    G4double airgap; // air gap between detectors and copper boxes

    G4double sourcez; // Z position of center of source box
    G4double sourcesize; // side length of source cube
    
    G4double leadz; // Z position of center of lead shield
    G4double leadthickness; // thickness (height) of lead cylinder
    G4double leadradius; // radius of lead shield

    G4double pethickness; // distance between outside of detector box and lead shield

    // dimensions of interior of detector box
    G4double box_halfheight;
    G4double box_halfwidth;

    // dimensions of world box
    G4double world_halfheight;
    G4double world_halfwidth = 4.*cm;

    G4LogicalVolume *worldLV;
    G4LogicalVolume *leadLV;

    G4Tubs* Cylinder(G4double thickness, G4double radius, G4String name) {return new G4Tubs(name, 0., radius, thickness/2., 0.*deg, 360.*deg);}
};

#endif