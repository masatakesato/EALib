#include	"BinaryBinomialCrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"Chromosome1D.h"



namespace ealib
{

	BinaryBinomialCrossover::BinaryBinomialCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray>, { 0.0f, CrossoverAttribute::Dynamic, 1 } )
	{

	}



	BinaryBinomialCrossover::~BinaryBinomialCrossover()
	{

	}



	void BinaryBinomialCrossover::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial	= children[0];// trial

		for( int i=0; i<pTrial->Size(); ++i )
		{
			const auto& pParentBitArray1 = parents[0]->GeneAs<BitArray>(i);
			auto& pTrialBitArray = pTrial->GeneAs<BitArray>(i);

			int32 numParams	= pTrialBitArray.BitLength<int32>();
			int32 jrand		= int32( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int j=0; j<numParams; ++j )
			{
				uint32 t_j = pTrialBitArray.GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					// Apply Mutation. parents[0] + F * ( parents[1] - parents[2] ) + F * ( parents[3] - parents[4] )...
					uint32 accum = 0;
					for( int k=1; k<numparents; k+=2 )
						accum |= ( uint32(parents[k]->GeneAs<BitArray>(i).GetBit( j )) ^ uint32(parents[k+1]->GeneAs<BitArray>(i).GetBit( j )) );// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );

					t_j	= uint32(pParentBitArray1.GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

					pTrialBitArray.SetBit( j, (bool)t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}//end of i loop

	}



	void BinaryBinomialCrossover::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pX0	= X[0]->GetChromosomeByType( TypeID );
		IChromosome* pTrial	= T[0]->GetChromosomeByType( TypeID );// trial

		for( int i=0; i<pTrial->Size(); ++i )
		{
			const auto& pBParent = pX0->GeneAs<BitArray>(i);
			auto& pBTrial = pTrial->GeneAs<BitArray>(i);

			int32 numParams	= pBTrial.BitLength<int32>();
			int32 jrand		= int32( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int32 j=0; j<numParams; ++j )
			{
				uint32 t_j = pBTrial.GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					// Apply Mutation. X[0] + F * ( X[1] - X[2] ) + F * ( X[3] - X[4] )...
					uint32 accum = 0;
					for( int32 k=1; k<X.Length<int32>(); k+=2 )
					{
						accum |= (	uint32( X[k]->GetChromosomeByType(TypeID)->GeneAs<BitArray>(i).GetBit( j )) ^
									uint32( X[k+1]->GetChromosomeByType(TypeID)->GeneAs<BitArray>(i).GetBit( j ))
								);// altered '+=' by 'OR', '-' by 'XOR'  //( pParents[i]->Gene( j ) - pParents[i+1]->Gene( j ) );
					}

					t_j	= uint32(pBParent.GetBit( j )) | uint32(pAttrib->F * (float)accum);// altered '+' by 'OR' 

					pBTrial.SetBit( j, (bool)t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}//end of i loop

	}





}// end of namespace