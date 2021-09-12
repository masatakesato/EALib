#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome2D	g_FloatChromosome;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	Chromosome2D	chromosome;


	RandomInitializer initRandom;



	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, 0.1f, 0.5f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib2" ), 0, -100, 100, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-1" ), (int16)0, (int16)-10, (int16)10, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-2" ), (int16)0, (int16)64, (int16)251, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-3" ), (int16)0, (int16)-10, (int16)10, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-4" ), (int16)0, (int16)-10, (int16)10, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib4" ), 0.0f, -10.0f, 7.5f, 0.0f, BoundaryType::Exclusive, BoundaryType::Exclusive, SamplingType::Enumerated },

		{ TYPE_ID<BitArray>, _T( "bitarr" ), (int64)0, (int64)0, (int64)9, (int64)0, BoundaryType::Exclusive, BoundaryType::Exclusive, SamplingType::Enumerated },
	};


	tcout << "//==================== Create GA2DChromosome	========================//" << tendl;
	chromosome.Init( designParams );


	chromosome.SetKey( _T("attrib4"), _T("Renamed_Attrib4") );

	// 変数の参照渡しであれば、型指定抜きで値代入可
	int16 a= 71, b=80;
	chromosome.GetDesignParameter( _T( "attrib3-2" ) ).SetLowerBoundary( a );
	chromosome.GetDesignParameter( _T( "attrib3-2" ) ).SetUpperBoundary( b );
	chromosome.Initialize( &initRandom );

	chromosome.GeneAs<float>( _T( "attrib1" ) ) = -99999.435f;


	tcout << chromosome.GeneAs<float>( _T( "attrib1" ) )  << tendl;

	DisplayChromosome( &chromosome, true );


	tcout << "//================== changing key ========================//" << tendl;
	tcout << "SetKey attrib1 -> Obj1.attrib;\n";
	tcout << "SetKey [4] -> pppp.-+45y9%;\n";
	tcout << tendl;
	chromosome.SetKey( _T("attrib1"), _T("Obj1.attrib") );
	chromosome.SetKey( 4, _T( "pppp.-+45y9%" ) );

	DisplayChromosome( &chromosome, true );


	tcout << "//================== changing value ========================//" << tendl;
	tcout << "Set Obj1.attrib to -41.33f;\n";
	tcout << "Set pppp.-+45y9% to -2465;\n";
	tcout << tendl;
	chromosome.GeneAs<float>( _T( "Obj1.attrib" ) ) = -41.33f;
	chromosome.GeneAs<short>( _T( "pppp.-+45y9%" ) ) = -2465;

	DisplayChromosome( &chromosome, true );


	tcout << "//==================== Copy GA2DChromosome ========================//" << tendl;
	Chromosome2D chromosome2;

	chromosome2	= chromosome;
	
	chromosome.Release();


//chromosome2.Initialize();
	chromosome2.GeneAs<float>( _T( "Obj1.attrib" ) ) = -5521.0f;

	DisplayChromosome( &chromosome2, true );

	chromosome2.Release();


	return 0;
}