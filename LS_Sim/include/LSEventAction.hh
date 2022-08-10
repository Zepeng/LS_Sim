#ifndef LSEventAction_h
#define LSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class LSEventAction : public G4UserEventAction
{
    public:
        LSEventAction();
        virtual ~LSEventAction();

        virtual void BeginOfEventAction(const G4Event* event);
        virtual void   EndOfEventAction(const G4Event* event);

};

#endif
