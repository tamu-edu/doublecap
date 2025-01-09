#ifndef DCPRIMARYGENERATOR_HH
#define DCPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Ions.hh"
#include "G4SystemOfUnits.hh"


class DCPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    DCPrimaryGenerator(G4int, G4int, G4double, G4double);
    ~DCPrimaryGenerator() {;}
    void GeneratePrimaries(G4Event *);
    void GetParticleDefinition();
    void SetParticlePositionMomentum();
private:
    G4double sourcesize; // side length of source cube
    G4double sourcez;
    G4ParticleGun *gun;
    G4ParticleDefinition *theParticle;
    G4int Z;
    G4int A;
};


class TestGenerator : public G4VUserPrimaryGeneratorAction {
public:
    TestGenerator();
    ~TestGenerator() {;}
    void GeneratePrimaries(G4Event *);
private:
    G4ParticleGun *gun;
    G4String pname = "neutron";
    G4double penergy = 0.05*eV;
};




#endif
