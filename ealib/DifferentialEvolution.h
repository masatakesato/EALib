#ifndef	DIFFERENTIAL_EVOLUTION_H
#define	DIFFERENTIAL_EVOLUTION_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"DE_Rand_1.h"



namespace ealib
{

	class CLASS_DECLSPEC DifferentialEvolution : public IEvolutionaryAlgorithm
	{
	public:

		DifferentialEvolution();
		DifferentialEvolution( const DifferentialEvolution& obj );
		~DifferentialEvolution();

		// Set attribute
		void SetAttribute( const DEAttribute& attrib );
		void SetDifferentialWeight( float f );		// 突然変異率を設定
		void SetCrossoverProbability( float cr );	// 交叉率の設定

		// Virtual Functions Override
		virtual bool InitPopulation( const DesignParamArray&, int );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[parentGen]; }
		virtual void TakeSnapshot( Population& pOut ) const;


	private:

		DEAttribute	m_MutateAttrib;
		Population	m_Population[3];
		int			parentGen, childGen, dummy;
		
		DE_Rand_1	m_DE_Rand_1_Mutator;


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



#endif	// DIFFERENTIAL_EVOLUTION_H //