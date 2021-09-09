#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome2D	g_Chromosome;
MixedSimpleGA	g_MixedSGASolver;
MixedCoDE		g_MixedCoDESolver;
MixedSHADE		g_MixedSHADESolver;
MultiIslandEA	g_MIEASolver;
Population		g_SnapShot;



class FloatAndShort: public IObjectiveFunction
{

	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		Chromosome2D	*pParams	= (Chromosome2D *)chromosome;

		float param1	= *pParams->GeneAs<float>( _T( "attrib1" ) );// [0.1, 0.5]
		short param2	= *pParams->GeneAs<short>( _T( "attrib3-2" ) );// [64, 251]
		short param3	= *pParams->GeneAs<short>( _T( "attrib3-3" ) );// [-250,128]

		float sign		= *pParams->GeneAs<short>( _T( "attrib3-4" ) )==1 ? 1.0f : -1.0f;

		float result = cos( param1 ) * float( param2 ) * float( param3 ) * sign;

		return result;
	}
};


FloatAndShort	g_Evaluator;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MAXIMIZE );


	//==============	Init Chromosome	================//
/*
	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, 0.1f, 0.5f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "attrib2" ), 0, 0, 0, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "attrib3-1" ), (int16)0, (int16)0, (int16)0, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-2" ), (int16)0, (int16)64, (int16)251, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-3" ), (int16)0, (int16)-250, (int16)128, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3-4" ), (int16)0, (int16)0, (int16)1, (int16)0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "attrib4" ), 0.0f, -10.0f, -7.5f, 0.0f, BoundaryType::Exclusive, BoundaryType::Exclusive, SamplingType::Enumerated },
	};
*/
	//g_Chromosome.Init( designParams );

	DesignParamArray designParams;


	DesignParameter p{ _T( "attrib1" ), 0.0f, 0.1f, 0.5f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated };
	AddDesignParam( designParams, p );
	AddDesignParam( designParams, { _T( "attrib2" ), 3, -5, 5, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated } );
	RemoveDesignParam( designParams, _T("attrib2") );

//	designParams.Release();

	SetBoundary( designParams, _T("attrib1"), -999.9f, 333.3f );

	g_Chromosome = Chromosome2D( designParams );

	DisplayChromosome( &g_Chromosome, true );
	// DisplayPopulation

	return 0;
}




//#include	<math.h>
//
//#include	<oreore/common/Utility.h>
//
//
//
//
//double Ackley( double x1, double x2 )
//{
//	const double n = 2.0;
//	const double c = cos( 2.0*M_PI*x1 ) + cos( 2.0*M_PI*x2 );
//	return 20.0 + exp( 1.0 ) - 20.0*exp( -0.2*sqrt( 1.0/n*(x1*x1 + x2*x2) ) )
//		- exp( 1.0/n*c );
//}