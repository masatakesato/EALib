#include	<ealib/EALib.h>

using namespace ealib;



class DummyEvaluator: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		return 555555.5f;
	}

};


DummyEvaluator	g_DummyEval;

Population	g_Population;
Population	g_Populations[2];



int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	DataIO IO;


	//============== Init Initializer ===============//
	RandomInitializer	initRandom;



	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_DummyEval, SOLVE_MODE::MAXIMIZE );


	IChromosome *pChromosome	= IO.LoadChromosome( _T( "data.csv" ) );
	DisplayChromosome( pChromosome, true );
	//SafeDelete( pChromosome );

	
	Chromosome1D<int32> chromosome1d_int;
	IO.LoadChromosome1D( chromosome1d_int, _T("data.csv") );
	DisplayChromosome( &chromosome1d_int, true );

	Chromosome2D chromosome2d;
	IO.LoadChromosome2D( chromosome2d, _T("data.csv") );
	DisplayChromosome( &chromosome2d, true );


	{
		tcout << _T( "//===================== Exporting single snapshots ====================//\n" );

		tstring outpath = _T( "snapshot.csv" );

		DesignParamArray designparams;

		pChromosome->ExtractOrderRestoredDesignParamArray( designparams );
		g_Population.Init( designparams, 16, Eval.NumObjectives() );
		g_Population.Initialize( &initRandom, &Eval );

		IO.ExportSnapshot( &g_Population, outpath );

		tcout << _T("  Exported to file ") << outpath << tendl;

	}

	tcout << tendl;

	{
		tcout << _T( "//==================== Exporting composed snapshots ===================//\n" );

		tstring outpath = _T( "composed_snapshots.csv" );

		DesignParamArray designparams;

		pChromosome->ExtractOrderRestoredDesignParamArray( designparams );
		g_Populations[0].Init( designparams, 16, Eval.NumObjectives() );
		g_Populations[0].Initialize( &initRandom, &Eval );

		chromosome2d.ExtractOrderRestoredDesignParamArray( designparams );
		g_Populations[1].Init( designparams, 50, Eval.NumObjectives() );
		g_Populations[1].Initialize( &initRandom, &Eval );

		IO.ExportSnapshots( 2, g_Populations, outpath );
	
		tcout << _T("  Exported to file ") <<  outpath << tendl << tendl;

	}


	SafeDelete( pChromosome );



	return 0;

}