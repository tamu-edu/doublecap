#ifndef DCFRIDGE_H
#define DCFRIDGE_H

// DCFridge.hh - definition of Bluefors fridge interior (ish) class FridgeBlock
// 
// consists of layers of copper and polyethylene

#include <vector>

class FridgeBlock {
public:
    FridgeBlock(G4double, G4double, G4double, G4double, G4double, G4double, G4LogicalVolume*);
    virtual ~FridgeBlock() {;}
    G4LogicalVolume* GetPlate() const {return plateLV;}
    void verbose(G4int v) {verbosity = v;}
private:
    
    G4int verbosity = 0;
    G4bool overlaps = false;
    
    G4LogicalVolume *motherLV;
    G4LogicalVolume *plateLV;

    G4double cuspace1; // spacing between MXC and cold plate
    G4double cuspace2; // spacing between 4K and cold plate
    G4double platethickness; // thickness of plates
    G4double plateshift; // shift of top of MXC plate from center
    G4double plateradius = 20.*cm; // radius of plates

    // THESE INCLUDE THE COPPER THICKNESS
    G4double box_halfheight;
    G4double box_halfwidth;

    G4Tubs* Cylinder(G4double thickness, G4String name) {return new G4Tubs(name, 0., plateradius, thickness/2., 0.*deg, 360.*deg);}
};



#endif