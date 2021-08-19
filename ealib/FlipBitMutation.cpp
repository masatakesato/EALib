#include	"FlipBitMutation.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"



namespace ealib
{

	FlipBitMutation::FlipBitMutation()
		: IMutationOperator( TYPE_ID<BitArray> )
	{
		
	}



	FlipBitMutation::~FlipBitMutation()
	{

	}



	void FlipBitMutation::Execute( IChromosome* chromosome, float mutate_prob, const void* attribs )
	{
		for( int i=0; i<chromosome->Size(); ++i )
		{
			BitArray *pBitString = chromosome->GeneAs<BitArray>( i );

			for( int j=0; j<pBitString->BitLength(); ++j )
			{
				float mutateProb	= float( OreOreLib::genrand_real1() );
				if( mutateProb < mutate_prob )
					pBitString->Flip( j );
			}
		}

	}



	void FlipBitMutation::Execute( int numchroms, IChromosome** chromosomes, float mutate_prob, const void* attribs )
	{
		for( int j=0; j<numchroms; ++j )
			Execute( chromosomes[j], mutate_prob, attribs );
	}



}// end of namespace
