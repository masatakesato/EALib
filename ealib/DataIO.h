#ifndef EALIB_DATA_IO_H
#define	EALIB_DATA_IO_H

#include	<exception>

#include	<oreore/common/TString.h>
#include	<oreore/container/Array.h>
#include	<oreore/container/ArrayView.h>
#include	<oreore/container/HashMap.h>

#include	"Typedefs.h"
#include	"DesignParameter.h"

#include	"Chromosome1D.h"
#include	"Chromosome2D.h"
#include	"Population.h"



namespace ealib
{

	//##################################################################################################//
	//																									//
	//									DataIO class implementation										//
	//																									//
	//##################################################################################################//


	class DataIOException : public std::exception{};


	class CLASS_DECLSPEC DataIO
	{
	public:

		using ParamSet = OreOreLib::Array< tstring >;
		using ParamSetArray = OreOreLib::Array< ParamSet >;

		// CSV loader
		bool LoadCSV( const tstring& filename, ParamSetArray& params );


		// Chromosome loader
		IChromosome* LoadChromosome( const tstring& filepath );

		template < typename T >
		bool LoadChromosome1D( Chromosome1D<T>& chromosome, const ParamSetArray& params );
		bool LoadChromosome2D( Chromosome2D& chromosome, const ParamSetArray& params );


		// Experimental implementation
		bool ExportSnapshot( const Population* population, const tstring& filepath );
		bool ExportSnapshots( int num, const Population populations[], const tstring& filepath );



	private:

		int CountType( const ParamSetArray& params, int16 type );
		ParamSet SplitString( tstring& input, TCHAR delimiter );
		tstring CreateSnapshotString( const Population* population );

		static tstring (* c_NumericToTString[ NUM_TYPES ] )( const void* );
		static void (* c_InitDesignParamFuncs[ NUM_TYPES ] )( DesignParameter*, const TCHAR*, const TCHAR*, const TCHAR*, const TCHAR* );

	};




	template < typename T >
	inline bool DataIO::LoadChromosome1D( Chromosome1D<T>& chromosome, const ParamSetArray& params )
	{
		//==================== Create DesignParameters ===================//
		int16 type = TYPE_ID<T>;

		int descCount = CountType( params, type );
		if( descCount==0 )	return false;

		DesignParamArray designParams( descCount );

		int i=0;
		for( const auto& row : params )
		{
			OreOreLib::ArrayView<tstring> data( row.begin(), row.Length() );

			// reject unknown type data
			if( !g_TypeInfoDict.Exists( data[1] ) )
				continue;

			// reject unspecified data type
			if( type != g_TypeInfoDict.At( data[1] ) )
				continue;

			designParams[i].SetKey( data[0] );
			designParams[i].SetType( type );
			designParams[i].SetSamplingType( SamplingType::Enumerated );
			designParams[i].SetBoundaryType( BoundaryType::Inclusive, BoundaryType::Inclusive );

			c_InitDesignParamFuncs[ designParams[i].TypeID() ]
			(
				&designParams[i],
				data[0].c_str(),// key
				data[2].c_str(), data[3].c_str(), data[4].c_str()// min, max, default_value
			);

			++i;
		}


		//==================== Init Chromsomes ===================//

		// Allocate
		chromosome.Init( designParams );


		return true;
	}


}// end of namespace


#endif // !EALIB_DATA_IO_H
