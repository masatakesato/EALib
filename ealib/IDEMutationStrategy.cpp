#include	"IDEMutationStrategy.h"

#include	<oreore/common/Utility.h>


namespace ealib
{

	IDEMutationStrategy::IDEMutationStrategy()
	{

	}



	IDEMutationStrategy::~IDEMutationStrategy()
	{
		m_refChromosomes.Release();
	}

		

	void IDEMutationStrategy::BindPopulationData( const OreOreLib::Array<IChromosome*>& chromosomes )
	{
		m_refChromosomes.Init( chromosomes );
	}



	void IDEMutationStrategy::UnbindPopulationData()
	{
		m_refChromosomes.Release();
	}


}// end of namespace
