#include	"DifferentialEvolution.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>



namespace ealib
{




	DifferentialEvolution::DifferentialEvolution()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
	{
		ClearAttribute();
	}



	DifferentialEvolution::DifferentialEvolution( const DifferentialEvolution& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_MutateAttrib( obj.m_MutateAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
	{		

	}



	DifferentialEvolution::~DifferentialEvolution()
	{
		ReleasePopulation();
	}

	

	void DifferentialEvolution::SetAttribute( const DEAttribute& attrib )
	{
		m_MutateAttrib	= attrib;
	}



	void DifferentialEvolution::SetDifferentialWeight( float f )
	{
		m_MutateAttrib.F = f;// m_DEAttrib.F = f;
	}



	void DifferentialEvolution::SetCrossoverProbability( float cr )
	{
		m_MutateAttrib.CR = cr;// m_DEAttrib.CR = cr;
	}



	bool DifferentialEvolution::InitPopulation( const DesignParamArray& designParams, int numObjectives )
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
	//void DifferentialEvolution::InitPopulation( const IChromosome* pChromosome, int numObjectives )
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



	void DifferentialEvolution::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ dummy ].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* DifferentialEvolution::Clone() const
	{
		return new DifferentialEvolution( *this );
	}



	void DifferentialEvolution::Step( Evaluator* pEval )
	{
		//IChromosome* refCandidates[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<const IChromosome*, 3> X = { nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T = { nullptr };

		m_DE_Rand_1_Mutator.BindPopulationData( m_Population[parentGen].Indivuduals() );

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome *x_i	= m_Population[parentGen].Individual( i );
			IChromosome *t_i	= m_Population[dummy].Individual( 0 );// 中間個体


			//=================	Mutation and Crossover	===================//
			// Select parents X{ rand1, rand2, rand3 }
			m_DE_Rand_1_Mutator.Execute( X, i );
			
			// Generate offspring t_i
			t_i->CopyGeneFrom( x_i );
			T[0] = t_i;
			m_refCrossover->Execute( X, T, &m_MutateAttrib );//m_refCrossover->Execute( 3, &X[0], 1, &T[0], &m_MutateAttrib );
			pEval->Evaluate( t_i );


			//======================	Selection	=======================//

			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->Fitness() > x_i->Fitness() )
				m_Population[childGen].Individual( i )->CopyGeneFrom( t_i );
			else
				m_Population[childGen].Individual( i )->CopyGeneFrom( x_i );

		}// end of i loop

		//m_Population[childGen].Sort( Population::SORT_FITNESS_DESCEND );
		
		// エリート保存. 効果なし
		//CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Sort( Population::SORT_FITNESS_DESCEND );

		m_Stats.Update( m_Population[parentGen] );
	}



	void DifferentialEvolution::Evolve( Evaluator* pEval, unsigned int seed )
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

		m_DE_Rand_1_Mutator.UnbindPopulationData();
	}



	void DifferentialEvolution::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[ parentGen ] );
	}



	void DifferentialEvolution::ClearAttribute()
	{
		m_Attrib.Clear();
		m_MutateAttrib.Clear();
	}



}// end of namespace