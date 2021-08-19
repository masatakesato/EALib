#ifndef DATA_SIZES_H
#define	DATA_SIZES_H


#include	<oreore/meta/TypeList.h>


namespace ealib
{

	template < typename TList >
	class DataSizes
	{
	public:

		DataSizes()
		{
			Init<TList>::Execute( (size_t *)&m_Sizes[0] );
		}


		size_t Size( int type ) const
		{
			return m_Sizes[ type ];
		}



	private:

		size_t m_Sizes[ OreOreLib::TypeList::Length<TList>::value ];


		template < typename TList >
		struct Init
		{
			static void Execute( size_t sizes[], int i=0 )
			{
				sizes[i] = sizeof( typename TList::head );
				Init< typename TList::tail >::Execute( sizes, i+1 );
			}
		};


		// typelist end
		template <>
		struct Init< OreOreLib::TypeList::NullType  >
		{
			static void Execute( size_t sizes[], int i ){}// Do nothing
		};



	};






}



#endif // !DATA_SIZES_H










