#ifndef DCACTIONINITIALIZATION_H
#define DCACTIONINITIALIZATION_H

// DCActionInitialization.hh
// 
//     DCInitialization   - G4VUserActionInitialization that uses 
//                               DCPrimaryGenerator
//


#include "G4VUserActionInitialization.hh"
#include "G4ThreeVector.hh"


class DCInitialization : public G4VUserActionInitialization {
public:
    DCInitialization(G4int, G4int, G4double, G4double, G4double, G4bool);
    ~DCInitialization() {;}
    void BuildForMaster() const override;
    void Build() const override;
private:
    G4int Z;
    G4int A;
    G4ThreeVector source_position;
    G4bool test;
};



#endif