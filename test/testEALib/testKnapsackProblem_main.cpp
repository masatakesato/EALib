#include	<crtdbg.h>
#include	<algorithm>
#include	<iostream>
using namespace std;

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>


#include	<ealib/EALib.h>

using namespace ealib;


Chromosome1D<BitArray>	g_Chromosome;
SimpleGA				g_SGASolver;
MultiIslandEA			g_MIEASolver;
DifferentialEvolution	g_DESolver;
Population				g_SnapShot;




// ナップサック問題を解いてみる http://ipr20.cs.ehime-u.ac.jp/column/ga/chapter4.html

class Knapsack: public IObjectiveFunction
{

	// ナップサック問題の目的関数
	virtual float Execute( IChromosome* chromosome, const void* attribs=0 )
	{
		const int MaxWeight = 40;// ナップサックに入る荷物の最大重量

		BitArray *pBitString	= chromosome->GeneAs<BitArray>();

		int totalWeight		= 0;
		float totalPrice	= 0.0f;

		for( int i=0; i<pBitString->BitLength(); ++i )
		{
			int bitValue = pBitString->GetBit( i );
			totalWeight += g_Weight[i] * bitValue;
			totalPrice	+= g_Price[i] * float( bitValue );
		}

		if( totalWeight > MaxWeight )// 最大重量より多く詰め込んだ場合はペナルティとして評価値を低くする
			totalPrice = 1.0f;

		return totalPrice;
	}

private:

	// 品物の重量
	const int g_Weight[50] =
	{
		2,10,7,2,4,9,10,7,8,5,
		3,10,9,8,8,5,7,3,9,7,
		2,10,7,9,7,2,10,4,9,10,
		4,7,8,5,2,3,10,9,7,8,
		8,5,7,5,7,3,9,7,7,9,
	};

	// 品物の値段
	float g_Price[50] =
	{
		21,22,28,21,12,24,15,2,25,28,
		4,22,36,2,7,40,14,40,33,21,
		28,22,14,36,28,21,18,12,24,15,
		21,2,25,28,28,4,22,36,31,2,
		7,40,14,4,28,40,33,35,21,20,
	};

};


Knapsack	g_Evaluator;




int main( int argc, char **argv )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );


	//============== Init Initializer ===============//
	RandomInitializer	initRandom;


	//========= Init Evaluator ===========//
	Evaluator Eval;
	Eval.BindObjectiveFunction( &g_Evaluator, SOLVE_MODE::MAXIMIZE );

	

	//========= Init Crossover	==========//
	// GA
	OnePointCrossover<BitArray>		crossover_OnePoint;
	TwoPointCrossover<BitArray>		crossover_TwoPoint;
	NPointCrossover<BitArray, 2>	crossover_NPoint;
	UniformCrossover<BitArray>		crossover_Uniform;
	EvenOddCrossover				crossover_EvenOdd;

	Crossover crossover_ga;
	crossover_ga.BindOperator( &crossover_Uniform );//&crossover_OnePoint );//&crossover_TwoPoint );&crossover_NPoint );//&crossover_EvenOdd );//

	// DE
	BDECrossover				crossover_BDE;

	ExponentialCrossover<BitArray> crossover_BinaryExponential;//BinaryExponentialCrossover	crossover_BinaryExponential;//
	BinaryBinomialCrossover		crossover_BinaryBinomial;// BinomialCrossover<BitArray>		crossover_BinaryBinomial;//

	HammingDECrossover			crossover_hummingDE;

	Crossover crossover_de;
	crossover_de.BindOperator( &crossover_BinaryBinomial );//&crossover_BDE ); //&crossover_BinaryExponential );//&crossover_hummingDE ); //


	//=============	Init Mutator	==========//
	FlipBitMutation	mutation_FlipBit;

	Mutator mutator;
	mutator.BindOperator( &mutation_FlipBit );





	//============ Init Chromosome	================//
	DesignParamArray designParams =
	{
		{ TYPE_ID<BitArray>, _T(""), 0, 0, 50, 0, BoundaryType::Inclusive, BoundaryType::Inclusive, SamplingType::Enumerated },
	};

	g_Chromosome.Init( designParams );


	//=============	Init Selector	===============//
	TournamentSelector tournamentSelector;
	tournamentSelector.SetTournamentSize( 2 );


	//============ test SimpleGA solver	===========//
	tcout << "//####################### SimpleGA #########################//" << tendl;

	g_Chromosome.Initialize( &initRandom );

	g_SGASolver.SetPopulationSize( 32 );
	g_SGASolver.SetNumGenerations( 500 );
	g_SGASolver.SetEliteSize( 2 );
	g_SGASolver.SetCrossoverRate( 0.95f );// 80 - 95[%]
	g_SGASolver.SetMutationRate( 0.01f );// 0.5 - 1.0[%]
	g_SGASolver.BindSelector( &tournamentSelector );
	g_SGASolver.BindMutator( &mutator );
	g_SGASolver.BindCrossover( &crossover_ga );

	//for( int i=0; i<15; ++i )
	{
		g_SGASolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
		g_SGASolver.Evolve( &Eval );
		g_SGASolver.TakeSnapshot( g_SnapShot );
		g_SGASolver.ReleasePopulation();
	}

	DisplayPopulation( &g_SnapShot, true );

	tcout << tendl;



	//========= test Multi-Island GA solver	========//
	tcout << "//####################### Multi-Island GA #########################//" << tendl;

	g_Chromosome.Initialize( &initRandom );

	g_MIEASolver.BindSolver( &g_SGASolver );
	//g_MIEASolver.GetSolver()->BindSelector( &tournamentSelector );

	// set ga parameters
	//g_MIEASolver.GetSolver()->SetPopulationSize( 32 );//g_MIEASolver.SetPopulationSize( 32 );
	//g_MIEASolver.GetSolver()->SetNumGenerations( 500 );// g_MIEASolver.SetNumGenerations( 500 );
	//g_MIEASolver.GetSolver()->SetEliteSize( 2 );// g_MIEASolver.SetEliteSize( 2 );
	//g_MIGASolver.SetCrossoverRate( 0.95 );// 80 - 95[%]
	//g_MIGASolver.SetMutationRate( 0.01 );// 0.5 - 1.0[%]

	// set miea parameters
	g_MIEASolver.SetIslandSize( 50 );
	g_MIEASolver.SetMigrationInterval( 2 );
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

	g_Chromosome.Initialize( &initRandom );

	// set de parameters
	g_DESolver.SetPopulationSize( 32 );
	//g_DESolver.SetEliteSize( 2 );
	g_DESolver.SetNumGenerations( 500 );
	g_DESolver.SetDifferentialWeight( 0.3f/*0.5*//*0.6*/ );// [0, 2]
	g_DESolver.SetCrossoverProbability( 0.2f/*0.1*//*0.3*/ );// [0, 1]
	g_DESolver.BindCrossover( &crossover_de );

	for( int i=0; i<5; ++i )
	{
		// execute
		g_DESolver.InitPopulation( &g_Chromosome, Eval.NumObjectives() );
		g_DESolver.Evolve( &Eval );
		g_DESolver.TakeSnapshot( g_SnapShot );
		g_DESolver.ReleasePopulation();

		DisplayPopulation( &g_SnapShot, true );
	}


	return 0;
}