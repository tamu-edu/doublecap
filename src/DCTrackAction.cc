
#include "DCTrackAction.hh"

// needed for printing stuff
#include "G4VProcess.hh"
#include "G4RunManager.hh"


G4int DCTrackAction::CAPTURECOUNT = 0;



DCTrackAction::DCTrackAction(DCRunAction *runAction) : 
    G4UserTrackingAction(),
    fRunAction(runAction),
    fIsCapture(false)
{
    fGammaPD = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
}


void DCTrackAction::PreUserTrackingAction(const G4Track *track) {

    fIsCapture = false;
    
    G4String procname; 
    G4String volname;
    
    const G4VProcess *cproc = track->GetCreatorProcess();
    const G4ParticleDefinition *part = track->GetParticleDefinition();
    G4VPhysicalVolume *fCurrentPV = track->GetVolume();

    if (!cproc) {
        procname = "no creator process";
    } else {
        procname = cproc->GetProcessName();
    }
    if (!fCurrentPV) {
        volname = "no volume";
    } else{
        volname = fCurrentPV->GetName();
    }
    if (procname == "nCapture" && volname == "lowmass" && part != fGammaPD) {
        CAPTURECOUNT++;
        G4cout << "==========================================" << G4endl;
        //G4cout << "in DCTrackAction::PreUserTrackingAction." << G4endl;
        G4cout << "GetCreatorProcess() = " << procname << G4endl;
        G4cout << "GetTrackID() = " << track->GetTrackID() << G4endl;
        G4cout << "EventID = " << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID() << G4endl;
        G4cout << "ParticleDefinition = " << part->GetParticleName() << G4endl;
        G4cout << "GetVolume() = " << volname << G4endl;
        G4cout << "Count: " << CAPTURECOUNT << G4endl;
    }
}


void DCTrackAction::PostUserTrackingAction(const G4Track *track) {
    return;
}


