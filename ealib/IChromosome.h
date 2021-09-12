#ifndef I_CHROMOSOME_H
#define	I_CHROMOSOME_H

#include	<oreore/common/Utility.h>
#include	<oreore/common/DLLExport.h>

#include	"DesignParameter.h"
#include	"PopulationResult.h"



namespace ealib
{
	class Initializer;

	// Chromosome interface
	class CLASS_DECLSPEC IChromosome
	{
	public:

		IChromosome();
		IChromosome( const IChromosome& obj );
		virtual ~IChromosome();

		virtual bool Init( const DesignParamArray& parameters );
		virtual void Release();

		void BindEvalResultView( EvalResultView* result );
		void UnbindEvalResultView();

		float Score() const						{ return m_pResult->Score( m_CurrentEval ); }
		void SetScore( float score )			{ m_pResult->Score( m_CurrentEval ) = score; }

		float Score( int idx ) const			{ return m_pResult->Score( idx ); }
		void SetScore( float score, int idx )	{ m_pResult->Score( idx ) = score; }

		float Fitness() const					{ return m_pResult->Fitness( m_CurrentEval ); }
		void SetFitness( float fitness )		{ m_pResult->Fitness( m_CurrentEval ) = fitness; }

		float Fitness( int idx ) const				{ return m_pResult->Fitness( idx ); }
		void SetFitness( float fitness, int idx )	{ m_pResult->Fitness( idx ) = fitness; }

		DesignParamArray& GetDesignParamArray()				{ return m_DesignParameters; }
		const DesignParamArray& GetDesignParamArray() const	{ return m_DesignParameters; }

		void ExtractOrderRestoredDesignParamArray( DesignParamArray& params )
		{
			params.Init( m_DesignParameters.Length() );// = m_DesignParameters;

			for( const auto& p : m_DesignParameters )
				params[ p.SequentialID() ] = p;
		}



		const EvalResultView* EvalResult() const		{ return m_pResult; }
		void CopyEvalResultFrom( const EvalResultView* const src )	{ this->m_pResult->CopyFrom( src );	}
		//void ClearResult()							{ m_pResult->Clear(); }

		int ID() const			{ return m_ID; }
		void SetID( int id )	{ m_ID = id; }
		
		void SetCurrentEvaluator( int idx )			{ m_CurrentEval = idx; }



		bool Dominates( const IChromosome* chromosome ) const;


		// Virtual Functions
		virtual void Initialize( Initializer* pInit );


		// Pure Virtual Functions.
		virtual int Size() const = 0;
		virtual int NumChromosomeTypes() const=0;
		virtual int16 TypeInfo() const=0;
		
		virtual IChromosome* GetChromosome( int i=0 ) const=0;
		virtual IChromosome* GetChromosomeByType( int16 type ) const=0;
		virtual IChromosome* Clone() const=0;

		virtual void* GetGene( int i=0 ) const=0;
		virtual void* GetGene( tstring key ) const=0;
		virtual void CopyGeneFrom( const IChromosome* pSrc )=0;
		virtual void ClearGene()=0;

		virtual DesignParameter& GetDesignParameter( int i )=0;
		virtual const DesignParameter& GetDesignParameter( int i ) const=0;
		virtual DesignParameter& GetDesignParameter( const tstring& key )=0;
		virtual const DesignParameter& GetDesignParameter( const tstring& key )	const=0;



		template< typename T > inline T& GeneAs( int i=0 )				{ return *(T*)GetGene(i); }
		template< typename T > inline const T& GeneAs( int i=0 ) const	{ return *(T*)GetGene(i); }

		template< typename T > inline T& GeneAs( const tstring& key )	{ return *(T*)GetGene(key); }
		template< typename T > inline const T& GeneAs( const tstring& key ) const { return *(T*)GetGene(key); }


	protected:

		DesignParamArray	m_DesignParameters;
		EvalResultView*		m_pResult;
		int					m_CurrentEval;// index for current evaluator.


	private:

		int	m_ID;


		friend int CompareScoreAscend( const IChromosome* pElem1, const IChromosome* pElem2 )
		{
			float fa = pElem1->Score();
			float fb = pElem2->Score();
			return ( fa > fb ) - ( fa < fb );// ascend
		}


		friend int CompareScoreDescend( const IChromosome* pElem1, const IChromosome* pElem2 )
		{
			float fa = pElem1->Score();
			float fb = pElem2->Score();
			return ( fa < fb ) - ( fa > fb );// descend
		}


		friend int CompareFitnessAscend( const IChromosome* pElem1, const IChromosome* pElem2 )
		{
			float fa = pElem1->Fitness();
			float fb = pElem2->Fitness();
			return ( fa > fb ) - ( fa < fb );// ascend
		}


		friend int CompareFitnessDescend( const IChromosome* pElem1, const IChromosome* pElem2 )
		{
			float fa = pElem1->Fitness();
			float fb = pElem2->Fitness();
			return ( fa < fb ) - ( fa > fb );// descend
		}


	};


	                 
//	using const IChromosome* = const IChromosome*;


}// end of namespace



#endif // !I_CHROMOSOME_H
