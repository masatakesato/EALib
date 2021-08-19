#ifndef RANDOM_INIT_OPERATOR_H
#define	RANDOM_INIT_OPERATOR_H

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>


#include	"IInitOperator.h"
#include	"IChromosome.h"



namespace ealib
{
	
	template< typename T >
	class RandomInitOperator : public IInitializeOperator
	{
	public:

		RandomInitOperator()
			: IInitializeOperator( TYPE_ID<T> )
		{
		
		}


		~RandomInitOperator()
		{
		
		}


		virtual void Execute( IChromosome* chromosome )
		{
			Execute_( chromosome );
		}



	private:

		template < typename Type=T >
		std::enable_if_t< std::is_arithmetic_v<Type>, void >
		Execute_( IChromosome* chromosome )
		{
//chromosome->ClearResult();
			for( int i=0; i<chromosome->Size(); ++i )
			{
				DesignParameter* pDParam = chromosome->GetDesignParameter( i );

				Type value = pDParam->DefaultValue<Type>();
				Lerp( value, pDParam->LowerBoundary<Type>(), pDParam->UpperBoundary<Type>(), OreOreLib::genrand_real1() );
				*chromosome->GeneAs<T>( i ) = value;
			}
		}


		template < typename Type=T >
		std::enable_if_t< std::is_same_v<Type, BitArray>, void >
		Execute_( IChromosome* chromosome )
		{
//chromosome->ClearResult();		
			for( int i=0; i<chromosome->Size(); ++i )
			{
				DesignParameter* pDParam = chromosome->GetDesignParameter( i );
				auto pbitarray = chromosome->GeneAs<BitArray>(i);

				pbitarray->Init( pDParam->UpperBoundary<int>() );
				pbitarray->RandomizeAll();
			}
		}


	};


}// end of namespace



#endif // !RANDOM_INIT_OPERATOR_H