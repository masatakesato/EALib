#ifndef	SHADE_H
#define	SHADE_H

#include	"IEvolutionaryAlgorithm.h"
#include	"DEAttribute.h"
#include	"DE_Current_to_pBest_1_Archive.h"


namespace ealib
{

	// Success-History Based Parameter Adaptation for Differential Evolution
	class CLASS_DECLSPEC SHADE : public IEvolutionaryAlgorithm
	{
	public:

		SHADE();
		SHADE( const SHADE& obj );
		~SHADE();

		// Set attribute
		void SetAttribute( const DEAttribute& attrib );
		void SetAttribute( const SHADEAttribute& attrib );
		void SetDifferentialWeight( float f );
		void SetCrossoverProbability( float cr );	// 交叉率の設定
		void SetBestPercentage( float p );	// P
		void SetLearningRate( float c );	// C

		// Virtual Functions Override
		virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );//virtual void Init();
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[parentGen]; }
		virtual void TakeSnapshot( Population& pOut ) const;


	private:

		DEAttribute		m_DEAttrib;
		SHADEAttribute	m_SHADEAttrib;
		Population		m_Population[4];
		int				parentGen, childGen, archive, dummy;
		
		int							m_numActiveArchives;
		OreOreLib::Array<int>		m_ArchiveIndices;

		// Control Parameters(F, CR and p) for each individual
		OreOreLib::Array<float>	m_Fs;
		OreOreLib::Array<float>	m_CRs;
		OreOreLib::Array<float>	m_Ps;

		float			m_MuF;
		float			m_MuCR;
		float			m_Pmin;

		DE_Current_to_pBest_1_Archive	m_Mutator;


		void UpdateControlParams();
		void ClearAttribute();


		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindSelector;
		using IEvolutionaryAlgorithm::UnbindSelector;
		virtual void BindSelector( ISelector* selector ){}
		virtual void UnbindSelector(){}

		// Disable Mutator binding
		using IEvolutionaryAlgorithm::BindMutator;
		using IEvolutionaryAlgorithm::UnbindMutator;
		virtual void BindMutator( Mutator* mutator ){}
		virtual void UnbindMutator(){}

	};






	// Mixed Integer version
	class CLASS_DECLSPEC MixedSHADE: public IEvolutionaryAlgorithm
	{
	public:

		MixedSHADE();
		MixedSHADE( const MixedSHADE& obj );
		~MixedSHADE();

		// Set attribute
		void SetAttribute( const DEAttribute& attrib );
		void SetAttribute( const SHADEAttribute& attrib );
		void SetDifferentialWeight( float f );
		void SetCrossoverProbability( float cr );	// 交叉率の設定
		void SetBestPercentage( float p );	// P
		void SetLearningRate( float c );	// C

		// Virtual Functions Override
		virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );//virtual void Init();
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[parentGen]; }
		virtual void TakeSnapshot( Population& pOut ) const;


	private:

		DEAttribute		m_DEAttrib;
		SHADEAttribute	m_SHADEAttrib;
		Population		m_Population[4];
		int				parentGen, childGen, archive, dummy;

		int				m_numActiveArchives;
		int				*m_ArchiveIndices;

		// Control Parameters(F, CR and p) for each individual
		OreOreLib::Array<float>	m_Fs;
		OreOreLib::Array<float>	m_CRs;
		OreOreLib::Array<float>	m_Ps;

		float			m_MuF;
		float			m_MuCR;
		float			m_Pmin;

		DE_Current_to_pBest_1_Archive	m_Mutator;


		void UpdateControlParams();
		void ClearAttribute();


		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindSelector;
		using IEvolutionaryAlgorithm::UnbindSelector;
		virtual void BindSelector( ISelector* selector ){}
		virtual void UnbindSelector(){}

		// Disable Mutator binding
		using IEvolutionaryAlgorithm::BindMutator;
		using IEvolutionaryAlgorithm::UnbindMutator;
		virtual void BindMutator( Mutator* mutator ){}
		virtual void UnbindMutator(){}

	};







}// end of namespace



#endif	// SHADE_H //