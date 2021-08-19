#include	<chrono>
#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome1D<float>	g_Chromosome;
SimpleGA				g_SGASolver;
DifferentialEvolution	g_DESolver;
MultiIslandEA			g_MIEASolver;
Population				g_SnapShot;



class RastriginND: public IObjectiveFunction
{
public:

	RastriginND(){}
	~RastriginND(){}

	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float result = 10.0f * float( chromosome->Size() );

		for( int i=0; i<chromosome->Size(); ++i )
		{
			float xi = *chromosome->GeneAs<float>( i );
			result	+= ( xi * xi -10.0f * cos( 2.0f*M_PI*xi ) );
		}

		return result;
	}

};


RastriginND g_Evaluator;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	

	std::chrono::system_clock::time_point  start, end; // 型は auto で可
	start = std::chrono::system_clock::now(); // 計測開始時間


	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MINIMIZE );


	//============	Init Crossover	============//
	// GA
	SBX			sbx_Op;
	BLXalpha	blxa_op;

	Crossover crossover_ga;
	crossover_ga.BindOperator( &blxa_op );//&sbx_Op

	// DE
	BinomialCrossover<float>	crossover_Binomial;
	ExponentialCrossover<float>	crossover_Exponential;

	Crossover crossover_de;
	crossover_de.BindOperator( &crossover_Binomial );//&crossover_Exponential


	//=============	Init Mutator	============//
	GaussianMutation<float>	mutation_Gaussian;

	Mutator mutator;
	mutator.BindOperator( &mutation_Gaussian );


	//==============	Init Chromosome	================//
	DesignParamArray designParams =
	{
		{ _T( "attrib0" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib1" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib2" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib3" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib4" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib5" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib6" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib7" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};

	g_Chromosome.Init( designParams );


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );


	//=============	Init Solvers	===============//
	// SimpleGA
	g_SGASolver.SetPopulationSize( 32 );
	g_SGASolver.SetEliteSize( 1 );
	g_SGASolver.SetNumGenerations( 500 );
	g_SGASolver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	g_SGASolver.SetMutationRate( 0.0075f );// 0.5 - 1.0[%]
	g_SGASolver.BindSelector( &tournamentSelector );
	g_SGASolver.BindMutator( &mutator );
	g_SGASolver.BindCrossover( &crossover_ga );

	// DE
	g_DESolver.SetPopulationSize( 32 );
	g_DESolver.SetNumGenerations( 500 );
	g_DESolver.SetDifferentialWeight( 0.5f );// 0.1f );// [0, 2]
	g_DESolver.SetCrossoverProbability( 0.25f );// 0.5f );// [0, 1]
	g_DESolver.BindCrossover( &crossover_de );


	//============ test SimpleGA solver	===========//
	tcout << "//####################### SimpleGA #########################//" << tendl;
	
	//for( int i=0; i<1; ++i )
	//{
		// execute
		g_SGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
		g_SGASolver.Evolve( &Eval );
		g_SGASolver.TakeSnapshot( g_SnapShot );
		g_SGASolver.ReleasePopulation();
	
		DisplayPopulation( &g_SnapShot, true );
	//}

	tcout << tendl;

	
	//========= test Multi-Island EA solver	========//
	tcout << "//####################### Multi-Island GA #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_SGASolver );
	g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	g_MIEASolver.GetSolver()->SetPopulationSize( 32 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 500 );
	g_MIEASolver.GetSolver()->SetEliteSize( 2 );// g_MIEASolver.SetEliteSize( 2 );

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


	//====== test Differential Evolution solver	=======//
	tcout << "//####################### Differential Evolution #########################//" << tendl;

	// execute
	g_DESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_DESolver.Evolve( &Eval );
	g_DESolver.TakeSnapshot( g_SnapShot );
	g_DESolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );
	
	tcout << tendl;


	//========= test Multi-Island DE solver	========//
	tcout << "//####################### Multi-Island DE #########################//" << tendl;

	g_MIEASolver.BindSolver( &g_DESolver );

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


	end = std::chrono::system_clock::now();  // 計測終了時間
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end-start ).count(); //処理に要した時間をミリ秒に変換
	tcout << "time: " << elapsed << "[ms].\n";


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
