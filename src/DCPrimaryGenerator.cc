


#include "DCUtilities.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCSensitiveDetector.hh"
#include "DCGeometry.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"

DCPrimaryGenerator::DCPrimaryGenerator(G4int Z, G4int A, DCGeometry *geom) : 
    G4VUserPrimaryGeneratorAction(), 
    gun(nullptr), 
    fParticle(nullptr) , 
    Z(Z), A(A),
    fGeometry(geom)
{
    gun = new G4ParticleGun(1);
}

void DCPrimaryGenerator::CalculateParams() {
    sourcesize = fGeometry->GetSourceSize();
    sourcez = fGeometry->GetSourceZ();
}


void DCPrimaryGenerator::GetParticleDefinition() {
    // define particle type for gun and set energy
    if (verbosity > 0) {
        G4cout << "Starting DCPrimaryGenerator::GetParticleDefinition" << G4endl;
    }
    G4double en;
    if (Z == 0 && A == 0) {
        fParticle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        en = 0.05*eV;
        if (verbosity > 1) {
            G4cout << "setting particle to neutron" << G4endl;
        }
    } else {
        fParticle = G4IonTable::GetIonTable()->GetIon(Z, A);
        en = 0.;
        if (verbosity > 1) {
            G4cout << "setting particle to Z = " << Z << ", A = " << A << G4endl;
        }
    }
    gun->SetParticleDefinition(fParticle);
    gun->SetParticleEnergy(en);
    if (verbosity > 1) {
        G4cout << "DCPrimaryGenerator finished assigning particle" << G4endl;
    }
}

void DCPrimaryGenerator::SetParticlePositionMomentum() {
    if (verbosity > 0) {
        G4cout << "Starting DCPrimaryGenerator::SetParticlePositionMomentum" << G4endl;
    }

    if (Z == 0 && A == 0) { // particle = neutron: randomize direction but not position
        
        gun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
        gun->SetParticleMomentumDirection(RandomDirection()); 
        
    } else {
        G4double q1 = G4UniformRand();
        G4double q2 = G4UniformRand();
        G4double q3 = G4UniformRand();
        if (verbosity > 2) {
            G4cout << "generated q1 = " << q1/mm << " mm" << G4endl;
            G4cout << "generated q2 = " << q2/mm << " mm" << G4endl;
            G4cout << "generated q3 = " << q3/mm << " mm" << G4endl;
        }
        gun->SetParticlePosition( 
            G4ThreeVector((q1 - 0.5)*sourcesize,
                        (q2 - 0.5)*sourcesize,
                        sourcez + (q3 - 0.5)*sourcesize 
        ));
    }
    if (verbosity > 1) {
        G4cout << "Ending DCPrimaryGenerator::SetParticlePositionMomentum" << G4endl;
    }
}

void DCPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    if (verbosity > 1) {
        G4cout << "starting GeneratePrimaries" << G4endl;
    }
    if (!fParticle) {
        CalculateParams();
        GetParticleDefinition();
    }
    SetParticlePositionMomentum();
    gun->GeneratePrimaryVertex(anEvent);
    if (verbosity > 1) {
        G4cout << "ending GeneratePrimaries" << G4endl;
    }
}

////////////////////////////////////////////////////////////
CaptureGenerator::CaptureGenerator(DCGeometry *geom) : 
    G4VUserPrimaryGeneratorAction(), 
    fGeometry(geom), fParticle(nullptr)
{
    gun = new G4ParticleGun(1);
}

void CaptureGenerator::SetParticleDefinition() {

    fParticle = G4IonTable::GetIonTable()->GetIon(14, 29, 8473.6*keV);

    gun->SetParticleDefinition(fParticle);
    gun->SetParticleEnergy(0.);
}

void CaptureGenerator::CalculateParams() {
    dimx = fGeometry->GetLMFace();
    dimy = fGeometry->GetLMFace();
    dimz = fGeometry->GetLMThickness();
}


void CaptureGenerator::SetParticlePosition() {
    
    gun->SetParticlePosition( 
        G4ThreeVector((G4UniformRand() - 0.5)*dimx,
                      (G4UniformRand() - 0.5)*dimy,
                      (G4UniformRand() - 0.5)*dimz 
    ));
}


void CaptureGenerator::GeneratePrimaries(G4Event *anEvent) {
    if (!fParticle) {
        CalculateParams();
        SetParticleDefinition();
    }
    SetParticlePosition();
    gun->GeneratePrimaryVertex(anEvent);
}


////////////////////////////////////////////////////////////



TestGenerator::TestGenerator() : G4VUserPrimaryGeneratorAction(), gun(nullptr) 
{
    gun = new G4ParticleGun(1);
    gun->SetParticleDefinition(
        G4ParticleTable::GetParticleTable()->FindParticle(pname)
    );
    gun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
    gun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
    gun->SetParticleEnergy(penergy);
}


void TestGenerator::GeneratePrimaries(G4Event *anEvent) {
    gun->GeneratePrimaryVertex(anEvent);
}






