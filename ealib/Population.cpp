#include	"Population.h"

#include	<oreore/common/Utility.h>
#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

#include	"ChromosomeFactory.h"



namespace ealib
{

	typedef int( *CompareFunc )( const IChromosome*, const IChromosome* );
	
	//typedef int( *CompareFunc )(const void*, const void*);
	//
	//static int CompareScoreAscend( const void* pElem1, const void* pElem2 )
	//{
	//	float fa = ((const IChromosome *)pElem1)->Score();
	//	float fb = ((const IChromosome *)pElem2)->Score();
	//	return (fa > fb) - (fa < fb);// ascend
	//}


	//static int CompareScoreDescend( const void* pElem1, const void* pElem2 )
	//{
	//	float fa = ((const IChromosome *)pElem1)->Score();
	//	float fb = ((const IChromosome *)pElem2)->Score();
	//	return (fa < fb) - (fa > fb);// descend
	//}


	//static int CompareFitnessAscend( const void* pElem1, const void* pElem2 )
	//{
	//	float fa = ((const IChromosome *)pElem1)->Fitness();
	//	float fb = ((const IChromosome *)pElem2)->Fitness();
	//	return (fa > fb) - (fa < fb);// ascend
	//}


	//static int CompareFitnessDescend( const void* pElem1, const void* pElem2 )
	//{
	//	float fa = ((const IChromosome *)pElem1)->Fitness();
	//	float fb = ((const IChromosome *)pElem2)->Fitness();
	//	return (fa < fb) - (fa > fb);// descend
	//}
	


	const CompareFunc c_CompareFuncs[]	=
	{
		CompareScoreAscend,
		CompareScoreDescend,
		CompareFitnessAscend,
		CompareFitnessDescend
	};



	static void QuickSort( IChromosome *array[], int begin, int end, int( *fpCompare )( const IChromosome* pElem1, const IChromosome* pElem2) )
	{
		int i = begin;
		int j = end;
		IChromosome* pivot;
		IChromosome* temp;

		pivot = array[(begin + end) / 2];  // 中央の値をpivotにする
		
		while( 1 )
		{
			while( /*array[i]->Fitness() < pivot*/fpCompare( array[i], pivot ) < 0 ){ ++i; }  /* 枢軸以上の値が見つかるまで右方向へ進めていく */
			while( /*array[j]->Fitness() > pivot*/fpCompare( array[j], pivot ) > 0 ){ --j; }  /* 枢軸以下の値が見つかるまで左方向へ進めていく */
			if( i >= j )break;  // 軸がぶつかったらソート終了

			// 入れ替え
			temp = array[i];
			array[i] = array[j];
			array[j] = temp;
			i++;
			j--;
		}

		// 軸の左側をソートする
		if( begin < i - 1 ){ QuickSort( array, begin, i - 1, fpCompare ); }
		// 軸の右側をソートする
		if( j + 1 < end ){ QuickSort( array, j + 1, end, fpCompare ); }
	}




	// Default constructor
	Population::Population()
	{

	}



	// Constructor
	Population::Population( const DesignParamArray& designParams, int pop_size, int num_objectives )
	{
		Init( designParams, pop_size, num_objectives );
	}



	// Constructor
	//Population::Population( const IChromosome* pChromosome, int pop_size, int num_objectives )
	//{
	//	Init( pChromosome, pop_size, num_objectives );
	//}



	// Destructor
	Population::~Population()
	{
		Release();
	}



	// Copy constructor
	Population::Population( const Population& obj )
		: m_PopResult( obj.NumIndividuals(), obj.NumObjectives() )
		, m_Individuals( obj.m_Individuals )
	{

		for( int32 i=0; i<m_Individuals.Length<int32>(); ++i )
		{
			m_Individuals[i] = obj.m_Individuals[i]->Clone();
			m_Individuals[i]->BindEvalResultView( &m_PopResult[i] );
			m_PopResult[i].CopyFrom( obj.m_Individuals[i]->EvalResult() );
		}
	}



	// Move constructor
	Population::Population( Population&& obj )
		: m_PopResult( (PopulationResult&&)obj.m_PopResult )
		, m_Individuals( (OreOreLib::Array<IChromosome*>&&)obj.m_Individuals )
	{

	}



	// Copy assignment operator
	Population& Population::operator=( const Population& obj )
	{
		if( this != &obj )
		{
			Release();

			m_PopResult.Init( obj.NumIndividuals(), obj.NumObjectives() );
			m_Individuals.Init( obj.m_Individuals.Length() );

			for( int32 i=0; i<m_Individuals.Length<int32>(); ++i )
			{
				m_Individuals[i] = obj.m_Individuals[i]->Clone();
				m_Individuals[i]->BindEvalResultView( &m_PopResult[i] );
				m_PopResult[i].CopyFrom( obj.m_Individuals[i]->EvalResult() );
			}
		}

		return *this;
	}



	// Move assignment operator
	Population& Population::operator=( Population&& obj )
	{
		if( this != &obj )
		{
			Release();

			m_PopResult		= (PopulationResult&&)obj.m_PopResult;
			m_Individuals	= (OreOreLib::Array<IChromosome*>&&)obj.m_Individuals;
		}

		return *this;
	}



	void Population::Init( const DesignParamArray& designParams, int pop_size, int num_objectives )
	{
		ChromosomeFactory<g_ChoromosomeTypes> factory;

		Release();
				
		m_PopResult.Init( pop_size, num_objectives );

		//===============	バッファを確保する	=================//
		// 1世代分(入力用と出力用の2つ)のChromosome(設計変数)群の配列を確保する.
		m_Individuals.Init( pop_size );

		for( int32 i=0; i<m_Individuals.Length<int32>(); ++i )
		{
			m_Individuals[i] = factory.Create( designParams );
			m_Individuals[i]->SetID( i );
			m_Individuals[i]->BindEvalResultView( &m_PopResult[i] );
		}
	}



	//void Population::Init( const IChromosome* pChromosome, int pop_size, int num_objectives )
	//{
	//	Release();
	//			
	//	m_PopResult.Init( pop_size, num_objectives );

	//	//===============	バッファを確保する	=================//
	//	// 1世代分(入力用と出力用の2つ)のChromosome(設計変数)群の配列を確保する.
	//	m_Individuals.Init( pop_size );

	//	for( int i=0; i<m_Individuals.Length(); ++i )
	//	{
	//		m_Individuals[i] = pChromosome->Clone();
	//		m_Individuals[i]->SetID( i );
	//		m_Individuals[i]->BindEvalResultView( &m_PopResult[i] );
	//	}
	//}



	void Population::Release()
	{
		// Release Chromosomes
		for( int32 i=0; i<m_Individuals.Length<int32>(); ++i )
			SafeDelete( m_Individuals[i] );
		m_Individuals.Release();

		// Release results
		m_PopResult.Release();
	}



	void Population::CopyChromosomes( const Population* pSrc, int startidx )
	{
		int32 length = static_cast<int32>( Min( pSrc->m_Individuals.Length(), m_Individuals.Length()-startidx ) );
		for( int32 i=0; i<length; ++i )
			m_Individuals[ i + startidx ]->CopyGeneFrom( pSrc->m_Individuals[ i ] );

	}



	Population* Population::Clone() const
	{
		return new Population( *this );
	}



	void Population::Initialize( Initializer* pInit, Evaluator* pEval )
	{
		for( auto pindividual : m_Individuals )
			pindividual->Initialize( pInit );

		Evaluate( pEval );
	}



	void Population::Clear( Evaluator* pEval )
	{
		for( auto pindividual : m_Individuals )
			pindividual->ClearGene();

		Evaluate( pEval );
	}



	void Population::Evaluate( Evaluator* pEval )
	{
		for( auto pindividual : m_Individuals )
			pEval->Evaluate( pindividual );

		Sort( SORT_MODE::SORT_FITNESS_DESCEND );// 適応度が高い順にソートする
	}


	void Population::Sort( SORT_MODE mode )
	{
		QuickSort( m_Individuals.begin(), 0, m_Individuals.Length()-1, c_CompareFuncs[mode] );
	}


	void Population::Shuffle()
	{
		for( int32 i=0; i<m_Individuals.Length<int32>(); ++i )
		{
			int32 j = int32( OreOreLib::genrand_real2() * m_Individuals.Length() );
			IChromosome* temp	= m_Individuals[i];
			m_Individuals[i]	= m_Individuals[j];
			m_Individuals[j]	= temp;
		}// end of i loop	

	}


}// end of namespace