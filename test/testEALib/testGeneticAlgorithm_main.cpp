#include	<crtdbg.h>
#include	<iostream>
#include	<chrono>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>
#include	<oreore/memory/SharedPtr.h>

#include	<ealib/EALib.h>
using namespace ealib;



Chromosome1D<float>	g_Chromosome;
SimpleGA		g_SGASolver;
MultiIslandEA	g_MIEASolver;
Population		g_SnapShot;



class SinCurveEvaluator: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float value		= *chromosome->GeneAs<float>();
		float result	= sin( value );

		//tcout << "value = " << value << ", sin(value) = " << result << tendl;

		return result;
	}

};



class LogisticFuncEvaluator: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float value		= *chromosome->GeneAs<float>();
		float result	= 1.0f / (1.0f + exp( -value ));
		
		//tcout << "value = " << value << ", f(value) = " << result << tendl;
		
		return result;
	}

};


//SinCurveEvaluator		g_SineCurve;
LogisticFuncEvaluator	g_LogisticFunc;





// ガウス分布 + 親の情報も使って突然変異を計算する
void Mutate_FineGaussian( const IChromosome* p1, const IChromosome* p2, IChromosome* chromosome, float mutateRate )
{


}





int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	std::chrono::system_clock::time_point  start, end; // 型は auto で可
	start = std::chrono::system_clock::now(); // 計測開始時間


	//============== Init Evaluator =================//
	OreOreLib::SharedPtr<LogisticFuncEvaluator> logifunc( new LogisticFuncEvaluator() );

	Evaluator Eval;
	Eval.BindObjectiveFunction( /*&g_LogisticFunc*/logifunc.Get(), SOLVE_MODE::MINIMIZE );// SinCurveEvaluator()


	//=============== Init Crossover ==============//
	SBX	crossover_SBX;
	BLXalpha crossover_BLXa;

	Crossover crossover_ga;
	crossover_ga.BindOperator( &crossover_BLXa  );//&crossover_SBX


	//================ Init Mutator ===============//
	GaussianMutation<float>	mutation_Gaussian;

	Mutator mutator;
	mutator.BindOperator( &mutation_Gaussian );


	//============	Init DesignParameters	================//
	DesignParamArray designParams =
	{
		{ _T(""), 0.0f, -M_PI * 2.0f, M_PI * 2.0f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};


	//============ Init Chromosome	================//
	g_Chromosome.Init( designParams );


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );

	RouletteWheelSelector rouletteSelector;


	//============ test SimpleGA solver	===========//
	tcout << "//================	SimpleGA solver	===================//" << tendl;

	g_SGASolver.SetPopulationSize( 50 );
	g_SGASolver.SetNumGenerations( 10 );
	g_SGASolver.SetCrossoverRate( 0.85f );// 80 - 95[%]
	g_SGASolver.SetMutationRate( 0.0095f );// 0.5 - 1.0[%]
	g_SGASolver.BindSelector( &rouletteSelector );// &tournamentSelector );
	g_SGASolver.BindMutator( &mutator );
	g_SGASolver.BindCrossover( &crossover_ga );

	g_SGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_SGASolver.Evolve( &Eval );
	g_SGASolver.TakeSnapshot( g_SnapShot );
	g_SGASolver.ReleasePopulation();
	
	DisplayPopulation( &g_SnapShot, true );


	tcout << tendl;

	//========= test Multi-Island GA solver	========//
	tcout << "//================	Multi-Island GA solver	===================//" << tendl;

	g_MIEASolver.BindSolver( &g_SGASolver );

	g_MIEASolver.SetIslandSize( 50 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	g_MIEASolver.GetSolver()->SetPopulationSize( 50 );//g_MIEASolver.SetPopulationSize( 50 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 10 );// g_MIEASolver.SetNumGenerations( 10 );
	g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );

	//for( int i=0; i<5; ++i )
	//{
	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( &g_SnapShot, true );
	//}

	tcout << tendl;


	end = std::chrono::system_clock::now();  // 計測終了時間
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end-start ).count(); //処理に要した時間をミリ秒に変換
	tcout << "time: " << elapsed << "[ms].\n";

	


	return 0;
}