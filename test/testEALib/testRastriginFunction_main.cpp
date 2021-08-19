#include	<chrono>
#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<labworks/EA/EALib.h>

using namespace ealib;


Chromosome1D<float>	g_Chromosome;
SimpleGA		g_SGASolver;
MultiIslandEA	g_MIEASolver;
Population		g_SnapShot;




class Rastrigin1D: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		float value		= *chromosome->GeneAs<float>();
		float result	= 10.0f + ( value * value -10.0f*cos( 2.0f*M_PI*value ) );

		//tcout << "value = " << value << ", sin(value) = " << result << tendl;

		return result;
	}
};


Rastrigin1D		g_Evaluator;



//// ガウス分布 + 親の情報も使って突然変異を計算する
//void Mutate_FineGaussian( const IChromosome* p1, const IChromosome* p2, IChromosome* chromosome, float mutateRate )
//{
//
//
//}
//


int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	std::chrono::system_clock::time_point  start, end; // 型は auto で可
	start = std::chrono::system_clock::now(); // 計測開始時間


	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MINIMIZE );


	//==========	Init Crossover	=========//
	SBX					crossover_SBX;
	BLXalpha			crossover_BLXalpha;

	Crossover crossover;
	crossover.BindOperator( &crossover_BLXalpha );//&crossover_SBX


	//===========	Init Mutator	========//
	GaussianMutation<float>	mutation_Gaussian;

	Mutator mutator;
	mutator.BindOperator( &mutation_Gaussian );


	//============	Init DesignParameter(s)	================//
	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "attrib2" ), 0, 0, 0, 0, BoundaryType::Inclusive, BoundaryType::Exclusive, SamplingType::Enumerated },
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


	end = std::chrono::system_clock::now();  // 計測終了時間
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end-start ).count(); //処理に要した時間をミリ秒に変換

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;

	tcout << "time elapsed: " << elapsed << "[ms].\n";

	return 0;
}