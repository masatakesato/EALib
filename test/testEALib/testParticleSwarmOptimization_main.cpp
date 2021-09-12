#include	<chrono>
#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	<ealib/EALib.h>
using namespace ealib;



//Chromosome1D<float>		g_Chromosome;
ParticleSwarmOptimization	g_PSOSolver;
MultiIslandEA				g_MIEASolver;
Population					g_SnapShot;



class Rastrigin1D: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		float value		= chromosome->GeneAs<float>();//*pParam->GetGene();
		float result	= 10.0f + ( value * value -10.0f*cos( 2.0f*M_PI*value ) );

		//tcout << "value = " << value << ", sin(value) = " << result << tendl;

		return result;
	}
};


class SinCurveEvaluator: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float value		= chromosome->GeneAs<float>();
		float result	= sin( value );

		//tcout << "value = " << value << ", sin(value) = " << result << tendl;

		return result;
	}

};



class LogisticFuncEvaluator: public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=NULL )
	{
		float value		= chromosome->GeneAs<float>();
		float result	= 1.0f / (1.0f + exp( -value ));
		
		//tcout << "value = " << value << ", f(value) = " << result << tendl;
		
		return result;
	}

};


SinCurveEvaluator		g_SineCurve;
LogisticFuncEvaluator	g_LogisticFunc;
Rastrigin1D				g_Rastrigin1D;




// ガウス分布 + 親の情報も使って突然変異を計算する
void Mutate_FineGaussian( const IChromosome* p1, const IChromosome* p2, IChromosome* chromosome, float mutateRate )
{


}





int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	std::chrono::system_clock::time_point  start, end; // 型は auto で可
	start = std::chrono::system_clock::now(); // 計測開始時間


	//================ Init Evaluator ===============//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Rastrigin1D, SOLVE_MODE::MINIMIZE );// &g_Rastrigin1D	&g_SineCurve	&g_LogisticFunc


	//============	Init DesignParameters	================//
	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, -5.12f, 5.12f, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T( "attrib2" ), 0, 0, 0, 0, BoundaryType::Inclusive, BoundaryType::Exclusive, SamplingType::Enumerated },
	};
	

	//============ Init Chromosome	================//
	//g_Chromosome.Init( designParams );


	//============ test PSO solver	===========//
	tcout << "//================	Particle Swarm Optimization solver	===================//" << tendl;
	
	g_PSOSolver.SetPopulationSize( 50 );
	g_PSOSolver.SetNumGenerations( 500 );
	g_PSOSolver.SetInertiaWeight( 0.5f );
	g_PSOSolver.SetAccelCoeff1( 0.5f );
	g_PSOSolver.SetAccelCoeff2( 0.25f );

	g_PSOSolver.InitPopulation( designParams, Eval.NumObjectives() );//g_PSOSolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_PSOSolver.Evolve( &Eval );
	g_PSOSolver.TakeSnapshot( g_SnapShot );
	g_PSOSolver.ReleasePopulation();
	
	DisplayPopulation( g_SnapShot, true );


	//========= test Multi-Island PSO solver	========//
	tcout << "//================	Multi-Island PSO solver	===================//" << tendl;

	g_MIEASolver.BindSolver( &g_PSOSolver );
	g_MIEASolver.GetSolver()->SetPopulationSize( 50 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 10 );

	g_MIEASolver.SetIslandSize( 50 );
	g_MIEASolver.SetMigrationInterval( 2 );
	g_MIEASolver.SetMigrationRate( 0.25f );

	g_MIEASolver.InitPopulation( designParams, Eval.NumObjectives() );//g_MIEASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	for( int i=0; i<5; ++i )	g_MIEASolver.Evolve( &Eval );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.ReleasePopulation();

	DisplayPopulation( g_SnapShot, true );


	end = std::chrono::system_clock::now();  // 計測終了時間
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( end-start ).count(); //処理に要した時間をミリ秒に変換

	tcout << "time elapsed: " << elapsed << "[ms].\n";

	return 0;
}