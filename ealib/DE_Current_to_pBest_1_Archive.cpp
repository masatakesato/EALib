#include	"DE_Current_to_pBest_1_Archive.h"

#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>


namespace ealib
{

	DE_Current_to_pBest_1_Archive::DE_Current_to_pBest_1_Archive()
		: IDEMutationStrategy()
		, m_P( 1 )
		, m_NumActiveArchives( 0 )
	{
		UnbindArchives();
	}



	DE_Current_to_pBest_1_Archive::~DE_Current_to_pBest_1_Archive()
	{
		UnbindArchives();
	}



	void DE_Current_to_pBest_1_Archive::SetP( float p )
	{
		m_P	= Saturate( p );
	}



	void DE_Current_to_pBest_1_Archive::SetNumArchives( int numarchives )
	{
		m_NumActiveArchives	= numarchives;
	}



	void DE_Current_to_pBest_1_Archive::BindArchives( int numarchives, const OreOreLib::Array<IChromosome*>& archives )//BindArchives( int numarchives, IChromosome** archives )
	{
		m_NumActiveArchives	= numarchives;
		m_refArchives.Init( archives );
	}



	void DE_Current_to_pBest_1_Archive::UnbindArchives()
	{
		m_NumActiveArchives	= 0;
		m_refArchives.Release();
	}



	void DE_Current_to_pBest_1_Archive::Execute( OreOreLib::Memory<const IChromosome*>& selections, int current )
	{
		if( !m_refChromosomes )
			return;

		int numChroms	= m_refChromosomes.Length();
		int numtotal	= numChroms + m_NumActiveArchives;

		int pbest = int( OreOreLib::genrand_real2() * (double)m_P * (double)numChroms );
		int r1	= int( OreOreLib::genrand_real2() * (double)numChroms );
		int r2 = int( (double)numtotal * OreOreLib::genrand_real2() );

		if( numChroms >= 4 )
		{
			//while( r1==pbest || r1==current )
			//	r1	= int( OreOreLib::genrand_real2() * (double)m_NumChroms );

			//while( r2==pbest || r2==r1 || r2==current )
			//	r2	= int( (double)numtotal * OreOreLib::genrand_real2() );

			while( r2==pbest || r2==current )
				r2	= int( (double)numtotal * OreOreLib::genrand_real2() );

			while( r1==pbest || r1==r2 || r1==current )
				r1	= int( OreOreLib::genrand_real2() * (double)numChroms );
		}

		selections[0] = m_refChromosomes[current];
		selections[1] = m_refChromosomes[pbest];
		selections[2] = m_refChromosomes[current];
		selections[3] = m_refChromosomes[r1];
		selections[4] = r2>=numChroms ? m_refArchives[r2-numChroms] : m_refChromosomes[r2];
	}




}// end of namespace