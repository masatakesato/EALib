#include	"HammingDECrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"



namespace ealib
{

	HammingDECrossover::HammingDECrossover()
		: ICrossoverOperator( TYPE_ID<BitArray> )
	{

	}


	HammingDECrossover::~HammingDECrossover()
	{

	}


	void HammingDECrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= chromosomes[0];
		IChromosome** pParents		= &chromosomes[1];

		for( int i=0; i<pTrial->Size(); ++i )
		{
			int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
			int numCoeffs	= ( numchroms - 1 ) / 2;
			int jrand		= int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j=0; j<numParams; ++j )
			{
				int t_j	= pTrial->GeneAs<BitArray>(i)->GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					int x_r1_j = pParents[0]->GeneAs<BitArray>(i)->GetBit( j ), x_r2_j = pParents[1]->GeneAs<BitArray>(i)->GetBit( j ), x_r3_j = pParents[2]->GeneAs<BitArray>(i)->GetBit( j );
					int fa = (int)round( pAttrib->F * float( x_r2_j != x_r3_j ) );
					t_j	= int( x_r1_j != fa );

					pTrial->GeneAs<BitArray>(i)->SetBit( j, t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}// end of i loop

	}


}// end of namespace