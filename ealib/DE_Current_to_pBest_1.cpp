#include	"DE_Current_to_pBest_1.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Current_to_pBest_1::DE_Current_to_pBest_1() : IDEMutationStrategy()
	{
		m_P	= 1.0f;
	}


	DE_Current_to_pBest_1::~DE_Current_to_pBest_1()
	{
		
	}


	void DE_Current_to_pBest_1::SetP( float p )
	{
		m_P	= Saturate( p );
	}


	void DE_Current_to_pBest_1::Execute( int num, IChromosome** selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int pbest = int( OreOreLib::genrand_real2() * (double)m_P * (double)m_NumChroms );
		int r1	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		int r2	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

		if( m_NumChroms >= 4 )
		{
			while( r1==pbest || r1==current )
				r1	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			while( r2==pbest || r2==r1 || r2==current )
				r2	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );
		}

		selections[0] = m_refChromosomes[current];
		selections[1] = m_refChromosomes[pbest];
		selections[2] = m_refChromosomes[current];
		selections[3] = m_refChromosomes[r1];
		selections[4] = m_refChromosomes[r2];
	}




}// end of namespace