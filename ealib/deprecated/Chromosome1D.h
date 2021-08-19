#ifndef	CHROMOSOME_1D_H
#define	CHROMOSOME_1D_H

#include	"IChromosome.h"

//#include	<unordered_map>

#include	<oreore/common/TString.h>
#include	<oreore/container/HashMap.h>

#include	"DesignParameter.h"



namespace ealib
{
	
	class CLASS_DECLSPEC Chromosome1D: public IChromosome
	{
	public:

		Chromosome1D();
		Chromosome1D( const DesignParamArray& parameters );
		Chromosome1D( const Chromosome1D& obj );
		virtual ~Chromosome1D();

		Chromosome1D &operator=( const Chromosome1D& obj );
		
		virtual void Init( const DesignParamArray& parameters );
		virtual void Release();

		bool SetKey( int i, const tstring& newkey );
		bool SetKey( const tstring& currkey, const tstring& newkey );

		template< typename T >
		T* GeneAs( int i=0 ) const			{ return (T*)&m_GeneArray[ m_ElementSize * i ]; }

		template< typename T >
		T* GeneAs( const tstring& key ) const	{ return (T*)&m_GeneArray[ m_ElementSize * m_KeyMap.At( key ) ]; }//{ return (T*)( m_GeneArray + m_ElementSize * m_pKeyMap->at( key ) ); }//
	
		// Virtual Functions Override
		using IChromosome::GetDesignParameter;
		virtual DesignParameter* GetDesignParameter( const tstring& key ) const	{ return (DesignParameter*)&m_DesignParameters[ m_KeyMap.At( key ) ]; }//{ return (DesignParameter*)&m_DesignParameters[ m_pKeyMap->at( key ) ]; }//

		// Pure Virtual Functions Override
		virtual int NumChromTypes() const			{ return 1; }
		virtual int16 TypeInfo() const				{ return m_TypeInfo; }
		virtual int Size() const					{ return m_DesignParameters.Length(); }
		virtual void* GetGene( int i=0 ) const		{ return (void*)&m_GeneArray[ m_ElementSize * i ]; }
		virtual void* GetGene( tstring key ) const	{ return (void*)&m_GeneArray[ m_ElementSize * m_KeyMap.At(key) ]; }//{ return m_GeneArray + m_ElementSize * m_pKeyMap->at(key); }//
		virtual IChromosome* Clone() const;
		virtual void CopyGeneFrom( const IChromosome* pSrc );
		virtual void ClearGene();


	private:

		int16						m_TypeInfo;
		size_t						m_ElementSize;
		OreOreLib::Array<uint8>		m_GeneArray;



		OreOreLib::HashMap< tstring, int, 64 >	m_KeyMap;
//		std::unique_ptr<std::unordered_map<tstring, int>>	m_pKeyMap;//std::unordered_map<tstring, int>	m_KeyMap;
		// m_KeyMapは名前重複があった場合の動作は保証しない. DesignParameterでKey(名前)を設定しない場合は、キー検索未登録扱いにする
	};



}// end of namespace


#endif	// CHROMOSOME_1D_H //