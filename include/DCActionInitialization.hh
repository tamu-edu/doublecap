#ifndef DCACTIONINITIALIZATION_H
#define DCACTIONINITIALIZATION_H

// DCActionInitialization.hh
// 
//     DCInitialization   - G4VUserActionInitialization that uses 
//                               DCPrimaryGenerator
//


#include "G4VUserActionInitialization.hh"
#include "G4ThreeVector.hh"
#include "DCGeometry.hh"


class DCInitialization : public G4VUserActionInitialization {
public:
    DCInitialization(G4int, G4int, DCGeometry*, G4int, G4String);
    ~DCInitialization() {;}
    void BuildForMaster() const override;
    void Build() const override;
private:
    G4int Z;
    G4int A;
    DCGeometry *geometry;
    G4int mode;
    G4String filename;
};



#endif