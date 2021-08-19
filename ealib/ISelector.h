#ifndef I_SELECTOR_H
#define	I_SELECTOR_H


#include	"DLLExport.h"


namespace ealib
{
	class IChromosome;


	class CLASS_DECLSPEC ISelector
	{
	public:

		ISelector();
		virtual ~ISelector();

		virtual void BindPopulationData( int numchroms, IChromosome **pchroms );
		virtual void UnbindPopulationData();

		// Virtual Functions
		virtual void Update()=0;
		virtual int Execute()=0;
		virtual void Execute( int num, int selections[], int exclude ){}


	protected:

		int			m_NumChroms;
		IChromosome **m_refChromosomes;

	};



}// end of namespace



#endif // !I_SELECTOR_H
