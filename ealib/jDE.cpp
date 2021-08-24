#include	"jDE.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>



namespace ealib
{

	jDE::jDE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		//, m_Fs()
		//, m_CRs()
	{
		ClearAttribute();
	}



	jDE::jDE( const jDE& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_jDEAttrib( obj.m_jDEAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		//, m_Fs()
		//, m_CRs()
	{

	}



	jDE::~jDE()
	{
		ReleasePopulation();
	}

	

	void jDE::SetAttribute( const jDEAttribute& attrib )
	{
		m_jDEAttrib	= attrib;
	}



	void jDE::SetLowerDifferentialWeight( float fl )
	{
		m_jDEAttrib.Fl = fl;
	}



	void jDE::SetUpperDifferentialWeight( float fu )
	{
		m_jDEAttrib.Fu = fu;
	}



	void jDE::SetT1( float t )
	{
		m_jDEAttrib.T1	= t;
	}



	void jDE::SetT2( float t )
	{
		m_jDEAttrib.T2	= t;
	}



	void jDE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//

			// 親世代/子世代それぞれの個体情報
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// F/CR
			m_Fs.Init( m_Attrib.PopulationSize );
			m_CRs.Init( m_Attrib.PopulationSize );

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}
	}



	// Deprecated. 2021.08.23
	//void jDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//

	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// F/CR
	//		m_Fs.Init( m_Attrib.PopulationSize );
	//		m_CRs.Init( m_Attrib.PopulationSize );

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



	void jDE::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ dummy ].Release();

		m_Fs.Release();
		m_CRs.Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* jDE::Clone() const
	{
		return new jDE( *this );
	}



	void jDE::Step( Evaluator* pEval )
	{
		UpdateControlParams();

		m_DE_Rand_1_Mutator.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome *x_i	= m_Population[ parentGen ].GetIndividual( i );
			IChromosome *t_i	= m_Population[ dummy ].GetIndividual( 0 );// 中間個体
			IChromosome* refCandidates[4] = { nullptr, nullptr, nullptr, nullptr };

			//=================	Mutation and Crossover	===================//
			m_DE_Rand_1_Mutator.Execute( 3, &refCandidates[1], i );
			refCandidates[0] = t_i;
			//IChromosome *randoms[] =
			//{
			//	t_i,// t_i. trial vector
			//	refCandidates[0],
			//	refCandidates[1],
			//	refCandidates[2],
			//};
			
			// 中間個体を生成する
			t_i->CopyGeneFrom( x_i );
			int id = x_i->ID();// 個体IDを使って、x_iとm_pFs[i]/m_pCRs[i]を一義的に割り当てる→前世代からの値持ち越しがあるので関係ある
			DEAttribute attr = { m_Fs[id], m_CRs[id],  m_Fs[id] };
			m_refCrossover->Execute( 4, refCandidates/*randoms*/, &attr );

			pEval->Evaluate( t_i );


			//======================	Selection	=======================//

			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->GetFitness() > x_i->GetFitness() )
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( t_i );
			else
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( x_i );

		}// end of i loop
		
		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[ parentGen ].Sort( Population::SORT_FITNESS_DESCEND );// 各個体は評価値計算済みのため、ソートする

		m_Stats.Update( m_Population[ parentGen ] );
	}



	void jDE::Evolve( Evaluator* pEval, unsigned int seed )
	{	
		if( !m_bReady )
			return;

		OreOreLib::init_genrand( seed );
		
		// Initialize 1st population (randomize variables and evaluate)
		m_Population[ parentGen ].Initialize( m_refInitializer, pEval );// Random initialization
		m_Stats.Reset( m_Population[ parentGen ] );

		// Initialize F/CR
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			m_Fs[i] = m_jDEAttrib.Fl + (float)OreOreLib::genrand_real1() * m_jDEAttrib.Fu;
			m_CRs[i] = (float)OreOreLib::genrand_real1();
		}


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



	void jDE::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}



	void jDE::UpdateControlParams()
	{
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			if( (float)OreOreLib::genrand_real1() < m_jDEAttrib.T1 )
				m_Fs[i] = m_jDEAttrib.Fl + (float)OreOreLib::genrand_real1() * m_jDEAttrib.Fu;
			if( (float)OreOreLib::genrand_real1() < m_jDEAttrib.T2 )
				m_CRs[i] = (float)OreOreLib::genrand_real1();
		}// end of i loop
	}



	void jDE::ClearAttribute()
	{
		m_Attrib.Clear();
		
		// default values for adaptive DE
		m_jDEAttrib.Fl	= 0.1f;
		m_jDEAttrib.Fu	= 0.9f;
		m_jDEAttrib.T1	= 0.1f;
		m_jDEAttrib.T2	= 0.1f;
	}


}// end of namespace