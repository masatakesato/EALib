// Implementation of Composite Differential Evolution Algorithm [A.Hammoudi 2015]
// https://www.semanticscholar.org/paper/Composite-Differential-Evolution-Algorithm-Mixed-V-Hammoudi-Djeddou/76a01217320462b99ab6cefaaa5d8f549c645aa8


#ifndef	COMPOSITE_DIFFERENTIAL_EVOLUTION_H
#define	COMPOSITE_DIFFERENTIAL_EVOLUTION_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"Chromosome2D.h"

#include	"DE_Best_1.h"
#include	"DE_Rand_1.h"
#include	"DE_Rand_2.h"
#include	"DE_Current_to_Rand_1.h"



namespace ealib
{
	enum DE_MUTATION
	{
		DE_MUTATION_UNKNOWN = -1,
		DE_RAND_1,
		DE_BEST_1,
		DE_RAND_2,
		DE_BEST_2,
		DE_CURRENT_TO_BEST_1,
		DE_RAND_TO_BEST_1,
		DE_CURRENT_TO_RAND_1,
		NUM_DE_MUTATION,
	};




	class CLASS_DECLSPEC CoDE: public IEvolutionaryAlgorithm
	{
	public:

		CoDE();
		CoDE( const CoDE& obj );
		~CoDE();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const DEAttribute& attrib, int i );
		void SetDifferentialWeight( float f, int i );		// 突然変異率を設定
		void SetCrossoverProbability( float cr, int i );	// 交叉率の設定
		
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

		DEAttribute	m_MutationAttrib[3];
		Population	m_Population[3];
		int			parentGen, childGen, trialGen;

		DE_Best_1				m_DE_Best_1;
		DE_Rand_1				m_DE_Rand_1;
		DE_Rand_2				m_DE_Rand_2;
		DE_Current_to_Rand_1	m_DE_Current_to_Rand_1;


		// Disable ClearAttribute
		void ClearAttribute();

		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindSelector;
		using IEvolutionaryAlgorithm::UnbindSelector;
		void BindSelector( ISelector* selector ){}
		void UnbindSelector(){}

		// Disable Mutator binding
		using IEvolutionaryAlgorithm::BindMutator;
		using IEvolutionaryAlgorithm::UnbindMutator;
		virtual void BindMutator( Mutator* mutator ){}
		virtual void UnbindMutator(){}

	};





	class CLASS_DECLSPEC MixedCoDE : public IEvolutionaryAlgorithm
	{
	public:

		MixedCoDE();
		MixedCoDE( const MixedCoDE& obj );
		~MixedCoDE();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const DEAttribute& attrib, int i );
		void SetDifferentialWeight( float f, int i );		// 突然変異率を設定
		void SetCrossoverProbability( float cr, int i );	// 交叉率の設定

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
		
		DEAttribute	m_MutationAttrib[3];
		Population	m_Population[3];
		int			parentGen, childGen, trialGen;

		DE_Best_1				m_DE_Best_1;
		DE_Rand_1				m_DE_Rand_1;
		DE_Rand_2				m_DE_Rand_2;
		DE_Current_to_Rand_1	m_DE_Current_to_Rand_1;


		// Disable ClearAttribute
		void ClearAttribute();

		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindSelector;
		using IEvolutionaryAlgorithm::UnbindSelector;
		void BindSelector( ISelector* selector ){}
		void UnbindSelector(){}

		// Disable Mutator binding
		using IEvolutionaryAlgorithm::BindMutator;
		using IEvolutionaryAlgorithm::UnbindMutator;
		virtual void BindMutator( Mutator* mutator ){}
		virtual void UnbindMutator(){}

	};


}// end of namespace



#endif	// COMPOSITE_DIFFERENTIAL_EVOLUTION_H //