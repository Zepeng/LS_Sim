
#ifndef LSTrackingAction_h
#define LSTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class LSTrackingAction : public G4UserTrackingAction {

    public:
        LSTrackingAction   ();
        ~LSTrackingAction  ();

        void PreUserTrackingAction  (const G4Track* track);
        void PostUserTrackingAction (const G4Track*);

};

#endif
