#include	"TournamentSelector.h"

#include	<float.h>

#include	<oreore/mathlib/MersenneTwister.h>

#include	"IChromosome.h"
#include	"Population.h"



namespace ealib
{

	TournamentSelector::TournamentSelector() : ISelector()
	{
		m_TournamentSize	= 2;// default value
	}



	TournamentSelector::~TournamentSelector()
	{

	}



	int TournamentSelector::Execute()
	{
		int selected	= -1;
		float max		= -FLT_MAX;

		if( !m_refChromosomes )
			return -1;

		for( int i=0; i<m_TournamentSize; ++i )
		{
			int r		= int( OreOreLib::genrand_real1() * double( m_refChromosomes.Length() - 1 ) );
			float score	= m_refChromosomes[r]->Fitness();
			if( score > max )
			{
				selected	= r;
				max			= score;
			}
		}// end of i loop

		return selected;
	}


}// end of namespace