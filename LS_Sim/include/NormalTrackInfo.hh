#ifndef NormalTrackInfo_hh
#define NormalTrackInfo_hh


#include "globals.hh"
#include "G4VUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <vector>
#include <map>

// Ref: http://geant4.slac.stanford.edu/Tips/

class NormalTrackInfo: public G4VUserTrackInformation {
    public:
        NormalTrackInfo();
        NormalTrackInfo(const G4Track* aTrack);
        NormalTrackInfo(const NormalTrackInfo* aTrackInfo);
        virtual ~NormalTrackInfo(); 


        inline void *operator new(size_t);
        inline void operator delete(void *aTrackInfo);
        inline int operator ==(const NormalTrackInfo& right) const {
            return (this==&right);
        }
    public:
        inline G4int GetOriginalTrackID() const {return originalTrackID;}

        inline void setFromCerenkov() {from_cerenkov = true;}
        inline G4bool isFromCerenkov(){return from_cerenkov;}

        inline void setReemission() {is_reemission = true;}
        inline G4bool isReemission(){return is_reemission;}


        inline void setOriginalOP() { m_op_is_original_op = true; }
        inline G4bool isOriginalOP() { return m_op_is_original_op; }

        inline void setParentName(const G4String& parent_name) { m_parent_name = parent_name; }
        inline const G4String& getParentName() const { return m_parent_name; }

        inline void addEdep(G4double edep) { m_edep += edep; }
        inline const G4double getEdep()    { return m_edep; }

        inline void addQedep(G4double qedep) { m_Qedep += qedep; }
        inline const G4double getQedep()     { return m_Qedep; }


    private:
        G4int originalTrackID;

        G4bool from_cerenkov;
        G4bool is_reemission;

        // the original OP info
        // Here, original means the parent of OP is not OP.
        G4bool m_op_is_original_op;

        // info of Parent Track
        G4String m_parent_name;

        // energy deposit
        G4double m_edep;
        G4double m_Qedep;
};

extern G4ThreadLocal G4Allocator<NormalTrackInfo>* aNormalTrackInformationAllocator;


inline void* NormalTrackInfo::operator new(size_t)
{ 
    void* aTrackInfo;
    if (!aNormalTrackInformationAllocator) {
        aNormalTrackInformationAllocator = new G4Allocator<NormalTrackInfo>;
    }
    aTrackInfo = (void*)aNormalTrackInformationAllocator->MallocSingle();
    return aTrackInfo;
}

inline void NormalTrackInfo::operator delete(void *aTrackInfo)
{ aNormalTrackInformationAllocator->FreeSingle((NormalTrackInfo*)aTrackInfo);}

#endif
