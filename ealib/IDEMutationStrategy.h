#ifndef I_DE_MUTATION_STRATEGY_H
#define	I_DE_MUTATION_STRATEGY_H

#include	<oreore/common/DLLExport.h>
#include	<oreore/container/Array.h>
#include	<oreore/container/ArrayView.h>



namespace ealib
{
	class IChromosome;


	class CLASS_DECLSPEC IDEMutationStrategy
	{
	public:

		IDEMutationStrategy();
		virtual ~IDEMutationStrategy();

		void BindPopulationData( const OreOreLib::Array<IChromosome*>& chromosomes );//void BindPopulationData( int numchroms, IChromosome **pchroms );
		void UnbindPopulationData();

		virtual void Execute( OreOreLib::Memory<const IChromosome*>& selections, int current )=0;


	protected:

		OreOreLib::ArrayView<IChromosome*>	m_refChromosomes;

	};


}// end of namespace



#endif // !I_DE_MUTATION_STRATEGY_H
