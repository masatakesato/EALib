#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<labworks/EA/EALib.h>
#include	<labworks/EA/BenchmarkFuncs.h>
using namespace ealib;



Chromosome1D<float>	g_Chromosome;
SimpleGA		g_SGASolver;
MultiIslandEA	g_MIEASolver;
Population		g_SnapShot;



AckleyFunction			g_AckleyFunction;
SphereFunction			g_SphereFunction;
RosenbrockFunction		g_RosenbrockFunction;
BealeFunction			g_BealeFunction;
GoldsteinPriceFunction	g_GoldsteinPriceFunction;
BukinFunctionN6			g_BukinFunctionN6;
MatyasFunction			g_MatyasFunction;
LeviFunctionN13			g_LeviFunctionN13;
ThreeHumpCamelFunction	g_ThreeHumpCamelFunctiom;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_AckleyFunction, SOLVE_MODE::MINIMIZE );


	//==========	Init Crossover	=========//
	SBX			crossover_SBX;
	BLXalpha	crossover_BLXalpha;

	Crossover crossover;
	crossover.BindOperator( &crossover_BLXalpha );//&crossover_SBX


	//===========	Init Mutator	========//
	GaussianMutation<float>	mutation_Gaussian;

	Mutator mutator;
	mutator.BindOperator( &mutation_Gaussian );


	//============	Init DesignParameters	================//

	DesignParamArray designParams =
	{
		{ _T(""), 0.0f, -32.768f, 32.768f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -32.768f, 32.768f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -32.768f, 32.768f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0.0f, -32.768f, 32.768f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};


	//============ Init Chromosome	================//
	g_Chromosome.Init( designParams );


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );


	tcout << "//============ test SimpleGA solver	===========//\n";

	g_SGASolver.SetPopulationSize( 50 );
	g_SGASolver.SetEliteSize( 1 );
	g_SGASolver.SetNumGenerations( 100 );
	g_SGASolver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	g_SGASolver.SetMutationRate( 0.0075f );// 0.5 - 1.0[%]
	//g_SGASolver.BindSelector( &tournamentSelector );
	g_SGASolver.BindMutator( &mutator );
	g_SGASolver.BindCrossover( &crossover );

	g_SGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_SGASolver.Evolve( &Eval );
	g_SGASolver.TakeSnapshot( g_SnapShot );
	g_SGASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );


	tcout << tendl;


	tcout << "//========= test Multi-Island GA solver	========//\n";

	g_MIEASolver.BindSolver( &g_SGASolver );
	g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	// set miga parameters
	g_MIEASolver.SetIslandSize( 50 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	// set simple ga parameters
	g_MIEASolver.GetSolver()->SetPopulationSize( 32 );//g_MIEASolver.SetPopulationSize( 32 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 500 );// g_MIEASolver.SetNumGenerations( 500 );
	g_MIEASolver.GetSolver()->SetEliteSize( 1 );// g_MIEASolver.SetEliteSize( 1 );


	// execute
	g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );


	return 0;
}