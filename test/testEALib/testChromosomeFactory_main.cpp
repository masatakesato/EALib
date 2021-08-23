#include	<chrono>
#include	<crtdbg.h>
#include	<iostream>
using namespace std;


#include	<ealib/EALib.h>
using namespace ealib;



//Chromosome1D<float>	g_fChromosome;



int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	

	ChromosomeFactory<g_ChoromosomeTypes>  factory;


	//============== Init Initializer ===============//
	RandomInitializer	initRandom;


	//==============	Init Chromosome	================//
	//DesignParamArray designParams =
	//{
	//	{ _T( "attrib0" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib1" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib2" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib3" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib4" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib5" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib6" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//	{ _T( "attrib7" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	//};

	DesignParamArray designParams =
	{
		{ TYPE_ID<BitArray>, _T("Bitset1"), 0, 0, 18, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ TYPE_ID<BitArray>, _T("Bitset2"), 0, 0, 5, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib0" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};

	
	auto chromosome = factory.Create( designParams );


	chromosome->Initialize( &initRandom );

	DisplayChromosome( chromosome, true );


	SafeDelete( chromosome );


	return 0;
}
