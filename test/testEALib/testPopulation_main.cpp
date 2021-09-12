#include	<chrono>
#include	<crtdbg.h>
#include	<iostream>
using namespace std;


#include	<ealib/EALib.h>
using namespace ealib;



int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	

	//==============	Init Chromosome	================//

	DesignParamArray designParams =
	{
		{ TYPE_ID<BitArray>, _T("Bitset1"), 0, 0, 18, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ TYPE_ID<BitArray>, _T("Bitset2"), 0, 0, 5, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib0" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};
	

	tcout << "//======================= pop.Init( designParams, 16, 1 ); ============================//\n";
	Population pop;
	pop.Init( designParams, 16, 1 );
	DisplayPopulation( pop, true );

	tcout << tendl;


	tcout << "//======================= Population pop2( designParams, 16, 1 ); ============================//\n";
	Population pop2( designParams, 16, 1 );
	DisplayPopulation( pop2, true );


	return 0;
}
