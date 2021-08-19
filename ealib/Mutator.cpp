#include	"Mutator.h"

#include	"IChromosome.h"



namespace ealib
{

	Mutator::Mutator()
		: m_refOperators{ nullptr }
	{
	
	}



	Mutator::~Mutator()
	{
		for( int i=0; i<NUM_TYPES; ++i )
			m_refOperators[i] = nullptr;
	}

	

	void Mutator::BindOperator( IMutationOperator* pFunc )
	{
		m_refOperators[ pFunc->TypeID ] = pFunc;
	}



	void Mutator::UnbindOperator( int type )
	{
		m_refOperators[ type ] = nullptr;
	}



	void Mutator::Execute( IChromosome* chromosome, float mutate_prob )
	{
		IMutationOperator* mutator = m_refOperators[ chromosome->TypeInfo() ];
		if( mutator )	mutator->Execute( chromosome, mutate_prob );
	}



}// end of namespace