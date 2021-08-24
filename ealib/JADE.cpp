#include	"JADE.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/Random.h>



namespace ealib
{


	JADE::JADE()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		, m_numActiveArchives( 0 )
		//, m_ArchiveIndices()
		//, m_pFs()
		//, m_pCRs()
		, m_MuF( 0 )
		, m_MuCR( 0 )

	{
		ClearAttribute();
	}



	JADE::JADE( const JADE& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_DEAttrib( obj.m_DEAttrib )
		, m_JADEAttrib( obj.m_JADEAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, archive( 2 )
		, dummy( 3 )
		, m_numActiveArchives( obj.m_numActiveArchives )
		//, m_ArchiveIndices()
		//, m_Fs()
		//, m_CRs()
		, m_MuF( 0 )
		, m_MuCR( 0 )
	{

	}



	JADE::~JADE()
	{
		ReleasePopulation();
	}
	


	void JADE::SetAttribute( const DEAttribute& attrib )
	{
		m_DEAttrib	= attrib;
	}



	void JADE::SetAttribute( const JADEAttribute& attrib )
	{
		m_JADEAttrib = attrib;
	}



	void JADE::SetDifferentialWeight( float f )
	{
		m_DEAttrib.F = f;
	}



	void JADE::SetCrossoverProbability( float cr )
	{
		m_DEAttrib.CR = cr;
	}



	void JADE::SetBestPercentage( float p )
	{
		m_JADEAttrib.P	= p;
	}



	void JADE::SetLearningRate( float c )
	{
		m_JADEAttrib.C	= c;
	}



	void JADE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//

			// 親世代/子世代それぞれの個体情報
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// アーカイブ個体情報
			m_Population[ archive ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// F/CR
			m_Fs.Init( m_Attrib.PopulationSize );
			m_CRs.Init( m_Attrib.PopulationSize );

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			// アーカイブ個体インデックスの仮記憶バッファ
			m_ArchiveIndices.Init( m_Attrib.PopulationSize );


			m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].ChromosomeArray() );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}
	}



	// Deprecated. 2021.08.23
	//void JADE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//

	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// アーカイブ個体情報
	//		m_Population[ archive ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// F/CR
	//		m_Fs.Init( m_Attrib.PopulationSize );
	//		m_CRs.Init( m_Attrib.PopulationSize );

	//		// ダミーデータも初期化する
	//		m_Population[ dummy ].Init( pChromosome, 1, numObjectives );

	//		// アーカイブ個体インデックスの仮記憶バッファ
	//		m_ArchiveIndices.Init( m_Attrib.PopulationSize );


	//		m_Mutator.BindArchives( m_numActiveArchives, m_Population[archive].ChromosomeArray() );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void JADE::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ archive ].Release();
		m_Population[ dummy ].Release();

		m_Fs.Release();
		m_CRs.Release();

		m_numActiveArchives	= 0;
		m_ArchiveIndices.Release();

		m_MuF				= 0.0f;
		m_MuCR				= 0.0f;

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* JADE::Clone() const
	{
		return new JADE( *this );
	}



	void JADE::Step( Evaluator* pEval )
	{

		float sumF			= 0.0f;
		float sumFSquare	= 0.0f;
		float sumCR			= 0.0f;
		int countSuccess	= 0;

		int	numArchives		= 0;


		UpdateControlParams();
		m_Mutator.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_Mutator.SetNumArchives( m_numActiveArchives );


		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome *x_i	= m_Population[ parentGen ].GetIndividual( i );
			IChromosome *t_i	= m_Population[ dummy ].GetIndividual( 0 );// 中間個体
			IChromosome* refCandidates[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

			//=================	Mutation and Crossover	===================//			
			m_Mutator.Execute( 5, &refCandidates[1], i );//m_Mutator.Execute( 5, refCandidates, i );
			refCandidates[0] = t_i;
			//IChromosome *randoms[] =
			//{
			//	t_i,				// t_i. trial vector
			//	refCandidates[0],	// x_i
			//	refCandidates[1],	// pbest
			//	refCandidates[2],	// x_i
			//	refCandidates[3],	// x_r1
			//	refCandidates[4],	// x_r2
			//};
			
			// 中間個体を生成する
			t_i->CopyGeneFrom( x_i );
			int id = x_i->ID();// 個体IDを使って、x_iとm_pFs[i]/m_pCRs[i]を一義的に割り当てる？→世代ごとに、個体別F/CRを新規生成するから多分関係ない
			DEAttribute attr = { m_Fs[id], m_CRs[id],  m_Fs[id] };
			m_refCrossover->Execute( 6, refCandidates, &attr );//m_refCrossover->Execute( 6, randoms, &attr );

			pEval->Evaluate( t_i );

			//======================	Selection	=======================//
			// 親個体と中間個体を比較して、適応度が高い個体を選択して次世代に残す
			if( t_i->GetFitness() > x_i->GetFitness() )
			{
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( t_i );
				
				sumF		+= m_Fs[id];
				sumFSquare	+= m_Fs[id] * m_Fs[id];
				sumCR		+= m_CRs[id];
				countSuccess++;

				// x_i をアーカイブする。この時点では、アーカイブ予定の親世代個体のインデックスiだけ記憶する
				m_ArchiveIndices[numArchives++]	= i;
			}
			else
			{
				m_Population[childGen].GetIndividual( i )->CopyGeneFrom( x_i );
			}

		}// end of i loop
		

		//===================	Update Archives		========================//
		for( int i=0; i<numArchives; ++i )
		{
			int archive_idx	= m_ArchiveIndices[i];
			int dest_idx = m_numActiveArchives >= m_Attrib.PopulationSize ? (int)( OreOreLib::genrand_real2() * (double)m_Attrib.PopulationSize ) : m_numActiveArchives++;
			
			m_Population[archive].GetIndividual( dest_idx )->CopyGeneFrom( m_Population[parentGen].GetIndividual( archive_idx ) );
		}


		//===================	Update m_MuF and m_MuCR	=====================//
		float c_inv = 1.0f - m_JADEAttrib.C;
		m_MuF	= c_inv * m_MuF;
		m_MuCR	= c_inv * m_MuCR;
		if( countSuccess > 0 )
		{
			m_MuF	+= m_JADEAttrib.C * sumFSquare / sumF;
			m_MuCR	+= m_JADEAttrib.C * sumCR / (float)countSuccess;
		}

		// 1世代分の処理が終わったら、親世代/子世代のバッファをスワップする
		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Sort( Population::SORT_FITNESS_DESCEND );

		m_Stats.Update( m_Population[parentGen] );
	}



	void JADE::Evolve( Evaluator* pEval, unsigned int seed )
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
			m_CRs[i]	= m_DEAttrib.CR;
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



	void JADE::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}



	void JADE::UpdateControlParams()
	{
		
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			// Update F
			double tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			while( tempF < 0 )	tempF = OreOreLib::rand_cauchy( (double)m_MuF, 0.1 );
			m_Fs[i] = (float)Min(tempF, 1.0);

			// Update CR
			double tempCR = OreOreLib::rand_normal( (double)m_MuCR, 0.1 );
			m_CRs[i]	= (float)Saturate( tempCR );

		}// end of i loop
	}



	void JADE::ClearAttribute()
	{
		m_Attrib.PopulationSize	= 100;
		m_Attrib.NumGenerations	= 1000;
		m_Attrib.EliteSize		= 0;

		m_DEAttrib.F			= 0.5f;
		m_DEAttrib.CR			= 0.5f;
		m_DEAttrib.F2			= 0.5f;

		m_JADEAttrib.C			= 0.1f;
		m_JADEAttrib.P			= 1.0f;
	}



}// end of namespace