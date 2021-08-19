#include	"BinaryStringChromosome.h"


#include	<oreore/common/TString.h>

#include	"Typedefs.h"



namespace ealib
{


	BinaryStringChromosome::BinaryStringChromosome()
		: IChromosome()
	{
		
	}



	BinaryStringChromosome::BinaryStringChromosome( int bitlength, const void *pInitialData )
		: IChromosome()
	{
		Init( bitlength, pInitialData );
	}



	BinaryStringChromosome::BinaryStringChromosome( const BinaryStringChromosome& obj )
		: IChromosome(obj)
	{
		if( obj.m_Gene.BitLength() > 0 )
		{
			m_Gene.Init( obj.m_Gene.BitLength() );
			m_Gene.CopyFrom( &obj.m_Gene );
		}
	}



	BinaryStringChromosome::~BinaryStringChromosome()
	{
		Release();
	}



	BinaryStringChromosome &BinaryStringChromosome::operator=( const BinaryStringChromosome& obj )
	{
		Release();

		if( !obj.m_DesignParameters.Empty() )
			m_DesignParameters = obj.m_DesignParameters;
		
		m_CurrentEval	= obj.m_CurrentEval;
		m_pResult		= nullptr;

		if( obj.m_Gene.BitLength() > 0 )
		{
			m_Gene.Init( obj.m_Gene.BitLength() );
			m_Gene.CopyFrom( &obj.m_Gene );
		}

		return *this;
	}



	void BinaryStringChromosome::Init( int bitlength, const void *pInitialData )
	{
		Release();
		m_Gene.Init( bitlength );
	}



	void BinaryStringChromosome::Release()
	{
		m_DesignParameters.Release();
		m_Gene.Release();
	}



	BitArray* BinaryStringChromosome::Gene() const
	{
		return (BitArray *)&m_Gene;
	}



	int BinaryStringChromosome::Gene( int i ) const
	{
		return m_Gene.GetBit( i );
	}



	int16 BinaryStringChromosome::TypeInfo() const
	{
		return TYPE_ID<BitArray>;
	}
	
	

	int BinaryStringChromosome::Size() const
	{
		return m_Gene.BitLength();
	}
	

	
	void* BinaryStringChromosome::GetGene( int i ) const
	{
		return (void *)&m_Gene;
	}
	


	void* BinaryStringChromosome::GetGene( tstring key ) const
	{
		return (void *)&m_Gene;
	}



	IChromosome* BinaryStringChromosome::Clone() const
	{
		return new BinaryStringChromosome( *this );
	}



	void BinaryStringChromosome::CopyGeneFrom( const IChromosome* pSrc )
	{
		m_Gene.CopyFrom( ((BinaryStringChromosome*)pSrc)->Gene() );

		if( m_pResult )	m_pResult->CopyFrom( pSrc->GetEvalResult() );
	}



	void BinaryStringChromosome::ClearGene()
	{
		m_Gene.UnsetAll();
	}



}// end of namespace