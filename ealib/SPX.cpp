#include	"SPX.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"



namespace ealib
{

	SPX::SPX()
		: ICrossoverOperator( TYPE_ID<float> )
	{

	}



	SPX::~SPX()
	{

	}
	


	void SPX::Execute( int numchroms, IChromosome** chromosomes, const void* attribs )
	{
		//int numParents = chromosomes[0]->Size() + 1;
		//int numChildren = numchroms - numParents;

		//assert( numChildren > 0 );

		//IChromosome** pTrials	= &chromosomes[ 0 ];
		//IChromosome** pParents	= &chromosomes[ numChildren ];


		// C = Center of Mass( pParents )

		// scale polygon by epsilon// Pi = epsilon * ( pParents[i] - G )


		// random sample polygon surface


// TODO: Refactor ICrossroverOperator to preallocate Pi.


	}



}// end of namespace