#ifndef POPULATION_H
#define	POPULATION_H

#include	"DLLExport.h"
#include	"IChromosome.h"
#include	"ISelector.h"
#include	"Evaluator.h"
#include	"Initializer.h"
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
		Population( const IChromosome* pChromosone, int pop_size, int num_objectives );
		virtual ~Population();
		Population( const Population& obj );
		Population( Population&& obj );
		Population& operator=( const Population& obj );
		Population& operator=( Population&& obj );

		
		void Init( const IChromosome* pChromosone, int pop_size, int num_objectives );
		void Release();
		void CopyChromosomes( const Population* pSrc, int startidx=0 );
		Population* Clone() const;

		void BindSelector( ISelector* selector );
		void UnbindSelector();

		void Initialize( Initializer* pInit, Evaluator* pEval );
		void Clear( Evaluator* pEval );
		void Evaluate( Evaluator* pEval );// 評価

		void UpdateSelector();
		int Select();
		void Sort( SORT_MODE mode=SORT_MODE::SORT_FITNESS_DESCEND );// ソート
		void Shuffle();	// 要素のランダム順序入れ替え
		

		int PopulationSize() const				{ return m_ChromosomeArray.Length(); }
		IChromosome** ChromosomeArray() const	{ return (IChromosome**)m_ChromosomeArray.begin(); }
		IChromosome* GetIndividual(int i) const	{ return m_ChromosomeArray[i]; }
		
		float GetScore( int i ) const			{ return m_ChromosomeArray[i]->GetScore(); }
		float GetScore( int i, int obj ) const	{ return m_ChromosomeArray[i]->GetScore( obj ); }

		float GetFitness( int i ) const			{ return m_ChromosomeArray[i]->GetFitness(); }
		float GetFitness( int i, int obj ) const{ return m_ChromosomeArray[i]->GetFitness( obj ); }

		int NumObjectives() const				{ return m_PopResult.NumObjectives(); }


// TODO: Create Population using DesignParamArray.
//Population( const DesignParamArray& designParams, int pop_size, int num_objectives );
//void Init( const DesignParamArray& designParams, int pop_size, int num_objectives );


	private:

		OreOreLib::Array<IChromosome*>	m_ChromosomeArray;
		PopulationResult				m_PopResult;
		ISelector*						m_refSelector;

	};


}// end of namespace



#endif // !POPULATION_H
