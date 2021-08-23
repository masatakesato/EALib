#ifndef	J_DE_H
#define	J_DE_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"DE_Rand_1.h"



namespace ealib
{

	class CLASS_DECLSPEC jDE : public IEvolutionaryAlgorithm
	{
	public:

		jDE();
		jDE( const jDE& obj );
		~jDE();

		// Set attribute
		void SetAttribute( const jDEAttribute& attrib );

		void SetLowerDifferentialWeight( float fl );
		void SetUpperDifferentialWeight( float fu );
		void SetT1( float t );
		void SetT2( float t );

		// Virtual Functions Override
		virtual void InitPopulation( const DesignParamArray& designParams, int numObjectives );
		virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[parentGen]; }
		virtual void TakeSnapshot( Population& pOut ) const;


	private:
		
		jDEAttribute	m_jDEAttrib;
		Population		m_Population[3];
		int				parentGen, childGen, dummy;

		// Control Parameters(F and CR) for each individual
		OreOreLib::Array<float>	m_Fs;
		OreOreLib::Array<float>	m_CRs;

		DE_Rand_1		m_DE_Rand_1_Mutator;


		void UpdateControlParams();
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



#endif	// J_DE_H //