#ifndef EVAL_RESULTS_H
#define	EVAL_RESULTS_H

#include	<oreore/container/Array.h>


namespace ealib
{


	struct EvalResultView
	{
		int		NumObjectives;
		int		ParetoRank;
		float	CrowdingDistance;
		float	ScoreFitnesses[1];// dynamic Score/Fitness array. [0]:score0, [1]:score1, [2]:fitness0, [3]:fitness1 ...


		const float& Score( int i=0 ) const&
		{
			return *( ScoreFitnesses + i );
		}

		float& Score( int i=0 ) &
		{
			return *( ScoreFitnesses + i );
		}

		const float& Fitness( int i=0 ) const &
		{
			return *( ScoreFitnesses + NumObjectives + i );
		}

		float& Fitness( int i=0 ) &
		{
			return *( ScoreFitnesses + NumObjectives + i );
		}


		void Clear()
		{
			memset( ScoreFitnesses, 0, sizeof( float ) * 2 * NumObjectives );
			ParetoRank		= 0;
			CrowdingDistance	= 0.0f;
		}


		void CopyFrom( const EvalResultView* const pSrc )
		{
			ParetoRank		= pSrc->ParetoRank;
			CrowdingDistance	= pSrc->CrowdingDistance;

			memcpy( ScoreFitnesses, pSrc->ScoreFitnesses, sizeof( float ) * 2 * Min( NumObjectives, pSrc->NumObjectives ) );
		}




		static int32 Size( int numObjectives )
		{
			return	sizeof( NumObjectives ) + 
					sizeof( ParetoRank ) +
					sizeof( CrowdingDistance ) +
					numObjectives * sizeof( float ) * 2;
		}


	private:

		EvalResultView();

		EvalResultView( void* mempool, int numObjectives )
			: NumObjectives( numObjectives )
			, ParetoRank( 0 )
			, CrowdingDistance( 0 )
		{
			//memset( mempool, 0, sizeof( float ) * 2 * numObjectives );
		}


		friend class PopulationResult;

	};





	class PopulationResult
	{
	public:

		// Default constructor
		PopulationResult()
			: m_numObjectives( 0 )
			, m_ResultSize( 0 )
		{

		}


		// Constructor
		PopulationResult( int popSize, int numObjectives )
			: m_numObjectives( numObjectives )
			, m_ResultSize( EvalResultView::Size( numObjectives ) )
			, m_Bytes( int(m_ResultSize * popSize) )
		{
			for( int i=0; i<popSize; ++i )
				Create( &m_Bytes[i*m_ResultSize], m_numObjectives );
		}


		// Destructor
		virtual ~PopulationResult()
		{
			Release();
		}


		// Copy constructor
		PopulationResult( const PopulationResult& obj )
					: m_numObjectives( obj.m_numObjectives )
					, m_ResultSize( obj.m_ResultSize )
					, m_Bytes( obj.m_Bytes )
		{

		}


		// Move constructor
		PopulationResult( PopulationResult&& obj )
					: m_numObjectives( obj.m_numObjectives )
					, m_ResultSize( obj.m_ResultSize )
					, m_Bytes( (OreOreLib::Array<uint8>&&)obj.m_Bytes )//std::move(obj.m_Bytes) )
		{
			obj.m_numObjectives = 0;
			obj.m_ResultSize	= 0;
		}


		// Copy assignment operator
		PopulationResult& operator=( const PopulationResult& obj )
		{
			if( this != &obj )
			{
				m_numObjectives	= obj.m_numObjectives;
				m_ResultSize	= obj.m_ResultSize;
				m_Bytes			= obj.m_Bytes;
			}

			return *this;
		}


		// Move assignment operator
		PopulationResult& operator=( PopulationResult&& obj )
		{
			if( this != &obj )
			{
				// transfer data from obj to this
				m_numObjectives	= obj.m_numObjectives;
				m_ResultSize	= obj.m_ResultSize;
				m_Bytes			= (OreOreLib::Array<uint8>&&)obj.m_Bytes;//std::move(obj.m_Bytes);

				// clear obj
				obj.m_numObjectives = 0;
				obj.m_ResultSize	= 0;
			}

			return *this;
		}


		void Init( int popSize, int numObjectives )
		{
			m_numObjectives	= numObjectives;
			m_ResultSize	= EvalResultView::Size( numObjectives );
			m_Bytes.Init( int( m_ResultSize * popSize ) );

			for( int i=0; i<popSize; ++i )
				Create( &m_Bytes[i*m_ResultSize], m_numObjectives );

			for( int i=0; i<popSize; ++i )
			{
				EvalResultView& view = (EvalResultView&)(m_Bytes[i * m_ResultSize]);
				//tcout << view.CrowdingDistance << tendl;
			}

		}



		void Release()
		{
			m_numObjectives = 0;
			m_ResultSize = 0;
			m_Bytes.Release();
		}



		const EvalResultView& operator[]( OreOreLib::MemSizeType n ) const&
		{
			return (EvalResultView&)m_Bytes[n * m_ResultSize];
		}


		// Subscription operator for read-write.( called if Memory is non-const )
		EvalResultView& operator[]( OreOreLib::MemSizeType n ) &
		{
			return (EvalResultView&)m_Bytes[n * m_ResultSize];
		}


		int NumObjectives() const
		{
			return m_numObjectives;
		}



	private:

		int						m_numObjectives;
		int32					m_ResultSize;
		OreOreLib::Array<uint8> m_Bytes;// byte array


		EvalResultView* Create( void* mem, int numObjectives )
		{
			return new ( mem ) EvalResultView( mem, numObjectives );
		}



	};



}// end of namespace


#endif // !EVALUATION_RESULTS_H
