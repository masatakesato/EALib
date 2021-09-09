#include	"MultiIslandEA.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>



namespace ealib
{


	MultiIslandEA::MultiIslandEA()
		: IEvolutionaryAlgorithm()
		, m_refSolver( nullptr )
		//, m_pSolverArray()
		//, m_Destinations()
		//, m_Migrants()
	{
		ClearAttribute();
	}



	MultiIslandEA::MultiIslandEA( const MultiIslandEA& obj )
		: IEvolutionaryAlgorithm(obj)
		, m_MIGAAttrib( obj.m_MIGAAttrib )
		, m_refSolver( obj.m_refSolver )
		//, m_pSolverArray()
		//, m_Destinations()
		//, m_Migrants()
	{

	}



	MultiIslandEA::~MultiIslandEA()
	{
		ReleasePopulation();
	}



	void MultiIslandEA::BindSolver( IEvolutionaryAlgorithm* pSolver )
	{
		m_refSolver	= pSolver;
		m_Attrib	= pSolver->GetAttribute();
	}



	void MultiIslandEA::UnbindSolver()
	{
		m_refSolver	= nullptr;
	}



	IEvolutionaryAlgorithm* MultiIslandEA::GetSolver() const
	{
		return m_refSolver;
	}



	void MultiIslandEA::SetAttribute( const MIEAAttribute& attrib )
	{
		m_MIGAAttrib	= attrib;
	}



	void MultiIslandEA::SetIslandSize( int island_size )
	{
		m_MIGAAttrib.IslandSize	= island_size;
	}



	void MultiIslandEA::SetMigrationMode( MIGRATION_MODE mig_mode )
	{
		m_MIGAAttrib.MigrationMode	= mig_mode;
	}



	void MultiIslandEA::SetMigrationInterval( int interval )
	{
		m_MIGAAttrib.MigrationInterval	= interval;
	}



	void MultiIslandEA::SetMigrationRate( float mig_rate )
	{
		m_MIGAAttrib.MigrationRate	= mig_rate;
	}



	bool MultiIslandEA::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			if( !m_refSolver )
				return false;

			ReleasePopulation();

			//====== m_Attribをソルバー設定で上書き更新する =====//
			m_Attrib	= m_refSolver->GetAttribute();

			//============== 島毎のGAを初期化する ==============//
			m_pSolverArray.Init( m_MIGAAttrib.IslandSize );
			m_bReady = true;
			for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
			{
				m_pSolverArray[i] = m_refSolver->Clone();
				m_bReady &= m_pSolverArray[i]->InitPopulation( designParams, numObjectives );
			}// end of i loop


			//=========== 移民バッファを初期化する	===========//
			m_Destinations.Init( m_MIGAAttrib.IslandSize );
			m_Migrants.Init( m_MIGAAttrib.IslandSize );
			int numMigrants	= Clamp( (int)ceil( m_MIGAAttrib.MigrationRate * (float)m_Attrib.PopulationSize ), 0, m_Attrib.PopulationSize );

			for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
			{
				m_Destinations[i]	= -1;
				m_Migrants[i].Init( designParams, numMigrants, numObjectives );
			}// end of i loop

			//m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}

		return m_bReady;
	}



	// Deprecated. 2021.08.23
	//void MultiIslandEA::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		if( !m_refSolver )
	//			return;

	//		ReleasePopulation();

	//		//====== m_Attribをソルバー設定で上書き更新する =====//
	//		m_Attrib	= m_refSolver->GetAttribute();

	//		//============== 島毎のGAを初期化する ==============//
	//		m_pSolverArray.Init( m_MIGAAttrib.IslandSize );

	//		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
	//		{
	//			m_pSolverArray[i] = m_refSolver->Clone();
	//			m_pSolverArray[i]->InitPopulation( pChromosome, numObjectives );
	//		}// end of i loop


	//		//=========== 移民バッファを初期化する	===========//
	//		m_Destinations.Init( m_MIGAAttrib.IslandSize );
	//		m_Migrants.Init( m_MIGAAttrib.IslandSize );
	//		int numMigrants	= Clamp( (int)ceil( m_MIGAAttrib.MigrationRate * (float)m_Attrib.PopulationSize ), 0, m_Attrib.PopulationSize );

	//		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
	//		{
	//			m_Destinations[i]	= -1;
	//			m_Migrants[i].Init( pChromosome, numMigrants, numObjectives );
	//		}// end of i loop

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void MultiIslandEA::ReleasePopulation()
	{
		for( int i=0; i<m_pSolverArray.Length(); ++i )
			SafeDelete( m_pSolverArray[i] );
		m_pSolverArray.Release();
		
		m_Destinations.Release();
		m_Migrants.Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* MultiIslandEA::Clone() const
	{
		return new MultiIslandEA( *this );
	}



	void MultiIslandEA::Step( Evaluator* pEval )
	{

	}



	void MultiIslandEA::Evolve( Evaluator* pEval, unsigned int seed )
	{
		if( m_pSolverArray.Empty() || !m_bReady )
			return;

		OreOreLib::init_genrand( seed );


		// Initialize 1st Generation
		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
		{
			m_pSolverArray[i]->GetPopulation()->Initialize( m_refInitializer, pEval );// Random initialization
			m_pSolverArray[i]->Statistics()->Reset( *m_pSolverArray[i]->GetPopulation() );
		}


		for( int gen=0; gen <m_Attrib.NumGenerations; ++gen )
		{
			//tcout << "Gen: " << gen << tendl;

			// 島ごとに1世代分の処理を進める
			for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
			{
				m_pSolverArray[i]->Step( pEval );

			}// end of i loop

			// TODO: Add Termination Procedure.


			 // 移住処理を行う
			if( gen % m_MIGAAttrib.MigrationInterval==0 )
			{
				// 移住対象の個体を移民船に乗せる
				Emigrate( m_MIGAAttrib.MigrationMode );

				// 移民船の個体を目的地の島に降ろす
				Immigrate( m_MIGAAttrib.MigrationMode );

				for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
					m_pSolverArray[i]->GetPopulation()->Sort( Population::SORT_FITNESS_DESCEND );
			}

		}// end of gen loop

	}


	void MultiIslandEA::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
		{
			const int& popsize		= m_Attrib.PopulationSize;
			const int& islandsize	= m_MIGAAttrib.IslandSize;

			pOut.Init( *m_pSolverArray[0]->GetPopulation()->GetDesignParamArray(), popsize * islandsize, m_Migrants[0].NumObjectives() );

			for( int i=0; i<islandsize; ++i )
				pOut.CopyChromosomes( m_pSolverArray[i]->GetPopulation(), i*popsize );

			pOut.Sort( Population::SORT_FITNESS_DESCEND );
		}
	}


	void MultiIslandEA::Emigrate( MIGRATION_MODE mode )
	{
		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
		{
			Population *pPopulation	= m_pSolverArray[i]->GetPopulation();

			// 個体の順序をソートする
			if( mode==SELECT_BEST_REPLACE_WORST || mode==SELECT_BEST_REPLACE_RANDOM )// 適応値の高い個体を移住させる場合
			{
				pPopulation->Sort( Population::SORT_FITNESS_DESCEND );// 適応度で降順にソート
			}
			else if( mode==SELECT_RANDOM_REPLACE_WORST || mode==SELECT_RANDOM_REPLACE_RANDOM )// ランダム選択した個体を移住させる場合
			{
				pPopulation->Shuffle();// シャッフル
			}
			else
			{
			}

			// 個体群の情報を島から移民バッファにコピーする
			for( int emig=0; emig<m_Migrants[i].NumIndividuals(); ++emig )
				m_Migrants[i].Individual( emig )->CopyGeneFrom( pPopulation->Individual( emig ) );

		}// end of i loop
	}



	void MultiIslandEA::Immigrate( MIGRATION_MODE mode )
	{
		//=========== 移民船それぞれが向かう島を、重複しないように決める ===========//
		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )	m_Destinations[i]	= i;

		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
		{
			int j				= int( OreOreLib::genrand_real2() * m_MIGAAttrib.IslandSize );
			int temp			= m_Destinations[i];
			m_Destinations[i]	= m_Destinations[j];
			m_Destinations[j]	= temp;
		}


		//========================= 移民を受け入れる	==========================//
		for( int i=0; i<m_MIGAAttrib.IslandSize; ++i )
		{
			int destIsland				= m_Destinations[i];
			Population *pDestPopulation	= m_pSolverArray[destIsland]->GetPopulation();

			// 個体の順序をソートする
			if( mode==SELECT_BEST_REPLACE_WORST || mode==SELECT_RANDOM_REPLACE_WORST )// 適応値の低い島民個体を、移民で置き換える場合
			{
				pDestPopulation->Sort( Population::SORT_FITNESS_ASCEND );// 昇順(適応値が低い順)にソート
			}
			else if( mode==  SELECT_BEST_REPLACE_RANDOM || mode==SELECT_RANDOM_REPLACE_RANDOM )// ランダム選択した島民個体を、移民で置き換える場合
			{
				pDestPopulation->Shuffle();// シャッフル
			}
			else
			{
			}

			// 個体群の情報を島から移民バッファにコピーする
			for( int immig=0; immig<m_Migrants[i].NumIndividuals(); ++immig )
				m_Migrants[i].Individual( immig )->CopyGeneFrom( pDestPopulation->Individual( immig ) );

		}// end of i loop



		
	}



	void MultiIslandEA::ClearAttribute()
	{
		m_MIGAAttrib.Clear();
		m_Attrib.Clear();
	}



}// end of namespace