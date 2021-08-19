#include	"IDEMutationStrategy.h"

#include	<oreore/common/Utility.h>


namespace ealib
{

	IDEMutationStrategy::IDEMutationStrategy()
	{
		m_NumChroms	= 0;
		m_refChromosomes	= nullptr;
	}


	IDEMutationStrategy::~IDEMutationStrategy()
	{
		UnbindPopulationData();
	}

		
	void IDEMutationStrategy::BindPopulationData( int numchroms, IChromosome **pchroms )
	{
		m_NumChroms			= numchroms;
		m_refChromosomes	= pchroms;
	}


	void IDEMutationStrategy::UnbindPopulationData()
	{
		m_NumChroms			= 0;
		m_refChromosomes	= nullptr;
	}


}// end of namespace
