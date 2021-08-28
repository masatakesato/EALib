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



	bool CoDE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			// 親世代/子世代それぞれの個体情報
			m_Population[parentGen].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[childGen].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// トライアルベクターも初期化する
			m_Population[trialGen].Init( designParams, 3, numObjectives );

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
	//void CoDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//
	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[parentGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[childGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// トライアルベクターも初期化する
	//		m_Population[trialGen].Init( pChromosome, 3, numObjectives );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



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
		static OreOreLib::StaticArray<const IChromosome*, 13> X;// = { nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T;// = { nullptr };

		static OreOreLib::ArrayView<const IChromosome*>	X_Current_to_Rand_1( &X[0], 5 ),
														X_Rand_1( &X[5], 3 ),
														X_Rand_2( &X[8], 5 );

		m_DE_Best_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_2.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Current_to_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );

		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			IChromosome* x_i = m_Population[parentGen].GetIndividual( i );

			//====================== current-to-rand/1/bin =========================//
			// Select parents X{ current, rand2, rand3, rand4, rand5 }
			m_DE_Current_to_Rand_1.Execute( X_Current_to_Rand_1.Length(), (IChromosome**)X_Current_to_Rand_1.begin(), i );
			IChromosome* t_i_best1bin = m_Population[trialGen].GetIndividual( 0 );// Trial vector1
			t_i_best1bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_best1bin
			T[0] = t_i_best1bin;
			m_refCrossover->Execute2( X_Current_to_Rand_1, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 5, &X_Current_to_Rand_1[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			pEval->Evaluate( t_i_best1bin );


			//========================== rand/1/bin ====================//
			// Select parents X{ rand1, rand2, rand3 }
			m_DE_Rand_1.Execute( X_Rand_1.Length(), (IChromosome**)X_Rand_1.begin(), i );
			IChromosome* t_i_rand1bin	= m_Population[trialGen].GetIndividual( 1 );// Trial vector2
			t_i_rand1bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_rand1bin
			T[0] = t_i_rand1bin;
			m_refCrossover->Execute2( X_Rand_1, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 3, &X_Rand_1[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			pEval->Evaluate( t_i_rand1bin );


			//====================== rand/2/bin =========================//
			// Select parents X{ rand1, rand2, rand3, rand4, rand5 }
			m_DE_Rand_2.Execute( X_Rand_2.Length(), (IChromosome**)X_Rand_2.begin(), i );
			IChromosome* t_i_rand2bin	= m_Population[trialGen].GetIndividual( 2 );// Trial vector3
			t_i_rand2bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_rand2bin
			T[0] = t_i_rand2bin;
			m_refCrossover->Execute2( X_Rand_2, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 5, &X_Rand_2[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
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
		if( m_bReady )
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



	bool MixedCoDE::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			// 親世代/子世代それぞれの個体情報
			m_Population[parentGen].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[childGen].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// トライアルベクターも初期化する
			m_Population[trialGen].Init( designParams, 3, numObjectives );

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
	//void MixedCoDE::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//
	//		// 親世代/子世代それぞれの個体情報
	//		m_Population[parentGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[childGen].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// トライアルベクターも初期化する
	//		m_Population[trialGen].Init( pChromosome, 3, numObjectives );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



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

		static OreOreLib::StaticArray<const IChromosome*, 11> X;// = { nullptr, nullptr, nullptr, nullptr, nullptr };
		static OreOreLib::StaticArray<IChromosome*, 1> T;// = { nullptr };

		static OreOreLib::ArrayView<const IChromosome*>	X_Best_1( &X[0], 3 ),// = { best, rand1, rand2 }
														X_Rand_1( &X[3], 3 ),// = { rand1, rand2, rand3 }
														X_Rand_2( &X[6], 5 );// = { rand1, rand2, rand3, rand4, rand5 }

		m_DE_Best_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Rand_2.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );
		m_DE_Current_to_Rand_1.BindPopulationData( m_Attrib.PopulationSize, m_Population[parentGen].ChromosomeArray() );


		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			Chromosome2D* x_i	= (Chromosome2D *)m_Population[parentGen].GetIndividual( i );

			//====================== best/1/bin =========================//
			// Select parents X{ best, rand1, rand2 }
			m_DE_Best_1.Execute( X_Best_1.Length(), (IChromosome **)X_Best_1.begin(), i );
			IChromosome *t_i_best1bin	= m_Population[trialGen].GetIndividual( 0 );//Chromosome2D *t_i_best1bin = (Chromosome2D *)m_Population[trialGen].GetIndividual( 0 );// Trial vector1
			t_i_best1bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_best1bin
			T[0] = t_i_best1bin;
			m_refCrossover->Execute2( X_Best_1, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 3, &X_Best_1[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			pEval->Evaluate( t_i_best1bin );


			//========================== rand/1/bin ====================//
			// Select parents X{ rand1, rand2, rand3 }
			m_DE_Rand_1.Execute( X_Rand_1.Length(), (IChromosome **)X_Rand_1.begin(), i );
			IChromosome* t_i_rand1bin	= m_Population[trialGen].GetIndividual( 1 );//Chromosome2D *t_i_rand1bin	= (Chromosome2D *)m_Population[trialGen].GetIndividual( 1 );// Trial vector2
			t_i_rand1bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_rand1bin
			T[0] = t_i_rand1bin;
			m_refCrossover->Execute2( X_Rand_1, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 3, &X_Rand_1[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			pEval->Evaluate( t_i_rand1bin );


			//====================== rand/2/bin =========================//
			// 中間個体を生成する
			m_DE_Rand_2.Execute( X_Rand_2.Length(), (IChromosome **)X_Rand_2.begin(), i );
			IChromosome* t_i_rand2bin = m_Population[trialGen].GetIndividual( 2 );//Chromosome2D *t_i_rand2bin	= (Chromosome2D *)m_Population[trialGen].GetIndividual( 2 );// Trial vector3
			t_i_rand2bin->CopyGeneFrom( x_i );
			
			// Generate trial offspring t_i_rand2bin
			T[0] = t_i_rand2bin;
			m_refCrossover->Execute2( X_Rand_2, T, &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
			//m_refCrossover->Execute( 5, &X_Rand_2[0], 1, &T[0], &m_MutationAttrib[int( OreOreLib::genrand_real2() * 3.0 )] );
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
		if( m_bReady )
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