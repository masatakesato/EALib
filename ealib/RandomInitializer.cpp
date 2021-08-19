#include	"RandomInitializer.h"

#include	"RandomInitOperator.h"



namespace ealib
{

	//##################################################################//
	//																	//
	//				Initializer array builder using TypeList			//
	//																	//
	//##################################################################//

	template < typename TList >
	struct RandomInitializer::BuildOperators
	{
		static void Execute( IInitializeOperator* ops[], int i=0 )
		{
			ops[i] = new RandomInitOperator< typename TList::head >();
			RandomInitializer::BuildOperators< typename TList::tail >::Execute( ops, i+1 );
		}
	};


	// typelist end
	template <>
	struct RandomInitializer::BuildOperators< OreOreLib::TypeList::NullType  >
	{
		static void Execute( IInitializeOperator* ops[], int i ){}// Do nothing
	};




	//##################################################################//
	//																	//
	//				RandomInitializer class implementation				//
	//																	//
	//##################################################################//


	RandomInitializer::RandomInitializer()
	{
		BuildOperators< g_ChoromosomeTypes >::Execute( m_refOperators );
	}



	RandomInitializer::~RandomInitializer()
	{
		for( int i=0; i<NUM_TYPES; ++i )
			delete m_refOperators[i];// = nullptr;
	}



}// end of namespace
