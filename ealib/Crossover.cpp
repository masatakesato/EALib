#include	"Crossover.h"

#include	"IChromosome.h"



namespace ealib
{

	Crossover::Crossover()
		: m_refOperators{ nullptr }
	{
	
	}



	Crossover::~Crossover()
	{
		for( int i=0; i<NUM_TYPES; ++i )
			m_refOperators[i] = nullptr;
	}

	

	void Crossover::BindOperator( ICrossoverOperator* pOperator )
	{
		m_refOperators[ pOperator->TypeID ] = pOperator;
	}
	


	void Crossover::UnbindOperator( int type )
	{
		m_refOperators[ type ] = nullptr;
	}



	void Crossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		ICrossoverOperator* crossover = m_refOperators[ chromosomes[0]->TypeInfo() ];
		if( crossover )	crossover->Execute( numchroms, chromosomes, attribs );
	}


}// end of namespace