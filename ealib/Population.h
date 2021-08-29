#ifndef POPULATION_H
#define	POPULATION_H

#include	<oreore/common/DLLExport.h>

#include	"IChromosome.h"
#include	"Initializer.h"
#include	"Evaluator.h"
#include	"PopulationResult.h"



namespace ealib
{

	class CLASS_DECLSPEC Population
	{
	public:

		enum SORT_MODE
		{
			SORT_SCORE_ASCEND,
			SORT_SCORE_DESCEND,
			SORT_FITNESS_ASCEND,
			SORT_FITNESS_DESCEND,
		};


		Population();
		Population( const DesignParamArray& designParams, int pop_size, int num_objectives );
		Population( const IChromosome* pChromosome, int pop_size, int num_objectives );
		virtual ~Population();
		Population( const Population& obj );
		Population( Population&& obj );
		Population& operator=( const Population& obj );
		Population& operator=( Population&& obj );

		void Init( const DesignParamArray& designParams, int pop_size, int num_objectives );
		void Init( const IChromosome* pChromosome, int pop_size, int num_objectives );
		void Release();
		void CopyChromosomes( const Population* pSrc, int startidx=0 );
		Population* Clone() const;

		void Initialize( Initializer* pInit, Evaluator* pEval );
		void Clear( Evaluator* pEval );
		void Evaluate( Evaluator* pEval );// 評価

		void Sort( SORT_MODE mode=SORT_MODE::SORT_FITNESS_DESCEND );// ソート
		void Shuffle();	// 要素のランダム順序入れ替え
		
		int NumIndividuals() const				{ return m_Individuals.Length(); }		
		const OreOreLib::Array<IChromosome*>& ChromArray() const { return m_Individuals; }
		OreOreLib::Array<IChromosome*>& ChromArray()  { return m_Individuals; }
		IChromosome* Individual( int i) const	{ return m_Individuals[i]; }


		float GetScore( int i ) const			{ return m_Individuals[i]->GetScore(); }
		float GetScore( int i, int obj ) const	{ return m_Individuals[i]->GetScore( obj ); }

		float GetFitness( int i ) const			{ return m_Individuals[i]->GetFitness(); }
		float GetFitness( int i, int obj ) const{ return m_Individuals[i]->GetFitness( obj ); }

		int NumObjectives() const				{ return m_PopResult.NumObjectives(); }


	private:

		OreOreLib::Array<IChromosome*>	m_Individuals;
		PopulationResult				m_PopResult;

	};


}// end of namespace



#endif // !POPULATION_H
