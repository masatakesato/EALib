#include	"Crossover.h"

#include	"IChromosome.h"
#include	"Chromosome2D.h"



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



void Crossover::Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs )
{
	ICrossoverOperator* crossover = m_refOperators[ parents[0]->TypeInfo() ];
	if( crossover )	crossover->Execute( numparents, parents, numchildren, children, attribs );
}


void Crossover::Execute2( OreOreLib::Memory<const IChromosome*>& parents, OreOreLib::Memory<IChromosome*>& children, const void* attribs )
{
	for( int i=0; i<parents[0]->NumChromTypes(); ++i )
	{
		auto type = parents[0]->GetChromosome(i)->TypeInfo();

		ICrossoverOperator* crossover = m_refOperators[ type ];
		if( crossover )	crossover->Execute( parents, children, attribs );
	}



//	ICrossoverOperator* crossover = m_refOperators[ parents[0]->TypeInfo() ];
//	if( crossover )	crossover->Execute( parents.Length(), parents.begin(), children.Length(), children.begin(), attribs );
}



int Crossover::NumParents( int type ) const
{
	return m_refOperators[ type ]->Attribute().NumParents;
}


int Crossover::NumChildren( int type ) const
{
	return m_refOperators[ type ]->Attribute().NumChildren;
}





}// end of namespace