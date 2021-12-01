#include	"HammingDECrossover.h"

#include	<oreore/mathlib/MersenneTwister.h>

#include	"Typedefs.h"
#include	"DEAttribute.h"
#include	"IChromosome.h"



namespace ealib
{

	HammingDECrossover::HammingDECrossover()
		: ICrossoverOperator( TYPE_ID<BitArray>, { 0.0f, 3, 1 } )
	{

	}


	HammingDECrossover::~HammingDECrossover()
	{

	}



	void HammingDECrossover::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;
		IChromosome* pTrial			= children[0];

		for( int i=0; i<pTrial->Size(); ++i )
		{
			auto& pTrialBitArray			= pTrial->GeneAs<BitArray>(i);
			const auto& pParentBitArray1	= parents[0]->GeneAs<BitArray>(i);
			const auto& pParentBitArray2	= parents[1]->GeneAs<BitArray>(i);
			const auto& pParentBitArray3	= parents[2]->GeneAs<BitArray>(i);

			int32 numParams	= static_cast<int32>( pTrialBitArray.BitLength() );
			int32 jrand		= int32( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int32 j=0; j<numParams; ++j )
			{
				int32 t_j	= pTrialBitArray.GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					int32	x_r1_j = pParentBitArray1.GetBit( j ),
							x_r2_j = pParentBitArray2.GetBit( j ),
							x_r3_j = pParentBitArray3.GetBit( j );

					int32 fa = (int32)round( pAttrib->F * float( x_r2_j != x_r3_j ) );
					t_j	= int( x_r1_j != fa );

					pTrialBitArray.SetBit( j, t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}// end of i loop

	}



	void HammingDECrossover::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		const DEAttribute *pAttrib	= (DEAttribute*)attribs;

		const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
		const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
		const IChromosome* pParent3	= X[2]->GetChromosomeByType( TypeID );
		IChromosome* pTrial			= T[0]->GetChromosomeByType( TypeID );

		for( int32 i=0; i<pTrial->Size(); ++i )
		{
			auto& pBTrial			= pTrial->GeneAs<BitArray>(i);
			const auto& pBParent1	= pParent1->GeneAs<BitArray>(i);
			const auto& pBParent2	= pParent2->GeneAs<BitArray>(i);
			const auto& pBParent3	= pParent3->GeneAs<BitArray>(i);

			int32 numParams	= pBTrial.BitLength<int32>();
			int32 jrand		= int32( OreOreLib::genrand_real2() * numParams );

			// Select Crossover point from dimention
			for( int32 j=0; j<numParams; ++j )
			{
				int t_j	= pBTrial.GetBit( j );

				// Crossover
				if( OreOreLib::genrand_real1() < pAttrib->CR || j==jrand )
				{
					int32	x_r1_j = pBParent1.GetBit( j ),
							x_r2_j = pBParent2.GetBit( j ),
							x_r3_j = pBParent3.GetBit( j );

					int32 fa = (int32)round( pAttrib->F * float( x_r2_j != x_r3_j ) );
					t_j	= int32( x_r1_j != fa );

					pBTrial.SetBit( j, t_j );
				}
				else
				{
					// *t_j = x_i_j;// pChildren[0] is assumed to be initialized with x_i
				}

			}// end of design parameter loop

		}// end of i loop

	}



}// end of namespace