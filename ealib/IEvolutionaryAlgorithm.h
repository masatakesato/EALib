#ifndef	I_EVOLUTIONARY_ALGORITHM_H
#define	I_EVOLUTIONARY_ALGORITHM_H

#include	"EACommon.h"
#include	"IChromosome.h"
#include	"Population.h"
#include	"EAStatistics.h"

#include	"Initializer.h"
#include	"Mutator.h"
#include	"Crossover.h"
#include	"ISelector.h"
#include	"ITerminationCriteria.h"



namespace ealib
{
	class ISelector;


	class CLASS_DECLSPEC IEvolutionaryAlgorithm
	{
	public:

		IEvolutionaryAlgorithm();
		IEvolutionaryAlgorithm( const IEvolutionaryAlgorithm& obj );
		virtual ~IEvolutionaryAlgorithm();

		// bind/unbind initializer
		void BindInitializer( Initializer* initializer );
		void UnbindInitializer();

		// bind/unbind mutator
		void BindMutator( Mutator* mutator );
		void UnbindMutator();

		// bind/unbind crossover
		void BindCrossover( Crossover* crossover );
		void UnbindCrossover();

		// bind/unbind selector
		void BindSelector( ISelector* selector );
		void UnbindSelector();

		// bind/unbind termination criteria
		void BindTerminationCriteria( ITerminationCriteria* crossover );
		void UnbindTerminationCriteria();


		void SetAttribute( const EACommonAttribute& attrib );
		void SetPopulationSize( int pop_size );	// 世代数を設定
		void SetNumGenerations( int gen );		// 1世代で生成する個体数を設定
		void SetEliteSize( int elite_size );	// エリート個体の数を設定


		EACommonAttribute GetAttribute() const	{ return m_Attrib; }
		int NumGenerations() const				{ return m_Attrib.NumGenerations; }
		int PopulationSize() const				{ return m_Attrib.PopulationSize; }
		int EliteSize() const					{ return m_Attrib.EliteSize; }
		EAStatistics* const Statistics() const	{ return (EAStatistics* const)&m_Stats; }

		// Pure Virtual Functions
		virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives )=0;
		virtual void ReleasePopulation()=0;
		virtual IEvolutionaryAlgorithm* Clone() const=0;
		virtual void Step( Evaluator* pEval )=0;
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 )=0;
		virtual Population* GetPopulation() const=0;
		virtual void TakeSnapshot( Population& pOut ) const=0;
		

// TODO: Impelement. 2021.08.23
		virtual void InitPopulation( const DesignParamArray&, int )=0;


void SetConvergenceCheckInterval( int interval ){ m_Stats.InitConvergenceChecker( interval ); }
void SetLogSize( int numlogs )					{ m_Stats.InitLogBuffer( numlogs ); }


	protected:

		EACommonAttribute		m_Attrib;
		EAStatistics			m_Stats;

		Initializer*			m_refInitializer;
		Mutator*				m_refMutator;
		Crossover*				m_refCrossover;
		ISelector*				m_refSelector;
		ITerminationCriteria*	m_refCriteria;

		bool					m_bReady;
	};


}// end of namespace



#endif	// I_EVOLUTIONARY_ALGORITHM_H //