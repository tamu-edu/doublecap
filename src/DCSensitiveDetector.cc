
#include "DCSensitiveDetector.hh"
#include "DCTrackInformation.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

G4ThreadLocal G4RootAnalysisManager *DCSensitiveDetector::fAnalysisMgr = nullptr;

DCSensitiveDetector::DCSensitiveDetector(const G4String& name, const G4String& hitsCollectionName) :
    G4VSensitiveDetector(name) 
{;}


void DCSensitiveDetector::Initialize(G4HCofThisEvent* hitsCollection) {
    if (!fAnalysisMgr) {
        fAnalysisMgr = G4RootAnalysisManager::Instance();
    }
}



G4bool DCSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {

    //G4cout << "Starting ProcessHits" << G4endl;

    G4double edep = step->GetTotalEnergyDeposit();
    const G4Track *track = step->GetTrack();
    const DCTrackInformation *info = (DCTrackInformation*)track->GetUserInformation();

    if (edep == 0.) return false;

    fAnalysisMgr->FillNtupleIColumn(0, 0, info->GetEventID());

    fAnalysisMgr->FillNtupleIColumn(0, 1, track->GetTrackID());

    fAnalysisMgr->FillNtupleDColumn(0, 2, edep);

    G4String volname = track->GetVolume()->GetName();
    fAnalysisMgr->FillNtupleSColumn(0, 3, volname);

    G4String pname = track->GetParticleDefinition()->GetParticleName();
    fAnalysisMgr->FillNtupleSColumn(0, 4, pname);

    G4String procname;
    if (track->GetTrackID() == 0) {
        procname = "n/a";
    } else{
        procname = track->GetCreatorProcess()->GetProcessName();
    }
    fAnalysisMgr->FillNtupleSColumn(0, 5, procname);

    G4String startvolume = info->GetVolume();
    fAnalysisMgr->FillNtupleSColumn(0, 6, startvolume);

    G4int isCapture = (procname == "nCapture" && startvolume == "lowmass") ? 1 : 0;
    fAnalysisMgr->FillNtupleIColumn(0, 7, isCapture);

    fAnalysisMgr->AddNtupleRow();

    //G4cout << "Ending ProcessHits" << G4endl;
    return true;
}


void DCSensitiveDetector::EndOfEvent(G4HCofThisEvent* hitCollection) {
    return;
}