#ifndef BINARY_STRING_H
#define	BINARY_STRING_H


#include	"DLLExport.h"


namespace ealib
{

	class CLASS_DECLSPEC BinaryString
	{
	public:

		BinaryString();
		BinaryString( int bitlength );
		BinaryString( const BinaryString& obj );
		virtual ~BinaryString();

		BinaryString &operator=( const BinaryString& obj );

		void Init( int bitlength );
		void Release();

		void Set( int b );
		void Unset( int b );
		void Flip( int b );
		void SetAll();
		void UnsetAll();
		void Randomize( int start, int length );// startビット目からlength個だけランダムにビット値を設定する
		void RandomizeAll();

		int BitLength() const	{ return m_BitLength; }
		int GetBit( int b ) const;
		void SetBit( int b, int val );

		inline void CopyFrom( const BinaryString* pSrc );
		inline void CopyFrom( int dst_start, const BinaryString* pSrc, int src_start, int length );

		void Display();


	private:

		typedef /*unsigned int*/unsigned char WORD;
		int		m_BitLength;// ビット長
		int		m_NumWords;	// ビット格納配列の要素数(ワード長)
		WORD	*m_pWords;	// ワード配列

		enum{ WORD_SIZE = sizeof( WORD ) * 8 };
		inline int bindex( int b ) const	{ return b / WORD_SIZE;	}
		inline int boffset( int b ) const	{ return b % WORD_SIZE; }

	};





}// end of namespace



#endif // !BINARY_STRING_H
