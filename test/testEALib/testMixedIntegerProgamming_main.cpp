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


	//=============	Init Mutator	============//
	GaussianMutation<float>	float_mutation_Gaussian;
	GaussianMutation<short>	short_mutation_Gaussian;

	Mutator mutator;
	mutator.BindOperator( &float_mutation_Gaussian );
	mutator.BindOperator( &short_mutation_Gaussian );


	//============	Init Crossover	============//
	// GA real coded
	SBX							crossover_SBX;
	BLXalpha					crossover_BLXalpha;
	// GA integer coded
	OnePointCrossover<short>	crossover_OnePoint;
	TwoPointCrossover<short>	crossover_TwoPoint;
	NPointCrossover<short, 2>	crossover_NPoint;
	UniformCrossover<short>		crossover_Uniform;

	Crossover crossover_ga;
	crossover_ga.BindOperator( &crossover_BLXalpha );
	crossover_ga.BindOperator( &crossover_Uniform );

	// DE
	BinomialCrossover<float>	crossover_BinominalFloat;
	BinomialCrossover<short>	crossover_BinominalShort;
	ExponentialCrossover<float>	crossover_ExponentialFloat;
	ExponentialCrossover<short>	crossover_ExponentialShort;

	Crossover crossover_de;
	crossover_de.BindOperator( &crossover_BinominalFloat );//&crossover_ExponentialFloat
	crossover_de.BindOperator( &crossover_BinominalShort );//&crossover_ExponentialShort


	//==============	Init Chromosome	================//
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

	g_Chromosome.Init( designParams );


	//====== test Mixed SimpleGA	=======//
	tcout << "//############# Mixed SimpleGA #############//" << tendl;

	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );

	RouletteWheelSelector rouletteSelector;


	g_MixedSGASolver.SetPopulationSize( 50 );
	g_MixedSGASolver.SetNumGenerations( 100 );
	g_MixedSGASolver.SetEliteSize( 1 );
	g_MixedSGASolver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	g_MixedSGASolver.SetMutationRate( 0.0075f );// 0.5 - 1.0[%]
	g_MixedSGASolver.BindSelector( &tournamentSelector );//&rouletteSelector );// 
	g_MixedSGASolver.BindMutator( &mutator );
	g_MixedSGASolver.BindCrossover( &crossover_ga );

	g_MixedSGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MixedSGASolver.Evolve( &Eval );
	g_MixedSGASolver.TakeSnapshot( g_SnapShot );
	g_MixedSGASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island GA solver	========//
	tcout << "//####################### Multi-Island Mixed SimpleGA ##################//" << tendl;

	g_MIEASolver.BindSolver( &g_MixedSGASolver );

	g_MIEASolver.SetIslandSize( 25 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	g_MIEASolver.GetSolver()->SetPopulationSize( 50 );//g_MIEASolver.SetPopulationSize( 50 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 100 );// g_MIEASolver.SetNumGenerations( 10 );
	g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );

	//for( int i=0; i<5; ++i )
	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//====== test MixedCoDE	=======//
	tcout << "//############################## Mixed CoDE #############################//" << tendl;

	// set de parameters
	g_MixedCoDESolver.SetPopulationSize( 50 );
	g_MixedCoDESolver.SetNumGenerations( 100 );
	//g_MixedCoDESolver.SetDifferentialWeight( 0.1 );// [0, 2]
	//g_MixedCoDESolver.SetCrossoverProbability( 0.1 );// [0, 1]
	g_MixedCoDESolver.BindCrossover( &crossover_de );

	// execute
	g_MixedCoDESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MixedCoDESolver.Evolve( &Eval );
	g_MixedCoDESolver.TakeSnapshot( g_SnapShot );
	g_MixedCoDESolver.ReleasePopulation();
	
	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island Mixed CoDE solver	========//
	tcout << "//####################### Multi-Island Mixed CoDE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_MixedCoDESolver );

	g_MIEASolver.GetSolver()->SetPopulationSize( 32 );//g_MIEASolver.SetPopulationSize( 32 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 500 );// g_MIEASolver.SetNumGenerations( 500 );
	g_MIEASolver.GetSolver()->SetEliteSize( 1 );// g_MIEASolver.SetEliteSize( 1 );

	g_MIEASolver.SetIslandSize( 25 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	// execute
	g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//====== test MixedSHADE =======//
	tcout << "//########################### Mixed SHADE ############################//" << tendl;

	// set de parameters
	g_MixedSHADESolver.SetPopulationSize( 50 );
	g_MixedSHADESolver.SetNumGenerations( 100 );

	// execute
	g_MixedSHADESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MixedSHADESolver.Evolve( &Eval );
	g_MixedSHADESolver.TakeSnapshot( g_SnapShot );
	g_MixedSHADESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island Mixed SHADE solver	========//
	tcout << "//####################### Multi-Island Mixed SHADE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_MixedSHADESolver );

	g_MIEASolver.GetSolver()->SetPopulationSize( 32 );//g_MIEASolver.SetPopulationSize( 32 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 500 );// g_MIEASolver.SetNumGenerations( 500 );
	g_MIEASolver.GetSolver()->SetEliteSize( 1 );// g_MIEASolver.SetEliteSize( 1 );

	g_MIEASolver.SetIslandSize( 25 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	// execute
	g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );


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