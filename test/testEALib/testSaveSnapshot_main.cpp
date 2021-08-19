#include	<labworks/EA/EALib.h>

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

	
	DataIO::ParamSetArray params;
	IO.LoadCSV( _T( "data.csv" ), params );

	Chromosome1D<int32> chromosome1d_int;
	IO.LoadChromosome1D( chromosome1d_int, params );
	DisplayChromosome( &chromosome1d_int, true );
	//chromosome1d_int.Release();


	//Chromosome1D chromosome1d_float;
	//InitChromosome1DFromCSV( chromosome1d_float, _T( "data.csv" ), TYPE_ID<float32> );
	//DisplayChromosome( &chromosome1d, true );
	//chromosome1d_float.Release();


	Chromosome2D chromosome2d;
	IO.LoadChromosome2D( chromosome2d, params );
	DisplayChromosome( &chromosome2d, true );
	//chromosome2d.Release();



	//g_Population.Init( &chromosome1d_int, 16 );
	g_Population.Init( pChromosome, 16, Eval.NumObjectives() );
	g_Population.Initialize( &initRandom, &Eval );

	IO.ExportSnapshot( &g_Population, _T( "snapshot.csv" ) );


	
	g_Populations[0].Init( pChromosome, 16, Eval.NumObjectives() );
	g_Populations[0].Initialize( &initRandom, &Eval );

	g_Populations[1].Init( &chromosome2d, 50, Eval.NumObjectives() );
	g_Populations[1].Initialize( &initRandom, &Eval );

	IO.ExportSnapshots( 2, g_Populations, _T( "composed_snapshots.csv" ) );
	


	SafeDelete( pChromosome );



	return 0;

}