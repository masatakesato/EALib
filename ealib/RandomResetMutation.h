#ifndef RANDOM_RESET_MUTATION_H
#define	RANDOM_RESET_MUTATION_H

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IMutationOperator.h"
#include	"IChromosome.h"



namespace ealib
{

	template< typename T, typename enable=void >
	class RandomResetMutation;


	// Random reset mutation( integral only )
	template< typename T >
	class /*CLASS_DECLSPEC*/ RandomResetMutation< T, std::enable_if_t< std::is_integral_v<T> > > : public IMutationOperator
	{
	public:
	
		RandomResetMutation()
			: IMutationOperator( TYPE_ID<T> )
		{

		}


		~RandomResetMutation()
		{

		}
	

		virtual void Execute( IChromosome* chromosome, float mutate_prob, const void* attribs=nullptr )
		{
			for( int i=0; i<chromosome->Size(); ++i )
			{
				const DesignParameter& pDParam = chromosome->GetDesignParameter( i );

				float mutateProb = float( OreOreLib::genrand_real1() );
				if( mutateProb < mutate_prob )
				{
					//tcout << "Mutate_RealNumber()... Applying Gaussian Mutation..." << tendl;
					T value = pDParam.DefaultValue<T>();
					Lerp( value, pDParam.LowerBoundary<T>(), pDParam.UpperBoundary<T>(), OreOreLib::genrand_real1() );
					chromosome->GeneAs<T>( i ) = value;
				}

			}// end of i loop

		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, float mutate_prob, const void* attribs=nullptr )
		{
			for( int j=0; j<numchroms; ++j )
				Execute( chromosomes[j], mutate_prob, attribs );
		}
		
	};


}// end of namespace


#endif // !RANDOM_RESET_MUTATION_H
