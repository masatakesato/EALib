#include	<ealib/EALib.h>
using namespace ealib;



int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	
	DataIO IO;
	RandomInitializer initRandom;

	DesignParamArray designParams;
	IO.LoadDesignParams( designParams, _T( "data.csv" ) );



	tcout << "//============== IO.LoadChromosome_( _T( \"data.csv\" ) ================//\n\n";

	IChromosome *pChromosome2	= IO.LoadChromosome( _T( "data.csv" ) );
	pChromosome2->Initialize( &initRandom );
	DisplayChromosome( pChromosome2, true );
	SafeDelete( pChromosome2 );

	tcout << tendl;



	tcout << "//======= IO.LoadChromosome1D_<int32>( chromosome1d__i32, params ) ========//\n\n";

	Chromosome1D<int32> chromosome1d__i32;
	IO.LoadChromosome1D( chromosome1d__i32, _T("data.csv") );
	DisplayChromosome( &chromosome1d__i32, true );
	chromosome1d__i32.Release();

	tcout << tendl;


	tcout << "//======= IO.LoadChromosome1D<float32>( chromosome1d__f32, params ) ========//\n\n";

	Chromosome1D<float32> chromosome1d__f32;
	IO.LoadChromosome1D( chromosome1d__f32, _T("data.csv") );
	DisplayChromosome( &chromosome1d__f32, true );
	chromosome1d__f32.Release();

	tcout << tendl;


	tcout << "//============== IO.LoadChromosome2D( chromosome2d, params ) ================//\n\n";

	Chromosome2D chromosome2d;
	IO.LoadChromosome2D( chromosome2d, _T("data.csv") );
	DisplayChromosome( &chromosome2d, true );
	chromosome2d.Release();

	tcout << tendl;


	return 0;

}