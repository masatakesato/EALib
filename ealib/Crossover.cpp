#include	"Crossover.h"

#include	"IChromosome.h"
#include	"Chromosome2D.h"



namespace ealib
{

	Crossover::Crossover()
	{
	
	}



	Crossover::~Crossover()
	{
		m_refOperators.Clear();
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



//int Crossover::NumParents( int type ) const
//{
//	return m_refOperators[ type ]->Attribute().NumParents;
//}
//
//
//int Crossover::NumChildren( int type ) const
//{
//	return m_refOperators[ type ]->Attribute().NumChildren;
//}



bool Crossover::GetFamilySize( int16 type, int& numparents, int& numchildren ) const
{
	auto op = m_refOperators[ type ];
	if( !op )
		return false;

	numparents	= op->Attribute().NumParents;
	numchildren	= op->Attribute().NumChildren;

	return true;
}



bool Crossover::GetFamilySize( int& numparents, int& numchildren ) const
{
	numparents	= -1;
	numchildren	= -1;
	bool isvalid = false;

	for( const auto* op : m_refOperators )
	{
		if( op )
		{
			isvalid = true;
			numparents = Max( numparents, op->Attribute().NumParents );
			numchildren = Max( numchildren, op->Attribute().NumChildren );
		}
	}

	return isvalid;
}



}// end of namespace