#ifndef PSO_ATTRIBUTE_H
#define	PSO_ATTRIBUTE_H


namespace ealib
{

	struct PSOAttribute
	{
		float W;	// inertia weight
		float C1;	// acceleration coefficients1
		float C2;	// acceleration coefficients2
	
		void Clear()
		{
			W	= 1.0f;
			C1	= 0.0f;
			C2	= 0.0f;
		}
	};


}// end of namespace

#endif // !PSO_ATTRIBUTE_H
