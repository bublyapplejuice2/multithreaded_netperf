#include <math.h>

#include "estimate.h"

#define t95 [CONFID_LEVEL_95]
#define t99 [CONFID_LEVEL_99]

static double crit_values[][__CONFID_LEVEL_CNT] = {
	[1]	= { t95 = 12.706,	t99 = 63.657 },
	[2]	= { t95 =  4.303,	t99 = 9.925 },
	[3]	= { t95 =  3.182,	t99 = 5.841 },
	[4]	= { t95 =  2.776,	t99 = 4.604 },
	[5]	= { t95 =  2.571,	t99 = 4.032 },
	[6]	= { t95 =  2.447,	t99 = 3.707 },
	[7]	= { t95 =  2.365,	t99 = 3.499 },
	[8]	= { t95 =  2.306,	t99 = 3.355 },
	[9]	= { t95 =  2.262,	t99 = 3.250 },
	[10]	= { t95 =  2.228,	t99 = 3.169 },
	[11]	= { t95 =  2.201,	t99 = 3.106 },
	[12]	= { t95 =  2.179,	t99 = 3.055 },
	[13]	= { t95 =  2.160,	t99 = 3.012 },
	[14]	= { t95 =  2.145,	t99 = 2.977 },
	[15]	= { t95 =  2.131,	t99 = 2.947 },
	[16]	= { t95 =  2.120,	t99 = 2.921 },
	[17]	= { t95 =  2.110,	t99 = 2.898 },
	[18]	= { t95 =  2.101,	t99 = 2.878 },
	[19]	= { t95 =  2.093,	t99 = 2.861 },
	[20]	= { t95 =  2.086,	t99 = 2.845 },
	[21]	= { t95 =  2.080,	t99 = 2.831 },
	[22]	= { t95 =  2.074,	t99 = 2.819 },
	[23]	= { t95 =  2.069,	t99 = 2.807 },
	[24]	= { t95 =  2.064,	t99 = 2.797 },
	[25]	= { t95 =  2.060,	t99 = 2.787 },
	[26]	= { t95 =  2.056,	t99 = 2.779 },
	[27]	= { t95 =  2.052,	t99 = 2.771 },
	[28]	= { t95 =  2.048,	t99 = 2.763 },
	[29]	= { t95 =  2.045,	t99 = 2.756 },
	[30]	= { t95 =  2.042,	t99 = 2.750 },
	[31]	= { t95 =  2.040,	t99 = 2.744 },
	[32]	= { t95 =  2.037,	t99 = 2.738 },
	[33]	= { t95 =  2.035,	t99 = 2.733 },
	[34]	= { t95 =  2.032,	t99 = 2.728 },
	[35]	= { t95 =  2.030,	t99 = 2.724 },
	[36]	= { t95 =  2.028,	t99 = 2.719 },
	[37]	= { t95 =  2.026,	t99 = 2.715 },
	[38]	= { t95 =  2.024,	t99 = 2.712 },
	[39]	= { t95 =  2.023,	t99 = 2.708 },
	[40]	= { t95 =  2.021,	t99 = 2.704 },
	[41]	= { t95 =  2.020,	t99 = 2.701 },
	[42]	= { t95 =  2.018,	t99 = 2.698 },
	[43]	= { t95 =  2.017,	t99 = 2.695 },
	[44]	= { t95 =  2.015,	t99 = 2.692 },
	[45]	= { t95 =  2.014,	t99 = 2.690 },
	[46]	= { t95 =  2.013,	t99 = 2.687 },
	[47]	= { t95 =  2.012,	t99 = 2.685 },
	[48]	= { t95 =  2.011,	t99 = 2.682 },
	[49]	= { t95 =  2.010,	t99 = 2.680 },
	[50]	= { t95 =  2.009,	t99 = 2.678 },
	[51]	= { t95 =  2.008,	t99 = 2.676 },
	[52]	= { t95 =  2.007,	t99 = 2.674 },
	[53]	= { t95 =  2.006,	t99 = 2.672 },
	[54]	= { t95 =  2.005,	t99 = 2.670 },
	[55]	= { t95 =  2.004,	t99 = 2.668 },
	[56]	= { t95 =  2.003,	t99 = 2.667 },
	[57]	= { t95 =  2.002,	t99 = 2.665 },
	[58]	= { t95 =  2.002,	t99 = 2.663 },
	[59]	= { t95 =  2.001,	t99 = 2.662 },
	[60]	= { t95 =  2.000,	t99 = 2.660 },
	[61]	= { t95 =  2.000,	t99 = 2.659 },
	[62]	= { t95 =  1.999,	t99 = 2.657 },
	[63]	= { t95 =  1.998,	t99 = 2.656 },
	[64]	= { t95 =  1.998,	t99 = 2.655 },
	[65]	= { t95 =  1.997,	t99 = 2.654 },
	[66]	= { t95 =  1.997,	t99 = 2.652 },
	[67]	= { t95 =  1.996,	t99 = 2.651 },
	[68]	= { t95 =  1.995,	t99 = 2.650 },
	[69]	= { t95 =  1.995,	t99 = 2.649 },
	[70]	= { t95 =  1.994,	t99 = 2.648 },
	[71]	= { t95 =  1.994,	t99 = 2.647 },
	[72]	= { t95 =  1.993,	t99 = 2.646 },
	[73]	= { t95 =  1.993,	t99 = 2.645 },
	[74]	= { t95 =  1.993,	t99 = 2.644 },
	[75]	= { t95 =  1.992,	t99 = 2.643 },
	[76]	= { t95 =  1.992,	t99 = 2.642 },
	[77]	= { t95 =  1.991,	t99 = 2.641 },
	[78]	= { t95 =  1.991,	t99 = 2.640 },
	[79]	= { t95 =  1.990,	t99 = 2.640 },
	[80]	= { t95 =  1.990,	t99 = 2.639 },
	[81]	= { t95 =  1.990,	t99 = 2.638 },
	[82]	= { t95 =  1.989,	t99 = 2.637 },
	[83]	= { t95 =  1.989,	t99 = 2.636 },
	[84]	= { t95 =  1.989,	t99 = 2.636 },
	[85]	= { t95 =  1.988,	t99 = 2.635 },
	[86]	= { t95 =  1.988,	t99 = 2.634 },
	[87]	= { t95 =  1.988,	t99 = 2.634 },
	[88]	= { t95 =  1.987,	t99 = 2.633 },
	[89]	= { t95 =  1.987,	t99 = 2.632 },
	[90]	= { t95 =  1.987,	t99 = 2.632 },
	[91]	= { t95 =  1.986,	t99 = 2.631 },
	[92]	= { t95 =  1.986,	t99 = 2.630 },
	[93]	= { t95 =  1.986,	t99 = 2.630 },
	[94]	= { t95 =  1.986,	t99 = 2.629 },
	[95]	= { t95 =  1.985,	t99 = 2.629 },
	[96]	= { t95 =  1.985,	t99 = 2.628 },
	[97]	= { t95 =  1.985,	t99 = 2.627 },
	[98]	= { t95 =  1.985,	t99 = 2.627 },
	[99]	= { t95 =  1.984,	t99 = 2.627 },
	[100]	= { t95 =  1.984,	t99 = 2.626 },
};

struct asympt_coeffs {
	double	c0;
	double	c1;
	double	c2;
};

static struct asympt_coeffs tval_coeffs[__CONFID_LEVEL_CNT] = {
	[CONFID_LEVEL_95] = {
		.c0	= 1.959963984540,
		.c1	= 2.371836,
		.c2	= 3.090131,
	},
	[CONFID_LEVEL_99] = {
		.c0	= 2.575829303549,
		.c1	= 4.912150,
		.c2	= 9.623527,
	},
};

/* Bessel corrected standard deviation */
double sdev_n1(double sum, double sum_sqr, unsigned int n)
{
	return sqrt((sum_sqr - sum * sum / n) / (n - 1));
}

/* Return critical value of Student distribution for given number of freedom
 * degrees and confidence level. For smaller nu a table of known values is
 * used, for larger nu, an asymptotic c0 + c1/n + c2/n^2 where c0 is the
 * z value (i.e. limit at infinity).
 */
static double tval(unsigned int nu, enum confid_level level)
{
	if (nu < sizeof(crit_values) / sizeof(crit_values[0]))
		return crit_values[nu][level];

	return tval_coeffs[level].c0 + tval_coeffs[level].c1 / nu +
	       tval_coeffs[level].c2 / (nu * nu);
}

/* half width of the confidence interfal */
double confid_interval(double sum, double sum_sqr, unsigned int n,
		       enum confid_level level)
{
	return sdev_n1(sum, sum_sqr, n) * tval(n - 1, level) / sqrt(n);
}