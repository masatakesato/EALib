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


	void DE_Best_1::Execute( int num, IChromosome** selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int best	= 0;
		int r2		= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r3		= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

		// Avoid duplicated selection if possible
		if( m_NumChroms >= 4 )
		{
			while( r2==best || r2==current )
				r2	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r3==best || r3==r2 || r3==current )
				r3	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		}

		selections[0] = m_refChromosomes[best];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
	}




}// end of namespace