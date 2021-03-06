#include	"SHADE.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/Random.h>

#include	"Chromosome2D.h"



namespace ealib
{

	//##########################################################################################//
	//										SHADE												//
	//##########################################################################################//

	SHADE::SHADE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		//, m_Fs()
		//, m_CRs()
		//, m_Ps()
		, m_MuF( 0 )
		, m_MuCR( 0 )
		, m_Pmin( 0 )
		, m_numActiveArchives( 0 )
		//, m_ArchiveIndices()

	{
		ClearAttribute();
	}



	SHADE::SHADE( const SHADE& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_DEAttrib( obj.m_DEAttrib )
		, m_SHADEAttrib( obj.m_SHADEAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		//, m_Fs()
		//, m_CRs()
		//, m_Ps()
		, m_MuF( 0 )
		, m_MuCR( 0 )
		, m_Pmin( 0 )
		, m_numActiveArchives( obj.m_numActiveArchives )
		//, m_ArchiveIndices()
	{

	}



	SHADE::~SHADE()
	{
		ReleasePopulation();
	}

	

	void SHADE::SetAttribute( const DEAttribute& attrib )
	{
		m_DEAttrib	= attrib;
	}



	void SHADE::SetAttribute( const SHADEAttribute& attrib )
	{
		m_SHADEAttrib = attrib;
	}



	void SHADE::SetDifferentialWeight( float f )
	{
		m_DEAttrib.F = f;
	}



	void SHADE::SetCrossoverProbability( float cr )
	{
		m_DEAttrib.CR = cr;
	}



	void SHADE::SetBestPercentage( float p )
	{
		m_SHADEAttrib.P	= p;
	}



	void SHADE::SetLearningRate( float c )
	{
		m_SHADEAttrib.C	= c;
	}



	bool SHADE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//

			// 親世代/子世代それぞれの個体情報
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// アーカイブ個体情報
			m_Population[archive].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// F/CR
			m_Fs.Init( m_Attrib.PopulationSize );
			m_CRs.Init( m_Attrib.PopulationSize );
			m_Ps.Init( m_Attrib.PopulationSize );

			m_Pmin	= 2.0f / (float)m_Attrib.PopulationSize;

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			m_ArchiveIndices.Init( m_Attrib.PopulationSize );


			m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].Indivuduals() );

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
	//void SHADE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//

	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// アーカイブ個体情報
	//		m_Population[archive].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// F/CR
	//		m_Fs.Init( m_Attrib.PopulationSize );
	//		m_CRs.Init( m_Attrib.PopulationSize );
	//		m_Ps.Init( m_Attrib.PopulationSize );

	//		m_Pmin	= 2.0f / (float)m_Attrib.PopulationSize;

	//		// ダミーデータも初期化する
	//		m_Population[ dummy ].Init( pChromosome, 1, numObjectives );

	//		m_ArchiveIndices.Init( m_Attrib.PopulationSize );


	//		m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].Indivuduals() );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void SHADE::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ archive ].Release();
		m_Population[ dummy ].Release();

		m_Fs.Release();
		m_CRs.Release();
		m_Ps.Release();

		m_MuF	= 0.0f;
		m_MuCR	= 0.0f;
		m_Pmin	= 0.0f;

		m_numActiveArchives	= 0;
		m_ArchiveIndices.Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* SHADE::Clone() const
	{
		return new SHADE( *this );
	}



	void SHADE::Step( Evaluator* pEval )
	{
		static OreOreLib::StaticArray<const IChromosome*, 5> X = { nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T = { nullptr };

		float sumF			= 0.0f;
		float sumFSquare	= 0.0f;
		float sumCR			= 0.0f;
		int countSuccess	= 0;

		int	numArchives		= 0;


		UpdateControlParams();
		m_Mutator.BindPopulationData( m_Population[parentGen].Indivuduals() );
		m_Mutator.SetNumArchives( m_numActiveArchives );


		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome *x_i	= m_Population[ parentGen ].Individual( i );
			IChromosome *t_i	= m_Population[ dummy ].Individual( 0 );// 中間個体


			//=================	DE_Current_to_pBest_1_Archive Mutation and Crossover	===================//
			// Select parents X{ x_i, pbest, x_i, x_r1, x_r2 }
			int id = x_i->ID();// 個体IDを使って、x_iとm_pFs[i]/m_pCRs[i]を一義的に割り当てる？→世代ごとに、個体別F/CRを新規生成するから多分関係ない
			m_Mutator.SetP( m_Ps[id] );
			m_Mutator.Execute( X, i );
			
			// Generate offspring t_i
			T[0] = t_i;
			t_i->CopyGeneFrom( x_i );
			DEAttribute attr = { m_Fs[id], m_CRs[id],  m_Fs[id] };
			m_refCrossover->Execute( X, T, &attr );
			//m_refCrossover->Execute( 5, &X[0], 1, &T[0], &attr );
			pEval->Evaluate( t_i );


			//======================	Selection	=======================//
			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->Fitness() > x_i->Fitness() )
			{
				m_Population[childGen].Individual( i )->CopyGeneFrom( t_i );
				
				sumF		+= m_Fs[id];
				sumFSquare	+= m_Fs[id] * m_Fs[id];
				sumCR		+= m_CRs[id];
				countSuccess++;

				// x_i をアーカイブする。この時点では、アーカイブ予定の親世代個体のインデックスiだけ記憶する
				m_ArchiveIndices[numArchives++]	= i;
			}
			else
			{
				m_Population[childGen].Individual( i )->CopyGeneFrom( x_i );
			}

		}// end of i loop
		

		//===================	Update Archives		========================//
		for( int i=0; i<numArchives; ++i )
		{
			int archive_idx	= m_ArchiveIndices[i];
			int dest_idx = m_numActiveArchives >= m_Attrib.PopulationSize ? (int)( OreOreLib::genrand_real2() * (double)m_Attrib.PopulationSize ) : m_numActiveArchives++;
			
			m_Population[archive].Individual( dest_idx )->CopyGeneFrom( m_Population[parentGen].Individual( archive_idx ) );
		}


		//===================	Update m_MuF and m_MuCR	=====================//
		float c_inv = 1.0f - m_SHADEAttrib.C;
		m_MuF	= c_inv * m_MuF;
		m_MuCR	= c_inv * m_MuCR;
		if( countSuccess > 0 )
		{
			m_MuF	+= m_SHADEAttrib.C * sumFSquare / sumF;
			m_MuCR	+= m_SHADEAttrib.C * sumCR / (float)countSuccess;
		}

		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Sort( Population::SORT_FITNESS_DESCEND );

		m_Stats.Update( m_Population[parentGen] );
	}



	void SHADE::Evolve( Evaluator* pEval, unsigned int seed )
	{	
		if( !m_bReady )
			return;

		OreOreLib::init_genrand( seed );
		
		//============	Initialize	=============//
		// Generate 1st population (randomize variables and evaluate)
		m_Population[ parentGen ].Initialize( m_refInitializer, pEval );// Random initialization
		m_Stats.Reset( m_Population[ parentGen ] );

		// Initialize F/CR
		m_MuF	= m_DEAttrib.F;
		m_MuCR	= m_DEAttrib.CR;

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			m_Fs[i]	= m_DEAttrib.F;
			m_CRs[i] = m_DEAttrib.CR;
		}

		m_numActiveArchives	= 0;

		//============= Do Iteration	==========//
		while( !m_refCriteria->CheckTermination( &m_Stats ) )
		{
			Step( pEval );
		}

		//for( int gen=0; gen<m_Attrib.NumGenerations; ++gen )
		//{
		//	Step( pEval );
		//}

		m_Mutator.UnbindPopulationData();
		m_Mutator.UnbindArchives();
	}



	void SHADE::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[parentGen] );
	}



	void SHADE::UpdateControlParams()
	{
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			// Update F
			double tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			while( tempF < 0 )	tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			m_Fs[i] = (float)Min(tempF, 1.0);

			// Update CR
			double tempCR = OreOreLib::rand_normal( (double)m_MuCR, 0.1 );
			m_CRs[i] = (float)Saturate( tempCR );

			// Update P
			m_Ps[i]	= ( 0.2f - m_Pmin ) * (float)OreOreLib::genrand_real1() + m_Pmin;

		}// end of i loop
	}



	void SHADE::ClearAttribute()
	{
		m_Attrib.PopulationSize	= 100;
		m_Attrib.NumGenerations	= 1000;
		m_Attrib.EliteSize		= 0;

		m_DEAttrib.F			= 0.5f;
		m_DEAttrib.CR			= 0.5f;
		m_DEAttrib.F2			= 0.5f;

		m_SHADEAttrib.C			= 0.1f;
		m_SHADEAttrib.P			= 1.0f;
	}





	//##########################################################################################//
	//										MixedSHADE											//
	//##########################################################################################//

	MixedSHADE::MixedSHADE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		//, m_Fs()
		//, m_CRs()
		//, m_Ps()
		, m_MuF( 0 )
		, m_MuCR( 0 )
		, m_Pmin( 0 )
		, m_numActiveArchives( 0 )
		//, m_ArchiveIndices( nullptr )
	{
		ClearAttribute();
	}



	MixedSHADE::MixedSHADE( const MixedSHADE& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_DEAttrib( obj.m_DEAttrib )
		, m_SHADEAttrib( obj.m_SHADEAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		//, m_Fs()
		//, m_CRs()
		//, m_Ps()
		, m_MuF( 0 )
		, m_MuCR( 0 )
		, m_Pmin( 0 )
		, m_numActiveArchives( obj.m_numActiveArchives )
		//, m_ArchiveIndices( nullptr )
	{

	}



	MixedSHADE::~MixedSHADE()
	{
		ReleasePopulation();
	}



	void MixedSHADE::SetAttribute( const DEAttribute& attrib )
	{
		m_DEAttrib	= attrib;
	}



	void MixedSHADE::SetAttribute( const SHADEAttribute& attrib )
	{
		m_SHADEAttrib = attrib;
	}



	void MixedSHADE::SetDifferentialWeight( float f )
	{
		m_DEAttrib.F = f;
	}



	void MixedSHADE::SetCrossoverProbability( float cr )
	{
		m_DEAttrib.CR = cr;
	}



	void MixedSHADE::SetBestPercentage( float p )
	{
		m_SHADEAttrib.P	= p;
	}



	void MixedSHADE::SetLearningRate( float c )
	{
		m_SHADEAttrib.C	= c;
	}



	bool MixedSHADE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//

			// 親世代/子世代それぞれの個体情報
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// アーカイブ個体情報
			m_Population[archive].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// F/CR
			m_Fs.Init( m_Attrib.PopulationSize );
			m_CRs.Init( m_Attrib.PopulationSize );
			m_Ps.Init( m_Attrib.PopulationSize );

			m_Pmin	= 2.0f / (float)m_Attrib.PopulationSize;

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			m_ArchiveIndices.Init( m_Attrib.PopulationSize );

			m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].Indivuduals() );

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
	//void MixedSHADE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//

	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// アーカイブ個体情報
	//		m_Population[archive].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// F/CR
	//		m_Fs.Init( m_Attrib.PopulationSize );
	//		m_CRs.Init( m_Attrib.PopulationSize );
	//		m_Ps.Init( m_Attrib.PopulationSize );

	//		m_Pmin	= 2.0f / (float)m_Attrib.PopulationSize;

	//		// ダミーデータも初期化する
	//		m_Population[ dummy ].Init( pChromosome, 1, numObjectives );

	//		m_ArchiveIndices	= new int[ m_Attrib.PopulationSize ];

	//		m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].Indivuduals() );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void MixedSHADE::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ archive ].Release();
		m_Population[ dummy ].Release();

		m_Fs.Release();
		m_CRs.Release();
		m_Ps.Release();

		m_MuF	= 0.0f;
		m_MuCR	= 0.0f;
		m_Pmin	= 0.0f;

		m_numActiveArchives	= 0;
		m_ArchiveIndices.Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* MixedSHADE::Clone() const
	{
		return new MixedSHADE( *this );
	}



	void MixedSHADE::Step( Evaluator* pEval )
	{
		static OreOreLib::StaticArray<const IChromosome*, 5> X = { nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T = { nullptr };
		//Chromosome2D* refCandidates[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, };

		float sumF			= 0.0f;
		float sumFSquare	= 0.0f;
		float sumCR			= 0.0f;
		int countSuccess	= 0;

		int	numArchives		= 0;
		int32 numChromTypes	= m_Population[parentGen].Individual(0)->NumChromosomeTypes();

		UpdateControlParams();
		m_Mutator.BindPopulationData( m_Population[parentGen].Indivuduals() );
		m_Mutator.SetNumArchives( m_numActiveArchives );


		for( int32 i=0; i<m_Attrib.PopulationSize; ++i )
		{
			Chromosome2D* x_i	= (Chromosome2D *)m_Population[ parentGen ].Individual( i );
			Chromosome2D* t_i	= (Chromosome2D *)m_Population[ dummy ].Individual( 0 );// Trial vector1// 中間個体


			//=================	DE_Current_to_pBest_1_Archive Mutation and Crossover	===================//
			// Select parents X{ x_i, pbest, x_i, x_r1, x_r2 }
			int id = x_i->ID();// 個体IDを使って、x_iとm_pFs[i]/m_pCRs[i]を一義的に割り当てる？→世代ごとに、個体別F/CRを新規生成するから多分関係ない
			m_Mutator.SetP( m_Ps[id] );
			m_Mutator.Execute( X, i );

			// Generate offspring t_i
			T[0] = t_i;
			t_i->CopyGeneFrom( x_i );
			DEAttribute attr = { m_Fs[id], m_CRs[id],  m_Fs[id] };
			m_refCrossover->Execute( X, T, &attr );
			pEval->Evaluate( t_i );


			//======================	Selection	=======================//
			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->Fitness() > x_i->Fitness() )
			{
				m_Population[childGen].Individual( i )->CopyGeneFrom( t_i );

				sumF		+= m_Fs[id];
				sumFSquare	+= m_Fs[id] * m_Fs[id];
				sumCR		+= m_CRs[id];
				countSuccess++;

				// x_i をアーカイブする。この時点では、アーカイブ予定の親世代個体のインデックスiだけ記憶する
				m_ArchiveIndices[numArchives++]	= i;
			}
			else
			{
				m_Population[childGen].Individual( i )->CopyGeneFrom( x_i );
			}

		}// end of i loop


		 //===================	Update Archives		========================//
		for( int i=0; i<numArchives; ++i )
		{
			int archive_idx	= m_ArchiveIndices[i];
			int dest_idx = m_numActiveArchives >= m_Attrib.PopulationSize ? (int)( OreOreLib::genrand_real2() * (double)m_Attrib.PopulationSize ) : m_numActiveArchives++;

			m_Population[archive].Individual( dest_idx )->CopyGeneFrom( m_Population[parentGen].Individual( archive_idx ) );
		}


		//===================	Update m_MuF and m_MuCR	=====================//
		float c_inv = 1.0f - m_SHADEAttrib.C;
		m_MuF	= c_inv * m_MuF;
		m_MuCR	= c_inv * m_MuCR;
		if( countSuccess > 0 )
		{
			m_MuF	+= m_SHADEAttrib.C * sumFSquare / sumF;
			m_MuCR	+= m_SHADEAttrib.C * sumCR / (float)countSuccess;
		}

		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Sort( Population::SORT_FITNESS_DESCEND );

		m_Stats.Update( m_Population[parentGen] );
	}



	void MixedSHADE::Evolve( Evaluator* pEval, unsigned int seed )
	{
		if( !m_bReady )
			return;

		OreOreLib::init_genrand( seed );

		//============	Initialize	=============//
		// Generate 1st population (randomize variables and evaluate)
		m_Population[ parentGen ].Initialize( m_refInitializer, pEval );// Random initialization
		m_Stats.Reset( m_Population[ parentGen ] );

		// Initialize F/CR
		m_MuF	= m_DEAttrib.F;
		m_MuCR	= m_DEAttrib.CR;

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			m_Fs[i] = m_DEAttrib.F;
			m_CRs[i] = m_DEAttrib.CR;
		}

		m_numActiveArchives	= 0;

		//============= Do Iteration	==========//
		while( !m_refCriteria->CheckTermination( &m_Stats ) )
		{
			Step( pEval );
		}

		//for( int gen=0; gen<m_Attrib.NumGenerations; ++gen )
		//{
		//	Step( pEval );
		//}

		m_Mutator.UnbindPopulationData();
		m_Mutator.UnbindArchives();
	}



	void MixedSHADE::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[parentGen] );
	}



	void MixedSHADE::UpdateControlParams()
	{
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			// Update F
			double tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			while( tempF < 0 )	tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			m_Fs[i] = (float)Min( tempF, 1.0 );

			// Update CR
			double tempCR = OreOreLib::rand_normal( (double)m_MuCR, 0.1 );
			m_CRs[i] = (float)Saturate( tempCR );

			// Update P
			m_Ps[i] = ( 0.2f - m_Pmin ) * (float)OreOreLib::genrand_real1() + m_Pmin;

		}// end of i loop
	}



	void MixedSHADE::ClearAttribute()
	{
		m_Attrib.PopulationSize	= 100;
		m_Attrib.NumGenerations	= 1000;
		m_Attrib.EliteSize		= 0;

		m_DEAttrib.F			= 0.5f;
		m_DEAttrib.CR			= 0.5f;
		m_DEAttrib.F2			= 0.5f;

		m_SHADEAttrib.C			= 0.1f;
		m_SHADEAttrib.P			= 1.0f;
	}



}// end of namespace