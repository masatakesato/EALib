#include	"DE_Best_1.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Best_1::DE_Best_1() : IDEMutationStrategy()
	{
		
	}



	DE_Best_1::~DE_Best_1()
	{

	}



	void DE_Best_1::Execute( OreOreLib::Memory<const IChromosome*>& selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int32 numChroms = m_refChromosomes.Length<int32>();

		int32 best	= 0;
		int32 r2		= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r3		= int32( OreOreLib::genrand_real2() * (double)numChroms );

		// Avoid duplicated selection if possible
		if( numChroms >= 4 )
		{
			while( r2==best || r2==current )
				r2	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r3==best || r3==r2 || r3==current )
				r3	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		}

		selections[0] = m_refChromosomes[best];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
	}




}// end of namespace