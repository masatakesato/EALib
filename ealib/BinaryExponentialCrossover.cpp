#include	"BinaryExponentialCrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"DEAttribute.h"
#include	"IChromosome.h"



namespace ealib
{

	BinaryExponentialCrossover::BinaryExponentialCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray> )
	{

	}



	BinaryExponentialCrossover::~BinaryExponentialCrossover()
	{

	}



	void BinaryExponentialCrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= chromosomes[0];// trial
		IChromosome **pParents		= &chromosomes[1];// parents
	
		for( int i=0; i<pTrial->Size(); ++i )
		{
			int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
			int numParents	= numchroms - 1;
			int j			= int( OreOreLib::genrand_real2() * numParams );
			int L			= 0;
	
			do
			{
				// Apply Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...
				uint32 accum = 0;
				for( int k=1; k<numParents; k+=2 )
					accum |= ( uint32(pParents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(pParents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by '|=', '-' by '^'

				uint32 t_j	= uint32(pParents[0]->GeneAs<BitArray>(i)->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by '|'

				pTrial->GeneAs<BitArray>(i)->SetBit( j, (int)t_j );

	
				j	= ( j+1 )%numParams;
				L++;
			} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

		}// end of i loop
	
	}


}// end of namespace