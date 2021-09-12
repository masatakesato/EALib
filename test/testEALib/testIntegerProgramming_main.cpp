#include	<crtdbg.h>
#include	<iostream>
# include	<random>
using namespace std;


#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>


#include	<ealib/EALib.h>

using namespace ealib;



//Chromosome1D<int>		g_Chromosome;
SimpleGA				g_SGASolver;
DifferentialEvolution	g_DESolver;




class IntegerPrpblem : public IObjectiveFunction
{
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		float x = (float)( chromosome->GeneAs<int>( 0 ) );
		float y = (float)( chromosome->GeneAs<int>( 1 ) );

		bool const1 = x > 1.0f;
		bool const2 = y > ( 0.25f * x - 0.5f );
		bool const3 = y < 40.0f;

		if( const1 && const2 && const3 )
			return x + y;
		else
			return 0.0;
	}

};


IntegerPrpblem	g_Evaluator;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	//============== Init Evaluator =================//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MAXIMIZE );


	//=============	Init Crossover	================//
	// GA
	//SBX				crossover_SBX;
	OnePointCrossover<int>	crossover_OnePoint;
	TwoPointCrossover<int>	crossover_TwoPoint;
	NPointCrossover<int, 2>	crossover_NPoint;
	UniformCrossover<int>	crossover_Uniform;

	Crossover crossover_ga;
	crossover_ga.BindOperator( &crossover_OnePoint );//&crossover_Uniform );

	// DE
	//BinomialCrossover binomial_Op;
	BinomialCrossover<int>		crossover_Binomial;
	ExponentialCrossover<int>	crossover_Exponential;

	Crossover crossover_de;
	crossover_de.BindOperator( &crossover_Binomial );//&crossover_Exponential );//


	//=============	Init Mutator	================//
	GaussianMutation<int>		mutate_Gaussian;
	RandomResetMutation<int>	mutate_RandomReset;
	CreepMutation<int>			mutate_Creep;

	Mutator mutator;
	mutator.BindOperator( &mutate_Gaussian );


	//=================	Init DesignParameters ====================//

	//OreOreLib::Array<DesignParameter>
	DesignParamArray designParams =
	{
		{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },

		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
		//{ _T(""), 0, -50, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};


	
	//==============	Init Chromosome	===============//
	//g_Chromosome.Init( designParams );


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );


	//============ test SimpleGA solver	===========//
	tcout << "//####################### SimpleGA #########################//" << tendl;
	
	// set simple ga parameters
	g_SGASolver.SetPopulationSize(64);
	g_SGASolver.SetEliteSize( 1 );
	g_SGASolver.SetNumGenerations( 500 );
	g_SGASolver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	g_SGASolver.SetMutationRate( 0.0075f );// 0.5 - 1.0[%]
	g_SGASolver.BindSelector( &tournamentSelector );
	g_SGASolver.BindMutator( &mutator );
	g_SGASolver.BindCrossover( &crossover_ga );// 整数用のクロスオーバー必要？？？

	g_SGASolver.InitPopulation( designParams, Eval.NumObjectives() );//g_SGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_SGASolver.Evolve( &Eval );
	DisplayPopulation( g_SGASolver.GetPopulation(), true );
	g_SGASolver.ReleasePopulation();

	tcout << tendl;


	//====== test Differential Evolution solver	=======//
	tcout << "//####################### Differential Evolution #########################//" << tendl;

	// set de parameters
	g_DESolver.SetPopulationSize(64 );
	g_DESolver.SetNumGenerations( 500 );
	g_DESolver.SetDifferentialWeight( 0.1f );// [0, 2]
	g_DESolver.SetCrossoverProbability( 0.1f );// [0, 1]
	g_DESolver.BindCrossover( &crossover_de );

	// execute
	g_DESolver.InitPopulation( designParams, Eval.NumObjectives() );//g_DESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
	g_DESolver.Evolve( &Eval );
	DisplayPopulation( g_DESolver.GetPopulation(), true );
	g_DESolver.ReleasePopulation();


	return 0;
}
