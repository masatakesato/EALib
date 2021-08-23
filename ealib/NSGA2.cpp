#include	"NSGA2.h"

#include	<exception>
#include	<stdexcept>

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"Chromosome2D.h"


// http://web.ntnu.edu.tw/~tcchiang/publications/nsga3cpp/nsga3cpp.htm NSGA-3 implementation


// http://read.pudn.com/downloads192/sourcecode/math/901231/NSGA-II/non_domination_sort_mod.m__.htm

namespace ealib
{


	//##########################################################################################//
	//										NSGA2												//
	//##########################################################################################//

	NSGA2::NSGA2()
		: IEvolutionaryAlgorithm()
	{
		ClearAttribute();

		parentGen	= 0;
		childGen	= 1;
		dummy		= 2;
	}


	NSGA2::NSGA2( const NSGA2& obj )
		: IEvolutionaryAlgorithm(obj)
	{
		m_SGAAttrib	= obj.m_SGAAttrib;
		parentGen	= 0;
		childGen	= 1;
		dummy		= 2;

		m_Parents	= obj.m_Parents;
	}


	NSGA2::~NSGA2()
	{
		ReleasePopulation();
	}
	

	void NSGA2::SetAttribute( const SGAAttribute& attrib )
	{
		m_SGAAttrib	= attrib;
	}


	void NSGA2::SetCrossoverRate( float c_rate )
	{
		m_SGAAttrib.CrossoverRate = c_rate;
	}


	void NSGA2::SetMutationRate( float m_rate )
	{
		m_SGAAttrib.MutationRate = m_rate;
	}



	void NSGA2::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// 次世代個体の親を格納するバッファの確保
			m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );
			m_Parents.Init( DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 ) );

			// ダミーデータも初期化する
			m_Population[ dummy ].Init( designParams, 1, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}
		// Check visual studio's settings. https://kagasu.hatenablog.com/entry/2017/05/04/223252
		// try/catch/finally example https://ufcpp.net/study/csharp/oo_exception.html
		// https://stackoverflow.com/questions/3641737/c-get-description-of-an-exception-caught-in-catch-block
	}



	// Deprecated. 2021.08.23
	//void NSGA2::InitPopulation( const IChromosome* pChromosome, int numObjectives )
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
	//	// Check visual studio's settings. https://kagasu.hatenablog.com/entry/2017/05/04/223252
	//	// try/catch/finally example https://ufcpp.net/study/csharp/oo_exception.html
	//	// https://stackoverflow.com/questions/3641737/c-get-description-of-an-exception-caught-in-catch-block
	//}



	void NSGA2::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ dummy ].Release();

		m_Parents.Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* NSGA2::Clone() const
	{
		return new NSGA2( *this );
	}



	void NSGA2::Step( Evaluator* pEval )
	{
		Select( &m_Population[parentGen] );
		Crossover( &m_Population[parentGen], &m_Population[childGen] );
		Mutate( &m_Population[childGen] );
		CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Evaluate( pEval );

		m_Stats.Update( m_Population[parentGen] );
	}



	void NSGA2::Evolve( Evaluator* pEval, unsigned int seed )
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



	void NSGA2::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}



	// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	void NSGA2::Select( Population* pPopulation )
	{
		// 適応値と親個体の初期化
		if( !m_refSelector )
			return;
		
		m_refSelector->BindPopulationData( pPopulation->PopulationSize(), pPopulation->ChromosomeArray() );
		m_refSelector->Update();

		// 親の選択
		for( int i=0; i<m_Parents.Length(); ++i )
		{
			int parent1	= m_refSelector->Execute();
			int parent2	= m_refSelector->Execute();

			while( parent1==parent2 )
				parent2 = m_refSelector->Execute();
				
			m_Parents[i].first = parent1;
			m_Parents[i].second = parent2;
		}// end of i loop
	}



	// 交叉処理を行う
	void NSGA2::Crossover( Population* pParentPopulation, Population* pChildPopulation )
	{
		for( int i=0; i<m_Parents.Length(); ++i )
		{
			IChromosome* p1	= pParentPopulation->GetIndividual( m_Parents[i].first );
			IChromosome* p2	= pParentPopulation->GetIndividual( m_Parents[i].second );
			IChromosome* c1	= pChildPopulation->GetIndividual( i*2 );
			IChromosome* c2	= ( i*2+1 )<( m_Attrib.PopulationSize-m_Attrib.EliteSize ) ? pChildPopulation->GetIndividual( i*2+1 ) : m_Population[ dummy ].GetIndividual(0);

			float crossProb	= float( OreOreLib::genrand_real1() );
			
			if( crossProb > m_SGAAttrib.CrossoverRate )
			{
				//tcout << i << ": Ignoring Crossover..." << tendl;
				// とりあえず現行世代の遺伝子をそのまま残す
				c1->CopyGeneFrom( p1 );
				c2->CopyGeneFrom( p2 );

				continue;
			}
			
			IChromosome* chromosomes[] ={ p1, p2, c1, c2 };
			m_refCrossover->Execute( 4, chromosomes, nullptr );

		}// end of i loop

	}



	// 突然変異
	void NSGA2::Mutate( Population* pPopulation )
	{
		for( int i=0; i<m_Attrib.PopulationSize-m_Attrib.EliteSize; ++i )
			m_refMutator->Execute( pPopulation->GetIndividual(i), m_SGAAttrib.MutationRate );

	}

	

	void NSGA2::CarryOver( Population* pParentPopulation, Population* pChildPopulation )
	{
		IChromosome *pParent, *pChild;

		int dst_start	= m_Attrib.PopulationSize - m_Attrib.EliteSize;
		for( int i=0; i<m_Attrib.EliteSize; ++i )
		{
			pParent	= pParentPopulation->GetIndividual(i);
			pChild	= pChildPopulation->GetIndividual( dst_start+i );

			pChild->CopyGeneFrom( pParent );
		}
	}



	void NSGA2::ClearAttribute()
	{
		m_Attrib.Clear();
		m_SGAAttrib.Clear();
	}



	static void Fast_Nondominated_sort( Population* p )
	{



	}



	void NSGA2::NonDominatedSorting( Population* P )
	{
		

	}
	


	void NSGA2::CrowdingDistanceAssignment()
	{
	}


	
	//##########################################################################################//
	//										MixedNSGA2											//
	//##########################################################################################//


	MixedNSGA2::MixedNSGA2()
		: IEvolutionaryAlgorithm()
	{
		ClearAttribute();

		parentGen			= 0;
		childGen			= 1;
		dummy				= 2;
	}



	MixedNSGA2::MixedNSGA2( const MixedNSGA2& obj )
		: IEvolutionaryAlgorithm( obj )
	{
		m_SGAAttrib			= obj.m_SGAAttrib;
		parentGen			= 0;
		childGen			= 1;
		dummy				= 2;

		m_Parents			= obj.m_Parents;
	}



	MixedNSGA2::~MixedNSGA2()
	{
		ReleasePopulation();
	}



	void MixedNSGA2::SetAttribute( const SGAAttribute& attrib )
	{
		m_SGAAttrib	= attrib;
	}



	void MixedNSGA2::SetCrossoverRate( float c_rate )
	{
		m_SGAAttrib.CrossoverRate = c_rate;
	}



	void MixedNSGA2::SetMutationRate( float m_rate )
	{
		m_SGAAttrib.MutationRate = m_rate;
	}



	void MixedNSGA2::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			m_Population[ parentGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ childGen ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// 次世代個体の親を格納するバッファの確保
			m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );
			m_Parents.Init( DivUp( Max( m_Attrib.PopulationSize-m_Attrib.EliteSize, 0 ), 2 ) );

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
	//void MixedNSGA2::InitPopulation( const IChromosome* pChromosome, int numObjectives )
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



	void MixedNSGA2::ReleasePopulation()
	{
		m_Population[ parentGen ].Release();
		m_Population[ childGen ].Release();
		m_Population[ dummy ].Release();

		m_Parents.Release();

		m_bReady = false;
	}


	IEvolutionaryAlgorithm* MixedNSGA2::Clone() const
	{
		return new MixedNSGA2( *this );
	}



	void MixedNSGA2::Step( Evaluator* pEval )
	{
		Select( &m_Population[parentGen] );
		Crossover( &m_Population[parentGen], &m_Population[childGen] );
		Mutate( &m_Population[childGen] );
		CarryOver( &m_Population[parentGen], &m_Population[childGen] );

		parentGen	^= 1;
		childGen	^= 1;

		m_Population[parentGen].Evaluate( pEval );

		m_Stats.Update( m_Population[parentGen] );
	}



	void MixedNSGA2::Evolve( Evaluator* pEval, unsigned int seed )
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



	void MixedNSGA2::TakeSnapshot( Population& pOut ) const
	{
		pOut = Population( m_Population[parentGen] );
	}



	// 優れた個体を選別する→いくつか方法がある, 適応度比例戦略、エリート保存戦略、トーナメント戦略
	void MixedNSGA2::Select( Population* pPopulation )
	{
		// 適応値と親個体の初期化
		if( !m_refSelector )
			return;
		
		m_refSelector->BindPopulationData( pPopulation->PopulationSize(), pPopulation->ChromosomeArray() );
		m_refSelector->Update();

		// 親の選択
		for( int i=0; i<m_Parents.Length(); ++i )
		{
			int parent1	= m_refSelector->Execute();
			int parent2	= m_refSelector->Execute();

			while( parent1==parent2 )
				parent2 = m_refSelector->Execute();

			m_Parents[i].first = parent1;
			m_Parents[i].second = parent2;
		}// end of i loop
	}



	// 交叉処理を行う
	void MixedNSGA2::Crossover( Population* pParentPopulation, Population* pChildPopulation )
	{
		int numChromTypes = pParentPopulation->GetIndividual(0)->NumChromTypes();

		for( int i=0; i<m_Parents.Length(); ++i )
		{
			IChromosome* p1	= pParentPopulation->GetIndividual( m_Parents[i].first );
			IChromosome* p2	= pParentPopulation->GetIndividual( m_Parents[i].second );
			IChromosome* c1	= pChildPopulation->GetIndividual( i*2 );
			IChromosome* c2	= ( i*2+1 )<( m_Attrib.PopulationSize-m_Attrib.EliteSize ) ? pChildPopulation->GetIndividual( i*2+1 ) : m_Population[ dummy ].GetIndividual(0);

			float crossProb	= float( OreOreLib::genrand_real1() );

			if( crossProb > m_SGAAttrib.CrossoverRate )
			{
				//tcout << i << ": Ignoring Crossover..." << tendl;
				// とりあえず現行世代の遺伝子をそのまま残す
				c1->CopyGeneFrom( p1 );
				c2->CopyGeneFrom( p2 );

				continue;
			}
			
			for( int j=0; j<numChromTypes; ++j )
			{
				IChromosome* chromosomes[] =
				{
					( (Chromosome2D*)p1 )->GetChromosome( j ),
					( (Chromosome2D*)p2 )->GetChromosome( j ),
					( (Chromosome2D*)c1 )->GetChromosome( j ),
					( (Chromosome2D*)c2 )->GetChromosome( j )
				};

				m_refCrossover->Execute( 4, chromosomes, nullptr );
			}

		}// end of i loop

	}



	// 突然変異
	void MixedNSGA2::Mutate( Population* pPopulation )
	{
		for( int i=0; i<m_Attrib.PopulationSize-m_Attrib.EliteSize; ++i )
		{
			Chromosome2D* pChrom = (Chromosome2D*)( pPopulation->GetIndividual(i) );

			for( int j=0; j<pChrom->NumChromTypes(); ++j )
				m_refMutator->Execute( pChrom->GetChromosome(j), m_SGAAttrib.MutationRate );

		}// end of i loop
	}



	void MixedNSGA2::CarryOver( Population* pParentPopulation, Population* pChildPopulation )
	{
		IChromosome *pParent, *pChild;

		int dst_start	= m_Attrib.PopulationSize - m_Attrib.EliteSize;
		for( int i=0; i<m_Attrib.EliteSize; ++i )
		{
			pParent	= pParentPopulation->GetIndividual( i );
			pChild	= pChildPopulation->GetIndividual( dst_start+i );

			pChild->CopyGeneFrom( pParent );
		}
	}



	void MixedNSGA2::ClearAttribute()
	{
		m_Attrib.Clear();
		m_SGAAttrib.Clear();
	}




}// end of namespace