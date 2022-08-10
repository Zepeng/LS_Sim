
#include "LSDetectorHit.hh"

G4ThreadLocal G4Allocator<LSDetectorHit>* LSDetectorHitAllocator = 0;

LSDetectorHit::LSDetectorHit()
    : G4VHit(),
      time(0), edep(0), wavelength(0), trackID(-1),
      isFromCerenkov(0), isReemission(0), isOriginalOP(0)
{;}

LSDetectorHit::~LSDetectorHit()
{;}


LSDetectorHit::LSDetectorHit( const LSDetectorHit& right)
    : G4VHit()
{
    time = right.time;
    edep = right.edep;
    wavelength = right.wavelength;
    trackID = right.trackID;

    isFromCerenkov = right.isFromCerenkov;
    isReemission = right.isReemission;
    isOriginalOP = right.isOriginalOP;
}


const LSDetectorHit& LSDetectorHit::operator=(const LSDetectorHit& right)
{
    time = right.time;
    edep = right.edep;
    wavelength = right.wavelength;
    trackID = right.trackID;

    isFromCerenkov = right.isFromCerenkov;
    isReemission = right.isReemission;
    isOriginalOP = right.isOriginalOP;

    return *this;
}

G4int LSDetectorHit::operator==(const LSDetectorHit& right) const
{
    return (  this == &right ) ? 1 : 0;
}
void LSDetectorHit::Draw()
{

}

void LSDetectorHit::Print()
{
    G4cout << "Hit Info : trackID = " << trackID << ", time = " << time 
           << ", FromCerenkov = " << isFromCerenkov << ", isReemission = " << isReemission
           << ", isOriginalOP = " << isOriginalOP << G4endl;
}





