#ifndef _PFE_COMPLEX_EXT_H
#define _PFE_COMPLEX_EXT_H 1105095033
/* generated 2005-0107-1150 ../../../pfe/../mk/Make-H.pl ../../../pfe/complex-ext.c */

#include <pfe/pfe-ext.h>

/** 
 *  -- Complex Arithmetic Word Set
 *
 *     Version 0.8.4
 *
 *     Code derived from the original source:
 *     Copyright (C) 1998 Julian V. Noble.
 *     This copyright notice must be preserved.
 * 
 *     Code derived from pfe:
 *     Copyright (C) 1998-2002 Tektronix, Inc.
 *
 *     Code not derived from the above:
 *     Copyright (C) 2002-2004 David N. Williams
 *
 *  @see     GNU LGPL
 *  @author  Julian V. Noble         @(#) %derived_by: guidod %
 *  @version %version: 33.24 %
 *    (%date_modified: Sun Nov 28 08:07:00 2004 %)
 *
 *  @description
 *         This is a port of Julian Noble's complex arithmetic
 *         lexicon to pfe.  There are a few differences, but his
 *         word set is pretty much intact.
 *
 *         This implementation uses the pfe default type for
 *         floats (double), and requires the pfe floating
 *         module.
 *
 *         In particular, it assumes a separate stack for
 *         floats.
 *
 *         It does not construct a separate complex number
 *         stack.
 *
 *         Complex numbers x+iy are stored on the fp stack as
 *         (f: -- x y).
 *
 *         Angles are in radians.
 *
 *         Our code uses higher-accuracy algorithms by William
 *         Kahan [1]. It uses the principal argument, with -pi <
 *         arg <= pi, for ARG, ZSQRT, ZLN, and Z^.  The Kahan
 *         alorithms implement an OpenMath compliant treatment
 *         of principal expressions, branch cuts, and branches
 *         for the elementary functions [2], which is based on
 *         Abramowitz and Stegun [3].
 *
 *         Kahan pays attention to signed zero, where available
 *         in IEEE 754/854 implementations.  We address that in
 *         this file as follows.
 *
 *         1. ZSINH, ZASINH, ZTANH, ZATANH, ZSIN, ZASIN, ZTAN,
 *            and ZATAN conserve the sign of zero, and Z*F, F*Z,
 *            1/Z, F/Z, and Z/F do the right thing.
 *
 *         2. We would like the analytic functions that are real
 *            and analytic on the real axis to do the right
 *            thing for the sign of the zero imaginary part. 
 *            This is not completely tested yet, and we're not
 *            sure it's always practical to implement.
 *
 *         3. For functions having branch cuts, signed zero in
 *            the appropriate x or y input produces correct
 *            values on the cuts.  This is probably the most
 *            important concern.
 *
 *         Kahan also uses IEEE-prescribed exception handling to
 *         avoid spurious overflow, underflow, and divide by
 *         zero signals.  We usually include that.
 */

#ifdef __cplusplus
extern "C" {
#endif


/* 19.26 decimal digits is enough for IEEE 754 80-bit extended
   double; Sparc requires 36 */
# ifndef  P4_PI
#  define P4_PI              3.1415926535897932384626433832795028842
# endif
# ifndef  P4_PI_OVER_2
#  define P4_PI_OVER_2       1.5707963267948966192313216916397514421
# endif
# ifndef  P4_1_OVER_SQRT_2
#  define P4_1_OVER_SQRT_2  0.70710678118654752440084436210484903928
# endif
# ifndef  P4_LN_2
#  define P4_LN_2           0.69314718055994530941723212145817656808
# endif
# ifndef  P4_SQRT_2
#  define P4_SQRT_2          1.4142135623730950488016887242096980786
# endif



/** Z@  ( addr --  f: -- z )
 */
extern P4_CODE (p4_z_fetch);

/** Z!  ( addr --  f: z -- )
 */
extern P4_CODE (p4_z_store);

/** X@  ( zaddr --  f: -- Re[z] )
 */
extern P4_CODE (p4_x_fetch);

/** X!  ( zaddr --  f: x -- )
 */
extern P4_CODE (p4_x_store);

/** Y@  ( zaddr --  f: -- Re[z] )
 */
extern P4_CODE (p4_y_fetch);

/** Y!  ( zaddr --  f: x -- )
 */
extern P4_CODE (p4_y_store);

/** Z.  (f: z -- )
 * Emit the complex number, including the sign of zero when
 * signbit() is available.
 */
extern P4_CODE (p4_z_dot);

/** ZS.  (f: z -- )
 * Emit the complex number in scientific notation, including the
 * sign of zero when signbit() is available.
 */
extern P4_CODE (p4_z_s_dot);

/** Z=0  (f: -- 0 0 )
 */
extern P4_CODE (p4_z_equals_0);

/** Z=1  (f: -- 1 0 )
 */
extern P4_CODE (p4_z_equals_1);

/** Z=I  (f: -- 0 1 )
 */
extern P4_CODE (p4_z_equals_i);

/** Z=-1  (f: -- -1 0 )
 */
extern P4_CODE (p4_z_equals_minus_1);

/** Z=-I  (f: -- 0 -1 )
 */
extern P4_CODE (p4_z_equals_minus_i);

/** REAL  (f: x y -- x )
 */
extern P4_CODE (p4_real);

/** IMAG  (f: x y -- y )
 */
extern P4_CODE (p4_imag);

/** CONJG  (f: x y -- x -y )
 */
extern P4_CODE (p4_conjg);

/** ZDROP  (f: z -- )
 */
extern P4_CODE (p4_z_drop);

/** ZDUP  (f: z -- z z )
 */
extern P4_CODE (p4_z_dup);

/** ZSWAP  (f: z1 z2 -- z2 z1 )
 */
extern P4_CODE (p4_z_swap);

/** ZOVER  (f: z1 z2 -- z1 z2 z1 )
 */
extern P4_CODE (p4_z_over);

/** ZNIP  (f: z1 z2 -- z2 )
 */
extern P4_CODE (p4_z_nip);

/** ZTUCK  (f: z1 z2 -- z2 z1 z2 )
 */
extern P4_CODE (p4_z_tuck);

/** ZROT  (f: z1 z2 z3 -- z2 z3 z1 )
 */
extern P4_CODE (p4_z_rot);

/** -ZROT  (f: z1 z2 z3 -- z3 z1 z2 )
 */
extern P4_CODE (p4_z_minus_rot);

/** Z*F  (f: x y f -- x*f y*f )
 */
extern P4_CODE (p4_z_star_f);

/** Z/F  (f: x y f -- x/f y/f )
 */
extern P4_CODE (p4_z_over_f);

/** 1/Z  (f: z -- 1/z )
 * Kahan algorithm *without* due attention to spurious
 * over/underflows and zeros and infinities.
 */
extern P4_CODE (p4_one_over_z);

/** F/Z  (f: f z -- f/z )
 * Kahan algorithm *without* due attention to spurious
 * over/underflows and zeros and infinities.
 */
extern P4_CODE (p4_f_over_z);

/** Z*  (f: x y u v -- x*u-y*v  x*v+y*u )
 * JVN uses the algorithm
 *     (x+iy)*(u+iv) = [(x+y)*u - y*(u+v)] + i[(x+y)*u + x*(v-u)]
 * requiring 3 multiplications and 5 additions.  We'll compare it
 * to the straightforward calculation.
 */
extern P4_CODE (p4_z_star);

/** Z*REAL  (f: z1 z2 -- Re[z1*z2] )
 * Compute the real part of the complex product without
 * computing the imaginary part.  Recommended by Kahan to avoid
 * gratuitous overflow or underflow signals from the unnecessary
 * part.
 */
extern P4_CODE (p4_z_star_real);

/** Z*IMAG  (f: z1 z2 -- Im[z1*z2] )
 * Compute the imaginary part of the complex product without
 * computing the real part.
 */
extern P4_CODE (p4_z_star_imag);

/** Z/  (f: u+iv z -- u/z+iv/z )
 * Kahan-like algorithm *without* due attention to spurious
 * over/underflows and zeros and infinities.
 */
extern P4_CODE (p4_z_slash);

/** Z+  (f: z1 z2 -- z1+z2 )
 */
extern P4_CODE (p4_z_plus);

/** Z-  (f: z1 z2 -- z1-z2 )
 */
extern P4_CODE (p4_z_minus);

/** X+  (f: z a -- x+a y )
 */
extern P4_CODE (p4_x_plus);

/** X-  (f: z a -- x-a y )
 */
extern P4_CODE (p4_x_minus);

/** Y+  (f: z a -- x y+a )
 */
extern P4_CODE (p4_y_plus);

/** Y-  (f: z a -- x y-a )
 */
extern P4_CODE (p4_y_minus);

/** ZNEGATE  (f: z -- -z )
 */
extern P4_CODE (p4_z_negate);

/** Z2/  (f: z -- z/2 )
 */
extern P4_CODE (p4_z_two_slash);

/** Z2*  (f: z -- z*2 )
 */
extern P4_CODE (p4_z_two_star);

/** I*  (f: x y -- -y x )
 */
extern P4_CODE (p4_i_star);

/** (-I)*  (f: x y -- y -x )
 */
extern P4_CODE (p4_parens_i_star);

/** Z^2  (f: z -- z^2 )
 * Kahan algorithm without removal of any spurious NaN created
 * by overflow.  It deliberately uses (x-y)(x+y) instead of
 * x^2-y^2 for the real part.
 */
extern P4_CODE (p4_z_hat_two);

/** |Z|^2  (f: x y -- |z|^2 )
 */
extern P4_CODE (p4_z_abs_hat_two);

/** Z^N  ( n f: z -- z^n )
 */
extern P4_CODE (p4_z_hat_n);

/** |Z|  (f: x y -- |z| )
 */
extern P4_CODE (p4_z_abs);

/** ZBOX  (f: z -- box[z] )
 * Defined *only* for zero and infinite arguments.  Kahan, p. 198.
 */
extern P4_CODE (p4_z_box);

/** ARG  (f: z -- principal.arg[z] )
 */
extern P4_CODE (p4_arg);

/** >POLAR  (f: x y -- r theta )
 * Convert the complex number z to its polar representation,
 * where theta is the principal argument.
 */
extern P4_CODE (p4_to_polar);

/** POLAR>  (f: r theta -- x y )
 */
extern P4_CODE (p4_polar_from);

/** ZSSQS  (f: z -- rho s: k )
 * Compute rho = |(x+iy)/2^k|^2, scaled to avoid overflow or
 * underflow, and leave the scaling integer k.  Kahan, p. 200.
 */
extern P4_CODE (p4_z_ssqs);

/** ZSQRT  (f: z -- sqrt[z] )
 * Compute the principal branch of the square root, with
 * Re sqrt[z] >= 0.  Kahan, p. 201.
 */
extern P4_CODE (p4_z_sqrt);

/** ZLN  (f: z -- ln|z|+i*theta )
 * Compute the principal branch of the complex natural
 * logarithm. The angle theta is the principal argument.  This
 * code uses Kahan's algorithm for the scaled logarithm
 * CLOGS(z,J) = ln(z*2^J), with J=0 and blind choices of the
 * threshholds T0, T1, and T2.  Namely, T0 = 1/sqrt(2), T1 =
 * 5/4, and T2 = ln(2);
 */
extern P4_CODE (p4_z_ln);

/** ZEXP  (f: z -- exp[z] )
 */
extern P4_CODE (p4_z_exp);

/** Z^  (f: x y u v -- [x+iy]^[u+iv] )
 * Compute in terms of the principal argument of x+iy.
 */
extern P4_CODE (p4_z_hat);

/** ZCOSH  (f: z -- cosh[z] )
 */
extern P4_CODE (p4_z_cosh);

/** ZSINH  (f: z -- sinh[z] )
 */
extern P4_CODE (p4_z_sinh);

/** ZTANH  (f: z -- tanh[z] )
 * Kahan, p. 204, including his divide by zero signal
 * suppression for infinite values of tan().  To quote the very
 * informative "man math" on our Darwin system about IEEE 754:
 * "Divide-by-Zero is signaled only when a function takes
 * exactly infinite values at finite operands."
 */
extern P4_CODE (p4_z_tanh);

/** ZCOTH  (f: z -- 1/tanh[z] )
 */
extern P4_CODE (p4_z_coth);

/** ZCOS  (f: z -- cosh[i*z] )
 */
extern P4_CODE (p4_z_cos);

/** ZSIN  (f: z -- -i*sinh[i*z] )
 */
extern P4_CODE (p4_z_sin);

/** ZTAN  (f: z -- -i*tanh[i*z] )
 */
extern P4_CODE (p4_z_tan);

/** ZCOT  (f: z -- -i*coth[-i*z] )
 */
extern P4_CODE (p4_z_cot);

/** ZACOS  (f: z -- u+iv=acos[z] )
 * Kahan, p.202.
 */
extern P4_CODE (p4_z_acos);

/** ZACOSH  (f: z -- u+iv=acosh[z] )
 * Kahan, p.203.
 */
extern P4_CODE (p4_z_acosh);

/** ZASIN  (f: z -- u+iv=asin[z] )
 * Kahan, p.203.
 */
extern P4_CODE (p4_z_asin);

/** ZASINH  (f: z -- -i*asin[i*z] )
 * Kahan, p. 203, couth.
 */
extern P4_CODE (p4_z_asinh);

/** ZATANH  (f: z -- u+iv=atanh[z] )
 * Kahan, p. 203.
 */
extern P4_CODE (p4_z_atanh);

/** ZATAN  (f: z -- -i*atanh[i*z] )
 * Kahan, p. 204, couth.
 */
extern P4_CODE (p4_z_atan);

extern P4_CODE (p4_z_constant_RT);

/** ZCONSTANT (f: x y "name" -- )
 * Create a word that contains x+iy in its body. The runtime
 * stack picture for name is (f: -- x y).
 */
extern P4_CODE (p4_z_constant);

extern P4_CODE (p4_z_literal_execution);

extern P4_CODE (p4_z_literal);

extern P4_CODE (p4_z_variable_RT);

extern P4_CODE (p4_z_variable);

_extern  double p4_real_of_one_over_z (double x, double y) ; /*{*/

_extern  double p4_imag_of_one_over_z (double x, double y) ; /*{*/

_extern  double p4_real_of_z_star (double x1, double y1, double x2, double y2) ; /*{*/

_extern  double p4_imag_of_z_star (double x1, double y1, double x2, double y2) ; /*{*/

_extern  double p4_carg (double x, double y) ; /*{*/

_extern  double p4_cssqs ( double x, double y, int *k) ; /*{*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif