#ifndef CREEP_MUTATION_H
#define	CREEP_MUTATION_H

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IMutationOperator.h"
#include	"IChromosome.h"



namespace ealib
{

	template< typename T, typename enable=void >
	class CreepMutation;


	// Creep mutation ( intergal only )
	template< typename T >
	class /*CLASS_DECLSPEC*/ CreepMutation< T, std::enable_if_t< std::is_arithmetic_v<T> > > : public IMutationOperator
	{
	public:

		CreepMutation()
			: IMutationOperator( TYPE_ID<T> )
		{

		}


		~CreepMutation()
		{

		}


		virtual void Execute( IChromosome* chromosome, float mutate_prob, const void* attribs=nullptr )
		{

			for( int i=0; i<chromosome->Size(); ++i )
			{
				const DesignParameter& pDParam = chromosome->GetDesignParameter( i );
				const float64 range = 0.1 * float64( pDParam.UpperBoundary<T>() - pDParam.LowerBoundary<T>() );

				float mutateProb	= float( OreOreLib::genrand_real1() );
				if( mutateProb < mutate_prob )
				{
					//tcout << "Mutate_RealNumber()... Applying Gaussian Mutation..." << tendl;
					T val = chromosome->GeneAs<T>( i ) + T( range * ( 2.0 * OreOreLib::genrand_real1() - 1.0 ) ); // offset by range * [-1.0, +1.0]
					chromosome->GeneAs<T>( i ) = Clamp( val, pDParam.LowerBoundary<T>(), pDParam.UpperBoundary<T>() );
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


#endif // !CREEP_MUTATION_H
