#include	"DE_Best_2.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Best_2::DE_Best_2() : IDEMutationStrategy()
	{
		
	}



	DE_Best_2::~DE_Best_2()
	{
		
	}



	void DE_Best_2::Execute( OreOreLib::Memory<IChromosome*> selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int32 numChroms = m_refChromosomes.Length<int32>();

		int32 best = 0;
		int32 r2	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r3	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r4	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r5	= int32( OreOreLib::genrand_real2() * (double)numChroms );

		if( numChroms >= 6 )
		{
			while( r2==best || r2==current )
				r2	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r3==best || r3==r2 || r3==current )
				r3	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r4==best || r4==r2 || r4==r3 || r4==current )
				r4	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r5==best || r5==r2 || r5==r3 || r5==r4 || r5==current )
				r5	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		}

		selections[0] = m_refChromosomes[best];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
		selections[3] = m_refChromosomes[r4];
		selections[4] = m_refChromosomes[r5];
	}




}// end of namespace