
#include "DCHit.hh"

G4ThreadLocal G4Allocator<DCHit>* TrackerHitAllocator = nullptr;

G4bool DCHit::operator==(const DCHit& right) const
{
  return (this == &right);
}


