// DCActionInitialization.cc - source code for DCActionInitialization.hh

#include "DCUtilities.hh"
#include "DCActionInitialization.hh"
#include "DCRunAction.hh"
#include "DCEventAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCInitialization::DCInitialization(G4int Z, G4int A, G4double sourcex, G4double sourcey, G4double sourcez) : 
    G4VUserActionInitialization(), 
    Z(Z), A(A),
    source_position(G4ThreeVector(sourcex, sourcey, sourcez)) 
{;}

void DCInitialization::Build() const {
    DCRunAction *runAction = new DCRunAction();
    SetUserAction(runAction);
    SetUserAction(new DCEventAction(runAction));

    SetUserAction(new DCPrimaryGenerator(Z, A, source_position));
}

DCPrimaryGenerator::DCPrimaryGenerator(G4int Z, G4int A, G4ThreeVector source_position) : 
    G4VUserPrimaryGeneratorAction(), 
    gun(nullptr), 
    theParticle(nullptr) , 
    Z(Z), A(A), l(1.*mm),
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
        G4ThreeVector((G4UniformRand() - 0.5)*l,
                      (G4UniformRand() - 0.5)*l,
                      (G4UniformRand() - 0.5)*l
                      
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


