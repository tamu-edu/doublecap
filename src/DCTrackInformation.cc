
#include "DCTrackInformation.hh"
#include "G4MTRunManager.hh"


G4ThreadLocal G4Allocator<DCTrackInformation>* DCTrkInfoAlloc = nullptr;

DCTrackInformation::DCTrackInformation() : 
    G4VUserTrackInformation(),
    fParentVolume(nullptr),
    fEventID(-1)
{;}

DCTrackInformation::DCTrackInformation(const G4Track *track) :
    G4VUserTrackInformation()
{
    SetSourceTrackInformation(track);
}

DCTrackInformation::DCTrackInformation(const DCTrackInformation *info) :
    G4VUserTrackInformation()
{
    fParentVolume = info->GetVolume();
    fEventID = info->GetEventID();
}

//DCTrackInformation::~DCTrackInformation() {;}

DCTrackInformation& DCTrackInformation::operator=(const DCTrackInformation& left) {
    fEventID = left.fEventID;
    fParentVolume = left.fParentVolume;
    return *this;
}

void DCTrackInformation::SetSourceTrackInformation(const G4Track *track) {
    G4VPhysicalVolume *pv = track->GetVolume();
    if (!pv) {
        fParentVolume = "";
    } else {
        fParentVolume = pv->GetName();
    }
    const G4Event *event = G4MTRunManager::GetRunManager()->GetCurrentEvent();
    if (!event) {
        fEventID = -1;
    } else {
        fEventID = event->GetEventID();
    }
}

