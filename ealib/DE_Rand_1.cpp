#include	"DE_Rand_1.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Rand_1::DE_Rand_1()
		: IDEMutationStrategy()
	{
		
	}



	DE_Rand_1::~DE_Rand_1()
	{
		
	}



	void DE_Rand_1::Execute( OreOreLib::Memory<const IChromosome*>& selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int numChroms = m_refChromosomes.Length();

		int r1	= int( OreOreLib::genrand_real2() * (double)numChroms );
		int r2	= int( OreOreLib::genrand_real2() * (double)numChroms );
		int r3	= int( OreOreLib::genrand_real2() * (double)numChroms );
		
		if( numChroms >= 4 )
		{
			while( r1==current )
				r1	= int( OreOreLib::genrand_real2() * (double)numChroms );

			while( r2==r1 || r2==current )
				r2	= int( OreOreLib::genrand_real2() * (double)numChroms );

			while( r3==r1 || r3==r2 || r3==current )
				r3	= int( OreOreLib::genrand_real2() * (double)numChroms );
		}
		
		selections[0] = m_refChromosomes[r1];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
	}




}// end of namespace