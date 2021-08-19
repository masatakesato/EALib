#include	"DE_Current_to_Best_1.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Current_to_Best_1::DE_Current_to_Best_1() : IDEMutationStrategy()
	{
		
	}


	DE_Current_to_Best_1::~DE_Current_to_Best_1()
	{
		
	}


	void DE_Current_to_Best_1::Execute( int num, IChromosome **selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int best = 0;
		int r3	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r4	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r5	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

		if( m_NumChroms >= 5 )
		{
			while( r3==best || r3==current )
				r3	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r4==best || r4==r3 || r4==current )
				r4	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r5==best || r5==r3 || r5==r4 || r5==current )
				r5	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		}

		selections[0] = m_refChromosomes[current];
		selections[1] = m_refChromosomes[best];
		selections[2] = m_refChromosomes[r3];
		selections[3] = m_refChromosomes[r4];
		selections[4] = m_refChromosomes[r5];
	}




}// end of namespace