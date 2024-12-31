


#include "DCUtilities.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCSensitiveDetector.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCPrimaryGenerator::DCPrimaryGenerator(G4int Z, G4int A, G4ThreeVector source_position) : 
    G4VUserPrimaryGeneratorAction(), 
    gun(nullptr), 
    theParticle(nullptr) , 
    Z(Z), A(A),
    source_position(source_position)
{
    gun = new G4ParticleGun(1);
}


void DCPrimaryGenerator::GetParticleDefinition() {
    // define particle type for gun and 
    if (Z == 0 && A == 0) {
        gun->SetParticleEnergy(1.*eV);
        theParticle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    } else {
        gun->SetParticleEnergy(0.);
        theParticle = G4IonTable::GetIonTable()->GetIon(Z, A);
    }
    gun->SetParticleDefinition(theParticle);
}

void DCPrimaryGenerator::SetParticlePositionMomentum() {

    gun->SetParticlePosition(source_position + 
        G4ThreeVector((G4UniformRand() - 0.5)*sideLength,
                      (G4UniformRand() - 0.5)*sideLength,
                      (G4UniformRand() - 0.5)*sideLength
                      
    ));

    if (Z == 0 && A == 0) { // particle = neutron: randomize direction
            
        gun->SetParticleMomentumDirection(RandomDirection()); // momentum direction (magnitude set by particle enery)
    }
}

void DCPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    if (!theParticle) {
        GetParticleDefinition();
    }
    SetParticlePositionMomentum();
    gun->GeneratePrimaryVertex(anEvent);
}




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






