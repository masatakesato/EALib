#include	"BinaryExponentialCrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"DEAttribute.h"
#include	"IChromosome.h"



namespace ealib
{

	BinaryExponentialCrossover::BinaryExponentialCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray>, { 0.0f, CrossoverAttribute::Dynamic, 1 } )
	{

	}



	BinaryExponentialCrossover::~BinaryExponentialCrossover()
	{

	}



	void BinaryExponentialCrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= chromosomes[0];// trial
		IChromosome** pParents		= &chromosomes[1];// parents
	
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



	void BinaryExponentialCrossover::Execute( int numparents, const IChromosome** parents, int numchildren, IChromosome** children, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= children[0];// trial
		const IChromosome** pParents	= &parents[0];// parents
	
		for( int i=0; i<pTrial->Size(); ++i )
		{
			auto pParentBitArray1 = parents[0]->GeneAs<BitArray>(i);
			auto pTrialBitArray = pTrial->GeneAs<BitArray>(i);

			int numParams	= pTrialBitArray->BitLength();
			int j			= int( OreOreLib::genrand_real2() * numParams );
			int L			= 0;
	
			do
			{
				// Apply Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...
				uint32 accum = 0;
				for( int k=1; k<numparents; k+=2 )
					accum |= ( uint32(parents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(parents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by '|=', '-' by '^'

				uint32 t_j	= uint32(pParentBitArray1->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by '|'

				pTrialBitArray->SetBit( j, (int)t_j );

	
				j	= ( j+1 )%numParams;
				L++;
			} while( OreOreLib::genrand_real1()<pAttrib->CR && L<numParams );

		}// end of i loop

	}



}// end of namespace