
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"

#include "DCEventAction.hh"


DCEventAction::DCEventAction(DCRunAction *ra, DCTrackAction *ta) :
    runAction(ra),
    trackAction(ta)
{;}


void DCEventAction::BeginOfEventAction(const G4Event* /*evt*/) {;}


void DCEventAction::EndOfEventAction(const G4Event *evt) {

    G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
    if (!HCE) {
        G4cout << "BAD" << G4endl;
        return;
    }


    // Get hits collections IDs
    if (id_lm < 0) {
        G4SDManager* SDMan = G4SDManager::GetSDMpointer();
        id_lm = SDMan->GetCollectionID("lm/edep");
        id_hm = SDMan->GetCollectionID("hm/edep");
        //id_hm = SDMan->GetCollectionID("DCHitsCollection");
    }

    G4double edep = 0.;

    //G4Track *track = step->GetTrack();

    // DCHitsCollection evtMap = (DCHitsCollection*)(HCE->GetHC(id_lm));
    auto evtMap = static_cast<G4THitsMap<G4double>*>(HCE->GetHC(id_lm));

    for (auto& mapElement: (*evtMap->GetMap())) {
        edep += *(mapElement.second);
    }

    if (edep > 0.) runAction->SumLM(edep);

    edep = 0.;

    evtMap = static_cast<G4THitsMap<G4double>*>(HCE->GetHC(id_hm));

    for (auto& mapElement: (*evtMap->GetMap())) {
        edep += *(mapElement.second);
    }

    if (edep > 0.) runAction->SumHM(edep);

}
