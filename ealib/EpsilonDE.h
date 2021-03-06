#ifndef	EPSILON_DE_H
#define	EPSILON_DE_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"DE_Rand_1.h"



namespace ealib
{

	class CLASS_DECLSPEC EpsilonDE : public IEvolutionaryAlgorithm
	{
	public:

		EpsilonDE();
		EpsilonDE( const EpsilonDE& obj );
		~EpsilonDE();

		// Set attribute
		void SetAttribute( const DEAttribute& attrib );
		void SetDifferentialWeight( float f );		// 突然変異率を設定
		void SetCrossoverProbability( float cr );	// 交叉率の設定

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

		DEAttribute	m_MutateAttrib;
		Population	m_Population[3];
		int			parentGen, childGen, dummy;

		DE_Rand_1	m_DE_Rand_1_Mutator;


		void ClearAttribute();


		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindSelector;// ( ISelector* selector );
		using IEvolutionaryAlgorithm::UnbindSelector;
		void BindSelector( ISelector* selector ){}
		void UnbindSelector(){}

	};


}// end of namespace



#endif	// EPSILON_DE_H //