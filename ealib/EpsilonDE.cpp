#include	"EpsilonDE.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>


#include	"DE_Rand_1.h"


// Differential Evolution + Epsilon constriant method. 
// Still in development( Just DE at current). 2021.08.24
// https://www.jstage.jst.go.jp/article/tjsai/24/1/24_1_34/_pdf



namespace ealib
{

	static DE_Rand_1	s_DE_Rand_1_Mutator;


	EpsilonDE::EpsilonDE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
	{
		ClearAttribute();
	}



	EpsilonDE::EpsilonDE( const EpsilonDE& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_MutateAttrib( obj.m_MutateAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
	{

	}



	EpsilonDE::~EpsilonDE()
	{
		ReleasePopulation();
	}

	

	void EpsilonDE::SetAttribute( const DEAttribute& attrib )
	{
		m_MutateAttrib	= attrib;
	}


	void EpsilonDE::SetDifferentialWeight( float f )
	{
		m_MutateAttrib.F = f;// m_DEAttrib.F = f;
	}



	void EpsilonDE::SetCrossoverProbability( float cr )
	{
		m_MutateAttrib.CR = cr;// m_DEAttrib.CR = cr;
	}



	bool EpsilonDE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//

			// 親世代/子世代それぞれの個体情報
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}

		return m_bReady;
	}



	// Deprecated. 2021.08.23
	//void EpsilonDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//

	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// ダミーデータも初期化する
	//		m_Population[ dummy ].Init( pChromosome, 1, numObjectives );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void EpsilonDE::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ dummy ].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* EpsilonDE::Clone() const
	{
		return new EpsilonDE( *this );
	}


	void EpsilonDE::Step( Evaluator* pEval )
	{
//IChromosome* refCandidates[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<const IChromosome*, 3> X = { nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T = { nullptr };

		s_DE_Rand_1_Mutator.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome *x_i	= m_Population[ parentGen ].GetIndividual( i );
			IChromosome *t_i	= m_Population[ dummy ].GetIndividual( 0 );// 中間個体

			//=================	Mutation and Crossover	===================//
			//s_DE_Rand_1_Mutator.Execute( 3, &refCandidates[1], i );//s_DE_Rand_1_Mutator.Execute( 3, refCandidates, i );
			s_DE_Rand_1_Mutator.Execute( 3, (IChromosome**)X.begin(), i );
			T[0] = t_i;//refCandidates[0] = t_i;
			//IChromosome *randoms[] =
			//{
			//	t_i,
			//	refCandidates[0],
			//	refCandidates[1],
			//	refCandidates[2],
			//};
			
			// 中間個体を生成する
			t_i->CopyGeneFrom( x_i );
//m_refCrossover->Execute( 4, refCandidates, &m_MutateAttrib );//m_refCrossover->Execute( 4, randoms, &m_MutateAttrib );
m_refCrossover->Execute2( X, T, &m_MutateAttrib );//m_refCrossover->Execute( 3, (const IChromosome**)&refCandidates[1], 1, &refCandidates[0], &m_MutateAttrib );

			pEval->Evaluate( t_i );


			//======================	Selection	=======================//

			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->GetFitness() > x_i->GetFitness() )
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( t_i );
			else
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( x_i );

		}// end of i loop

		//m_Population[childGen].Sort( Population::SORT_FITNESS_DESCEND );
		
		// エリート保存. 効果なし
		//CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[ parentGen ].Sort( Population::SORT_FITNESS_DESCEND );

		m_Stats.Update( m_Population[ parentGen ] );
	}



	void EpsilonDE::Evolve( Evaluator* pEval, unsigned int seed )
	{	
		if( !m_bReady )
			return;
		
		OreOreLib::init_genrand( seed );

		// Initialize 1st population (randomize variables and evaluate)
		m_Population[ parentGen ].Initialize( m_refInitializer, pEval );// Random initialization
		m_Stats.Reset( m_Population[ parentGen ] );

		while( !m_refCriteria->CheckTermination( &m_Stats ) )
		{
			Step( pEval );
		}

		//for( int gen=0; gen<m_Attrib.NumGenerations; ++gen )
		//{
		//	Step( pEval );
		//}

		s_DE_Rand_1_Mutator.UnbindPopulationData();
	}


	void EpsilonDE::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[parentGen] );
	}


	void EpsilonDE::ClearAttribute()
	{
		m_Attrib.Clear();
		m_MutateAttrib.Clear();
	}

}// end of namespace