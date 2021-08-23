#ifndef	MULTI_ISLAND_EA_H
#define	MULTI_ISLAND_EA_H


#include	"EACommon.h"
#include	"IEvolutionaryAlgorithm.h"



namespace ealib
{

	class CLASS_DECLSPEC MultiIslandEA : public IEvolutionaryAlgorithm
	{
	public:

		MultiIslandEA();
		MultiIslandEA( const MultiIslandEA& obj );
		~MultiIslandEA();
		
		void BindSolver( IEvolutionaryAlgorithm* pSolver );
		void UnbindSolver();
		IEvolutionaryAlgorithm* GetSolver() const;

		// Set parameters
		void SetAttribute( const MIEAAttribute& attrib );
		void SetIslandSize( int island_size );
		void SetMigrationMode( MIGRATION_MODE mig_mode );
		void SetMigrationInterval( int interval );
		void SetMigrationRate( float mig_rate );


		int IslandSize() const { return m_MIGAAttrib.IslandSize; }


		// Virtual Functions Override
		virtual void InitPopulation( const DesignParamArray& designParams, int numObjectives );
		//virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );// Deprecated. 2021.08.23
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const	{ return m_pSolverArray[0]->GetPopulation(); }
		virtual void TakeSnapshot( Population& pOut ) const;



	private:

		MIEAAttribute			m_MIGAAttrib;// マルチアイランドGAの設定値
		IEvolutionaryAlgorithm	*m_refSolver;

		OreOreLib::Array<IEvolutionaryAlgorithm*>	m_pSolverArray;

		OreOreLib::Array<int>			m_Destinations;
		OreOreLib::Array<Population>	m_Migrants;


		void Emigrate( MIGRATION_MODE mode );
		void Immigrate( MIGRATION_MODE mode );

		void ClearAttribute();


		// forbidden funcs
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

		using IEvolutionaryAlgorithm::SetAttribute;
		using IEvolutionaryAlgorithm::SetPopulationSize;
		using IEvolutionaryAlgorithm::SetNumGenerations;
		using IEvolutionaryAlgorithm::SetEliteSize;

	};



}// end of namespace


#endif	// MULTI_ISLAND_EA_H //