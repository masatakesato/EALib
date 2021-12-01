﻿#include	"SPX.h"

#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>

//#include	"IChromosome.h"
#include	"Chromosome1DFactory.h"



namespace ealib
{
	const Chromosome1DFactory<g_ChoromosomeTypes>	c_Factory;

	const float c_Epsilon = 1.15f;




	SPX::SPX( const DesignParamArray& designParams )
		: ICrossoverOperator( TYPE_ID<float>, { 0.0f, designParams.Length<int32>()+1, 1 } )
		, m_G( c_Factory.Create( designParams ) )
		, m_rs( m_xs.Length() )
		, m_xs( designParams.Length() + 1 )
		, m_Cs( designParams.Length() + 1 )
	{
		for( int32 i=0; i<m_xs.Length<int32>(); ++i )
			m_xs[i] = m_G->Clone();
	}



	SPX::~SPX()
	{
		Release();
	}
	


	void SPX::Init( const DesignParamArray& designParams )
	{
		Release();

		m_G	= c_Factory.Create( designParams );

		m_xs.Init( designParams.Length() + 1 );
		for( int32 i=0; i<m_xs.Length<int32>(); ++i )
			m_xs[i] = m_G->Clone();
	}



	void SPX::Release()
	{
		SafeDelete( m_G );

		m_rs.Release();

		for( int32 i=0; i<m_xs.Length<int32>(); ++i )
			SafeDelete( m_xs[i] );
		m_xs.Release();


		for( int32 i=0; i<m_Cs.Length<int32>(); ++i )
			SafeDelete( m_Cs[i] );
		m_Cs.Release();
	}



	void SPX::Execute( int numparents, const IChromosome* parents[], int numchildren, IChromosome* children[], const void* attribs )
	{
		ASSERT( numparents >= m_xs.Length<int32>() );

		m_G->ClearGene();

		// Calculate center of mass
		for( int i=0; i<m_G->Size(); ++i )
		{
			float& gi = m_G->GeneAs<float>(i);

			for( int32 j=0; j<m_xs.Length<int32>(); ++j )
				gi += parents[j]->GeneAs<float>(i);

			gi /= numparents;
		}

		// Generte random numbers
		for( int32 i=0; i<m_rs.Length<int32>(); ++i )
		{
			m_rs[i] = (float)pow( OreOreLib::genrand_real1(), 1.0/(1.0+i) );
		}


		// Calculate xi and Ci
		for( int32 k=0; k<m_xs.Length<int32>(); ++k )
		{
			auto* xi = m_xs[k];
			const auto* pi = parents[k];

			// xi = G + epsilon * (P[k] - G )
			for( int j=0; j<xi->Size(); ++j )
				xi->GeneAs<float>(j) = c_Epsilon * ( pi->GeneAs<float>(j) - m_G->GeneAs<float>(j) );

			// C[k]
			if( k==0 )
			{
				// C[0] = 0
				m_Cs[k]->ClearGene();
			}
			else
			{
				// C[k] = r[k-1] * ( x[k-1] - x[k] + C[k-1] )
				for( int j=0; j<xi->Size(); ++j )
					m_Cs[k]->GeneAs<float>(j) = m_rs[k-1] * ( m_xs[k-1]->GeneAs<float>(j) - m_xs[k]->GeneAs<float>(j) + m_Cs[k-1]->GeneAs<float>(j) );
			}
		}

		// Generate offspring
		auto* c = children[0];

		auto* xn = m_xs[ m_xs.Length()-1 ];// [ m_Cs.L]
		auto* Cn = m_Cs[ m_Cs.Length()-1 ];

		for( int j=0; j<c->Size(); ++j )
			c->GeneAs<float>(j) = xn->GeneAs<float>(j) + Cn->GeneAs<float>(j);

	}



	void SPX::Execute( OreOreLib::Memory<const IChromosome*>& X, OreOreLib::Memory<IChromosome*>& T, const void* attribs )
	{
		assert( X.Length() >= m_xs.Length() );

		m_G->ClearGene();

		// Calculate center of mass
		for( int32 i=0; i<m_G->Size(); ++i )
		{
			float& gi = m_G->GeneAs<float>(i);

			for( int32 j=0; j<m_xs.Length<int32>(); ++j )
				gi += X[j]->GetChromosomeByType(TypeID)->GeneAs<float>(i);

			gi /= X.Length();
		}

		// Generte random numbers
		for( int32 i=0; i<m_rs.Length<int32>(); ++i )
		{
			m_rs[i] = (float)pow( OreOreLib::genrand_real1(), 1.0/(1.0+i) );
		}


		// Calculate xi and Ci
		for( int32 k=0; k<m_xs.Length<int32>(); ++k )
		{
			auto* xi = m_xs[k];
			auto* pi = X[k]->GetChromosomeByType( TypeID );

			// xi = G + epsilon * (P[k] - G )
			for( int j=0; j<xi->Size(); ++j )
				xi->GeneAs<float>(j) = c_Epsilon * ( pi->GeneAs<float>(j) - m_G->GeneAs<float>(j) );

			// C[k]
			if( k==0 )
			{
				// C[0] = 0
				m_Cs[k]->ClearGene();
			}
			else
			{
				// C[k] = r[k-1] * ( x[k-1] - x[k] + C[k-1] )
				for( int j=0; j<xi->Size(); ++j )
					m_Cs[k]->GeneAs<float>(j) = m_rs[k-1] * ( m_xs[k-1]->GeneAs<float>(j) - m_xs[k]->GeneAs<float>(j) + m_Cs[k-1]->GeneAs<float>(j) );
			}
		}

		// Generate offspring
		auto* c = T[0]->GetChromosomeByType( TypeID );

		auto* xn = m_xs[ m_xs.Length()-1 ];// [ m_Cs.L]
		auto* Cn = m_Cs[ m_Cs.Length()-1 ];

		for( int j=0; j<c->Size(); ++j )
			c->GeneAs<float>(j) = xn->GeneAs<float>(j) + Cn->GeneAs<float>(j);

	}



}// end of namespace