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
			Execute_<T>( chromosome );
		}



	private:

		template < typename T >
		void Execute_( IChromosome* chromosome )
		{
			//chromosome->ClearResult();
			for( int i=0; i<chromosome->Size(); ++i )
			{
				DesignParameter* pDParam = chromosome->GetDesignParameter( i );

				T value = pDParam->DefaultValue<T>();
				Lerp( value, pDParam->LowerBoundary<T>(), pDParam->UpperBoundary<T>(), OreOreLib::genrand_real1() );
				*chromosome->GeneAs<T>( i ) = value;
			}
		}


		template <>
		void Execute_<BitArray>( IChromosome* chromosome )
		{
			//chromosome->ClearResult();		
			for( int i=0; i<chromosome->Size(); ++i )
				chromosome->GeneAs<BitArray>(i)->RandomizeAll();
		}


	};


}// end of namespace



#endif // !RANDOM_INIT_OPERATOR_H