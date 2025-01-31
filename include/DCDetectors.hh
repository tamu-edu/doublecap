#ifndef DCDETECTORS_H
#define DCDETECTORS_H

// DCDetectors.hh
// 
// includes classes for DC geometry construction
// 
//     DCDetector         - G4VUserDetectorConstruction that creates 
//                               silicon detector inside copper box
// 
//     SourceBlock             - G4VUserDetectorConstruction that creates 
//                               source block of specified dimensions at 
//                               specified location surrounded by lead shield



#include "G4VUserDetectorConstruction.hh"

class DCDetector {
public:
    DCDetector(G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4LogicalVolume*);
    virtual ~DCDetector() {;}
    void Construct();
    void PlaceVolumes(G4LogicalVolume*);
    G4LogicalVolume* GetCopper() const {return copperLV;}
    G4LogicalVolume* GetAirgap() const {return airgapLV;}
    G4LogicalVolume* GetLowMass() const {return lmLV;}
    G4LogicalVolume* GetHighMass() const {return hmLV;}
    G4double GetBoxHalfHeight() const {return box_halfheight;}
    G4double GetBoxHalfWidth() const {return box_halfwidth;}
    void verbose(G4int v) {verbosity = v;}
private:
    
    G4int verbosity = 0;
    G4bool overlaps = false;
    
    // component volumes
    G4LogicalVolume *copperLV;
    G4LogicalVolume *airgapLV;
    G4LogicalVolume *lmLV;
    G4LogicalVolume *hmLV;

    // dimensions
    G4double lm_face; // half-width of square low mass detector
    G4double lm_thick; // thickness of low mass detector
    G4double hm_diameter; // diameter of cylindrical high mass detectors
    G4double hm_thick; // thickness of high mass detectors
    G4double cu_thick; // thickness of copper layers
    G4double spacing; // spacing between silicon
    G4double airgap; // air gap between detectors and copper boxes

    G4double hm_offset; // offset between centers of detectors
    G4double box_halfheight; // half-height of interior of copper box
    G4double box_halfwidth; // half-width of interior of copper box
};


class SourceBlock {
public:
    SourceBlock(G4double, G4double, G4LogicalVolume*);
    virtual ~SourceBlock() {;}
    void Construct();
    G4LogicalVolume* GetBlock() const {return sourceLV;}
    void verbose(G4int v) {verbosity = v;}
private:

    G4int verbosity = 0;
    G4bool overlaps = false;
    
    G4double sourcez; // z coord of center
    G4double sourcesize; // side length

    G4LogicalVolume* sourceLV;
    G4LogicalVolume *motherLV;
};


#endif
