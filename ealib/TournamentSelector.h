#ifndef TORUNAMENT_SELECTOR_H
#define	TORUNAMENT_SELECTOR_H


#include	"ISelector.h"



namespace ealib
{

	class CLASS_DECLSPEC TournamentSelector : public ISelector
	{
	public:

		TournamentSelector();
		virtual ~TournamentSelector();

		int GetTournamentSize() const	{ return m_TournamentSize; }
		void SetTournamentSize( int n )	{ m_TournamentSize = n; }
		
		// Virtual Functions Override
		virtual void Update(){}
		virtual int Execute();


	private:

		int	m_TournamentSize;

	};



}// end of namespace



#endif // !TORUNAMENT_SELECTOR_H
