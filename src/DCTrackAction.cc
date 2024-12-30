
#include "DCTrackAction.hh"
#include "DCTrackInformation.hh"

// needed for printing stuff
#include "G4TrackingManager.hh"



DCTrackAction::DCTrackAction() : G4UserTrackingAction() {;}


void DCTrackAction::PreUserTrackingAction(const G4Track *track) {

    if (verbosity > 1) {
        G4cout << "Starting PreUserTrackingAction" << G4endl;
    }

    DCTrackInformation *info = new DCTrackInformation(track);
    track->SetUserInformation(info);


    if (verbosity > 1) {
        G4cout << "Ending PreUserTrackingAction" << G4endl;
    }
}


void DCTrackAction::PostUserTrackingAction(const G4Track *track) {

    if (verbosity > 1) {
        G4cout << "Starting PostUserTrackingAction" << G4endl;
    }


    // copy DCTrackInformation to daughters

    G4TrackVector *secondaries = fpTrackingManager->GimmeSecondaries();
    if (secondaries) {
        //G4cout << "yes secondaries" << G4endl;
        DCTrackInformation *info = (DCTrackInformation*)(track->GetUserInformation());
        if (!info) {
            G4cout << "no info" << G4endl;
        }
        size_t nSeco = secondaries->size();
        for (size_t i = 0; i < nSeco; ++i) {
            DCTrackInformation *infoNew = new DCTrackInformation(info);
            secondaries->operator[](i)->SetUserInformation(infoNew);
        }
    }

    if (verbosity > 1) {
        G4cout << "Ending PostUserTrackingAction" << G4endl;
    }
}


