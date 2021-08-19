﻿#include	"ChromosomeViewer.h"



namespace ealib
{

	//##################################################################################################//
	//																									//
	//										Helper functions											//
	//																									//
	//##################################################################################################//


	const ChromosomeViewer< g_ChoromosomeTypes > c_Viewr;


	// Chromosome display function
	void DisplayChromosome( const IChromosome* chromosome, bool viewgene )
	{
		assert( chromosome != nullptr );

		c_Viewr.View( chromosome, chromosome->TypeInfo(), viewgene );
	}



	// Population display function
	void DisplayPopulation( const Population* pop, bool viewgene )
	{
		//Sort( SORT_MODE::SORT_FITNESS_DESCEND );

		tcout << typeid(*pop).name() << _T(":\n");

		tcout << _T( "  Best " );
		DisplayChromosome( pop->GetIndividual(0), viewgene );

		tcout << _T( "  Worst " );
		DisplayChromosome( pop->GetIndividual(pop->PopulationSize()-1), viewgene );

		//for( int i=0; i<num; ++i )
		//	DisplayChromosome( pop->GetIndividual(i), viewgene );
	}


}// end of namespace