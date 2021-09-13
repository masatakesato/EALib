#include	"DataViewer.h"



namespace ealib
{

	//##################################################################################################//
	//																									//
	//										Helper functions											//
	//																									//
	//##################################################################################################//

	// Viewer instances
	const DesignParameterViewer< g_ChoromosomeTypes > c_DesignParamViewer;
	const ChromosomeViewer< g_ChoromosomeTypes > c_ChromViewer;




	// DesignParameter display function
	void DisplayDesignParameter( const DesignParameter& param )
	{
		tcout << typeid(param).name() << _T(":\n");

		c_DesignParamViewer.View( param );

		tcout << tendl;
	}



	// DesignParamArrayr display function
	void DisplayDesignParamArray( const DesignParamArray& params )
	{
		tcout << _T("DesignParamArray:\n");

		for( const auto& param : params )
			c_DesignParamViewer.View( param );

		tcout << tendl;
	}



	// Chromosome display function
	void DisplayChromosome( const IChromosome* chromosome, bool viewgene )
	{
		assert( chromosome != nullptr );

		c_ChromViewer.View( chromosome, chromosome->TypeInfo(), viewgene );
	}



	// Population display function
	void DisplayPopulation( const Population& pop, bool viewgene )
	{
		if( pop.NumIndividuals()==0 )
		{
			tcout << typeid(pop).name() << _T("...Empty\n");
			return;
		}

		//Sort( SORT_MODE::SORT_FITNESS_DESCEND );

		tcout << typeid(pop).name() << _T(":\n");

		tcout << _T("  Best ");
		DisplayChromosome( pop.Individual(0), viewgene );

		tcout << _T("  Worst ");
		DisplayChromosome( pop.Individual(pop.NumIndividuals()-1), viewgene );
	}


}// end of namespace