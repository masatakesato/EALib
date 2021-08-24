#include	"EvenOddCrossover.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"



namespace ealib
{

	EvenOddCrossover::EvenOddCrossover()
		: ICrossoverOperator( TYPE_ID<BitArray> )
	{

	}



	EvenOddCrossover::~EvenOddCrossover()
	{

	}



	void EvenOddCrossover::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		for( int i=0; i<chromosomes[0]->Size(); ++i )
		{
			const BitArray *pParent1	= chromosomes[0]->GeneAs<BitArray>(i);
			const BitArray *pParent2	= chromosomes[1]->GeneAs<BitArray>(i);
			BitArray *pChild1			= chromosomes[2]->GeneAs<BitArray>(i);
			BitArray *pChild2			= chromosomes[3]->GeneAs<BitArray>(i);

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


}// end of namespace