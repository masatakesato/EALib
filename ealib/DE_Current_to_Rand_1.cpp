﻿#include	"DE_Current_to_Rand_1.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Current_to_Rand_1::DE_Current_to_Rand_1()
		: IDEMutationStrategy()
	{
		
	}



	DE_Current_to_Rand_1::~DE_Current_to_Rand_1()
	{
		
	}



	void DE_Current_to_Rand_1::Execute( OreOreLib::Memory<const IChromosome*>& selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int32 numChroms = m_refChromosomes.Length<int32>();

		int32 r2	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r3	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r4	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		int32 r5	= int32( OreOreLib::genrand_real2() * (double)numChroms );

		if( numChroms >= 5 )
		{
			while( r2==current )
				r2	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r3==r2 || r3==current )
				r3	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r4==r2 || r4==r3 || r4==current )
				r4	= int32( OreOreLib::genrand_real2() * (double)numChroms );

			while( r5==r2 || r5==r3 || r5==r4 || r5==current )
				r5	= int32( OreOreLib::genrand_real2() * (double)numChroms );
		}

		selections[0] = m_refChromosomes[current];
		selections[1] = m_refChromosomes[r2];
		selections[2] = m_refChromosomes[r3];
		selections[3] = m_refChromosomes[r4];
		selections[4] = m_refChromosomes[r5];
	}




}// end of namespace