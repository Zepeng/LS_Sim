#ifndef LSActionInitialization_h
#define LSActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class LSActionInitialization : public G4VUserActionInitialization
{
    public:
        LSActionInitialization();
        virtual ~LSActionInitialization();

        virtual void Build() const;
};


#endif
