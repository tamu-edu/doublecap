#ifndef DCACTIONINITIALIZATION_H
#define DCACTIONINITIALIZATION_H

// DCActionInitialization.hh
//
// includes initialization classes:
// 
//     DCInitialization   - G4VUserActionInitialization that uses 
//                               DCPrimaryGenerator
//
//     DCPrimaryGenerator - G4VUserPrimaryGeneratorAction that implements 
//                               empty GeneratePrimaries() method
//


#include "G4VUserActionInitialization.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Ions.hh"
#include "G4SystemOfUnits.hh"


class DCInitialization : public G4VUserActionInitialization {
public:
    DCInitialization(G4int, G4int, G4double, G4double, G4double);
    ~DCInitialization() {;}
    void Build() const;
private:
    G4int Z;
    G4int A;
    G4ThreeVector source_position;
};


class DCPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    DCPrimaryGenerator(G4int, G4int, G4ThreeVector);
    ~DCPrimaryGenerator() {;}
    void GeneratePrimaries(G4Event *);
    void GetParticleDefinition();
    void SetParticlePositionMomentum();
private:
    G4double sideLength = 1.*mm;
    G4ParticleGun *gun;
    G4ParticleDefinition *theParticle;
    G4int Z;
    G4int A;
    G4ThreeVector source_position;
    G4double l; // side length of source cube
};


#endif