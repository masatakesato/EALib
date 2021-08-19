#ifndef BINARY_STRING_CHROMOSOME_H
#define	BINARY_STRING_CHROMOSOME_H


#include	<oreore/container/BitArray.h>

#include	"IChromosome.h"



namespace ealib
{

	class CLASS_DECLSPEC BinaryStringChromosome : public IChromosome
	{
	public:

		BinaryStringChromosome();
		BinaryStringChromosome( int bitlength, const void *pInitialData=nullptr );
		BinaryStringChromosome( const BinaryStringChromosome& obj );
		virtual ~BinaryStringChromosome();

		BinaryStringChromosome &operator=( const BinaryStringChromosome& obj );

		void Init( int bitlength, const void *pInitialData=nullptr );// btstring specific Init method 
		virtual void Release();

		BitArray* Gene() const;//	{ return (BitArray *)&m_Gene; }// 染色体データを取得する
		int Gene( int i ) const;//	{ return m_Gene.GetBit( i ); }// iビット目の染色体を取得する

		// Pure Virtual Functions Override
		virtual int NumChromTypes() const			{ return 1; }
		virtual int16 TypeInfo() const;//				{ return TYPE_ID<BitArray>; }
		virtual int Size() const;//					{ return m_Gene.BitLength(); }
		virtual void* GetGene( int i=0 ) const;//		{ return (void *)&m_Gene; }
		virtual void* GetGene( tstring key ) const;//	{ return (void *)&m_Gene; }
		virtual IChromosome* Clone() const;
		virtual void CopyGeneFrom( const IChromosome* pSrc );
		virtual void ClearGene();


	private:

		BitArray	m_Gene;
		
		using IChromosome::GetDesignParameter;

		using IChromosome::Init;

	};



}// end of namespace




#endif // !BINARY_STRING_CHROMOSOME_H
