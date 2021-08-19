#include	"ISelector.h"




namespace ealib
{

	ISelector::ISelector()
	{
		m_NumChroms			= 0;
		m_refChromosomes	= nullptr;
	}



	ISelector::~ISelector()
	{
		m_NumChroms			= 0;
		m_refChromosomes	= nullptr;
	}



	void ISelector::BindPopulationData( int numchroms, IChromosome **pchroms )
	{
		m_NumChroms			= numchroms;
		m_refChromosomes	= pchroms;
	}



	void ISelector::UnbindPopulationData()
	{
		m_NumChroms			= 0;
		m_refChromosomes	= nullptr;
	}


}// end of namespace