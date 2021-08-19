﻿#include	"BinaryBinomialCrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"Chromosome1D.h"



namespace ealib
{

	BinaryBinomialCrossover::BinaryBinomialCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray> )
	{

	}



	BinaryBinomialCrossover::~BinaryBinomialCrossover()
	{

	}



	void BinaryBinomialCrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial		= chromosomes[0];// trial
		IChromosome** pParents	= &chromosomes[1];// parents

		for( int i=0; i<pTrial->Size(); ++i )
		{
			int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
			int numParents	= numchroms - 1;
			int jrand		= int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j=0; j<numParams; ++j )
			{
				uint32 t_j = pTrial->GeneAs<BitArray>(i)->GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					// Apply Mutation. pParents[0] + F * ( pParents[1] - pParents[2] ) + F * ( pParents[3] - pParents[4] )...
					uint32 accum = 0;
					for( int k=1; k<numParents; k+=2 )
						accum |= ( uint32(pParents[k]->GeneAs<BitArray>(i)->GetBit( j )) ^ uint32(pParents[k+1]->GeneAs<BitArray>(i)->GetBit( j )) );// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );

					t_j	= uint32(pParents[0]->GeneAs<BitArray>(i)->GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

					pTrial->GeneAs<BitArray>(i)->SetBit( j, (int)t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}//end of i loop

	}



}// end of namespace