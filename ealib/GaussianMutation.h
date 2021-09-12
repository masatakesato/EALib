#ifndef GAUSSIAN_MUTATION_H
#define	GAUSSIAN_MUTATION_H

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IMutationOperator.h"
#include	"IChromosome.h"



namespace ealib
{

	template< typename T, typename enabled=void >
	class GaussianMutation;

	
	// Mutation operator ( for arithmetic chromosome ).
	template< typename T >
	class /*CLASS_DECLSPEC*/ GaussianMutation< T, std::enable_if_t< std::is_arithmetic_v<T> > > : public IMutationOperator
	{
	public:
	
		GaussianMutation()
			 : IMutationOperator( TYPE_ID<T> )
		{

		}


		~GaussianMutation()
		{

		}


		virtual void Execute( IChromosome* chromosome, float mutate_prob, const void* attribs=nullptr )
		{
			for( int i=0; i<chromosome->Size(); ++i )
			{
				const DesignParameter& pDParam = chromosome->GetDesignParameter( i );
				const float range		=  0.1f * float( pDParam.UpperBoundary<T>() - pDParam.LowerBoundary<T>() );

				float mutateProb	= float( OreOreLib::genrand_real1() );
				if( mutateProb < mutate_prob )
				{
					//tcout << "Mutate_RealNumber()... Applying Gaussian Mutation..." << tendl;
					float param_i = (float)chromosome->GeneAs<T>( i );
					float value = GaussDistributionRandom( param_i, range );
					chromosome->GeneAs<T>( i ) = (T)Clamp( value, (float)pDParam.LowerBoundary<T>(), (float)pDParam.UpperBoundary<T>() );
				}

			}// end of i loop

		}


		virtual void Execute( int numchroms, IChromosome** chromosomes, float mutate_prob, const void* attribs=nullptr )
		{
			for( int j=0; j<numchroms; ++j )
				Execute( chromosomes[j], mutate_prob );
		}



	private:

		
		float GaussDistributionRandom( float mu, float sigma )
		{
			float z = sqrt( -2.0f * (float)log( OreOreLib::genrand_real1() ) ) * sin( 2.0f * M_PI * (float)OreOreLib::genrand_real1() );
			return float( mu + sigma * z );
		}


	};


}// end of namespace


#endif // !GAUSSIAN_MUTATION_H
