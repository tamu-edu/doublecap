#ifndef DCPRIMARYGENERATOR_HH
#define DCPRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Ions.hh"
#include "G4SystemOfUnits.hh"

class DCGeometry;

class DCPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    DCPrimaryGenerator(G4int, G4int, DCGeometry*);
    ~DCPrimaryGenerator() {;}
    void GeneratePrimaries(G4Event*);
    void GetParticleDefinition();
    void SetParticlePositionMomentum();
    void CalculateParams();
private:
    G4int verbosity = 0;
    DCGeometry *fGeometry;
    G4double sourcesize; // side length of source cube
    G4double sourcez;
    G4ParticleGun *gun;
    G4ParticleDefinition *fParticle;
    G4int Z;
    G4int A;
};


class CaptureGenerator : public G4VUserPrimaryGeneratorAction {
public:
    CaptureGenerator(DCGeometry*);
    ~CaptureGenerator() {;}
    void GeneratePrimaries(G4Event*);
    void SetParticlePosition();
    void SetParticleDefinition();
    void CalculateParams();
private:
    G4double dimx;
    G4double dimy;
    G4double dimz;
    DCGeometry *fGeometry;
    G4ParticleDefinition *fParticle;
    G4ParticleGun *gun;
};


class TestGenerator : public G4VUserPrimaryGeneratorAction {
public:
    TestGenerator();
    ~TestGenerator() {;}
    void GeneratePrimaries(G4Event *);
private:
    G4ParticleGun *gun;
    G4String pname = "neutron";
    G4double penergy = 0.025*eV;
};




#endif
