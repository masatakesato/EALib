#ifndef	NSGA2_H
#define	NSGA2_H


#include	"EACommon.h"
#include	"IEvolutionaryAlgorithm.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/container/Pair.h>


#include	<unordered_map>
#include	<algorithm>
#include	<numeric>
#include	<limits>



namespace ealib
{

	class CLASS_DECLSPEC NSGA2 : public IEvolutionaryAlgorithm
	{
		using Parents = Pair<int, int>;

	public:

		NSGA2();
		NSGA2( const NSGA2& obj );
		~NSGA2();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const SGAAttribute& attrib );
		void SetCrossoverRate( float c_rate );	// 交叉率の設定
		void SetMutationRate( float m_rate );	// 突然変異率を設定

		// Virtual Functions Override
		virtual bool InitPopulation( const DesignParamArray& designParams, int numObjectives );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population& GetPopulation() { return m_Population[ parentGen ]; }
		virtual const Population& GetPopulation() const { return m_Population[ parentGen ]; }
		virtual void TakeSnapshot( Population& pOut ) const;

		void NonDominatedSorting( Population *P );
		void CrowdingDistanceAssignment();


	private:

		SGAAttribute	m_SGAAttrib;

		Population		m_Population[3];
		int				parentGen, childGen, dummy;

		OreOreLib::Array<Parents>	m_Parents;

		
		void Select( Population* pPopulation );	// 選択
		void Crossover( Population* pParentPopulation, Population* pChildPopulation );// 交叉
		void Mutate( Population* pPopulation );	// 突然変異
		void CarryOver( Population* pParentPopulation, Population* pChildPopulation );// エリート保存

		void ClearAttribute();
	};




	class CLASS_DECLSPEC MixedNSGA2: public IEvolutionaryAlgorithm
	{
		using Parents = Pair<int, int>;

	public:

		MixedNSGA2();
		MixedNSGA2( const MixedNSGA2& obj );
		~MixedNSGA2();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const SGAAttribute& attrib );
		void SetCrossoverRate( float c_rate );	// 交叉率の設定
		void SetMutationRate( float m_rate );	// 突然変異率を設定

		// Virtual Functions Override
		virtual bool InitPopulation( const DesignParamArray& designParams, int numObjectives );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population& GetPopulation() { return m_Population[ parentGen ]; }
		virtual const Population& GetPopulation() const { return m_Population[ parentGen ]; }
		virtual void TakeSnapshot( Population& pOut ) const;


	private:

		SGAAttribute	m_SGAAttrib;

		Population		m_Population[3];// [0]:初期探索集団Q, [1]:アーカイブ母集団
		int				parentGen, childGen, dummy;

		OreOreLib::Array<Parents>	m_Parents;


		void Select( Population* pPopulation );	// 選択
		void Crossover( Population* pParentPopulation, Population* pChildPopulation );// 交叉
		void Mutate( Population* pPopulation );	// 突然変異
		void CarryOver( Population* pParentPopulation, Population* pChildPopulation );// エリート保存

		void ClearAttribute();

	};

	

	template< typename order_arr, typename value_arr >
	void reorder( order_arr index_arr, size_t arr_size, value_arr struct_arr )
	{
		// sort them
		for( int i=0; i<arr_size; ++i )
		{
			int newIndex = index_arr[i];
			// take out the item at the specified index
			auto newItem = struct_arr[newIndex];
			// take out the item in that current position
			auto oldItem = struct_arr[i];
			// swap the two items
			struct_arr[i] = newItem;
			struct_arr[newIndex] = oldItem;
			// now, tell the sorted indices the location of the old item after swap
			for( int j=i; j<arr_size; ++j )
			{
				if( index_arr[j]==i )
				{
					index_arr[j] = newIndex;
					break; // we only need the first one, so then we're done
				}
			}// rnd of j loop
		}// end of i loop
	}


	static std::unordered_map< int, std::vector<int> > fast_nondominated_sort_index_ver( Population *P )
	{
		std::unordered_map< int, std::vector<int> > F;// Fronts. F[front][element].
		std::unordered_map< int, std::vector<int> > D( P->NumIndividuals() );// Individuals that P[i] dominates.
		std::vector<int>				n( P->NumIndividuals() );// =[0] * len( P )// Num of individuals that dominates P[i].
		
		for( int i=0; i<P->NumIndividuals(); ++i )// for i in range( len( P ) ):
		{
			n[i] = 0;// number of solutions which dominate the ai.
			//D[i] =[]// set of solutions which ai dominates.

			const IChromosome* ai = P->Individual( i );

			for( int j=0; j<P->NumIndividuals(); ++j )//for j in range( len( P ) ):
			{
				const IChromosome* aj = P->Individual( j );
				if( ai->Dominates( aj ) ) // ajが劣解の場合はSpに追加する
					D[i].push_back( j );
				else if( aj->Dominates( ai ) )//# aiが劣解の場合はランクを上げる
					n[i] = n[i] + 1;
			}// end of j loop

			if( n[i]==0 )
				F[1].push_back( i ); //#F[1].append( ai )
		}// end of i loop

		int r=1;
		while( F.find(r) != F.end() )
		{
			std::vector<int> H;
			for( const auto i: F[r] )// Get individual i from Front r.
			{
				for( const auto j: D[i] )// P[i]が支配する劣解のリストを取得したい.
				{
					n[j]--;// n[j] = n[j] - 1;
					if( n[j]==0 ) H.push_back( j );
				}
			}
				
			r++;// r = r + 1
			if( !H.empty() ) F[r] = H;//if( H ): F[r] = H
		}

		return F;
	}

	
	// P: Population, front: single front
	static void crowding_distance_population( Population* P, Evaluator* pEval, std::vector<int> front )//
	{
		const int N = P->NumIndividuals();
		const int M = pEval->NumObjectives();
		std::vector<float>	Fdist(N);// = numpy.zeros( N, dtype='float32' )# TODO: Must be reordered along with F.Should be attached to F. 2018.08.31

		//=============== Initialize distance with zero ==============//
		std::fill( Fdist.begin(), Fdist.end(), 0.0f );

		//=============== Calculate crowding distance ================//
		std::vector<int> order_obj_m = front;// ( N );// TODO: Preallocate as NSGA2 member variable. 2018.09.21
		//std::iota( order_obj_m.begin(), order_obj_m.end(), 0 );

		for( int m=0; m<M; ++m )
		{
			// Sort F using objective m
			std::sort( order_obj_m.begin(), order_obj_m.end(), [&]( int x, int y ) { return P->Fitness( x, m ) < P->Fitness( y, m ); } );// return CompareFitnessAscend( F[x], F[y] );
			
			// Get fitness range of m's objective.
			auto fmax_m = P->Fitness( order_obj_m[N-1], m );
			auto fmin_m = P->Fitness( order_obj_m[0], m );
			auto f_range_m = Max( fmax_m - fmin_m, 1.0e-5f );

			// Set extreme solution's distance to infinity.(to guarantee that they will be selected in the next generation)
			Fdist[order_obj_m[0]] = Fdist[order_obj_m[N-1]] = std::numeric_limits<float>::infinity();

			// Accumulate distance to neighbors.
			for( int i=1; i<N-1; ++i )
			{
				auto il = order_obj_m[i-1];
				auto ir = order_obj_m[i+1];
				auto ic = order_obj_m[i];
				Fdist[ic] += ( P->Fitness( ir, m ) - P->Fitness( il, m ) ) / f_range_m;
			}
		}
		
		//=========== Reorder P's individuals using crowding distance ============//
		// Generate reorder indices
		std::vector<int> sorted_idx( N );// TODO: Preallocate as NSGA2 member variable. 2018.09.21
		std::iota( sorted_idx.begin(), sorted_idx.end(), 0 );
		std::sort( sorted_idx.begin(), sorted_idx.end(), [&]( int x, int y ) { return Fdist[x] > Fdist[y]; } );
		
		// For debug purpose
		//reorder< std::vector<int>, std::vector<float>& >( sorted_idx, sorted_idx.size(), Fdist );

		// Reorder F's individuals using sorted_idx
		reorder< std::vector<int>, IChromosome** >( sorted_idx, sorted_idx.size(), P->Indivuduals().begin() );
		
#ifdef _DEBUG
		tcout << "crowding_distance_population...result" << tendl;
		for( int i=0; i<N; ++i )
		{
			tcout << "( ";
			for( int j=0; j<M; ++j )
				tcout << P->Fitness( i, j ) << ",";

			tcout << ")\n";
		}
#endif

	}






	// TODO: Implement CrowdedTorunamentSelector
	// TODO: ランクが高い方を選ぶ. 同一ランクの場合は混雑距離が大きい方を選ぶ.
	// TODO: ということは、、、全個体について混雑距離の計算が必要


}// end of namespace


#endif	// NSGA2_H //