#include	<crtdbg.h>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>
#include	<oreore/MathLib.h>


#include	"labworks/EA/NSGA2.h"
#include	"labworks/EA/MultiIslandEA.h"
#include	"labworks/EA/Chromosome1D.h"


// Initializers
#include	"labworks/EA/Initializer.h"


// Selectors
#include	"labworks/EA/TournamentSelector.h"
#include	"labworks/EA/RouletteWheelSelector.h"

// Crossovers
#include	"labworks/EA/SBX.h"
#include	"labworks/EA/BLXalpha.h"

// Mutators
#include	"labworks/EA/GaussianMutation.h"
//#include	"labworks/EA/FlipBitMutation.h"


#include	"labworks/EA/PopulationResult.h"


using namespace ealib;


Chromosome1D<float>	g_Chromosome;
NSGA2			g_NSGA2Solver;
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


SinCurveEvaluator		g_SineCurve;
LogisticFuncEvaluator	g_LogisticFunc;





// ガウス分布 + 親の情報も使って突然変異を計算する
void Mutate_FineGaussian( const IChromosome* p1, const IChromosome* p2, IChromosome* chromosome, float mutateRate )
{


}







static void crowding_distance_vec2f_test( std::vector<Vec2f>& F )
{
	const int N = static_cast<int>(F.size());
	const int M = 2;
	std::vector<float>	Fdist( N );// = numpy.zeros( N, dtype='float32' )# TODO: Must be reordered along with F.Should be attached to F. 2018.08.31

	// Initialize distance with zero
	std::fill( Fdist.begin(), Fdist.end(), 0.0f );

	// Accumulate crowding distance.
	std::vector<int> indices_sorted( N );// TODO: Preallocate as NSGA2 member variable. 2018.09.21
	std::iota( indices_sorted.begin(), indices_sorted.end(), 0 );

	for( int m=0; m<M; ++m )
	{
		// Sort F using objective m
		std::sort( indices_sorted.begin(), indices_sorted.end(), [&]( int x, int y ) { return F[x].xy[m] < F[y].xy[m]; } );// return CompareFitnessAscend( F[x], F[y] );

		// Get fitness range of m's objective.
		auto fmax_m = F[indices_sorted[N-1]].xy[m];
		auto fmin_m = F[indices_sorted[0]].xy[m];
		auto f_range_m = Max( fmax_m - fmin_m, 1.0e-5f );

		// Set extreme solution's distance to infinity.(to guarantee that they will be selected in the next generation)
		Fdist[indices_sorted[0]] = Fdist[indices_sorted[N-1]] = std::numeric_limits<float>::infinity();

		// Calculate other solutions' distance
		for( int i=1; i<N-1; ++i )
		{
			auto il = indices_sorted[i-1];
			auto ir = indices_sorted[i+1];
			auto ic = indices_sorted[i];
			Fdist[ic] += ( F[ir].xy[m] - F[il].xy[m] ) / f_range_m;
		}
	}

	std::vector<int> sorted_idx( N );// TODO: Preallocate as NSGA2 member variable. 2018.09.21
	std::iota( sorted_idx.begin(), sorted_idx.end(), 0 );
	std::sort( sorted_idx.begin(), sorted_idx.end(), [&]( int x, int y ) { return Fdist[x] > Fdist[y]; } );

	reorder< std::vector<int>, std::vector<float>& >( sorted_idx, sorted_idx.size(), Fdist );

	// Reorder F's individuals using to indices_sorted 
	reorder< std::vector<int>, std::vector<Vec2f>& >( sorted_idx, sorted_idx.size(), F );

#ifdef _DEBUG
	tcout << "crowding_distance_vec2f_test...result" << tendl;
	for( int i=0; i<N; ++i )
	{
		tcout << "( ";
		for( int j=0; j<M; ++j )
			tcout << F[i].xy[j] << ",";

		tcout << ")\n";
	}
#endif
}







int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, -2.0f * M_PI, 2.0f * M_PI, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib2" ), 0.0f, -2.0f * M_PI, 2.0f * M_PI, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};


	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_SineCurve, SOLVE_MODE::MAXIMIZE );
	Eval.BindObjectiveFunction( &g_SineCurve, SOLVE_MODE::MAXIMIZE );


	Chromosome1D<float>	chromosome1;

	chromosome1.Init( designParams );

	Population	pop;

	pop.Init( &chromosome1, 7, Eval.NumObjectives() );

	const Vec2f result[] ={
		Vec2f( 0.0, 0.0 ),
		Vec2f( 7.0,7.0 ),
		Vec2f( 3.0,5.0 ),
		Vec2f( 3.0,3.0 ),
		Vec2f( 5.0,3.0 ),
		Vec2f( 1.0, 7.0 ),
		Vec2f( 7.0, 1.0 ),
	};

	for( int i=0; i<pop.PopulationSize(); ++i )
	{
		auto pop_ = pop.GetIndividual( i );

		for( int j=0; j<Eval.NumObjectives(); ++j )
		{
			pop_->SetScore( result[i].xy[j], j );
			pop_->SetFitness( result[i].xy[j], j );
		}
	}

	auto fronts = fast_nondominated_sort_index_ver( &pop );


	//================== reorder functionality test =======================//
	//std::vector<int> order ={ 0,5,3,4,1,2,6 };

	//std::vector<Vec2f> value ={
	//	Vec2f( 0.0, 0.0 ),
	//	Vec2f( 7.0,7.0 ),
	//	Vec2f( 3.0,5.0 ),
	//	Vec2f( 3.0,3.0 ),
	//	Vec2f( 5.0,3.0 ),
	//	Vec2f( 1.0, 7.0 ),
	//	Vec2f( 7.0, 1.0 )
	//};

	//reorder<std::vector<int>, std::vector<Vec2f>&>( order, order.size(), value );

	//// WARNING: order_ indices will be broken after reorder_ operation.
	//int order_[] ={ 0,5,3,4,1,2,6 };
	//Vec2f value_[] ={
	//	Vec2f( 0.0, 0.0 ),
	//	Vec2f( 7.0,7.0 ),
	//	Vec2f( 3.0,5.0 ),
	//	Vec2f( 3.0,3.0 ),
	//	Vec2f( 5.0,3.0 ),
	//	Vec2f( 1.0, 7.0 ),
	//	Vec2f( 7.0, 1.0 )
	//};

	//reorder<int[], Vec2f[]>( order_, ArraySize(order_), value_ );


	//================== crowding distance functionality test =================//

	
	std::vector<Vec2f> front_values =
	{
		Vec2f( 0.0, 0.0),
		Vec2f( 7.0, 4.0),
		Vec2f( 3.0, 5.0),
		Vec2f( 3.0, 3.0),
		Vec2f( 5.0, 3.0),
		Vec2f( 1.0, 1.0),
		Vec2f( 4.0, 7.0),
	};

	// Vec2f version.
	//crowding_distance_vec2f_test( front_values );


	// Population version.
	for( int i=0; i<pop.PopulationSize(); ++i )
	{
		auto individual = pop.GetIndividual( i );

		for( int j=0; j<Eval.NumObjectives(); ++j )
		{
			individual->SetScore( front_values[i].xy[j], j );
			individual->SetFitness( front_values[i].xy[j], j );
		}
	}

	std::vector<int> front ={ 0, 1, 2, 3, 4, 5, 6 };


	crowding_distance_population( &pop, &Eval, front );



	return 0;


	/*
	//=========	Init MutationOperatos	============//
	GaussianMutator	mutation_Gaussian;

	Mutators mutator;
	mutator.BindOperator( &mutation_Gaussian );


	//============	Init DesignParameters	================//
	DesignParamArray designParams =
	{
		{ _T( "attrib1" ), 0.0f, -2.0f * M_PI, 2.0f * M_PI, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T( "attrib2" ), 0.0f, -2.0f * M_PI, 2.0f * M_PI, 0.0f, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};


	SBX	crossover_SBX;
	BLXalpha crossover_BLXa;

	//============ Init Chromosome	================//
	g_Chromosome.Init( designParams );
	g_Chromosome.BindEvaluator( &g_LogisticFunc );//&g_SineCurve );//


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );

	RouletteWheelSelector rouletteSelector;


	//============ test NSGA2 solver	===========//
	tcout << "//================	NSGA2 solver	===================//" << tendl;

	//g_NSGA2Solver.BindCrossover( &crossover_BLXa );

	//g_NSGA2Solver.SetPopulationSize( 50 );
	//g_NSGA2Solver.SetNumGenerations( 10 );
	//g_NSGA2Solver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	//g_NSGA2Solver.SetMutationRate( 0.0075f );// 0.5 - 1.0[%]
	//g_NSGA2Solver.BindChromosome( &g_Chromosome );
	//g_NSGA2Solver.BindSelector( &rouletteSelector );// &tournamentSelector );
	//g_NSGA2Solver.BindMutators( &mutator );
	//g_NSGA2Solver.BindCrossover( &crossover );

	g_NSGA2Solver.InitPopulation( Eval.NumObjectives() );
	g_NSGA2Solver.Evolve( SOLVE_MODE::MINIMIZE );
	//g_NSGA2Solver.TakeSnapshot( g_SnapShot );
	//g_NSGA2Solver.Release();

	//DisplayPopulation( &g_SnapShot, true );

	/*
	//========= test Multi-Island GA solver	========//
	tcout << "//================	Multi-Island GA solver	===================//" << tendl;

	g_MIEASolver.BindChromosome( &g_Chromosome );
	g_MIEASolver.BindSolver( &g_NSGA2Solver );

	g_MIEASolver.SetIslandSize( 50 );
	g_MIEASolver.SetMigrationInterval( 5 );
	g_MIEASolver.SetMigrationRate( 0.15f );

	g_MIEASolver.GetSolver()->SetPopulationSize( 50 );//g_MIEASolver.SetPopulationSize( 50 );
	g_MIEASolver.GetSolver()->SetNumGenerations( 10 );// g_MIEASolver.SetNumGenerations( 10 );
	g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	g_MIEASolver.InitPopulation( Eval.NumObjectives() );

	//for( int i=0; i<5; ++i )
	g_MIEASolver.Evolve( SOLVE_MODE::MINIMIZE );
	g_MIEASolver.TakeSnapshot( g_SnapShot );
	g_MIEASolver.Release();


	DisplayPopulation( &g_SnapShot, true );
	*/
	return 0;
}