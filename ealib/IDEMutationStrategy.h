#ifndef I_DE_MUTATION_STRATEGY_H
#define	I_DE_MUTATION_STRATEGY_H


#include	"DLLExport.h"



namespace ealib
{
	class IChromosome;

	class CLASS_DECLSPEC IDEMutationStrategy
	{
	public:

		IDEMutationStrategy();
		virtual ~IDEMutationStrategy();

		void BindPopulationData( int numchroms, IChromosome **pchroms );
		void UnbindPopulationData();

		virtual void Execute( int num, IChromosome** selections, int current )=0;


	protected:

		int			m_NumChroms;
		IChromosome **m_refChromosomes;
	};


}// end of namespace



#endif // !I_DE_MUTATION_STRATEGY_H
