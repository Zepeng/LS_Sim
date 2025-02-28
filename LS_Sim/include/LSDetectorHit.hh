
#ifndef LSDetectorHit_h
#define LSDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class LSDetectorHit : public G4VHit
{
    public:
        LSDetectorHit();
        LSDetectorHit(const LSDetectorHit&);
        virtual ~LSDetectorHit();

        // operators
        const LSDetectorHit& operator=(const LSDetectorHit&);
        G4int operator==(const LSDetectorHit&) const;

        inline void* operator new    (size_t);
        inline void operator  delete (void*);

        // methods from base class
        virtual void Draw();
        virtual void Print();

    private:
        G4double trackID;
        G4double time;
        G4double edep;
        G4double wavelength;
	
	G4double global_pos_x;
	G4double global_pos_y;
	G4double global_pos_z;

        G4bool isFromCerenkov;
        G4bool isReemission;
        G4bool isOriginalOP;


    public:
		inline void SetGlobalPosX(G4double x)		{ global_pos_x = x ;}
		inline G4double GetGlobalPosX()const		{ return global_pos_x;}		
		
		inline void SetGlobalPosY(G4double y)		{ global_pos_y = y ;}
		inline G4double GetGlobalPosY()const		{ return global_pos_y;}		
		
		inline void SetGlobalPosZ(G4double z)		{ global_pos_z = z ;}
		inline G4double GetGlobalPosZ()const		{ return global_pos_z;}		

        inline void SetTrackID(G4int tid)           { trackID = tid; }
        inline G4int GetTrackID()                   { return trackID; }

        inline void SetTime(G4double t)             { time = t; }
        inline G4double GetTime()                   { return time; }

        inline void SetEdep(G4double de)            { edep = de; }
        inline G4double GetEdep()                   { return edep;}
        
        inline void SetWavelength(G4double wl)      { wavelength = wl;}
        inline G4double GetWavelength()             { return wavelength; }

        void SetFromCerenkov(G4bool flag)           { isFromCerenkov = flag; }
        G4bool IsFromCerenkov()                     { return isFromCerenkov; }

        void SetReemission(G4bool flag)             { isReemission = flag;}
        G4bool IsReemission()                       { return isReemission; }

        void SetOriginalOP(G4bool flag)             { isOriginalOP = flag; }
        G4bool IsOriginalOP()                       { return isOriginalOP; }


};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<LSDetectorHit> LSDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<LSDetectorHit>* LSDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* LSDetectorHit::operator new(size_t)
{
  if(!LSDetectorHitAllocator)
      LSDetectorHitAllocator = new G4Allocator<LSDetectorHit>;
  return (void *) LSDetectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void LSDetectorHit::operator delete(void *hit)
{
  LSDetectorHitAllocator->FreeSingle((LSDetectorHit*) hit);
}


#endif
