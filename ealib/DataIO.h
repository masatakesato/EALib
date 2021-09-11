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

		bool LoadDesignParams( DesignParamArray& designparams, const tstring& filepath );

		// Chromosome loader
		IChromosome* LoadChromosome( const tstring& filepath );
		template < typename T > bool LoadChromosome1D( Chromosome1D<T>& chromosome, const tstring& filepath );
		bool LoadChromosome2D( Chromosome2D& chromosome, const tstring& filepath );

		// Experimental implementation
		bool ExportSnapshot( const Population* population, const tstring& filepath );
		bool ExportSnapshots( int num, const Population populations[], const tstring& filepath );


	private:

		using StrParamSet = OreOreLib::Array< tstring >;
		using StrParamSetArray = OreOreLib::Array< StrParamSet >;


		bool LoadCSV( StrParamSetArray& params, const tstring& filename );
		StrParamSet SplitString( tstring& input, TCHAR delimiter );
		tstring CreateSnapshotString( const Population* population );

		static tstring (* c_NumericToTString[ NUM_TYPES ] )( const void* );
		static void (* c_InitDesignParamFuncs[ NUM_TYPES ] )( DesignParameter*, const TCHAR*, const TCHAR*, const TCHAR*, const TCHAR* );

	};




	template < typename T >
	inline bool DataIO::LoadChromosome1D( Chromosome1D<T>& chromosome, const tstring& filepath )
	{
		//============== Load DesignParameters from CSV ============//
		DesignParamArray designParams, designParamsT;
		LoadDesignParams( designParams, filepath );


		//=================== Filter by type ======================//
		FilterDesignParamsByType<T>( designParamsT, designParams );


		//==================== Init Chromsomes ===================//
		if( !designParamsT )
			return false;

		return chromosome.Init( designParamsT );
	}



}// end of namespace


#endif // !EALIB_DATA_IO_H
