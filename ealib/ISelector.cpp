#include	"ISelector.h"



namespace ealib
{

	ISelector::ISelector()
	{

	}



	ISelector::~ISelector()
	{
		m_refChromosomes.Release();
	}



	void ISelector::BindPopulationData( const OreOreLib::Array<IChromosome*>& chromosomes )
	{
		m_refChromosomes.Init( chromosomes );
	}



	void ISelector::UnbindPopulationData()
	{
		m_refChromosomes.Release();
	}


}// end of namespace