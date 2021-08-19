#include	"CoDE.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>


//#include	"DE_Best_1.h"
//#include	"DE_Rand_1.h"
//#include	"DE_Rand_2.h"
//#include	"DE_Current_to_Rand_1.h"



namespace ealib
{

	//static DE_Best_1			s_DE_Best_1;
	//static DE_Rand_1			s_DE_Rand_1;
	//static DE_Rand_2			s_DE_Rand_2;
	//static DE_Current_to_Rand_1	s_DE_Current_to_Rand_1;


	//##########################################################################################//
	//										CoDE												//
	//##########################################################################################//

	CoDE::CoDE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, trialGen( 2 )
	{
		ClearAttribute();
	}



	CoDE::CoDE( const CoDE& obj )
		: IEvolutionaryAlgorithm( obj )
		, parentGen( 0 )
		, childGen( 1 )
		, trialGen( 2 )
	{
		std::copy( obj.m_MutationAttrib, obj.m_MutationAttrib + 3, m_MutationAttrib );
		//memcpy( m_MutationAttrib, obj.m_MutationAttrib, sizeof( DEAttribute ) * 3 );
	}



	CoDE::~CoDE()
	{
		ReleasePopulation();
	}



	void CoDE::SetAttribute( const DEAttribute& attrib, int i )
	{
		if( i<ArraySize( m_MutationAttrib ) )
			m_MutationAttrib[i] = attrib;
	}



	void CoDE::SetDifferentialWeight( float f, int i )
	{
		if( i<ArraySize( m_MutationAttrib ) )	m_MutationAttrib[i].F = f;
	}



	void CoDE::SetCrossoverProbability( float cr, int i )
	{
		if( i<ArraySize( m_MutationAttrib ) )	m_MutationAttrib[i].CR = cr;
	}



	// 初期集団を生成する
	void CoDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			// 親世代/子世代それぞれの個体情報
			m_Population[parentGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
			m_Population[childGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

			// トライアルベクターも初期化する
			m_Population[trialGen].Init( pChromosome, 3, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}
	}



	void CoDE::ReleasePopulation()
	{
		for( int i=0; i<3; ++i )
			m_Population[i].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* CoDE::Clone() const
	{
		return new CoDE( *this );
	}



	void CoDE::Step( Evaluator* pEval )
	{
		m_DE_Best_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_2.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Current_to_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome* x_i = m_Population[parentGen].GetIndividual( i );
			IChromosome* m_refCandidates[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

			//====================== current-to-rand/1/bin =========================//
			// 中間個体を生成する
			m_DE_Current_to_Rand_1.Execute( 5, m_refCandidates, i );
			IChromosome* t_i_best1bin	= m_Population[trialGen].GetIndividual( 0 );// Trial vector1
			t_i_best1bin->CopyGeneFrom( x_i );
			{
				IChromosome *randoms[] =
				{
					t_i_best1bin,
					m_refCandidates[0],// current
					m_refCandidates[1],// rand2
					m_refCandidates[2],// rand3
					m_refCandidates[3],// rand4
					m_refCandidates[4],// rand5
				};

				m_refCrossover->Execute( 6, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_best1bin );


			//========================== rand/1/bin ====================//
			// 中間個体を生成する
			m_DE_Rand_1.Execute( 3, m_refCandidates, i );
			IChromosome* t_i_rand1bin	= m_Population[trialGen].GetIndividual( 1 );// Trial vector2
			t_i_rand1bin->CopyGeneFrom( x_i );
			{
				IChromosome *randoms[] =
				{
					t_i_rand1bin,
					m_refCandidates[0],// rand1
					m_refCandidates[1],// rand2
					m_refCandidates[2],// rand3
				};

				m_refCrossover->Execute( 4, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_rand1bin );


			//====================== rand/2/bin =========================//
			// 中間個体を生成する
			m_DE_Rand_2.Execute( 5, m_refCandidates, i );
			IChromosome* t_i_rand2bin	= m_Population[trialGen].GetIndividual( 2 );// Trial vector3
			t_i_rand2bin->CopyGeneFrom( x_i );
			{
				IChromosome *randoms[] =
				{
					t_i_rand2bin,
					m_refCandidates[0],// rand1
					m_refCandidates[1],// rand2
					m_refCandidates[2],// rand3
					m_refCandidates[3],// rand4
					m_refCandidates[4],// rand5
				};

				m_refCrossover->Execute( 6, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_rand2bin );
			
			m_Population[trialGen].Sort( Population::SORT_FITNESS_DESCEND );
			IChromosome *pbest = m_Population[trialGen].GetIndividual( 0 );


			//======================	Selection	=======================//
			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( pbest->GetFitness() > x_i->GetFitness() )
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( pbest );
			else
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( x_i );

		}// end of i loop

		 // 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;
		
		m_Population[parentGen].Sort( Population::SORT_FITNESS_DESCEND );// 評価関数は計算済み.ソートだけすればよい

		// m_Statusを更新する
		m_Stats.Update( m_Population[parentGen] );
	}



	void CoDE::Evolve( Evaluator* pEval, unsigned int seed )
	{
		if( !m_bReady )
			return;

		OreOreLib::init_genrand( seed );

		
		m_Population[ parentGen ].Initialize( m_refInitializer, pEval );// Random initialization
		m_Stats.Reset( m_Population[ parentGen ] );

		while( !m_refCriteria->CheckTermination( &m_Stats ) )
		{
			Step( pEval );
		}

		//for( int gen=0; gen<m_Attrib.NumGenerations; ++gen )
		//{
		//	Step( pEval);
		//}
		
		m_DE_Best_1.UnbindPopulationData();
		m_DE_Rand_1.UnbindPopulationData();
		m_DE_Rand_2.UnbindPopulationData();
		m_DE_Current_to_Rand_1.UnbindPopulationData();

	}



	void CoDE::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}



	void CoDE::ClearAttribute()
	{
		m_Attrib.Clear();

		//F = [0.1, CR = 0.9], [F = 0.2, CR = 0.8], and[F = 0.3, CR = 0.7]

		//F = [1.0, CR = 0.1], [F = 1.0, CR = 0.9], and[F = 0.8, CR = 0.2]
		m_MutationAttrib[0].F	= 1.0f;
		m_MutationAttrib[0].CR	= 0.1f;
		
		m_MutationAttrib[1].F	= 1.0f;
		m_MutationAttrib[1].CR	= 0.9f;
		
		m_MutationAttrib[2].F	= 0.8f;
		m_MutationAttrib[2].CR	= 0.2f;
	}







	//##########################################################################################//
	//										MixedCoDE											//
	//##########################################################################################//

	MixedCoDE::MixedCoDE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, trialGen( 2 )
	{
		ClearAttribute();
	}



	MixedCoDE::MixedCoDE( const MixedCoDE& obj )
		: IEvolutionaryAlgorithm(obj)
		, parentGen( 0 )
		, childGen( 1 )
		, trialGen( 2 )
	{
		//std::copy( obj.m_MutationAttrib, obj.m_MutationAttrib + 3, m_MutationAttrib );
		memcpy( m_MutationAttrib, obj.m_MutationAttrib, sizeof(DEAttribute) * 3 );
	}



	MixedCoDE::~MixedCoDE()
	{
		ReleasePopulation();
	}

	

	void MixedCoDE::SetAttribute( const DEAttribute& attrib, int i )
	{
		if( i<ArraySize(m_MutationAttrib) )
			m_MutationAttrib[i] = attrib;
	}



	void MixedCoDE::SetDifferentialWeight( float f, int i )
	{
		if( i<ArraySize( m_MutationAttrib ) )	m_MutationAttrib[i].F = f;
	}



	void MixedCoDE::SetCrossoverProbability( float cr, int i )
	{
		if( i<ArraySize( m_MutationAttrib ) )	m_MutationAttrib[i].CR = cr;
	}



	// 初期集団を生成する
	void MixedCoDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			// 親世代/子世代それぞれの個体情報
			m_Population[parentGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
			m_Population[childGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

			// トライアルベクターも初期化する
			m_Population[trialGen].Init( pChromosome, 3, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}
	}



	void MixedCoDE::ReleasePopulation()
	{
		for( int i=0; i<3; ++i )
			m_Population[i].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* MixedCoDE::Clone() const
	{
		return new MixedCoDE( *this );
	}



	void MixedCoDE::Step( Evaluator* pEval )
	{
		int numChromTypes = m_Population[parentGen].GetIndividual(0)->NumChromTypes();


		m_DE_Best_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_2.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Current_to_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );


		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			Chromosome2D* x_i	= (Chromosome2D *)m_Population[parentGen].GetIndividual( i );
			Chromosome2D* m_refCandidates[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

			//====================== best/1/bin =========================//
			// 中間個体を生成する
			m_DE_Best_1.Execute( 3, (IChromosome **)m_refCandidates, i );
			Chromosome2D *t_i_best1bin	= (Chromosome2D *)m_Population[trialGen].GetIndividual( 0 );// Trial vector1
			t_i_best1bin->CopyGeneFrom( x_i );
			for( int j=0; j<numChromTypes; ++j )
			{
				IChromosome *randoms[] =
				{
					t_i_best1bin->GetChromosome( j ),
					m_refCandidates[0]->GetChromosome( j ),// best
					m_refCandidates[1]->GetChromosome( j ),// rand1
					m_refCandidates[2]->GetChromosome( j ),// rand2
				};

				m_refCrossover->Execute( 4, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_best1bin );

			//========================== rand/1/bin ====================//
			// 中間個体を生成する
			m_DE_Rand_1.Execute( 3, (IChromosome **)m_refCandidates, i );
			Chromosome2D *t_i_rand1bin	= (Chromosome2D *)m_Population[trialGen].GetIndividual( 1 );// Trial vector2
			t_i_rand1bin->CopyGeneFrom( x_i );
			for( int j=0; j<numChromTypes; ++j )
			{
				IChromosome *randoms[] =
				{
					t_i_rand1bin->GetChromosome( j ),
					m_refCandidates[0]->GetChromosome( j ),// rand1
					m_refCandidates[1]->GetChromosome( j ),// rand2
					m_refCandidates[2]->GetChromosome( j ),// rand3
				};

				m_refCrossover->Execute( 4, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_rand1bin );

			//====================== rand/2/bin =========================//
			// 中間個体を生成する
			m_DE_Rand_2.Execute( 5, (IChromosome **)m_refCandidates, i );
			Chromosome2D *t_i_rand2bin	= (Chromosome2D *)m_Population[trialGen].GetIndividual( 2 );// Trial vector3
			t_i_rand2bin->CopyGeneFrom( x_i );
			for( int j=0; j<numChromTypes; ++j )
			{
				IChromosome *randoms[] =
				{
					t_i_rand2bin->GetChromosome( j ),
					m_refCandidates[0]->GetChromosome( j ),// rand1
					m_refCandidates[1]->GetChromosome( j ),// rand2
					m_refCandidates[2]->GetChromosome( j ),// rand3
					m_refCandidates[3]->GetChromosome( j ),// rand4
					m_refCandidates[4]->GetChromosome( j ),// rand5
				};

				m_refCrossover->Execute( 4, randoms, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			}
			pEval->Evaluate( t_i_rand2bin );
			
			m_Population[trialGen].Sort( Population::SORT_FITNESS_DESCEND );
			IChromosome *pbest = m_Population[trialGen].GetIndividual( 0 );


			//======================	Selection	=======================//
			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( pbest->GetFitness() > x_i->GetFitness() )
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( pbest );
			else
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( x_i );

		}// end of i loop
		
		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[ parentGen ].Sort( Population::SORT_FITNESS_DESCEND );// 評価関数は計算済み.ソートだけすればよい

		// m_Statusを更新する
		m_Stats.Update( m_Population[ parentGen ] );
	}



	void MixedCoDE::Evolve( Evaluator* pEval, unsigned int seed )
	{	
		if( !m_bReady )
			return;
		
		OreOreLib::init_genrand( seed );

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

		m_DE_Best_1.UnbindPopulationData();
		m_DE_Rand_1.UnbindPopulationData();
		m_DE_Rand_2.UnbindPopulationData();
		m_DE_Current_to_Rand_1.UnbindPopulationData();
	}



	void MixedCoDE::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}

	

	void MixedCoDE::ClearAttribute()
	{
		m_Attrib.Clear();
		
		//F = 0.1, CR = 0.9], [F = 0.2, CR = 0.8], and[F = 0.3, CR = 0.7]
		m_MutationAttrib[0].F	= 0.1f;
		m_MutationAttrib[0].CR	= 0.9f;

		m_MutationAttrib[1].F	= 0.2f;
		m_MutationAttrib[1].CR	= 0.8f;

		m_MutationAttrib[2].F	= 0.3f;
		m_MutationAttrib[2].CR	= 0.7f;
	}



}// end of namespace