/*

	2007-10-15 first version

*/
//#include "..\common.h"

#ifndef MOSEGEANALYSIS_INCLUDED
#define MOSEGANALYSIS_INCLUDED

const int NPOINTS=256;
const int NPOINTS2=128;
const int NTAPER=32; /* number of tapered samples, one side, must be dividable by 4 */
const int NSPECTRA=14;	/* number of spectra */
const float PI=(float)3.1415926535898; /* (float)(4.0*atan(1.0)); */
const float FS=2.0F;
const int TIMEINTERVAL=1800; /* in seconds */

void MoseGAnalyseSpectrum(float *displacements, unsigned char flag, int i, WAVE_DATA_COMMON *result);	//, char *cspc, char *cpeak, char *cmom);
void MoseGAnalyseTimeseries(float *heave, unsigned char *flag, int n, WAVE_DATA_COMMON *result, BOOL type);	//, char *cheight, char* cperiod);

#endif