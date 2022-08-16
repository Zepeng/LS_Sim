#ifndef LSActionInitialization_h
#define LSActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class LSActionInitializationMessenger;
class LSActionInitialization : public G4VUserActionInitialization
{
    public:
        LSActionInitialization();
        virtual ~LSActionInitialization();

        virtual void Build() const;
		void SetOpticksMode(int mode);
	private:
		LSActionInitializationMessenger* theMessenger;
		int m_opticksMode;
};


#endif
