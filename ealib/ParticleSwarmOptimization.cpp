#include	"ParticleSwarmOptimization.h"

#include	<oreore/common/TString.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>


// http://www.mnemstudio.org/particle-swarm-introduction.htm



namespace ealib
{

	ParticleSwarmOptimization::ParticleSwarmOptimization()
		: IEvolutionaryAlgorithm()
		, individual( 0 )
		, personalbest( 1 )
		, velocity( 2 )
		, groupbest( 3 )
	{
		ClearAttribute();
	}



	ParticleSwarmOptimization::ParticleSwarmOptimization( const ParticleSwarmOptimization& obj )
		: IEvolutionaryAlgorithm(obj)
		, m_PSOAttrib( obj.m_PSOAttrib )
		, individual( 0 )
		, personalbest( 1 )
		, velocity( 2 )
		, groupbest( 3 )
	{

	}



	ParticleSwarmOptimization::~ParticleSwarmOptimization()
	{
		ReleasePopulation();
	}

	

	void ParticleSwarmOptimization::SetAttribute( const PSOAttribute& attrib )
	{
		m_PSOAttrib	= attrib;
	}



	void ParticleSwarmOptimization::SetInertiaWeight( float w )
	{
		m_PSOAttrib.W	= w;
	}



	void ParticleSwarmOptimization::SetAccelCoeff1( float c1 )
	{
		m_PSOAttrib.C1	= c1;
	}



	void ParticleSwarmOptimization::SetAccelCoeff2( float c2 )
	{
		m_PSOAttrib.C2	= c2;
	}



	bool ParticleSwarmOptimization::InitPopulation( const DesignParamArray& designParams, int numObjectives )
	{
		try
		{
			//===============	バッファを確保する	=================//
			m_Population[ individual ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ personalbest ].Init( designParams, m_Attrib.PopulationSize, numObjectives );
			m_Population[ velocity ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			// 次世代個体の親を格納するバッファの確保
			m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );

			// ダミーデータも初期化する
			m_Population[ groupbest ].Init( designParams, m_Attrib.PopulationSize, numObjectives );

			m_bReady = true;
		}
		catch( ... )
		{
			HANDLE_EXCEPTION();
			ReleasePopulation();
		}

		return m_bReady;
	}



	// Deprecated. 2021.08.23
	//void ParticleSwarmOptimization::InitPopulation( const IChromosome* pChromosome, int numObjectives )
	//{
	//	try
	//	{
	//		//===============	バッファを確保する	=================//
	//		m_Population[ individual ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ personalbest ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );
	//		m_Population[ velocity ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		// 次世代個体の親を格納するバッファの確保
	//		m_Attrib.EliteSize	= Min( m_Attrib.EliteSize, m_Attrib.PopulationSize );

	//		// ダミーデータも初期化する
	//		m_Population[ groupbest ].Init( pChromosome, m_Attrib.PopulationSize, numObjectives );

	//		m_bReady = true;
	//	}
	//	catch( ... )
	//	{
	//		HANDLE_EXCEPTION();
	//		ReleasePopulation();
	//	}
	//}



	void ParticleSwarmOptimization::ReleasePopulation()
	{
		m_Population[ individual ].Release();
		m_Population[ personalbest ].Release();
		m_Population[ velocity ].Release();
		m_Population[ groupbest ].Release();

		m_bReady = false;
	}



	IEvolutionaryAlgorithm* ParticleSwarmOptimization::Clone() const
	{
		return new ParticleSwarmOptimization( *this );
	}



	void ParticleSwarmOptimization::Step( Evaluator* pEval )
	{
		auto& x			= m_Population[ individual ].Indivuduals();
		auto& pbest		= m_Population[ personalbest ].Indivuduals();
		auto& v			= m_Population[ velocity ].Indivuduals();
		auto* pgbest	= m_Population[ groupbest ].Individual(0);

		// Update Best
		int best = 0;
		float bestFitness = x[best]->Fitness();
		for( int i=1; i<m_Attrib.PopulationSize; ++i )
		{
			// Update Personal Best
			if( x[i]->Fitness() > pbest[i]->Fitness() )
				pbest[i]->CopyGeneFrom( x[i] );

			// Search Group best
			if( x[i]->Fitness() > bestFitness )
			{
				best = i;
				bestFitness = x[best]->Fitness();
			}
		}

		pgbest->CopyGeneFrom( x[best] );// Copy Group Best

		// Update Individuals
		for( int i=0; i<m_Attrib.PopulationSize; ++i )
		{
			//IChromosome* pbest	= m_Population[personalBest].Individual(i);
			// Update Self position
			IChromosome *x_i	= x[i];
			IChromosome *v_i	= v[i];

			float lambda1	= m_PSOAttrib.C1 * (float)OreOreLib::genrand_real1();
			float lambda2	= m_PSOAttrib.C2 * (float)OreOreLib::genrand_real1();

			for( int j=0; j<x_i->Size(); ++j )
			{
				const DesignParameter& pDParam = x_i->GetDesignParameter( j );
				float& x_i_j	= x_i->GeneAs<float>( j );
				float& v_i_j	= v_i->GeneAs<float>( j );

				// こっちはベクトルの更新
				v_i_j	= m_PSOAttrib.W * v_i_j + lambda1 * ( pbest[i]->GeneAs<float>(j) - x_i_j ) + lambda2 * ( pgbest->GeneAs<float>(j) - x_i_j );

				// 位置を更新する
				x_i_j	= v_i_j + x_i_j;

				float lbound	= pDParam.LowerBoundary<float>();
				float ubound	= pDParam.UpperBoundary<float>();

				if( x_i_j > ubound )
				{
					x_i_j	= ubound - fabs( x_i_j - ubound );
					v_i_j *= -1.0f;
				}
				else if( x_i_j < lbound )
				{
					x_i_j	= lbound + fabs( x_i_j - lbound );
					v_i_j *= -1.0f;
				}


			}// end of j loop
			
			pEval->Evaluate( x_i );

		}// end of i loop

		m_Stats.Update( m_Population[individual] );
	}



	void ParticleSwarmOptimization::Evolve( Evaluator* pEval, unsigned int seed )
	{	
		if( !m_bReady )
			return;

		OreOreLib::init_genrand( seed );
		
		
		m_Population[ individual ].Initialize( m_refInitializer, pEval );// Random initialize
		m_Population[ personalbest ].CopyChromosomes( &m_Population[individual] );// init personal best with initial position
		m_Population[ velocity ].Clear( pEval );
		m_Population[ groupbest ].Clear( pEval );


		m_Stats.Reset( m_Population[ individual ] );

		while( !m_refCriteria->CheckTermination( &m_Stats ) )
		{
			Step( pEval );
		}

		//for( int gen=0; gen<m_Attrib.NumGenerations; ++gen )
		//{
		//	Step( pEval );
		//}
		
		m_Population[individual].Sort( Population::SORT_FITNESS_DESCEND );
	}



	void ParticleSwarmOptimization::TakeSnapshot( Population& pOut ) const
	{
		if( m_bReady )
			pOut = Population( m_Population[individual] );
	}



	void ParticleSwarmOptimization::ClearAttribute()
	{
		m_Attrib.Clear();
		m_PSOAttrib.Clear();
	}


}// end of namespace