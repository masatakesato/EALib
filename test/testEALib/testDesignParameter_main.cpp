#include	<crtdbg.h>

#include	<oreore/common/TString.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome2D	g_Chromosome;


int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );



	tcout << _T("//===================== Construct designParams ======================//\n" );

	DesignParamArray designParams;


	DesignParameter p{ _T( "attrib1" ), 0.0f, 0.1f, 0.5f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated };
	AddDesignParam( designParams, p );
	AddDesignParam( designParams, { _T( "attrib2" ), 3, -5, 5, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated } );
	AddDesignParam( designParams, { _T( "attrib3" ), 10.0f, 3.14f, 6.28f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated } );
	AddDesignParam( designParams, { _T( "attrib4" ), 30, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated } );

	SetBoundary( designParams, _T("attrib1"), -999.9f, 333.3f );

	g_Chromosome = Chromosome2D( designParams );
	DisplayChromosome( &g_Chromosome, true );

	tcout << tendl;


	tcout << _T("//===================== Remove attrib2 ======================//\n" );

	RemoveDesignParam( designParams, _T("attrib2") );

	g_Chromosome = Chromosome2D( designParams );
	DisplayChromosome( &g_Chromosome, true );

	tcout << tendl;


	tcout << _T("//===================== Filter designParams by type int32 ======================//\n" );

	DesignParamArray designParams2;

	FilterDesignParamsByType<int32>( designParams2, designParams );

	g_Chromosome = Chromosome2D( designParams2 );
	DisplayChromosome( &g_Chromosome, true );

	tcout << tendl;

	return 0;
}
