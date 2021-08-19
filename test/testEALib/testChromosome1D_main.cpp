#include	<crtdbg.h>
#include	<iostream>
using namespace std;


#include	<oreore/common/TString.h>

#include	<labworks/EA/EALib.h>
using namespace ealib;



Chromosome1D<float>	g_Chromosome;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	//============	Init Initializer	========//
	RandomInitializer	initializer;


	//============	Init DesignParameters	================//

	DesignParamArray designParams =
	{
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -1.0f, 1.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T("abssrf"), 0.0f, -5.0f, 5.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ TYPE_ID<BitArray>, _T("Bitset1"), 0, 0, 18, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ TYPE_ID<BitArray>, _T("Bitset2"), 0, 0, 5, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};



	//============ Init Chromosome	================//
	g_Chromosome.Init( designParams );
	g_Chromosome.Initialize( &initializer );

	for( int i=0; i<designParams.Length(); ++i )
	{
		g_Chromosome.GetDesignParameter(i)->SetLowerBoundary( 0.0f + float(i) );
		g_Chromosome.GetDesignParameter(i)->SetUpperBoundary( 0.1f + float(i) );
	}


	g_Chromosome.GetDesignParameter( _T("abssrf") )->SetUpperBoundary( 6.5f );

	// change key
	g_Chromosome.SetKey( _T( "abssrf" ), _T( "newkey" ) );
	g_Chromosome.SetKey( 0, _T( "Obj1.param1" ) );

	// Randomize
	g_Chromosome.Initialize( &initializer );
	DisplayChromosome( &g_Chromosome, true );


	// Manually set value
	tcout << "//========================= Editing param =========================//" << tendl;
	*g_Chromosome.GeneAs( _T( "Obj1.param1" ) ) = -56.046f;
	DisplayChromosome( &g_Chromosome, true );

	Chromosome1D<float>	g_Chromosome2;
	g_Chromosome2 = g_Chromosome;

	g_Chromosome.Release();


	tcout << "//======= Display copied chromosome after removing original ========//" << tendl;
	DisplayChromosome( &g_Chromosome2, true );


	return 0;
}



