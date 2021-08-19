#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome1D<float>	g_Chromosome;
MultiIslandEA	g_MIEASolver;
jDE				g_jDESolver;
JADE			g_JADESolver;
CoDE			g_CoDESolver;
SHADE			g_SHADESolver;
Population		g_SnapShot;



class RastriginND: public IObjectiveFunction
{
public:

	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float result = 10.0f * float( chromosome->Size() );

		for( int i=0; i<chromosome->Size(); ++i )
		{
			float xi = *chromosome->GeneAs<float>( i );
			result	+= ( xi * xi -10.0f * cos( 2.0f*M_PI*xi ) );
		}

		//tcout << "result = " << result << tendl;
		return result;
	}

};


RastriginND g_Evaluator;





int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	

	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MINIMIZE );



//	while(1)
	{
	//============	Init Crossover	============//
	// DE
	BinomialCrossover<float>	crossover_Binomial;
	ExponentialCrossover<float>	crossover_Exponential;

	Crossover crossover_de;
	crossover_de.BindOperator( &crossover_Binomial );// &crossover_Exponential


	//=============	Init Mutator	============//

	
	
	//==============	Init Chromosome	================//
	DesignParamArray designParams =
	{
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};

	g_Chromosome.Init( designParams );

//InitChromosome1DFromCSV( g_Chromosome, __T( "D:/data/Repository/programs/Simulation/Project/EvolutionaryAlgorithm/testSHADE/params.csv" ), TYPE_ID<float32> );



	//====== test CoDE solver	=======//
	tcout << "//####################### CoDE #########################//" << tendl;

	// set de parameters
	g_CoDESolver.SetPopulationSize( 100 );
	g_CoDESolver.SetNumGenerations( 1500 );
	g_CoDESolver.BindCrossover( &crossover_de );

	// execute
	g_CoDESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_CoDESolver.Evolve( &Eval );
	g_CoDESolver.TakeSnapshot( g_SnapShot );
	g_CoDESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island CoDE solver	========//
	tcout << "//####################### Multi-Island CoDE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_CoDESolver );

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


	//====== test jDE solver	=======//
	tcout << "//####################### jDE #########################//" << tendl;

	// set de parameters
	g_jDESolver.SetPopulationSize( 100 );
	g_jDESolver.SetNumGenerations( 1500 );
	//g_jDESolver.SetLowerDifferentialWeight( 0.1f );
	//g_jDESolver.SetUpperDifferentialWeight( 0.9f );
	//g_jDESolver.SetT1( 0.1f );
	//g_jDESolver.SetT2( 0.1f );
	g_jDESolver.BindCrossover( &crossover_de );

	// execute
	g_jDESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_jDESolver.Evolve( &Eval );
	g_jDESolver.TakeSnapshot( g_SnapShot );
	g_jDESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;
	

	//========= test Multi-Island jDE solver	========//
	tcout << "//####################### Multi-Island jDE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_jDESolver );

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


	//====== test JADE solver	=======//
	tcout << "//####################### JADE #########################//" << tendl;
	
	// 経過世代数が少ない(500世代etc...)時点では、Binomialが高い適応値を出す
	// 経過世代数が多くなると(1000世代以降?)、Exponentialの方が高い適応値で安定収束する

	// set de parameters
	g_JADESolver.SetPopulationSize( 100 );
	g_JADESolver.SetNumGenerations( 1500 );
	//g_JADESolver.SetDifferentialWeight( 0.5f );// 0.05f );// [0, 2]
	//g_JADESolver.SetCrossoverProbability( 0.5f );// 0.25f );// [0, 1]
	//g_JADESolver.SetBestPercentage( 0.5f );
	g_JADESolver.BindCrossover( &crossover_de );

	// execute
	g_JADESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_JADESolver.Evolve( &Eval );
	g_JADESolver.TakeSnapshot( g_SnapShot );
	g_JADESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island JADE solver	========//
	tcout << "//####################### Multi-Island JADE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_JADESolver );

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


	//====== test SHADE solver	=======//
	tcout << "//####################### SHADE #########################//" << tendl;

	// 経過世代数が少ない(500世代etc...)時点では、Binomialが高い適応値を出す
	// 経過世代数が多くなると(1000世代以降?)、Exponentialの方が高い適応値で安定収束する
	
	// set de parameters
	g_SHADESolver.SetPopulationSize( 100 );
	g_SHADESolver.SetNumGenerations( 1500 );
	//g_SHADESolver.SetDifferentialWeight( 0.5f );// 0.05f );// [0, 2]
	//g_SHADESolver.SetCrossoverProbability( 0.5f );// 0.25f );// [0, 1]
	//g_SHADESolver.SetBestPercentage( 0.5f );
	g_SHADESolver.BindCrossover( &crossover_de );

	// execute
	g_SHADESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_SHADESolver.Evolve( &Eval );
	g_SHADESolver.TakeSnapshot( g_SnapShot );
	g_SHADESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;


	//========= test Multi-Island SHADE solver	========//
	tcout << "//####################### Multi-Island SHADE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_SHADESolver );

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


	}

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
