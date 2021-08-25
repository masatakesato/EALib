﻿#include	"BDECrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"



// Implementation of "Algorithm 1" from "The Multiple Knapsack Problem Approached by a Binary Differential Evolution Algorithm with Adaptive Parameters"
// https://www.polibits.gelbukh.com/2015_51/The%20Multiple%20Knapsack%20Problem%20Approached%20by%20a%20Binary%20Differential%20Evolution%20Algorithm%20with%20Adaptive%20Parameters.pdf



namespace ealib
{

	BDECrossover::BDECrossover()
		: ICrossoverOperator( TYPE_ID<BitArray>, { 0.5f, 1, 1 }  )
	{

	}



	BDECrossover::~BDECrossover()
	{

	}



	void BDECrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial		= chromosomes[0];
		IChromosome* pParent	= chromosomes[1];

		for( int i=0; i<pTrial->Size(); ++i )
		{
			int numParams	= pTrial->GeneAs<BitArray>(i)->BitLength();
			int jrand		= int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimension
			for( int j=0; j<numParams; ++j )
			{
				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR/*PR*/ || j==jrand )
				{
					if( OreOreLib::genrand_real1() < pAttrib->F/*MUT*/ )
					{
						pTrial->GeneAs<BitArray>(i)->Flip( j );// Mutation
					}
					else
					{
						pTrial->GeneAs<BitArray>(i)->SetBit( j, pParent->GeneAs<BitArray>(i)->GetBit( j ) );// Crossover
					}
				}

			}// end of design parameter loop

		}// end of i loop

	}



	void BDECrossover::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		const IChromosome* pParent	= parents[0];
		IChromosome* pTrial			= children[0];

		for( int i=0; i<pTrial->Size(); ++i )
		{
			const auto* pParentBitArray = pParent->GeneAs<BitArray>(i);
			auto pTrialBitArray = pTrial->GeneAs<BitArray>(i);

			int numParams	= pTrialBitArray->BitLength();
			int jrand		= int( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimension
			for( int j=0; j<numParams; ++j )
			{
				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR/*PR*/ || j==jrand )
				{
					if( OreOreLib::genrand_real1() < pAttrib->F/*MUT*/ )
					{
						pTrialBitArray->Flip( j );// Mutation
					}
					else
					{
						pTrialBitArray->SetBit( j, pParentBitArray->GetBit( j ) );// Crossover
					}
				}

			}// end of design parameter loop

		}// end of i loop

	}


}// end of namespace