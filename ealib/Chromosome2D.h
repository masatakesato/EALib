#ifndef	CHROMOSOME_2D_H
#define	CHROMOSOME_2D_H

#include	"IChromosome.h"

//#include	<unordered_map>

#include	<oreore/common/TString.h>
#include	<oreore/container/Pair.h>
#include	<oreore/container/HashMap.h>

#include	"Typedefs.h"



namespace ealib
{

	class CLASS_DECLSPEC Chromosome2D : public IChromosome
	{
		using Index2D = Pair<int, int>;// first: chromosome index, second: gene index

	public:

		Chromosome2D();
		Chromosome2D( const DesignParamArray& designParams );
		Chromosome2D( const Chromosome2D& obj );
		virtual ~Chromosome2D();

		Chromosome2D &operator=( const Chromosome2D& obj );
		
		virtual void Init( const DesignParamArray& designParams );
		virtual void Release();

		bool SetKey( int i, const tstring& newkey );
		bool SetKey( const tstring& currkey, const tstring& newkey );

		template< typename T >
		T* GetGeneAs( int i=0 ) const	{ const Index2D& index = m_IndexMap.At(i); return m_Chromosomes[ index.first ]->GeneAs<T>( index.second ); }//{ const Index2D& index = m_pIndexMap->at(i); return (T*)( m_Chromosomes[ index.first ]->GetGene( index.second ) ); }//

		template< typename T >
		T* GetGeneAs( const tstring& key ) const	{ const Index2D& index = m_KeyMap.At( key ); return m_Chromosomes[ index.first ]->GeneAs<T>( index.second ); }//{ const Index2D& index = m_pKeyMap->at( key ); return (T*)( m_Chromosomes[ index.first ]->GetGene( index.second ) ); }//
		
		// Virtual Functions Override
		virtual void Initialize( Initializer* pInit );
		virtual DesignParameter* GetDesignParameter( int i )				const { const Index2D& index = m_IndexMap.At( i ); return m_Chromosomes[ index.first ]->GetDesignParameter( index.second ); }//{ const Index2D& index = m_pIndexMap->at( i ); return m_Chromosomes[ index.first ]->GetDesignParameter( index.second ); }//
		virtual DesignParameter* GetDesignParameter( const tstring& key )	const { const Index2D& index = m_KeyMap.At( key ); return m_Chromosomes[ index.first ]->GetDesignParameter( index.second ); }//{ const Index2D& index = m_pKeyMap->at( key ); return m_Chromosomes[ index.first ]->GetDesignParameter( index.second ); }//

		// Pure Virtual Functions Override.
		virtual IChromosome* GetChromosome( int i=0 ) const;
		virtual IChromosome* GetChromosomeByType( int16 type ) const;
		virtual int NumChromosomeTypes() const		{ return m_Chromosomes.Length(); }
		//virtual int16 TypeInfo() const			{ return TYPE_UNKNOWN; }// Disabled
		virtual int Size() const					{ return m_DesignParameters.Length(); }
		virtual void* GetGene( int i=0 ) const		{ const Index2D& index = m_IndexMap.At(i); return m_Chromosomes[ index.first ]->GetGene( index.second ); }//{ const Index2D& index = m_pIndexMap->at(i); return m_Chromosomes[ index.first ]->GetGene( index.second ); }//
		virtual void* GetGene( tstring key ) const	{ const Index2D& index = m_KeyMap.At( key ); return m_Chromosomes[ index.first ]->GetGene( index.second ); }//{ const Index2D& index = m_pKeyMap->at( key ); return m_Chromosomes[ index.first ]->GetGene( index.second ); }//
		virtual IChromosome* Clone() const;
		virtual void CopyGeneFrom( const IChromosome* pSrc );// execute only when chromosome is Chromosome2D class
		virtual void ClearGene();


	private:
		
		OreOreLib::StaticArray<int, NUM_TYPES>	m_TypeToIndex;// typeid to chrosomome element index conversion table
		OreOreLib::Array<IChromosome*>	m_Chromosomes;

		// m_KeyMapは名前重複があった場合の動作は保証しない. DesignParameterでKey(名前)を設定しない場合は、キー検索未登録扱いにする
		OreOreLib::HashMap< tstring, Index2D, 128 >	m_KeyMap;
		OreOreLib::HashMap< int, Index2D, 128 >		m_IndexMap;

		//std::unique_ptr< std::unordered_map< tstring, Index2D > >	m_pKeyMap;//std::unordered_map<tstring, Index2D>	m_KeyMap;// Index2D=( index of m_Chromosomes, data position inside IChromosome )
		//std::unique_ptr< std::unordered_map< int, Index2D > >		m_pIndexMap;//std::unordered_map<int, Index2D>		m_IndexMap;


		// Private functions
		void ClearTypeToIndex();
		void DeepCopyChromosomeArray( const Chromosome2D& src );
		void DeepRemoveChromosomeArray();// Check if m_Chromosomes is empty before calling this method!!!


		using IChromosome::TypeInfo;// Disable TypeInfo method
		virtual int16 TypeInfo() const	{ return TYPE_UNKNOWN; }

	};


}// end of namespace


#endif	// CHROMOSOME_2D_H //