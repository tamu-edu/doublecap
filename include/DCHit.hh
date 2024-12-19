#ifndef DCHIT_HH
#define DCHIT_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4VHit.hh"


class DCHit : public G4VHit {
public:
    DCHit() = default;
    DCHit(const DCHit&) = default;
    virtual ~DCHit() override = default;

    // operators
    DCHit& operator=(const DCHit&) = default;
    G4bool operator==(const DCHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    void SetEvtNumber(G4int evtnum) {fEvtNum = evtnum;}
    void SetTrackID(G4int id) {fTrackID = id;}
    void SetEdep(G4double Edep) {fEdep = Edep;}
    void SetIsCapture(G4bool iscap) {fIsCapture = iscap;}
    void SetVolName(G4String name) {fVolName = name;}
    void SetParticleName(G4String name) {fParticleName = name;}

    G4int GetEvtNumber() const {return fEvtNum;}
    G4int GetTrackID() const {return fTrackID;}
    G4double GetEdep() const {return fEdep;}
    G4bool GetIsCapture() const {return fIsCapture;}
    G4String GetVolName() const {return fVolName;}
    G4String GetParticleName() const {return fParticleName;}

private:
    G4int fEvtNum = -1;
    G4int fTrackID = -1;
    G4double fEdep = 0.;
    G4bool fIsCapture = false;
    G4String fVolName = "";
    G4String fParticleName = "";
};


using DCHitsCollection = G4THitsCollection<DCHit>;


extern G4ThreadLocal G4Allocator<DCHit>* DCHitAllocator;

inline void* DCHit::operator new(size_t)
{
  if (!DCHitAllocator) DCHitAllocator = new G4Allocator<DCHit>;
  return (void*)DCHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DCHit::operator delete(void* hit)
{
  DCHitAllocator->FreeSingle((DCHit*)hit);
}


#endif