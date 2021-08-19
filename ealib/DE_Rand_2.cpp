#include	"DE_Rand_2.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"


namespace ealib
{

	DE_Rand_2::DE_Rand_2() : IDEMutationStrategy()
	{
		
	}


	DE_Rand_2::~DE_Rand_2()
	{
		
	}


	void DE_Rand_2::Execute( int num, IChromosome** selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int r1	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r2	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r3	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r4	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r5	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

		if( m_NumChroms >= 6 )
		{
			while( r1 == current )
				r1	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r2==r1 || r2==current )
				r2	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r3==r1 || r3==r2 || r3==current )
				r3	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r4==r1 || r4==r2 || r4==r3 || r4==current )
				r4	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r5==r1 || r5==r2 || r5==r3 || r5==r4 || r5==current )
				r5	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		}
		
		selections[0] = m_refChromosomes[r1];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
		selections[3] = m_refChromosomes[r4];
		selections[4] = m_refChromosomes[r5];
	}




}// end of namespace