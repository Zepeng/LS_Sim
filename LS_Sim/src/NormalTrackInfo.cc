
#include "NormalTrackInfo.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<NormalTrackInfo>* aNormalTrackInformationAllocator = 0;



NormalTrackInfo::NormalTrackInfo()
{
    from_cerenkov =false;
    is_reemission =false;
    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;
}

NormalTrackInfo::NormalTrackInfo(const G4Track* aTrack)
{
    from_cerenkov =false;
    is_reemission =false;

    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;
}

NormalTrackInfo::NormalTrackInfo(const NormalTrackInfo* aTrackInfo)
{
    from_cerenkov = aTrackInfo->from_cerenkov;
    is_reemission = aTrackInfo->is_reemission;

    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;
}

NormalTrackInfo::~NormalTrackInfo(){;}
