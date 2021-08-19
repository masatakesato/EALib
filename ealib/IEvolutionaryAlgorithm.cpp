#include	"IEvolutionaryAlgorithm.h"

#include	<oreore/mathlib/MathLib.h>

#include	"RandomInitializer.h"
#include	"TerminateOnIteration.h"



namespace ealib
{

	// Empty mutator. Dose nothing.
	const Mutator	c_DummyMutator;

	// Empty crossover. Doses nothing
	const Crossover	c_DummyCrossover;

	// Initializer
	const RandomInitializer	c_RandomInitializer;

	// Default termination Criteria
	const TerminateOnIteration	c_TerminateOnIteration;




	IEvolutionaryAlgorithm::IEvolutionaryAlgorithm()
		: m_Attrib{}
		//, m_refChromosome( nullptr )
		, m_Stats()
		, m_refInitializer( (Initializer *)&c_RandomInitializer )
		, m_refMutator( (Mutator*)&c_DummyMutator )
		, m_refCrossover( (Crossover*)&c_DummyCrossover )
		, m_refCriteria( (ITerminationCriteria*)&c_TerminateOnIteration )
		, m_bReady( false )
	{
		m_Attrib.Clear();
		m_Stats.Init( m_Attrib.NumGenerations, 20, 10 );
	}



	IEvolutionaryAlgorithm::IEvolutionaryAlgorithm( const IEvolutionaryAlgorithm& obj )
		: m_Attrib( obj.m_Attrib )
		//, m_refChromosome( obj.m_refChromosome )
		, m_Stats( obj.m_Stats )
		, m_refInitializer( obj.m_refInitializer )
		, m_refMutator( obj.m_refMutator )
		, m_refCrossover( obj.m_refCrossover )
		, m_refCriteria( obj.m_refCriteria )
		, m_bReady( false )
	{
		
	}



	IEvolutionaryAlgorithm::~IEvolutionaryAlgorithm()
	{
		//m_refChromosome = nullptr;

		m_Stats.Release();

		m_refInitializer	= nullptr;
		m_refMutator		= nullptr;
		m_refCrossover		= nullptr;
		m_refCriteria		= nullptr;
	}



	//void IEvolutionaryAlgorithm::BindChromosome( IChromosome *genome )
	//{
	//	m_refChromosome = genome;
	//}



	//void IEvolutionaryAlgorithm::UnbindChromosome()
	//{
	//	m_refChromosome = nullptr;
	//}



	void IEvolutionaryAlgorithm::BindInitializer( Initializer* initializer )
	{
		m_refInitializer	= initializer;
	}



	void IEvolutionaryAlgorithm::UnbindInitializer()
	{
		m_refInitializer	= (Initializer*)&c_RandomInitializer;
	}



	void IEvolutionaryAlgorithm::BindMutator( Mutator* mutator )
	{
		m_refMutator = mutator;
	}



	void IEvolutionaryAlgorithm::UnbindMutator()
	{
		m_refMutator = (Mutator*)&c_DummyMutator;
	}

	

	void IEvolutionaryAlgorithm::BindCrossover( Crossover* crossover )
	{
		m_refCrossover = crossover;
	}



	void IEvolutionaryAlgorithm::UnbindCrossover()
	{
		m_refCrossover = (Crossover*)&c_DummyCrossover;
	}



	void IEvolutionaryAlgorithm::BindTerminationCriteria( ITerminationCriteria* criteria )
	{
		m_refCriteria = criteria;
	}



	void IEvolutionaryAlgorithm::UnbindTerminationCriteria()
	{
		m_refCriteria = (ITerminationCriteria*)&c_TerminateOnIteration;
	}



	void IEvolutionaryAlgorithm::SetAttribute( const EACommonAttribute& attrib )
	{
		m_Attrib	= attrib;
	}



	void IEvolutionaryAlgorithm::SetPopulationSize( int pop_size )
	{
		m_Attrib.PopulationSize	= Max(pop_size, 2);
	}



	void IEvolutionaryAlgorithm::SetNumGenerations( int gen )
	{
		m_Attrib.NumGenerations = gen;
		m_Stats.SetMaxIteration( gen );
	}



	void IEvolutionaryAlgorithm::SetEliteSize( int elite_size )
	{
		m_Attrib.EliteSize	= elite_size;
	}



}// end of namespace