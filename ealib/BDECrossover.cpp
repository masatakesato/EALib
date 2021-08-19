#include	"BDECrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"


namespace ealib
{

	BDECrossover::BDECrossover()
		: ICrossoverOperator( TYPE_ID<BitArray> )
	{

	}



	BDECrossover::~BDECrossover()
	{

	}



	void BDECrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= chromosomes[0];
		IChromosome** pParents		= &chromosomes[1];

		for( int i=0; i<pTrial->Size(); ++i )
		{
			int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
			int numCoeffs	= ( numchroms - 1 ) / 2;
			int jrand		= int( OreOreLib::genrand_real2() * numParams );


			// Select Crossover point from dimension
			for( int j=0; j<numParams; ++j )
			{
				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR/*PR*/ || j==jrand )
				{
					if( OreOreLib::genrand_real1() < pAttrib->F/*MUT*/ )
					{
						pTrial->GeneAs<BitArray>(i)->Flip( j );
					}
					else
					{
						pTrial->GeneAs<BitArray>(i)->SetBit( j, pParents[0]->GeneAs<BitArray>(i)->GetBit( j ) );
					}
				}

			}// end of design parameter loop

		}// end of i loop

	}


}// end of namespace