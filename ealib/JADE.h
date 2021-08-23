#ifndef	JADE_H
#define	JADE_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"DE_Current_to_pBest_1_Archive.h"


namespace ealib
{

	class CLASS_DECLSPEC JADE : public IEvolutionaryAlgorithm
	{
	public:

		JADE();
		JADE( const JADE& obj );
		~JADE();

		// Set attribute
		void SetAttribute( const DEAttribute& attrib );
		void SetAttribute( const JADEAttribute& attrib );
		void SetDifferentialWeight( float f );
		void SetCrossoverProbability( float cr );	// 交叉率の設定
		void SetBestPercentage( float p );	// P
		void SetLearningRate( float c );	// C

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

		DEAttribute		m_DEAttrib;
		JADEAttribute	m_JADEAttrib;
		Population		m_Population[4];
		int				parentGen, childGen, archive, dummy;

		int				m_numActiveArchives;
		OreOreLib::Array<int>	m_ArchiveIndices;

		// Control Parameters(F and CR) for each individual
		OreOreLib::Array<float>	m_Fs;
		OreOreLib::Array<float>	m_CRs;

		float			m_MuF;
		float			m_MuCR;

		DE_Current_to_pBest_1_Archive	m_Mutator;


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



#endif	// JADE_H //