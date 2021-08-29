#include	"EvenOddCrossover.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"



namespace ealib
{

	EvenOddCrossover::EvenOddCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray>, { 0.0f, 2, 2 } )
	{

	}



	EvenOddCrossover::~EvenOddCrossover()
	{

	}



	void EvenOddCrossover::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		for( int i=0; i<(*parents)->Size(); ++i )
		{
			const BitArray *pParent1	= parents[0]->GeneAs<BitArray>(i);
			const BitArray *pParent2	= parents[1]->GeneAs<BitArray>(i);
			BitArray *pChild1			= children[0]->GeneAs<BitArray>(i);
			BitArray *pChild2			= children[1]->GeneAs<BitArray>(i);

			int bitLength = Min(Min(Min(pParent1->BitLength(), pParent2->BitLength()), pChild1->BitLength()), pChild2->BitLength());

			pChild1->CopyFrom( pParent1 );
			pChild2->CopyFrom( pParent2 );

			for( int j=0; j<bitLength; j+=2 )
			{
				pChild1->SetBit(j, pParent2->GetBit(j));// +-+-+-+-
				pChild2->SetBit(j, pParent1->GetBit(j));// -+-+-+-+
			}// end of j loop

		}// end of i loop

	}



	void EvenOddCrossover::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		const IChromosome* pParent1	= X[0]->GetChromosomeByType( TypeID );
		const IChromosome* pParent2	= X[1]->GetChromosomeByType( TypeID );
		IChromosome* pChild1		= T[0]->GetChromosomeByType( TypeID );
		IChromosome* pChild2		= T[1]->GetChromosomeByType( TypeID );

		for( int i=0; i<pParent1->Size(); ++i )
		{
			const BitArray *pBParent1	= pParent1->GeneAs<BitArray>(i);
			const BitArray *pBParent2	= pParent2->GeneAs<BitArray>(i);
			BitArray *pBChild1			= pChild1->GeneAs<BitArray>(i);
			BitArray *pBChild2			= pChild2->GeneAs<BitArray>(i);

			int bitLength = Min(Min(Min(pBParent1->BitLength(), pBParent2->BitLength()), pBChild1->BitLength()), pBChild2->BitLength());

			pBChild1->CopyFrom( pBParent1 );
			pBChild2->CopyFrom( pBParent2 );

			for( int j=0; j<bitLength; j+=2 )
			{
				pBChild1->SetBit(j, pBParent2->GetBit(j));// +-+-+-+-
				pBChild2->SetBit(j, pBParent1->GetBit(j));// -+-+-+-+
			}// end of j loop

		}// end of i loop		
	
	}



}// end of namespace