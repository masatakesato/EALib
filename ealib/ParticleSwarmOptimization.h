#ifndef	PARTICLE_SWARM_OPTIMIZATION_H
#define	PARTICLE_SWARM_OPTIMIZATION_H


#include	"IEvolutionaryAlgorithm.h"
#include	"PSOAttribute.h"

#include	<oreore/container/Pair.h>



namespace ealib
{

	class CLASS_DECLSPEC ParticleSwarmOptimization : public IEvolutionaryAlgorithm
	{
	public:

		ParticleSwarmOptimization();
		ParticleSwarmOptimization( const ParticleSwarmOptimization& obj );
		~ParticleSwarmOptimization();

		// Set attribute
		using IEvolutionaryAlgorithm::SetAttribute;
		void SetAttribute( const PSOAttribute& attrib );
		void SetInertiaWeight( float w );
		void SetAccelCoeff1( float c1 );
		void SetAccelCoeff2( float c2 );

		// Virtual Functions Override
		virtual void InitPopulation( const IChromosome* pChromosome, int numObjectives );
		virtual void ReleasePopulation();
		virtual IEvolutionaryAlgorithm* Clone() const;
		virtual void Step( Evaluator* pEval );
		virtual void Evolve( Evaluator* pEval, unsigned int seed=0 );
		virtual Population* GetPopulation() const{ return (Population *)&m_Population[individual]; }
		virtual void TakeSnapshot( Population& pOut ) const;
		
		
	private:

		PSOAttribute	m_PSOAttrib;

		Population		m_Population[4];
		int				individual, personalbest, velocity, groupbest;

		
		void ClearAttribute();


		// Disable Selector binding
		using IEvolutionaryAlgorithm::BindMutator;
		using IEvolutionaryAlgorithm::UnbindMutator;
		void BindSelector( ISelector* selector ){}
		void UnbindSelector(){}

	};



}// end of namespace


#endif	// PARTICLE_SWARM_OPTIMIZATION_H //