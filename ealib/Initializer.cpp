#include	"Initializer.h"

#include	"IChromosome.h"


namespace ealib
{

	Initializer::Initializer()
				: m_refOperators{ nullptr }
	{

	}



	Initializer::~Initializer()
	{
		for( int i=0; i<NUM_TYPES; ++i )
			m_refOperators[i] = nullptr;
	}



	void Initializer::BindOperator( IInitializeOperator* pFunc )
	{
		m_refOperators[ pFunc->TypeID ] = pFunc;
	}


			
	void Initializer::UnbindOperator( int type )
	{
		m_refOperators[ type ] = nullptr;
	}



	void Initializer::Execute( IChromosome* chromosome )
	{
		IInitializeOperator* initOp = m_refOperators[ chromosome->TypeInfo() ];
		if( initOp )	initOp->Execute( chromosome );
	}

}// end of namespace
