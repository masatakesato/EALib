#include	"ChromosomeViewer.h"



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
		if( pop->NumIndividuals()==0 )
		{
			tcout << typeid(*pop).name() << _T("...Empty\n");
			return;
		}

		//Sort( SORT_MODE::SORT_FITNESS_DESCEND );

		tcout << typeid(*pop).name() << _T(":\n");

		tcout << _T( "  Best " );
		DisplayChromosome( pop->Individual(0), viewgene );

		tcout << _T( "  Worst " );
		DisplayChromosome( pop->Individual(pop->NumIndividuals()-1), viewgene );

		//for( int i=0; i<num; ++i )
		//	DisplayChromosome( pop->Individual(i), viewgene );
	}


}// end of namespace