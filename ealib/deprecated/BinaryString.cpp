#include	"BinaryString.h"


#include	<oreore/common/TString.h>
#include	<oreore/common/Utility.h>
#include	<oreore/mathlib/MathLib.h>
#include	<oreore/mathlib/MersenneTwister.h>




namespace ealib
{

	// Default constructor
	BinaryString::BinaryString()
	{
		m_BitLength	= 0;
		m_NumWords	= 0;
		m_pWords	= nullptr;
	}



	// Constructor
	BinaryString::BinaryString( int bitlength )
	{
		Init( bitlength );
	}



	// Copy constructor
	BinaryString::BinaryString( const BinaryString& obj )
	{
		m_BitLength	= obj.m_BitLength;
		m_NumWords	= obj.m_NumWords;
		if( obj.m_pWords )
		{
			m_pWords	= new WORD[m_NumWords];
			memcpy( m_pWords, obj.m_pWords, sizeof( WORD )*m_NumWords );
		}
		else
		{
			m_pWords	= nullptr;
		}


	}



	// Destructor
	BinaryString::~BinaryString()
	{
		Release();
	}


	
	// Copy assignment operator
	BinaryString& BinaryString::operator=( const BinaryString& obj )
	{
		Release();

		m_BitLength	= obj.m_BitLength;
		m_NumWords	= obj.m_NumWords;
		if( obj.m_pWords )
		{
			m_pWords	= new WORD[m_NumWords];
			memcpy( m_pWords, obj.m_pWords, sizeof( WORD )*m_NumWords );
		}
		else
		{
			m_pWords	= nullptr;
		}

		return *this;
	}



	void BinaryString::Init( int bitlength )
	{
		Release();

		m_BitLength	= bitlength;
		m_NumWords	= DivUp( m_BitLength, (int)WORD_SIZE );

		m_pWords	= new WORD[m_NumWords];
		UnsetAll();
	}



	void BinaryString::Release()
	{
		m_BitLength	= 0;
		m_NumWords	= 0;
		SafeDeleteArray( m_pWords );
	}



	void BinaryString::Set( int b )
	{
		m_pWords[bindex( b )] |= 1 << boffset( b );
	}



	void BinaryString::Unset( int b )
	{
		m_pWords[bindex( b )] &= ~(1 << boffset( b ));
	}



	void BinaryString::Flip( int b )
	{
		m_pWords[bindex( b )] ^= 1 << boffset( b );
	}



	void BinaryString::SetAll()
	{
		const uint8 nonzero	= ~0;
		memset( m_pWords, nonzero, sizeof( WORD )*m_NumWords );
	}



	void BinaryString::UnsetAll()
	{
		const uint8 zero	= 0;
		memset( m_pWords, zero, sizeof( WORD )*m_NumWords );
	}



	void BinaryString::Randomize( int start, int length )
	{
		int numFlips = int( OreOreLib::genrand_real1() * length );
		for( int b=0; b<numFlips; ++b )
		{
			int idx	= int( OreOreLib::genrand_real2() * length ) +start;
			Flip( idx );
		}
	}



	void BinaryString::RandomizeAll()
	{
		UnsetAll();

		int numFlips = int( OreOreLib::genrand_real1() * m_BitLength );
		for( int b=0; b<numFlips; ++b )
		{
			int idx	= int( OreOreLib::genrand_real2() * m_BitLength );
			Flip( idx );
		}

	}

	

	int BinaryString::GetBit( int b ) const
	{
		return int( (m_pWords[bindex( b )] & 1<<boffset( b )) > 0 );
	}



	void BinaryString::SetBit( int b, int val )
	{
		uint32 bitOffset	= boffset( b );
		m_pWords[bindex( b )]	= m_pWords[bindex( b )] & ~(1<<bitOffset) | val<<bitOffset;
	}



	void BinaryString::CopyFrom( const BinaryString* pSrc )
	{
		memcpy( m_pWords, pSrc->m_pWords, sizeof( WORD )*m_NumWords );
	}



	void BinaryString::CopyFrom( int dst_start, const BinaryString* pSrc, int src_start, int length )
	{
		for( int dst=dst_start, src=src_start; dst<Min( dst_start+length, m_BitLength ); ++dst, ++src )
		{
			SetBit( dst, pSrc->GetBit( src ) );
		}

	}



	void BinaryString::Display()
	{
		//tcout << "BinaryString..." << tendl;
		for( int b=0; b<m_BitLength; ++b )
		{
			tcout << GetBit( b );
			if( b%8==7 && b>0 )	tcout << " ";
		}

		tcout << tendl;
	}



}// end of namespace