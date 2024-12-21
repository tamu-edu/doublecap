#ifndef DCTRACKINFORMATION_HH
#define DCTRACKINFORMATION_HH


#include "G4Allocator.hh"
#include "G4Threading.hh"
#include "G4VUserTrackInformation.hh"
#include "G4Track.hh"


class DCTrackInformation : public G4VUserTrackInformation {
public:
    DCTrackInformation();
    DCTrackInformation(const G4Track*);
    DCTrackInformation(const DCTrackInformation*);
    ~DCTrackInformation() override = default;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    DCTrackInformation& operator=(const DCTrackInformation&);

    void SetSourceTrackInformation(const G4Track*);

    G4String GetVolume() const { return fParentVolume; }
    G4int GetEventID() const { return fEventID; }

private:
    G4int fEventID;
    G4String fParentVolume;
};

extern G4ThreadLocal G4Allocator<DCTrackInformation>* DCTrkInfoAlloc;


inline void* DCTrackInformation::operator new(size_t) {
    if (!DCTrkInfoAlloc) DCTrkInfoAlloc = new G4Allocator<DCTrackInformation>;
    return (void*)DCTrkInfoAlloc->MallocSingle();
}

inline void DCTrackInformation::operator delete(void* aTrackInfo) {
    DCTrkInfoAlloc->FreeSingle((DCTrackInformation*)aTrackInfo);
}


#endif
