
#include "DCSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4VProcess.hh"


DCSensitiveDetector::DCSensitiveDetector(const G4String& name, const G4String& hitsCollectionName) :
    G4VSensitiveDetector(name) 
{
    collectionName.insert(hitsCollectionName);
}


void DCSensitiveDetector::Initialize(G4HCofThisEvent* hitsCollection) {

    fAnalysisMgr = G4RootAnalysisManager::Instance();

    fHitsCollection = new DCHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    
    hitsCollection->AddHitsCollection(hcID, fHitsCollection);


    fGammaPD = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

}



G4bool DCSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {

    G4double edep = step->GetTotalEnergyDeposit();

    if (edep == 0.) return false;

    const G4Event& *event = G4RunManager::GetRunManager()->GetCurrentEvent();

    if (!event) {
        //newHit->SetEvtNumber(-1);
        fAnalysisMgr->FillNtupleIColumn(0, 0, -1);
    } else {
        //newHit->SetEvtNumber(event->GetEventID());
        fAnalysisMgr->FillNtupleIColumn(0, 0, event->GetEventID());
    }

    const G4Track& *track = step->GetTrack();

    fAnalysisMgr->FillNtupleIColumn(0, 1, track->GetTrackID());
    //newHit->SetTrackID(track->GetTrackID());

    fAnalysisMgr->FillNtupleDColumn(0, 2, edep);
    //newHit->SetEdep(edep);

    G4String procname; 
    G4String volname;
    
    const G4VProcess *cproc = track->GetCreatorProcess();
    const G4ParticleDefinition *part = track->GetParticleDefinition();
    G4VPhysicalVolume *fCurrentPV = track->GetVolume();


    if (!fCurrentPV) {
        volname = "no volume";
    } else{
        volname = fCurrentPV->GetName();
    }
    //newHit->SetVolName(volname);
    fAnalysisMgr->FillNtupleSColumn(0, 3, volname);

    //newHit->SetParticleName(part->GetParticleName());
    fAnalysisMgr->FillNtupleSColumn(0, 4, part->GetParticleName());

    if (!cproc) {
        procname = "no creator process";
    } else {
        procname = cproc->GetProcessName();
    }
    //newHit->SetIsCapture(procname == "nCapture" && volname == "lowmass" && part != fGammaPD);

    G4String startvolume;

    G4int isCapture = (procname == "nCapture" && startvolume == "lowmass" && part == fGammaPD) ? 1 : 0;

    fAnalysisMgr->FillNtupleSColumn(0, 5, procname);

    fAnalysisMgr->FillNtupleIColumn(0, 6, isCapture);
    fAnalysisMgr->AddNtupleRow();

    return true;
}



void DCSensitiveDetector::EndOfEvent(G4HCofThisEvent* hitCollection) {
    return;
}