﻿#include	"SimpleGA.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"Chromosome2D.h"



namespace ealib
{


	//##########################################################################################//
	//									SimpleGA												//
	//##########################################################################################//

	SimpleGA::SimpleGA()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		, m_NumFamilies( 0 )
	{
		ClearAttribute();
	}



	SimpleGA::SimpleGA( const SimpleGA& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_SGAAttrib( obj.m_SGAAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		, m_NumFamilies( obj.m_NumFamilies )
		, X( obj.X )
		, T( obj.T )
	{

	}



	SimpleGA::~SimpleGA()
	{
		ReleasePopulation();
	}

	

	void SimpleGA::SetAttribute( const SGAAttribute& attrib )
	{
		m_SGAAttrib	= attrib;
	}



	void SimpleGA::SetCrossoverRate( float c_rate )
	{
		m_SGAAttrib.CrossoverRate = c_rate;
	}



	void SimpleGA::SetMutationRate( float m_rate )
	{
		m_SGAAttrib.MutationRate = m_rate;
	}



	// Deprecated. 2021.08.23
	//void SimpleGA::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// 次世代個体の親を格納するバッファの確保
	//		m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );
	//		m_Parents.Init( DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 ) );

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



	bool SimpleGA::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// 次世代個体の親を格納するバッファの確保
			m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );

			m_NumFamilies	= DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 );
		
			if( m_refCrossover )
			{
				int numParents=0, numChildren=0;
				if( m_refCrossover->GetFamilySize( numParents, numChildren ) == false )
					throw CrossoverException();
				X.Init( numParents );
				T.Init( numChildren );

				// ダミーデータも初期化する
				m_Population[ dummy ].Init( designParams, numChildren, numObjectives );
			}

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}

		return m_bReady;

	}



	void SimpleGA::ReleasePopulation()
	{
		m_Population[parentGen].Release();
		m_Population[childGen].Release();

		X.Release();
		T.Release();

		m_Population[ dummy ].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* SimpleGA::Clone() const
	{
		return new SimpleGA( *this );
	}



	void SimpleGA::Step( Evaluator* pEval )
	{
		//Select( &m_Population[parentGen] );
		//Crossover( &m_Population[parentGen], &m_Population[childGen] );
		Select_Crossover( &m_Population[parentGen], &m_Population[childGen] );
		Mutate( &m_Population[childGen] );
		CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		parentGen	^= 1;
		childGen	^= 1;

		m_Population[ parentGen ].Evaluate( pEval );

		m_Stats.Update( m_Population[ parentGen ] );
	}



	void SimpleGA::Evolve( Evaluator* pEval, unsigned int seed )
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

	}



	void SimpleGA::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[parentGen] );
	}





	// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	void SimpleGA::Select_Crossover( Population* pParentPopulation, Population* pChildPopulation )
	{
		// 適応値と親個体の初期化
		if( !m_refSelector )
			return;
		
		m_refSelector->BindPopulationData( pParentPopulation->Indivuduals() );
		m_refSelector->Update();

		int32 numParents = X.Length<int32>();
		int32 numChildren = T.Length<int32>();

		for( int i=0; i<m_NumFamilies; ++i )
		{
			// Select Parents
			for( int32 j=0; j<numParents; ++j )
			{
				const IChromosome* chrom = m_Population[ parentGen ].Individual( m_refSelector->Execute() );

				while( OreOreLib::Exists( X, chrom ) )
					chrom = m_Population[ parentGen ].Individual( m_refSelector->Execute() );
				
				X[j] = chrom;
			}// end of j loop


			// Construct children group T
			for( int32 j=0; j<numChildren; ++j )
			{
				int popidx = i*numChildren + j;

				if( popidx < m_Attrib.PopulationSize-m_Attrib.EliteSize )// Refer pChindPopulation's individual if possible
					T[j] = pChildPopulation->Individual( popidx );
				else
					T[j] = m_Population[ dummy ].Individual( j );// Refer dummy data if popidx is out of range.
			}

			// Crossover
			float crossProb	= float( OreOreLib::genrand_real1() );
			
			if( crossProb > m_SGAAttrib.CrossoverRate || !m_refCrossover )
			{
				//tcout << i << ": Ignoring Crossover..." << tendl;
				// Keep parents as offsprings
				for( int j=0; j<numChildren; ++j )
					T[j]->CopyGeneFrom( X[j] );

				continue;
			}
			
			m_refCrossover->Execute( X, T, nullptr );//m_refCrossover->Execute( m_NumParents, &X[0], m_NumChildren, &T[0], nullptr );

		}// end of i loop

	}





	// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	//void SimpleGA::Select( Population* pPopulation )
	//{
	//	// 適応値と親個体の初期化
	//	if( !m_refSelector )
	//		return;
	//	
	//	m_refSelector->BindPopulationData( pPopulation->Indivuduals() );
	//	m_refSelector->Update();

	//	// 親の選択
	//	for( int i=0; i<m_Parents.Length(); ++i )
	//	{
	//		int parent1	= m_refSelector->Execute();
	//		int parent2	= m_refSelector->Execute();

	//		while( parent1==parent2 )
	//			parent2 = m_refSelector->Execute();
	//			
	//		m_Parents[i].first = parent1;
	//		m_Parents[i].second = parent2;
	//	}// end of i loop
	//}



	// 交叉処理を行う
	//void SimpleGA::Crossover( Population* pParentPopulation, Population* pChildPopulation )
	//{
	//	for( int i=0; i<m_Parents.Length(); ++i )
	//	{
	//		IChromosome* p1	= pParentPopulation->Individual( m_Parents[i].first );
	//		IChromosome* p2	= pParentPopulation->Individual( m_Parents[i].second );
	//		IChromosome* c1	= pChildPopulation->Individual( i*2 );
	//		IChromosome* c2	= ( i*2+1 )<( m_Attrib.PopulationSize-m_Attrib.EliteSize ) ? pChildPopulation->Individual( i*2+1 ) : m_Population[ dummy ].Individual( 0 );

	//		float crossProb	= float( OreOreLib::genrand_real1() );
	//		
	//		if( crossProb > m_SGAAttrib.CrossoverRate )
	//		{
	//			//tcout << i << ": Ignoring Crossover..." << tendl;
	//			// とりあえず現行世代の遺伝子をそのまま残す
	//			c1->CopyGeneFrom( p1 );
	//			c2->CopyGeneFrom( p2 );

	//			continue;
	//		}
	//		
	//		IChromosome* chromosomes[] ={ p1, p2, c1, c2 };
	//		m_refCrossover->Execute( 4, chromosomes, nullptr );

	//	}// end of i loop

	//}



	void SimpleGA::Mutate( Population* pPopulation )
	{
		for( int i=0; i<m_Attrib.PopulationSize-m_Attrib.EliteSize; ++i )
			m_refMutator->Execute( pPopulation->Individual(i), m_SGAAttrib.MutationRate );

	}

	

	void SimpleGA::CarryOver( Population* pParentPopulation, Population* pChildPopulation )
	{
		IChromosome *pParent, *pChild;

		int dst_start	= m_Attrib.PopulationSize - m_Attrib.EliteSize;
		for( int i=0; i<m_Attrib.EliteSize; ++i )
		{
			pParent	= pParentPopulation->Individual(i);
			pChild	= pChildPopulation->Individual( dst_start+i );

			pChild->CopyGeneFrom( pParent );
		}
	}



	void SimpleGA::ClearAttribute()
	{
		m_Attrib.Clear();
		m_SGAAttrib.Clear();
	}




	
	//##########################################################################################//
	//									MixedSimpleGA											//
	//##########################################################################################//


	MixedSimpleGA::MixedSimpleGA()
		: IEvolutionaryAlgorithm()
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		, m_NumFamilies( 0 )
	{
		ClearAttribute();
	}



	MixedSimpleGA::MixedSimpleGA( const MixedSimpleGA& obj )
		: IEvolutionaryAlgorithm( obj )
		, m_SGAAttrib( obj.m_SGAAttrib )
		, parentGen( 0 )
		, childGen( 1 )
		, dummy( 2 )
		, m_NumFamilies( obj.m_NumFamilies )
		, X( obj.X )
		, T( obj.T )
	{

	}



	MixedSimpleGA::~MixedSimpleGA()
	{
		ReleasePopulation();
	}



	void MixedSimpleGA::SetAttribute( const SGAAttribute& attrib )
	{
		m_SGAAttrib	= attrib;
	}



	void MixedSimpleGA::SetCrossoverRate( float c_rate )
	{
		m_SGAAttrib.CrossoverRate = c_rate;
	}



	void MixedSimpleGA::SetMutationRate( float m_rate )
	{
		m_SGAAttrib.MutationRate = m_rate;
	}



	bool MixedSimpleGA::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// 次世代個体の親を格納するバッファの確保
			m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );

			m_NumFamilies	= DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 );

			if( m_refCrossover )
			{
				int numParents=0, numChildren=0;
				if( m_refCrossover->GetFamilySize( numParents, numChildren ) == false )
					throw CrossoverException();
				X.Init( numParents );
				T.Init( numChildren );
				// ダミーデータも初期化する
				m_Population[ dummy ].Init( designParams, numChildren, numObjectives );
			}


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
	//void MixedSimpleGA::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//
	//		m_Population[ parentGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ childGen ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// 次世代個体の親を格納するバッファの確保
	//		m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );
	//		m_Parents.Init( DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 ) );

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



	void MixedSimpleGA::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();

		X.Release();
		T.Release();

		m_Population[ dummy ].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* MixedSimpleGA::Clone() const
	{
		return new MixedSimpleGA( *this );
	}



	void MixedSimpleGA::Step( Evaluator* pEval )
	{
		Select_Crossover( &m_Population[parentGen], &m_Population[childGen] );
		Mutate( &m_Population[childGen] );
		CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Evaluate( pEval );

		m_Stats.Update( m_Population[parentGen] );
	}



	void MixedSimpleGA::Evolve( Evaluator* pEval, unsigned int seed )
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

	}



	void MixedSimpleGA::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[parentGen] );
	}



	// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	void MixedSimpleGA::Select_Crossover( Population* pParentPopulation, Population* pChildPopulation )
	{
		// 適応値と親個体の初期化
		if( !m_refSelector )
			return;
		
		m_refSelector->BindPopulationData( pParentPopulation->Indivuduals() );
		m_refSelector->Update();

		int32 numParents = X.Length<int32>();
		int32 numChildren = T.Length<int32>();


		for( int32 i=0; i<m_NumFamilies; ++i )
		{

			// Select Parents
			for( int j=0; j<numParents; ++j )
			{
				const IChromosome* chrom = m_Population[ parentGen ].Individual( m_refSelector->Execute() );

				while( OreOreLib::Exists( X, chrom ) )
					chrom = m_Population[ parentGen ].Individual( m_refSelector->Execute() );
				
				X[j] = chrom;
			}// end of j loop


			// Construct children group T
			for( int32 j=0; j<numChildren; ++j )
			{
				int popidx = i*numChildren + j;

				if( popidx < m_Attrib.PopulationSize-m_Attrib.EliteSize )// Refer pChindPopulation's individual if possible
					T[j] = pChildPopulation->Individual( popidx );
				else
					T[j] = m_Population[ dummy ].Individual( j );// Refer dummy data if popidx is out of range.
			}

			float crossProb	= float( OreOreLib::genrand_real1() );
			
			if( crossProb > m_SGAAttrib.CrossoverRate || !m_refCrossover )
			{
				//tcout << i << ": Ignoring Crossover..." << tendl;
				// Keep parents as offsprings
				for( int j=0; j<numChildren; ++j )
					T[j]->CopyGeneFrom( X[j] );

				continue;
			}
			
			m_refCrossover->Execute( X, T, nullptr );

		}// end of i loop

	}



	void MixedSimpleGA::Mutate( Population* pPopulation )
	{
		for( int32 i=0; i<m_Attrib.PopulationSize-m_Attrib.EliteSize; ++i )
		{
			Chromosome2D* pChrom = (Chromosome2D*)( pPopulation->Individual(i) );

			for( int32 j=0; j<pChrom->NumChromosomeTypes(); ++j )
				m_refMutator->Execute( pChrom->GetChromosome(j), m_SGAAttrib.MutationRate );

		}// end of i loop
	}



	void MixedSimpleGA::CarryOver( Population* pParentPopulation, Population* pChildPopulation )
	{
		IChromosome *pParent, *pChild;

		int dst_start	= m_Attrib.PopulationSize - m_Attrib.EliteSize;
		for( int i=0; i<m_Attrib.EliteSize; ++i )
		{
			pParent	= pParentPopulation->Individual( i );
			pChild	= pChildPopulation->Individual( dst_start+i );

			pChild->CopyGeneFrom( pParent );
		}
	}



	void MixedSimpleGA::ClearAttribute()
	{
		m_Attrib.Clear();
		m_SGAAttrib.Clear();
	}





	// Deprecated. Using Select_Crossover method.

	//// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	//void MixedSimpleGA::Select( Population* pPopulation )
	//{
	//	// 適応値と親個体の初期化
	//	if(	!m_refSelector )
	//		return;
	
	//	m_refSelector->BindPopulationData( pPopulation->Indivuduals() );
	//	m_refSelector->Update();
	
	//	// 親の選択
	//	for( int i=0; i<m_Parents.Length(); ++i )
	//	{
	//		int parent1	= m_refSelector->Execute();
	//		int parent2	= m_refSelector->Execute();
	
	//		while( parent1==parent2 )
	//			parent2 = m_refSelector->Execute();
	
	//		m_Parents[i].first = parent1;
	//		m_Parents[i].second = parent2;
	
	//	}// end of i loop
	//}
	
	
	
	//// 交叉処理を行う
	//void MixedSimpleGA::Crossover( Population* pParentPopulation, Population* pChildPopulation )
	//{
	//	static OreOreLib::StaticArray<const IChromosome*, 2> X = { nullptr, nullptr };
	//	static OreOreLib::StaticArray<IChromosome*, 2> T = { nullptr, nullptr };
	
	//	for( int i=0; i<m_Parents.Length(); ++i )
	//	{
	//		X[0] = pParentPopulation->Individual( m_Parents[i].first );
	//		X[1] = pParentPopulation->Individual( m_Parents[i].second );
	//		T[0] = pChildPopulation->Individual( i*2 );
	//		T[1] = ( i*2+1 )<( m_Attrib.PopulationSize-m_Attrib.EliteSize ) ? pChildPopulation->Individual( i*2+1 ) : m_Population[ dummy ].Individual( 0 );
	
	//		float crossProb	= float( OreOreLib::genrand_real1() );
	
	//		if( crossProb > m_SGAAttrib.CrossoverRate /*|| !m_pCrossover*/ )
	//		{
	//			//tcout << i << ": Ignoring Crossover..." << tendl;
	//			// とりあえず現行世代の遺伝子をそのまま残す
	//			T[0]->CopyGeneFrom( X[0] );
	//			T[1]->CopyGeneFrom( X[1] );
	
	//			continue;
	//		}
	//		
	//		m_refCrossover->Execute( X, T, nullptr );
	
	//		for( int j=0; j<X[0]->NumChromosomeTypes(); ++j )
	//		{
	//			//IChromosome* chromosomes[] =
	//			//{
	//			//	( (Chromosome2D*)p1 )->GetChromosome( j ),
	//			//	( (Chromosome2D*)p2 )->GetChromosome( j ),
	//			//	( (Chromosome2D*)c1 )->GetChromosome( j ),
	//			//	( (Chromosome2D*)c2 )->GetChromosome( j )
	//			//};
	
	//			//m_refCrossover->Execute( 4, chromosomes, nullptr );
	//			//m_refCrossover->Execute( 2, (const IChromosome**)&chromosomes[0], 2, &chromosomes[2], nullptr );
	//		}
	
	//	}// end of i loop
	
	//}




}// end of namespace