/*
 #  
 #  File        : CImg.h
 #  
 #  Description : The C++ Template Image Processing Library
 #                ( http://cimg.sourceforge.net )
 #
 #  Copyright   : David Tschumperle
 #                ( http://www.greyc.ensicaen.fr/~dtschump/ )
 #   
 #  This software is governed by the CeCILL-C license under French law and
 #  abiding by the rules of distribution of free software.  You can  use, 
 #  modify and or redistribute the software under the terms of the CeCILL-C
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info". 
 #  
 #  As a counterpart to the access to the source code and  rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty  and the software's author,  the holder of the
 #  economic rights,  and the successive licensors  have only  limited
 #  liability. 
 #  
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading,  using,  modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean  that it is complicated to manipulate,  and  that  also
 #  therefore means  that it is reserved for developers  and  experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or 
 #  data to be ensured and,  more generally, to use and operate it in the 
 #  same conditions as regards security. 
 #  
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL-C license and that you accept its terms.
 #  
 */

#ifndef cimg_version
#define cimg_version 1.15

// Avoid strange warning messages on Visual C++ express 2005.
#if ( defined(_MSC_VER) && _MSC_VER>=1400 )
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif

// Standard C++ includes
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <ctime>

// Overcome VisualC++ 6.0 and DMC compilers namespace 'std::' bug
#if ( defined(_MSC_VER) && _MSC_VER<=1200 ) || defined(__DMC__)
#define std
#endif

/*
 #
 # Set CImg configuration flags.
 #   
 # If compilation flags are not adapted to your system,
 # you may override their values, before including
 # the header file "CImg.h" (use the #define directive).
 # 
 */

// Try to detect the current system and set value of 'cimg_OS'.
#ifndef cimg_OS
#if defined(sun)         || defined(__sun)      || defined(linux)       || defined(__linux) \
 || defined(__linux__)   || defined(__CYGWIN__) || defined(BSD)         || defined(__FreeBSD__) \
 || defined(__OPENBSD__) || defined(__MACOSX__) || defined(__APPLE__)   || defined(sgi) \
 || defined(__sgi)
// Unix-like (Linux, Solaris, BSD, Irix,...)
#define cimg_OS            1
#ifndef cimg_display_type
#define cimg_display_type  1
#endif
#ifndef cimg_color_terminal
#define cimg_color_terminal
#endif
#elif defined(_WIN32) || defined(__WIN32__)
// Windows
#define cimg_OS            2
#ifndef cimg_display_type
#define cimg_display_type  2
#endif
#else
// Unknown configuration : will compile with minimal dependencies.
#define cimg_OS            0
#ifndef cimg_display_type
#define cimg_display_type  0
#endif
#endif
#endif

// Debug configuration.
//
// Define 'cimg_debug' to : 0 to remove dynamic debug messages (exceptions are still thrown)
//                          1 to display dynamic debug messages (default behavior).
//                          2 to add memory access controls (may slow down the code, but display extra warning messages)
#ifndef cimg_debug
#define cimg_debug         1
#endif

// Architecture-dependent includes.
#if cimg_OS==1
#include <sys/time.h>
#include <unistd.h>
#elif cimg_OS==2
#include <windows.h>
// Discard unuseful macros in windows.h
// to allow compilation with VC++ 6.0.
#ifdef min
#undef min
#undef max
#undef abs
#endif
#endif
#if cimg_display_type==1
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <pthread.h>
#ifdef cimg_use_xshm
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif
#ifdef cimg_use_xrandr
#include <X11/extensions/Xrandr.h>
#endif
#endif

// Configuration for native PNG and JPEG support
// Define 'cimg_use_png' or 'cimg_use_jpeg' to enable native PNG or JPEG files support.
// This requires you link your code with the zlib/png or jpeg libraries.
// Without these libraries, PNG and JPEG support will be done by the Image Magick's 'convert' tool, if installed
// (this is the case on most unix plateforms).
#ifdef cimg_use_png
extern "C" {
#include "png.h"
}
#endif
#ifdef cimg_use_jpeg
extern "C" {
#include "jpeglib.h"
}
#endif
#ifdef cimg_use_magick
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Magick++.h>
#endif

/*
 #
 #
 # Define some useful macros. Macros of the CImg Library are prefixed by 'cimg_'
 # Documented macros below may be safely used in your own code
 # (particularly useful for option parsing, image loops and neighborhoods).
 #
 #
 */

// Macros used to describe the program usage, and retrieve command line arguments
// (See corresponding module 'Retrieving command line arguments' in the generated documentation).
#define cimg_usage(usage) cimg_library::cimg::option((const char*)0,argc,argv,(const char*)0,usage)
#define cimg_option(name,defaut,usage) cimg_library::cimg::option(name,argc,argv,defaut,usage)
  
// Macros used for neighborhood definitions and manipulations.
// (see module 'Using Image Loops' in the generated documentation).
#define CImg_2x2x1(I,T) T I##cc,I##nc=0,I##cn,I##nn=0
#define CImg_3x1x1(I,T) T I##pp,I##cp,I##np=0
#define CImg_3x3x1(I,T) T I##pp,I##cp,I##np=0,I##pc,I##cc,I##nc=0,I##pn,I##cn,I##nn=0
#define CImg_4x1x1(I,T) T I##pp,I##cp,I##np=0,I##ap=0
#define CImg_4x4x1(I,T) T I##pp,I##cp,I##np=0,I##ap=0, \
						  I##pc,I##cc,I##nc=0,I##ac=0, \
						  I##pn,I##cn,I##nn=0,I##an=0, \
						  I##pa,I##ca,I##na=0,I##aa=0
#define CImg_5x1x1(I,T) T I##bb,I##pb,I##cb,I##nb=0,I##ab=0
#define CImg_5x5x1(I,T) T I##bb,I##pb,I##cb,I##nb=0,I##ab=0, \
						  I##bp,I##pp,I##cp,I##np=0,I##ap=0, \
						  I##bc,I##pc,I##cc,I##nc=0,I##ac=0, \
						  I##bn,I##pn,I##cn,I##nn=0,I##an=0, \
						  I##ba,I##pa,I##ca,I##na=0,I##aa=0
#define CImg_2x2x2(I,T) T I##ccc,I##ncc=0,I##cnc,I##nnc=0, \
						  I##ccn,I##ncn=0,I##cnn,I##nnn=0
#define CImg_3x3x3(I,T) T I##ppp,I##cpp,I##npp=0,I##pcp,I##ccp,I##ncp=0,I##pnp,I##cnp,I##nnp=0, \
						  I##ppc,I##cpc,I##npc=0,I##pcc,I##ccc,I##ncc=0,I##pnc,I##cnc,I##nnc=0, \
						  I##ppn,I##cpn,I##npn=0,I##pcn,I##ccn,I##ncn=0,I##pnn,I##cnn,I##nnn=0

#define CImg_2x2x1_ref(I,T,tab) T &I##cc=(tab)[0],&I##nc=(tab)[1],&I##cn=(tab)[2],&I##nn=(tab)[3]
#define CImg_3x3x1_ref(I,T,tab) T &I##pp=(tab)[0],&I##cp=(tab)[1],&I##np=(tab)[2], \
								  &I##pc=(tab)[3],&I##cc=(tab)[4],&I##nc=(tab)[5], \
								  &I##pn=(tab)[6],&I##cn=(tab)[7],&I##nn=(tab)[8]
#define CImg_4x4x1_ref(I,T,tab) T &I##pp=(tab)[0],&I##cp=(tab)[1],&I##np=(tab)[2],&I##ap=(tab)[3], \
								  &I##pc=(tab)[4],&I##cc=(tab)[5],&I##nc=(tab)[6],&I##ap=(tab)[7], \
								  &I##pn=(tab)[8],&I##cn=(tab)[9],&I##nn=(tab)[10],&I##aa=(tab)[11], \
								  &I##pa=(tab)[12],&I##ca=(tab)[13],&I##na=(tab)[14],&I##aa=(tab)[15]
#define CImg_5x5x1_ref(I,T,tab) T &I##bb=(tab)[0],&I##pb=(tab)[1],&I##cb=(tab)[2],&I##nb=(tab)[3],&I##ab=(tab)[4], \
								  &I##bp=(tab)[5],&I##pp=(tab)[6],&I##cp=(tab)[7],&I##np=(tab)[8],&I##ap=(tab)[9], \
								  &I##bc=(tab)[10],&I##pc=(tab)[11],&I##cc=(tab)[12],&I##nc=(tab)[13],&I##ac=(tab)[14], \
								  &I##bn=(tab)[15],&I##pn=(tab)[16],&I##cn=(tab)[17],&I##nn=(tab)[18],&I##an=(tab)[19], \
								  &I##ba=(tab)[20],&I##pa=(tab)[21],&I##ca=(tab)[22],&I##na=(tab)[23],&I##aa=(tab)[24]
#define CImg_2x2x2_ref(I,T,tab) T &I##ccc=(tab)[0],&I##ncc=(tab)[1],&I##cnc=(tab)[2],&I##nnc=(tab)[3], \
								  &I##ccn=(tab)[4],&I##ncn=(tab)[5],&I##cnn=(tab)[6],&I##nnn=(tab)[7]
#define CImg_3x3x3_ref(I,T,tab) T &I##ppp=(tab)[0],&I##cpp=(tab)[1],&I##npp=(tab)[2], \
								  &I##pcp=(tab)[3],&I##ccp=(tab)[4],&I##ncp=(tab)[5], \
								  &I##pnp=(tab)[6],&I##cnp=(tab)[7],&I##nnp=(tab)[8], \
								  &I##ppc=(tab)[9],&I##cpc=(tab)[10],&I##npc=(tab)[11], \
								  &I##pcc=(tab)[12],&I##ccc=(tab)[13],&I##ncc=(tab)[14], \
								  &I##pnc=(tab)[15],&I##cnc=(tab)[16],&I##nnc=(tab)[17], \
								  &I##ppn=(tab)[18],&I##cpn=(tab)[19],&I##npn=(tab)[20], \
								  &I##pcn=(tab)[21],&I##ccn=(tab)[22],&I##ncn=(tab)[23], \
								  &I##pnn=(tab)[24],&I##cnn=(tab)[25],&I##nnn=(tab)[26]

#define cimg_copy2x2x1(J,I) I##cc=J##cc, I##nc=J##nc, I##cn=J##cn, I##nn=J##nn
#define cimg_copy3x3x1(J,I) I##pp=J##pp, I##cp=J##cp, I##np=J##np, \
							I##pc=J##pc, I##cc=J##cc, I##nc=J##nc, \
							I##pn=J##pn, I##cn=J##cn, I##nn=J##nn
#define cimg_copy5x5x1(J,I) I##bb=J##bb, I##pb=J##pb, I##cb=J##cb, I##nb=J##nb, I##ab=J##ab, \
							I##bp=J##bp, I##pp=J##pp, I##cp=J##cp, I##np=J##np, I##ap=J##ap, \
							I##bc=J##bc, I##pc=J##pc, I##cc=J##cc, I##nc=J##nc, I##ac=J##ac, \
							I##bn=J##bn, I##pn=J##pn, I##cn=J##cn, I##nn=J##nn, I##an=J##an, \
							I##ba=J##ba, I##pa=J##pa, I##ca=J##ca, I##na=J##na, I##aa=J##aa

#define cimg_squaresum2x2x1(I) ( I##cc*I##cc + I##nc*I##nc + I##cn*I##cn + I##nn*I##nn )
#define cimg_squaresum3x3x1(I) ( I##pp*I##pp + I##cp*I##cp + I##np*I##np + \
								 I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + \
								 I##pn*I##pn + I##cn*I##cn + I##nn*I##nn )
#define cimg_squaresum4x4x1(I) ( I##pp*I##pp + I##cp*I##cp + I##np*I##np + I##ap*I##ap + \
								 I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + I##ac*I##ac + \
								 I##pn*I##pn + I##cn*I##cn + I##nn*I##nn + I##an*I##an + \
								 I##pa*I##pa + I##ca*I##ca + I##na*I##na + I##aa*I##aa )
#define cimg_squaresum5x5x1(I) ( I##bb*I##bb + I##pb*I##pb + I##cb*I##cb + I##nb*I##nb + I##ab*I##ab + \
								 I##bp*I##bp + I##pp*I##pp + I##cp*I##cp + I##np*I##np + I##ap*I##ap + \
								 I##bc*I##bc + I##pc*I##pc + I##cc*I##cc + I##nc*I##nc + I##ac*I##ac + \
								 I##bn*I##bn + I##pn*I##pn + I##cn*I##cn + I##nn*I##nn + I##an*I##an + \
								 I##ba*I##ba + I##pa*I##pa + I##ca*I##ca + I##na*I##na + I##aa*I##aa )
#define cimg_squaresum2x2x2(I) ( I##ccc*I##ccc + I##ncc*I##ncc + I##cnc*I##cnc + I##nnc*I##nnc + \
								 I##ccn*I##ccn + I##ncn*I##ncn + I##cnn*I##cnn + I##nnn*I##nnn )
#define cimg_squaresum3x3x3(I) ( I##ppp*I##ppp + I##cpp*I##cpp + I##npp*I##npp + \
								 I##pcp*I##pcp + I##ccp*I##ccp + I##ncp*I##ncp + \
								 I##pnp*I##pnp + I##cnp*I##cnp + I##nnp*I##nnp + \
								 I##ppc*I##ppc + I##cpc*I##cpc + I##npc*I##npc + \
								 I##pcc*I##pcc + I##ccc*I##ccc + I##ncc*I##ncc + \
								 I##pnc*I##pnc + I##cnc*I##cnc + I##nnc*I##nnc + \
								 I##ppn*I##ppn + I##cpn*I##cpn + I##npn*I##npn + \
								 I##pcn*I##pcn + I##ccn*I##ccn + I##ncn*I##ncn + \
								 I##pnn*I##pnn + I##cnn*I##cnn + I##nnn*I##nnn )

#define cimg_corr2x2x1(I,m) ( I##cc*(m)(0,0)+I##nc*(m)(1,0)+I##cn*(m)(0,1)+I##nn*(m)(1,1) )
#define cimg_corr3x3x1(I,m) ( I##pp*(m)(0,0)+I##cp*(m)(1,0)+I##np*(m)(2,0) + \
							  I##pc*(m)(0,1)+I##cc*(m)(1,1)+I##nc*(m)(2,1) + \
							  I##pn*(m)(0,2)+I##cn*(m)(1,2)+I##nn*(m)(2,2) )
#define cimg_corr4x4x1(I,m) ( I##pp*(m)(0,0)+I##cp*(m)(1,0)+I##np*(m)(2,0)+I##ap*(m)(3,0) + \
							  I##pc*(m)(0,1)+I##cc*(m)(1,1)+I##nc*(m)(2,1)+I##ac*(m)(3,1) + \
							  I##pn*(m)(0,2)+I##cn*(m)(1,2)+I##nn*(m)(2,2)+I##an*(m)(3,2) + \
							  I##pa*(m)(0,3)+I##ca*(m)(1,3)+I##na*(m)(2,3)+I##aa*(m)(3,3) )
#define cimg_corr5x5x1(I,m) ( I##bb*(m)(0,0)+I##pb*(m)(1,0)+I##cb*(m)(2,0)+I##nb*(m)(3,0)+I##ab*(m)(4,0) + \
							  I##bp*(m)(0,1)+I##pp*(m)(1,1)+I##cp*(m)(2,1)+I##np*(m)(3,1)+I##ap*(m)(4,1) + \
							  I##bc*(m)(0,2)+I##pc*(m)(1,2)+I##cc*(m)(2,2)+I##nc*(m)(3,2)+I##ac*(m)(4,2) + \
							  I##bn*(m)(0,3)+I##pn*(m)(1,3)+I##cn*(m)(2,3)+I##nn*(m)(3,3)+I##an*(m)(4,3) + \
							  I##ba*(m)(0,4)+I##pa*(m)(1,4)+I##ca*(m)(2,4)+I##na*(m)(3,4)+I##aa*(m)(4,4) )
#define cimg_corr2x2x2(I,m) ( I##ccc*(m)(0,0,0)+I##ncc*(m)(1,0,0)+I##cnc*(m)(0,1,0)+I##nnc*(m)(1,1,0) + \
							  I##ccn*(m)(0,0,1)+I##ncn*(m)(1,0,1)+I##cnn*(m)(0,1,1)+I##nnn*(m)(1,1,1) )
#define cimg_corr3x3x3(I,m) ( I##ppp*(m)(0,0,0)+I##cpp*(m)(1,0,0)+I##npp*(m)(2,0,0) + \
							  I##pcp*(m)(0,1,0)+I##ccp*(m)(1,1,0)+I##ncp*(m)(2,1,0) + \
							  I##pnp*(m)(0,2,0)+I##cnp*(m)(1,2,0)+I##nnp*(m)(2,2,0) + \
							  I##ppc*(m)(0,0,1)+I##cpc*(m)(1,0,1)+I##npc*(m)(2,0,1) + \
							  I##pcc*(m)(0,1,1)+I##ccc*(m)(1,1,1)+I##ncc*(m)(2,1,1) + \
							  I##pnc*(m)(0,2,1)+I##cnc*(m)(1,2,1)+I##nnc*(m)(2,2,1) + \
							  I##ppn*(m)(0,0,2)+I##cpn*(m)(1,0,2)+I##npn*(m)(2,0,2) + \
							  I##pcn*(m)(0,1,2)+I##ccn*(m)(1,1,2)+I##ncn*(m)(2,1,2) + \
							  I##pnn*(m)(0,2,2)+I##cnn*(m)(1,2,2)+I##nnn*(m)(2,2,2) )

#define cimg_conv2x2x1(I,m) ( I##cc*(m)(1,1)+I##nc*(m)(0,1)+I##cn*(m)(1,0)+I##nn*(m)(0,0) )
#define cimg_conv3x3x1(I,m) ( I##pp*(m)(2,2)+I##cp*(m)(1,2)+I##np*(m)(0,2) + \
							  I##pc*(m)(2,1)+I##cc*(m)(1,1)+I##nc*(m)(0,1) + \
							  I##pn*(m)(2,0)+I##cn*(m)(1,0)+I##nn*(m)(0,0) )
#define cimg_conv4x4x1(I,m) ( I##pp*(m)(3,3)+I##cp*(m)(2,3)+I##np*(m)(1,3)+I##ap*(m)(0,3) + \
							  I##pc*(m)(3,2)+I##cc*(m)(2,2)+I##nc*(m)(1,2)+I##ac*(m)(0,2) + \
							  I##pn*(m)(3,1)+I##cn*(m)(2,1)+I##nn*(m)(1,1)+I##an*(m)(0,1) + \
							  I##pa*(m)(3,0)+I##ca*(m)(2,0)+I##na*(m)(1,0)+I##aa*(m)(0,0) )
#define cimg_conv5x5x1(I,m) ( I##bb*(m)(4,4)+I##pb*(m)(3,4)+I##cb*(m)(2,4)+I##nb*(m)(1,4)+I##ab*(m)(0,4) + \
							  I##bp*(m)(4,3)+I##pp*(m)(3,3)+I##cp*(m)(2,3)+I##np*(m)(1,3)+I##ap*(m)(0,3) + \
							  I##bc*(m)(4,2)+I##pc*(m)(3,2)+I##cc*(m)(2,2)+I##nc*(m)(1,2)+I##ac*(m)(0,2) + \
							  I##bn*(m)(4,1)+I##pn*(m)(3,1)+I##cn*(m)(2,1)+I##nn*(m)(1,1)+I##an*(m)(0,1) + \
							  I##ba*(m)(4,0)+I##pa*(m)(3,0)+I##ca*(m)(2,0)+I##na*(m)(1,0)+I##aa*(m)(0,0) )
#define cimg_conv2x2x2(I,m) ( I##ccc*(m)(1,1,1)+I##ncc*(m)(0,1,1)+I##cnc*(m)(1,0,1)+I##nnc*(m)(0,0,1) + \
							  I##ccn*(m)(1,1,0)+I##ncn*(m)(0,1,0)+I##cnn*(m)(1,0,0)+I##nnn*(m)(0,0,0) )
#define cimg_conv3x3x3(I,m) ( I##ppp*(m)(2,2,2)+I##cpp*(m)(1,2,2)+I##npp*(m)(0,2,2) + \
							  I##pcp*(m)(2,1,2)+I##ccp*(m)(1,1,2)+I##ncp*(m)(0,1,2) + \
							  I##pnp*(m)(2,0,2)+I##cnp*(m)(1,0,2)+I##nnp*(m)(0,0,2) + \
							  I##ppc*(m)(2,2,1)+I##cpc*(m)(1,2,1)+I##npc*(m)(0,2,1) + \
							  I##pcc*(m)(2,1,1)+I##ccc*(m)(1,1,1)+I##ncc*(m)(0,1,1) + \
							  I##pnc*(m)(2,0,1)+I##cnc*(m)(1,0,1)+I##nnc*(m)(0,0,1) + \
							  I##ppn*(m)(2,2,0)+I##cpn*(m)(1,2,0)+I##npn*(m)(0,2,0) + \
							  I##pcn*(m)(2,1,0)+I##ccn*(m)(1,1,0)+I##ncn*(m)(0,1,0) + \
							  I##pnn*(m)(2,0,0)+I##cnn*(m)(1,0,0)+I##nnn*(m)(0,0,0) )

#define cimg_get2x2x1(img,x,y,z,v,I) \
   I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), \
   I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v)
#define cimg_get3x3x1(img,x,y,z,v,I) \
  I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), \
  I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), \
  I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v)
#define cimg_get4x4x1(img,x,y,z,v,I) \
  I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), I##ap=(img)(_a##x,_p##y,z,v), \
  I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), I##ac=(img)(_a##x,    y,z,v), \
  I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v), I##an=(img)(_a##x,_n##y,z,v), \
  I##pa=(img)(_p##x,_a##y,z,v), I##ca=(img)(x,_a##y,z,v), I##na=(img)(_n##x,_a##y,z,v), I##aa=(img)(_a##x,_a##y,z,v)
#define cimg_get5x5x1(img,x,y,z,v,I) \
  I##bb=(img)(_b##x,_b##y,z,v), I##pb=(img)(_p##x,_b##y,z,v), I##cb=(img)(x,_b##y,z,v), I##nb=(img)(_n##x,_b##y,z,v), I##ab=(img)(_a##x,_b##y,z,v), \
  I##bp=(img)(_b##x,_p##y,z,v), I##pp=(img)(_p##x,_p##y,z,v), I##cp=(img)(x,_p##y,z,v), I##np=(img)(_n##x,_p##y,z,v), I##ap=(img)(_a##x,_p##y,z,v), \
  I##bc=(img)(_b##x,    y,z,v), I##pc=(img)(_p##x,    y,z,v), I##cc=(img)(x,    y,z,v), I##nc=(img)(_n##x,    y,z,v), I##ac=(img)(_a##x,    y,z,v), \
  I##bn=(img)(_b##x,_n##y,z,v), I##pn=(img)(_p##x,_n##y,z,v), I##cn=(img)(x,_n##y,z,v), I##nn=(img)(_n##x,_n##y,z,v), I##an=(img)(_a##x,_n##y,z,v), \
  I##ba=(img)(_b##x,_a##y,z,v), I##pa=(img)(_p##x,_a##y,z,v), I##ca=(img)(x,_a##y,z,v), I##na=(img)(_n##x,_a##y,z,v), I##aa=(img)(_a##x,_a##y,z,v)
#define cimg_get2x2x2(img,x,y,z,v,I) \
  I##ccc=(img)(x,y,    z,v), I##ncc=(img)(_n##x,y,    z,v), I##cnc=(img)(x,_n##y,    z,v), I##nnc=(img)(_n##x,_n##y,    z,v), \
  I##ccc=(img)(x,y,_n##z,v), I##ncc=(img)(_n##x,y,_n##z,v), I##cnc=(img)(x,_n##y,_n##z,v), I##nnc=(img)(_n##x,_n##y,_n##z,v)
#define cimg_get3x3x3(img,x,y,z,v,I) \
  I##ppp=(img)(_p##x,_p##y,_p##z,v), I##cpp=(img)(x,_p##y,_p##z,v), I##npp=(img)(_n##x,_p##y,_p##z,v), \
  I##pcp=(img)(_p##x,    y,_p##z,v), I##ccp=(img)(x,    y,_p##z,v), I##ncp=(img)(_n##x,    y,_p##z,v), \
  I##pnp=(img)(_p##x,_n##y,_p##z,v), I##cnp=(img)(x,_n##y,_p##z,v), I##nnp=(img)(_n##x,_n##y,_p##z,v), \
  I##ppc=(img)(_p##x,_p##y,    z,v), I##cpc=(img)(x,_p##y,    z,v), I##npc=(img)(_n##x,_p##y,    z,v), \
  I##pcc=(img)(_p##x,    y,    z,v), I##ccc=(img)(x,    y,    z,v), I##ncc=(img)(_n##x,    y,    z,v), \
  I##pnc=(img)(_p##x,_n##y,    z,v), I##cnc=(img)(x,_n##y,    z,v), I##nnc=(img)(_n##x,_n##y,    z,v), \
  I##ppn=(img)(_p##x,_p##y,_n##z,v), I##cpn=(img)(x,_p##y,_n##z,v), I##npn=(img)(_n##x,_p##y,_n##z,v), \
  I##pcn=(img)(_p##x,    y,_n##z,v), I##ccn=(img)(x,    y,_n##z,v), I##ncn=(img)(_n##x,    y,_n##z,v), \
  I##pnn=(img)(_p##x,_n##y,_n##z,v), I##cnn=(img)(x,_n##y,_n##z,v), I##nnn=(img)(_n##x,_n##y,_n##z,v)

#define CImg_2x2(I,T) CImg_2x2x1(I,T)
#define CImg_3x3(I,T) CImg_3x3x1(I,T)
#define CImg_4x4(I,T) CImg_4x4x1(I,T)
#define CImg_5x5(I,T) CImg_5x5x1(I,T)
#define CImg_2x2_ref(I,T,tab) CImg_2x2x1_ref(I,T,tab)
#define CImg_3x3_ref(I,T,tab) CImg_3x3x1_ref(I,T,tab)
#define CImg_4x4_ref(I,T,tab) CImg_4x4x1_ref(I,T,tab)
#define CImg_5x5_ref(I,T,tab) CImg_5x5x1_ref(I,T,tab)
#define cimg_copy2x2(J,I) cimg_copy2x2x1(J,I)
#define cimg_copy3x3(J,I) cimg_copy3x3x1(J,I)
#define cimg_copy5x5(J,I) cimg_copy5x5x1(J,I)
#define cimg_squaresum2x2(I) cimg_squaresum2x2x1(I)
#define cimg_squaresum3x3(I) cimg_squaresum3x3x1(I)
#define cimg_squaresum4x4(I) cimg_squaresum4x4x1(I)
#define cimg_squaresum5x5(I) cimg_squaresum5x5x1(I)
#define cimg_corr2x2(I) cimg_corr2x2x1(I)
#define cimg_corr3x3(I) cimg_corr3x3x1(I)
#define cimg_corr4x4(I) cimg_corr4x4x1(I)
#define cimg_corr5x5(I) cimg_corr5x5x1(I)
#define cimg_conv2x2(I) cimg_conv2x2x1(I)
#define cimg_conv3x3(I) cimg_conv3x3x1(I)
#define cimg_conv4x4(I) cimg_conv4x4x1(I)
#define cimg_conv5x5(I) cimg_conv5x5x1(I)
#define cimg_get2x2(img,x,y,z,k,I) cimg_get2x2x1(img,x,y,z,k,I)
#define cimg_get3x3(img,x,y,z,k,I) cimg_get3x3x1(img,x,y,z,k,I)
#define cimg_get4x4(img,x,y,z,k,I) cimg_get4x4x1(img,x,y,z,k,I)
#define cimg_get5x5(img,x,y,z,k,I) cimg_get5x5x1(img,x,y,z,k,I)
#define cimg_map2x2(img,x,y,z,k,I) cimg_map2x2x1(img,x,y,z,k,I)
#define cimg_map3x3(img,x,y,z,k,I) cimg_map3x3x1(img,x,y,z,k,I)
#define cimg_map4x4(img,x,y,z,k,I) cimg_map4x4x1(img,x,y,z,k,I)
#define cimg_map5x5(img,x,y,z,k,I) cimg_map5x5x1(img,x,y,z,k,I)

// Macros used to define special image loops.
// (see module 'Using Image Loops' in the generated documentation).
#define cimg_map(img,ptr,T_ptr)   for (T_ptr *ptr=(img).data+(img).size(); (ptr--)>(img).data; )
#define cimgl_map(list,l)         for (unsigned int l=0; l<(list).size; l++)
#define cimg_mapoff(img,off)      for (unsigned int off=0; off<(img).size(); off++)
#define cimg_mapX(img,x)          for (int x=0; x<(int)((img).width); x++)
#define cimg_mapY(img,y)          for (int y=0; y<(int)((img).height);y++)
#define cimg_mapZ(img,z)          for (int z=0; z<(int)((img).depth); z++)
#define cimg_mapV(img,v)          for (int v=0; v<(int)((img).dim);   v++)
#define cimg_mapXY(img,x,y)       cimg_mapY(img,y) cimg_mapX(img,x)
#define cimg_mapXZ(img,x,z)       cimg_mapZ(img,z) cimg_mapX(img,x)
#define cimg_mapYZ(img,y,z)       cimg_mapZ(img,z) cimg_mapY(img,y)
#define cimg_mapXV(img,x,v)       cimg_mapV(img,v) cimg_mapX(img,x)
#define cimg_mapYV(img,y,v)       cimg_mapV(img,v) cimg_mapY(img,y)
#define cimg_mapZV(img,z,v)       cimg_mapV(img,v) cimg_mapZ(img,z)
#define cimg_mapXYZ(img,x,y,z)    cimg_mapZ(img,z) cimg_mapXY(img,x,y)
#define cimg_mapXYV(img,x,y,v)    cimg_mapV(img,v) cimg_mapXY(img,x,y)
#define cimg_mapXZV(img,x,z,v)    cimg_mapV(img,v) cimg_mapXZ(img,x,z)
#define cimg_mapYZV(img,y,z,v)    cimg_mapV(img,v) cimg_mapYZ(img,y,z)
#define cimg_mapXYZV(img,x,y,z,v) cimg_mapV(img,v) cimg_mapXYZ(img,x,y,z)
#define cimg_imapX(img,x,n)       for (int x=(n); x<(int)((img).width-(n)); x++)
#define cimg_imapY(img,y,n)       for (int y=(n); y<(int)((img).height-(n)); y++)
#define cimg_imapZ(img,z,n)       for (int z=(n); z<(int)((img).depth-(n)); z++)
#define cimg_imapV(img,v,n)       for (int v=(n); v<(int)((img).dim-(n)); v++)
#define cimg_imapXY(img,x,y,n)    cimg_imapY(img,y,n) cimg_imapX(img,x,n)
#define cimg_imapXYZ(img,x,y,z,n) cimg_imapZ(img,z,n) cimg_imapXY(img,x,y,n)
#define cimg_bmapX(img,x,n)       for (int x=0; x<(int)((img).width);  x==(n)-1?(x=(img).width-(n)): x++)
#define cimg_bmapY(img,y,n)       for (int y=0; y<(int)((img).height); y==(n)-1?(x=(img).height-(n)):y++)
#define cimg_bmapZ(img,z,n)       for (int z=0; z<(int)((img).depth);  z==(n)-1?(x=(img).depth-(n)): z++)
#define cimg_bmapV(img,v,n)       for (int v=0; v<(int)((img).dim);    v==(n)-1?(x=(img).dim-(n)):   v++)
#define cimg_bmapXY(img,x,y,n)    cimg_mapY(img,y) for (int x=0; x<(int)((img).width); (y<(n) || y>=(int)((img).height)-(n))?x++: \
														  ((x<(n)-1 || x>=(int)((img).width)-(n))?x++:(x=(img).width-(n))))
#define cimg_bmapXYZ(img,x,y,z,n) cimg_mapYZ(img,y,z) for (int x=0; x<(int)((img).width); (y<(n) || y>=(int)((img).height)-(n) || z<(n) || z>=(int)((img).depth)-(n))?x++: \
															 ((x<(n)-1 || x>=(int)((img).width)-(n))?x++:(x=(img).width-(n))))
#define cimg_2mapX(img,x)         for (int x=0,_n##x=1; _n##x<(int)((img).width)   || x==--_n##x; x++, _n##x++)
#define cimg_2mapY(img,y)         for (int y=0,_n##y=1; _n##y<(int)((img).height)  || y==--_n##y; y++, _n##y++)
#define cimg_2mapZ(img,z)         for (int z=0,_n##z=1; _n##z<(int)((img).depth)   || z==--_n##z; z++, _n##z++)
#define cimg_2mapXY(img,x,y)      cimg_2mapY(img,y) cimg_2mapX(img,x)
#define cimg_2mapXZ(img,x,z)      cimg_2mapZ(img,z) cimg_2mapX(img,x)
#define cimg_2mapYZ(img,y,z)      cimg_2mapZ(img,z) cimg_2mapY(img,y)
#define cimg_2mapXYZ(img,x,y,z)   cimg_2mapZ(img,z) cimg_2mapXY(img,x,y)
#define cimg_3mapX(img,x)         for (int x=0,_p##x=0,_n##x=1; _n##x<(int)((img).width)  || x==--_n##x;  _p##x=x++,_n##x++)
#define cimg_3mapY(img,y)         for (int y=0,_p##y=0,_n##y=1; _n##y<(int)((img).height) || y==--_n##y;  _p##y=y++,_n##y++)
#define cimg_3mapZ(img,z)         for (int z=0,_p##z=0,_n##z=1; _n##z<(int)((img).depth)  || z==--_n##z;  _p##z=z++,_n##z++)
#define cimg_3mapXY(img,x,y)      cimg_3mapY(img,y) cimg_3mapX(img,x)
#define cimg_3mapXZ(img,x,z)      cimg_3mapZ(img,z) cimg_3mapX(img,x)
#define cimg_3mapYZ(img,y,z)      cimg_3mapZ(img,z) cimg_3mapY(img,y)
#define cimg_3mapXYZ(img,x,y,z)   cimg_3mapZ(img,z) cimg_3mapXY(img,x,y)
#define cimg_4mapX(img,x)         for (int _p##x=0,x=0,_n##x=1,_a##x=2; \
									   _a##x<(int)((img).width)  || _n##x==--_a##x || x==(_a##x=--_n##x); \
									   _p##x=x++,_n##x++,_a##x++)
#define cimg_4mapY(img,y)         for (int _p##y=0,y=0,_n##y=1,_a##y=2; \
									   _a##y<(int)((img).height) || _n##y==--_a##y || y==(_a##y=--_n##y); \
									   _p##y=y++,_n##y++,_a##y++)
#define cimg_4mapZ(img,z)         for (int _p##z=0,z=0,_n##z=1,_a##z=2; \
									   _a##z<(int)((img).depth)  || _n##z==--_a##z || z==(_a##z=--_n##z); \
									   _p##z=z++,_n##z++,_a##z++)
#define cimg_4mapXY(img,x,y)      cimg_4mapY(img,y) cimg_4mapX(img,x)
#define cimg_4mapXZ(img,x,z)      cimg_4mapZ(img,z) cimg_4mapX(img,x)
#define cimg_4mapYZ(img,y,z)      cimg_4mapZ(img,z) cimg_4mapY(img,y)
#define cimg_4mapXYZ(img,x,y,z)   cimg_4mapZ(img,z) cimg_4mapXY(img,x,y)
#define cimg_5mapX(img,x)         for (int _b##x=0,_p##x=0,x=0,_n##x=1,_a##x=2; \
									   _a##x<(int)((img).width)  || _n##x==--_a##x || x==(_a##x=--_n##x); \
									   _b##x=_p##x,_p##x=x++,_n##x++,_a##x++)
#define cimg_5mapY(img,y)         for (int _b##y=0,_p##y=0,y=0,_n##y=1,_a##y=2; \
									   _a##y<(int)((img).height) || _n##y==--_a##y || y==(_a##y=--_n##y); \
									   _b##y=_p##y,_p##y=y++,_n##y++,_a##y++)
#define cimg_5mapZ(img,z)         for (int _b##z=0,_p##z=0,z=0,_n##z=1,_a##z=2; \
									   _a##z<(int)((img).depth)  || _n##z==--_a##z || z==(_a##z=--_n##z); \
									   _b##z=_p##z,_p##z=z++,_n##z++,_a##z++)
#define cimg_5mapXY(img,x,y)      cimg_5mapY(img,y) cimg_5mapX(img,x)
#define cimg_5mapXZ(img,x,z)      cimg_5mapZ(img,z) cimg_5mapX(img,x)
#define cimg_5mapYZ(img,y,z)      cimg_5mapZ(img,z) cimg_5mapY(img,y)
#define cimg_5mapXYZ(img,x,y,z)   cimg_5mapZ(img,z) cimg_5mapXY(img,x,y)

#define cimg_map2x2x1(img,x,y,z,v,I) cimg_2mapY(img,y) \
	   for (int _n##x=1, x=(int)((I##cc=(img)(0,    y,z,v)), \
								 (I##cn=(img)(0,_n##y,z,v)), \
				 0); \
			(_n##x<(int)((img).width) && ((I##nc=(img)(_n##x,    y,z,v)), \
										  (I##nn=(img)(_n##x,_n##y,z,v)), \
										  1)) || x==--_n##x; \
			I##cc=I##nc, I##cn=I##nn, \
			x++,_n##x++ )

#define cimg_map3x3x1(img,x,y,z,v,I) cimg_3mapY(img,y) \
	   for (int _n##x=1, _p##x=(int)((I##cp=I##pp=(img)(0,_p##y,z,v)), \
									 (I##cc=I##pc=(img)(0,  y,z,v)), \
									 (I##cn=I##pn=(img)(0,_n##y,z,v))), \
									 x=_p##x=0; \
			(_n##x<(int)((img).width) && ((I##np=(img)(_n##x,_p##y,z,v)), \
										  (I##nc=(img)(_n##x,    y,z,v)), \
										  (I##nn=(img)(_n##x,_n##y,z,v)), \
										  1)) || x==--_n##x; \
			  I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, \
			  I##cp=I##np, I##cc=I##nc, I##cn=I##nn, \
			  _p##x=x++,_n##x++ )


#define cimg_map4x4x1(img,x,y,z,v,I) cimg_4mapY(img,y) \
	   for (int _a##x=2, _n##x=1, x=(int)((I##cp=I##pp=(img)(0,_p##y,z,v)), \
										  (I##cc=I##pc=(img)(0,    y,z,v)), \
										  (I##cn=I##pn=(img)(0,_n##y,z,v)), \
										  (I##ca=I##pa=(img)(0,_a##y,z,v)), \
										  (I##np=(img)(_n##x,_p##y,z,v)), \
										  (I##nc=(img)(_n##x,    y,z,v)), \
										  (I##nn=(img)(_n##x,_n##y,z,v)), \
										  (I##na=(img)(_n##x,_a##y,z,v)), \
						  0), _p##x=0; \
			(_a##x<(int)((img).width) && ((I##ap=(img)(_a##x,_p##y,z,v)), \
										  (I##ac=(img)(_a##x,    y,z,v)), \
										  (I##an=(img)(_a##x,_n##y,z,v)), \
										  (I##aa=(img)(_a##x,_a##y,z,v)), \
										  1)) || _n##x==--_a##x || x==(_a##x=--_n##x); \
			  I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, I##pa=I##ca, \
			  I##cp=I##np, I##cc=I##nc, I##cn=I##nn, I##ca=I##na, \
			  I##np=I##ap, I##nc=I##ac, I##nn=I##an, I##na=I##aa, \
			  _p##x=x++, _n##x++, _a##x++ )

#define cimg_map5x5x1(img,x,y,z,v,I) cimg_5mapY(img,y) \
	   for (int _a##x=2, _n##x=1, _b##x=(int)((I##cb=I##pb=I##bb=(img)(0,_b##y,z,v)), \
											  (I##cp=I##pp=I##bp=(img)(0,_p##y,z,v)), \
											  (I##cc=I##pc=I##bc=(img)(0,    y,z,v)), \
											  (I##cn=I##pn=I##bn=(img)(0,_n##y,z,v)), \
											  (I##ca=I##pa=I##ba=(img)(0,_a##y,z,v)), \
											  (I##nb=(img)(_n##x,_b##y,z,v)), \
											  (I##np=(img)(_n##x,_p##y,z,v)), \
											  (I##nc=(img)(_n##x,   y,z,v)), \
											  (I##nn=(img)(_n##x,_n##y,z,v)), \
											  (I##na=(img)(_n##x,_a##y,z,v))), \
											  x=0, _p##x=_b##x=0; \
			(_a##x<(int)((img).width) && ((I##ab=(img)(_a##x,_b##y,z,v)), \
										  (I##ap=(img)(_a##x,_p##y,z,v)), \
										  (I##ac=(img)(_a##x,    y,z,v)), \
										  (I##an=(img)(_a##x,_n##y,z,v)), \
										  (I##aa=(img)(_a##x,_a##y,z,v)), \
										  1)) || _n##x==--_a##x || x==(_a##x=--_n##x); \
			  I##bb=I##pb, I##bp=I##pp, I##bc=I##pc, I##bn=I##pn, I##ba=I##pa, \
			  I##pb=I##cb, I##pp=I##cp, I##pc=I##cc, I##pn=I##cn, I##pa=I##ca, \
			  I##cb=I##nb, I##cp=I##np, I##cc=I##nc, I##cn=I##nn, I##ca=I##na, \
			  I##nb=I##ab, I##np=I##ap, I##nc=I##ac, I##nn=I##an, I##na=I##aa, \
			  _b##x=_p##x, _p##x=x++, _n##x++, _a##x++ )

#define cimg_map2x2x2(img,x,y,z,v,I) cimg_2mapYZ(img,y,z) \
	   for (int _n##x=1, x=(int)((I##ccc=(img)(0,    y,    z,v)), \
								 (I##cnc=(img)(0,_n##y,    z,v)), \
								 (I##ccn=(img)(0,    y,_n##z,v)), \
								 (I##cnn=(img)(0,_n##y,_n##z,v)), \
								 0); \
			(_n##x<(int)((img).width) && ((I##ncc=(img)(_n##x,    y,    z,v)), \
										  (I##nnc=(img)(_n##x,_n##y,    z,v)), \
										  (I##ncn=(img)(_n##x,    y,_n##z,v)), \
										  (I##nnn=(img)(_n##x,_n##y,_n##z,v)), \
										  1)) || x==--_n##x; \
			  I##ccc=I##ncc, I##cnc=I##nnc, \
			  I##ccn=I##ncn, I##cnn=I##nnn, \
			  x++, _n##x++ )

#define cimg_map3x3x3(img,x,y,z,v,I) cimg_3mapYZ(img,y,z) \
	   for (int _n##x=1, _p##x=(int)((I##cpp=I##ppp=(img)(0,_p##y,_p##z,v)), \
									 (I##ccp=I##pcp=(img)(0,    y,_p##z,v)), \
									 (I##cnp=I##pnp=(img)(0,_n##y,_p##z,v)), \
									 (I##cpc=I##ppc=(img)(0,_p##y,    z,v)), \
									 (I##ccc=I##pcc=(img)(0,    y,    z,v)), \
									 (I##cnc=I##pnc=(img)(0,_n##y,    z,v)), \
									 (I##cpn=I##ppn=(img)(0,_p##y,_n##z,v)), \
									 (I##ccn=I##pcn=(img)(0,    y,_n##z,v)), \
									 (I##cnn=I##pnn=(img)(0,_n##y,_n##z,v))),\
									 x=_p##x=0; \
			(_n##x<(int)((img).width) && ((I##npp=(img)(_n##x,_p##y,_p##z,v)), \
										  (I##ncp=(img)(_n##x,    y,_p##z,v)), \
										  (I##nnp=(img)(_n##x,_n##y,_p##z,v)), \
										  (I##npc=(img)(_n##x,_p##y,    z,v)), \
										  (I##ncc=(img)(_n##x,    y,    z,v)), \
										  (I##nnc=(img)(_n##x,_n##y,    z,v)), \
										  (I##npn=(img)(_n##x,_p##y,_n##z,v)), \
										  (I##ncn=(img)(_n##x,    y,_n##z,v)), \
										  (I##nnn=(img)(_n##x,_n##y,_n##z,v)), \
										  1)) || x==--_n##x; \
			  I##ppp=I##cpp, I##pcp=I##ccp, I##pnp=I##cnp, \
			  I##cpp=I##npp, I##ccp=I##ncp, I##cnp=I##nnp, \
			  I##ppc=I##cpc, I##pcc=I##ccc, I##pnc=I##cnc, \
			  I##cpc=I##npc, I##ccc=I##ncc, I##cnc=I##nnc, \
			  I##ppn=I##cpn, I##pcn=I##ccn, I##pnn=I##cnn, \
			  I##cpn=I##npn, I##ccn=I##ncn, I##cnn=I##nnn, \
			  _p##x=x++, _n##x++ )

/*
 #------------------------------------------------
 #
 #
 #  Definition of the cimg_library:: namespace
 #
 #
 #------------------------------------------------
 */

//! Namespace that encompasses all classes and functions of the %CImg library.
/**
   This namespace is defined to avoid class names collisions that could happen
   with the include of other C++ header files. Anyway, it should not happen
   very often and you may start most of your programs with
   \code
   #include "CImg.h"
   using namespace cimg_library;
   \endcode
   to simplify the declaration of %CImg Library objects variables afterward.
**/

namespace cimg_library {

  // Define the CImg classes.
  template<typename T=float> struct CImg;
  template<typename T=float> struct CImgl;
  struct CImgStats;
  struct CImgDisplay;
  struct CImgException; 

  namespace cimg {

	// The bodies of the functions below are defined at the end of the file
	inline int dialog(const char *title,const char *msg,const char *button1_txt="OK",
			  const char *button2_txt=0,const char *button3_txt=0,
			  const char *button4_txt=0,const char *button5_txt=0,
			  const char *button6_txt=0,const bool centering = false);

	template<typename tfunc, typename tp, typename tf>
	inline void marching_cubes(const tfunc& func, const float isovalue,
				   const float x0,const float y0,const float z0,
				   const float x1,const float y1,const float z1,
				   const float resx,const float resy,const float resz,
				   CImgl<tp>& points, CImgl<tf>& primitives,
				   const bool invert_faces = false);

	template<typename tfunc, typename tp, typename tf>
	inline void marching_squares(const tfunc& func, const float isovalue,
				 const float x0,const float y0,
				 const float x1,const float y1,
				 const float resx,const float resy,
				 CImgl<tp>& points, CImgl<tf>& primitives);
  }
  
  /*
   #----------------------------------------------
   #
   #
   # Definition of the CImgException structures
   #
   #
   #----------------------------------------------
   */

  // Never use the following macro in your own code !
#define cimg_exception_err(etype,disp_flag) \
  if (cimg_debug>=1) { \
	std::va_list ap; \
	va_start(ap,format); \
	std::vsprintf(message,format,ap); \
	va_end(ap); \
	if (disp_flag) { \
	  try { cimg::dialog(etype,message,"Abort"); } \
	  catch (CImgException&) { std::fprintf(stderr,"# %s :\n%s\n\n",etype,message); } \
	} else std::fprintf(stderr,"# %s :\n%s\n\n",etype,message); \
  }
	
  //! Class which is thrown when an error occured during a %CImg library function call.
  /** 
	  
	  \section ex1 Overview
  
	  CImgException is the base class of %CImg exceptions.
	  Exceptions are thrown by the %CImg Library when an error occured in a %CImg library function call.
	  CImgException is seldom thrown itself. Children classes that specify the kind of error encountered
	  are generally used instead. These sub-classes are :

	  - \b CImgInstanceException : Thrown when the instance associated to the called %CImg function is not
	  correctly defined. Generally, this exception is thrown when one tries to process \a empty images. The example
	  below will throw a \a CImgInstanceException.
	  \code
	  CImg<float> img;        // Construct an empty image.
	  img.blur(10);           // Try to blur the image.
	  \endcode

	  - \b CImgArgumentException : Thrown when one of the arguments given to the called %CImg function is not correct.
	  Generally, this exception is thrown when arguments passed to the function are outside an admissible range of values.
	  The example below will throw a \a CImgArgumentException.
	  \code
	  CImg<float> img(100,100,1,3);   // Define a 100x100 color image with float pixels.
	  img = 0;                     // Try to fill pixels from the 0 pointer (invalid argument to operator=() ).
	  \endcode

	  - \b CImgIOException : Thrown when an error occured when trying to load or save image files.
	  The example below will throw a \a CImgIOException.
	  \code
	  CImg<float> img("file_doesnt_exist.jpg");    // Try to load a file that doesn't exist.
	  \endcode
	  
	  - \b CImgDisplayException : Thrown when an error occured when trying to display an image in a window.
	  This exception is thrown when image display request cannot be satisfied.
	  
	  The parent class CImgException may be thrown itself when errors that cannot be classified in one of
	  the above type occur. It is recommended not to throw CImgExceptions yourself, since there are normally
	  reserved to %CImg Library functions.
	  \b CImgInstanceException, \b CImgArgumentException, \b CImgIOException and \b CImgDisplayException are simple
	  subclasses of CImgException and are thus not detailled more in this reference documentation.

	  \section ex2 Exception handling

	  When an error occurs, the %CImg Library first displays the error in a modal window.
	  Then, it throws an instance of the corresponding exception class, generally leading the program to stop
	  (this is the default behavior).
	  You can bypass this default behavior by handling the exceptions yourself,
	  using a code block <tt>try { ... } catch() { ... }</tt>.
	  In this case, you can avoid the apparition of the modal window, by
	  defining the environment variable <tt>cimg_debug</tt> to 0 before including the %CImg header file.      
	  The example below shows how to cleanly handle %CImg Library exceptions :
	  \code
	  #define cimg_debug 0     // Disable modal window in CImg exceptions.
	  #define "CImg.h"
	  int main() {  
		try {
		  ...; // Here, do what you want.
		}
		catch (CImgInstanceException &e) {
		  std::fprintf(stderr,"CImg Library Error : %s",e.message);  // Display your own error message
		  ...                                                        // Do what you want now.
		}
	  }
	  \endcode      
  **/
  struct CImgException {
	char message[1024]; //!< Message associated with the error that thrown the exception.
	CImgException() { message[0]='\0'; }
	CImgException(const char *format,...) { cimg_exception_err("CImgException",true); }
  };

  // The \ref CImgInstanceException class is used to throw an exception related
  // to a non suitable instance encountered in a library function call.  
  struct CImgInstanceException : CImgException { 
	CImgInstanceException(const char *format,...) { cimg_exception_err("CImgInstanceException",true); }
  };

  // The \ref CImgArgumentException class is used to throw an exception related
  // to invalid arguments encountered in a library function call.
  struct CImgArgumentException : CImgException { 
	CImgArgumentException(const char *format,...) { cimg_exception_err("CImgArgumentException",true); }
  };

  // The \ref CImgIOException class is used to throw an exception related 
  // to Input/Output file problems encountered in a library function call.
  struct CImgIOException : CImgException { 
	CImgIOException(const char *format,...) { cimg_exception_err("CImgIOException",true); }
  };

  // The CImgDisplayException class is used to throw an exception related to display problems
  // encountered in a library function call.
  struct CImgDisplayException : CImgException {
	CImgDisplayException(const char *format,...) { cimg_exception_err("CImgDisplayException",false); }
  };  

  /*
   #-------------------------------------
   #
   #
   # Definition of the namespace 'cimg'
   #
   #
   #-------------------------------------
   */
  
  //! Namespace that encompasses \a low-level functions and variables of the %CImg Library.
  /**
	 Most of the functions and variables within this namespace are used by the library for low-level processing.
	 Nevertheless, documented variables and functions of this namespace may be used safely in your own source code.

	 \warning Never write <tt>using namespace cimg_library::cimg;</tt> in your source code, since a lot of functions of the
	 <tt>cimg::</tt> namespace have prototypes similar to standard C functions defined in the global namespace <tt>::</tt>.     
  **/
  namespace cimg {

	// Define the trait that will be used to determine the best data type to work with.
	template<typename T,typename t> struct largest { typedef t type; };
	template<> struct largest<unsigned char,bool> { typedef unsigned char type; };
	template<> struct largest<unsigned char,char> { typedef short type; };
	template<> struct largest<char,bool> { typedef char type; };
	template<> struct largest<char,unsigned char> { typedef short type; };
	template<> struct largest<char,unsigned short> { typedef int type; };
	template<> struct largest<char,unsigned int> { typedef float type; };
	template<> struct largest<char,unsigned long> { typedef float type; };
	template<> struct largest<unsigned short,bool> { typedef unsigned short type; };
	template<> struct largest<unsigned short,unsigned char> { typedef unsigned short type; };
	template<> struct largest<unsigned short,char> { typedef short type; };
	template<> struct largest<unsigned short,short> { typedef int type; };
	template<> struct largest<short,bool> { typedef short type; };
	template<> struct largest<short,unsigned char> { typedef short type; };
	template<> struct largest<short,char> { typedef short type; };
	template<> struct largest<short,unsigned short> { typedef int type; };
	template<> struct largest<short,unsigned int> { typedef float type; };
	template<> struct largest<short,unsigned long> { typedef float type; };
	template<> struct largest<unsigned int,bool> { typedef unsigned int type; };
	template<> struct largest<unsigned int,unsigned char> { typedef unsigned int type; };
	template<> struct largest<unsigned int,char> { typedef unsigned int type; };
	template<> struct largest<unsigned int,unsigned short> { typedef unsigned int type; };
	template<> struct largest<unsigned int,short> { typedef float type; };
	template<> struct largest<unsigned int,int> { typedef float type; };
	template<> struct largest<int,bool> { typedef int type; };
	template<> struct largest<int,unsigned char> { typedef int type; };
	template<> struct largest<int,char> { typedef int type; };
	template<> struct largest<int,unsigned short> { typedef int type; };
	template<> struct largest<int,short> { typedef int type; };
	template<> struct largest<int,unsigned int> { typedef float type; };
	template<> struct largest<int,unsigned long> { typedef float type; };
	template<> struct largest<float,bool> { typedef float type; };
	template<> struct largest<float,unsigned char> { typedef float type; };
	template<> struct largest<float,char> { typedef float type; };
	template<> struct largest<float,unsigned short> { typedef float type; };
	template<> struct largest<float,short> { typedef float type; };
	template<> struct largest<float,unsigned int> { typedef float type; };
	template<> struct largest<float,int> { typedef float type; };
	template<> struct largest<float,unsigned long> { typedef float type; };
	template<> struct largest<float,long> { typedef float type; };
	template<> struct largest<double,bool> { typedef double type; };
	template<> struct largest<double,unsigned char> { typedef double type; };
	template<> struct largest<double,char> { typedef double type; };
	template<> struct largest<double,unsigned short> { typedef double type; };
	template<> struct largest<double,short> { typedef double type; };
	template<> struct largest<double,unsigned int> { typedef double type; };
	template<> struct largest<double,int> { typedef double type; };
	template<> struct largest<double,unsigned long> { typedef double type; };
	template<> struct largest<double,long> { typedef double type; };
	template<> struct largest<double,float> { typedef double type; };

	template<typename T> struct type {
	  static T min() { return (T)-1>0?(T)0:(T)-(1U<<(8*sizeof(T)-1)); }
	  static T max() { return (T)-1>0?(T)-1:(T)((1U<<(8*sizeof(T)-1))-1); }
	};
	template<> struct type<unsigned char> {
	  static unsigned char min() { return 0; }
	  static unsigned char max() { return (unsigned char)~0U; }
	};
	template<> struct type<unsigned short> {
	  static unsigned short min() { return 0; }
	  static unsigned short max() { return (unsigned short)~0U; }
	};
	template<> struct type<unsigned int> {
	  static unsigned int min() { return 0; }
	  static unsigned int max() { return (unsigned int)~0U; }
	};
	template<> struct type<unsigned long> {
	  static unsigned long min() { return 0; }
	  static unsigned long max() { return (unsigned long)~0UL; }
	};
	template<> struct type<bool> {
	  static bool min() { return false; }
	  static bool max() { return true; }
	};
	template<> struct type<float> {
	  static float min() { return -3.4E38f; }
	  static float max() { return  3.4E38f; }
	};
	template<> struct type<double> {
	  static double min() { return -1.7E308; }
	  static double max() { return  1.7E308; }
	};
	
	// Define internal library variables.
#if cimg_display_type==1
	struct X11info {
	  volatile unsigned int nb_wins;
	  pthread_mutex_t* mutex;
	  pthread_t*       event_thread;
	  CImgDisplay*     wins[1024];
	  Display*         display; 
	  unsigned int     nb_bits;
	  GC*              gc;
	  bool             blue_first;
	  bool             byte_order;
	  bool             shm_enabled;
#ifdef cimg_use_xrandr
	  XRRScreenSize *resolutions;
	  Rotation curr_rotation;
	  unsigned int curr_resolution;
	  unsigned int nb_resolutions;
#endif      
	  X11info():nb_wins(0),mutex(0),event_thread(0),display(0),
		nb_bits(0),gc(0),blue_first(false),byte_order(false),shm_enabled(false) {
#ifdef cimg_use_xrandr
	resolutions = 0;
	curr_rotation = 0;
	curr_resolution = nb_resolutions = 0;
#endif
	  }
	};
#if defined(cimg_module)
	X11info& X11attr();
#elif defined(cimg_main)
	X11info& X11attr() { static X11info val; return val; }
#else
	inline X11info& X11attr() { static X11info val; return val; }
#endif

#elif cimg_display_type==2
	struct Win32info {
	  HANDLE wait_event;
	  Win32info() { wait_event = CreateEvent(0,FALSE,FALSE,0); }
	};
#if defined(module)
	Win32info& Win32attr();
#elif defined(cimg_main)
	Win32info& Win32attr() { static Win32info val; return val; }
#else
	inline Win32info& Win32attr() { static Win32info val; return val; }
#endif
#endif
	
#ifdef cimg_color_terminal
	const char t_normal[9]  = {0x1b,'[','0',';','0',';','0','m','\0'};
	const char t_red[11]    = {0x1b,'[','4',';','3','1',';','5','9','m','\0'};
	const char t_bold[5]    = {0x1b,'[','1','m','\0'};
	const char t_purple[11] = {0x1b,'[','0',';','3','5',';','5','9','m','\0'};
#else
	const char t_normal[1]  = {'\0'};
	const char *const t_red = cimg::t_normal, *const t_bold = cimg::t_normal, *const t_purple = cimg::t_normal;
#endif
	
#if cimg_display_type==1
	// Keycodes for X11-based graphical systems
	const unsigned int keyESC        = XK_Escape;
	const unsigned int keyF1         = XK_F1;
	const unsigned int keyF2         = XK_F2;
	const unsigned int keyF3         = XK_F3;
	const unsigned int keyF4         = XK_F4;
	const unsigned int keyF5         = XK_F5;
	const unsigned int keyF6         = XK_F6;
	const unsigned int keyF7         = XK_F7;
	const unsigned int keyF8         = XK_F8;
	const unsigned int keyF9         = XK_F9;
	const unsigned int keyF10        = XK_F10;
	const unsigned int keyF11        = XK_F11;
	const unsigned int keyF12        = XK_F12;
	const unsigned int keyPAUSE      = XK_Pause;
	const unsigned int key1          = XK_1;
	const unsigned int key2          = XK_2;
	const unsigned int key3          = XK_3;
	const unsigned int key4          = XK_4;
	const unsigned int key5          = XK_5;
	const unsigned int key6          = XK_6;
	const unsigned int key7          = XK_7;
	const unsigned int key8          = XK_8;
	const unsigned int key9          = XK_9;
	const unsigned int key0          = XK_0;
	const unsigned int keyBACKSPACE  = XK_BackSpace;
	const unsigned int keyINSERT     = XK_Insert;
	const unsigned int keyHOME       = XK_Home;
	const unsigned int keyPAGEUP     = XK_Page_Up;
	const unsigned int keyTAB        = XK_Tab;
	const unsigned int keyQ          = XK_q;
	const unsigned int keyW          = XK_w;
	const unsigned int keyE          = XK_e;
	const unsigned int keyR          = XK_r;
	const unsigned int keyT          = XK_t;
	const unsigned int keyY          = XK_y;
	const unsigned int keyU          = XK_u;
	const unsigned int keyI          = XK_i;
	const unsigned int keyO          = XK_o;
	const unsigned int keyP          = XK_p;
	const unsigned int keyDELETE     = XK_Delete;
	const unsigned int keyEND        = XK_End;
	const unsigned int keyPAGEDOWN   = XK_Page_Down;
	const unsigned int keyCAPSLOCK   = XK_Caps_Lock;
	const unsigned int keyA          = XK_a;
	const unsigned int keyS          = XK_s;
	const unsigned int keyD          = XK_d;
	const unsigned int keyF          = XK_f;
	const unsigned int keyG          = XK_g;
	const unsigned int keyH          = XK_h;
	const unsigned int keyJ          = XK_j;
	const unsigned int keyK          = XK_k;
	const unsigned int keyL          = XK_l;
	const unsigned int keyENTER      = XK_Return;
	const unsigned int keySHIFTLEFT  = XK_Shift_L;
	const unsigned int keyZ          = XK_z;
	const unsigned int keyX          = XK_x;
	const unsigned int keyC          = XK_c;
	const unsigned int keyV          = XK_v;
	const unsigned int keyB          = XK_b;
	const unsigned int keyN          = XK_n;
	const unsigned int keyM          = XK_m;
	const unsigned int keySHIFTRIGHT = XK_Shift_R;
	const unsigned int keyARROWUP    = XK_Up;
	const unsigned int keyCTRLLEFT   = XK_Control_L;
	const unsigned int keyAPPLEFT    = XK_Super_L;
	const unsigned int keySPACE      = XK_space;
	const unsigned int keyALTGR      = XK_Alt_R;
	const unsigned int keyAPPRIGHT   = XK_Super_R;
	const unsigned int keyMENU       = XK_Menu;
	const unsigned int keyCTRLRIGHT  = XK_Control_R;
	const unsigned int keyARROWLEFT  = XK_Left;
	const unsigned int keyARROWDOWN  = XK_Down;
	const unsigned int keyARROWRIGHT = XK_Right;  
#endif

#if cimg_display_type==0 || (cimg_display_type==2 && cimg_OS==2)
	// Keycodes for Windows-OS
	const unsigned int keyESC        = 27;
	const unsigned int keyF1         = 112;
	const unsigned int keyF2         = 113;
	const unsigned int keyF3         = 114;
	const unsigned int keyF4         = 115;
	const unsigned int keyF5         = 116;
	const unsigned int keyF6         = 117;
	const unsigned int keyF7         = 118;
	const unsigned int keyF8         = 119;
	const unsigned int keyF9         = 120;
	const unsigned int keyF10        = 121;
	const unsigned int keyF11        = 122;
	const unsigned int keyF12        = 123;
	const unsigned int keyPAUSE      = 19;
	const unsigned int key1          = 49;
	const unsigned int key2          = 50;
	const unsigned int key3          = 51;
	const unsigned int key4          = 52;
	const unsigned int key5          = 53;
	const unsigned int key6          = 54;
	const unsigned int key7          = 55;
	const unsigned int key8          = 56;
	const unsigned int key9          = 57;
	const unsigned int key0          = 48;
	const unsigned int keyBACKSPACE  = 8;
	const unsigned int keyINSERT     = 45;
	const unsigned int keyHOME       = 36;
	const unsigned int keyPAGEUP     = 33;
	const unsigned int keyTAB        = 9;
	const unsigned int keyQ          = 81;
	const unsigned int keyW          = 87;
	const unsigned int keyE          = 69;
	const unsigned int keyR          = 82;
	const unsigned int keyT          = 84;
	const unsigned int keyY          = 89;
	const unsigned int keyU          = 85;
	const unsigned int keyI          = 73;
	const unsigned int keyO          = 79;
	const unsigned int keyP          = 80;
	const unsigned int keyDELETE     = 8;
	const unsigned int keyEND        = 35;
	const unsigned int keyPAGEDOWN   = 34;
	const unsigned int keyCAPSLOCK   = 20;
	const unsigned int keyA          = 65;
	const unsigned int keyS          = 83;
	const unsigned int keyD          = 68;
	const unsigned int keyF          = 70;
	const unsigned int keyG          = 71;
	const unsigned int keyH          = 72;
	const unsigned int keyJ          = 74;
	const unsigned int keyK          = 75;
	const unsigned int keyL          = 76;
	const unsigned int keyENTER      = 13;
	const unsigned int keySHIFTLEFT  = 16;
	const unsigned int keyZ          = 90;
	const unsigned int keyX          = 88;
	const unsigned int keyC          = 67;
	const unsigned int keyV          = 86;
	const unsigned int keyB          = 66;
	const unsigned int keyN          = 78;
	const unsigned int keyM          = 77;
	const unsigned int keySHIFTRIGHT = 16;
	const unsigned int keyARROWUP    = 38;
	const unsigned int keyCTRLLEFT   = 17;
	const unsigned int keyAPPLEFT    = 91;
	const unsigned int keySPACE      = 32;
	const unsigned int keyALTGR      = 17;
	const unsigned int keyAPPRIGHT   = 92;
	const unsigned int keyMENU       = 93;
	const unsigned int keyCTRLRIGHT  = 17;
	const unsigned int keyARROWLEFT  = 37;
	const unsigned int keyARROWDOWN  = 40;
	const unsigned int keyARROWRIGHT = 39;
#endif

#ifdef PI
#undef PI
#endif
	const double PI = 3.14159265358979323846;   //!< Definition of the mathematical constant PI
	
	// Definition of a 7x11 font, used to return a default font for drawing text.
	const unsigned int font7x11[7*11*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x90,0x0,0x7f0000,0x40000,0x0,0x0,0x4010c0a4,0x82000040,0x11848402,0x18480050,0x80430292,0x8023,0x9008000,
	  0x40218140,0x4000040,0x21800402,0x18000051,0x1060500,0x8083,0x10000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x24002,0x4031,0x80000000,0x10000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x81c0400,0x40020000,0x80070080,0x40440e00,0x0,0x0,0x1,0x88180000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x200000,0x0,0x0,0x80000,0x0,0x0,0x20212140,0x5000020,0x22400204,0x240000a0,0x40848500,0x4044,0x80010038,0x20424285,0xa000020,
	  0x42428204,0x2428e0a0,0x82090a14,0x4104,0x85022014,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10240a7,0x88484040,0x40800000,0x270c3,0x87811e0e,
	  0x7c70e000,0x78,0x3c23c1ef,0x1f3e1e89,0xf1c44819,0xa23cf0f3,0xc3cff120,0xc18307f4,0x4040400,0x20000,0x80080080,0x40200,0x0,
	  0x40000,0x2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8188,0x50603800,0xf3c00000,0x1c004003,0xc700003e,0x18180,0xc993880,0x10204081,
	  0x2071ef9,0xf3e7cf9f,0x3e7c7911,0xe3c78f1e,0x7d1224,0x48906048,0x0,0x4000000,0x0,0x9000,0x0,0x0,0x2000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x10240aa,0x14944080,0x23610000,0x68940,0x40831010,0x8891306,0x802044,0x44522208,0x90202088,0x40448819,0xb242890a,0x24011111,
	  0x49448814,0x4040a00,0xe2c3c7,0x8e3f3cb9,0xc1c44216,0xee38b0f2,0xe78f9120,0xc18507e2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x101c207,0x88a04001,0x9c00000,0x2200a041,0x8200113a,0x8240,0x50a3110,0x2850a142,0x850c2081,0x2040204,0x8104592,0x142850a1,
	  0x42cd1224,0x4888bc48,0x70e1c387,0xe3b3c70,0xe1c38e1c,0x38707171,0xc3870e1c,0x10791224,0x48906c41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x10003ee,0x15140080,0x21810000,0x48840,0x40851020,0x8911306,0x31fd804,0x9c522408,0x90204088,0x4045081a,0xba42890a,0x24011111,
	  0x49285024,0x2041b00,0x132408,0x910844c8,0x4044821b,0x7244c913,0x24041111,0x49488822,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x28204,0x85006001,0x6a414000,0x3a004043,0xc700113a,0x8245,0x50a3a00,0x2850a142,0x850c4081,0x2040204,0x81045d2,0x142850a1,
	  0x24951224,0x48852250,0x8102040,0x81054089,0x12244204,0x8108992,0x24489122,0x991224,0x4888b222,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x1000143,0xa988080,0x2147c01f,0x88840,0x83091c2c,0x1070f000,0xc000608,0xa48bc408,0x9e3c46f8,0x40460816,0xaa42f10b,0xc3811111,
	  0x35102044,0x1041100,0xf22408,0x9f084488,0x40470212,0x62448912,0x6041111,0x55308846,0x8061c80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x1028704,0x8f805801,0x4be28fdf,0x220001f0,0x111a,0x60000182,0x82c5c710,0x44891224,0x489640f1,0xe3c78204,0x810e552,0x142850a1,
	  0x18a51224,0x48822250,0x78f1e3c7,0x8f1f40f9,0xf3e7c204,0x8108912,0x24489122,0x7ea91224,0x4888a222,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x10007e2,0x85648080,0x20010000,0x88841,0x8f8232,0x20881000,0xc1fc610,0xbefa2408,0x90204288,0x40450816,0xa642810a,0x4041110a,
	  0x36282084,0x1042080,0x1122408,0x90084488,0x40450212,0x62448912,0x184110a,0x55305082,0x8042700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x1028207,0x82004801,0x68050040,0x1c000040,0x110a,0x60000001,0x45484d10,0x7cf9f3e7,0xcf944081,0x2040204,0x8104532,0x142850a1,
	  0x18a51224,0x48822248,0x89122448,0x91244081,0x2040204,0x8108912,0x24489122,0xc91224,0x48852214,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x282,
	  0x89630080,0x20010c00,0x30108842,0x810222,0x20882306,0x3001800,0x408a2208,0x90202288,0x40448814,0xa642810a,0x2041110a,0x26442104,
	  0x840000,0x1122408,0x90084488,0x40448212,0x62448912,0x84130a,0x36485102,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x101c208,0x4f802801,
	  0x8028040,0x40,0x130a,0x2,0x85e897a0,0x44891224,0x489c2081,0x2040204,0x8104532,0x142850a1,0x24cd1224,0x48823c44,0x89122448,
	  0x91244081,0x2040204,0x8108912,0x24489122,0xc93264,0xc9852214,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100028f,0x109f0080,0x20010c00,
	  0x303071f3,0xc7011c1c,0x4071c306,0x802010,0x3907c1ef,0x1f201e89,0xf3844f90,0xa23c80f2,0x17810e04,0x228223f4,0x840000,0xfbc3c7,
	  0x8f083c88,0x40444212,0x6238f0f2,0x7039d04,0x228423e2,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1008780,0x2201800,0xf0014000,0x1f0,
	  0x1d0a,0x5,0x851e140,0x83060c18,0x30671ef9,0xf3e7cf9f,0x3e7c7911,0xe3c78f1e,0x42f8e1c3,0x8702205c,0x7cf9f3e7,0xcf9b3c78,0xf1e3c204,
	  0x8107111,0xc3870e1c,0x10f1d3a7,0x4e823c08,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x40,0x40000400,0x200000,0x0,0x2,0x0,0x0,0x0,0x0,0x18,
	  0x0,0x4,0x44007f,0x0,0x400,0x400000,0x8010,0x0,0x6002,0x8040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000000,0x200800,0x0,0x0,0x100a,
	  0x400000,0x44,0x0,0x400,0x0,0x0,0x0,0x0,0x0,0x0,0x800,0x0,0x0,0x0,0x0,0x62018,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x31,0x80000800,
	  0x400000,0x0,0x4,0x0,0x0,0x0,0x0,0xc,0x0,0x7,0x3c0000,0x0,0x3800,0x3800000,0x8010,0x0,0x1c001,0x881c0000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x207000,0x0,0x0,0x100a,0xc00000,0x3c,0x0,0xc00,0x0,0x0,0x0,0x0,0x0,0x0,0x1800,0x0,0x0,0x0,0x0,0x1c2070
	};

	// Definition of a 10x13 font (used in dialog boxes).
	const unsigned int font10x13[256*10*13/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80100c0,
	  0x68000300,0x801,0xc00010,0x100c000,0x68100,0x100c0680,0x2,0x403000,0x1000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc,0x0,0x0,0x0,0x0,0x0,0x4020120,
	  0x58120480,0x402,0x1205008,0x2012050,0x58080,0x20120581,0x40000001,0x804812,0x2000000,0x0,0x300,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x140,0x80000,0x200402,0x800000,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x7010,0x7000000,0x8000200,0x20000,0xc0002000,0x8008,0x0,0x0,0x0,0x0,0x808,0x4000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x80000000,0x0,0x0,0x0,0x40000,0x0,0x0,0x0,0x0,0x480,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70,0x80100c0,0x68000480,0x1001,
	  0xc00010,0x1018000,0x68100,0x100c0680,0x4,0x403000,0x1020000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20140,0x28081883,0x200801,
	  0x2a00000,0x10,0x1c0201c0,0x70040f80,0xc0f81c07,0x0,0x70,0x3e0303c0,0x3c3c0f83,0xe03c2107,0xe08810,0x18c31070,0x3c0703c0,
	  0x783e0842,0x22222208,0x83e04010,0x1008000,0x4000200,0x20001,0x2002,0x408008,0x0,0x0,0x100000,0x0,0x1008,0x2000000,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20080,0x38000880,0x8078140f,0x81c00000,0x3e000,0xc020180,0x60080001,0xe0000002,0xc00042,0x108e2010,
	  0xc0300c0,0x300c0303,0xf83c3e0f,0x83e0f81c,0x701c070,0x3c0c41c0,0x701c0701,0xc0001d08,0x42108421,0x8820088,0x4020120,0x58140480,
	  0x802,0x1205008,0x3014050,0xc058080,0x20120581,0x40000002,0x804814,0x2020050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20140,
	  0x281e2484,0x80200801,0x1c02000,0x10,0x22060220,0x880c0801,0x82208,0x80000001,0x20008,0x41030220,0x40220802,0x402102,0x209010,
	  0x18c31088,0x22088220,0x80080842,0x22222208,0x80204010,0x1014000,0x200,0x20001,0x2000,0x8008,0x0,0x0,0x100000,0x0,0x1008,
	  0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x40000500,0x80800010,0x40200000,0x41000,0x12020040,0x10000003,0xa0000006,
	  0x12000c4,0x31014000,0xc0300c0,0x300c0302,0x80402008,0x2008008,0x2008020,0x220c4220,0x88220882,0x20002208,0x42108421,0x8820088,
	  0x0,0x300,0x0,0x0,0x0,0x14000000,0x0,0x200200,0x0,0x20000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0xfc282504,0x80001000,
	  0x82a02000,0x20,0x22020020,0x8140802,0x102208,0x80801006,0x18008,0x9c848220,0x80210802,0x802102,0x20a010,0x15429104,0x22104220,
	  0x80080842,0x22221405,0x404008,0x1022000,0x703c0,0x381e0701,0xc0783c02,0xc09008,0x1d83c070,0x3c078140,0x381c0882,0x21242208,
	  0x81e01008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x201e0,0x40220500,0x80800027,0x20e02800,0x9c800,0x12020040,
	  0x20000883,0xa0200002,0x120a044,0x11064010,0x12048120,0x48120484,0x80802008,0x2008008,0x2008020,0x210a4411,0x4411044,0x10884508,
	  0x42108421,0x503c0b0,0x1c0701c0,0x701c0707,0x70381c07,0x1c07008,0x2008020,0x20f01c0,0x701c0701,0xc0201c08,0x82208822,0x883c088,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x50281903,0x20001000,0x80802000,0x20,0x22020040,0x30240f03,0xc0101c08,0x80801018,
	  0x1fc06010,0xa48483c0,0x80210f03,0xe0803f02,0x20c010,0x15429104,0x22104220,0x70080841,0x41540805,0x804008,0x1041000,0x8220,
	  0x40220881,0x882202,0x40a008,0x12422088,0x22088180,0x40100882,0x21241408,0x80201008,0x2031000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x20280,0x401c0200,0x700028,0x21205000,0x92800,0xc1fc080,0x10000883,0xa0200002,0x1205049,0x12c19010,0x12048120,0x48120484,
	  0xf0803c0f,0x3c0f008,0x2008020,0x790a4411,0x4411044,0x10504908,0x42108421,0x5022088,0x2008020,0x8020080,0x88402208,0x82208808,
	  0x2008020,0x1e088220,0x88220882,0x20002608,0x82208822,0x8822088,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x501c0264,
	  0xa0001000,0x8001fc00,0x7000020,0x22020080,0x83e0082,0x20202207,0x80000020,0x1020,0xa4848220,0x80210802,0x9c2102,0x20c010,
	  0x12425104,0x3c1043c0,0x8080841,0x41540802,0x804008,0x1000000,0x78220,0x40220f81,0x882202,0x40c008,0x12422088,0x22088100,
	  0x60100881,0x41540805,0x406008,0x1849000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20280,0xf0140200,0x880028,0x20e0a03f,0x709c800,
	  0x201c0,0x60000881,0xa0000007,0xc0284b,0x122eb020,0x12048120,0x48120487,0x80802008,0x2008008,0x2008020,0x21094411,0x4411044,
	  0x10204908,0x42108421,0x2022088,0x1e0781e0,0x781e0787,0xf8403e0f,0x83e0f808,0x2008020,0x22088220,0x88220882,0x21fc2a08,0x82208822,
	  0x5022050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20001,0xf80a0294,0x40001000,0x80002000,0x20,0x22020100,0x8040082,0x20202200,
	  0x80000018,0x1fc06020,0xa48fc220,0x80210802,0x842102,0x20a010,0x12425104,0x20104240,0x8080841,0x41541402,0x1004008,0x1000000,
	  0x88220,0x40220801,0x882202,0x40a008,0x12422088,0x22088100,0x18100881,0x41540805,0x801008,0x2046000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x20280,0x401c0f80,0x80880028,0x20005001,0x94800,0x20000,0x880,0xa0000000,0x5015,0x4215040,0x3f0fc3f0,0xfc3f0fc8,
	  0x80802008,0x2008008,0x2008020,0x21094411,0x4411044,0x10505108,0x42108421,0x203c088,0x22088220,0x88220888,0x80402008,0x2008008,
	  0x2008020,0x22088220,0x88220882,0x20002a08,0x82208822,0x5022050,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xa00a0494,0x60001000,
	  0x80002004,0x8020,0x22020200,0x88040882,0x20402201,0x801006,0x18000,0x9f084220,0x40220802,0x442102,0x209010,0x10423088,0x20088220,
	  0x8080840,0x80882202,0x2004008,0x1000000,0x88220,0x40220881,0x882202,0x409008,0x12422088,0x22088100,0x8100880,0x80881402,
	  0x1001008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20280,0x40220200,0x80700027,0x20002801,0x92800,0x1fc000,0x980,
	  0xa0000000,0xa017,0x84417840,0x21084210,0x84210848,0x80402008,0x2008008,0x2008020,0x2208c220,0x88220882,0x20882208,0x42108421,
	  0x2020088,0x22088220,0x88220888,0xc8402208,0x82208808,0x2008020,0x22088220,0x88220882,0x20203208,0x82208822,0x2022020,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0xa03c0463,0x90000801,0x2004,0x8040,0x1c0703e0,0x70040701,0xc0401c06,0x801001,0x20020,
	  0x400843c0,0x3c3c0f82,0x3c2107,0x1c0881e,0x10423070,0x20070210,0xf0080780,0x80882202,0x3e04004,0x1000000,0x783c0,0x381e0701,
	  0x782202,0x408808,0x12422070,0x3c078100,0x700c0780,0x80882202,0x1e01008,0x2000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x201e0,
	  0xf8000200,0x80080010,0x40000001,0x41000,0x0,0xe80,0xa0000000,0x21,0x8e21038,0x21084210,0x84210848,0xf83c3e0f,0x83e0f81c,
	  0x701c070,0x3c08c1c0,0x701c0701,0xc0005c07,0x81e0781e,0x20200b0,0x1e0781e0,0x781e0787,0x30381c07,0x1c07008,0x2008020,0x1c0881c0,
	  0x701c0701,0xc0201c07,0x81e0781e,0x203c020,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000,0x801,0x4,0x40,0x0,0x0,0x0,0x1000,
	  0x0,0x3c000000,0x0,0x0,0x0,0x0,0x10000,0x0,0x0,0x4004,0x1000000,0x0,0x0,0x80000,0x400000,0x0,0x20008000,0x0,0x4,0x1008,0x2000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x8008000f,0x80000000,0x3e000,0x0,0x800,0xa0000400,0x0,0x0,0x0,0x0,0x80000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100000,0x0,0x0,0x0,0x0,0x2000,0x0,0x4020040,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000,
	  0x402,0x8,0x40,0x0,0x0,0x0,0x2000,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,0x0,0x0,0x7004,0x70000fc,0x0,0x0,0x700000,0x800000,0x0,0x20008000,
	  0x0,0x4,0x808,0x4000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x80f00000,0x0,0x0,0x0,0x800,0xa0001800,0x0,0x0,0x0,0x0,
	  0x300000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600000,0x0,0x0,0x0,0x0,0x0,0x0,0x4020040
	};

	// Definition of a 8x17 font
	const unsigned int font8x17[8*17*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x2400,0x2400,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20081834,0x1c0000,0x20081800,0x20081800,0x342008,
	  0x18340000,0x200818,0x80000,0x0,0x180000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4200000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x380000,0x4000,0x2000c00,0x40100840,0x70000000,0x0,0x0,0x1c,0x10700000,0x7,0x0,
	  0x1800,0x1800,0x0,0x0,0x0,0x14,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1010242c,0x14140000,0x10102414,0x10102414,0x2c1010,0x242c1400,
	  0x101024,0x14100038,0x0,0x240000,0x0,0x0,0x30000000,0x0,0x0,0x4000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x12,0x0,0x8100000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x80000,0x10004000,0x2001000,0x40000040,0x10000000,0x0,0x0,0x10,0x10100000,0x4,
	  0x0,0x18000000,0x0,0x0,0x0,0x34002400,0x2400,0x0,0x0,0x0,0x3c,0x0,0x8000000,0x0,0x60607800,0x0,0x140000,0x0,0x0,0x0,0x0,0x0,
	  0x44,0x10081834,0x240000,0x10081800,0x10081800,0x1c341008,0x18340000,0x100818,0x84000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x102812,
	  0x8601c10,0x8100800,0x2,0x1c383e3e,0x67e1e7f,0x3e3c0000,0x38,0x1e087e1e,0x7c7f7f1e,0x417c1c42,0x4063611c,0x7e1c7e3e,0xfe414181,
	  0x63827f10,0x40081000,0x8004000,0x2001000,0x40000040,0x10000000,0x0,0x10000000,0x10,0x10100000,0x3c000008,0x0,0x24003e00,
	  0x3f007f00,0x0,0x0,0x2ce91800,0x1882,0x10101c,0xc2103c,0x143c3c00,0x3c00,0x18003c3c,0x10001f00,0x181c00,0x20200810,0x8080808,
	  0x8083e1e,0x7f7f7f7f,0x7c7c7c7c,0x7c611c1c,0x1c1c1c00,0x1e414141,0x41824044,0x810242c,0x14180000,0x8102414,0x8102414,0x382c0810,
	  0x242c1400,0x81024,0x14104014,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x102816,0x3e902010,0x10084910,0x4,0x22084343,0xa402102,0x41620000,
	  0x44,0x33144121,0x42404021,0x41100444,0x40636122,0x43224361,0x10416381,0x22440310,0x20082800,0x4000,0x2001000,0x40000040,
	  0x10000000,0x0,0x10000000,0x10,0x10100000,0x24000008,0x0,0x606100,0x68000300,0x8106c,0x34000000,0x4f0000,0x44,0x101020,0x441040,
	  0x420200,0x4200,0x24000404,0x7d00,0x82200,0x20203010,0x14141414,0x14082821,0x40404040,0x10101010,0x42612222,0x22222200,0x23414141,
	  0x41447e48,0x0,0x0,0x0,0x0,0x4000000,0x18,0x0,0x4000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10287f,0x49902010,0x10083e10,0x4,0x41080101,
	  0x1a404002,0x41411818,0x1004004,0x21144140,0x41404040,0x41100448,0x40555141,0x41414140,0x10412281,0x14280610,0x20084400,0x1c7c1c,
	  0x3e3c7c3a,0x5c703844,0x107f5c3c,0x7c3e3c3c,0x7e424281,0x66427e10,0x10100000,0x40100008,0x1010,0xa04000,0x48100610,0x100c3024,
	  0x24000000,0x4f3c00,0x2c107e28,0x3820,0x42281060,0x9d1e12,0xbd00,0x24100818,0x427d00,0x82248,0x20200800,0x14141414,0x14142840,
	  0x40404040,0x10101010,0x41514141,0x41414142,0x43414141,0x41284350,0x1c1c1c1c,0x1c1c6c1c,0x3c3c3c3c,0x70707070,0x3c5c3c3c,
	  0x3c3c3c18,0x3e424242,0x42427c42,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x102824,0x48623010,0x10081c10,0x8,0x41080103,0x127c5e04,
	  0x41411818,0xe7f3808,0x4f144140,0x41404040,0x41100450,0x40555141,0x41414160,0x1041225a,0x1c280410,0x1008c600,0x226622,0x66661066,
	  0x62100848,0x10496266,0x66663242,0x10426681,0x24220260,0x100c0000,0xf8280008,0x1010,0x606000,0x48280428,0x28042014,0x48000000,
	  0x494200,0x52280228,0x105420,0x3cee1058,0xa12236,0xa500,0x18101004,0x427d00,0x8226c,0x76767e10,0x14141414,0x14142840,0x40404040,
	  0x10101010,0x41514141,0x41414124,0x45414141,0x41284150,0x22222222,0x22221222,0x66666666,0x10101010,0x66626666,0x66666600,
	  0x66424242,0x42226622,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100024,0x381c4900,0x10086bfe,0x8,0x4908021c,0x22036304,0x3e630000,
	  0x70000710,0x51227e40,0x417f7f43,0x7f100470,0x40554941,0x43417e3e,0x1041225a,0x8100810,0x10080000,0x24240,0x42421042,0x42100850,
	  0x10494242,0x42422040,0x1042245a,0x18240410,0x10103900,0x407c003e,0x1818,0x1c3e10,0x4f7c087c,0x7c002010,0x48000000,0x4008,
	  0x527c0410,0x105078,0x2410104c,0xa13e6c,0x7f00b900,0xfe3c3c,0x421d18,0x1c1c36,0x38383810,0x22222222,0x22144e40,0x7f7f7f7f,
	  0x10101010,0xf1494141,0x41414118,0x49414141,0x4110435c,0x2020202,0x2021240,0x42424242,0x10101010,0x42424242,0x424242ff,0x4e424242,
	  0x42244224,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000fe,0xe664d00,0x10080810,0x380010,0x41080c03,0x42014108,0x633d0000,0x70000710,
	  0x51224140,0x41404041,0x41100448,0x40494541,0x7e414203,0x1041145a,0x14101010,0x10080000,0x3e4240,0x427e1042,0x42100870,0x10494242,
	  0x4242203c,0x1042245a,0x18241810,0x10104600,0xf8f60008,0x1010,0x600320,0x48f610f6,0xf6000000,0x187eff,0x3c04,0x5ef61810,0x105020,
	  0x24fe0064,0x9d006c,0x138ad00,0x100000,0x420518,0x36,0xc0c0c020,0x22222222,0x22224840,0x40404040,0x10101010,0x41454141,0x41414118,
	  0x51414141,0x41107e46,0x3e3e3e3e,0x3e3e7e40,0x7e7e7e7e,0x10101010,0x42424242,0x42424200,0x5a424242,0x42244224,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x28,0x9094500,0x10080010,0x10,0x41081801,0x7f014118,0x41010000,0xe7f3800,0x513e4140,0x41404041,0x41100444,
	  0x40414541,0x40414101,0x10411466,0x36103010,0x8080000,0x424240,0x42401042,0x42100848,0x10494242,0x42422002,0x10423c5a,0x18142010,
	  0x10100000,0x407c0010,0x1010,0x260140,0x487c307c,0x7c000000,0x180000,0x202,0x507c2010,0x105020,0x3c10003c,0x423e36,0x1004200,
	  0x100000,0x420500,0x3e6c,0x41e0440,0x3e3e3e3e,0x3e3e7840,0x40404040,0x10101010,0x41454141,0x41414124,0x61414141,0x41104042,
	  0x42424242,0x42425040,0x40404040,0x10101010,0x42424242,0x42424218,0x72424242,0x42144214,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100048,
	  0x49096200,0x8100010,0x18001810,0x22082043,0x2432310,0x61421818,0x1004010,0x4f634121,0x42404021,0x41104444,0x40414322,0x40234143,
	  0x10411466,0x22106010,0x8080000,0x466622,0x66621066,0x42100844,0x10494266,0x66662042,0x10461824,0x24184010,0x10100000,0x24381010,
	  0x34001018,0xda4320,0x68386038,0x38000000,0x0,0x4204,0x50384010,0x105420,0x4210100c,0x3c0012,0x3c00,0x0,0x460500,0x48,0xc020c44,
	  0x63636363,0x63228821,0x40404040,0x10101010,0x42432222,0x22222242,0x62414141,0x41104042,0x46464646,0x46465022,0x62626262,
	  0x10101010,0x66426666,0x66666618,0x66464646,0x46186618,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100048,0x3e063d00,0x8100000,0x18001820,
	  0x1c3e7f3e,0x23c1e20,0x3e3c1818,0x10,0x20417e1e,0x7c7f401e,0x417c3842,0x7f41431c,0x401e40be,0x103e0866,0x41107f10,0x4080000,
	  0x3a5c1c,0x3a3c103a,0x427c0842,0xe49423c,0x7c3e203c,0xe3a1824,0x66087e10,0x10100000,0x3c103010,0x245a1010,0x5a3e10,0x3f107f10,
	  0x10000000,0x0,0x3c08,0x2e107e10,0x1038fc,0x101004,0x0,0x0,0xfe0000,0x7f0500,0x0,0x14041438,0x41414141,0x41418e1e,0x7f7f7f7f,
	  0x7c7c7c7c,0x7c431c1c,0x1c1c1c00,0xbc3e3e3e,0x3e10405c,0x3a3a3a3a,0x3a3a6e1c,0x3c3c3c3c,0x7c7c7c7c,0x3c423c3c,0x3c3c3c00,
	  0x7c3a3a3a,0x3a087c08,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8000000,0x4200000,0x10000020,0x0,0x0,0x10,0x0,0x30000000,0x0,
	  0x0,0x0,0x60000,0x0,0x1c,0x4380000,0x0,0x2,0x800,0x0,0x40020000,0x0,0x8000c,0x10600000,0x2010,0x48000000,0x240000,0x0,0x0,
	  0x0,0x0,0x0,0x1000,0x1078,0x0,0x0,0x0,0x400500,0x0,0x1e081e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x84008,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8000000,0x0,0x20000040,0x0,0x0,0x20,0x0,0x1e000000,0x0,0x0,0x0,0x20000,0x0,
	  0x0,0x2000000,0x0,0x26,0x800,0x0,0x40020000,0x0,0x100000,0x10000000,0x2030,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000,0x1000,0x0,
	  0x0,0x0,0x400000,0x8000000,0x41e0400,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x104010,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfe,0x0,0x1c,0x7000,0x0,0x40020000,0x0,0x300000,
	  0x0,0xe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1000,0x0,0x0,0x0,0x400000,0x38000000,0x0,0x0,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x1c,0x0,0x0,0x0,0x0,0x0,0x304030,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	// Definition of a 10x19 font
	const unsigned int font10x19[10*19*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3600000,0x36000,0x0,0x0,0x0,0x0,0x6c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x180181c0,0xe81b0300,0x1801,0x81c06c18,0x181c06c,0xe8180,0x181c0e81,0xb0000006,0x60701b,0x1800000,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c00000,0x1c000,0x0,0x0,0x0,0x0,0x6c,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0xc030360,0xb81b0480,0xc03,0x3606c0c,0x303606c,0xb80c0,0x30360b81,0xb0000003,0xc0d81b,0x3000000,0x0,
	  0x300,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800,0x0,0x0,0x0,0x0,0x0,0x2200000,
	  0x22000,0x0,0x0,0x0,0x0,0x0,0x0,0x30000,0x0,0xe0,0x38078000,0x0,0x480,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3000c080,0x480,0x3000,
	  0xc0800030,0xc08000,0x300,0xc080000,0xc,0x302000,0xc00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20120,0x41c01,0xe020060c,
	  0x800000,0x4,0x1e0703e0,0xf8060fc1,0xe1fe1e07,0x80000000,0x78,0x307e0,0x3c7c1fe7,0xf83c408f,0x80f10440,0x18660878,0x7e0787e0,
	  0x78ff9024,0xa0140a0,0x27f83840,0x700e000,0x18000400,0x8000,0x70004002,0x410078,0x0,0x0,0x0,0x0,0x1808,0xc000000,0xf000000,
	  0xe000000,0x1400,0x1e0001f,0x8007f800,0x0,0x0,0x3a3b,0x61400000,0x14202,0x20000,0x38002020,0x3c1b00,0x3e00000,0xf8,0x1c0001c0,
	  0x78060001,0xf800000e,0x1e00020,0x8004020,0xc0300c0,0x300c0301,0xf83c7f9f,0xe7f9fe3e,0xf83e0f8,0x7c1821e0,0x781e0781,0xe0001f10,
	  0x24090240,0xa02400f8,0x18018140,0xe81b0480,0x1801,0x81406c18,0x181406c,0x190e8180,0x18140e81,0xb0000006,0x60501b,0x184006c,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20120,0x26042202,0x200c06,0x800000,0x8,0x210d0611,0x40e0803,0x10026188,0x40000000,
	  0x8c,0xf030418,0xc6431004,0xc64082,0x110840,0x18660884,0x41084410,0x8c081024,0xa012110,0x40082020,0x101b000,0xc000400,0x8000,
	  0x80004002,0x410008,0x0,0x0,0x100000,0x0,0x2008,0x2000000,0x18800000,0x10000000,0x2200,0x2300024,0x800,0x0,0x0,0x2e13,0x60800000,
	  0x8104,0x20040,0x64001040,0x80401b07,0x80100000,0x1e000,0x22000020,0x40c0003,0xc8000002,0x3300020,0x8004020,0xc0300c0,0x300c0301,
	  0x40c64010,0x4010008,0x2008020,0x43182210,0x84210842,0x10002190,0x24090240,0x9044018c,0xc030220,0xb81b0300,0xc03,0x2206c0c,
	  0x302206c,0x1e0b80c0,0x30220b81,0xb0000003,0xc0881b,0x304006c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20120,0x241f2202,
	  0x200802,0x4900000,0x8,0x21010408,0x20a0802,0x44090,0x20000000,0x4,0x11878408,0x80411004,0x804082,0x111040,0x1ce50986,0x40986409,
	  0x81022,0x12012108,0x80102020,0x1031800,0x400,0x8000,0x80004000,0x10008,0x0,0x0,0x100000,0x0,0x2008,0x2000000,0x10000000,
	  0x10000000,0x18,0x4000044,0x1000,0x30180,0xd81b0000,0x13,0xe0000000,0x88,0x40,0x400018c0,0x80400018,0x61f00000,0x61800,0x22020020,
	  0x4000007,0xc8000002,0x2100020,0x8038000,0x1e0781e0,0x781e0301,0x40804010,0x4010008,0x2008020,0x41142619,0x86619866,0x18002190,
	  0x24090240,0x8887e104,0x0,0x0,0x0,0x0,0x0,0x2000000,0x0,0x0,0x0,0x40000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20120,0x2434a202,
	  0x200802,0x3e00000,0x10,0x40810008,0x21a0804,0x44090,0x20000000,0x80040004,0x20848409,0x409004,0x1004082,0x112040,0x14a50902,
	  0x40902409,0x81022,0x11321208,0x80202010,0x1060c00,0x7c5e0,0x781e8783,0xf07a5f0e,0x1c10808,0xfc5f078,0x5e07a170,0x7c7e1024,
	  0xa016190,0x27f82008,0x2000000,0x20000000,0x10000000,0x80200024,0x4000044,0x2000,0x18180,0xc8320000,0x12,0xa1f00037,0x7f888,
	  0x1e0,0x40410880,0x80600017,0xa2100000,0x5e800,0x22020040,0x38001027,0xc8000002,0x2100020,0x8004020,0x12048120,0x48120482,
	  0x41004010,0x4010008,0x2008020,0x40942409,0x2409024,0x9044390,0x24090240,0x88841918,0x1f07c1f0,0x7c1f07c3,0x70781e07,0x81e07838,
	  0xe0380e0,0x1f17c1e0,0x781e0781,0xe0001f90,0x24090240,0x9025e102,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20001,0xff241c41,
	  0x1001,0x1c02000,0x10,0x40810008,0x6120f85,0xe0086190,0x20c03007,0x8007800c,0x27848419,0x409004,0x1004082,0x114040,0x14a48902,
	  0x40902409,0x81022,0x11321205,0x602010,0x1000000,0x86610,0x84218840,0x80866182,0x411008,0x9261884,0x61086189,0x82101022,0x12012108,
	  0x40082008,0x2000000,0x20030000,0x20000000,0x80200024,0x4000044,0x3006030,0xc018100,0x4c260000,0x12,0x26080048,0x83000850,
	  0x20250,0x403e0500,0x8078002c,0x12302200,0x92400,0x1c0200c0,0x4001027,0xc8000002,0x3308820,0x8004020,0x12048120,0x48120482,
	  0x41004010,0x4010008,0x2008020,0x40922409,0x2409024,0x8884690,0x24090240,0x85040920,0x21886218,0x86218860,0x88842108,0x42108408,
	  0x2008020,0x21186210,0x84210842,0x10302190,0x24090240,0x88461084,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x4c240182,
	  0x80001001,0x6b02000,0x20,0x4c810010,0x78220846,0x10081e10,0x20c0301c,0x1fe0e018,0x4d8487e1,0x409fe7,0xf9007f82,0x11a040,
	  0x13248902,0x41102418,0xe0081022,0x11320c05,0x402008,0x1000000,0x2409,0x409020,0x81024082,0x412008,0x9240902,0x40902101,0x101022,
	  0x11321208,0x40102008,0x2000000,0x7e0c8000,0xfc000003,0xf0fc0018,0x43802047,0x8c8040c8,0x32008300,0x44240000,0x0,0x4000048,
	  0x8c801050,0x20440,0x40221dc0,0x808c0028,0x11d0667f,0x8009c400,0x1fc180,0x4001023,0xc8300002,0x1e0ccfb,0x3ec7b020,0x12048120,
	  0x48120482,0x79007f9f,0xe7f9fe08,0x2008020,0xf0922409,0x2409024,0x8504490,0x24090240,0x85040920,0x802008,0x2008020,0x89004090,
	  0x24090208,0x2008020,0x40902409,0x2409024,0x8304390,0x24090240,0x88440884,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,
	  0x481c0606,0xc8001001,0x802000,0x20,0x4c810020,0x4220024,0x8102108,0x60000070,0x3820,0x48884419,0x409004,0x10e4082,0x112040,
	  0x13244902,0x7e1027e0,0x3c081021,0x21320c02,0x802008,0x1000000,0x7e409,0x409020,0x81024082,0x414008,0x9240902,0x40902101,
	  0x80101022,0x11320c08,0x40202008,0x2038800,0x200bc000,0x20000000,0x80200003,0x80f04044,0xbc080bc,0x2f000200,0x0,0x0,0x6001048,
	  0x8bc02020,0x20441,0xf8220200,0x80820028,0x1000cc00,0x80094400,0x201e0,0x78001021,0xc830000f,0x8000663c,0xf03c0c0,0x21084210,
	  0x84210846,0x41004010,0x4010008,0x2008020,0x40912409,0x2409024,0x8204890,0x24090240,0x82040930,0x1f87e1f8,0x7e1f87e0,0x89004090,
	  0x24090208,0x2008020,0x40902409,0x2409024,0x8004690,0x24090240,0x88440884,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,
	  0x480719c4,0x48001001,0x81fc00,0x7800020,0x40810040,0x2420024,0x8104087,0xa0000070,0x3820,0x48884409,0x409004,0x1024082,0x111040,
	  0x13244902,0x40102410,0x2081021,0x214a1202,0x1802008,0x1000000,0x182409,0x409fe0,0x81024082,0x41a008,0x9240902,0x40902100,
	  0xf8101021,0x214a0c04,0x80c0c008,0x1847000,0x7c1ee000,0x20000000,0x8020000c,0x8c044,0x1ee181ee,0x7b800000,0x707,0xf3ff0000,
	  0x3e0084f,0x9ee0c020,0x20440,0x40221fc0,0xc2002c,0x13f11000,0x87892400,0x20000,0x1020,0x48000000,0x3f011c6,0x31cc6180,0x21084210,
	  0x84210844,0x41004010,0x4010008,0x2008020,0x40912409,0x2409024,0x8505090,0x24090240,0x8204191c,0x60982609,0x82609823,0xf9007f9f,
	  0xe7f9fe08,0x2008020,0x40902409,0x2409024,0x9fe4c90,0x24090240,0x84840848,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0xfe048224,
	  0x28001001,0x2000,0x40,0x40810080,0x27f8024,0x8104080,0x2000001c,0x1fe0e020,0x488fc409,0x409004,0x1024082,0x110840,0x10242902,
	  0x40102408,0x2081021,0x214a1202,0x1002004,0x1000000,0x102409,0x409000,0x81024082,0x411008,0x9240902,0x40902100,0x6101021,
	  0x214a0c04,0x81002008,0x2000000,0x201dc000,0x20000000,0x80200000,0x98044,0x1dc101dc,0x77000000,0x700,0x0,0x180448,0x1dc10020,
	  0x20440,0x403e0200,0x620017,0xa000cc00,0x80052800,0x20000,0x1020,0x48000000,0x6606,0x206100,0x3f0fc3f0,0xfc3f0fc7,0xc1004010,
	  0x4010008,0x2008020,0x4090a409,0x2409024,0x8886090,0x24090240,0x8207e106,0x40902409,0x2409024,0x81004010,0x4010008,0x2008020,
	  0x40902409,0x2409024,0x8005890,0x24090240,0x84840848,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x98048224,0x30001001,0x2000,
	  0x40,0x21010100,0x2020024,0x8204080,0x40000007,0x80078000,0x48884408,0x80411004,0x824082,0x110840,0x10242986,0x40086409,0x2081021,
	  0xe14a2102,0x2002004,0x1000000,0x106409,0x409000,0x81024082,0x410808,0x9240902,0x40902100,0x2101021,0x214a1202,0x82002008,
	  0x2000000,0x300f8000,0x20000000,0x80fc001d,0xe4088044,0xf8200f8,0x3e000000,0x300,0x0,0x80c48,0xf820020,0x20640,0x40410200,
	  0x803c0018,0x60006600,0x61800,0x0,0x1020,0x48000000,0xcc0a,0x20a100,0x21084210,0x84210844,0x40804010,0x4010008,0x2008020,
	  0x4110a619,0x86619866,0x19046110,0x24090240,0x82040102,0x41906419,0x6419064,0x81004010,0x4010008,0x2008020,0x40902409,0x2409024,
	  0x8307090,0x24090240,0x82840828,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000,0x90248222,0x30000802,0x200c,0xc080,0x21010301,
	  0x4021042,0x10202108,0xc0c03000,0x80040020,0x4d902418,0xc6431004,0xc24082,0x6210440,0x10241884,0x40084409,0x86080840,0xc0842102,
	  0x4002002,0x1000000,0x18e610,0x84218820,0x80864082,0x410408,0x9240884,0x61086101,0x6101860,0xc0842103,0x4002008,0x2000000,
	  0x10850180,0x20330000,0x80200013,0x26184024,0x5040050,0x14000000,0x0,0x0,0x4180848,0x85040020,0x20350,0x40000200,0x800c0007,
	  0x80002200,0x1e000,0x0,0x1860,0x48000000,0x880a,0x40a188,0x40902409,0x2409028,0x40c64010,0x4010008,0x2008020,0x43106210,0x84210842,
	  0x10006108,0x42108421,0x2040102,0x6398e639,0x8e6398e4,0x88842088,0x22088208,0x2008020,0x21102210,0x84210842,0x10306118,0x66198661,
	  0x83061030,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20001,0x901f01c1,0xe8000802,0xc,0xc080,0x1e07c7f8,0xf8020f81,0xe0401e07,
	  0x80c03000,0x20,0x279027e0,0x3c7c1fe4,0x3c408f,0x83c1027f,0x90241878,0x4007c404,0xf8080780,0xc0844082,0x7f82002,0x1000000,
	  0xfa5e0,0x781e87c0,0x807a409f,0xc0410207,0x9240878,0x5e07a100,0xf80e0fa0,0xc0846183,0x7f82008,0x2000000,0xf020100,0x40321360,
	  0x80200014,0xa3e0201f,0x8207f820,0x8000000,0x0,0x0,0x3e01037,0x207f820,0x201e1,0xfc000200,0x80040000,0x0,0x0,0x1fc000,0x17b0,
	  0x48000000,0x12,0xc120f0,0x40902409,0x2409028,0x783c7f9f,0xe7f9fe3e,0xf83e0f8,0x7c1061e0,0x781e0781,0xe000be07,0x81e0781e,
	  0x204017c,0x3e8fa3e8,0xfa3e8fa3,0x70781f07,0xc1f07c7f,0x1fc7f1fc,0x1e1021e0,0x781e0781,0xe0007e0f,0xa3e8fa3e,0x8305e030,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40000,0xc06,0xc,0x100,0x0,0x0,0x0,0x3000,0x0,0x20000000,0x0,0x0,0x0,0x0,0xc000,
	  0x0,0x0,0x2001,0x1000000,0x0,0x0,0x20000,0x400000,0x0,0x40002000,0x0,0x1,0x2008,0x2000000,0x100,0x40240000,0x80200008,0x40000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x80040000,0x0,0x0,0x0,0x1000,0x48000000,0x1f,0x181f000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1040010,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40000,0x60c,0x18,0x0,
	  0x0,0x0,0x0,0x6000,0x0,0x10000000,0x0,0x0,0x0,0x0,0x4000,0x0,0x0,0x3800,0x7000000,0x0,0x0,0x840000,0x400000,0x0,0x40002000,
	  0x0,0x2,0x2008,0x2000000,0x200,0x40440000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x80780000,0x0,0x0,0x0,0x1000,0x48000400,
	  0x2,0x1e02000,0x0,0x0,0x80000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000,0x0,0x0,0x0,0x0,0x0,0x0,0x2040020,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x4000,0x0,0xf000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x780000,0x3800000,0x0,0x40002000,0x0,0xe,0x1808,0xc000000,0x3,0x80000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80000000,
	  0x0,0x0,0x0,0x1000,0x1c00,0x0,0x0,0x0,0x0,0x380000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x380000,0x0,0x0,0x0,0x0,0x0,0x0,0xe0400e0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3fc,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	   
	// Definition of a 12x24 font
	 const unsigned int font12x24[12*24*256/32] = {
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x19,0x80000000,0x198000,0x0,0x0,0x0,0x0,
	   0x0,0x198,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc001806,0xc81980,0x60000000,0xc001806,0x1980c00,0x18060198,0xc80c,
	   0x180600,0xc8198000,0xc001,0x80601980,0x18000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf,0x0,0xf0000,0x0,0x0,0x0,0x0,0x0,0x198,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x600300f,0x1301980,0x90000000,0x600300f,0x1980600,0x300f0198,0x13006,0x300f01,0x30198000,0x6003,
	   0xf01980,0x30000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x0,0x60000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7007,0x3c0000,0x3006019,
	   0x80000000,0x90000000,0x3006019,0x80000300,0x60198000,0x3,0x601980,0x0,0x3006,0x1980000,0x60000000,0x0,0x0,0xe0000000,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000000,
	   0x0,0x0,0x0,0x0,0x0,0xc800019,0x80000000,0x198000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x0,0x0,0x1001,0x420000,0x0,0x0,0x90000000,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18000c06,0xc80001,0x10000000,0x18000c06,0x1800,0xc060000,0xc818,0xc0600,0xc8000000,
	   0x18000,0xc0600000,0xc000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6019,0x80660207,0x800f8060,0x300c004,0x0,0x6,
	   0xe00703f,0x3f00383,0xf80f07fc,0x1f01f000,0x0,0xf8,0x607f,0x7c7e07,0xfe7fe0f8,0x6063fc1f,0x86066007,0xe7060f0,0x7f80f07f,
	   0x81f8fff6,0x6606c03,0x70ee077f,0xe0786000,0xf0070000,0xc000060,0xc0,0x3e000,0x60006003,0x600fc00,0x0,0x0,0x0,0x0,0x0,0x3c0603,
	   0xc0000000,0x7800000,0xf0000,0x0,0xf00001f,0x80001fe0,0x7fe000,0x0,0x0,0x0,0x168fe609,0x0,0x90e07,0x6000,0x3c000e,0x70000f8,
	   0x1980001f,0x0,0x1f8,0xf00000f,0xf00180,0xfe000,0xe00e,0x1001,0x20060,0x6006006,0x600600,0x600fe07c,0x7fe7fe7f,0xe7fe3fc3,
	   0xfc3fc3fc,0x7e07060f,0xf00f00,0xf00f0000,0xf360660,0x6606606e,0x76001e0,0xc00180f,0x1681981,0x10000000,0xc00180f,0x1980c00,
	   0x180f0198,0x3801680c,0x180f01,0x68198000,0xc001,0x80f01980,0x18600198,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6019,
	   0x8044020c,0xc01f8060,0x2004004,0x0,0xc,0x3f81f07f,0x87f80383,0xf81f87fc,0x3f83f800,0x0,0x1fc,0x780607f,0x81fe7f87,0xfe7fe1fc,
	   0x6063fc1f,0x860c6007,0xe7061f8,0x7fc1f87f,0xc3fcfff6,0x6606c03,0x30c6067f,0xe0783000,0xf00d8000,0x6000060,0xc0,0x7e000,0x60006003,
	   0x600fc00,0x0,0x0,0xc00,0x0,0x0,0x7c0603,0xe0000000,0xfc00000,0x1f0000,0x0,0x900003f,0xc0003fe0,0x7fe000,0x0,0x0,0x0,0x1302660f,
	   0x0,0xf0606,0x6004,0x7e0006,0x60601f8,0x19800001,0x80000000,0x1f8,0x19800010,0x81080300,0x3f2000,0x2011,0x1001,0x1c0060,0x6006006,
	   0x600600,0x601fe1fe,0x7fe7fe7f,0xe7fe3fc3,0xfc3fc3fc,0x7f87061f,0x81f81f81,0xf81f8000,0x3fa60660,0x66066066,0x66003f0,0x6003009,
	   0x1301981,0x10000000,0x6003009,0x1980600,0x30090198,0x1f013006,0x300901,0x30198000,0x6003,0x901980,0x30600198,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6019,0x80cc0f8c,0xc0180060,0x6006044,0x40000000,0xc,0x3181b041,0xc41c0783,0x388018,
	   0x71c71800,0x0,0x106,0x18c0f061,0xc38261c6,0x600384,0x60606001,0x86186007,0xe78630c,0x60e30c60,0xe7040606,0x630cc03,0x39c30c00,
	   0xc0603000,0x3018c000,0x3000060,0xc0,0x60000,0x60000000,0x6000c00,0x0,0x0,0xc00,0x0,0x0,0x600600,0x60000000,0x18400000,0x180000,
	   0x0,0x19800070,0x40003600,0xc000,0x0,0x0,0x0,0x25a06,0x0,0x6030c,0x4,0xe20007,0xe060180,0xf000,0x80000000,0xf0000,0x10800000,
	   0x80080600,0x7f2000,0x2020,0x80001001,0x20000,0xf00f00f,0xf00f00,0x601b0382,0x60060060,0x6000600,0x60060060,0x61c78630,0xc30c30c3,
	   0xc30c000,0x30e60660,0x66066063,0xc600738,0x3006019,0x80000000,0xe0000000,0x3006019,0x80000300,0x60198000,0x3e000003,0x601980,
	   0x0,0x3006,0x1980000,0x60600000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6019,0x80cc1fcc,0xc0180060,0x6006035,0x80000000,
	   0x18,0x71c03000,0xc00c0583,0x300018,0x60c60c00,0x0,0x6,0x3060f060,0xc30060c6,0x600300,0x60606001,0x86306007,0x9e78670e,0x60670e60,
	   0x66000606,0x630c606,0x19830c01,0xc0601800,0x30306000,0x60,0xc0,0x60000,0x60000000,0x6000c00,0x0,0x0,0xc00,0x0,0x0,0x600600,
	   0x60000000,0x18000000,0x300000,0x0,0x78060,0x6600,0x1c000,0x300c,0x39819c0,0x0,0x25a00,0x0,0x30c,0x4,0xc00003,0xc060180,0x30c1f,
	   0x80000000,0x30c000,0x10800001,0x80700000,0x7f2000,0x2020,0x80001001,0x20060,0xf00f00f,0xf00f00,0xf01b0300,0x60060060,0x6000600,
	   0x60060060,0x60c78670,0xe70e70e7,0xe70e000,0x70c60660,0x66066063,0xc7f8618,0x0,0x0,0x0,0x0,0x0,0x0,0x7000000,0x0,0x0,0x0,
	   0x0,0x600000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6019,0x87ff3a4c,0xc0180060,0x400600e,0x600000,0x18,0x60c03000,
	   0xc00c0d83,0x700018,0x60c60c00,0x20,0x400006,0x3060f060,0xc6006066,0x600600,0x60606001,0x86606006,0x966c6606,0x60660660,0x66000606,
	   0x630c666,0xf019801,0x80601800,0x30603000,0x1f06f,0xf01ec0,0xf03fe1ec,0x6703e01f,0x61c0c06,0xdc6701f0,0x6f01ec0c,0xe1f87fc6,
	   0xc60cc03,0x71c60c7f,0xc0600600,0x60000000,0x30000000,0x300000,0x40040,0x88060,0x6600,0x18000,0x300c,0x1981980,0x0,0x2421f,
	   0x80003ce0,0x7fc198,0x601f,0xc02021,0x980600c0,0x40230,0x80000000,0x402000,0x19806003,0x80006,0xc7f2000,0x2020,0x80001001,
	   0x420060,0xf00f00f,0xf00f00,0xf01b0600,0x60060060,0x6000600,0x60060060,0x6066c660,0x66066066,0x6606208,0x60e60660,0x66066061,
	   0x987fc670,0x1f01f01f,0x1f01f01,0xf039c0f0,0xf00f00f,0xf03e03,0xe03e03e0,0x1f06701f,0x1f01f01,0xf01f0060,0x1e660c60,0xc60c60c6,
	   0xc6f060c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x7ff3207,0x8c0c0000,0xc00300e,0x600000,0x30,0x60c03000,
	   0xc01c0983,0xf0600030,0x31860c06,0x6001e0,0x78000e,0x23e1f861,0xc6006066,0x600600,0x60606001,0x86c06006,0x966c6606,0x60660660,
	   0xe7000606,0x630c666,0xf01f803,0x600c00,0x30000000,0x3f87f,0x83f83fc3,0xf83fe3fc,0x7f83e01f,0x6380c07,0xfe7f83f8,0x7f83fc0d,
	   0xf3fc7fc6,0xc71cc03,0x3183187f,0xc0600600,0x60000000,0xff806000,0x300000,0x40040,0x88070,0x6600,0x60030060,0x6001818,0x1883180,
	   0x0,0x2423f,0xc0007ff0,0x607fc1f8,0x603f,0x80c01fc1,0xf80601e0,0x5f220,0x80420000,0x5f2000,0xf006006,0x80006,0xc7f2000,0x2020,
	   0x82107c07,0xc03c0060,0x1f81f81f,0x81f81f80,0xf03b0600,0x60060060,0x6000600,0x60060060,0x6066c660,0x66066066,0x660671c,0x61660660,
	   0x66066061,0xf860e6c0,0x3f83f83f,0x83f83f83,0xf87fe3f8,0x3f83f83f,0x83f83e03,0xe03e03e0,0x3f87f83f,0x83f83f83,0xf83f8060,
	   0x3fc60c60,0xc60c60c3,0x187f8318,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x883200,0x300c0000,0xc003035,0x80600000,
	   0x30,0x66c03001,0xc0f81983,0xf86f0030,0x1f071c06,0x600787,0xfe1e001c,0x6261987f,0x86006067,0xfe7fc600,0x7fe06001,0x87c06006,
	   0xf6646606,0x60e6067f,0xc3e00606,0x61986f6,0x600f007,0x600c00,0x30000000,0x21c71,0x830831c3,0x1c06031c,0x71c06003,0x6700c06,
	   0x6671c318,0x71831c0f,0x16040c06,0xc318606,0x1b031803,0x80600600,0x60000000,0x30009000,0x300000,0x40040,0x7003e,0x67e0,0x90070090,
	   0x9001818,0x8c3100,0x0,0x60,0x4000e730,0x900380f0,0x6034,0x80c018c7,0xfe060338,0xb0121,0x80c60000,0x909000,0x6008,0x1080006,
	   0xc3f2000,0x2011,0x3180060,0x60060e0,0x19819819,0x81981981,0x9833c600,0x7fe7fe7f,0xe7fe0600,0x60060060,0x60664660,0x66066066,
	   0x66063b8,0x62660660,0x66066060,0xf06066c0,0x21c21c21,0xc21c21c2,0x1c466308,0x31c31c31,0xc31c0600,0x60060060,0x31871c31,0x83183183,
	   0x18318000,0x71860c60,0xc60c60c3,0x18718318,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x1981a00,0xe03e0000,0xc003044,
	   0x40600000,0x60,0x66c03001,0x80f03182,0x1c7f8030,0x3f83fc06,0x601e07,0xfe078038,0x6661987f,0x86006067,0xfe7fc61e,0x7fe06001,
	   0x87e06006,0x66666606,0x7fc6067f,0x81f80606,0x61986f6,0x6006006,0x600600,0x30000000,0xc60,0xc60060c6,0xc06060c,0x60c06003,
	   0x6e00c06,0x6660c60c,0x60c60c0e,0x6000c06,0xc318666,0x1f031803,0x600600,0x603c2000,0x30016800,0x1fe0000,0x1f81f8,0x1c1f,0x804067e1,
	   0x68060168,0x16800810,0xc42300,0x0,0x60,0x20c331,0x68030060,0x6064,0x3fc1040,0xf006031c,0xa011e,0x818c7fe0,0x909000,0x7fe1f,
	   0x80f00006,0xc0f2060,0xf80e,0x18c0780,0x780781c0,0x19819819,0x81981981,0x9833c600,0x7fe7fe7f,0xe7fe0600,0x60060060,0xfc666660,
	   0x66066066,0x66061f0,0x66660660,0x66066060,0x606066e0,0xc00c00,0xc00c00c0,0xc066600,0x60c60c60,0xc60c0600,0x60060060,0x60c60c60,
	   0xc60c60c6,0xc60c000,0x61c60c60,0xc60c60c3,0x1860c318,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x1980f81,0x80373000,
	   0xc003004,0x7fe0001,0xf0000060,0x60c03003,0x183180,0xc71c060,0x3181ec00,0x7000,0xe070,0x66619860,0xc6006066,0x60061e,0x60606001,
	   0x87606006,0x66626606,0x7f860661,0xc01c0606,0x6198696,0xf00600e,0x600600,0x30000000,0x1fc60,0xc60060c7,0xfc06060c,0x60c06003,
	   0x7c00c06,0x6660c60c,0x60c60c0c,0x7f00c06,0xc3b8666,0xe01b007,0x3c00600,0x3c7fe000,0xff03ec00,0x1fe0000,0x40040,0xe001,0xc0806603,
	   0xec0e03ec,0x3ec00010,0x0,0x60000000,0x7f,0x10c3f3,0xec070060,0x6064,0x3fc1040,0x6000030c,0xa0100,0x3187fe1,0xf09f1000,0x7fe00,
	   0x6,0xc012060,0x0,0xc63c03,0xc03c0380,0x19819819,0x81981981,0x98330600,0x60060060,0x6000600,0x60060060,0xfc662660,0x66066066,
	   0x66060e0,0x6c660660,0x66066060,0x6060e630,0x1fc1fc1f,0xc1fc1fc1,0xfc3fe600,0x7fc7fc7f,0xc7fc0600,0x60060060,0x60c60c60,0xc60c60c6,
	   0xc60c7fe,0x62c60c60,0xc60c60c1,0xb060c1b0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0xffe02c6,0x3c633000,0xc003004,
	   0x7fe0001,0xf00000c0,0x60c03006,0xc6180,0xc60c060,0x60c00c00,0x7000,0xe060,0x66639c60,0x66006066,0x600606,0x60606001,0x86306006,
	   0x66636606,0x60060660,0xc0060606,0x61f8696,0xf00600c,0x600300,0x30000000,0x3fc60,0xc60060c7,0xfc06060c,0x60c06003,0x7c00c06,
	   0x6660c60c,0x60c60c0c,0x1f80c06,0xc1b0666,0xe01b00e,0x3c00600,0x3c43c000,0x3007de00,0x600000,0x40040,0x30000,0x61006607,0xde0c07de,
	   0x7de00000,0x0,0xf07fefff,0x1f,0x8008c3f7,0xde0e0060,0x6064,0xc01047,0xfe00018c,0xb013f,0x86300061,0xf0911000,0x6000,0x6,
	   0xc012060,0x3f,0x8063c0cc,0x3cc0c700,0x39c39c39,0xc39c39c1,0x98630600,0x60060060,0x6000600,0x60060060,0x60663660,0x66066066,
	   0x66061f0,0x78660660,0x66066060,0x607fc618,0x3fc3fc3f,0xc3fc3fc3,0xfc7fe600,0x7fc7fc7f,0xc7fc0600,0x60060060,0x60c60c60,0xc60c60c6,
	   0xc60c7fe,0x64c60c60,0xc60c60c1,0xb060c1b0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0xffe0260,0x6661b000,0xc003000,
	   0x600000,0xc0,0x60c0300c,0xc7fe0,0xc60c060,0x60c01c00,0x1e07,0xfe078060,0x6663fc60,0x66006066,0x600606,0x60606001,0x86386006,
	   0x6636606,0x60060660,0xe0060606,0x60f039c,0x1b806018,0x600300,0x30000000,0x70c60,0xc60060c6,0x6060c,0x60c06003,0x7600c06,
	   0x6660c60c,0x60c60c0c,0x1c0c06,0xc1b03fc,0xe01f01c,0xe00600,0x70000000,0x3007fc00,0x600000,0x40040,0x0,0x62006607,0xfc1807fc,
	   0x7fc00000,0x0,0xf0000000,0x1,0xc004c307,0xfc1c0060,0x6064,0xc018c0,0x600000d8,0x5f200,0x3180060,0x50a000,0x6000,0x6,0xc012000,
	   0x0,0xc601c0,0x4201c600,0x3fc3fc3f,0xc3fc3fc3,0xfc7f0600,0x60060060,0x6000600,0x60060060,0x60663660,0x66066066,0x66063b8,
	   0x70660660,0x66066060,0x607f860c,0x70c70c70,0xc70c70c7,0xcc60600,0x60060060,0x6000600,0x60060060,0x60c60c60,0xc60c60c6,0xc60c000,
	   0x68c60c60,0xc60c60c1,0xf060c1f0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3300260,0x6661e000,0xc003000,0x600000,
	   0x180,0x71c03018,0xc7fe0,0xc60c0c0,0x60c01800,0x787,0xfe1e0060,0x6663fc60,0x630060c6,0x600306,0x60606001,0x86186006,0x661e70e,
	   0x60070c60,0x60060606,0x60f039c,0x19806038,0x600180,0x30000000,0x60c60,0xc60060c6,0x6060c,0x60c06003,0x6700c06,0x6660c60c,
	   0x60c60c0c,0xc0c06,0xc1b039c,0x1f00e018,0x600600,0x60000000,0x1803f800,0x600000,0x40040,0x39e00,0x63006603,0xf83803f8,0x3f800000,
	   0x0,0x60000000,0x0,0xc00cc303,0xf8180060,0x6064,0xc01fc0,0x60060070,0x40200,0x18c0060,0x402000,0x6000,0x6,0xc012000,0x0,0x18c0140,
	   0x2014600,0x3fc3fc3f,0xc3fc3fc3,0xfc7f0300,0x60060060,0x6000600,0x60060060,0x60c61e70,0xe70e70e7,0xe70e71c,0x60e60660,0x66066060,
	   0x6060060c,0x60c60c60,0xc60c60c6,0xcc60600,0x60060060,0x6000600,0x60060060,0x60c60c60,0xc60c60c6,0xc60c000,0x70c60c60,0xc60c60c0,
	   0xe060c0e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x33022e0,0x6670c000,0xc003000,0x600600,0x60180,0x31803030,
	   0x41c0184,0x1831c0c0,0x71c23806,0x6001e0,0x780000,0x62630c60,0xe38261c6,0x600386,0x60606043,0x860c6006,0x661e30c,0x60030c60,
	   0x740e0607,0xe0f039c,0x31c06030,0x600180,0x30000000,0x61c71,0x830831c3,0x406031c,0x60c06003,0x6300c06,0x6660c318,0x71831c0c,
	   0x41c0c07,0x1c0e039c,0x1b00e030,0x600600,0x60000000,0x1c41b00e,0x601cc0,0x401f8,0x45240,0xe1803601,0xb03001b0,0x1b000000,
	   0x0,0x0,0x41,0xc008e711,0xb0300060,0x6034,0x80c02020,0x60060030,0x30c00,0xc60000,0x30c000,0x0,0x7,0x1c012000,0x0,0x3180240,
	   0x6024608,0x30c30c30,0xc30c30c3,0xc630382,0x60060060,0x6000600,0x60060060,0x61c61e30,0xc30c30c3,0xc30c208,0x70c70e70,0xe70e70e0,
	   0x6060068c,0x61c61c61,0xc61c61c6,0x1cc62308,0x30430430,0x43040600,0x60060060,0x31860c31,0x83183183,0x18318060,0x31c71c71,
	   0xc71c71c0,0xe07180e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x2203fc0,0x663f6000,0x6006000,0x600600,0x60300,
	   0x3f81fe7f,0xc7f80187,0xf83f80c0,0x3f83f006,0x600020,0x400060,0x33e6067f,0xc1fe7f87,0xfe6001fe,0x6063fc7f,0x60e7fe6,0x660e3f8,
	   0x6001f860,0x37fc0603,0xfc06030c,0x30c0607f,0xe06000c0,0x30000000,0x7fc7f,0x83f83fc3,0xfc0603fc,0x60c7fe03,0x61807c6,0x6660c3f8,
	   0x7f83fc0c,0x7f80fc3,0xfc0e039c,0x3180607f,0xc0600600,0x60000000,0xfc0e00c,0x601986,0x66040040,0x4527f,0xc0803fe0,0xe07fe0e0,
	   0xe000000,0x0,0x0,0x7f,0x80107ff0,0xe07fc060,0x603f,0x83fe0000,0x60060018,0xf000,0x420000,0xf0000,0x7fe00,0x7,0xfe012000,
	   0x0,0x2100640,0xc0643f8,0x60660660,0x66066067,0xec3e1fe,0x7fe7fe7f,0xe7fe3fc3,0xfc3fc3fc,0x7f860e3f,0x83f83f83,0xf83f8000,
	   0x5fc3fc3f,0xc3fc3fc0,0x606006fc,0x7fc7fc7f,0xc7fc7fc7,0xfcffe3f8,0x3fc3fc3f,0xc3fc7fe7,0xfe7fe7fe,0x3f860c3f,0x83f83f83,
	   0xf83f8060,0x7f83fc3f,0xc3fc3fc0,0x607f8060,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x2201f80,0x3c1e7000,0x6006000,
	   0x600,0x60300,0xe01fe7f,0xc3f00183,0xe01f0180,0x1f01e006,0x600000,0x60,0x3006067f,0x807c7e07,0xfe6000f8,0x6063fc3e,0x6067fe6,
	   0x660e0f0,0x6000f060,0x3bf80601,0xf806030c,0x60e0607f,0xe06000c0,0x30000000,0x1ec6f,0xf01ec0,0xf80601ec,0x60c7fe03,0x61c03c6,
	   0x6660c1f0,0x6f01ec0c,0x3f007c1,0xcc0e030c,0x71c0c07f,0xc0600600,0x60000000,0x7804018,0xe01186,0x66040040,0x39e3f,0x80401fe0,
	   0x407fe040,0x4000000,0x0,0x0,0x3f,0x203ce0,0x407fc060,0x601f,0x3fe0000,0x60060018,0x0,0x0,0x0,0x7fe00,0x6,0xe6012000,0x0,
	   0x7e0,0x1807e1f0,0x60660660,0x66066066,0x6c3e07c,0x7fe7fe7f,0xe7fe3fc3,0xfc3fc3fc,0x7e060e0f,0xf00f00,0xf00f0000,0x8f01f81f,
	   0x81f81f80,0x60600670,0x1ec1ec1e,0xc1ec1ec1,0xec79c0f0,0xf80f80f,0x80f87fe7,0xfe7fe7fe,0x1f060c1f,0x1f01f01,0xf01f0000,0x4f01cc1c,
	   0xc1cc1cc0,0xc06f00c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200,0x0,0x6006000,0x600,0x600,0x0,0x0,0x0,0x0,
	   0x600000,0x0,0x18000000,0x0,0x0,0x0,0x0,0x0,0x1800,0x0,0x0,0x0,0x600060,0x30000000,0x0,0x0,0xc,0x3,0x0,0x0,0x60000c00,0x0,
	   0x0,0xc000,0x600600,0x60000000,0x18,0xc03100,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x601f8,0x0,0x0,0x0,0x0,0x6,
	   0x12000,0x2000000,0x40,0x20004000,0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0xc06000c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200,0x0,0x2004000,0xc00,0x0,0x0,0x0,0x0,0x0,0xc00000,
	   0x0,0x1c000000,0x0,0x0,0x0,0x0,0x0,0xc00,0x0,0x0,0x0,0x780000,0xf0000000,0x0,0x0,0x21c,0x3,0x0,0x0,0x60000c00,0x0,0x0,0xc000,
	   0x7c0603,0xe0000000,0x10,0xc02300,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x601f0,0x0,0x0,0x0,0x0,0x6,0x12000,0x1000000,
	   0x40,0x7e004000,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc06000c0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200,0x0,0x300c000,0xc00,0x0,0x0,0x0,0x0,0x0,0xc00000,0x0,0x7800000,0x0,
	   0x0,0x0,0x0,0x0,0x800,0x0,0x0,0x0,0x780000,0xf0000000,0x0,0x0,0x3f8,0x3e,0x0,0x0,0x60000c00,0x0,0x0,0x38000,0x3c0603,0xc0000000,
	   0x10,0xfc00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x60000,0x0,0x0,0x0,0x0,0x6,0x0,0x1000000,0x0,0x0,0x0,0x0,
	   0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x80600380,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xffc,0x0,
	   0x0,0x1f0,0x3c,0x0,0x0,0x60000c00,0x0,0x0,0x38000,0x600,0x0,0x0,0xf000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x6,0x0,0xe000000,0x0,0x0,0x0,0x0,0x70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x3,0x80600380,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xffc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	   0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	// Definition of a 16x32
	const unsigned int font16x32[16*32*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc300000,0x0,0xc300000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70000e0,0x3c00730,0xe7001c0,0x0,0x70000e0,0x3c00e70,0x70000e0,0x3c00e70,0x730,0x70000e0,0x3c00730,
	  0xe700000,0x700,0xe003c0,0xe7000e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x6600000,0x0,0x6600000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x18001c0,0x6600ff0,0xe7003e0,0x0,0x18001c0,0x6600e70,0x18001c0,0x6600e70,0xff0,0x18001c0,0x6600ff0,0xe700000,0x180,
	  0x1c00660,0xe7001c0,0x0,0x0,0x0,0x380,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,
	  0x0,0x3c00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c00380,
	  0xc300ce0,0xe700630,0x0,0x1c00380,0xc300e70,0x1c00380,0xc300e70,0xce0,0x1c00380,0xc300ce0,0xe700000,0x1c0,0x3800c30,0xe700380,
	  0x0,0x0,0x0,0x7c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0xe000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1800000,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0xc300000,0x0,0xc300000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x700000,0x0,0x0,0x0,0x7c007c00,0x3e000000,
	  0x0,0x0,0x630,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe000070,0x1800000,0xc60,0x0,0xe000070,0x1800000,0xe000070,
	  0x1800000,0x0,0xe000070,0x1800000,0x0,0xe00,0x700180,0x70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x800000,0x0,0x600600,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x3f0,0xfc0,0x0,0x7000000,0x38000000,0x1c0000,0xfc0000,0x380001c0,0xe01c00,0x7f800000,0x0,0x0,0x0,0x0,0x0,0x0,0x7c,
	  0x1801f00,0x0,0x0,0x1c,0x0,0x0,0x3c00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7300000,0x6600000,0x0,0x6600000,0x0,0x0,0x0,0x0,0xe700000,
	  0x0,0x0,0x0,0x0,0x0,0xe00000,0x0,0x0,0x0,0xc000c00,0x43800000,0x0,0x0,0x630,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0xf80,0x70000e0,0x3c00730,0xe700c60,0x0,0x70000e0,0x3c00e70,0x70000e0,0x3c00e70,0xe000730,0x70000e0,0x3c00730,0xe700000,0x700,
	  0xe003c0,0xe7000e0,0x38000e70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x6300000,0x803c00,0x7c00180,
	  0xc00300,0x1000000,0x0,0x1c,0x3c007c0,0xfc007e0,0xe01ff8,0x3f03ffc,0x7e007c0,0x0,0x0,0x7c0,0x1c0,0x7f8003f0,0x7f007ff8,0x7ff803f0,
	  0x70381ffc,0xff0700e,0x7000783c,0x783807c0,0x7fc007c0,0x7fc00fc0,0x7fff7038,0x700ee007,0x780f780f,0x7ffc03f0,0x70000fc0,0x3c00000,
	  0x3000000,0x38000000,0x1c0000,0x1fc0000,0x380001c0,0xe01c00,0x7f800000,0x0,0x0,0x0,0x0,0x0,0x0,0xfc,0x1801f80,0x0,0x1f80000,
	  0x7e,0x0,0x0,0x2400000,0xfc00000,0x7ff0000,0x7ffc0000,0x0,0x0,0x0,0x0,0xf30fb0c,0x2400000,0x0,0x240780f,0x1c0,0xfc,0x780f,
	  0x18003f0,0xe700000,0x7c00000,0x0,0xff0,0x3c00000,0x78007c0,0xc00000,0xff80000,0xf80,0x7c00000,0xc000c00,0x18001c0,0x1c001c0,
	  0x1c001c0,0x1c003e0,0x7fe03f0,0x7ff87ff8,0x7ff87ff8,0x1ffc1ffc,0x1ffc1ffc,0x7f007838,0x7c007c0,0x7c007c0,0x7c00000,0x7c67038,
	  0x70387038,0x7038780f,0x70001fe0,0x30000c0,0x2400f30,0xe700c60,0x0,0x30000c0,0x2400e70,0x30000c0,0x2400e70,0xf700f30,0x30000c0,
	  0x2400f30,0xe700000,0x300,0xc00240,0xe7000c0,0x38000e70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,
	  0x630018c,0x807e00,0xfe00180,0xc00300,0x1000000,0x0,0x38,0xff01fc0,0x3ff01ff0,0x1e01ff8,0x7f83ffc,0x1ff80ff0,0x0,0x0,0xff0,
	  0x1f003e0,0x7fe00ff8,0x7fc07ff8,0x7ff80ff8,0x70381ffc,0xff0701c,0x7000783c,0x78381ff0,0x7fe01ff0,0x7fe01ff0,0x7fff7038,0x781ee007,
	  0x3c1e380e,0x7ffc0380,0x380001c0,0x3c00000,0x1800000,0x38000000,0x1c0000,0x3c00000,0x380001c0,0xe01c00,0x3800000,0x0,0x0,
	  0x0,0x7000000,0x0,0x0,0x1e0,0x18003c0,0x0,0x3fc0000,0x70,0x0,0x0,0x6600000,0x1ff00000,0x1fff0000,0x7ffc0000,0x0,0x0,0x0,0x0,
	  0xcf0239c,0x3c00000,0x0,0x3c0380e,0x1c0,0x2001fe,0x380e,0x18007f8,0xe700000,0x8600000,0x0,0xff0,0x7e00000,0x8c00870,0x1800000,
	  0x1ff80000,0x180,0xc600000,0xc000c00,0x38001c0,0x3e003e0,0x3e003e0,0x3e001c0,0x7fe0ff8,0x7ff87ff8,0x7ff87ff8,0x1ffc1ffc,0x1ffc1ffc,
	  0x7fc07838,0x1ff01ff0,0x1ff01ff0,0x1ff00000,0x1fec7038,0x70387038,0x7038380e,0x70003ce0,0x1800180,0x6600cf0,0xe7007c0,0x0,
	  0x1800180,0x6600e70,0x1800180,0x6600e70,0x7c00cf0,0x1800180,0x6600cf0,0xe700000,0x180,0x1800660,0xe700180,0x38000e70,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x630030c,0x3f0e700,0x1e200180,0x1800180,0x21100000,0x0,
	  0x38,0x1e7819c0,0x38781038,0x1e01c00,0xf080038,0x1c381c38,0x0,0x0,0x1878,0x7fc03e0,0x70e01e18,0x70e07000,0x70001e18,0x703801c0,
	  0x707038,0x70007c7c,0x7c381c70,0x70701c70,0x70703830,0x1c07038,0x381ce007,0x1c1c3c1e,0x3c0380,0x380001c0,0x7e00000,0xc00000,
	  0x38000000,0x1c0000,0x3800000,0x38000000,0x1c00,0x3800000,0x0,0x0,0x0,0x7000000,0x0,0x0,0x1c0,0x18001c0,0x0,0x70c0000,0xe0,
	  0x0,0x0,0xc300000,0x38300000,0x3c700000,0x3c0000,0x0,0x0,0x0,0x0,0xce022f4,0x1800000,0x0,0x1803c1e,0x1c0,0x2003c2,0x3c1e,
	  0x1800e08,0x7e0,0x300000,0x0,0x7e00000,0xe700000,0x600030,0x3000000,0x3f980000,0x180,0x18200000,0xc000c00,0x1e0001c0,0x3e003e0,
	  0x3e003e0,0x3e003e0,0xfe01e18,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x70e07c38,0x1c701c70,0x1c701c70,0x1c700000,0x3c787038,
	  0x70387038,0x70383c1e,0x70003870,0xc00300,0xc300ce0,0x380,0x0,0xc00300,0xc300000,0xc00300,0xc300000,0xfc00ce0,0xc00300,0xc300ce0,
	  0x0,0xc0,0x3000c30,0x300,0x38000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x630031c,0xff8c300,
	  0x1c000180,0x1800180,0x39380000,0x0,0x70,0x1c3801c0,0x203c001c,0x3e01c00,0x1c000038,0x381c3838,0x0,0x0,0x1038,0xe0e03e0,0x70703c08,
	  0x70707000,0x70003808,0x703801c0,0x707070,0x70007c7c,0x7c383838,0x70383838,0x70387010,0x1c07038,0x381c700e,0x1e3c1c1c,0x780380,
	  0x1c0001c0,0xe700000,0x0,0x38000000,0x1c0000,0x3800000,0x38000000,0x1c00,0x3800000,0x0,0x0,0x0,0x7000000,0x0,0x0,0x1c0,0x18001c0,
	  0x0,0xe000000,0xe0,0x0,0x1000100,0x3800,0x70100000,0x38700000,0x780000,0x1c0,0x7801ce0,0xe380000,0x0,0x2264,0x0,0x0,0x1c1c,
	  0x0,0x200780,0x1c1c,0x1800c00,0x1818,0x7f00000,0x0,0x18180000,0xc300000,0x600070,0x0,0x7f980000,0x180,0x18300000,0xc000c00,
	  0x3000000,0x3e003e0,0x3e003e0,0x3e003e0,0xee03c08,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x70707c38,0x38383838,0x38383838,
	  0x38380000,0x38387038,0x70387038,0x70381c1c,0x7fc03870,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xbc00000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x38000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x6300318,0xe88c300,0x1c000180,0x38001c0,
	  0xfe00180,0x0,0x70,0x1c3801c0,0x1c001c,0x6e01c00,0x1c000078,0x381c3818,0x0,0x40000,0x40000038,0x1c0607e0,0x70703800,0x70707000,
	  0x70003800,0x703801c0,0x7070e0,0x70007c7c,0x7c383838,0x70383838,0x70387000,0x1c07038,0x381c700e,0xf780e38,0x700380,0x1c0001c0,
	  0x1c380000,0x0,0x38000000,0x1c0000,0x3800000,0x38000000,0x1c00,0x3800000,0x0,0x0,0x0,0x7000000,0x0,0x0,0x1c0,0x18001c0,0x0,
	  0xe000000,0xe0,0x0,0x1000100,0x4400,0x70000000,0x38700000,0x700000,0xe0,0x7001c70,0xe380000,0x0,0x2264,0x0,0x0,0xe38,0x0,
	  0x200700,0xe38,0x1800c00,0x300c,0xc300000,0x0,0x300c0000,0xc300180,0x6003c0,0x0,0x7f980000,0x180,0x18300000,0xc000c00,0x1800000,
	  0x7e007e0,0x7e007e0,0x7e003e0,0xee03800,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x70707c38,0x38383838,0x38383838,0x38380000,
	  0x38387038,0x70387038,0x70380e38,0x7ff039f0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1e00000,0x0,0x0,0x0,0x40000,0x0,0x0,0x38000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x6300318,0x1c80e700,0x1c000180,0x38001c0,0x3800180,
	  0x0,0xe0,0x381c01c0,0x1c001c,0x6e01c00,0x38000070,0x381c381c,0x0,0x3c0000,0x78000078,0x38030770,0x70707800,0x70387000,0x70007000,
	  0x703801c0,0x7071c0,0x7000745c,0x7638701c,0x7038701c,0x70387000,0x1c07038,0x1c38718e,0x7700f78,0xf00380,0xe0001c0,0x381c0000,
	  0x7e0,0x39e003e0,0x79c03f0,0x3ffc079c,0x39e01fc0,0xfe01c1e,0x3807778,0x39e007e0,0x39e0079c,0x73c07e0,0x7ff83838,0x701ce007,
	  0x783c701c,0x1ffc01c0,0x18001c0,0x0,0x1c000100,0xe0,0x0,0x1000100,0x4200,0x70000000,0x70700100,0xf00100,0x10000e0,0x7000c70,
	  0xc700000,0x0,0x2204,0x7e00000,0x1e380100,0x1ffc0f78,0x0,0xf80700,0xf78,0x1800e00,0x63e6,0x18300000,0x0,0x6fe60000,0xe700180,
	  0xc00060,0x3838,0x7f980000,0x180,0x18300000,0xc000c00,0x18001c0,0x7700770,0x7700770,0x77007f0,0xee07800,0x70007000,0x70007000,
	  0x1c001c0,0x1c001c0,0x70387638,0x701c701c,0x701c701c,0x701c1008,0x707c7038,0x70387038,0x70380f78,0x707039c0,0x7e007e0,0x7e007e0,
	  0x7e007e0,0x1f3c03e0,0x3f003f0,0x3f003f0,0x1fc01fc0,0x1fc01fc0,0x7f039e0,0x7e007e0,0x7e007e0,0x7e00380,0x7ce3838,0x38383838,
	  0x3838701c,0x39e0701c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x6307fff,0x1c807e0c,0xe000180,
	  0x30000c0,0x3800180,0x0,0xe0,0x381c01c0,0x1c001c,0xce01fe0,0x38000070,0x381c381c,0x3800380,0xfc0000,0x7e0000f0,0x30030770,
	  0x70707000,0x70387000,0x70007000,0x703801c0,0x707380,0x700076dc,0x7638701c,0x7038701c,0x70387800,0x1c07038,0x1c3873ce,0x7f00770,
	  0xe00380,0xe0001c0,0x700e0000,0x1ff8,0x3ff00ff0,0xffc0ff8,0x3ffc0ffc,0x3bf01fc0,0xfe01c3c,0x3807f78,0x3bf00ff0,0x3ff00ffc,
	  0x77e0ff0,0x7ff83838,0x3838e007,0x3c783838,0x1ffc01c0,0x18001c0,0x0,0x7ff00380,0x1e0,0x0,0x1000100,0x4200,0x78000000,0x70700380,
	  0xe00380,0x3800060,0xe000e30,0x1c600000,0x0,0x2204,0xff00000,0x7f7c0380,0x1ffc0770,0x1c0,0x3fc0700,0x18040770,0x1800780,0x4e12,
	  0x18300104,0x0,0x4c320000,0x7e00180,0x1c00030,0x3838,0x7f980000,0x180,0x18302080,0xc000c00,0x18001c0,0x7700770,0x7700770,
	  0x7700770,0x1ee07000,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x70387638,0x701c701c,0x701c701c,0x701c381c,0x705c7038,0x70387038,
	  0x70380770,0x70383b80,0x1ff81ff8,0x1ff81ff8,0x1ff81ff8,0x3fbe0ff0,0xff80ff8,0xff80ff8,0x1fc01fc0,0x1fc01fc0,0xff83bf0,0xff00ff0,
	  0xff00ff0,0xff00380,0xffc3838,0x38383838,0x38383838,0x3ff03838,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x1c0,0x7fff,0x1c803c38,0xf000000,0x70000e0,0xfe00180,0x0,0x1c0,0x381c01c0,0x3c0078,0xce01ff0,0x39e000f0,0x1c38381c,0x3800380,
	  0x3e07ffc,0xf8001f0,0x307b0770,0x70e07000,0x70387000,0x70007000,0x703801c0,0x707700,0x700076dc,0x7638701c,0x7038701c,0x70387e00,
	  0x1c07038,0x1c3873ce,0x3e007f0,0x1e00380,0x70001c0,0x0,0x1038,0x3c381e18,0x1c7c1e3c,0x3801e3c,0x3c7801c0,0xe01c78,0x380739c,
	  0x3c781c38,0x3c381c3c,0x7c21e10,0x7003838,0x3838700e,0x1ef03838,0x3c01c0,0x18001c0,0x0,0x7fe007c0,0x1c0,0x0,0x1000100,0x6400,
	  0x7e000000,0x707007c0,0x1e007c0,0x7c00070,0xe000638,0x18600000,0x0,0x0,0x1e100000,0x73ce07c0,0x3c07f0,0x1c0,0x7240700,0x1ddc3ffe,
	  0x1800de0,0x8c01,0x1870030c,0x0,0x8c310000,0x3c00180,0x3800030,0x3838,0x7f980000,0x180,0x183030c0,0xc000c00,0x430001c0,0x7700770,
	  0x7700770,0x7700770,0x1ce07000,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x70387638,0x701c701c,0x701c701c,0x701c1c38,0x70dc7038,
	  0x70387038,0x703807f0,0x70383b80,0x10381038,0x10381038,0x10381038,0x21e71e18,0x1e3c1e3c,0x1e3c1e3c,0x1c001c0,0x1c001c0,0x1e383c78,
	  0x1c381c38,0x1c381c38,0x1c380380,0x1c383838,0x38383838,0x38383838,0x3c383838,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x1c0,0x630,0x1e8000e0,0x1f000000,0x70000e0,0x39380180,0x0,0x1c0,0x3b9c01c0,0x3c07f0,0x18e01078,0x3bf800e0,
	  0x7e0383c,0x3800380,0x1f807ffc,0x3f001c0,0x61ff0e38,0x7fc07000,0x70387ff0,0x7ff07000,0x7ff801c0,0x707f00,0x7000729c,0x7338701c,
	  0x7070701c,0x70703fc0,0x1c07038,0x1e7873ce,0x1c003e0,0x3c00380,0x70001c0,0x0,0x1c,0x3c381c00,0x1c3c1c1c,0x3801c3c,0x383801c0,
	  0xe01cf0,0x380739c,0x38381c38,0x3c381c3c,0x7801c00,0x7003838,0x3838700e,0xfe03c78,0x7801c0,0x18001c0,0x0,0x1c000c20,0xff8,
	  0x0,0x1ff01ff0,0x3818,0x3fc00100,0x707e0c20,0x3c00c20,0xc200030,0xc000618,0x18c00000,0x0,0x0,0x1c000080,0xe1ce0c20,0x7803e0,
	  0x1c0,0xe200700,0xff83ffe,0x1801878,0x9801,0x1cf0071c,0x7ffc0000,0x8c310000,0x7ffe,0x7000030,0x3838,0x3f980380,0x180,0xc6038e0,
	  0x7f9c7f9c,0x3e1c01c0,0xe380e38,0xe380e38,0xe380f78,0x1cfc7000,0x7ff07ff0,0x7ff07ff0,0x1c001c0,0x1c001c0,0xfe387338,0x701c701c,
	  0x701c701c,0x701c0e70,0x719c7038,0x70387038,0x703803e0,0x70383b80,0x1c001c,0x1c001c,0x1c001c,0xe71c00,0x1c1c1c1c,0x1c1c1c1c,
	  0x1c001c0,0x1c001c0,0x1c383838,0x1c381c38,0x1c381c38,0x1c380000,0x3c383838,0x38383838,0x38383c78,0x3c383c78,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x630,0xf800380,0x3f830000,0x70000e0,0x31080180,0x0,0x380,0x3b9c01c0,
	  0x7807e0,0x38e00038,0x3c3800e0,0xff01c3c,0x3800380,0x7c000000,0x7c03c0,0x61870e38,0x7fc07000,0x70387ff0,0x7ff070fc,0x7ff801c0,
	  0x707f80,0x7000739c,0x7338701c,0x7ff0701c,0x7fe00ff0,0x1c07038,0xe7073ce,0x1c003e0,0x3800380,0x38001c0,0x0,0x1c,0x381c3800,
	  0x381c380e,0x380381c,0x383801c0,0xe01de0,0x380739c,0x3838381c,0x381c381c,0x7001e00,0x7003838,0x1c70718e,0x7e01c70,0xf00380,
	  0x18001e0,0x1e000000,0x1c001bb0,0xff8,0x0,0x1000100,0xe0,0xff00300,0x707e1bb0,0x3801bb0,0x1bb00010,0x8000308,0x30c00000,0x0,
	  0x0,0x1e0000c0,0xe1ce1bb0,0xf003e0,0x1c0,0x1c203ff8,0x63003e0,0x180181c,0x9801,0xfb00e38,0x7ffc0000,0x8fc10000,0x7ffe,0xe000860,
	  0x3838,0x1f980380,0x180,0x7c01c70,0x1f001f0,0x1f003c0,0xe380e38,0xe380e38,0xe380e38,0x1cfc7000,0x7ff07ff0,0x7ff07ff0,0x1c001c0,
	  0x1c001c0,0xfe387338,0x701c701c,0x701c701c,0x701c07e0,0x731c7038,0x70387038,0x703803e0,0x70383980,0x1c001c,0x1c001c,0x1c001c,
	  0xe73800,0x380e380e,0x380e380e,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c0000,0x387c3838,0x38383838,0x38381c70,
	  0x381c1c70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0xc30,0x7f00e00,0x33c30000,0x70000e0,0x1007ffe,
	  0x0,0x380,0x3b9c01c0,0xf00078,0x30e0001c,0x3c1c01c0,0x1c381fdc,0x0,0x70000000,0x1c0380,0x63030e38,0x70707000,0x70387000,0x700070fc,
	  0x703801c0,0x707b80,0x7000739c,0x7338701c,0x7fc0701c,0x7fc001f0,0x1c07038,0xe703e5c,0x3e001c0,0x7800380,0x38001c0,0x0,0x7fc,
	  0x381c3800,0x381c380e,0x380381c,0x383801c0,0xe01fe0,0x380739c,0x3838381c,0x381c381c,0x7001fc0,0x7003838,0x1c70718e,0x7c01c70,
	  0xe01f00,0x180007c,0x7f8c0000,0x7fc03fb8,0x1c0,0x0,0x1000100,0x700,0x1f00600,0x70703fb8,0x7803fb8,0x3fb80000,0x8000000,0x180,
	  0x0,0x0,0x1fc00060,0xe1ce3fb8,0xe001c0,0x1c0,0x1c203ff8,0xc1801c0,0x180c,0x9801,0x1c70,0xc0000,0x8cc10000,0x180,0xfe007c0,
	  0x3838,0x7980380,0xff0,0xe38,0x3e003e00,0x3e000380,0xe380e38,0xe380e38,0xe380e38,0x38e07000,0x70007000,0x70007000,0x1c001c0,
	  0x1c001c0,0x70387338,0x701c701c,0x701c701c,0x701c03c0,0x731c7038,0x70387038,0x703801c0,0x703838e0,0x7fc07fc,0x7fc07fc,0x7fc07fc,
	  0xe73800,0x380e380e,0x380e380e,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c7ffc,0x38dc3838,0x38383838,0x38381c70,
	  0x381c1c70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0xc60,0xf83878,0x71e30000,0x70000e0,0x1007ffe,
	  0x7f0,0x380,0x381c01c0,0x1e0003c,0x60e0001c,0x381c01c0,0x381c079c,0x0,0x7c000000,0x7c0380,0x63031c1c,0x70307000,0x70387000,
	  0x7000701c,0x703801c0,0x7071c0,0x7000739c,0x71b8701c,0x7000701c,0x71e00078,0x1c07038,0xe703e7c,0x7e001c0,0xf000380,0x38001c0,
	  0x0,0x1ffc,0x381c3800,0x381c3ffe,0x380381c,0x383801c0,0xe01fc0,0x380739c,0x3838381c,0x381c381c,0x7000ff0,0x7003838,0x1ef03bdc,
	  0x3800ee0,0x1e01f00,0x180007c,0x61fc0000,0x7fc07f3c,0x1c0,0x0,0x1000100,0x1800,0x780c00,0x70707f3c,0xf007f3c,0x7f3c0000,0x0,
	  0x3c0,0x3ffcffff,0x0,0xff00030,0xe1fe7f3c,0x1e001c0,0x1c0,0x1c200700,0xc183ffe,0xe0c,0x9801,0x1ff038e0,0xc07f0,0x8c610000,
	  0x180,0x0,0x3838,0x1980380,0x0,0x1ff0071c,0xe000e000,0xe0000f80,0x1c1c1c1c,0x1c1c1c1c,0x1c1c1e38,0x38e07000,0x70007000,0x70007000,
	  0x1c001c0,0x1c001c0,0x703871b8,0x701c701c,0x701c701c,0x701c03c0,0x761c7038,0x70387038,0x703801c0,0x70703870,0x1ffc1ffc,0x1ffc1ffc,
	  0x1ffc1ffc,0xfff3800,0x3ffe3ffe,0x3ffe3ffe,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c7ffc,0x389c3838,0x38383838,
	  0x38380ee0,0x381c0ee0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0xfffc,0xbc60fc,0x70e30000,0x70000e0,
	  0x180,0x7f0,0x700,0x381c01c0,0x3e0001c,0x7ffc001c,0x381c03c0,0x381c001c,0x0,0x1f807ffc,0x3f00380,0x63031ffc,0x70387000,0x70387000,
	  0x7000701c,0x703801c0,0x7071e0,0x7000701c,0x71b8701c,0x7000701c,0x70f00038,0x1c07038,0x7e03e7c,0x77001c0,0xe000380,0x1c001c0,
	  0x0,0x3c1c,0x381c3800,0x381c3ffe,0x380381c,0x383801c0,0xe01fe0,0x380739c,0x3838381c,0x381c381c,0x70003f8,0x7003838,0xee03bdc,
	  0x3c00ee0,0x3c00380,0x18000e0,0xf00000,0x1c007e7c,0x3c0,0x0,0x1000100,0x0,0x381800,0x70707e7c,0xe007e7c,0x7e7c0000,0x0,0x7c0,
	  0x0,0x0,0x3f80018,0xe1fe7e7c,0x3c001c0,0x1c0,0x1c200700,0xc183ffe,0xf0c,0x8c01,0x38e0,0xc07f0,0x8c710000,0x180,0x0,0x3838,
	  0x1980000,0x0,0x71c,0x7000f0,0x700f00,0x1ffc1ffc,0x1ffc1ffc,0x1ffc1ffc,0x3fe07000,0x70007000,0x70007000,0x1c001c0,0x1c001c0,
	  0x703871b8,0x701c701c,0x701c701c,0x701c07e0,0x7c1c7038,0x70387038,0x703801c0,0x7ff03838,0x3c1c3c1c,0x3c1c3c1c,0x3c1c3c1c,
	  0x3fff3800,0x3ffe3ffe,0x3ffe3ffe,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c0000,0x391c3838,0x38383838,0x38380ee0,
	  0x381c0ee0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfffc,0x9c01ce,0x70f60000,0x70000e0,0x180,
	  0x0,0x700,0x381c01c0,0x780001c,0x7ffc001c,0x381c0380,0x381c003c,0x0,0x3e07ffc,0xf800380,0x63031ffc,0x70387000,0x70387000,
	  0x7000701c,0x703801c0,0x7070f0,0x7000701c,0x71b8701c,0x7000701c,0x70700038,0x1c07038,0x7e03e7c,0xf7801c0,0x1e000380,0x1c001c0,
	  0x0,0x381c,0x381c3800,0x381c3800,0x380381c,0x383801c0,0xe01fe0,0x380739c,0x3838381c,0x381c381c,0x7000078,0x7003838,0xee03a5c,
	  0x7c00fe0,0x78001c0,0x18001c0,0x0,0x1c003ef8,0x380,0x0,0x1000100,0x810,0x383000,0x70703ef8,0x1e003ef8,0x3ef80000,0x0,0x7c0,
	  0x0,0x0,0x78000c,0xe1c03ef8,0x78001c0,0x1c0,0x1c200700,0x63001c0,0x18003f8,0x4e12,0x1c70,0xc0000,0x4c320000,0x180,0x0,0x3838,
	  0x1980000,0x0,0xe38,0x700118,0x701e00,0x1ffc1ffc,0x1ffc1ffc,0x1ffc1ffc,0x7fe07000,0x70007000,0x70007000,0x1c001c0,0x1c001c0,
	  0x703871b8,0x701c701c,0x701c701c,0x701c0e70,0x7c1c7038,0x70387038,0x703801c0,0x7fc0381c,0x381c381c,0x381c381c,0x381c381c,
	  0x78e03800,0x38003800,0x38003800,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c0000,0x3b1c3838,0x38383838,0x38380fe0,
	  0x381c0fe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1860,0x9c0186,0x707e0000,0x30000c0,0x180,
	  0x0,0xe00,0x183801c0,0xf00001c,0xe0001c,0x181c0380,0x381c0038,0x0,0xfc0000,0x7e000000,0x61873c1e,0x70383800,0x70707000,0x7000381c,
	  0x703801c0,0x707070,0x7000701c,0x70f83838,0x70003838,0x70780038,0x1c07038,0x7e03c3c,0xe3801c0,0x1c000380,0xe001c0,0x0,0x381c,
	  0x381c3800,0x381c3800,0x380381c,0x383801c0,0xe01ef0,0x380739c,0x3838381c,0x381c381c,0x7000038,0x7003838,0xfe03e7c,0xfe007c0,
	  0x70001c0,0x18001c0,0x0,0xe001ff0,0x380,0x0,0x1000100,0x162c,0x381800,0x30701ff0,0x1c001ff0,0x1ff00000,0x0,0x3c0,0x0,0x0,
	  0x380018,0xe1c01ff0,0x70001c0,0x1c0,0x1c200700,0xff801c0,0x18000f0,0x63e6,0xe38,0x0,0x6c3e0000,0x0,0x0,0x3838,0x1980000,0x0,
	  0x1c70,0xf0000c,0xf01c00,0x3c1e3c1e,0x3c1e3c1e,0x3c1e3c1c,0x70e03800,0x70007000,0x70007000,0x1c001c0,0x1c001c0,0x707070f8,
	  0x38383838,0x38383838,0x38381c38,0x38387038,0x70387038,0x703801c0,0x7000381c,0x381c381c,0x381c381c,0x381c381c,0x70e03800,
	  0x38003800,0x38003800,0x1c001c0,0x1c001c0,0x381c3838,0x381c381c,0x381c381c,0x381c0380,0x3e1c3838,0x38383838,0x383807c0,0x381c07c0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18c0,0x9c0186,0x783c0000,0x38001c0,0x180,0x3800000,
	  0x3800e00,0x1c3801c0,0x1e00003c,0xe00038,0x1c1c0780,0x381c0038,0x3800380,0x3c0000,0x78000000,0x61ff380e,0x70383808,0x70707000,
	  0x7000381c,0x703801c0,0x40707078,0x7000701c,0x70f83838,0x70003838,0x70384038,0x1c07038,0x7e03c3c,0x1e3c01c0,0x3c000380,0xe001c0,
	  0x0,0x383c,0x3c381c00,0x1c3c1c00,0x3801c3c,0x383801c0,0xe01c78,0x380739c,0x38381c38,0x3c381c3c,0x7000038,0x7003878,0x7c01e78,
	  0x1ef007c0,0xf0001c0,0x18001c0,0x0,0xe000ee0,0x7800380,0xe380000,0x1001ff0,0x2242,0x40380c00,0x38700ee0,0x3c000ee0,0xee00000,
	  0x0,0x0,0x0,0x0,0x380030,0xe1c00ee0,0xf0001c0,0x1c0,0xe200700,0xdd801c0,0x1800038,0x300c,0x71c,0x0,0x300c0000,0x0,0x0,0x3838,
	  0x1980000,0x0,0x38e0,0xb0000c,0xb01c08,0x380e380e,0x380e380e,0x380e380e,0x70e03808,0x70007000,0x70007000,0x1c001c0,0x1c001c0,
	  0x707070f8,0x38383838,0x38383838,0x3838381c,0x38387038,0x70387038,0x703801c0,0x7000381c,0x383c383c,0x383c383c,0x383c383c,
	  0x70e01c00,0x1c001c00,0x1c001c00,0x1c001c0,0x1c001c0,0x1c383838,0x1c381c38,0x1c381c38,0x1c380380,0x1c383878,0x38783878,0x387807c0,
	  0x3c3807c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x18c0,0x10b801ce,0x3c3e0000,0x38001c0,0x180,
	  0x3800000,0x3801c00,0x1e7801c0,0x3c002078,0xe02078,0x1c380700,0x1c3810f0,0x3800380,0x40000,0x40000380,0x307b380e,0x70701e18,
	  0x70e07000,0x70001c1c,0x703801c0,0x60e0703c,0x7000701c,0x70f83c78,0x70003c70,0x703c70f0,0x1c03870,0x3c01c3c,0x3c1c01c0,0x78000380,
	  0x7001c0,0x0,0x3c7c,0x3c381e18,0x1c7c1e0c,0x3801c3c,0x383801c0,0xe01c38,0x3c0739c,0x38381c38,0x3c381c3c,0x7001078,0x7803c78,
	  0x7c01c38,0x1c780380,0x1e0001c0,0x18001c0,0x0,0x70c06c0,0x7000380,0xe300000,0x1000100,0x2142,0x70f00600,0x3c7006c0,0x780006c0,
	  0x6c00000,0x0,0x0,0x0,0x0,0x10780060,0x73e206c0,0x1e0001c0,0x1c0,0x7240700,0x180c01c0,0x1800018,0x1818,0x30c,0x0,0x18180000,
	  0x0,0x0,0x3c78,0x1980000,0x0,0x30c0,0x130000c,0x1301c18,0x380e380e,0x380e380e,0x380e380e,0x70e01e18,0x70007000,0x70007000,
	  0x1c001c0,0x1c001c0,0x70e070f8,0x3c783c78,0x3c783c78,0x3c781008,0x7c783870,0x38703870,0x387001c0,0x70003a3c,0x3c7c3c7c,0x3c7c3c7c,
	  0x3c7c3c7c,0x79f11e18,0x1e0c1e0c,0x1e0c1e0c,0x1c001c0,0x1c001c0,0x1c783838,0x1c381c38,0x1c381c38,0x1c380380,0x1c383c78,0x3c783c78,
	  0x3c780380,0x3c380380,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x38c0,0x1ff800fc,0x1fee0000,
	  0x1800180,0x180,0x3800000,0x3801c00,0xff01ffc,0x3ffc3ff0,0xe03ff0,0xff00700,0x1ff81fe0,0x3800380,0x0,0x380,0x3000780f,0x7ff00ff8,
	  0x7fc07ff8,0x70000ffc,0x70381ffc,0x7fe0701c,0x7ff8701c,0x70781ff0,0x70001ff0,0x701c7ff0,0x1c01fe0,0x3c01c38,0x380e01c0,0x7ffc0380,
	  0x7001c0,0x0,0x1fdc,0x3ff00ff0,0xffc0ffc,0x3800fdc,0x38383ffe,0xe01c3c,0x1fc739c,0x38380ff0,0x3ff00ffc,0x7001ff0,0x3f81fb8,
	  0x7c01c38,0x3c3c0380,0x1ffc01c0,0x18001c0,0x0,0x3fc0380,0x7000380,0xc70718c,0x1000100,0x2244,0x7ff00200,0x1fff0380,0x7ffc0380,
	  0x3800000,0x0,0x0,0x0,0x0,0x1ff000c0,0x7f7e0380,0x1ffc01c0,0x1c0,0x3fc3ffe,0x1c0,0x1800018,0x7e0,0x104,0x0,0x7e00000,0x7ffe,
	  0x0,0x3fde,0x1980000,0x0,0x2080,0x3300018,0x3300ff0,0x780f780f,0x780f780f,0x780f780e,0xf0fe0ff8,0x7ff87ff8,0x7ff87ff8,0x1ffc1ffc,
	  0x1ffc1ffc,0x7fc07078,0x1ff01ff0,0x1ff01ff0,0x1ff00000,0x7ff01fe0,0x1fe01fe0,0x1fe001c0,0x70003bf8,0x1fdc1fdc,0x1fdc1fdc,
	  0x1fdc1fdc,0x3fbf0ff0,0xffc0ffc,0xffc0ffc,0x3ffe3ffe,0x3ffe3ffe,0xff03838,0xff00ff0,0xff00ff0,0xff00000,0x3ff01fb8,0x1fb81fb8,
	  0x1fb80380,0x3ff00380,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0,0x31c0,0x7e00078,0x7cf0000,0x1800180,
	  0x0,0x3800000,0x3803800,0x3c01ffc,0x3ffc0fe0,0xe01fc0,0x3e00e00,0x7e00f80,0x3800380,0x0,0x380,0x18007007,0x7fc003f0,0x7f007ff8,
	  0x700003f0,0x70381ffc,0x3f80701e,0x7ff8701c,0x707807c0,0x700007c0,0x701e1fc0,0x1c00fc0,0x3c01818,0x780f01c0,0x7ffc0380,0x3801c0,
	  0x0,0xf9c,0x39e003e0,0x79c03f0,0x380079c,0x38383ffe,0xe01c1e,0x7c739c,0x383807e0,0x39e0079c,0x7000fc0,0x1f80f38,0x3801c38,
	  0x781e0380,0x1ffc01c0,0x18001c0,0x0,0x1f80100,0xe000700,0x1c60718c,0x1000100,0x1e3c,0x1fc00100,0x7ff0100,0x7ffc0100,0x1000000,
	  0x0,0x0,0x0,0x0,0xfc00080,0x3e3c0100,0x1ffc01c0,0x1c0,0xf83ffe,0x1c0,0x1800838,0x0,0x0,0x0,0x0,0x7ffe,0x0,0x3b9e,0x1980000,
	  0x0,0x0,0x2300038,0x23003e0,0x70077007,0x70077007,0x70077007,0xe0fe03f0,0x7ff87ff8,0x7ff87ff8,0x1ffc1ffc,0x1ffc1ffc,0x7f007078,
	  0x7c007c0,0x7c007c0,0x7c00000,0xc7c00fc0,0xfc00fc0,0xfc001c0,0x700039f0,0xf9c0f9c,0xf9c0f9c,0xf9c0f9c,0x1f1e03e0,0x3f003f0,
	  0x3f003f0,0x3ffe3ffe,0x3ffe3ffe,0x7e03838,0x7e007e0,0x7e007e0,0x7e00000,0x63e00f38,0xf380f38,0xf380380,0x39e00380,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800000,0x0,0xc00300,0x0,0x3000000,0x3800,0x0,0x0,0x0,0x0,
	  0x0,0x300,0x0,0x0,0x1c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe0,0x0,0x0,0x0,0x0,0x380,0x3801c0,0x0,0x0,0x0,0x0,0x1c,0x0,0xe00000,
	  0x0,0x0,0x3800001c,0x0,0x0,0x0,0x700,0x1c0,0x18001c0,0x0,0x0,0xe000700,0x18600000,0x1000100,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x200000,0x0,0x1800ff0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x1980000,0x1800000,0x0,0x6300070,0x6300000,0x0,
	  0x0,0x0,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40000000,
	  0x0,0x700,0x38000700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800000,0x0,0xc00300,0x0,0x7000000,
	  0x7000,0x0,0x0,0x0,0x0,0x0,0x700,0x0,0x0,0xf040000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x78,0x0,0x0,0x0,0x0,0x3f0,0x1c0fc0,0x0,0x0,
	  0x0,0x0,0x1c,0x0,0xe00000,0x0,0x0,0x3800001c,0x0,0x0,0x0,0x700,0x1e0,0x18003c0,0x0,0x0,0xc000700,0x18c00000,0x1000000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200000,0x0,0x18007e0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x1980000,0xc00000,
	  0x0,0x7f800e0,0x7f80000,0x0,0x0,0x0,0x60,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x60,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x700,0x38000700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800000,
	  0x0,0x600600,0x0,0x6000000,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x7fc0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30,0x0,0x0,0x0,0x0,
	  0x3f0,0xfc0,0x0,0x0,0x0,0x0,0x838,0x0,0x1e00000,0x0,0x0,0x3800001c,0x0,0x0,0x0,0xf00,0xfc,0x1801f80,0x0,0x0,0x8008e00,0x30c00000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200000,0x0,0x1800000,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x1980000,0xc00000,
	  0x0,0x3001c0,0x300000,0x0,0x0,0x0,0x60,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x60,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0xf00,0x38000f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x800000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0xff0,0x0,0x1fc00000,0x0,0x0,0x3800001c,0x0,0x0,0x0,0x3e00,0x7c,0x1801f00,0x0,0x0,0x800fe00,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x200000,0x0,0x1800000,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x0,0x7c00000,0x0,0x3001fc,0x300000,
	  0x0,0x0,0x0,0x3e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x3e00,0x38003e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfff8,0x0,0x0,0x0,0x7e0,0x0,0x1f000000,
	  0x0,0x0,0x3800001c,0x0,0x0,0x0,0x3c00,0x0,0x1800000,0x0,0x0,0x7800,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x0,0x7800000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00,0x38003c00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfff8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1800000,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	// Definition of a 19x38 font
	const unsigned int font19x38[19*38*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c380000,0x0,0x1c380,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800007,0x3c003,0x86000000,
	  0x1e00000,0x3,0x80000700,0x3c00000,0x380000,0x70003c00,0x0,0xe1800e,0x1c00,0xf000e18,0x0,0x0,0x700000e0,0x780000,0x7000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe700000,0x0,0xe700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c0000e,0x7e003,0xe60071c0,0x7f80000,0x1,0xc0000e00,0x7e0038e,0x1c0000,
	  0xe0007e00,0x38e00000,0xf98007,0x3800,0x1f800f98,0x1c70000,0x0,0x380001c0,0xfc0071,0xc000e000,0x0,0x0,0x0,0x0,0x3e00000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x7e00000,0x0,0x7e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe0001c,0xe7006,0x7c0071c0,0xe180000,0x0,0xe0001c00,0xe70038e,0xe0001,0xc000e700,0x38e00000,
	  0x19f0003,0x80007000,0x39c019f0,0x1c70000,0x0,0x1c000380,0x1ce0071,0xc001c000,0x0,0x0,0x0,0x0,0x7f00000,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,
	  0x0,0x3c00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x700038,0x1c3806,0x3c0071c0,0xc0c0000,0x0,0x70003800,0x1c38038e,0x70003,0x8001c380,0x38e00000,0x18f0001,0xc000e000,
	  0x70e018f0,0x1c70000,0x0,0xe000700,0x3870071,0xc0038000,0x0,0x0,0x0,0x0,0xe380000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0xe000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x60000000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c38,0x0,0x1,0xc3800000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c00000,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0xc0c0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe000003,0x80018000,0x0,0xc180000,
	  0xe,0x380,0x1800000,0xe00000,0x38001800,0x0,0x38,0xe00,0x6000000,0x0,0x1,0xc0000070,0x300000,0x3800,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7000000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x78c00,0xc30,
	  0x0,0x0,0xc3000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800000,0x0,0x0,0x0,0xe0,0x1c000f,0xc0000000,0x0,0x0,
	  0x0,0xc0c0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7000007,0x3c003,0xc6000000,0xc180000,0x7,0x700,
	  0x3c00000,0x700000,0x70003c00,0x0,0xf1801c,0x1c00,0xf000f18,0x0,0x0,0xe00000e0,0x780000,0x7000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x1c007000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfe0000,0xfe000,0x0,0x3800000,0x700000,0x38,
	  0x7,0xe000001c,0x1c00,0x1c00700,0x7fc0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf800e,0x3e0000,0x0,0x0,0x0,0x1e00000,0x0,0x1,
	  0xf8000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7cc00,0x660,0x0,0x0,0x66000000,0x0,0x0,0x0,0x0,0x7,0x1c000000,0x0,0x0,0x0,0x3fe00000,
	  0x0,0x0,0x7000000,0x0,0x0,0x0,0x3e0,0x7c001f,0xe0000000,0x0,0x0,0x0,0xe1c0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x1f80,0x380000e,0x7e007,0xe60071c0,0xc180000,0x3,0x80000e00,0x7e0038e,0x380000,0xe0007e00,0x38e00f00,0x1f9800e,
	  0x3800,0x1f801f98,0x1c70000,0x0,0x700001c0,0xfc0071,0xc000e007,0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c7000,0x61c00600,0x1e00007e,0x70000,0x18003000,0x1800000,0x0,0x0,0x1c01f0,0x7e003f,0xc003f800,
	  0x1e03ffc,0x7f01ff,0xfc03f000,0x7e000000,0x0,0x0,0xfc0,0x1e,0x7fe000,0x7e03fe00,0x3fff07ff,0xe007e038,0x383ffe0,0xff81c01,
	  0xe1c000f8,0xf8f00e0,0xfc01ffc,0x3f00ff,0xc000fe07,0xfffc7007,0x1c007700,0x73c01ef,0x78ffff,0xfe0380,0xfe000,0x38000000,0x1800000,
	  0x700000,0x38,0x1f,0xe000001c,0x1c00,0x1c00700,0x7fc0000,0x0,0x0,0x0,0x0,0x1c000000,0x0,0x0,0x0,0x3f800e,0x3f8000,0x0,0xfc0000,
	  0x0,0x7f00000,0x0,0x1,0x98000000,0x7f00000,0x3ffe00,0xffff0,0x0,0x0,0x0,0x0,0x0,0xcf81f,0xee3807e0,0x0,0x0,0x7e03c01e,0x1c,
	  0x0,0x1f800000,0xf0078038,0xfc007,0x1c000000,0xfe00000,0x0,0x0,0x3fe000f0,0xf,0xc001f800,0x6000000,0xffc000,0x0,0x1c0007e0,
	  0x360,0x6c0010,0x70000700,0xf0001e,0x3c000,0x78000f00,0x7f800ff,0xf007e01f,0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83fc0,
	  0x7807007,0xe000fc00,0x1f8003f0,0x7e0000,0x1f867,0x70e00e,0x1c01c380,0x38f00787,0x3fe0,0x180000c,0x66006,0x7c0071c0,0xe380000,
	  0x1,0x80000c00,0x660038e,0x180000,0xc0006600,0x38e0078e,0x19f0006,0x3000,0x198019f0,0x1c70000,0x0,0x30000180,0xcc0071,0xc000c007,
	  0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c7000,0x61800600,0x7f8001ff,0x70000,
	  0x38003800,0x1800000,0x0,0x0,0x3807fc,0x1fe00ff,0xf00ffe00,0x3e03ffc,0xff81ff,0xfc07fc01,0xff800000,0x0,0x0,0x3fe0,0xfe001e,
	  0x7ff801,0xff83ff80,0x3fff07ff,0xe01ff838,0x383ffe0,0xff81c03,0xc1c000f8,0xf8f80e0,0x3ff01fff,0xffc0ff,0xf003ff87,0xfffc7007,
	  0x1e00f700,0x71c03c7,0x70ffff,0xfe01c0,0xfe000,0x7c000000,0xc00000,0x700000,0x38,0x3f,0xe000001c,0x1c00,0x1c00700,0x7fc0000,
	  0x0,0x0,0x0,0x0,0x1c000000,0x0,0x0,0x0,0x3f800e,0x3f8000,0x0,0x3fe0000,0x0,0xff00000,0x0,0x3,0xc000000,0x1ffc0000,0xfffe00,
	  0xffff0,0x0,0x0,0x0,0x0,0x0,0xc781f,0xee3803c0,0x0,0x0,0x3c01c01c,0x1c,0xc000,0x7fc00000,0x70070038,0x3fe007,0x1c000000,0x1ff80000,
	  0x0,0x0,0x3fe003fc,0x1f,0xe003fc00,0xc000000,0x3ffc000,0x0,0x7c000ff0,0x60,0xc0000,0x30000700,0xf0001e,0x3c000,0x78000f00,
	  0x3f000ff,0xf01ff81f,0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83ff8,0x7c0701f,0xf803ff00,0x7fe00ffc,0x1ff8000,0x7fe67,
	  0x70e00e,0x1c01c380,0x38700707,0x7ff0,0xc00018,0xc3006,0x3c0071c0,0x7f00000,0x0,0xc0001800,0xc30038e,0xc0001,0x8000c300,0x38e003fc,
	  0x18f0003,0x6000,0x30c018f0,0x1c70000,0x0,0x18000300,0x1860071,0xc0018007,0x38e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c7000,0xe1801fc0,0x618001ff,0x70000,0x30001800,0x21840000,0x0,0x0,0x380ffe,0x1fe00ff,
	  0xfc0fff00,0x3e03ffc,0x1ff81ff,0xfc0ffe03,0xffc00000,0x0,0x0,0x7ff0,0x3ff803f,0x7ffc03,0xffc3ffc0,0x3fff07ff,0xe03ffc38,0x383ffe0,
	  0xff81c07,0x81c000f8,0xf8f80e0,0x7ff81fff,0x81ffe0ff,0xf80fff87,0xfffc7007,0xe00e700,0x70e0387,0x80f0ffff,0xe001c0,0xe000,
	  0xfe000000,0xe00000,0x700000,0x38,0x3c,0x1c,0x1c00,0x1c00700,0x1c0000,0x0,0x0,0x0,0x0,0x1c000000,0x0,0x0,0x0,0x78000e,0x3c000,
	  0x0,0x7ff0000,0x0,0xf100000,0x0,0x7,0xe000000,0x7ffc0000,0x1fffe00,0xffff0,0x0,0x0,0x0,0x0,0x0,0x3,0xf780180,0x0,0x0,0x1801e03c,
	  0x1c,0xc000,0xffc00000,0x780f0038,0x786000,0x7f00,0x18380000,0x0,0xfe00,0x30c,0x10,0x70020e00,0x1c000000,0x7f8c000,0x0,0x6c001c38,
	  0x60,0xc0000,0x70000700,0x1f8003f,0x7e000,0xfc001f80,0x3f000ff,0xf03ffc1f,0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83ffc,
	  0x7c0703f,0xfc07ff80,0xfff01ffe,0x3ffc000,0xffec7,0x70e00e,0x1c01c380,0x38780f07,0xf070,0xe00038,0x1c3800,0x0,0x3e00000,0x0,
	  0xe0003800,0x1c380000,0xe0003,0x8001c380,0x3e0,0x3,0x8000e000,0x70e00000,0x0,0x0,0x1c000700,0x3870000,0x38007,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c7000,0xe3807ff0,0xc0c003c1,0x70000,0x70001c00,
	  0x718e0000,0x0,0x0,0x700f1e,0x1ce00c0,0x3c0c0f80,0x7e03800,0x3e08000,0x381e0f03,0xc1e00000,0x0,0x0,0x7078,0x783c03f,0x701e07,
	  0xc1c383e0,0x38000700,0x7c1c38,0x3801c00,0x381c0f,0x1c000fc,0x1f8f80e0,0x78781c07,0x81e1e0e0,0x780f0180,0xe007007,0xe00e380,
	  0xe0f0783,0x80e0000e,0xe000e0,0xe001,0xef000000,0x0,0x700000,0x38,0x38,0x1c,0x0,0x700,0x1c0000,0x0,0x0,0x0,0x0,0x1c000000,
	  0x0,0x0,0x0,0x70000e,0x1c000,0x0,0xf830000,0x0,0x1e000000,0x0,0x0,0x10000,0x780c0000,0x3e38000,0xe0,0x0,0x0,0x0,0x0,0x0,0x3,
	  0xd580000,0x0,0x0,0xe038,0x1c,0xc000,0xf0400000,0x380e0038,0x702000,0x1ffc0,0xc0000,0x0,0x3ff80,0x606,0x0,0x30000600,0x0,
	  0x7f8c000,0x0,0xc001818,0x60,0xc0003,0xe0000700,0x1f8003f,0x7e000,0xfc001f80,0x73801ee,0x7c1c1c,0x38000,0x70000e00,0xe0001,
	  0xc0003800,0x700383e,0x7c0703c,0x3c078780,0xf0f01e1e,0x3c3c000,0xf0f87,0x70e00e,0x1c01c380,0x38380e07,0xe038,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0xff0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x1c,0x1c7000,0xc380fff0,0xc0c00380,0x70000,0x70001c00,0x3dbc0070,0x0,0x0,0x701e0f,0xe0000,0x1e000380,
	  0x6e03800,0x7800000,0x781c0707,0x80e00000,0x0,0x0,0x4038,0xe00c03f,0x700e07,0x4380f0,0x38000700,0x700438,0x3801c00,0x381c0e,
	  0x1c000ec,0x1b8fc0e0,0xf03c1c03,0xc3c0f0e0,0x3c1e0000,0xe007007,0xe00e380,0xe070703,0xc1e0001e,0xe000e0,0xe001,0xc7000000,
	  0x0,0x700000,0x38,0x38,0x1c,0x0,0x700,0x1c0000,0x0,0x0,0x0,0x0,0x1c000000,0x0,0x0,0x0,0x70000e,0x1c000,0x0,0xe010000,0x0,
	  0x1c000000,0x10,0x20000,0x6c000,0xf0000000,0x3838000,0x1e0,0x0,0xf000f,0xf1e00,0x78f00000,0x0,0x3,0xdd80000,0x0,0x0,0xf078,
	  0x0,0xc001,0xe0000000,0x1c1c0038,0x700000,0x3c1e0,0xc0000,0x0,0x783c0,0x606,0x0,0x30000e00,0x0,0xff8c000,0x0,0xc00300c,0x60,
	  0xc0003,0xe0000000,0x1f8003f,0x7e000,0xfc001f80,0x73801ce,0x70041c,0x38000,0x70000e00,0xe0001,0xc0003800,0x700380f,0x7e07078,
	  0x1e0f03c1,0xe0783c0f,0x781e000,0x1c0787,0x70e00e,0x1c01c380,0x383c1e07,0xff00e038,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x878,
	  0x0,0x0,0x0,0x7,0x80000080,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,
	  0x1c7000,0xc301e630,0xc0c00380,0x70000,0xe0000e00,0xff00070,0x0,0x0,0xe01c07,0xe0000,0xe000380,0xce03800,0x7000000,0x701c0707,
	  0x600000,0x0,0x4000010,0x38,0x1c00e07f,0x80700e0e,0x38070,0x38000700,0xe00038,0x3801c00,0x381c1c,0x1c000ec,0x1b8ec0e0,0xe01c1c01,
	  0xc38070e0,0x1c1c0000,0xe007007,0x701c380,0xe078e01,0xc1c0003c,0xe00070,0xe003,0x83800000,0x7f,0x71f000,0x3e003e38,0x3f007ff,
	  0xe01f1c1c,0x7801fc00,0x3fc00701,0xe01c0077,0x8f071e00,0xf801c7c,0x7c700e,0x3e01fc03,0xfff8380e,0xe007700,0x73c0787,0x387ffc,
	  0x70000e,0x1c000,0x0,0xe000000,0x0,0x1c000000,0x10,0x20000,0xc2000,0xe0000000,0x3838000,0x3c0,0x0,0xf000f,0x78e00,0x70e00000,
	  0x0,0x3,0xc980fe0,0x1f0,0xf8000007,0xffc07070,0x0,0x3f801,0xc0000000,0x1e3c0038,0x700000,0x70070,0x7fc0000,0x0,0xe00e0,0x606,
	  0x1c0000,0x70007c00,0x380e,0xff8c000,0x0,0xc00300c,0x60,0xc0000,0x70000000,0x3fc007f,0x800ff001,0xfe003fc0,0x73801ce,0xe0001c,
	  0x38000,0x70000e00,0xe0001,0xc0003800,0x7003807,0x7607070,0xe0e01c1,0xc0383807,0x700e000,0x1c0387,0x70e00e,0x1c01c380,0x381c1c07,
	  0xffc0e0f8,0x3f8007f,0xfe001,0xfc003f80,0x7f007e3,0xe003e001,0xf8003f00,0x7e000fc,0xfe001f,0xc003f800,0x7f00003c,0x38f0007,
	  0xc000f800,0x1f0003e0,0x7c0007,0x8003f0c3,0x80e0701c,0xe0381c0,0x70700387,0x1f01c00e,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c701f,0xfff1c600,0xc0c00380,0x70000,0xe0000e00,0x3c00070,0x0,0x0,0xe03c07,
	  0x800e0000,0xe000380,0x1ce03800,0x7000000,0x701c0707,0x7003c0,0x780000,0x3c00001e,0x38,0x18006073,0x80700e0e,0x38070,0x38000700,
	  0xe00038,0x3801c00,0x381c38,0x1c000ee,0x3b8ee0e1,0xe01e1c01,0xc78078e0,0x1c1c0000,0xe007007,0x701c387,0xe03de00,0xe3800038,
	  0xe00070,0xe007,0x1c00000,0x1ff,0xc077f801,0xff807fb8,0xff807ff,0xe03fdc1d,0xfc01fc00,0x3fc00703,0xc01c007f,0xdf877f00,0x3fe01dfe,
	  0xff700e,0xff07ff03,0xfff8380e,0x700f700,0x71e0f03,0x80707ffc,0x70000e,0x1c000,0x0,0x1c000008,0x0,0x1c000000,0x10,0x20000,
	  0x82000,0xe0000000,0x7038000,0x80000380,0x2000040,0x7000e,0x38700,0xf1e00000,0x0,0x3,0xc183ff8,0x3fd,0xfc008007,0xffc038e0,
	  0x0,0xffc01,0xc0008008,0xe380038,0x380000,0xe3e38,0x1ffc0040,0x80000000,0x1cfc70,0x606,0x1c0000,0xe0007c00,0x380e,0xff8c000,
	  0x0,0xc00300c,0x8100060,0xc0000,0x30000700,0x39c0073,0x800e7001,0xce0039c0,0x73801ce,0xe0001c,0x38000,0x70000e00,0xe0001,
	  0xc0003800,0x7003807,0x77070f0,0xf1e01e3,0xc03c7807,0x8f00f080,0x83c0787,0x70e00e,0x1c01c380,0x380e3807,0xffe0e1c0,0xffe01ff,
	  0xc03ff807,0xff00ffe0,0x1ffc0ff7,0xf01ff807,0xfc00ff80,0x1ff003fe,0xfe001f,0xc003f800,0x7f0003fc,0x3bf801f,0xf003fe00,0x7fc00ff8,
	  0x1ff0007,0x8007fd83,0x80e0701c,0xe0381c0,0x70380707,0x7f80e01c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x1c,0x1c701f,0xfff1c600,0x618081c0,0x70000,0xe0000e00,0x3c00070,0x0,0x0,0xe03803,0x800e0000,0xe000380,0x18e03800,
	  0xf000000,0xf01c0707,0x7003c0,0x780000,0xfc00001f,0x80000078,0x301e6073,0x80700e1c,0x38038,0x38000700,0x1c00038,0x3801c00,
	  0x381c70,0x1c000e6,0x338ee0e1,0xc00e1c01,0xc70038e0,0x1c1c0000,0xe007007,0x701c387,0xe01dc00,0xf7800078,0xe00070,0xe00e,0xe00000,
	  0x3ff,0xe07ffc03,0xffc0fff8,0x1ffc07ff,0xe07ffc1d,0xfe01fc00,0x3fc00707,0x801c007f,0xdf877f80,0x7ff01fff,0x1fff00e,0xff07ff03,
	  0xfff8380e,0x700e380,0xe0e0e03,0x80707ffc,0x70000e,0x1c000,0x0,0x7ffc001c,0x0,0x1c000000,0x10,0x20000,0x82000,0xe0000000,
	  0x7038001,0xc0000780,0x70000e0,0x3800e,0x38700,0xe1c00000,0x0,0x3,0xc183ff8,0x7ff,0xfc01c007,0xffc03de0,0x0,0x1ffc01,0xc001c01c,
	  0xf780038,0x3c0000,0xcff18,0x380c00c1,0x80000000,0x18fe30,0x30c,0x1c0001,0xc0000e00,0x380e,0xff8c000,0x0,0xc00300c,0xc180060,
	  0xc0000,0x30000700,0x39c0073,0x800e7001,0xce0039c0,0xe1c038e,0x1c0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x7003803,0x877070e0,
	  0x71c00e3,0x801c7003,0x8e0071c0,0x1c380fc7,0x70e00e,0x1c01c380,0x380f7807,0x1e0e380,0x1fff03ff,0xe07ffc0f,0xff81fff0,0x3ffe0fff,
	  0xf03ffc0f,0xfe01ffc0,0x3ff807ff,0xfe001f,0xc003f800,0x7f0007fe,0x3bfc03f,0xf807ff00,0xffe01ffc,0x3ff8007,0x800fff83,0x80e0701c,
	  0xe0381c0,0x70380707,0xffc0e01c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1c701f,
	  0xfff1c600,0x7f8381e0,0x70000,0xc0000600,0xff00070,0x0,0x0,0x1c03803,0x800e0000,0xe000f00,0x38e03fe0,0xe000000,0xe00e0e07,
	  0x7003c0,0x780007,0xf0ffff87,0xf00000f0,0x307fe0f3,0xc0703c1c,0x38038,0x38000700,0x1c00038,0x3801c00,0x381ce0,0x1c000e6,0x338e70e1,
	  0xc00e1c01,0xc70038e0,0x3c1e0000,0xe007007,0x783c38f,0x8e01fc00,0x770000f0,0xe00038,0xe01c,0x700000,0x381,0xe07c1e07,0xc0c1e0f8,
	  0x3c1e0038,0xf07c1f,0xe001c00,0x1c0070f,0x1c0079,0xf3c7c380,0xf0781f07,0x83c1f00f,0xc10f0300,0x1c00380e,0x700e380,0xe0f1e03,
	  0xc0f00078,0x70000e,0x1c000,0x0,0xfff8003e,0x0,0x3c000000,0x10,0x20000,0xc6000,0xf0000000,0x7038003,0xe0000f00,0xf8001f0,
	  0x3801c,0x18300,0xe1800000,0x0,0x3,0xc187818,0x70f,0x9e03e000,0x7801dc0,0x1c,0x3cc401,0xc000efb8,0x7f7f0038,0x3f0000,0x1ce11c,
	  0x300c01c3,0x80000000,0x38c638,0x3fc,0x1c0003,0x80000600,0x380e,0xff8c000,0x0,0xc00300c,0xe1c0060,0xc0010,0x70000700,0x79e00f3,
	  0xc01e7803,0xcf0079e0,0xe1c038e,0x1c0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x7003803,0x873870e0,0x71c00e3,0x801c7003,
	  0x8e0070e0,0x38381dc7,0x70e00e,0x1c01c380,0x38077007,0xf0e700,0x1c0f0381,0xe0703c0e,0x781c0f0,0x381e083e,0x787c0c1e,0xf03c1e0,
	  0x783c0f07,0x800e0001,0xc0003800,0x7000fff,0x3e1c078,0x3c0f0781,0xe0f03c1e,0x783c000,0x1e0f03,0x80e0701c,0xe0381c0,0x70380f07,
	  0xc1e0e03c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x1,0x8701c600,0x1e0f01e0,0x1,
	  0xc0000700,0x3dbc0070,0x0,0x0,0x1c03803,0x800e0000,0x1e01fe00,0x70e03ff8,0xe3e0001,0xe007fc07,0x80f003c0,0x78001f,0xc0ffff81,
	  0xfc0001e0,0x30e1e0e1,0xc07ff81c,0x38038,0x3ffe07ff,0xc1c0003f,0xff801c00,0x381de0,0x1c000e7,0x738e70e1,0xc00e1c03,0xc70038e0,
	  0x780f8000,0xe007007,0x383838d,0x8e00f800,0x7f0000e0,0xe00038,0xe000,0x0,0x200,0xf0780e07,0x8041c078,0x380e0038,0xe03c1e,
	  0xf001c00,0x1c0071e,0x1c0070,0xe1c783c0,0xe0381e03,0x8380f00f,0xe0000,0x1c00380e,0x381c380,0xe07bc01,0xc0e00078,0x70000e,
	  0x1c000,0x0,0x1c000061,0x0,0x38000000,0x10,0x20000,0x7c000,0x7c000000,0x703fc06,0x10000e00,0x18400308,0x1801c,0x1c381,0xc3800000,
	  0x0,0x0,0x7000,0xe0f,0xe061000,0x7801fc0,0x1c,0x38c001,0xc0007ff0,0x7fff0038,0x77c000,0x19c00c,0x301c0387,0x0,0x30c618,0xf0,
	  0x1c0007,0x600,0x380e,0x7f8c007,0x80000000,0xc001818,0x70e03fc,0x387f871f,0xe0e00700,0x70e00e1,0xc01c3803,0x870070e0,0xe1c038f,
	  0xe1c0001f,0xff03ffe0,0x7ffc0fff,0x800e0001,0xc0003800,0x7003803,0x873870e0,0x71c00e3,0x801c7003,0x8e007070,0x703839c7,0x70e00e,
	  0x1c01c380,0x3807f007,0x70e700,0x10078200,0xf0401e08,0x3c10078,0x200f001c,0x3878041c,0x70380e0,0x701c0e03,0x800e0001,0xc0003800,
	  0x7001e0f,0x3c1e070,0x1c0e0381,0xc070380e,0x701c000,0x1c0f03,0x80e0701c,0xe0381c0,0x701c0e07,0x80e07038,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x3,0x8600e600,0x7803f0,0x1,0xc0000700,0x718e0070,0x0,0x0,0x38038c3,
	  0x800e0000,0x3c01f800,0x60e03ffc,0xeff8001,0xc001f003,0xc1f003c0,0x7800fe,0xffff80,0x3f8003c0,0x60c0e0e1,0xc07fe01c,0x38038,
	  0x3ffe07ff,0xc1c07e3f,0xff801c00,0x381fe0,0x1c000e3,0x638e30e1,0xc00e1c07,0x870038ff,0xf00ff800,0xe007007,0x38381cd,0x9c007000,
	  0x3e0001e0,0xe0001c,0xe000,0x0,0x0,0x70780f0f,0x3c078,0x70070038,0x1e03c1c,0x7001c00,0x1c0073c,0x1c0070,0xe1c701c1,0xe03c1e03,
	  0xc780f00f,0xe0000,0x1c00380e,0x381c387,0xe03f801,0xc0e000f0,0x70000e,0x1c007,0xe0100000,0x1c0000cd,0x80000003,0xffc00000,
	  0x3ff,0x807ff000,0xe0,0x7fc00060,0x703fc0c,0xd8001e00,0x3360066c,0x1c018,0xc181,0x83000000,0x0,0x0,0x7000,0x300e07,0xe0cd800,
	  0xf000f80,0x1c,0x78c00f,0xff0038e0,0x3e00038,0xe1e000,0x19800c,0x383c070e,0x7fffc00,0x30fc18,0x0,0xffff80e,0x20e00,0x380e,
	  0x7f8c007,0x80000000,0xc001c38,0x38703ff,0xf87fff0f,0xcfe00f00,0x70e00e1,0xc01c3803,0x870070e0,0x1e1e078f,0xe1c0001f,0xff03ffe0,
	  0x7ffc0fff,0x800e0001,0xc0003800,0x700ff83,0x871870e0,0x71c00e3,0x801c7003,0x8e007038,0xe03871c7,0x70e00e,0x1c01c380,0x3803e007,
	  0x70e700,0x38000,0x70000e00,0x1c00038,0x7001c,0x38f00038,0x3870070,0xe00e1c01,0xc00e0001,0xc0003800,0x7001c07,0x380e0f0,0x1e1e03c3,
	  0xc078780f,0xf01e000,0x3c0f03,0x80e0701c,0xe0381c0,0x701c0e07,0x80f07038,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x3,0x8600ff00,0x1e00778,0x38000001,0xc0000700,0x21843fff,0xe0000000,0x0,0x38039e3,0x800e0000,
	  0x7c01fe00,0xe0e0203e,0xeffc001,0xc00ffe03,0xff700000,0x7f0,0x0,0x7f00380,0x618060e1,0xc07ffc1c,0x38038,0x3ffe07ff,0xc1c07e3f,
	  0xff801c00,0x381ff0,0x1c000e3,0x638e38e1,0xc00e1fff,0x870038ff,0xc003fe00,0xe007007,0x38381cd,0x9c00f800,0x3e0003c0,0xe0001c,
	  0xe000,0x0,0x0,0x7070070e,0x38038,0x70070038,0x1c01c1c,0x7001c00,0x1c00778,0x1c0070,0xe1c701c1,0xc01c1c01,0xc700700e,0xfc000,
	  0x1c00380e,0x381c3c7,0x1e01f001,0xe1e001e0,0xf0000e,0x1e01f,0xf8300000,0x1c00019c,0xc0000003,0xffc00000,0x10,0x20000,0x700,
	  0x1ff000c0,0x703fc19,0xcc003c00,0x67300ce6,0xc038,0xc181,0x83000000,0x0,0x0,0x7e00,0x180e07,0xe19cc00,0x1e000f80,0x1c,0x70c00f,
	  0xff007070,0x3e00038,0xe0f000,0x19800c,0x1fec0e1c,0x7fffc00,0x30f818,0x0,0xffff81f,0xf003fc00,0x380e,0x3f8c007,0x80000000,
	  0x7f800ff0,0x1c3803f,0xe007fc00,0xff800e00,0x70e00e1,0xc01c3803,0x870070e0,0x1c0e070f,0xe1c0001f,0xff03ffe0,0x7ffc0fff,0x800e0001,
	  0xc0003800,0x700ff83,0x871c70e0,0x71c00e3,0x801c7003,0x8e00701d,0xc038e1c7,0x70e00e,0x1c01c380,0x3803e007,0x70e3c0,0x38000,
	  0x70000e00,0x1c00038,0x7001c,0x38e00038,0x3870070,0xe00e1c01,0xc00e0001,0xc0003800,0x7003c07,0x8380e0e0,0xe1c01c3,0x80387007,
	  0xe00e1ff,0xfe381b83,0x80e0701c,0xe0381c0,0x701e1e07,0x707878,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x1c,0x3,0xe007fe0,0x7800e3c,0x38000001,0xc0000700,0x1803fff,0xe0000000,0x0,0x70039c3,0x800e0000,0xf8000f80,
	  0xc0e0000e,0xf83c003,0xc01e0f01,0xff700000,0x7c0,0x0,0x1f00780,0x618061c0,0xe0701e1c,0x38038,0x38000700,0x1c07e38,0x3801c00,
	  0x381e78,0x1c000e3,0xe38e18e1,0xc00e1fff,0x70038ff,0xe0007f80,0xe007007,0x1c701dd,0x9c00f800,0x1c000780,0xe0000e,0xe000,0x0,
	  0x7f,0xf070070e,0x38038,0x7fff0038,0x1c01c1c,0x7001c00,0x1c007f8,0x1c0070,0xe1c701c1,0xc01c1c01,0xc700700e,0x7fc00,0x1c00380e,
	  0x1c381c7,0x1c01f000,0xe1c001c0,0xfe0000e,0xfe1f,0xfff00000,0x7ff003fc,0xe0000003,0xffc00000,0x10,0x20000,0x3800,0x3fc0180,
	  0x703803f,0xce007800,0xff381fe7,0x30,0x0,0xc0,0x0,0x0,0x3fe0,0xc0e07,0xfe3fce00,0x1c000700,0x1c,0x70c00f,0xff006030,0x1c00000,
	  0xe07800,0x19800c,0xfcc1c38,0x7fffc00,0x30d818,0x0,0xffff81f,0xf001f800,0x380e,0xf8c007,0x80000000,0x7f8007e0,0xe1c3fe,0x7fc00f,
	  0xf8001e00,0xe0701c0,0xe0381c07,0x380e070,0x1c0e070e,0x1c0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x700ff83,0x870c70e0,
	  0x71c00e3,0x801c7003,0x8e00700f,0x8038c1c7,0x70e00e,0x1c01c380,0x3801c007,0xf0e3e0,0x3ff807f,0xf00ffe01,0xffc03ff8,0x7ff03ff,
	  0xf8e0003f,0xff87fff0,0xfffe1fff,0xc00e0001,0xc0003800,0x7003803,0x8380e0e0,0xe1c01c3,0x80387007,0xe00e1ff,0xfe383383,0x80e0701c,
	  0xe0381c0,0x700e1c07,0x703870,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x3,0xc000ff0,
	  0x3c1e1c1c,0x38000001,0xc0000700,0x1803fff,0xe0000007,0xf8000000,0x7003803,0x800e0001,0xf0000381,0xc0e00007,0xf01e003,0x801c0700,
	  0x7c700000,0x7c0,0x0,0x1f00700,0x618061c0,0xe0700e1c,0x38038,0x38000700,0x1c00e38,0x3801c00,0x381e38,0x1c000e1,0xc38e1ce1,
	  0xc00e1ffc,0x70038e0,0xf0000780,0xe007007,0x1c701dd,0xdc01fc00,0x1c000780,0xe0000e,0xe000,0x0,0x1ff,0xf070070e,0x38038,0x7fff0038,
	  0x1c01c1c,0x7001c00,0x1c007f8,0x1c0070,0xe1c701c1,0xc01c1c01,0xc700700e,0x3ff00,0x1c00380e,0x1c381cd,0x9c00e000,0xe1c003c0,
	  0xf80000e,0x3e18,0x3ff00000,0xffe007fd,0xf0000000,0x38000000,0x10,0x20000,0x1c000,0x3c0300,0x703807f,0xdf007801,0xff7c3fef,
	  0x80000000,0x0,0x3e0,0x7ffe7ff,0xff000000,0x1ff8,0x60e07,0xfe7fdf00,0x3c000700,0x1c,0x70c001,0xc0006030,0x7fff0000,0xf03800,
	  0x19800c,0x1c38,0x1c07,0xf830cc18,0x0,0x1c0000,0x0,0x380e,0x18c007,0x80000000,0x0,0xe1cfe0,0x1fc003f,0x80003c00,0xe0701c0,
	  0xe0381c07,0x380e070,0x1c0e070e,0x1c0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x7003803,0x870e70e0,0x71c00e3,0x801c7003,
	  0x8e007007,0x3981c7,0x70e00e,0x1c01c380,0x3801c007,0x1e0e0f8,0xfff81ff,0xf03ffe07,0xffc0fff8,0x1fff07ff,0xf8e0003f,0xff87fff0,
	  0xfffe1fff,0xc00e0001,0xc0003800,0x7003803,0x8380e0e0,0xe1c01c3,0x80387007,0xe00e1ff,0xfe386383,0x80e0701c,0xe0381c0,0x700e1c07,
	  0x703870,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x7f,0xffc00678,0x707f9c1e,0x38000001,
	  0xc0000700,0x70,0x7,0xf8000000,0xe003803,0x800e0003,0xe00001c3,0x80e00007,0xe00e007,0x80380380,0x700000,0x7f0,0x0,0x7f00700,
	  0x618061ff,0xe070071c,0x38038,0x38000700,0x1c00e38,0x3801c00,0x381c3c,0x1c000e1,0xc38e1ce1,0xc00e1c00,0x70038e0,0x700003c0,
	  0xe007007,0x1c701d8,0xdc03dc00,0x1c000f00,0xe00007,0xe000,0x0,0x3ff,0xf070070e,0x38038,0x7fff0038,0x1c01c1c,0x7001c00,0x1c007fc,
	  0x1c0070,0xe1c701c1,0xc01c1c01,0xc700700e,0x3f00,0x1c00380e,0x1c381cd,0x9c01f000,0x73800780,0xfe0000e,0xfe10,0x7c00000,0x1c000ffb,
	  0xf8000000,0x38000000,0x10,0x20000,0x20000,0x1e0700,0x70380ff,0xbf80f003,0xfefe7fdf,0xc0000000,0x0,0x3f0,0x7ffe7ff,0xff000000,
	  0x1f8,0x30e07,0xfeffbf80,0x78000700,0x1c,0x70c001,0xc0006030,0x7fff0000,0x783800,0x1ce11c,0xe1c,0x1c07,0xf838ce38,0x0,0x1c0000,
	  0x0,0x380e,0x18c000,0x0,0x0,0x1c38c00,0x1800030,0x7800,0xfff01ff,0xe03ffc07,0xff80fff0,0x3fff0ffe,0x1c0001c,0x38000,0x70000e00,
	  0xe0001,0xc0003800,0x7003803,0x870e70e0,0x71c00e3,0x801c7003,0x8e00700f,0x803b81c7,0x70e00e,0x1c01c380,0x3801c007,0xffe0e03c,
	  0x1fff83ff,0xf07ffe0f,0xffc1fff8,0x3fff0fff,0xf8e0003f,0xff87fff0,0xfffe1fff,0xc00e0001,0xc0003800,0x7003803,0x8380e0e0,0xe1c01c3,
	  0x80387007,0xe00e000,0x38c383,0x80e0701c,0xe0381c0,0x70073807,0x701ce0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0xffc0063c,0x40619c0f,0x30000001,0xc0000700,0x70,0x7,0xf8000000,0xe003803,0x800e0007,0xc00001c3,
	  0xfffc0007,0xe00e007,0x380380,0xf00000,0xfe,0xffff80,0x3f800700,0x618063ff,0xf070071c,0x38038,0x38000700,0x1c00e38,0x3801c00,
	  0x381c1e,0x1c000e0,0x38e0ee1,0xc00e1c00,0x70038e0,0x380001c0,0xe007007,0x1ef01d8,0xdc038e00,0x1c001e00,0xe00007,0xe000,0x0,
	  0x7c0,0x7070070e,0x38038,0x70000038,0x1c01c1c,0x7001c00,0x1c0079e,0x1c0070,0xe1c701c1,0xc01c1c01,0xc700700e,0x780,0x1c00380e,
	  0xe701cd,0x9c01f000,0x73800f00,0xe0000e,0xe000,0x0,0x1c0007f7,0xf0000000,0x70000000,0x10,0x20000,0x0,0xe0e00,0x703807f,0x7f01e001,
	  0xfdfc3fbf,0x80000000,0x0,0x7f0,0x0,0x0,0x3c,0x18e07,0x7f7f00,0xf0000700,0x1c,0x70c001,0xc0007070,0x1c00000,0x3e7000,0xcff18,
	  0x3ffc070e,0x1c07,0xf818c630,0x0,0x1c0000,0x0,0x380e,0x18c000,0x0,0x3ffc,0x3870000,0xe000fc00,0x380f000,0x1fff83ff,0xf07ffe0f,
	  0xffc1fff8,0x3fff0ffe,0x1c0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x7003803,0x870770e0,0x71c00e3,0x801c7003,0x8e00701d,
	  0xc03f01c7,0x70e00e,0x1c01c380,0x3801c007,0xffc0e01c,0x3e0387c0,0x70f80e1f,0x1c3e038,0x7c071e1c,0xe00038,0x70000,0xe0001c00,
	  0xe0001,0xc0003800,0x7003803,0x8380e0e0,0xe1c01c3,0x80387007,0xe00e000,0x398383,0x80e0701c,0xe0381c0,0x70073807,0x701ce0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0xffc0061c,0xc0dc07,0xf0000001,0xc0000700,
	  0x70,0x0,0x0,0x1c003c07,0x800e000f,0x1c3,0xfffc0007,0xe00e007,0x380380,0xe00000,0x1f,0xc0ffff81,0xfc000700,0x618063ff,0xf070070e,
	  0x38070,0x38000700,0xe00e38,0x3801c00,0x381c0e,0x1c000e0,0x38e0ee1,0xe01e1c00,0x78078e0,0x380001c0,0xe007007,0xee01f8,0xfc078f00,
	  0x1c001c00,0xe00003,0x8000e000,0x0,0x700,0x7070070e,0x38038,0x70000038,0x1c01c1c,0x7001c00,0x1c0070e,0x1c0070,0xe1c701c1,
	  0xc01c1c01,0xc700700e,0x380,0x1c00380e,0xe700ed,0xb803f800,0x77800f00,0x70000e,0x1c000,0x0,0xe0003f7,0xe0000000,0x70000000,
	  0x10,0x20000,0x1c0e0,0xe1c00,0x703803f,0x7e01c000,0xfdf81fbf,0x0,0x0,0x3f0,0x0,0x0,0x1c,0x1ce07,0x3f7e00,0xf0000700,0x1c,
	  0x70c001,0xc00038e0,0x1c00038,0xf7000,0xe3e38,0x3ffc0387,0x1c00,0x1cc770,0x0,0x1c0000,0x0,0x380e,0x18c000,0x0,0x3ffc,0x70e0001,
	  0xe001fe00,0x780e000,0x1fff83ff,0xf07ffe0f,0xffc1fff8,0x3fff0ffe,0xe0001c,0x38000,0x70000e00,0xe0001,0xc0003800,0x7003807,
	  0x70770f0,0xf1e01e3,0xc03c7807,0x8f00f038,0xe03e03c7,0x70e00e,0x1c01c380,0x3801c007,0xff00e00e,0x38038700,0x70e00e1c,0x1c38038,
	  0x70071c1c,0xe00038,0x70000,0xe0001c00,0xe0001,0xc0003800,0x7003803,0x8380e0e0,0xe1c01c3,0x80387007,0xe00e000,0x3b0383,0x80e0701c,
	  0xe0381c0,0x70077807,0x701de0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x1c00061c,
	  0xc0de03,0xe0000001,0xc0000700,0x70,0x0,0x0,0x1c001c07,0xe001e,0x1c3,0xfffc0007,0x600e00e,0x380380,0xe00000,0x7,0xf0ffff87,
	  0xf0000000,0x60c0e380,0x7070070e,0x38070,0x38000700,0xe00e38,0x3801c00,0x381c0f,0x1c000e0,0x38e06e0,0xe01c1c00,0x38070e0,
	  0x1c0001c0,0xe007007,0xee00f8,0xf80f0700,0x1c003c00,0xe00003,0x8000e000,0x0,0x700,0x70780f0f,0x3c078,0x70000038,0x1e03c1c,
	  0x7001c00,0x1c0070f,0x1c0070,0xe1c701c1,0xe03c1e03,0xc780f00e,0x380,0x1c00380e,0xe700f8,0xf807bc00,0x3f001e00,0x70000e,0x1c000,
	  0x0,0xe0001ff,0xc0000000,0x70000000,0x10,0x20000,0x33110,0xe0e00,0x383801f,0xfc03c000,0x7ff00ffe,0x0,0x0,0x3e0,0x0,0x0,0x1c,
	  0x38e07,0x1ffc01,0xe0000700,0x1c,0x78c001,0xc0007ff0,0x1c00038,0x7c000,0x70070,0x1c3,0x80001c00,0xe00e0,0x0,0x1c0000,0x0,
	  0x380e,0x18c000,0x0,0x0,0xe1c0001,0xe0010700,0x780e000,0x1c038380,0x70700e0e,0x1c1c038,0x78070e0e,0xe0001c,0x38000,0x70000e00,
	  0xe0001,0xc0003800,0x7003807,0x7037070,0xe0e01c1,0xc0383807,0x700e070,0x701c0387,0x70e00e,0x1c01c380,0x3801c007,0xe00e,0x38038700,
	  0x70e00e1c,0x1c38038,0x70071c1c,0xf00038,0x70000,0xe0001c00,0xe0001,0xc0003800,0x7003c07,0x8380e0f0,0x1e1e03c3,0xc078780f,
	  0xf01e007,0x803e0783,0x80e0701c,0xe0381c0,0x7003f007,0x80f00fc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x6,0x1800061c,0xc0de01,0xc0000000,0xc0000e00,0x70,0xf0000,0x3c00,0x38001c0f,0xe003c,0x3c0,0xe0000e,0x701e00e,
	  0x3c0780,0x1e003c0,0x780000,0xfc00001f,0x80000000,0x60e1e780,0x78700f07,0x4380f0,0x38000700,0xf00e38,0x3801c00,0xc0781c07,
	  0x81c000e0,0x38e07e0,0xe03c1c00,0x380f0e0,0x1e0003c0,0xe00780f,0xee00f0,0x780e0780,0x1c007800,0xe00001,0xc000e000,0x0,0x700,
	  0xf0780e07,0x8041c078,0x38020038,0xe03c1c,0x7001c00,0x1c00707,0x801c0070,0xe1c701c0,0xe0381e03,0x8380f00e,0x80380,0x1c003c1e,
	  0x7e00f8,0xf80f1e00,0x3f003c00,0x70000e,0x1c000,0x0,0xf0100f7,0x80078000,0x700078f0,0x10,0x7ff000,0x61208,0x1e0700,0x383800f,
	  0x78078000,0x3de007bc,0x0,0x0,0x0,0x0,0x0,0x401c,0x70e0f,0xf7803,0xc0000700,0x1c,0x38c001,0xc000efb8,0x1c00038,0x1e000,0x3c1e0,
	  0xc1,0x80000000,0x783c0,0x0,0x0,0x0,0x3c1e,0x18c000,0x0,0x0,0xc180003,0x60000300,0xd80e010,0x3c03c780,0x78f00f1e,0x1e3c03c,
	  0x70039c0e,0x70041c,0x38000,0x70000e00,0xe0001,0xc0003800,0x700380f,0x703f070,0x1e0e03c1,0xc078380f,0x701e0e0,0x381c0787,
	  0x80f0f01e,0x1e03c3c0,0x7801c007,0xe00e,0x38078700,0xf0e01e1c,0x3c38078,0x700f1c1c,0x78041c,0x1038020,0x70040e00,0x800e0001,
	  0xc0003800,0x7001c07,0x380e070,0x1c0e0381,0xc070380e,0x701c007,0x801e0703,0xc1e0783c,0xf0781e0,0xf003f007,0x80e00fc0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0xe,0x1801867c,0xc0cf83,0xe0000000,0xe0000e00,
	  0x70,0xf0000,0x3c00,0x38000f1e,0xe0070,0x180780,0xe0603e,0x783c01e,0x1e0f01,0x7c003c0,0x780000,0x3c00001e,0x700,0x307fe700,
	  0x38701e07,0xc1c383e0,0x38000700,0x7c1e38,0x3801c00,0xe0f01c03,0x81c000e0,0x38e03e0,0x78781c00,0x1e1e0e0,0xe180780,0xe003c1e,
	  0x7c00f0,0x781e03c0,0x1c007000,0xe00001,0xc000e000,0x0,0x783,0xf07c1e07,0xc0c1e0f8,0x3e0e0038,0xf07c1c,0x7001c00,0x1c00703,
	  0xc01e0070,0xe1c701c0,0xf0781f07,0x83c1f00e,0xe0f80,0x1e003c3e,0x7e00f8,0xf80e0e00,0x3f003800,0x70000e,0x1c000,0x0,0x7830077,
	  0xf0000,0x700078f0,0x10,0x20000,0x41208,0xc03c0380,0x3c38007,0x70070000,0x1dc003b8,0x0,0x0,0x0,0x0,0x0,0x707c,0x6070f,0x86077003,
	  0x80000700,0x1c,0x3ec401,0xc001c01c,0x1c00038,0xf000,0x1ffc0,0x40,0x80000000,0x3ff80,0x0,0x0,0x0,0x3e3e,0x18c000,0x0,0x0,
	  0x8100006,0x60000300,0x1980f070,0x3801c700,0x38e0071c,0xe3801c,0x70039c0e,0x7c1c1c,0x38000,0x70000e00,0xe0001,0xc0003800,
	  0x700383e,0x701f03c,0x3c078780,0xf0f01e1e,0x3c3c1c0,0x1c3f0f03,0xc1e0783c,0xf0781e0,0xf001c007,0xe81e,0x3c1f8783,0xf0f07e1e,
	  0xfc3c1f8,0x783f1e3e,0x187c0c1f,0x703e0e0,0x7c1c0f83,0x800e0001,0xc0003800,0x7001e0f,0x380e078,0x3c0f0781,0xe0f03c1e,0x783c007,
	  0x801e0f03,0xc3e0787c,0xf0f81e1,0xf003f007,0xc1e00fc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x1c,0xe,0x3801fff8,0x6187ff,0xe0000000,0xe0000e00,0x70,0xf0000,0x3c00,0x38000ffe,0x1fff0ff,0xfe1fff80,0xe07ffc,0x3ffc01c,
	  0x1fff01,0xff8003c0,0x780000,0x4000010,0x700,0x301e6700,0x387ffe03,0xffc3ffc0,0x3fff0700,0x3ffe38,0x383ffe0,0xfff01c03,0xc1fff8e0,
	  0x38e03e0,0x7ff81c00,0x1ffe0e0,0xf1fff80,0xe003ffe,0x7c00f0,0x781c01c0,0x1c00ffff,0xe00001,0xc000e000,0x0,0x3ff,0x707ffc03,
	  0xffc0fff8,0x1ffe0038,0x7ffc1c,0x707fff0,0x1c00701,0xc00ff070,0xe1c701c0,0x7ff01fff,0x1fff00e,0xfff00,0xff81fee,0x7e00f0,
	  0x781e0f00,0x1e007ffc,0x70000e,0x1c000,0x0,0x3ff003e,0xf0000,0xe00070e0,0x60830010,0x20000,0x41208,0xfffc01c0,0x1fffe03,0xe00ffff0,
	  0xf8001f0,0x0,0x0,0x0,0x0,0x0,0x7ff8,0xc07fd,0xfe03e007,0xffc00700,0x1c,0x1ffc1f,0xffc08008,0x1c00038,0x7000,0x7f00,0x0,0x0,
	  0xfe00,0x0,0xffff800,0x0,0x3ff7,0x8018c000,0x0,0x0,0x6,0x60000700,0x19807ff0,0x3801c700,0x38e0071c,0xe3801c,0x70039c0f,0xf03ffc1f,
	  0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83ffc,0x701f03f,0xfc07ff80,0xfff01ffe,0x3ffc080,0x83fff03,0xffe07ffc,0xfff81ff,
	  0xf001c007,0xeffc,0x1ffb83ff,0x707fee0f,0xfdc1ffb8,0x3ff70ff7,0xf83ffc0f,0xff01ffe0,0x3ffc07ff,0x83fff87f,0xff0fffe1,0xfffc0ffe,
	  0x380e03f,0xf807ff00,0xffe01ffc,0x3ff8007,0x803ffe01,0xfee03fdc,0x7fb80ff,0x7001e007,0xffc00780,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0xc,0x3801fff0,0x7f83fe,0x70000000,0xe0000e00,0x0,0xf0000,0x3c00,0x700007fc,
	  0x1fff0ff,0xfe1ffe00,0xe07ff8,0x1ff801c,0xffe01,0xff0003c0,0x780000,0x0,0x700,0x38000f00,0x3c7ffc01,0xff83ff80,0x3fff0700,
	  0x1ffc38,0x383ffe0,0x7fe01c01,0xe1fff8e0,0x38e03e0,0x3ff01c00,0xffc0e0,0x71fff00,0xe001ffc,0x7c00f0,0x783c01e0,0x1c00ffff,
	  0xe00000,0xe000e000,0x0,0x1ff,0x7077f801,0xff807fb8,0xffc0038,0x3fdc1c,0x707fff0,0x1c00701,0xe007f070,0xe1c701c0,0x3fe01dfe,
	  0xff700e,0x7fe00,0xff80fee,0x3c0070,0x703c0780,0x1e007ffc,0x70000e,0x1c000,0x0,0x1fe001c,0xe0000,0xe000e1c0,0x71c78010,0x20000,
	  0x21318,0xfff800c0,0xfffe01,0xc00ffff0,0x70000e0,0x0,0x0,0x0,0x0,0x0,0x3ff0,0x1803fd,0xfe01c007,0xffc00700,0x1c,0xffc1f,0xffc00000,
	  0x1c00038,0x7000,0x0,0x0,0x0,0x0,0x0,0xffff800,0x0,0x3ff7,0x8018c000,0x0,0x0,0xc,0x60000e00,0x31803fe0,0x7801ef00,0x3de007bc,
	  0xf7801e,0xf003fc0f,0xf01ff81f,0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83ff8,0x701f01f,0xf803ff00,0x7fe00ffc,0x1ff8000,
	  0x67fe01,0xffc03ff8,0x7ff00ff,0xe001c007,0xeff8,0xffb81ff,0x703fee07,0xfdc0ffb8,0x1ff70ff7,0xf81ff807,0xfe00ffc0,0x1ff803ff,
	  0x3fff87f,0xff0fffe1,0xfffc07fc,0x380e01f,0xf003fe00,0x7fc00ff8,0x1ff0000,0x37fc00,0xfee01fdc,0x3fb807f,0x7001e007,0x7f800780,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0xc,0x30007fc0,0x1e00f8,0x78000000,0x70001c00,
	  0x0,0xe0000,0x3c00,0x700001f0,0x1fff0ff,0xfe07f800,0xe01fe0,0x7e0038,0x3f800,0xfc0003c0,0x700000,0x0,0x700,0x18000e00,0x1c7ff000,
	  0x7e03fe00,0x3fff0700,0x7f038,0x383ffe0,0x1f801c00,0xf1fff8e0,0x38e01e0,0xfc01c00,0x3f80e0,0x787fc00,0xe0007f0,0x7c00f0,0x387800f0,
	  0x1c00ffff,0xe00000,0xe000e000,0x0,0xfc,0x7071f000,0x3f003e38,0x3f00038,0x1f1c1c,0x707fff0,0x1c00700,0xf003f070,0xe1c701c0,
	  0x1f801c7c,0x7c700e,0x1f800,0x3f8078e,0x3c0070,0x707803c0,0x1c007ffc,0x70000e,0x1c000,0x0,0x7c0008,0x1e0000,0xe000e1c0,0x71c30010,
	  0x20000,0x1e1f0,0x3fe00020,0x3ffe00,0x800ffff0,0x2000040,0x0,0x0,0x0,0x0,0x0,0xfc0,0x3001f0,0x78008007,0xffc00700,0x1c,0x3f81f,
	  0xffc00000,0x1c00038,0x407000,0x0,0x0,0x0,0x0,0x0,0xffff800,0x0,0x39c7,0x18c000,0x0,0x0,0x18,0x60001c00,0x61801f80,0x7000ee00,
	  0x1dc003b8,0x77000e,0xe001f80f,0xf007e01f,0xff83fff0,0x7ffe0fff,0xc1fff03f,0xfe07ffc0,0xfff83fc0,0x700f007,0xe000fc00,0x1f8003f0,
	  0x7e0000,0xe1f800,0x7f000fe0,0x1fc003f,0x8001c007,0xe7f0,0x7e380fc,0x701f8e03,0xf1c07e38,0xfc703c1,0xe003f001,0xf8003f00,
	  0x7e000fc,0x3fff87f,0xff0fffe1,0xfffc03f8,0x380e00f,0xc001f800,0x3f0007e0,0xfc0000,0x61f800,0x78e00f1c,0x1e3803c,0x7001c007,
	  0x1f000700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x70001c00,0x0,
	  0x1c0000,0x0,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0xe00000,0x0,0x0,0xc000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,
	  0x0,0x0,0x0,0x0,0xe00000,0x7000e000,0x0,0x0,0x0,0x0,0x0,0x1c00,0x0,0x1c00000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x0,0x1c000000,
	  0x70000e,0x1c000,0x0,0x0,0x1c0000,0xe000c180,0x10,0x20000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,
	  0x0,0x38,0x70e000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x18c000,0x2000,0x0,0x1f,0xf8003800,0x7fe00000,0x0,0x0,0x0,0x0,0x4000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x400000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x400000,
	  0x0,0x0,0x1c007,0x700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x30001800,
	  0x0,0x1c0000,0x0,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0xe00000,0x0,0x0,0xe000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1e000,
	  0x0,0x0,0x0,0x0,0x0,0xe00000,0x7000e000,0x0,0x0,0x0,0x0,0x0,0x1c00,0x0,0x1c00000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x0,0x1c000000,
	  0x70000e,0x1c000,0x0,0x0,0x1c0001,0xe001c380,0x10,0x20000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,
	  0x0,0x38,0x7fe000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x18c000,0x3000,0x0,0x1f,0xf8007000,0x7fe00000,0x0,0x0,0x0,0x0,0x6000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x1c007,0x700,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x38003800,
	  0x0,0x380000,0x1,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x1c00000,0x0,0x0,0x3c18000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf000,
	  0x0,0x0,0x0,0x0,0x0,0xfe0000,0x380fe000,0x0,0x0,0x0,0x0,0x0,0x3800,0x0,0x1c00000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x0,0x38000000,
	  0x78000e,0x3c000,0x0,0x0,0x180001,0xc0018300,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,0x0,
	  0x38,0x1f8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x18c000,0x1800,0x0,0x0,0x6000e000,0x1800000,0x0,0x0,0x0,0x0,0x3000,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x38007,0xe00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x18003000,
	  0x0,0x300000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1800000,0x0,0x0,0x1ff8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4000,0x0,0x0,
	  0x0,0x0,0x0,0xfe0000,0xfe000,0x0,0x0,0x0,0x0,0x0,0x607800,0x0,0x3c00000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x0,0x78000000,
	  0x3f800e,0x3f8000,0x0,0x0,0x300043,0xc0018200,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,
	  0x0,0x38,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x0,0x11800,0x0,0x0,0x6001ff00,0x1800000,0x0,0x0,0x0,0x0,0x23000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x23000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x78007,
	  0x1e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x600,0x0,0x0,0x1c007000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfe0000,
	  0xfe000,0x0,0x0,0x0,0x0,0x0,0x7ff000,0x0,0x7f800000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x3,0xf8000000,0x3f800e,0x3f8000,0x0,
	  0x0,0x10007f,0x80000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,0x0,0x38,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x3800,0x0,0x1f800,0x0,0x0,0x6001ff00,0x1800000,0x0,0x0,0x0,0x0,0x3f000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f8007,0xfe00,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7fff8,0x0,0x0,0x0,0x0,0x7fe000,0x0,
	  0x7f000000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x3,0xf0000000,0xf800e,0x3e0000,0x0,0x0,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3800,0x0,0x1f000,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x3e000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x3f0007,0xfc00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x7fff8,0x0,0x0,0x0,0x0,0x1fc000,0x0,0x7e000000,0x0,0x0,0x1c00,0x7000,0x0,0x0,0x0,0x3,0xc0000000,0xe,0x0,
	  0x0,0x0,0x3e,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x3800,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c0007,0xf000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7fff8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0xe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	// Definition of a 29x57 font
	const unsigned int font29x57[29*57*256/32] = {
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x781e00,0x0,0x0,0x7,0x81e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7c0000,0xf8000,0x7e00000,0x0,0x7,
	  0xc0000000,0x0,0x7c00,0xf80,0x7e000,0x0,0x7c00000,0xf80000,0x7e000000,0x0,0x0,0x1f00,0x3e0,0x1f800,0x0,0x0,0x0,0x3,0xe0000000,
	  0x7c00003f,0x0,0xf8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x3c3c00,0x0,0x0,0x3,0xc3c00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e1f00,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e0000,
	  0x1f0000,0x7e00000,0xf838001f,0xf80001f,0xf0000000,0x0,0x3e00,0x1f00,0x7e000,0x3e1f000,0x3e00000,0x1f00000,0x7e00003e,0x1f000000,
	  0x3e0,0xe0000f80,0x7c0,0x1f800,0x3e0e00,0x7c3e000,0x0,0x1,0xf0000000,0xf800003f,0x1f0f,0x800001f0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1e7800,0x0,0x0,
	  0x1,0xe7800000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1e0000,0x1e0000,0xff00001,0xfe38001f,0xf80003f,
	  0xf8000000,0x0,0x1e00,0x1e00,0xff000,0x3e1f000,0x1e00000,0x1e00000,0xff00003e,0x1f000000,0x7f8,0xe0000780,0x780,0x3fc00,0x7f8e00,
	  0x7c3e000,0x0,0x0,0xf0000000,0xf000007f,0x80001f0f,0x800001e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xef000,0x0,0x0,0x0,0xef000000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0000,0x3c0000,0x1e780003,0xfff8001f,0xf80003c,0x78000000,0x0,0xf00,0x3c00,0x1e7800,
	  0x3e1f000,0xf00000,0x3c00001,0xe780003e,0x1f000000,0xfff,0xe00003c0,0xf00,0x79e00,0xfffe00,0x7c3e000,0x0,0x0,0x78000001,0xe00000f3,
	  0xc0001f0f,0x800003c0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7e000,0x0,0x0,0x0,0x7e000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x78000,0x780000,0x3c3c0003,0x8ff0001f,0xf800078,0x3c000000,0x0,0x780,0x7800,0x3c3c00,0x3e1f000,0x780000,0x7800003,0xc3c0003e,
	  0x1f000000,0xe3f,0xc00001e0,0x1e00,0xf0f00,0xe3fc00,0x7c3e000,0x0,0x0,0x3c000003,0xc00001e1,0xe0001f0f,0x80000780,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x1f,0xf0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x7e000,0x0,0x0,0x0,0x7e000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc00,0x7e000,0xfe000,0x0,0x3c000,0xf00000,0x781e0003,
	  0x83e0001f,0xf800070,0x1c000000,0x0,0x3c0,0xf000,0x781e00,0x3e1f000,0x3c0000,0xf000007,0x81e0003e,0x1f000000,0xe0f,0x800000f0,
	  0x3c00,0x1e0780,0xe0f800,0x7c3e000,0x0,0x0,0x1e000007,0x800003c0,0xf0001f0f,0x80000f00,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0xf8000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3fc00,0x1fe000,0x3ff800,0x0,0x0,0x0,0x0,0x0,0x70,0x1c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c,0x78000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x78,0xf000000,0x0,0x0,0x780f0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7c0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x3fc00,0x1fe000,0x3ffc00,0x0,0x0,0x0,0x0,0x0,0x70,0x1c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f00000,0x3e000,0x3e00000,0x0,0x78,0x3c000000,0x0,0x1f000,0x3e0,
	  0x3e000,0x0,0x1f000000,0x3e0000,0x3e000000,0x0,0x0,0x7c00,0xf8,0xf800,0x0,0x0,0x0,0xf,0x80000000,0x1f00001f,0x0,0x3e,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x30000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf80000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0xf80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x781c0000,0x38,0xe000000,0x0,0x0,0x380e0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf80,0x0,0x0,0x0,0x0,0x0,0x0,0x39c00,0x1ce000,0x303e00,
	  0x0,0x0,0x0,0x0,0x0,0x78,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4000,0x0,0x0,0x0,0x0,
	  0x0,0x0,0xf80000,0x7c000,0x3e00000,0xf0380000,0x70,0x1c000000,0x0,0xf800,0x7c0,0x3e000,0x0,0xf800000,0x7c0000,0x3e000000,
	  0x0,0x3c0,0xe0003e00,0x1f0,0xf800,0x3c0e00,0x0,0x0,0x7,0xc0000000,0x3e00001f,0x0,0x7c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0xff,0x0,
	  0xf8,0xf8000,0x1c000,0x0,0x0,0x0,0x0,0x1f,0xc0000000,0x1ff8,0xff00,0x0,0x0,0x3fe000,0x0,0x1fc00001,0xfe000000,0x0,0x0,0x0,
	  0x0,0x7f800,0x0,0x0,0x0,0xff00000,0x0,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0xf8000000,0xfe,0x0,0x7f80,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x3f,0xf0000000,0x7fe0,0x0,0x0,0x780000,0x1,0xe0000000,0x0,0x780000,0x3,0xfe000000,0x78000,0x3c00,0xf000,0x7800003,0xffe00000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfc0000f0,0x3f000,0x0,0x0,0x3fc00,0x0,0x0,0x1fc000,0x0,0x0,0x0,0x1fc0,
	  0x0,0xff000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xfe1c0000,0x1c,0x1c000000,0x0,0x0,0x1c1c0,0x0,0x0,0x0,0x0,0x1fe0000,
	  0x0,0x0,0x1ff,0x1f0f8,0x0,0xff000,0x0,0x0,0x0,0x3f,0xff00000f,0x80000000,0xfe0,0x3f80,0xf00,0x0,0x0,0x0,0x1,0xf8000003,0xe0000000,
	  0x1c00,0xe000,0xe00,0x0,0x0,0x0,0x0,0x0,0x3c,0x78000000,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f0,0x3f80,0x1fc00,0xfe000,
	  0x7f0000,0x0,0x1fc07000,0x0,0x0,0x0,0x0,0x0,0x3f800,0x780000,0x78000,0x7f00001,0xfc38001f,0xf800070,0x1c000000,0x0,0x7800,
	  0x780,0x7f000,0x3e1f000,0x7800000,0x780000,0x7f00003e,0x1f0003f0,0x7f0,0xe0001e00,0x1e0,0x1fc00,0x7f0e00,0x7c3e000,0x0,0x3,
	  0xc0000000,0x3c00003f,0x80001f0f,0x80000078,0x1e0000,0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,0x1e078000,0x30000000,0x3ff,0xc00001e0,0xf0,
	  0x78000,0x1c000,0x0,0x0,0x0,0x0,0x1e0007f,0xf000007e,0x1ffff,0x7ffe0,0x1f80,0x3ffff80,0xfff803,0xfffff800,0xfff80007,0xff800000,
	  0x0,0x0,0x0,0x0,0x1ffe00,0x0,0xfe0003,0xfff80000,0x3ffe01ff,0xe00003ff,0xffe01fff,0xff0003ff,0xe01e0007,0x803ffff0,0xfff80,
	  0x3c000fc0,0x7800001f,0x8003f07e,0x1e000f,0xfe0007ff,0xf00003ff,0x8007ffe0,0x1fff8,0x7fffffe,0xf0003c1,0xe000079e,0xf1f,0x1f3e0,
	  0x1f01ff,0xfff8003f,0xf003c000,0x7fe0,0x3f00,0x0,0x3c0000,0x1,0xe0000000,0x0,0x780000,0xf,0xfe000000,0x78000,0x3c00,0xf000,
	  0x7800003,0xffe00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0xfc0000f0,0x3fe00,0x0,0x0,0xfff00,0x0,0x0,0x3fe000,
	  0x0,0x0,0x0,0x1dc0,0x0,0x3fff00,0x0,0x3ffff80,0x1f,0xffff8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff1c07ff,0x3c0f001e,0x3c000000,
	  0x0,0x0,0x1e3c0,0xf80007c,0x0,0x780000,0x0,0xfff8000,0x3e00,0x1f00000,0x7ff,0xc001f0f8,0x0,0x3ffc00,0x0,0x0,0x0,0x3f,0xff00003f,
	  0xe0000000,0x3ff8,0xffe0,0x1e00,0x0,0xfffc00,0x0,0x7,0xf800000f,0xf8000000,0x1c00,0xe000,0xe00,0xf000,0x1fc000,0xfe0000,0x7f00000,
	  0x3f800001,0xfc00003f,0xf80000ff,0xffc003ff,0xe007ffff,0xc03ffffe,0x1fffff0,0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01ffc,
	  0xfc00,0x3c001ffc,0xffe0,0x7ff00,0x3ff800,0x1ffc000,0x0,0x7ff8f0f0,0x3c0780,0x1e03c00,0xf01e000,0x783e0001,0xf01e0000,0xffe00,
	  0x3c0000,0xf0000,0x7700001,0xfe38001f,0xf800070,0x1c000000,0x0,0x3c00,0xf00,0x77000,0x3e1f000,0x3c00000,0xf00000,0x7700003e,
	  0x1f0000f8,0xc0007f8,0xe0000f00,0x3c0,0x1dc00,0x7f8e00,0x7c3e000,0x0,0x1,0xe0000000,0x7800003b,0x80001f0f,0x800000f0,0x1e0000,
	  0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x780000,0x3c1e0000,0x1e070000,0x300001f0,0x7ff,0xc00001e0,0x1e0,0x7c000,0x1c000,0x0,0x0,0x0,0x0,0x3c000ff,0xf80007fe,
	  0x3ffff,0x801ffff8,0x1f80,0x3ffff80,0x3fff803,0xfffff801,0xfffc000f,0xffc00000,0x0,0x0,0x0,0x0,0x7fff80,0x0,0xfe0003,0xffff0000,
	  0xffff01ff,0xfc0003ff,0xffe01fff,0xff000fff,0xf01e0007,0x803ffff0,0xfff80,0x3c001f80,0x7800001f,0xc007f07e,0x1e001f,0xff0007ff,
	  0xfc0007ff,0xc007fffc,0x3fffc,0x7fffffe,0xf0003c1,0xf0000f9e,0xf0f,0x8003e1e0,0x1e01ff,0xfff8003f,0xf001e000,0x7fe0,0x3f00,
	  0x0,0x1e0000,0x1,0xe0000000,0x0,0x780000,0x1f,0xfe000000,0x78000,0x3c00,0xf000,0x7800003,0xffe00000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0xf,0xfc0000f0,0x3ff00,0x0,0x0,0x1fff80,0x0,0x0,0xffe000,0x0,0x0,0x0,0x3de0,0x0,0x7fff80,0x0,0xfffff80,
	  0x1f,0xffff8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xe7bc07ff,0x3e1f000f,0x78000000,0x0,0x0,0xf780,0x7800078,0x0,0x780000,0x180000,
	  0x1fff8000,0x1e00,0x1e0003c,0xfff,0xc001f0f8,0x0,0x7ffe00,0x0,0x0,0x0,0x3f,0xff00007f,0xf0000000,0x3ffc,0xfff0,0x3c00,0x0,
	  0x7fffc00,0x0,0x7,0xf800003f,0xfe000000,0x1c00,0xe000,0xe00,0xf000,0x1fc000,0xfe0000,0x7f00000,0x3f800001,0xfc00001f,0xe00001ff,
	  0xffc00fff,0xf007ffff,0xc03ffffe,0x1fffff0,0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01fff,0xc000fc00,0x3c003ffe,0x1fff0,
	  0xfff80,0x7ffc00,0x3ffe000,0x0,0xfffce0f0,0x3c0780,0x1e03c00,0xf01e000,0x781e0001,0xe01e0000,0x3fff00,0x1e0000,0x1e0000,0xf780003,
	  0xcf78001f,0xf800078,0x3c000000,0x0,0x1e00,0x1e00,0xf7800,0x3e1f000,0x1e00000,0x1e00000,0xf780003e,0x1f0000fc,0x7c000f3d,
	  0xe0000780,0x780,0x3de00,0xf3de00,0x7c3e000,0x0,0x0,0xf0000000,0xf000007b,0xc0001f0f,0x800001e0,0x1e0000,0x3e1f00,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,
	  0x3c1e0000,0x1e0f0000,0x300007fc,0xfff,0xc00001e0,0x1e0,0x3c000,0x1c000,0x0,0x0,0x0,0x0,0x3c001ff,0xfc001ffe,0x3ffff,0xc01ffffc,
	  0x3f80,0x3ffff80,0x7fff803,0xfffff803,0xfffe001f,0xffe00000,0x0,0x0,0x0,0x0,0xffff80,0x7f800,0xfe0003,0xffff8001,0xffff01ff,
	  0xff0003ff,0xffe01fff,0xff001fff,0xf01e0007,0x803ffff0,0xfff80,0x3c003f00,0x7800001f,0xc007f07f,0x1e003f,0xff8007ff,0xff000fff,
	  0xe007ffff,0x7fffc,0x7fffffe,0xf0003c0,0xf0000f1e,0xf07,0x8003c1f0,0x3e01ff,0xfff8003f,0xf001e000,0x7fe0,0x7f80,0x0,0xe0000,
	  0x1,0xe0000000,0x0,0x780000,0x1f,0xfe000000,0x78000,0x3c00,0xf000,0x7800003,0xffe00000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,
	  0x0,0x0,0x0,0x0,0xf,0xfc0000f0,0x3ff00,0x0,0x0,0x3fff80,0x0,0x0,0xffe000,0x0,0x0,0x0,0x78f0,0x0,0xffff80,0x0,0x3fffff80,0x1f,
	  0xffff8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xc7f80070,0x3e1f0007,0x70000000,0x0,0x0,0x7700,0x7c000f8,0x0,0x780000,0x180000,
	  0x3fff8000,0x1f00,0x3e0003c,0x1f03,0xc001f0f8,0x0,0x703f00,0x0,0x0,0x0,0x3f,0xff0000f0,0xf8000000,0x303e,0xc0f8,0x7800,0x0,
	  0xffffc00,0x0,0x7,0x3800003e,0x3e000000,0x1c00,0xe000,0x3c00,0xf000,0x1fc000,0xfe0000,0x7f00000,0x3f800001,0xfc00000f,0xe00001ff,
	  0xffc01fff,0xf007ffff,0xc03ffffe,0x1fffff0,0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01fff,0xf000fe00,0x3c007fff,0x3fff8,
	  0x1fffc0,0xfffe00,0x7fff000,0x1,0xffffc0f0,0x3c0780,0x1e03c00,0xf01e000,0x781f0003,0xe01e0000,0x3fff80,0xe0000,0x3c0000,0x1e3c0003,
	  0x8ff0001f,0xf80003c,0x78000000,0x0,0xe00,0x3c00,0x1e3c00,0x3e1f000,0xe00000,0x3c00001,0xe3c0003e,0x1f00007f,0xf8000e3f,0xc0000380,
	  0xf00,0x78f00,0xe3fc00,0x7c3e000,0x0,0x0,0x70000001,0xe00000f1,0xe0001f0f,0x800003c0,0x1e0000,0x3e1f00,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,0x3c0f0000,
	  0x30000ffe,0xf80,0xc00001e0,0x3c0,0x1e000,0x101c040,0x0,0x0,0x0,0x0,0x78003f0,0x7e001ffe,0x3f807,0xe01f00fe,0x3f80,0x3ffff80,
	  0x7e01803,0xfffff007,0xe03f003f,0x3f00000,0x0,0x0,0x0,0x0,0xfc0fc0,0x3ffe00,0xfe0003,0xffffc003,0xf81f01ff,0xff8003ff,0xffe01fff,
	  0xff003f01,0xf01e0007,0x803ffff0,0xfff80,0x3c007e00,0x7800001f,0xc007f07f,0x1e007e,0xfc007ff,0xff801f83,0xf007ffff,0x800fc07c,
	  0x7fffffe,0xf0003c0,0xf0000f0f,0x1e07,0xc007c0f8,0x7c01ff,0xfff8003c,0xf000,0x1e0,0xffc0,0x0,0xf0000,0x1,0xe0000000,0x0,0x780000,
	  0x3e,0x0,0x78000,0x3c00,0xf000,0x7800000,0x1e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,0x0,0x0,0x0,0x0,0x1f,0x800000f0,0x1f80,
	  0x0,0x0,0x7e0780,0x0,0x0,0x1f82000,0x0,0x0,0x0,0x7070,0x0,0x1f80f80,0x0,0x7fffff80,0x1f,0xffff8000,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x1,0xc3f80070,0x3f3f0007,0xf0000000,0x0,0x0,0x7f00,0x3e001f0,0x0,0x780000,0x180000,0x7f018000,0xf80,0x7c0003c,0x3e00,
	  0x4001f0f8,0xfe00,0x400f00,0x0,0x0,0x0,0x7f000000,0xe0,0x38000000,0x1e,0x38,0x7800,0x0,0x1ffe1c00,0x0,0x0,0x38000078,0xf000000,
	  0x1c00,0xe000,0x7f800,0xf000,0x1fc000,0xfe0000,0x7f00000,0x3f800001,0xfc00001f,0xf00001ff,0xffc03f81,0xf007ffff,0xc03ffffe,
	  0x1fffff0,0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01fff,0xf800fe00,0x3c00fc1f,0x8007e0fc,0x3f07e0,0x1f83f00,0xfc1f800,
	  0x3,0xf07fc0f0,0x3c0780,0x1e03c00,0xf01e000,0x780f8007,0xc01e0000,0x7e0fc0,0xf0000,0x3c0000,0x1c1c0003,0x87f0001f,0xf80003f,
	  0xf8000000,0x0,0xf00,0x3c00,0x1c1c00,0x3e1f000,0xf00000,0x3c00001,0xc1c0003e,0x1f00003f,0xc0000e1f,0xc00003c0,0xf00,0x70700,
	  0xe1fc00,0x7c3e000,0x0,0x0,0x78000001,0xe00000e0,0xe0001f0f,0x800003c0,0x1e0000,0x3e1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,0x3c0f0001,0xff801e0f,
	  0x1f00,0x1e0,0x3c0,0x1e000,0x3c1c1e0,0x0,0x0,0x0,0x0,0x78007c0,0x1f001f9e,0x3c001,0xf010003e,0x7780,0x3c00000,0xf800000,0xf007,
	  0xc01f007c,0x1f80000,0x0,0x0,0x0,0x0,0xe003e0,0x7fff00,0x1ef0003,0xc007e007,0xc00301e0,0x1fc003c0,0x1e00,0x7c00,0x301e0007,
	  0x80007800,0x780,0x3c00fc00,0x7800001f,0xe00ff07f,0x1e00f8,0x3e00780,0x1fc03e00,0xf807801f,0xc01f001c,0xf000,0xf0003c0,0xf0000f0f,
	  0x1e03,0xc00f8078,0x780000,0xf0003c,0xf000,0x1e0,0x1f3e0,0x0,0x78000,0x1,0xe0000000,0x0,0x780000,0x3c,0x0,0x78000,0x0,0x0,
	  0x7800000,0x1e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,0x0,0x0,0x0,0x0,0x1f,0xf0,0xf80,0x0,0x0,0xf80180,0x0,0x0,0x1e00000,
	  0x0,0x0,0x0,0xe038,0x0,0x3e00380,0x0,0xfe0f0000,0x0,0xf0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xc0f00070,0x3b370003,0xe0000000,
	  0x0,0x0,0x3e00,0x1e001e0,0x0,0x780000,0x180000,0x7c000000,0x780,0x780003c,0x3c00,0x0,0x7ffc0,0x780,0x0,0x0,0x3,0xffe00000,
	  0x1c0,0x3c000000,0xe,0x38,0xf000,0x0,0x3ffe1c00,0x0,0x0,0x38000078,0xf000000,0x1c00,0xe000,0x7f000,0xf000,0x3de000,0x1ef0000,
	  0xf780000,0x7bc00003,0xde00001e,0xf00003e7,0x80007c00,0x30078000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,
	  0xe0001e03,0xfc00fe00,0x3c01f007,0xc00f803e,0x7c01f0,0x3e00f80,0x1f007c00,0x7,0xc01f80f0,0x3c0780,0x1e03c00,0xf01e000,0x78078007,
	  0x801e0000,0x7803c0,0x78000,0x780000,0x380e0003,0x81e00000,0x1f,0xf0000000,0x0,0x780,0x7800,0x380e00,0x0,0x780000,0x7800003,
	  0x80e00000,0x1ff,0x80000e07,0x800001e0,0x1e00,0xe0380,0xe07800,0x0,0x0,0x0,0x3c000003,0xc00001c0,0x70000000,0x780,0x1e0000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x780000,0x3c1e0000,0x3c0e0007,0xfff01c07,0x1e00,0x1e0,0x780,0xf000,0x3e1c3e0,0x0,0x0,0x0,0x0,0xf0007c0,0x1f00181e,0x20000,
	  0xf000001f,0xf780,0x3c00000,0x1f000000,0x1f00f,0x800f8078,0xf80000,0x0,0x0,0x0,0x0,0x8003e0,0x1fc0f80,0x1ef0003,0xc001e007,
	  0x800101e0,0x7e003c0,0x1e00,0x7800,0x101e0007,0x80007800,0x780,0x3c00f800,0x7800001e,0xe00ef07f,0x801e00f0,0x1e00780,0x7c03c00,
	  0x78078007,0xc01e0004,0xf000,0xf0003c0,0x78001e0f,0x1e03,0xe00f807c,0xf80000,0x1f0003c,0x7800,0x1e0,0x3e1f0,0x0,0x3c000,0x1,
	  0xe0000000,0x0,0x780000,0x3c,0x0,0x78000,0x0,0x0,0x7800000,0x1e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,0x0,0x0,0x0,0x0,
	  0x1e,0xf0,0x780,0x0,0x0,0x1f00080,0x0,0x0,0x3c00000,0x0,0x0,0x0,0x1e03c,0x0,0x3c00080,0x0,0xf80f0000,0x0,0x1f0000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x70,0x3bf70003,0xe0000000,0x0,0x0,0x3e00,0x1f003e0,0x0,0x780000,0x180000,0x78000000,0x7c0,0xf80003c,
	  0x3c00,0x0,0x1f01f0,0x780,0x0,0x0,0xf,0x80f80000,0x1c0,0x1c000000,0xe,0x38,0x1e000,0x0,0x7ffe1c00,0x0,0x0,0x380000f0,0x7800000,
	  0x1c00,0xe000,0x7fc00,0xf000,0x3de000,0x1ef0000,0xf780000,0x7bc00003,0xde00001e,0xf00003c7,0x80007800,0x10078000,0x3c0000,
	  0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x7e00ff00,0x3c01e003,0xc00f001e,0x7800f0,0x3c00780,0x1e003c00,
	  0x7,0x800f00f0,0x3c0780,0x1e03c00,0xf01e000,0x7807c00f,0x801e0000,0xf803c0,0x3c000,0xf00000,0x780f0000,0x0,0x7,0xc0000000,
	  0x0,0x3c0,0xf000,0x780f00,0x0,0x3c0000,0xf000007,0x80f00000,0x7ff,0xc0000000,0xf0,0x3c00,0x1e03c0,0x0,0x0,0x0,0x0,0x1e000007,
	  0x800003c0,0x78000000,0xf00,0x1e0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,0x3c1e001f,0xfff03803,0x80001e00,0x1e0,0x780,0xf000,0xf9cf80,
	  0x0,0x0,0x0,0x0,0xf000780,0xf00001e,0x0,0xf800000f,0xe780,0x3c00000,0x1e000000,0x1e00f,0x78078,0x7c0000,0x0,0x0,0x0,0x0,0x1e0,
	  0x3f003c0,0x1ef0003,0xc000f00f,0x800001e0,0x1f003c0,0x1e00,0xf000,0x1e0007,0x80007800,0x780,0x3c01f000,0x7800001e,0xe00ef07f,
	  0x801e01f0,0x1e00780,0x3c07c00,0x78078003,0xc03e0000,0xf000,0xf0003c0,0x78001e0f,0x1e01,0xf01f003c,0xf00000,0x3e0003c,0x7800,
	  0x1e0,0x7c0f8,0x0,0x0,0x1,0xe0000000,0x0,0x780000,0x3c,0x0,0x78000,0x0,0x0,0x7800000,0x1e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,
	  0x0,0x0,0x0,0x0,0x0,0x1e,0xf0,0x780,0x0,0x0,0x1e00000,0x0,0x0,0x3c00000,0x0,0x8,0x40,0x0,0x7e0000,0x7c00000,0x1,0xf00f0000,
	  0x0,0x3e0000,0x0,0x3f,0xfc0,0xfc3f0,0xfc3f0,0x0,0x0,0x0,0x70,0x39e70000,0x0,0x0,0x0,0x0,0xf003c0,0x0,0x0,0x180000,0xf8000000,
	  0x3c0,0xf00003c,0x3c00,0x0,0x3c0078,0x7ff80,0x0,0x0,0x1e,0x3c0000,0x1c0,0x1c000000,0xe,0xf0,0x0,0x0,0x7ffe1c00,0x0,0x0,0x380000f0,
	  0x7800000,0x1c00,0xe000,0x3c00,0x0,0x3de000,0x1ef0000,0xf780000,0x7bc00003,0xde00001e,0xf00003c7,0x8000f800,0x78000,0x3c0000,
	  0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x1f00ff00,0x3c03e003,0xc01f001e,0xf800f0,0x7c00780,0x3e003c00,
	  0xf,0x800f80f0,0x3c0780,0x1e03c00,0xf01e000,0x7803c00f,0x1fffc0,0xf001e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x307,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1e0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,0x781e003f,0xfff03803,
	  0x80001e00,0x1e0,0xf80,0xf000,0x3dde00,0x0,0x0,0x0,0x0,0xf000f00,0x780001e,0x0,0x7800000f,0x1e780,0x3c00000,0x3e000000,0x3e00f,
	  0x780f0,0x7c0000,0x0,0x0,0x0,0x0,0x1e0,0x7c001e0,0x3ef8003,0xc000f00f,0x1e0,0xf003c0,0x1e00,0xf000,0x1e0007,0x80007800,0x780,
	  0x3c03e000,0x7800001e,0xf01ef07b,0xc01e01e0,0xf00780,0x3e07800,0x3c078003,0xe03c0000,0xf000,0xf0003c0,0x78001e0f,0x1e00,0xf01e003e,
	  0x1f00000,0x3c0003c,0x7800,0x1e0,0x78078,0x0,0x0,0x1,0xe0000000,0x0,0x780000,0x3c,0x0,0x78000,0x0,0x0,0x7800000,0x1e00000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x3c000,0x0,0x0,0x0,0x0,0x0,0x1e,0xf0,0x780,0x0,0x0,0x1e00000,0x0,0x0,0x3c00000,0x0,0x18,0xc0,0x0,
	  0xe70000,0x7800000,0x1,0xe00f0000,0x0,0x3c0000,0x0,0x3f,0xfc0,0xfc1f0,0x1f83f0,0x0,0x0,0x0,0x70,0x39e70000,0x0,0x0,0x0,0x0,
	  0xf807c0,0x0,0x0,0x180000,0xf0000000,0x3e0,0x1f00003c,0x3e00,0x0,0x70001c,0x3fff80,0x0,0x0,0x38,0xe0000,0x1c0,0x1c000078,
	  0x1c,0x1fe0,0x0,0x0,0xfffe1c00,0x0,0x0,0x380000f0,0x7800000,0x1c00,0xe000,0xe00,0x0,0x7df000,0x3ef8000,0x1f7c0000,0xfbe00007,
	  0xdf00003c,0x780003c7,0x8000f000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0xf00f780,
	  0x3c03c001,0xe01e000f,0xf00078,0x78003c0,0x3c001e00,0xf,0xf80f0,0x3c0780,0x1e03c00,0xf01e000,0x7803e01f,0x1ffff8,0xf001e0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0xc000,0x0,0x0,0x0,0x0,0x1e0000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x780000,0x3c1e0000,0x781e003e,0x30703803,0x80001e00,0x1e0,0xf00,0x7800,0xff800,0x1e0000,0x0,0x0,0x0,0x1e000f00,0x780001e,
	  0x0,0x7800000f,0x3c780,0x3c00000,0x3c000000,0x3c00f,0x780f0,0x3c0000,0x0,0x0,0x2000000,0x800000,0x1e0,0x78000e0,0x3c78003,
	  0xc000f01e,0x1e0,0xf803c0,0x1e00,0x1e000,0x1e0007,0x80007800,0x780,0x3c07c000,0x7800001e,0x701cf07b,0xc01e01e0,0xf00780,0x1e07800,
	  0x3c078001,0xe03c0000,0xf000,0xf0003c0,0x7c003e0f,0x1e00,0xf83e001e,0x1e00000,0x7c0003c,0x3c00,0x1e0,0xf807c,0x0,0x0,0x1fe0001,
	  0xe1fc0000,0x7f00003,0xf8780007,0xf000003c,0x7f0,0x783f0,0x0,0x0,0x7800000,0x1e00000,0x3e0f8000,0xfc00007,0xf8000007,0xf00001fc,
	  0xf,0xc0003fc0,0x3c000,0x0,0x0,0x0,0x0,0x0,0x1e,0xf0,0x780,0x0,0x0,0x3c00000,0x0,0x0,0x3c00000,0x0,0x18,0xc0,0x0,0x1818000,
	  0x7800000,0x1,0xe00f0000,0x0,0x7c0000,0x0,0x1f,0x80001f80,0x7c1f8,0x1f83e0,0x0,0x0,0x0,0x70,0x38c70007,0xf8000000,0x7f03,
	  0xf0000000,0x0,0x780780,0x0,0x0,0xfe0000,0xf0000000,0x1e0,0x1e00003c,0x3f00,0x0,0xe07f0e,0x7fff80,0x0,0x0,0x70,0x70000,0x1c0,
	  0x1c000078,0x3c,0x1fc0,0x0,0x0,0xfffe1c00,0x0,0x0,0x380000f0,0x7800000,0x1c00,0xe000,0xe00,0x0,0x78f000,0x3c78000,0x1e3c0000,
	  0xf1e00007,0x8f00003c,0x78000787,0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,
	  0xf80f780,0x3c03c001,0xe01e000f,0xf00078,0x78003c0,0x3c001e00,0xf,0x1f80f0,0x3c0780,0x1e03c00,0xf01e000,0x7801e01e,0x1ffffc,
	  0xf007e0,0x3fc000,0x1fe0000,0xff00000,0x7f800003,0xfc00001f,0xe0000fc0,0xfc00007f,0xfe0,0x7f00,0x3f800,0x1fc000,0x0,0x0,0x0,
	  0x1,0xf000001f,0x80000ff0,0x7f80,0x3fc00,0x1fe000,0xff0000,0x1f80000,0x1fc1e000,0x0,0x0,0x0,0x0,0x1e1fc0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e0000,
	  0x781c007c,0x30003803,0x80001f00,0x1e0,0xf00,0x7800,0x7f000,0x1e0000,0x0,0x0,0x0,0x1e000f00,0x780001e,0x0,0x7800000f,0x3c780,
	  0x3c00000,0x3c000000,0x3c00f,0x780f0,0x3c0000,0x0,0x0,0x1e000000,0xf00000,0x3e0,0xf0000e0,0x3c78003,0xc000f01e,0x1e0,0x7803c0,
	  0x1e00,0x1e000,0x1e0007,0x80007800,0x780,0x3c0f8000,0x7800001e,0x701cf079,0xe01e01e0,0xf00780,0x1e07800,0x3c078001,0xe03c0000,
	  0xf000,0xf0003c0,0x3c003c0f,0x3e00,0x787c001f,0x3e00000,0xf80003c,0x3c00,0x1e0,0x1f003e,0x0,0x0,0x1fffc001,0xe7ff0000,0x3ffe000f,
	  0xfe78003f,0xfc001fff,0xfe001ffc,0xf0078ffc,0x1ffc00,0x7ff000,0x7800f80,0x1e0000f,0x7f1fc01e,0x3ff0001f,0xfe00079f,0xfc0007ff,
	  0x3c003c7f,0xf001fff8,0x1fffff0,0x3c003c0,0xf0000f1e,0xf1f,0x7c1f0,0x1f00ff,0xffe0001e,0xf0,0x780,0x0,0x0,0x3c00000,0x100000,
	  0x0,0x7800000,0x0,0x18,0xc0,0x0,0x1818000,0x7800000,0x1,0xe00f0000,0x1000000,0xf80000,0x40000002,0xf,0x80001f00,0x7e0f8,0x1f07c0,
	  0x0,0x0,0x0,0x70,0x38c7003f,0xff000000,0xff8f,0xf8000100,0xffffe,0x7c0f80,0x0,0x0,0x3ffc000,0xf0000020,0x1001f0,0x3c00003c,
	  0x1f80,0x0,0x1c3ffc7,0x7c0780,0x0,0x0,0xe3,0xff038000,0xe0,0x38000078,0x78,0x1ff0,0x0,0x3c003c0,0xfffe1c00,0x0,0x0,0x380000f0,
	  0x7800000,0x1c00,0xe000,0xe00,0xf000,0x78f000,0x3c78000,0x1e3c0000,0xf1e00007,0x8f00003c,0x78000787,0x8001e000,0x78000,0x3c0000,
	  0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x780f3c0,0x3c03c001,0xe01e000f,0xf00078,0x78003c0,0x3c001e00,
	  0x4000200f,0x3f80f0,0x3c0780,0x1e03c00,0xf01e000,0x7801f03e,0x1ffffe,0xf01fe0,0x3fff800,0x1fffc000,0xfffe0007,0xfff0003f,
	  0xff8001ff,0xfc003ff3,0xfe0003ff,0xe0007ff8,0x3ffc0,0x1ffe00,0xfff000,0x3ff80001,0xffc0000f,0xfe00007f,0xf000003f,0xf8003c7f,
	  0xe0003ffc,0x1ffe0,0xfff00,0x7ff800,0x3ffc000,0x1f80000,0xfff1c03c,0x3c01e0,0x1e00f00,0xf007800,0x781f0001,0xf01e7ff0,0x7c0007c,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,
	  0x3c1e003f,0xfffff078,0x30003803,0x80000f00,0x1e0,0x1f00,0x7800,0x7f000,0x1e0000,0x0,0x0,0x0,0x3c000f00,0x780001e,0x0,0x7800000f,
	  0x78780,0x3c00000,0x3c000000,0x7c00f,0x780f0,0x3c0007,0xe000003f,0x0,0xfe000000,0xfe0000,0x3c0,0x1f000070,0x7c7c003,0xc000f01e,
	  0x1e0,0x7803c0,0x1e00,0x1e000,0x1e0007,0x80007800,0x780,0x3c1f0000,0x7800001e,0x783cf079,0xe01e03c0,0xf00780,0x1e0f000,0x3c078001,
	  0xe03c0000,0xf000,0xf0003c0,0x3c003c07,0x81f03c00,0x7c7c000f,0x87c00000,0xf00003c,0x1e00,0x1e0,0x3e001f,0x0,0x0,0x3fffe001,
	  0xefff8000,0x7fff001f,0xff78007f,0xfe001fff,0xfe003ffe,0xf0079ffe,0x1ffc00,0x7ff000,0x7801f00,0x1e0000f,0xffbfe01e,0x7ff8003f,
	  0xff0007bf,0xfe000fff,0xbc003cff,0xf803fffc,0x1fffff0,0x3c003c0,0x78001e1e,0xf0f,0x800f80f0,0x1e00ff,0xffe0001e,0xf0,0x780,
	  0x0,0x0,0x3c00000,0x380000,0x0,0x7800000,0x0,0x18,0xc0,0x0,0x1008000,0x7800000,0x3,0xe00f0000,0x3800000,0xf00000,0xe0000007,
	  0xf,0x80001f00,0x3e0f8,0x1e07c0,0x0,0x0,0x0,0x70,0x3807007f,0xff800000,0x1ffdf,0xfc000380,0xffffe,0x3e1f00,0x0,0x0,0xfffe000,
	  0xf0000030,0x3800f8,0x7c00003c,0xfc0,0x0,0x18780c3,0xf00780,0x80100,0x0,0xc3,0xffc18000,0xf0,0x78000078,0xf0,0xf0,0x0,0x3c003c0,
	  0xfffe1c00,0x0,0x0,0x380000f0,0x7800801,0x1c00,0xe000,0x1e00,0xf000,0xf8f800,0x7c7c000,0x3e3e0001,0xf1f0000f,0x8f80007c,0x7c000787,
	  0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x780f3c0,0x3c078001,0xe03c000f,
	  0x1e00078,0xf0003c0,0x78001e00,0xe000701f,0x3fc0f0,0x3c0780,0x1e03c00,0xf01e000,0x7800f87c,0x1e007f,0xf07e00,0x7fffc00,0x3fffe001,
	  0xffff000f,0xfff8007f,0xffc003ff,0xfe007ff7,0xff0007ff,0xf000fffc,0x7ffe0,0x3fff00,0x1fff800,0x3ff80001,0xffc0000f,0xfe00007f,
	  0xf00000ff,0xf8003cff,0xf0007ffe,0x3fff0,0x1fff80,0xfffc00,0x7ffe000,0x1f80001,0xfffb803c,0x3c01e0,0x1e00f00,0xf007800,0x780f0001,
	  0xe01efff8,0x3c00078,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x780000,0x3c1e003f,0xfffff078,0x30001c07,0xf80,0x1e0,0x1e00,0x3c00,0xff800,0x1e0000,0x0,0x0,0x0,0x3c001e00,
	  0x3c0001e,0x0,0x7800001e,0x70780,0x3c00000,0x78000000,0x78007,0x800f00f0,0x3e0007,0xe000003f,0x3,0xfe000000,0xff8000,0x7c0,
	  0x1e000070,0x783c003,0xc001f01e,0x1e0,0x7803c0,0x1e00,0x1e000,0x1e0007,0x80007800,0x780,0x3c3e0000,0x7800001e,0x3838f079,
	  0xe01e03c0,0x780780,0x1e0f000,0x1e078001,0xe03c0000,0xf000,0xf0003c0,0x3c007c07,0x81f03c00,0x3ef80007,0x87800000,0x1f00003c,
	  0x1e00,0x1e0,0x7c000f,0x80000000,0x0,0x3ffff001,0xffffc000,0xffff003f,0xff7800ff,0xff001fff,0xfe007ffe,0xf007bffe,0x1ffc00,
	  0x7ff000,0x7803e00,0x1e0000f,0xffffe01e,0xfff8007f,0xff8007ff,0xff001fff,0xbc003dff,0xf807fffc,0x1fffff0,0x3c003c0,0x78001e0f,
	  0x1e07,0xc01f00f0,0x1e00ff,0xffe0001e,0xf0,0x780,0x0,0x0,0x7c00000,0x7c0000,0x0,0x7800000,0x0,0x18,0xc0,0x0,0x1018000,0x7800000,
	  0x3,0xc00f0000,0x7c00000,0x1f00001,0xf000000f,0x80000007,0xc0003e00,0x1e07c,0x3e0780,0x0,0x0,0x0,0x70,0x380700ff,0xff800000,
	  0x3ffff,0xfe0007c0,0xffffe,0x1e1e00,0x0,0x780000,0x1fffe000,0xf0000078,0x7c0078,0x7800003c,0xff0,0x0,0x38e0003,0x80f00780,
	  0x180300,0x0,0x1c3,0x81e1c000,0x7f,0xf0000078,0x1e0,0x38,0x0,0x3c003c0,0xfffe1c00,0x0,0x0,0x380000f0,0x7800c01,0x80001c00,
	  0xe000,0x603e00,0xf000,0xf07800,0x783c000,0x3c1e0001,0xe0f0000f,0x7800078,0x3c000f87,0x8001e000,0x78000,0x3c0000,0x1e00000,
	  0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x780f3c0,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f01,0xf000f81e,
	  0x7bc0f0,0x3c0780,0x1e03c00,0xf01e000,0x78007878,0x1e001f,0xf0f800,0x7fffe00,0x3ffff001,0xffff800f,0xfffc007f,0xffe003ff,
	  0xff007fff,0xff800fff,0xf001fffe,0xffff0,0x7fff80,0x3fffc00,0x3ff80001,0xffc0000f,0xfe00007f,0xf00001ff,0xfc003dff,0xf000ffff,
	  0x7fff8,0x3fffc0,0x1fffe00,0xffff000,0x1f80003,0xffff803c,0x3c01e0,0x1e00f00,0xf007800,0x780f0001,0xe01ffffc,0x3c00078,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,
	  0x3c1e003f,0xfffff078,0x30001e0f,0x300780,0x1e0,0x1e00,0x3c00,0x3dde00,0x1e0000,0x0,0x0,0x0,0x78001e00,0x3c0001e,0x0,0xf800003e,
	  0xf0780,0x3dfc000,0x783f8000,0xf8007,0xc01f00f0,0x3e0007,0xe000003f,0x1f,0xfc000000,0x7ff000,0xf80,0x3e007c70,0x783c003,0xc001e03c,
	  0x1e0,0x3c03c0,0x1e00,0x3c000,0x1e0007,0x80007800,0x780,0x3c7c0000,0x7800001e,0x3878f078,0xf01e03c0,0x780780,0x1e0f000,0x1e078001,
	  0xe03e0000,0xf000,0xf0003c0,0x1e007807,0x83f03c00,0x3ef00007,0xcf800000,0x3e00003c,0xf00,0x1e0,0xf80007,0xc0000000,0x0,0x3e01f801,
	  0xfe07e001,0xf80f007e,0x7f801f8,0x1f801fff,0xfe00fc0f,0xf007f83f,0x1ffc00,0x7ff000,0x7807c00,0x1e0000f,0x87e1e01f,0xe0fc00fc,
	  0xfc007f8,0x1f803f03,0xfc003df0,0x3807e03c,0x1fffff0,0x3c003c0,0x78003e0f,0x1e03,0xe03e00f8,0x3e00ff,0xffe0001e,0xf0,0x780,
	  0x0,0x0,0x7800000,0xfe0000,0x0,0x7800000,0x0,0x18,0xc0,0x0,0x1818000,0x7c00000,0x3,0xc00f0000,0xfe00000,0x3e00003,0xf800001f,
	  0xc0000007,0xc0003e00,0x1e03c,0x3c0f80,0x0,0x0,0x0,0x70,0x380700fc,0x7800000,0x7c1fe,0x3e000fe0,0xffffe,0x1f3e00,0x0,0x780000,
	  0x3f98e000,0xf000003c,0xfcf8007c,0xf800003c,0x3ffc,0x0,0x31c0001,0x80f00f80,0x380700,0x0,0x183,0x80e0c000,0x3f,0xe0000078,
	  0x3c0,0x38,0x0,0x3c003c0,0xfffe1c00,0x0,0x0,0x38000078,0xf000e01,0xc003ffe0,0x1fff00,0x7ffc00,0xf000,0xf07800,0x783c000,0x3c1e0001,
	  0xe0f0000f,0x7800078,0x3c000f07,0x8003c000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,
	  0x3c0f1e0,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0xf801f01e,0xf3c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78007cf8,
	  0x1e000f,0x80f0f000,0x7c03f00,0x3e01f801,0xf00fc00f,0x807e007c,0x3f003e0,0x1f80707f,0x8f801f80,0xf003f03f,0x1f81f8,0xfc0fc0,
	  0x7e07e00,0x3ff80001,0xffc0000f,0xfe00007f,0xf00003ff,0xfc003fc1,0xf801f81f,0x800fc0fc,0x7e07e0,0x3f03f00,0x1f81f800,0x1f80007,
	  0xe07f003c,0x3c01e0,0x1e00f00,0xf007800,0x780f8003,0xe01fe07e,0x3e000f8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3f,0xfffff078,0x30000ffe,0x1f007c0,0x0,0x1e00,
	  0x3c00,0xf9cf80,0x1e0000,0x0,0x0,0x0,0x78001e00,0x3c0001e,0x0,0xf00000fc,0x1e0780,0x3fff800,0x78ffe000,0xf0003,0xe03e00f0,
	  0x3e0007,0xe000003f,0x7f,0xe01fffff,0xf00ffc00,0x1f80,0x3c01ff70,0x783c003,0xc007e03c,0x1e0,0x3c03c0,0x1e00,0x3c000,0x1e0007,
	  0x80007800,0x780,0x3cfc0000,0x7800001e,0x3c78f078,0xf01e03c0,0x780780,0x3e0f000,0x1e078003,0xc01f0000,0xf000,0xf0003c0,0x1e007807,
	  0x83f83c00,0x1ff00003,0xcf000000,0x3e00003c,0xf00,0x1e0,0x0,0x0,0x0,0x20007801,0xfc03e003,0xe003007c,0x3f803e0,0x7c0003c,
	  0xf807,0xf007e00f,0x3c00,0xf000,0x780f800,0x1e0000f,0x87e1f01f,0x803c00f8,0x7c007f0,0xf803e01,0xfc003f80,0x80f8004,0x3c000,
	  0x3c003c0,0x3c003c0f,0x1e03,0xe03e0078,0x3c0000,0x7c0001e,0xf0,0x780,0x0,0x0,0x3ffff800,0x1ff0000,0x0,0x7800000,0x0,0x18,
	  0xc0,0x0,0x1818000,0x3e00000,0x3,0xc00f0000,0x1ff00000,0x3e00007,0xfc00003f,0xe0000003,0xc0003c00,0xf03c,0x3c0f00,0x0,0x0,
	  0x0,0x70,0x380701f0,0x800000,0x780fc,0x1e001ff0,0x7c,0xf3c00,0x0,0x780000,0x7e182000,0xf000001f,0xfff00ffc,0xffc0003c,0x3cfe,
	  0x0,0x31c0001,0x80f01f80,0x780f00,0x0,0x183,0x80e0c000,0xf,0x80000078,0x780,0x38,0x0,0x3c003c0,0x7ffe1c00,0x0,0x0,0x38000078,
	  0xf000f01,0xe003ffe0,0x1fff00,0x7ff800,0xf000,0xf07800,0x783c000,0x3c1e0001,0xe0f0000f,0x78000f8,0x3e000f07,0x8003c000,0x78000,
	  0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x3c0f1e0,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,
	  0x78000f00,0x7c03e01e,0x1e3c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78003cf0,0x1e0007,0x80f1e000,0x4000f00,0x20007801,0x3c008,
	  0x1e0040,0xf00200,0x780403f,0x7803e00,0x3007c00f,0x803e007c,0x1f003e0,0xf801f00,0x780000,0x3c00000,0x1e000000,0xf00007f0,
	  0x3e003f00,0x7801f00f,0x800f807c,0x7c03e0,0x3e01f00,0x1f00f800,0x1f80007,0xc03e003c,0x3c01e0,0x1e00f00,0xf007800,0x78078003,
	  0xc01fc03e,0x1e000f0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x780000,0x0,0xf078007c,0x300007fc,0x7e00fe0,0x0,0x1e00,0x3c00,0x3e1c3e0,0x1e0000,0x0,0x0,0x0,0xf0001e00,
	  0x3c0001e,0x1,0xf000fff8,0x1e0780,0x3fffe00,0x79fff000,0x1f0001,0xfffc00f0,0x7e0007,0xe000003f,0x3ff,0x801fffff,0xf003ff80,
	  0x3f00,0x3c03fff0,0xf01e003,0xffffc03c,0x1e0,0x3c03ff,0xffc01fff,0xfe03c000,0x1fffff,0x80007800,0x780,0x3df80000,0x7800001e,
	  0x1c70f078,0x781e03c0,0x780780,0x3c0f000,0x1e078007,0xc01f8000,0xf000,0xf0003c0,0x1e007807,0x83f83c00,0xfe00003,0xff000000,
	  0x7c00003c,0x780,0x1e0,0x0,0x0,0x0,0x7c01,0xf801f007,0xc00100f8,0x1f803c0,0x3c0003c,0x1f003,0xf007c00f,0x80003c00,0xf000,
	  0x783f000,0x1e0000f,0x3c0f01f,0x3e01f0,0x3e007e0,0x7c07c00,0xfc003f00,0xf0000,0x3c000,0x3c003c0,0x3c003c0f,0x1e01,0xf07c007c,
	  0x7c0000,0xfc0001e,0xf0,0x780,0x0,0x0,0x3ffff000,0x3838000,0x0,0x7800000,0x0,0x18,0xc0,0x0,0xff0000,0x3f00000,0x3,0xc00fff00,
	  0x38380000,0x7c0000e,0xe000070,0x70000001,0xe0003c00,0xf01e,0x780e00,0x0,0x0,0x0,0x0,0x1e0,0x0,0x780f8,0xf003838,0xfc,0xffc00,
	  0x0,0x780000,0x7c180000,0xf000000f,0xffe00fff,0xffc0003c,0x783f,0x80000000,0x6380000,0xc0f83f80,0xf81f00,0x0,0x303,0x80e06000,
	  0x0,0x78,0xf00,0x78,0x0,0x3c003c0,0x7ffe1c00,0x0,0x0,0x3800003c,0x3e000f81,0xf003ffe0,0x1fff00,0x1fc000,0xf000,0x1e03c00,
	  0xf01e000,0x780f0003,0xc078001e,0x3c000f0,0x1e000f07,0xff83c000,0x7ffff,0x803ffffc,0x1ffffe0,0xfffff00,0xf00000,0x7800000,
	  0x3c000001,0xe0001e00,0x3c0f0f0,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x3e07c01e,0x1e3c0f0,0x3c0780,0x1e03c00,
	  0xf01e000,0x78003ff0,0x1e0007,0x80f1e000,0xf80,0x7c00,0x3e000,0x1f0000,0xf80000,0x7c0001e,0x3c07c00,0x10078007,0x803c003c,
	  0x1e001e0,0xf000f00,0x780000,0x3c00000,0x1e000000,0xf00007c0,0x1e003e00,0x7c03e007,0xc01f003e,0xf801f0,0x7c00f80,0x3e007c00,
	  0xf,0x801f003c,0x3c01e0,0x1e00f00,0xf007800,0x7807c007,0xc01f801f,0x1f001f0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x0,0xe078003c,0x300001f0,0x3f801ff0,0x0,
	  0x3c00,0x1e00,0x3c1c1e0,0x1e0000,0x0,0x0,0x0,0xf0001e0f,0x3c0001e,0x3,0xe000fff0,0x3c0780,0x3ffff00,0x7bfff800,0x1e0000,0x7ff00078,
	  0x7e0007,0xe000003f,0x1ffc,0x1fffff,0xf0007ff0,0x7e00,0x3c07c3f0,0xf01e003,0xffff003c,0x1e0,0x3c03ff,0xffc01fff,0xfe03c000,
	  0x1fffff,0x80007800,0x780,0x3ffc0000,0x7800001e,0x1ef0f078,0x781e03c0,0x780780,0x7c0f000,0x1e07801f,0x800ff000,0xf000,0xf0003c0,
	  0xf00f807,0x83b83c00,0xfc00001,0xfe000000,0xf800003c,0x780,0x1e0,0x0,0x0,0x0,0x3c01,0xf000f007,0xc00000f0,0xf80780,0x3c0003c,
	  0x1e001,0xf007c007,0x80003c00,0xf000,0x787e000,0x1e0000f,0x3c0f01f,0x1e01e0,0x1e007c0,0x3c07800,0x7c003f00,0xf0000,0x3c000,
	  0x3c003c0,0x3e007c07,0x80003c00,0xf8f8003c,0x780000,0xf80001e,0xf0,0x780,0x0,0x0,0x7ffff000,0x601c000,0x3,0xffff0000,0x0,
	  0xfff,0xf8007fff,0xc0000000,0x7e003c,0x1fe0000,0xc0003,0xc00fff00,0x601c0000,0xf800018,0x70000c0,0x38000001,0xe0007800,0x701e,
	  0x701e00,0x0,0x0,0x0,0x0,0x1e0,0x6,0x700f8,0xf00601c,0xf8,0x7f800,0x0,0x780000,0xf8180000,0xf000000f,0x87c00fff,0xffc0003c,
	  0xf01f,0xc0000000,0x6380000,0xc07ff780,0x1f03e03,0xfffffe00,0x303,0x81c06000,0x0,0x1ffff,0xfe001e00,0x180f8,0x0,0x3c003c0,
	  0x3ffe1c00,0x3f00000,0x0,0x3800003f,0xfe0007c0,0xf8000000,0x18000000,0xc0000006,0x1f000,0x1e03c00,0xf01e000,0x780f0003,0xc078001e,
	  0x3c000f0,0x1e001f07,0xff83c000,0x7ffff,0x803ffffc,0x1ffffe0,0xfffff00,0xf00000,0x7800000,0x3c000001,0xe000fff8,0x3c0f0f0,
	  0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x1f0f801e,0x3c3c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78001fe0,0x1e0007,
	  0x80f1e000,0x780,0x3c00,0x1e000,0xf0000,0x780000,0x3c0001e,0x3c07c00,0xf0007,0x8078003c,0x3c001e0,0x1e000f00,0x780000,0x3c00000,
	  0x1e000000,0xf0000f80,0x1f003e00,0x3c03c003,0xc01e001e,0xf000f0,0x7800780,0x3c003c00,0xf,0x3f003c,0x3c01e0,0x1e00f00,0xf007800,
	  0x7803c007,0x801f000f,0xf001e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1,0xe078003f,0xb0000000,0xfc003cf0,0x0,0x3c00,0x1e00,0x101c040,0x1e0000,0x0,0x0,0x1,
	  0xe0001e1f,0x83c0001e,0x7,0xe000fff0,0x3c0780,0x3c03f80,0x7fc0fc00,0x1e0000,0xfff80078,0xfe0007,0xe000003f,0x7fe0,0x1fffff,
	  0xf0000ffc,0xfc00,0x780f81f0,0xf01e003,0xffff003c,0x1e0,0x3c03ff,0xffc01fff,0xfe03c000,0x1fffff,0x80007800,0x780,0x3ffc0000,
	  0x7800001e,0x1ef0f078,0x3c1e03c0,0x780780,0x1fc0f000,0x1e07ffff,0x7ff00,0xf000,0xf0003c0,0xf00f007,0xc3b87c00,0x7c00001,0xfe000000,
	  0xf800003c,0x3c0,0x1e0,0x0,0x0,0x0,0x3c01,0xf000f007,0x800000f0,0xf80780,0x1e0003c,0x1e001,0xf0078007,0x80003c00,0xf000,0x78fc000,
	  0x1e0000f,0x3c0f01e,0x1e01e0,0x1e007c0,0x3c07800,0x7c003e00,0xf0000,0x3c000,0x3c003c0,0x1e007807,0x80003c00,0x7df0003c,0x780000,
	  0x1f00001e,0xf0,0x780,0x0,0x0,0x7800000,0xe7ce000,0x3,0xffff0000,0x0,0xfff,0xf8007fff,0xc0000000,0x1f0,0xffe000,0x1c0003,
	  0xc00fff00,0xe7ce0000,0xf800039,0xf38001cf,0x9c000000,0xe0007800,0x780e,0x701c00,0x0,0x0,0x0,0x0,0x1e0,0x7,0xf0078,0xf00e7ce,
	  0x1f0,0x7f800,0x0,0x780000,0xf0180000,0xf000000e,0x1c0001f,0xe000003c,0xf007,0xe0000000,0x6380000,0xc03fe780,0x3e07c03,0xfffffe00,
	  0x303,0xffc06000,0x0,0x1ffff,0xfe003ffe,0x1fff0,0x0,0x3c003c0,0x1ffe1c00,0x3f00000,0x7,0xffc0001f,0xfc0003e0,0x7c000001,0xfc00000f,
	  0xe000007f,0x1e000,0x1e03c00,0xf01e000,0x780f0003,0xc078001e,0x3c000f0,0x1e001e07,0xff83c000,0x7ffff,0x803ffffc,0x1ffffe0,
	  0xfffff00,0xf00000,0x7800000,0x3c000001,0xe000fff8,0x3c0f078,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0xf9f001e,
	  0x783c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78001fe0,0x1e0007,0x80f1e000,0x780,0x3c00,0x1e000,0xf0000,0x780000,0x3c0001e,0x3c07800,
	  0xf0003,0xc078001e,0x3c000f0,0x1e000780,0x780000,0x3c00000,0x1e000000,0xf0000f00,0xf003c00,0x3c03c003,0xc01e001e,0xf000f0,
	  0x7800780,0x3c003c00,0xf,0x7f003c,0x3c01e0,0x1e00f00,0xf007800,0x7803c007,0x801f000f,0xf001e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1,0xe070001f,0xf8000007,
	  0xf0007cf8,0x7800000,0x3c00,0x1e00,0x1c000,0x1e0000,0x0,0x0,0x1,0xe0001e1f,0x83c0001e,0xf,0xc000fff8,0x780780,0x2000f80,0x7f803e00,
	  0x3e0003,0xfffe007c,0x1fe0000,0x0,0x3ff00,0x0,0x1ff,0x8001f000,0x780f00f0,0x1f00f003,0xffffc03c,0x1e0,0x3c03ff,0xffc01fff,
	  0xfe03c00f,0xf81fffff,0x80007800,0x780,0x3ffe0000,0x7800001e,0xee0f078,0x3c1e03c0,0x7807ff,0xff80f000,0x1e07fffe,0x3ffe0,
	  0xf000,0xf0003c0,0xf00f003,0xc7bc7800,0xfc00000,0xfc000001,0xf000003c,0x3c0,0x1e0,0x0,0x0,0x0,0x3c01,0xe000f80f,0x800001e0,
	  0xf80f00,0x1e0003c,0x3c000,0xf0078007,0x80003c00,0xf000,0x79f8000,0x1e0000f,0x3c0f01e,0x1e03c0,0x1f00780,0x3e0f000,0x7c003e00,
	  0xf0000,0x3c000,0x3c003c0,0x1e007807,0x81e03c00,0x7df0003e,0xf80000,0x3e00003e,0xf0,0x7c0,0xfc000,0x80000000,0x7800000,0x1e7cf000,
	  0x3,0xffff0000,0x0,0x18,0xc0,0x0,0xf80,0x7ffc00,0x380003,0xc00fff01,0xe7cf0000,0x1f000079,0xf3c003cf,0x9e000000,0xe0007000,
	  0x380e,0xe01c00,0x0,0x0,0x0,0x0,0x1e0,0x3,0x800f0078,0xf01e7cf,0x3e0,0x3f000,0x0,0x780000,0xf018001f,0xfff8001e,0x1e0000f,
	  0xc000003c,0xf003,0xe0000000,0x6380000,0xc00fc780,0x7c0f803,0xfffffe00,0x303,0xfe006000,0x0,0x1ffff,0xfe003ffe,0x1ffe0,0x0,
	  0x3c003c0,0xffe1c00,0x3f00000,0x7,0xffc00007,0xf00001f0,0x3e00001f,0xfc0000ff,0xe00007ff,0x3e000,0x3e01e00,0x1f00f000,0xf8078007,
	  0xc03c003e,0x1e001e0,0xf001e07,0xff83c000,0x7ffff,0x803ffffc,0x1ffffe0,0xfffff00,0xf00000,0x7800000,0x3c000001,0xe000fff8,
	  0x3c0f078,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x7fe001e,0xf03c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78000fc0,
	  0x1e0007,0x80f1f000,0x780,0x3c00,0x1e000,0xf0000,0x780000,0x3c0001e,0x3c0f800,0x1e0003,0xc0f0001e,0x78000f0,0x3c000780,0x780000,
	  0x3c00000,0x1e000000,0xf0000f00,0xf003c00,0x3c078003,0xe03c001f,0x1e000f8,0xf0007c0,0x78003e00,0x1e,0xf7803c,0x3c01e0,0x1e00f00,
	  0xf007800,0x7803e00f,0x801e000f,0x80f803e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1,0xe0f0000f,0xff00001f,0x8000f87c,0x7800000,0x3c00,0x1e00,0x1c000,0x7fffff80,
	  0x0,0x0,0x3,0xc0001e1f,0x83c0001e,0x1f,0x800000fe,0xf00780,0x7c0,0x7f001e00,0x3c0007,0xe03f003f,0x3fe0000,0x0,0x3fc00,0x0,
	  0x7f,0x8001e000,0x781f00f0,0x1e00f003,0xc007e03c,0x1e0,0x3c03c0,0x1e00,0x3c00f,0xf81e0007,0x80007800,0x780,0x3f9f0000,0x7800001e,
	  0xfe0f078,0x3c1e03c0,0x7807ff,0xff00f000,0x1e07fff8,0xfff8,0xf000,0xf0003c0,0xf81f003,0xc7bc7800,0xfe00000,0x78000003,0xe000003c,
	  0x1e0,0x1e0,0x0,0x0,0x0,0x1fffc01,0xe000780f,0x1e0,0x780f00,0x1e0003c,0x3c000,0xf0078007,0x80003c00,0xf000,0x7bf0000,0x1e0000f,
	  0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0xf8000,0x3c000,0x3c003c0,0x1f00f807,0x81f03c00,0x3fe0001e,0xf00000,0x7c00007c,
	  0xf0,0x3e0,0x3ff801,0x80000000,0x7800000,0x3cfcf800,0x3,0xffff0000,0x0,0x18,0xc0,0x0,0x7c00,0x1fff00,0x700003,0xc00f0003,
	  0xcfcf8000,0x3e0000f3,0xf3e0079f,0x9f000000,0xf000,0x1000,0x0,0x0,0x0,0x0,0x0,0x1f0,0x1,0xc00f0078,0xf03cfcf,0x800007c0,0x1e000,
	  0x0,0x780001,0xe018001f,0xfff8001c,0xe00007,0x8000003c,0xf001,0xf0000000,0x6380000,0xc0000000,0xf81f003,0xfffffe00,0x303,
	  0x87006000,0x0,0x1ffff,0xfe003ffe,0x7f00,0x0,0x3c003c0,0x3fe1c00,0x3f00000,0x7,0xffc00000,0xf8,0x1f0001ff,0xf0000fff,0x80007ffc,
	  0xfc000,0x3c01e00,0x1e00f000,0xf0078007,0x803c003c,0x1e001e0,0xf001e07,0x8003c000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,
	  0x7800000,0x3c000001,0xe000fff8,0x3c0f078,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x3fc001e,0x1e03c0f0,0x3c0780,
	  0x1e03c00,0xf01e000,0x78000780,0x1e0007,0x80f0fc00,0x3fff80,0x1fffc00,0xfffe000,0x7fff0003,0xfff8001f,0xffc0001e,0x3c0f000,
	  0x1e0003,0xc0f0001e,0x78000f0,0x3c000780,0x780000,0x3c00000,0x1e000000,0xf0001e00,0xf803c00,0x3c078001,0xe03c000f,0x1e00078,
	  0xf0003c0,0x78001e07,0xfffffe1e,0x1e7803c,0x3c01e0,0x1e00f00,0xf007800,0x7801e00f,0x1e0007,0x807803c0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x3,0xc0f00007,
	  0xffc0007e,0xf03e,0x7800000,0x3c00,0x1e00,0x1c000,0x7fffff80,0x0,0x0,0x3,0xc0001e1f,0x83c0001e,0x3f,0x3e,0xf00780,0x3c0,0x7e001e00,
	  0x7c000f,0x800f001f,0xffde0000,0x0,0x3e000,0x0,0xf,0x8003e000,0x781e0070,0x1e00f003,0xc001f03c,0x1e0,0x3c03c0,0x1e00,0x3c00f,
	  0xf81e0007,0x80007800,0x780,0x3f1f0000,0x7800001e,0x7c0f078,0x1e1e03c0,0x7807ff,0xfc00f000,0x1e07fffe,0xffc,0xf000,0xf0003c0,
	  0x781e003,0xc71c7800,0x1ff00000,0x78000003,0xe000003c,0x1e0,0x1e0,0x0,0x0,0x0,0xffffc01,0xe000780f,0x1e0,0x780fff,0xffe0003c,
	  0x3c000,0xf0078007,0x80003c00,0xf000,0x7ff0000,0x1e0000f,0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0x7f000,0x3c000,
	  0x3c003c0,0xf00f007,0xc1f07c00,0x1fc0001f,0x1f00000,0xfc000ff8,0xf0,0x1ff,0xfffe07,0x80000000,0x7800000,0x7ffcfc00,0x0,0xf000000,
	  0x0,0x18,0xc0,0x0,0x3e000,0x1ff80,0xe00003,0xc00f0007,0xffcfc000,0x3e0001ff,0xf3f00fff,0x9f800000,0x6000,0x0,0x0,0x7c000,
	  0x0,0x0,0x0,0xfe,0x0,0xe00f007f,0xff07ffcf,0xc0000fc0,0x1e000,0x0,0x780001,0xe018001f,0xfff8001c,0xe00007,0x80000000,0xf800,
	  0xf0000000,0x6380000,0xc0000000,0x1f03c000,0x1e00,0x303,0x83806000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xfe1c00,0x3f00000,0x0,
	  0x0,0x3c,0xf801fff,0xfff8,0x7ffc0,0x1f8000,0x3c01e00,0x1e00f000,0xf0078007,0x803c003c,0x1e001e0,0xf003c07,0x8003c000,0x78000,
	  0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x3c0f03c,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,
	  0x78000f00,0x1f8001e,0x1e03c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1e000f,0x80f0ff00,0x1ffff80,0xffffc00,0x7fffe003,
	  0xffff001f,0xfff800ff,0xffc007ff,0xffc0f000,0x1fffff,0xc0fffffe,0x7fffff0,0x3fffff80,0x780000,0x3c00000,0x1e000000,0xf0001e00,
	  0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e07,0xfffffe1e,0x3c7803c,0x3c01e0,0x1e00f00,0xf007800,0x7801f01f,
	  0x1e0007,0x807c07c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x780000,0x3,0xc0f00000,0xfff003f0,0x1f00f03e,0x7800000,0x3c00,0x1e00,0x1c000,0x7fffff80,0x0,0x7ff80000,0x3,
	  0xc0001e0f,0x3c0001e,0x7e,0x1f,0x1e00780,0x3e0,0x7e000f00,0x78000f,0x7800f,0xff9e0000,0x0,0x3fc00,0x0,0x7f,0x8003c000,0x781e0070,
	  0x3e00f803,0xc000f03c,0x1e0,0x3c03c0,0x1e00,0x3c00f,0xf81e0007,0x80007800,0x780,0x3e0f8000,0x7800001e,0x7c0f078,0x1e1e03c0,
	  0x7807ff,0xf000f000,0x1e07807f,0xfe,0xf000,0xf0003c0,0x781e003,0xc71c7800,0x3ef00000,0x78000007,0xc000003c,0x1e0,0x1e0,0x0,
	  0x0,0x0,0x1ffffc01,0xe000780f,0x1e0,0x780fff,0xffe0003c,0x3c000,0xf0078007,0x80003c00,0xf000,0x7ff0000,0x1e0000f,0x3c0f01e,
	  0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0x7ff80,0x3c000,0x3c003c0,0xf00f003,0xc1f07800,0x1fc0000f,0x1e00000,0xf8000ff0,0xf0,
	  0xff,0xffffff,0x80000000,0x3fffc000,0xfff9fe00,0x0,0xf000000,0x0,0x18,0xc0,0x0,0x1f0000,0x1fc0,0x1c00003,0xc00f000f,0xff9fe000,
	  0x7c0003ff,0xe7f81fff,0x3fc00000,0x0,0x0,0x0,0xfe000,0x1ffffc0f,0xfffffc00,0x0,0xff,0xf0000000,0x700f007f,0xff0fff9f,0xe0000f80,
	  0x1e000,0x0,0x780001,0xe018001f,0xfff8001c,0xe00fff,0xffc00000,0xf800,0xf0000000,0x6380000,0xc0ffff80,0x3e078000,0x1e00,0x7ff80303,
	  0x83c06000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x3f00000,0x0,0x7f,0xff00001e,0x7c1fff0,0xfff80,0x7ffc00,0x3f0000,0x7c01f00,
	  0x3e00f801,0xf007c00f,0x803e007c,0x1f003e0,0xf803c07,0x8003c000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,
	  0xe0001e00,0x3c0f03c,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x1f8001e,0x3c03c0f0,0x3c0780,0x1e03c00,0xf01e000,
	  0x78000780,0x1e001f,0xf07f80,0x3ffff80,0x1ffffc00,0xffffe007,0xffff003f,0xfff801ff,0xffc03fff,0xffc0f000,0x1fffff,0xc0fffffe,
	  0x7fffff0,0x3fffff80,0x780000,0x3c00000,0x1e000000,0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e07,
	  0xfffffe1e,0x787803c,0x3c01e0,0x1e00f00,0xf007800,0x7800f01e,0x1e0007,0x803c0780,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1ff,0xffff8000,0x3ff80fc0,0x7fc1e01f,
	  0x7800000,0x3c00,0x1e00,0x0,0x7fffff80,0x0,0x7ff80000,0x7,0x80001e00,0x3c0001e,0xfc,0xf,0x1e00780,0x1e0,0x7c000f00,0x78000f,
	  0x78007,0xff1e0000,0x0,0x3ff00,0x0,0x1ff,0x8003c000,0x781e0070,0x3c007803,0xc000f03c,0x1e0,0x3c03c0,0x1e00,0x3c000,0x781e0007,
	  0x80007800,0x780,0x3c07c000,0x7800001e,0x7c0f078,0xf1e03c0,0x780780,0xf000,0x1e07801f,0x3e,0xf000,0xf0003c0,0x781e003,0xcf1c7800,
	  0x3cf80000,0x7800000f,0x8000003c,0xf0,0x1e0,0x0,0x0,0x0,0x3ffffc01,0xe000780f,0x1e0,0x780fff,0xffe0003c,0x3c000,0xf0078007,
	  0x80003c00,0xf000,0x7ff8000,0x1e0000f,0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0x3fff0,0x3c000,0x3c003c0,0xf81f003,
	  0xc3b87800,0xf80000f,0x1e00001,0xf0000ff0,0xf0,0xff,0xf03fff,0x80000000,0x3fff8001,0xfff1ff00,0x0,0xf000000,0x0,0x18,0xc0,
	  0x0,0x380000,0x7c0,0x3c00003,0xc00f001f,0xff1ff000,0xf80007ff,0xc7fc3ffe,0x3fe00000,0x0,0x0,0x0,0x1ff000,0x7ffffe1f,0xffffff00,
	  0x0,0x7f,0xfe000000,0x780f007f,0xff1fff1f,0xf0001f00,0x1e000,0x0,0x780001,0xe0180000,0xf000001c,0xe00fff,0xffc00000,0x7c00,
	  0xf0000000,0x31c0001,0x80ffff80,0x3e078000,0x1e00,0x7ff80183,0x81c0c000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x3f00000,
	  0x0,0x7f,0xff00001e,0x7c7ff03,0xc03ff8fe,0x1ffc0f0,0x7e0000,0x7800f00,0x3c007801,0xe003c00f,0x1e0078,0xf003c0,0x7803c07,0x8003c000,
	  0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x3c0f01e,0x3c078000,0xf03c0007,0x81e0003c,
	  0xf0001e0,0x78000f00,0x3fc001e,0x7803c0f0,0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1e007f,0xf03fe0,0x7ffff80,0x3ffffc01,
	  0xffffe00f,0xffff007f,0xfff803ff,0xffc07fff,0xffc0f000,0x1fffff,0xc0fffffe,0x7fffff0,0x3fffff80,0x780000,0x3c00000,0x1e000000,
	  0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e07,0xfffffe1e,0x707803c,0x3c01e0,0x1e00f00,0xf007800,
	  0x7800f01e,0x1e0007,0x803c0780,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1ff,0xffff8000,0x30f81f00,0xffe1e00f,0x87800000,0x3c00,0x1e00,0x0,0x1e0000,0x0,0x7ff80000,
	  0x7,0x80001e00,0x3c0001e,0x1f8,0x7,0x83c00780,0x1e0,0x7c000f00,0xf8001e,0x3c001,0xfc1e0000,0x0,0x7fe0,0x0,0xffc,0x3c000,0x781e0070,
	  0x3ffff803,0xc000783c,0x1e0,0x3c03c0,0x1e00,0x3c000,0x781e0007,0x80007800,0x780,0x3c07c000,0x7800001e,0x380f078,0xf1e03c0,
	  0x780780,0xf000,0x1e07800f,0x8000001e,0xf000,0xf0003c0,0x3c3c003,0xcf1e7800,0x7c780000,0x7800000f,0x8000003c,0xf0,0x1e0,0x0,
	  0x0,0x0,0x7f003c01,0xe000780f,0x1e0,0x780fff,0xffe0003c,0x3c000,0xf0078007,0x80003c00,0xf000,0x7f7c000,0x1e0000f,0x3c0f01e,
	  0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0xfff8,0x3c000,0x3c003c0,0x781e003,0xc3b87800,0x1fc00007,0x83e00003,0xe0000ff8,0xf0,
	  0x1ff,0xc007fe,0x0,0x7fff8001,0xffe3ff00,0x0,0x1e000000,0x0,0x18,0xc0,0x0,0x0,0x3c0,0x7800003,0xc00f001f,0xfe3ff000,0xf80007ff,
	  0x8ffc3ffc,0x7fe00000,0x0,0x0,0x0,0x1ff000,0x0,0x0,0x0,0x1f,0xff000000,0x3c0f007f,0xff1ffe3f,0xf0003e00,0x1e000,0x0,0x780001,
	  0xe0180000,0xf000001e,0x1e00fff,0xffc00000,0x3f00,0xf0000000,0x31c0001,0x80ffff80,0x1f03c000,0x1e00,0x7ff80183,0x81c0c000,
	  0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x0,0x0,0x7f,0xff00003c,0xf87f007,0xc03f83ff,0x81fc01f0,0x7c0000,0x7ffff00,0x3ffff801,
	  0xffffc00f,0xfffe007f,0xfff003ff,0xff807fff,0x8003c000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,
	  0xe0001e00,0x3c0f01e,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0x7fe001e,0xf003c0f0,0x3c0780,0x1e03c00,0xf01e000,
	  0x78000780,0x1ffffe,0xf00ff0,0xfe00780,0x7f003c03,0xf801e01f,0xc00f00fe,0x7807f0,0x3c0ffff,0xffc0f000,0x1fffff,0xc0fffffe,
	  0x7fffff0,0x3fffff80,0x780000,0x3c00000,0x1e000000,0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e00,
	  0x1e,0xf07803c,0x3c01e0,0x1e00f00,0xf007800,0x7800783e,0x1e0007,0x801e0f80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1ff,0xffff8000,0x307c0801,0xe1f1e00f,0x87000000,
	  0x3c00,0x1e00,0x0,0x1e0000,0x0,0x7ff80000,0xf,0x1e00,0x3c0001e,0x3f0,0x7,0x83fffffc,0x1e0,0x7c000f00,0xf0001e,0x3c000,0x3e0000,
	  0x0,0x1ffc,0x1fffff,0xf0007ff0,0x3c000,0x781e0070,0x7ffffc03,0xc000781e,0x1e0,0x7803c0,0x1e00,0x3c000,0x781e0007,0x80007800,
	  0x780,0x3c03e000,0x7800001e,0xf078,0x79e03c0,0x780780,0xf000,0x1e078007,0x8000000f,0xf000,0xf0003c0,0x3c3c001,0xee0ef000,
	  0xf87c0000,0x7800001f,0x3c,0x78,0x1e0,0x0,0x0,0x0,0x7c003c01,0xe000780f,0x1e0,0x780f00,0x3c,0x3c000,0xf0078007,0x80003c00,
	  0xf000,0x7e3e000,0x1e0000f,0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0x1ffc,0x3c000,0x3c003c0,0x781e003,0xe3b8f800,
	  0x1fc00007,0x83c00007,0xc00000fc,0xf0,0x3e0,0x8001f8,0x0,0x7800000,0xffc7fe00,0x0,0x1e000000,0x0,0x18,0xc0,0x0,0x0,0x1e0,
	  0xf000003,0xc00f000f,0xfc7fe001,0xf00003ff,0x1ff81ff8,0xffc00000,0x0,0x0,0x0,0x1ff000,0x0,0x0,0x0,0x3,0xff800000,0x1e0f0078,
	  0xffc7f,0xe0007c00,0x1e000,0x0,0x780001,0xe0180000,0xf000000e,0x1c00007,0x80000000,0x1f81,0xe0000000,0x38e0003,0x80000000,
	  0xf81f000,0x1e00,0x7ff801c3,0x80e1c000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x0,0x0,0x0,0xf8,0x1f070007,0xc03803ff,0xc1c001f0,
	  0xf80000,0xfffff00,0x7ffff803,0xffffc01f,0xfffe00ff,0xfff007ff,0xffc07fff,0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,
	  0xf00000,0x7800000,0x3c000001,0xe0001e00,0x780f00f,0x3c078000,0xf03c0007,0x81e0003c,0xf0001e0,0x78000f00,0xf9f001e,0xf003c0f0,
	  0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1ffffc,0xf003f8,0xf800780,0x7c003c03,0xe001e01f,0xf00f8,0x7807c0,0x3c0fc1e,0xf000,
	  0x1e0000,0xf00000,0x7800000,0x3c000000,0x780000,0x3c00000,0x1e000000,0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,
	  0xf0003c0,0x78001e00,0x1e,0x1e07803c,0x3c01e0,0x1e00f00,0xf007800,0x7800783c,0x1e0007,0x801e0f00,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ff,0xffff8000,0x303c0001,
	  0xc071e007,0xcf000000,0x3c00,0x1e00,0x0,0x1e0000,0x0,0x0,0xf,0xf00,0x780001e,0x7e0,0x7,0x83fffffc,0x1e0,0x7c000f00,0x1f0001e,
	  0x3c000,0x3c0000,0x0,0x3ff,0x801fffff,0xf003ff80,0x3c000,0x781e0070,0x7ffffc03,0xc000781e,0x1e0,0x7803c0,0x1e00,0x1e000,0x781e0007,
	  0x80007800,0x780,0x3c01f000,0x7800001e,0xf078,0x79e03c0,0xf00780,0xf000,0x3e078007,0xc000000f,0xf000,0xf0003c0,0x3c3c001,
	  0xee0ef000,0xf03e0000,0x7800003e,0x3c,0x78,0x1e0,0x0,0x0,0x0,0xf8003c01,0xe000780f,0x1e0,0x780f00,0x3c,0x3c000,0xf0078007,
	  0x80003c00,0xf000,0x7c3e000,0x1e0000f,0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0xfc,0x3c000,0x3c003c0,0x3c3e001,0xe7b8f000,
	  0x3fe00007,0xc7c0000f,0xc000003e,0xf0,0x7c0,0x0,0x0,0x7c00000,0x7fcffc00,0x0,0x1e000000,0x0,0x18,0xc0,0x0,0x0,0x1e0,0x1e000003,
	  0xc00f0007,0xfcffc003,0xe00001ff,0x3ff00ff9,0xff800000,0x0,0x0,0x0,0x1ff000,0x0,0x0,0x0,0x0,0x1f800000,0xf0f0078,0x7fcff,
	  0xc000fc00,0x1e000,0x0,0x780001,0xe0180000,0xf000000f,0x87c00007,0x80000000,0xfe3,0xe0000000,0x18780c3,0x0,0x7c0f800,0x1e00,
	  0xc3,0x80e18000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x0,0x0,0x0,0x1f0,0x3e00000f,0xc0000303,0xe00003f0,0xf00000,0xfffff80,
	  0x7ffffc03,0xffffe01f,0xffff00ff,0xfff807ff,0xffc07fff,0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,
	  0x3c000001,0xe0001e00,0x780f00f,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e00,0x1f0f801f,0xe00780f0,0x3c0780,0x1e03c00,
	  0xf01e000,0x78000780,0x1ffff8,0xf000f8,0x1f000780,0xf8003c07,0xc001e03e,0xf01f0,0x780f80,0x3c1f01e,0xf000,0x1e0000,0xf00000,
	  0x7800000,0x3c000000,0x780000,0x3c00000,0x1e000000,0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e00,
	  0x1e,0x3c07803c,0x3c01e0,0x1e00f00,0xf007800,0x78007c7c,0x1e0007,0x801f1f00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x81c00000,0x303c0003,0x8039e003,0xef000000,
	  0x3c00,0x1e00,0x0,0x1e0000,0x0,0x0,0x1e,0xf00,0x780001e,0xfc0,0x7,0x83fffffc,0x1e0,0x3c000f00,0x1e0001e,0x3c000,0x3c0000,
	  0x0,0x7f,0xe01fffff,0xf00ffc00,0x3c000,0x781f00f0,0x7ffffc03,0xc000781e,0x1e0,0x7803c0,0x1e00,0x1e000,0x781e0007,0x80007800,
	  0x780,0x3c01f000,0x7800001e,0xf078,0x7de01e0,0xf00780,0x7800,0x3c078003,0xc000000f,0xf000,0xf0003c0,0x3e7c001,0xee0ef001,
	  0xf01e0000,0x7800003e,0x3c,0x3c,0x1e0,0x0,0x0,0x0,0xf0003c01,0xe000780f,0x1e0,0x780f00,0x3c,0x3c000,0xf0078007,0x80003c00,
	  0xf000,0x781f000,0x1e0000f,0x3c0f01e,0x1e03c0,0xf00780,0x1e0f000,0x3c003c00,0x3e,0x3c000,0x3c003c0,0x3c3c001,0xe71cf000,0x7df00003,
	  0xc780000f,0x8000003e,0xf0,0x780,0x0,0x0,0x3c00000,0x3fcff800,0x0,0x1e000000,0x0,0x18,0xc0,0x0,0x1f00fc,0x1e0,0x1e000001,
	  0xe00f0003,0xfcff8003,0xe00000ff,0x3fe007f9,0xff000000,0x0,0x0,0x0,0x1ff000,0x0,0x0,0x0,0x0,0x7c00000,0xf0f0078,0x3fcff,0x8000f800,
	  0x1e000,0x0,0x780001,0xe0180000,0xf000001f,0xffe00007,0x8000003c,0x7ff,0xc0000000,0x1c3ffc7,0x0,0x3e07c00,0x1e00,0xe3,0x80738000,
	  0x0,0x78,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x0,0x0,0x0,0x3e0,0x7c00001d,0xc0000001,0xe0000770,0x1f00000,0xfffff80,0x7ffffc03,
	  0xffffe01f,0xffff00ff,0xfff807ff,0xffc07fff,0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,
	  0xe0001e00,0x780f00f,0x3c03c001,0xe01e000f,0xf00078,0x78003c0,0x3c001e00,0x3e07c01f,0xc00780f0,0x3c0780,0x1e03c00,0xf01e000,
	  0x78000780,0x1fffc0,0xf0007c,0x1e000780,0xf0003c07,0x8001e03c,0xf01e0,0x780f00,0x3c1e01e,0xf000,0x1e0000,0xf00000,0x7800000,
	  0x3c000000,0x780000,0x3c00000,0x1e000000,0xf0001e00,0x7803c00,0x3c078001,0xe03c000f,0x1e00078,0xf0003c0,0x78001e00,0x1e,0x7807803c,
	  0x3c01e0,0x1e00f00,0xf007800,0x78003c78,0x1e0007,0x800f1e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x83c00000,0x303c0003,0x8039e001,0xee000000,0x1e00,0x3c00,
	  0x0,0x1e0000,0x0,0x0,0x1e,0xf00,0x780001e,0x1f80,0x7,0x83fffffc,0x1e0,0x3c000f00,0x1e0001e,0x3c000,0x3c0000,0x0,0x1f,0xfc1fffff,
	  0xf07ff000,0x0,0x780f00f0,0x78003c03,0xc000781e,0x1e0,0xf803c0,0x1e00,0x1e000,0x781e0007,0x80007800,0x780,0x3c00f800,0x7800001e,
	  0xf078,0x3de01e0,0xf00780,0x7800,0x3c078003,0xe000000f,0xf000,0xf0003c0,0x1e78001,0xfe0ff003,0xe01f0000,0x7800007c,0x3c,0x3c,
	  0x1e0,0x0,0x0,0x0,0xf0007c01,0xe000f80f,0x800001e0,0xf80f00,0x3c,0x1e001,0xf0078007,0x80003c00,0xf000,0x780f800,0x1e0000f,
	  0x3c0f01e,0x1e03c0,0x1f00780,0x3e0f000,0x7c003c00,0x1e,0x3c000,0x3c003c0,0x3c3c001,0xe71cf000,0xf8f80003,0xe780001f,0x1e,
	  0xf0,0x780,0x0,0x0,0x3c00000,0x1ffff000,0x0,0x1e000000,0x0,0x18,0xc0,0x0,0x3bc1de,0x1e0,0xf000001,0xe00f0001,0xffff0007,0xc000007f,
	  0xffc003ff,0xfe000000,0x0,0x0,0x0,0xfe000,0x0,0x0,0x0,0x0,0x3c00000,0x1e0f0078,0x1ffff,0x1f000,0x1e000,0x0,0x780000,0xf0180000,
	  0xf000001f,0xfff00007,0x8000003c,0x1ff,0x80000000,0xe0ff0e,0x0,0x1f03e00,0x1e00,0x70,0x70000,0x0,0x78,0x0,0x0,0x0,0x3c003c0,
	  0xe1c00,0x0,0x0,0x0,0x7c0,0xf8000019,0xc0000000,0xe0000670,0x1e00000,0xf000780,0x78003c03,0xc001e01e,0xf00f0,0x780780,0x3c0f807,
	  0x8001e000,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0xf80f007,0xbc03c001,0xe01e000f,
	  0xf00078,0x78003c0,0x3c001e00,0x7c03e00f,0x800780f0,0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1e0000,0xf0003c,0x1e000f80,
	  0xf0007c07,0x8003e03c,0x1f01e0,0xf80f00,0x7c1e01e,0xf800,0x1e0000,0xf00000,0x7800000,0x3c000000,0x780000,0x3c00000,0x1e000000,
	  0xf0001e00,0x7803c00,0x3c078003,0xe03c001f,0x1e000f8,0xf0007c0,0x78003e00,0x1f8001f,0xf00f803c,0x3c01e0,0x1e00f00,0xf007800,
	  0x78003e78,0x1e000f,0x800f9e00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf,0x3c00000,0x303c0003,0x8039f001,0xfe000000,0x1e00,0x3c00,0x0,0x1e0000,0x0,0x0,0x3c,0xf00,
	  0x780001e,0x3f00,0x7,0x80000780,0x3e0,0x3e000f00,0x3c0001e,0x3c000,0x7c0000,0x0,0x3,0xfe000000,0xff8000,0x0,0x3c0f81f0,0xf0001e03,
	  0xc000780f,0x1e0,0xf003c0,0x1e00,0xf000,0x781e0007,0x80007800,0x780,0x3c007c00,0x7800001e,0xf078,0x3de01e0,0xf00780,0x7800,
	  0x3c078001,0xe000000f,0xf000,0xf0003c0,0x1e78001,0xfc07f003,0xe00f0000,0x78000078,0x3c,0x1e,0x1e0,0x0,0x0,0x0,0xf0007c01,
	  0xf000f007,0x800000f0,0xf80780,0x3c,0x1e001,0xf0078007,0x80003c00,0xf000,0x7807c00,0x1e0000f,0x3c0f01e,0x1e01e0,0x1e007c0,
	  0x3c07800,0x7c003c00,0x1e,0x3c000,0x3c007c0,0x1e78001,0xe71df000,0xf8f80001,0xef80003e,0x1e,0xf0,0x780,0x0,0x0,0x3c00000,
	  0xfffe000,0x0,0x3e000000,0x0,0x18,0x7fff,0xc0000000,0x60c306,0x1e0,0x7800001,0xe00f0000,0xfffe0007,0x8000003f,0xff8001ff,
	  0xfc000000,0x0,0x0,0x0,0x7c000,0x0,0x0,0x0,0x0,0x3c00000,0x3c0f0078,0xfffe,0x3e000,0x1e000,0x0,0x780000,0xf0180000,0xf000003c,
	  0xfcf80007,0x8000003c,0x7f,0x0,0x70001c,0x0,0xf81f00,0x0,0x38,0xe0000,0x0,0x0,0x0,0x0,0x0,0x3c003c0,0xe1c00,0x0,0x0,0x0,0xf81,
	  0xf0000039,0xc0000000,0xe0000e70,0x1e00000,0x1e0003c0,0xf0001e07,0x8000f03c,0x781e0,0x3c0f00,0x1e0f007,0x8000f000,0x78000,
	  0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0xf00f007,0xbc03c001,0xe01e000f,0xf00078,0x78003c0,
	  0x3c001e00,0xf801f00f,0x800780f0,0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1e0000,0xf0003c,0x1e000f80,0xf0007c07,0x8003e03c,
	  0x1f01e0,0xf80f00,0x7c1e01e,0x7800,0xf0000,0x780000,0x3c00000,0x1e000000,0x780000,0x3c00000,0x1e000000,0xf0000f00,0xf003c00,
	  0x3c03c003,0xc01e001e,0xf000f0,0x7800780,0x3c003c00,0x1f8000f,0xe00f003c,0x7c01e0,0x3e00f00,0x1f007800,0xf8001ef8,0x1f000f,
	  0x7be00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0xf,0x3c00000,0x307c0003,0x8038f000,0xfc000000,0x1e00,0x3c00,0x0,0x1e0000,0xfc0000,0x0,0x7e00003c,0x780,0xf00001e,
	  0x7e00,0xf,0x80000780,0x3c0,0x3e001e00,0x3c0001f,0x7c000,0x780007,0xe000003f,0x0,0xfe000000,0xfe0000,0x0,0x3c07c3f0,0xf0001e03,
	  0xc000f80f,0x800001e0,0x1f003c0,0x1e00,0xf000,0x781e0007,0x80007800,0x4000f80,0x3c003c00,0x7800001e,0xf078,0x1fe01f0,0x1f00780,
	  0x7c00,0x7c078001,0xf000001f,0xf000,0xf0003c0,0x1e78001,0xfc07f007,0xc00f8000,0x780000f8,0x3c,0x1e,0x1e0,0x0,0x0,0x0,0xf0007c01,
	  0xf000f007,0xc00000f0,0xf80780,0x3c,0x1f003,0xf0078007,0x80003c00,0xf000,0x7807c00,0x1e0000f,0x3c0f01e,0x1e01e0,0x1e007c0,
	  0x3c07800,0x7c003c00,0x1e,0x3c000,0x3c007c0,0x1e78000,0xfe0fe001,0xf07c0001,0xef00007c,0x1e,0xf0,0x780,0x0,0x0,0x1e00000,
	  0x7cfc000,0xfc00000,0x3c00000f,0xc3f00000,0x18,0x7fff,0xc0000000,0x406303,0x3e0,0x3c00001,0xf00f0000,0x7cfc000f,0x8000001f,
	  0x3f0000f9,0xf8000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x780700f8,0x7cfc,0x7c000,0x1e000,0x0,0x780000,0xf8180000,
	  0xf0000070,0x3c0007,0x8000003c,0x3f,0x80000000,0x3c0078,0x0,0x780f00,0x0,0x1e,0x3c0000,0x0,0x0,0x0,0x0,0x0,0x3e007c0,0xe1c00,
	  0x0,0x0,0x0,0xf01,0xe0000071,0xc0000000,0xe0001c70,0x1e00000,0x1e0003c0,0xf0001e07,0x8000f03c,0x781e0,0x3c0f00,0x1e0f007,
	  0x8000f800,0x78000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x1f00f003,0xfc03e003,0xe01f001f,
	  0xf800f8,0x7c007c0,0x3e003e01,0xf000f80f,0xf00f0,0x3c0780,0x1e03c00,0xf01e000,0x78000780,0x1e0000,0xf0003c,0x1e000f80,0xf0007c07,
	  0x8003e03c,0x1f01e0,0xf80f00,0x7c1e01e,0x7c00,0xf0000,0x780000,0x3c00000,0x1e000000,0x780000,0x3c00000,0x1e000000,0xf0000f00,
	  0xf003c00,0x3c03c003,0xc01e001e,0xf000f0,0x7800780,0x3c003c00,0x1f8000f,0xc00f003c,0x7c01e0,0x3e00f00,0x1f007800,0xf8001ef0,
	  0x1f000f,0x7bc00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x780000,0xf,0x3800040,0x30780003,0x8038f800,0x78000000,0x1e00,0x3c00,0x0,0x1e0000,0xfc0000,0x0,0x7e000078,
	  0x780,0x1f00001e,0xfc00,0x20001f,0x780,0x80007c0,0x1f001e00,0x7c0000f,0x78000,0xf80007,0xe000003f,0x0,0x1e000000,0xf00000,
	  0x3c000,0x3c03fff0,0xf0001e03,0xc001f007,0x800101e0,0x7e003c0,0x1e00,0x7800,0x781e0007,0x80007800,0x6000f00,0x3c003e00,0x7800001e,
	  0xf078,0x1fe00f0,0x1e00780,0x3c00,0x78078000,0xf020001e,0xf000,0x7800780,0xff0001,0xfc07f00f,0x8007c000,0x780001f0,0x3c,0xf,
	  0x1e0,0x0,0x0,0x0,0xf800fc01,0xf801f007,0xc00100f8,0x1f807c0,0x40003c,0xf807,0xf0078007,0x80003c00,0xf000,0x7803e00,0x1f0000f,
	  0x3c0f01e,0x1e01f0,0x3e007e0,0x7c07c00,0xfc003c00,0x1e,0x3e000,0x3e007c0,0x1ff8000,0xfe0fe003,0xe03e0001,0xff0000fc,0x1e,
	  0xf0,0x780,0x0,0x0,0x1f00080,0x3cf8000,0xfc00000,0x3c00001f,0x83f00000,0x18,0xc0,0x0,0xc06203,0x40003c0,0x1c00000,0xf80f0000,
	  0x3cf8001f,0xf,0x3e000079,0xf0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x700780fc,0x3cf8,0xfc000,0x1e000,0x0,0x780000,
	  0x7c180000,0xf0000020,0x100007,0x8000003c,0xf,0x80000000,0x1f01f0,0x0,0x380700,0x0,0xf,0x80f80000,0x0,0x0,0x0,0x0,0x0,0x3e007c0,
	  0xe1c00,0x0,0x0,0x0,0xe01,0xc0000071,0xc0000001,0xc0001c70,0x1e00040,0x1e0003c0,0xf0001e07,0x8000f03c,0x781e0,0x3c0f00,0x1e0f007,
	  0x80007800,0x10078000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e00,0x7e00f003,0xfc01e003,0xc00f001e,
	  0x7800f0,0x3c00780,0x1e003c00,0xe000700f,0x800f0078,0x7803c0,0x3c01e00,0x1e00f000,0xf0000780,0x1e0000,0xf0003c,0x1f001f80,
	  0xf800fc07,0xc007e03e,0x3f01f0,0x1f80f80,0xfc1e01f,0x7c00,0x100f8000,0x807c0004,0x3e00020,0x1f000100,0x780000,0x3c00000,0x1e000000,
	  0xf0000f80,0x1f003c00,0x3c03e007,0xc01f003e,0xf801f0,0x7c00f80,0x3e007c00,0x1f8000f,0x801f003e,0x7c01f0,0x3e00f80,0x1f007c00,
	  0xf8001ff0,0x1f801f,0x7fc00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0xf,0x7800078,0x31f80001,0xc070fc00,0xfc000000,0x1e00,0x7c00,0x0,0x1e0000,0xfc0000,0x0,0x7e000078,
	  0x7c0,0x1f00001e,0x1f000,0x38003f,0x780,0xe000f80,0x1f803e00,0x780000f,0x800f8000,0x1f00007,0xe000003f,0x0,0x2000000,0x800000,
	  0x3c000,0x3e01ff71,0xf0001f03,0xc007f007,0xc00301e0,0x1fc003c0,0x1e00,0x7c00,0x781e0007,0x80007800,0x7801f00,0x3c001f00,0x7800001e,
	  0xf078,0xfe00f8,0x3e00780,0x3e00,0xf8078000,0xf838003e,0xf000,0x7c00f80,0xff0000,0xfc07e00f,0x8003c000,0x780001e0,0x3c,0xf,
	  0x1e0,0x0,0x0,0x0,0xf801fc01,0xfc03e003,0xe003007c,0x3f803e0,0x1c0003c,0xfc0f,0xf0078007,0x80003c00,0xf000,0x7801f00,0xf8000f,
	  0x3c0f01e,0x1e00f8,0x7c007f0,0xf803e01,0xfc003c00,0x8003e,0x1f000,0x1e00fc0,0xff0000,0xfe0fe007,0xc01f0000,0xfe0000f8,0x1e,
	  0xf0,0x780,0x0,0x0,0xf80180,0x1cf0000,0x1f800000,0x3c00001f,0x83e00000,0x18,0xc0,0x0,0xc06203,0x70007c0,0xe00000,0x7e0f0000,
	  0x1cf0001e,0x7,0x3c000039,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x100,0x7c00000,0xe00780fc,0x2001cf0,0xf8000,0x1e000,0x0,
	  0x780000,0x7e182000,0xf0000000,0x7,0x8000003c,0x7,0xc0000000,0x7ffc0,0x0,0x180300,0x0,0x3,0xffe00000,0x0,0x0,0x0,0x0,0x0,
	  0x3f00fc0,0xe1c00,0x0,0x0,0x0,0xc01,0x800000e1,0xc0000003,0xc0003870,0x1f001c0,0x3e0003e1,0xf0001f0f,0x8000f87c,0x7c3e0,0x3e1f00,
	  0x1f1e007,0x80007c00,0x30078000,0x3c0000,0x1e00000,0xf000000,0xf00000,0x7800000,0x3c000001,0xe0001e03,0xfc00f001,0xfc01f007,
	  0xc00f803e,0x7c01f0,0x3e00f80,0x1f007c00,0x4000201f,0xc01f007c,0xf803e0,0x7c01f00,0x3e00f801,0xf0000780,0x1e0000,0xf0007c,
	  0x1f003f80,0xf801fc07,0xc00fe03e,0x7f01f0,0x3f80f80,0x1fc1f03f,0x803e00,0x3007c003,0x803e001c,0x1f000e0,0xf800700,0x780000,
	  0x3c00000,0x1e000000,0xf00007c0,0x3e003c00,0x3c01f00f,0x800f807c,0x7c03e0,0x3e01f00,0x1f00f800,0x1f80007,0xc03e001e,0xfc00f0,
	  0x7e00780,0x3f003c01,0xf8000fe0,0x1fc03e,0x3f800,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1e,0x780007f,0xfff00001,0xe0f07f03,0xfe000000,0xf00,0x7800,0x0,
	  0x1e0000,0xfc0000,0x0,0x7e0000f0,0x3f0,0x7e000fff,0xfc03ffff,0xf83f00fe,0x780,0xfc03f80,0xfc0fc00,0xf800007,0xe03f0018,0x7e00007,
	  0xe000003f,0x0,0x0,0x0,0x3c000,0x1e007c71,0xe0000f03,0xffffe003,0xf01f01ff,0xff8003ff,0xffe01e00,0x3f01,0xf81e0007,0x803ffff0,
	  0x7e03f00,0x3c000f00,0x7ffffe1e,0xf078,0xfe007e,0xfc00780,0x1f83,0xf0078000,0x783f00fe,0xf000,0x3f03f00,0xff0000,0xfc07e01f,
	  0x3e000,0x780003ff,0xfffc003c,0x7,0x800001e0,0x0,0x0,0x0,0x7e07fc01,0xfe07e001,0xf80f007e,0x7f801f8,0xfc0003c,0x7ffe,0xf0078007,
	  0x807ffffe,0xf000,0x7801f00,0xfff00f,0x3c0f01e,0x1e00fc,0xfc007f8,0x1f803f03,0xfc003c00,0xf80fc,0x1fff0,0x1f83fc0,0xff0000,
	  0xfc07e007,0xc01f0000,0xfe0001ff,0xffe0001e,0xf0,0x780,0x0,0x0,0xfe0780,0xfe0000,0x1f000000,0x3c00001f,0x7c00e03,0x81c00018,
	  0xc0,0x0,0x406203,0x7e01fc0,0x700000,0x7fffff80,0xfe0003f,0xffffc003,0xf800001f,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f0,
	  0x1f800001,0xc007c1fe,0x6000fe0,0x1ffffe,0x1e000,0x0,0x780000,0x3f98e03f,0xffff8000,0x7,0x8000003c,0x7,0xc0000000,0xfe00,
	  0x0,0x80100,0x0,0x0,0x7f000000,0x0,0x1ffff,0xfe000000,0x0,0x0,0x3f83fe8,0xe1c00,0x0,0x0,0x0,0x801,0xc1,0xc0000007,0x80003070,
	  0xfc0fc0,0x3c0001e1,0xe0000f0f,0x7878,0x3c3c0,0x1e1e00,0xf1e007,0xffc03f01,0xf007ffff,0xc03ffffe,0x1fffff0,0xfffff80,0x7fffe003,
	  0xffff001f,0xfff800ff,0xffc01fff,0xf800f001,0xfc00fc1f,0x8007e0fc,0x3f07e0,0x1f83f00,0xfc1f800,0x1f,0xf07e003f,0x3f001f8,
	  0x1f800fc0,0xfc007e07,0xe0000780,0x1e0000,0xf301f8,0xfc0ff80,0x7e07fc03,0xf03fe01f,0x81ff00fc,0xff807e0,0x7fc0f87f,0x81801f80,
	  0xf003f01f,0x801f80fc,0xfc07e0,0x7e03f00,0xfffffc07,0xffffe03f,0xffff01ff,0xfff807e0,0x7e003c00,0x3c01f81f,0x800fc0fc,0x7e07e0,
	  0x3f03f00,0x1f81f800,0x1f8000f,0xe07e001f,0x83fc00fc,0x1fe007e0,0xff003f07,0xf8000fe0,0x1fe07e,0x3f800,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1e,0x780007f,
	  0xffe00000,0xffe03fff,0xdf000000,0xf00,0x7800,0x0,0x0,0xfc0000,0x0,0x7e0000f0,0x1ff,0xfc000fff,0xfc03ffff,0xf83ffffc,0x780,
	  0xfffff00,0x7fff800,0xf000007,0xffff001f,0xffe00007,0xe000003f,0x0,0x0,0x0,0x3c000,0x1e000001,0xe0000f03,0xffffc001,0xffff01ff,
	  0xff0003ff,0xffe01e00,0x1fff,0xf81e0007,0x803ffff0,0x7fffe00,0x3c000f80,0x7ffffe1e,0xf078,0xfe003f,0xff800780,0xfff,0xf0078000,
	  0x7c3ffffc,0xf000,0x3ffff00,0xff0000,0xf803e01e,0x1e000,0x780003ff,0xfffc003c,0x7,0x800001e0,0x0,0x0,0x0,0x7fffbc01,0xffffc000,
	  0xffff003f,0xfff800ff,0xffc0003c,0x3ffe,0xf0078007,0x807ffffe,0xf000,0x7800f80,0x7ff00f,0x3c0f01e,0x1e007f,0xff8007ff,0xff001fff,
	  0xbc003c00,0xffffc,0x1fff0,0x1fffbc0,0xff0000,0x7c07c00f,0x800f8000,0x7e0001ff,0xffe0001e,0xf0,0x780,0x0,0x0,0x7fff80,0x7c0000,
	  0x1f000000,0x3c00001e,0x7c00f07,0xc1e00018,0xc0,0x0,0x60e303,0x7ffff80,0x380000,0x3fffff80,0x7c0003f,0xffffc001,0xf000000f,
	  0x80000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ff,0xff800003,0x8003ffff,0xfe0007c0,0x1ffffe,0x1e000,0x0,0x780000,0x1fffe03f,0xffff8000,
	  0x7,0x8000003c,0x3,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ffff,0xfe000000,0x0,0x0,0x3fffdf8,0xe1c00,0x0,0x0,0x0,0x0,0x1c1,
	  0xc000000f,0x7070,0x7fffc0,0x3c0001e1,0xe0000f0f,0x7878,0x3c3c0,0x1e1e00,0xf1e007,0xffc01fff,0xf007ffff,0xc03ffffe,0x1fffff0,
	  0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01fff,0xf000f001,0xfc007fff,0x3fff8,0x1fffc0,0xfffe00,0x7fff000,0x3b,0xfffc003f,
	  0xfff001ff,0xff800fff,0xfc007fff,0xe0000780,0x1e0000,0xf3fff8,0xffff780,0x7fffbc03,0xfffde01f,0xffef00ff,0xff7807ff,0xfbc0ffff,
	  0xff800fff,0xf001ffff,0x800ffffc,0x7fffe0,0x3ffff00,0xfffffc07,0xffffe03f,0xffff01ff,0xfff803ff,0xfc003c00,0x3c00ffff,0x7fff8,
	  0x3fffc0,0x1fffe00,0xffff000,0x1f,0xfffc001f,0xffbc00ff,0xfde007ff,0xef003fff,0x780007e0,0x1ffffc,0x1f800,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1e,0x700003f,
	  0xffc00000,0x7fc01fff,0x9f800000,0xf80,0xf800,0x0,0x0,0xfc0000,0x0,0x7e0000f0,0xff,0xf8000fff,0xfc03ffff,0xf83ffff8,0x780,
	  0xffffe00,0x7fff000,0xf000003,0xfffe001f,0xffc00007,0xe000003f,0x0,0x0,0x0,0x3c000,0xf000003,0xe0000f83,0xffff0000,0xffff01ff,
	  0xfc0003ff,0xffe01e00,0xfff,0xf01e0007,0x803ffff0,0x7fffc00,0x3c0007c0,0x7ffffe1e,0xf078,0x7e003f,0xff000780,0x7ff,0xe0078000,
	  0x3c3ffff8,0xf000,0x1fffe00,0x7e0000,0xf803e03e,0x1f000,0x780003ff,0xfffc003c,0x7,0x800001e0,0x0,0x0,0x0,0x3fff3c01,0xefff8000,
	  0x7ffe001f,0xff78007f,0xff80003c,0x1ffc,0xf0078007,0x807ffffe,0xf000,0x78007c0,0x3ff00f,0x3c0f01e,0x1e003f,0xff0007bf,0xfe000fff,
	  0xbc003c00,0xffff8,0xfff0,0xfff3c0,0x7e0000,0x7c07c01f,0x7c000,0x7c0001ff,0xffe0001e,0xf0,0x780,0x0,0x0,0x3fff80,0x380000,
	  0x3e000000,0x7c00003e,0x7801f07,0xc1e00018,0xc0,0x0,0x39c1ce,0x7ffff00,0x1c0000,0xfffff80,0x380003f,0xffffc000,0xe0000007,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ff,0xff000007,0x1ffcf,0xfe000380,0x1ffffe,0x1e000,0x0,0x780000,0xfffe03f,0xffff8000,0x7,
	  0x8000003c,0x3,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ffff,0xfe000000,0x0,0x0,0x3dffdf8,0xe1c00,0x0,0x0,0x0,0x0,0x381,
	  0xc000001e,0xe070,0x7fff80,0x7c0001f3,0xe0000f9f,0x7cf8,0x3e7c0,0x1f3e00,0xfbe007,0xffc00fff,0xf007ffff,0xc03ffffe,0x1fffff0,
	  0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01fff,0xc000f000,0xfc007ffe,0x3fff0,0x1fff80,0xfffc00,0x7ffe000,0x79,0xfff8001f,
	  0xffe000ff,0xff0007ff,0xf8003fff,0xc0000780,0x1e0000,0xf3fff0,0x7ffe780,0x3fff3c01,0xfff9e00f,0xffcf007f,0xfe7803ff,0xf3c07ff3,
	  0xff8007ff,0xe000ffff,0x7fff8,0x3fffc0,0x1fffe00,0xfffffc07,0xffffe03f,0xffff01ff,0xfff801ff,0xf8003c00,0x3c007ffe,0x3fff0,
	  0x1fff80,0xfffc00,0x7ffe000,0x1d,0xfff8000f,0xff3c007f,0xf9e003ff,0xcf001ffe,0x780007c0,0x1efff8,0x1f000,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780000,0x1e,0xf000003,
	  0xfe000000,0x1f000fff,0xfc00000,0x780,0xf000,0x0,0x0,0xf80000,0x0,0x7e0001e0,0x7f,0xf0000fff,0xfc03ffff,0xf81ffff0,0x780,
	  0x7fff800,0x1ffe000,0x1f000000,0xfff8001f,0xff000007,0xe000003e,0x0,0x0,0x0,0x3c000,0xf800003,0xc0000783,0xfff80000,0x3ffe01ff,
	  0xe00003ff,0xffe01e00,0x7ff,0xc01e0007,0x803ffff0,0x3fff800,0x3c0003c0,0x7ffffe1e,0xf078,0x7e000f,0xfe000780,0x3ff,0xc0078000,
	  0x3e1fffe0,0xf000,0x7ff800,0x7e0000,0xf803e07c,0xf800,0x780003ff,0xfffc003c,0x3,0xc00001e0,0x0,0x0,0x0,0xffe3c01,0xe7ff0000,
	  0x3ffc000f,0xfe78003f,0xfe00003c,0x7f0,0xf0078007,0x807ffffe,0xf000,0x78003e0,0xff00f,0x3c0f01e,0x1e001f,0xfe00079f,0xfc0007ff,
	  0x3c003c00,0x7ffe0,0x1ff0,0x7fe3c0,0x7e0000,0x7c07c03e,0x3e000,0x7c0001ff,0xffe0001e,0xf0,0x780,0x0,0x0,0xfff00,0x100000,
	  0x3e000000,0x7800003c,0xf800f07,0xc1e00018,0xc0,0x0,0x1f80fc,0x3fffc00,0xc0000,0x3ffff80,0x100003f,0xffffc000,0x40000002,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xfc000006,0xff87,0xfc000100,0x1ffffe,0x1e000,0x0,0x780000,0x3ffc03f,0xffff8000,0x7,
	  0x8000003c,0x3,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ffff,0xfe000000,0x0,0x0,0x3dff9f8,0xe1c00,0x0,0x0,0x0,0x0,0x3ff,
	  0xf800003c,0xfffe,0x1ffe00,0x780000f3,0xc000079e,0x3cf0,0x1e780,0xf3c00,0x7bc007,0xffc003ff,0xe007ffff,0xc03ffffe,0x1fffff0,
	  0xfffff80,0x7fffe003,0xffff001f,0xfff800ff,0xffc01ffc,0xf000,0xfc001ffc,0xffe0,0x7ff00,0x3ff800,0x1ffc000,0x70,0xfff00007,
	  0xff80003f,0xfc0001ff,0xe0000fff,0x780,0x1e0000,0xf3ffe0,0x1ffc780,0xffe3c00,0x7ff1e003,0xff8f001f,0xfc7800ff,0xe3c03fe1,
	  0xff0003ff,0xc0007ffc,0x3ffe0,0x1fff00,0xfff800,0xfffffc07,0xffffe03f,0xffff01ff,0xfff800ff,0xf0003c00,0x3c003ffc,0x1ffe0,
	  0xfff00,0x7ff800,0x3ffc000,0x38,0xfff00007,0xfe3c003f,0xf1e001ff,0x8f000ffc,0x780007c0,0x1e7ff0,0x1f000,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,
	  0x1fc,0x0,0x780,0xf000,0x0,0x0,0x1f80000,0x0,0x1e0,0x1f,0xc0000000,0x0,0x1ff80,0x0,0xffc000,0x7f8000,0x0,0x3fe00007,0xfc000000,
	  0x7e,0x0,0x0,0x0,0x0,0x7c00000,0x0,0x0,0xff00000,0x0,0x0,0xfe,0x0,0x0,0x3fc000,0x0,0x0,0x0,0x3,0xf8000000,0xff,0xc0000000,
	  0x1ff00,0x0,0x1fe000,0x0,0x0,0x0,0x0,0x3c,0x3,0xc00001e0,0x0,0x0,0x0,0x3f80000,0x1fc0000,0x7f00003,0xf8000007,0xf0000000,
	  0x0,0xf0000000,0x0,0xf000,0x0,0x0,0x0,0x7,0xf8000787,0xf00001fc,0x3c000000,0x7f80,0x0,0x1f8000,0x0,0x0,0x0,0x7c000000,0x1e,
	  0xf0,0x780,0x0,0x0,0x3fc00,0x0,0x3c000000,0x7800003c,0xf000601,0xc00018,0xc0,0x0,0x0,0x3fe000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0xf,0xf0000000,0x7e03,0xf0000000,0x0,0x0,0x0,0x0,0xfe0000,0x0,0x0,0x3c,0x2007,0x80000000,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c7e0f0,0xe1c00,0x0,0x3800000,0x0,0x0,0x3ff,0xf8000078,0xfffe,0x7f800,0x0,0x0,0x0,0x0,
	  0x0,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f0,0x3f80,0x1fc00,0xfe000,0x7f0000,0x70,0x3fc00001,0xfe00000f,0xf000007f,
	  0x800003fc,0x0,0x0,0xff00,0x7f0000,0x3f80000,0x1fc00000,0xfe000007,0xf000003f,0x80001f80,0xfc00007f,0xfe0,0x7f00,0x3f800,
	  0x1fc000,0x0,0x0,0x0,0x3f,0xc0000000,0xff0,0x7f80,0x3fc00,0x1fe000,0xff0000,0x78,0x3fc00001,0xf800000f,0xc000007e,0x3f0,0x7c0,
	  0x1e1fc0,0x1f000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,0x3c0,0x1e000,0x0,0x0,0x1f00000,0x0,0x3c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x7c,0x0,0x0,0x0,0x0,0x3e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0xe0000000,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x3c,0x1,0xe00001e0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0000000,0x0,0xf000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x78000000,0x1e,0xf0,0x780,0x0,0x0,0x0,0x0,0x3c000000,0x78000078,0xf000000,0x18,0xc0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x180000,0x0,0x0,0x3c,0x3c0f,0x80000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0xe1c00,0x0,0x1800000,0x0,0x0,0x3ff,0xf80000f0,0xfffe,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0xc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0xc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30,0x0,0x0,0x0,0x0,0x780,0x1e0000,0x1e000,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,
	  0x0,0x0,0x3c0,0x1e000,0x0,0x0,0x1f00000,0x0,0x3c0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7c,0x0,0x0,0x0,0x0,0x1f80000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0xf0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c,0x1,0xe00001e0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xe0000000,0x0,0xf000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x0,0xf8000000,
	  0x1f,0xf0,0xf80,0x0,0x0,0x0,0x0,0x78000000,0xf8000078,0x1e000000,0x8,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x180000,0x0,0x0,0x3c,0x3fff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x3c00000,0xe1c00,0x0,0x1c00000,0x0,0x0,0x1,0xc00001e0,0x70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf80,0x1e0000,0x3e000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,0x1e0,0x3c000,0x0,0x0,0x1f00000,
	  0x0,0x780,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7c,0x0,0x0,0x0,0x0,0xfe0100,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0xf8000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0xf0000000,0xf0007fe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0xe0000000,
	  0x0,0xf000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x0,0xf0000000,0x1f,0x800000f0,0x1f80,0x0,0x0,0x0,0x0,
	  0x78000000,0xf0000070,0x1c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x180000,0x0,0x0,0x3c,0x3ffe,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0xe1c00,0x0,0xe00000,
	  0x0,0x0,0x1,0xc00003ff,0xe0000070,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0xf00,0x1e0000,0x3c000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,0x1e0,0x7c000,0x0,0x0,0x1e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x78,0x0,0x0,0x0,0x0,0x7fff80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x78000000,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0xf0000000,0x7fe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4003,0xe0000000,0x0,0x1f000,0x0,0x0,
	  0x0,0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x1,0xf0000000,0xf,0xfc0000f0,0x3ff00,0x0,0x0,0x0,0x0,0x70000001,0xf00000e0,
	  0x1c000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x180000,
	  0x0,0x0,0x3c,0xff8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0xe1c00,0x0,0xe00000,0x0,0x0,0x1,0xc00003ff,
	  0xe0000070,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f00,0x1e0000,
	  0x7c000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,0xf0,0x78000,0x0,0x0,0x3e00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x0,
	  0x0,0x0,0x0,0x1fff80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,
	  0xf0000000,0x7fe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x780f,0xc0000000,0x0,0x3e000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,
	  0x0,0x0,0x0,0x0,0x3,0xe0000000,0xf,0xfc0000f0,0x3ff00,0x0,0x0,0x0,0x0,0xf0000103,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x180000,0x0,0x0,0x3c,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x0,0x0,0x21e00000,0x0,0x0,0x1,0xc00003ff,0xe0000070,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10f,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10f,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3e00,0x1e0000,0xf8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30000000,0x0,0x0,
	  0xf8,0xf8000,0x0,0x0,0x3c00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0,0x0,0x0,0x0,0x0,0x1fe00,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0xf0000000,0x7fe0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x7fff,0xc0000000,0x0,0x3ffe000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x7f,0xe0000000,0x7,0xfc0000f0,
	  0x3fe00,0x0,0x0,0x0,0x0,0x600001ff,0xe0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x180000,0x0,0x0,0x3c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x0,0x0,
	  0x3fe00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1ff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x7fe00,0x1e0000,0x1ff8000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x1fffffe0,0x0,0x0,0x0,0x0,0x0,0x0,0x7fff,0x80000000,0x0,0x3ffc000,0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,
	  0x0,0x0,0x0,0x0,0x7f,0xc0000000,0x0,0xfc0000f0,0x3f000,0x0,0x0,0x0,0x0,0x1ff,0xc0000000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x3c00000,0x0,0x0,0x3fc00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fe,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fe,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7fc00,0x1e0000,0x1ff0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fffffe0,0x0,0x0,0x0,0x0,0x0,0x0,0x3ffe,0x0,0x0,0x3ff8000,0x0,0x0,0x0,
	  0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x7f,0x80000000,0x0,0xf0,0x0,0x0,0x0,0x0,0x0,0x1ff,0x80000000,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x0,0x0,0x3f800000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fc,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fc,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f800,0x1e0000,0x1fe0000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fffffe0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f8,0x0,0x0,0x3fe0000,
	  0x0,0x0,0x0,0x0,0x780,0x0,0x3c000000,0x0,0x0,0x0,0x0,0x0,0x7e,0x0,0x0,0xf0,0x0,0x0,0x0,0x0,0x0,0xfe,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c00000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7e000,0x1e0000,0x1f80000,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1fffffe0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	// Definition of a 40x38 'danger' color logo
	const unsigned char logo40x38[4576] = {
	  177,200,200,200,3,123,123,0,36,200,200,200,1,123,123,0,2,255,255,0,1,189,189,189,1,0,0,0,34,200,200,200,
	  1,123,123,0,4,255,255,0,1,189,189,189,1,0,0,0,1,123,123,123,32,200,200,200,1,123,123,0,5,255,255,0,1,0,0,
	  0,2,123,123,123,30,200,200,200,1,123,123,0,6,255,255,0,1,189,189,189,1,0,0,0,2,123,123,123,29,200,200,200,
	  1,123,123,0,7,255,255,0,1,0,0,0,2,123,123,123,28,200,200,200,1,123,123,0,8,255,255,0,1,189,189,189,1,0,0,0,
	  2,123,123,123,27,200,200,200,1,123,123,0,9,255,255,0,1,0,0,0,2,123,123,123,26,200,200,200,1,123,123,0,10,255,
	  255,0,1,189,189,189,1,0,0,0,2,123,123,123,25,200,200,200,1,123,123,0,3,255,255,0,1,189,189,189,3,0,0,0,1,189,
	  189,189,3,255,255,0,1,0,0,0,2,123,123,123,24,200,200,200,1,123,123,0,4,255,255,0,5,0,0,0,3,255,255,0,1,189,
	  189,189,1,0,0,0,2,123,123,123,23,200,200,200,1,123,123,0,4,255,255,0,5,0,0,0,4,255,255,0,1,0,0,0,2,123,123,123,
	  22,200,200,200,1,123,123,0,5,255,255,0,5,0,0,0,4,255,255,0,1,189,189,189,1,0,0,0,2,123,123,123,21,200,200,200,
	  1,123,123,0,5,255,255,0,5,0,0,0,5,255,255,0,1,0,0,0,2,123,123,123,20,200,200,200,1,123,123,0,6,255,255,0,5,0,0,
	  0,5,255,255,0,1,189,189,189,1,0,0,0,2,123,123,123,19,200,200,200,1,123,123,0,6,255,255,0,1,123,123,0,3,0,0,0,1,
	  123,123,0,6,255,255,0,1,0,0,0,2,123,123,123,18,200,200,200,1,123,123,0,7,255,255,0,1,189,189,189,3,0,0,0,1,189,
	  189,189,6,255,255,0,1,189,189,189,1,0,0,0,2,123,123,123,17,200,200,200,1,123,123,0,8,255,255,0,3,0,0,0,8,255,255,
	  0,1,0,0,0,2,123,123,123,16,200,200,200,1,123,123,0,9,255,255,0,1,123,123,0,1,0,0,0,1,123,123,0,8,255,255,0,1,189,
	  189,189,1,0,0,0,2,123,123,123,15,200,200,200,1,123,123,0,9,255,255,0,1,189,189,189,1,0,0,0,1,189,189,189,9,255,255,
	  0,1,0,0,0,2,123,123,123,14,200,200,200,1,123,123,0,11,255,255,0,1,0,0,0,10,255,255,0,1,189,189,189,1,0,0,0,2,123,
	  123,123,13,200,200,200,1,123,123,0,23,255,255,0,1,0,0,0,2,123,123,123,12,200,200,200,1,123,123,0,11,255,255,0,1,189,
	  189,189,2,0,0,0,1,189,189,189,9,255,255,0,1,189,189,189,1,0,0,0,2,123,123,123,11,200,200,200,1,123,123,0,11,255,255,
	  0,4,0,0,0,10,255,255,0,1,0,0,0,2,123,123,123,10,200,200,200,1,123,123,0,12,255,255,0,4,0,0,0,10,255,255,0,1,189,189,
	  189,1,0,0,0,2,123,123,123,9,200,200,200,1,123,123,0,12,255,255,0,1,189,189,189,2,0,0,0,1,189,189,189,11,255,255,0,1,
	  0,0,0,2,123,123,123,9,200,200,200,1,123,123,0,27,255,255,0,1,0,0,0,3,123,123,123,8,200,200,200,1,123,123,0,26,255,
	  255,0,1,189,189,189,1,0,0,0,3,123,123,123,9,200,200,200,1,123,123,0,24,255,255,0,1,189,189,189,1,0,0,0,4,123,123,
	  123,10,200,200,200,1,123,123,0,24,0,0,0,5,123,123,123,12,200,200,200,27,123,123,123,14,200,200,200,25,123,123,123,86,
	  200,200,200,91,49,124,118,124,71,32,124,95,49,56,114,52,82,121,0};
  
	// Return a 'stringification' of standard integral types.
	const char* const bool_st    = "bool";
	const char* const uchar_st   = "unsigned char";
	const char* const char_st    = "char";
	const char* const ushort_st  = "unsigned short";
	const char* const short_st   = "short";
	const char* const uint_st    = "unsigned int";
	const char* const int_st     = "int";
	const char* const ulong_st   = "unsigned long";
	const char* const long_st    = "long";
	const char* const float_st   = "float";
	const char* const double_st  = "double";
	const char* const unknown_st = "unknown";
	template<typename t> inline const char* get_type(const t&) { return cimg::unknown_st; }
	inline const char* get_type(const bool&          ) { return cimg::bool_st;   }
	inline const char* get_type(const unsigned char& ) { return cimg::uchar_st;  }
	inline const char* get_type(const char&          ) { return cimg::char_st;   }
	inline const char* get_type(const unsigned short&) { return cimg::ushort_st; }
	inline const char* get_type(const short&         ) { return cimg::short_st;  }
	inline const char* get_type(const unsigned int&  ) { return cimg::uint_st;   }
	inline const char* get_type(const int&           ) { return cimg::int_st;    }
	inline const char* get_type(const unsigned long& ) { return cimg::ulong_st;  }
	inline const char* get_type(const long&          ) { return cimg::long_st;   }
	inline const char* get_type(const float&         ) { return cimg::float_st;  }
	inline const char* get_type(const double&        ) { return cimg::double_st; }
										   
	// Display a warning message if parameter 'cond' is true.
#if cimg_debug>=1    
	inline void warn(const bool cond,const char *format,...) {
	  if (cond) {
		std::va_list ap;
		va_start(ap,format);
		std::fprintf(stderr,"<CImg Warning> ");
		std::vfprintf(stderr,format,ap);
		std::fputc('\n',stderr);
		va_end(ap);
	  }
	}
#else
	inline void warn(const bool cond,const char *format,...) {}
#endif

	inline int xln(const int x) {
	  return x>0?(int)(1+std::log10((double)x)):1; 
	}
	
	inline char uncase(const char x) {
	  return (char)((x<'A'||x>'Z')?x:x-'A'+'a'); 
	}
	
	inline float atof(const char *str) {
	  float x=0,y=1;
	  if (!str) return 0; else { std::sscanf(str,"%g/%g",&x,&y); return x/y; }
	}

	inline int strlen(const char *s) {
	  if (s) { int k; for (k=0; s[k]; k++) ; return k; }
	  return -1; 
	}

	inline int strncmp(const char *s1,const char *s2,const int l) {
	  if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=std::abs(s1[k] - s2[k]); return n; }
	  return 0;
	}

	inline int strncasecmp(const char *s1,const char *s2,const int l) {
	  if (s1 && s2) { int n=0; for (int k=0; k<l; k++) n+=std::abs(uncase(s1[k])-uncase(s2[k])); return n; }
	  return 0;
	}
	
	inline int strcmp(const char *s1,const char *s2) { 
	  const int l1 = cimg::strlen(s1), l2 = cimg::strlen(s2);
	  return cimg::strncmp(s1,s2,1+(l1<l2?l1:l2));
	}

	inline int strcasecmp(const char *s1,const char *s2) { 
	  const int l1 = cimg::strlen(s1), l2 = cimg::strlen(s2);
	  return cimg::strncasecmp(s1,s2,1+(l1<l2?l1:l2));
	}

	inline int strfind(const char *s,const char c) {
	  if (s) { 
		int l; for (l=cimg::strlen(s); l>=0 && s[l]!=c; l--) ;
		return l; 
	  }
	  return -1; 
	}

	inline const char* basename(const char *s)  {
	  return (cimg_OS!=2)?(s?s+1+cimg::strfind(s,'/'):0):(s?s+1+cimg::strfind(s,'\\'):0); 
	}
	
	inline void system(const char *command) {
#if cimg_OS==2
	  PROCESS_INFORMATION pi;
	  STARTUPINFO si;
	  memset(&pi, 0, sizeof(pi));
	  memset(&si, 0, sizeof(si));
	  GetStartupInfo(&si);
	  si.cb = sizeof(si);
	  si.wShowWindow = SW_HIDE;
	  si.dwFlags |= SW_HIDE;
	  BOOL res = CreateProcess(0,(LPTSTR)command,0,0,FALSE,0,0,0,&si,&pi);
	  if (res) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	  }
#else
	  std::system(command);
#endif
	}
	
	//! Return path of the ImageMagick's \c convert tool.
	/**
	   If you have installed the <a href="http://www.imagemagick.org">ImageMagick package</a>
	   in a standard directory, this function should return the correct path of the \c convert tool
	   used by the %CImg Library to load and save compressed image formats.
	   Conversely, if the \c convert executable is not auto-detected by the function,
	   you can define the macro \c cimg_convert_path with the correct path
	   of the \c convert executable, before including <tt>CImg.h</tt> in your program :
	   \code
	   #define cimg_convert_path "/users/thatsme/local/bin/convert"
	   #include "CImg.h"
	   
	   int main() {
		 CImg<> img("my_image.jpg");     // Read a JPEG image file.
	 return 0;
	   }
	   \endcode
	   
	   Note that non compressed image formats can be read without installing ImageMagick.
	   
	   \sa temporary_path(), get_load_convert(), load_convert(), save_convert().
	**/
	inline const char* convert_path() {
	  static char *st_convert_path = 0;
	  if (!st_convert_path) {
		st_convert_path = new char[1024];
	bool path_found = false;
#ifdef cimg_convert_path
	{
	  std::FILE *file = 0;	
	  std::strcpy(st_convert_path,cimg_convert_path);
	  if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	}
#endif
#if cimg_OS==2
	{
	  std::FILE *file = 0;	
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"C:\\PROGRA~1\\IMAGEM~1.%u-Q\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"C:\\PROGRA~1\\IMAGEM~1.%u\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"C:\\PROGRA~1\\IMAGEM~1.%u-Q\\VISUA~1\\BIN\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"C:\\PROGRA~1\\IMAGEM~1.%u\\VISUA~1\\BIN\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"D:\\PROGRA~1\\IMAGEM~1.%u-Q\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"D:\\PROGRA~1\\IMAGEM~1.%u\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"D:\\PROGRA~1\\IMAGEM~1.%u-Q\\VISUA~1\\BIN\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  { for (unsigned int k=0; k<=9 && !path_found; k++) {
		std::sprintf(st_convert_path,"D:\\PROGRA~1\\IMAGEM~1.%u\\VISUA~1\\BIN\\convert.exe",k);
		if ((file=std::fopen(st_convert_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }}
	  if (!path_found) std::strcpy(st_convert_path,"convert.exe");
	}
#else
	{
	  if (!path_found) std::strcpy(st_convert_path,"convert");
	}
#endif
	  }
	  return st_convert_path;
	}

	//! Return path of the \c XMedcon tool.
	/**
	   If you have installed the <a href="http://xmedcon.sourceforge.net/">XMedcon package</a>
	   in a standard directory, this function should return the correct path of the \c medcon tool
	   used by the %CIg Library to load DICOM image formats.
	   Conversely, if the \c medcon executable is not auto-detected by the function,
	   you can define the macro \c cimg_medcon_path with the correct path
	   of the \c medcon executable, before including <tt>CImg.h</tt> in your program :
	   \code
	   #define cimg_medcon_path "/users/thatsme/local/bin/medcon"
	   #include "CImg.h"
	   
	   int main() {
		 CImg<> img("my_image.dcm");    // Read a DICOM image file.
	 return 0;
	   }
	   \endcode
	   
	   Note that \c medcon is only needed if you want to read DICOM image formats.

	   \sa temporary_path(), get_load_dicom(), load_dicom().
	**/
	inline const char* medcon_path() {
	  static char *st_medcon_path = 0;
	  if (!st_medcon_path) {
		st_medcon_path = new char[1024];
	bool path_found = false;
#ifdef cimg_medcon_path
	{
	  std::FILE *file = 0;	
	  std::strcpy(st_medcon_path,cimg_medcon_path);
	  if ((file=std::fopen(st_medcon_path,"r"))!=0) { std::fclose(file); path_found = true; }
	}
#endif
#if cimg_OS==2
	{
	  std::FILE *file = 0;
	  if (!path_found) {
		std::sprintf(st_medcon_path,"C:\\PROGRA~1\\XMedCon\\bin\\medcon.bat");
		if ((file=std::fopen(st_medcon_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }
	  if (!path_found) {
		std::sprintf(st_medcon_path,"D:\\PROGRA~1\\XMedCon\\bin\\medcon.bat");
		if ((file=std::fopen(st_medcon_path,"r"))!=0) { std::fclose(file); path_found = true; }
	  }
	  if (!path_found) std::strcpy(st_medcon_path,"medcon.bat");
	}
#else
	{
	  if (!path_found) std::strcpy(st_medcon_path,"medcon");
	}
#endif
	  }
	  return st_medcon_path;
	}

	//! Return path to store temporary files.
	/**
	   If you are running on a standard Unix or Windows system, this function should return a correct path
	   where temporary files can be stored. If such a path is not auto-detected by this function,
	   you can define the macro \c cimg_temporary_path with a correct path, before including <tt>CImg.h</tt>
	   in your program :
	   \code
	   #define cimg_temporary_path "/users/thatsme/tmp"
	   #include "CImg.h"

	   int main() {
		 CImg<> img("my_image.jpg");   // Read a JPEG image file (using the defined temporay path).
	 return 0;
	   }
	   \endcode
	   
	   A temporary path is necessary to load and save compressed image formats, using \c convert
	   or \c medcon.
	   
	   \sa convert_path(), get_load_convert(), load_convert(), save_convert(), get_load_dicom(), load_dicom().
	**/
	inline const char* temporary_path() {
	  static char *st_temporary_path = 0;
	  if (!st_temporary_path) {
		st_temporary_path = new char[1024];
#ifdef cimg_temporary_path
		std::strcpy(st_temporary_path,cimg_temporary_path);
		const char* testing_path[7] = { st_temporary_path, "/tmp","C:\\WINNT\\Temp", "C:\\WINDOWS\\Temp","","C:",0 };
#else
		const char* testing_path[6] = { "/tmp","C:\\WINNT\\Temp", "C:\\WINDOWS\\Temp","","C:",0 };
#endif
		char filetmp[1024];
		std::FILE *file=0;
		int i=-1;
		while (!file && testing_path[++i]) {
		  std::sprintf(filetmp,"%s/CImg%.4d.ppm",testing_path[i],std::rand()%10000);
		  if ((file=std::fopen(filetmp,"w"))!=0) { std::fclose(file); std::remove(filetmp); }
		}
		if (!file) 
	  throw CImgIOException("cimg::temporary_path() : Unable to find a temporary path accessible for writing\n"
				"you have to set the macro 'cimg_temporary_path' to a valid path where you have writing access :\n"
				"#define cimg_temporary_path \"path\" (before including 'CImg.h')");
		std::strcpy(st_temporary_path,testing_path[i]);
	  }
	  return st_temporary_path;
	}
	
	inline const char *filename_split(const char *const filename, char *const body=0) {
	  if (!filename) { if (body) body[0]='\0'; return 0; }
	  int l = cimg::strfind(filename,'.');
	  if (l>=0) { if (body) { std::strncpy(body,filename,l); body[l]='\0'; }}
	  else { if (body) std::strcpy(body,filename); l=(int)std::strlen(filename)-1; }
	  return filename+l+1;
	}
	
	inline char* filename_number(const char *filename,const int number,const unsigned int n,char *const string) {
	  if (!filename) { if (string) string[0]='\0'; return 0; }
	  char format[1024],body[1024];
	  const char *ext = cimg::filename_split(filename,body);
	  if (n>0) std::sprintf(format,"%s_%%.%ud.%s",body,n,ext);
	  else std::sprintf(format,"%s_%%d.%s",body,ext);
	  std::sprintf(string,format,number);
	  return string;
	}

	inline std::FILE *fopen(const char *const path,const char *const mode) {
	  if(!path || !mode) throw CImgArgumentException("cimg::fopen() : Can't open file '%s' with mode '%s'",path,mode);
	  if (path[0]=='-') return (mode[0]=='r')?stdin:stdout; 
	  else {
		std::FILE *dest = std::fopen(path,mode);
		if(!dest) throw CImgIOException("cimg::fopen() : File '%s' cannot be opened %s",
										path,mode[0]=='r'?"for reading":(mode[0]=='w'?"for writing":""),path);
		return dest;
	  }
	}

	inline int fclose(std::FILE *file) {
	  warn(!file,"cimg::fclose() : Can't close (null) file");
	  if (!file || file==stdin || file==stdout) return 0;
	  const int errn=std::fclose(file);
	  warn(errn!=0,"cimg::fclose() : Error %d during file closing",errn);
	  return errn;
	}
	template<typename T> inline int fread(T *ptr,const unsigned int nmemb,std::FILE *stream) {
	  if (!ptr || nmemb<=0 || !stream)
		throw CImgArgumentException("cimg::fread() : Can't read %u x %u bytes of file pointer '%p' in buffer '%p'",
					nmemb,sizeof(T),stream,ptr);
	  const unsigned int errn = (unsigned int)std::fread((void*)ptr,sizeof(T),nmemb,stream);
	  cimg::warn(errn!=nmemb,"cimg::fread() : File reading problems, only %u/%u elements read",errn,nmemb);
	  return errn;
	}
	template<typename T> inline int fwrite(const T *ptr,const unsigned int nmemb,std::FILE *stream) {
	  if (!ptr || nmemb<=0 || !stream)
		throw CImgArgumentException("cimg::fwrite() : Can't write %u x %u bytes of file pointer '%p' from buffer '%p'",
					nmemb,sizeof(T),stream,ptr);
	  const unsigned int errn = (unsigned int)std::fwrite(ptr,sizeof(T),nmemb,stream);
	  if(errn!=nmemb)
		throw CImgIOException("cimg::fwrite() : File writing problems, only %u/%u elements written",errn,nmemb);
	  return errn;
	}
	
	// Exchange the values of variables \p a and \p b
	template<typename T> inline void swap(T& a,T& b) { T t=a; a=b; b=t; }
	template<typename T1,typename T2> inline void swap(T1& a1,T1& b1,T2& a2,T2& b2) {
	  cimg::swap(a1,b1); cimg::swap(a2,b2); 
	}
	template<typename T1,typename T2,typename T3> inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3) {
	  cimg::swap(a1,b1,a2,b2); cimg::swap(a3,b3); 
	}
	template<typename T1,typename T2,typename T3,typename T4>
	inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4) {
	  cimg::swap(a1,b1,a2,b2,a3,b3); cimg::swap(a4,b4); 
	}
	template<typename T1,typename T2,typename T3,typename T4,typename T5>
	inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4,T5& a5,T5& b5) {
	  cimg::swap(a1,b1,a2,b2,a3,b3,a4,b4); cimg::swap(a5,b5); 
	}
	template<typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	inline void swap(T1& a1,T1& b1,T2& a2,T2& b2,T3& a3,T3& b3,T4& a4,T4& b4,T5& a5,T5& b5,T6& a6,T6& b6) {
	  cimg::swap(a1,b1,a2,b2,a3,b3,a4,b4,a5,b5); cimg::swap(a6,b6);
	}
	
	template<typename T> inline void endian_swap(T* const buffer, const unsigned int size) {
	  switch (sizeof(T)) {
	  case 1: break;
	  case 2: {
	for (unsigned short *ptr = (unsigned short*)buffer+size; ptr>(unsigned short*)buffer;) {
	  const unsigned short val = *(--ptr);
	  *ptr = (val>>8)|((val<<8));
	}
	  } break;
	  case 4: {
	for (unsigned int *ptr = (unsigned int*)buffer+size; ptr>(unsigned int*)buffer;) {
	  const unsigned int val = *(--ptr);
	  *ptr = (val>>24)|((val>>8)&0xff00)|((val<<8)&0xff0000)|(val<<24);
	}
	  } break;
	  default: {
	for (T* ptr = buffer+size; ptr>buffer; --ptr) {
	  unsigned char *pb=(unsigned char*)(--ptr), *pe=pb+sizeof(T);
	  for (int i=0; i<(int)sizeof(T)/2; i++) cimg::swap(*(pb++),*(--pe));
	} break;
	  }
	  }
	}
	template<typename T> inline T& endian_swap(T& a) { endian_swap(&a,1); return a; }

	inline const char* option(const char *const name, const int argc, char **argv,
				  const char *defaut, const char *const usage=0) {
	  static bool first=true, visu=false;
	  const char *res = 0;
	  if (first) { 
	first=false;
	visu = (cimg::option("-h",argc,argv,(const char*)0)!=0); 
	visu |= (cimg::option("-help",argc,argv,(const char*)0)!=0); 
	visu |= (cimg::option("--help",argc,argv,(const char*)0)!=0); 
	  }
	  if (!name && visu) {
		std::fprintf(stderr,"\n %s%s%s",cimg::t_red,cimg::basename(argv[0]),cimg::t_normal);
		if (usage) std::fprintf(stderr," : %s",usage);
		std::fprintf(stderr," (%s, %s)\n\n",__DATE__,__TIME__);
	  }
	  if (name) {
		if (argc>0) {
	  int k=0,i;
		  while (k<argc && cimg::strcmp(argv[k],name)) k++;
		  i=k;
		  res=(k++==argc?defaut:(k==argc?argv[--k]:argv[k]));
		} else res = defaut;
		if (visu && usage) std::fprintf(stderr,"    %s%-8s%s = %-12s : %s%s%s\n",
										cimg::t_bold,name,cimg::t_normal,res?res:"0",cimg::t_purple,usage,cimg::t_normal);
	  }
	  return res;
	}

	inline bool option(const char *const name, const int argc, char **argv,
					   const bool defaut, const char *const usage=0) {
	  const char *s = cimg::option(name,argc,argv,(const char*)0);
	  const bool res = s?(cimg::strcasecmp(s,"false") && cimg::strcasecmp(s,"off") && cimg::strcasecmp(s,"0")):defaut;
	  cimg::option(name,0,0,res?"true":"false",usage);
	  return res;
	}

	inline int option(const char *const name, const int argc, char **argv,
					  const int defaut, const char *const usage=0) {
	  const char *s = cimg::option(name,argc,argv,(const char*)0);
	  const int res = s?std::atoi(s):defaut;
	  char tmp[256];
	  std::sprintf(tmp,"%d",res);
	  cimg::option(name,0,0,tmp,usage);
	  return res;
	}

	inline char option(const char *const name, const int argc, char **argv,
			   const char defaut, const char *const usage=0) {
	  const char *s = cimg::option(name,argc,argv,(const char*)0);
	  const char res = s?s[0]:defaut;
	  char tmp[8];
	  tmp[0] = res;
	  tmp[1] ='\0';
	  cimg::option(name,0,0,tmp,usage);
	  return res;
	}

	inline float option(const char *const name, const int argc, char **argv,
			const float defaut, const char *const usage=0) {
	  const char *s = cimg::option(name,argc,argv,(const char*)0);
	  const float res = s?cimg::atof(s):defaut;
	  char tmp[256];
	  std::sprintf(tmp,"%g",res);
	  cimg::option(name,0,0,tmp,usage);
	  return res;
	}

	inline double option(const char *const name, const int argc, char **argv,
			 const double defaut, const char *const usage=0) {
	  const char *s = cimg::option(name,argc,argv,(const char*)0);
	  const double res = s?cimg::atof(s):defaut;
	  char tmp[256];
	  std::sprintf(tmp,"%g",res);
	  cimg::option(name,0,0,tmp,usage);
	  return res;
	}
	
	//! Return \c false for little endian CPUs (Intel), \c true for big endian CPUs (Motorola).
	inline bool endian() { const int x=1; return ((unsigned char*)&x)[0]?false:true; }

	//! Print informations about %CImg environement variables.
	/**
	   Printing is done on the standart error output.
	**/
	inline void info() {
	  std::fprintf(stderr,"\n %sCImg Library %g%s, compiled %s ( %s ) with the following flags :\n\n",
				   cimg::t_red,cimg_version,cimg::t_normal,__DATE__,__TIME__);
	  std::fprintf(stderr,"  > Architecture   : %s%-12s%s %s(cimg_OS=%d)\n%s",
				   cimg::t_bold,
				   cimg_OS==1?"Unix":(cimg_OS==2?"Windows":"Unknown"),
				   cimg::t_normal,cimg::t_purple,cimg_OS,cimg::t_normal);
	  std::fprintf(stderr,"  > Display type   : %s%-12s%s %s(cimg_display_type=%d)%s\n",
				   cimg::t_bold,cimg_display_type==0?"No":
		   (cimg_display_type==1?"X11":
			(cimg_display_type==2?"WindowsGDI":
			 "Unknown")),
		   cimg::t_normal,cimg::t_purple,cimg_display_type,cimg::t_normal);
#ifdef cimg_color_terminal
	  std::fprintf(stderr,"  > Color terminal : %s%-12s%s %s(cimg_color_terminal defined)%s\n",
		   cimg::t_bold,"Yes",cimg::t_normal,cimg::t_purple,cimg::t_normal);
#else
	  std::fprintf(stderr,"  > Color terminal : %-12s (cimg_color_terminal undefined)\n","No");
#endif
	  std::fprintf(stderr,"  > Debug messages : %s%-12s%s %s(cimg_debug=%d)%s\n",cimg::t_bold,
		   cimg_debug==2?"High":(cimg_debug==1?"Yes":"No"),
				   cimg::t_normal,cimg::t_purple,cimg_debug,cimg::t_normal);
	  std::fprintf(stderr,"\n");
	}
	
	//! Get the value of a system timer with a millisecond precision.
	inline unsigned long time() {
#if cimg_OS==1
	  struct timeval st_time;
	  gettimeofday(&st_time,0);
	  return (unsigned long)(st_time.tv_usec/1000 + st_time.tv_sec*1000);
#elif cimg_OS==2
	  static SYSTEMTIME st_time;
	  GetSystemTime(&st_time);
	  return (unsigned long)(st_time.wMilliseconds + 1000*(st_time.wSecond + 60*(st_time.wMinute + 60*st_time.wHour)));
#else 
	  return 0;
#endif
	}

	//! Sleep for a certain numbers of milliseconds.
	/**
	   This function frees the CPU ressources during the sleeping time.
	   It may be used to temporize your program properly, without wasting CPU time.
	   \sa wait(), time().
	**/
	inline void sleep(const unsigned int milliseconds) {
#if cimg_OS==1
	  struct timespec tv;
	  tv.tv_sec = milliseconds/1000;
	  tv.tv_nsec = (milliseconds%1000)*1000000;
	  nanosleep(&tv,0);
#elif cimg_OS==2
	  Sleep(milliseconds);
#endif
	}

	inline unsigned int wait(const unsigned int milliseconds, unsigned long& timer) {
	  if (!timer) timer = cimg::time();
	  const unsigned long current_time = cimg::time();
	  if (current_time>=timer+milliseconds) { timer = current_time; return 0; }
	  const unsigned long time_diff = timer + milliseconds - current_time;
	  timer = current_time + time_diff;
	  cimg::sleep(time_diff);
	  return (unsigned int)time_diff;
	}

	//! Wait for a certain number of milliseconds since the last call.
	/**
	   This function is equivalent to sleep() but the waiting time is computed with regard to the last call
	   of wait(). It may be used to temporize your program properly.
	   \sa sleep(), time().
	**/
	inline unsigned int wait(const unsigned int milliseconds) {
	  static unsigned long timer = 0;
	  if (!timer) timer = cimg::time();
	  return wait(milliseconds,timer);
	}

	template<typename T> inline const T rol(const T& a, const unsigned int n=1) {
	  return (T)((a<<n)|(a>>((sizeof(T)<<3)-n))); 
	}
	
	template<typename T> inline const T ror(const T& a, const unsigned int n=1) {
	  return (T)((a>>n)|(a<<((sizeof(T)<<3)-n))); 
	}

#if ( !defined(_MSC_VER) || _MSC_VER>1200 )
	//! Return the absolute value of \p a
	template<typename T> inline T abs(const T& a) { return a>=0?a:-a; }
	inline bool abs(const bool a) { return a; }
	inline unsigned char abs(const unsigned char a) { return a; }
	inline unsigned short abs(const unsigned short a) { return a; }
	inline unsigned int abs(const unsigned int a) { return a; }
	inline unsigned long abs(const unsigned long a) { return a; }
	inline double abs(const double a) { return std::fabs(a); }
	inline float abs(const float a)   { return (float)std::fabs((double)a); }
	inline int abs(const int a)       { return std::abs(a); }
	
	//! Return the minimum between \p a and \p b.
	template<typename T> inline const T& min(const T& a,const T& b) { return a<=b?a:b; }

	//! Return the minimum between \p a,\p b and \a c.
	template<typename T> inline const T& min(const T& a,const T& b,const T& c) { return cimg::min(cimg::min(a,b),c); }

	//! Return the minimum between \p a,\p b,\p c and \p d.
	template<typename T> inline const T& min(const T& a,const T& b,const T& c,const T& d) { return cimg::min(cimg::min(a,b,c),d); }

	//! Return the maximum between \p a and \p b.
	template<typename T> inline const T& max(const T& a,const T& b) { return a>=b?a:b; }

	//! Return the maximum between \p a,\p b and \p c.
	template<typename T> inline const T& max(const T& a,const T& b,const T& c) { return cimg::max(cimg::max(a,b),c); }

	//! Return the maximum between \p a,\p b,\p c and \p d.
	template<typename T> inline const T& max(const T& a,const T& b,const T& c,const T& d) { return cimg::max(cimg::max(a,b,c),d); }

	//! Return the sign of \p x.
	template<typename T> inline T sign(const T& x) { return (x<0)?(T)(-1):(x==0?(T)0:(T)1); }
#else
	
	// Special versions due to object reference bug in VisualC++ 6.0.
	template<typename T> inline const T abs(const T a) { return a>=0?a:-a; }
	template<typename T> inline const T min(const T a,const T b) { return a<=b?a:b; }
	template<typename T> inline const T min(const T a,const T b,const T c) { return cimg::min(cimg::min(a,b),c); }
	template<typename T> inline const T min(const T a,const T b,const T c,const T& d) { return cimg::min(cimg::min(a,b,c),d); }
	template<typename T> inline const T max(const T a,const T b) { return a>=b?a:b; }
	template<typename T> inline const T max(const T a,const T b,const T c) { return cimg::max(cimg::max(a,b),c); }
	template<typename T> inline const T max(const T a,const T b,const T c,const T& d) { return cimg::max(cimg::max(a,b,c),d); }
	template<typename T> inline char sign(const T x) { return (x<0)?-1:(x==0?0:1); }
#endif

	//! Return the nearest power of 2 higher than \p x.
	template<typename T> inline unsigned long nearest_pow2(const T& x) {
	  unsigned long i=1;
	  while (x>i) i<<=1;
	  return i;
	}

	//! Return \p x modulo \p m (generic modulo).
	/**
	   This modulo function accepts negative and floating-points modulo numbers \p m.
	**/
	inline double mod(const double& x,const double& m) { return x-m*std::floor(x/m); }
	inline float  mod(const float& x,const float& m)   { return (float)(x-m*std::floor((double)x/m)); }
	inline int    mod(const int x,const int m)         { return x>=0?x%m:(x%m?m+x%m:0); }

	//! Return minmod(\p a,\p b).
	/**
	   The operator minmod(\p a,\p b) is defined to be :
	   - minmod(\p a,\p b) = min(\p a,\p b), if (\p a * \p b)>0.
	   - minmod(\p a,\p b) = 0,              if (\p a * \p b)<=0
	**/
	template<typename T> inline T minmod(const T& a,const T& b) { return a*b<=0?0:(a>0?(a<b?a:b):(a<b?b:a)); }

	//! Return a random variable between [0,1], followin a uniform distribution.
	inline double rand() { return (double)std::rand()/RAND_MAX; }

	//! Return a random variable between [-1,1], following a uniform distribution.
	inline double crand() { return 1-2*cimg::rand(); }

	//! Return a random variable following a gaussian distribution and a standard deviation of 1.
	inline double grand() {
	  return std::sqrt(-2*std::log((double)(1e-10 + (1-2e-10)*cimg::rand())))*std::cos((double)(2*PI*cimg::rand())); 
	}

	inline double pythagore(double a, double b) {
	  const double absa = cimg::abs(a), absb = cimg::abs(b);
	  if (absa>absb) { const double tmp = absb/absa; return absa*std::sqrt(1.0+tmp*tmp); }
	  else { const double tmp = absa/absb; return (absb==0?0:absb*std::sqrt(1.0+tmp*tmp)); }
	}
	
	// End of the 'cimg' namespace
  }
  
  /*
   #----------------------------------------
   #
   #
   #
   # Definition of the CImgStats structure
   #
   #
   #
   #----------------------------------------
   */
  //! Class used to compute basic statistics on pixel values of a \ref CImg<T> image.
  /** 
	  Constructing a CImgStats instance from an image CImg<T> or a list CImgl<T>
	  will compute the minimum, maximum and average pixel values of the input object.
	  Optionally, the variance of the pixel values can be computed.
	  Coordinates of the pixels whose values are minimum and maximum are also stored.
	  The example below shows how to use CImgStats objects to retrieve simple statistics of an image :
	  \code 
	  const CImg<float> img("my_image.jpg");                 // Read JPEG image file.
	  const CImgStats stats(img);                            // Compute basic statistics on the image.
	  stats.print("My statistics");                          // Display statistics.
	  std::printf("Max-Min = %lf",stats.max-stats.min);      // Compute the difference between extremum values.
	  \endcode

	  Note that statistics are computed by considering the set of \a scalar values of the image pixels.
	  No vector-valued statistics are computed.
  **/
  struct CImgStats {
	double min;                 //!< Minimum of the pixel values.
	double max;                 //!< Maximum of the pixel values.
	double mean;                //!< Mean of the pixel values.
	double variance;            //!< Variance of the pixel values.
	int xmin;                   //!< X-coordinate of the pixel with minimum value.
	int ymin;                   //!< Y-coordinate of the pixel with minimum value.
	int zmin;                   //!< Z-coordinate of the pixel with minimum value.
	int vmin;                   //!< V-coordinate of the pixel with minimum value.
	int lmin;                   //!< Image number (for a list) containing the minimum pixel.
	int xmax;                   //!< X-coordinate of the pixel with maximum value.
	int ymax;                   //!< Y-coordinate of the pixel with maximum value.
	int zmax;                   //!< Z-coordinate of the pixel with maximum value.
	int vmax;                   //!< V-coordinate of the pixel with maximum value.
	int lmax;                   //!< Image number (for a list) containing the maximum pixel.
	
	//! Empty constructor.
	CImgStats():min(0),max(0),mean(0),variance(0),xmin(-1),ymin(-1),zmin(-1),vmin(-1),lmin(-1),
		xmax(-1),ymax(-1),zmax(-1),vmax(-1),lmax(-1) {}

	//! Copy constructor.
	CImgStats(const CImgStats& stats) {
	  assign(stats);
	};

	//! Constructor that computes statistics of an input image \p img.
	/** 
	\param img The input image.
	\param compute_variance If true, the \c variance field is computed, else it is set to 0.
	**/
	template<typename T> CImgStats(const CImg<T>& img, const bool compute_variance=true) {
	  assign(img,compute_variance);
	}
	  
	//! Constructor that computes statistics of an input image list \p list.
	/**
	   \param list The input list of images.
	   \param compute_variance If true, the \c variance field is computed, else it is set to 0.
	**/
	template<typename T> CImgStats(const CImgl<T>& list, const bool compute_variance=true) {
	  assign(list,compute_variance);
	}

	//! Assignement operator.
	CImgStats& operator=(const CImgStats& stats) {
	  return assign(stats);
	}

	//! In-place version of the corresponding constructor.
	CImgStats& assign(const CImgStats& stats) {
	  min = stats.min;
	  max = stats.max;
	  mean = stats.mean;
	  variance = stats.variance;
	  xmin = stats.xmin; ymin = stats.ymin; zmin = stats.zmin; vmin = stats.vmin; lmin = stats.lmin;
	  xmax = stats.xmax; ymax = stats.ymax; zmax = stats.zmax; vmax = stats.vmax; lmax = stats.lmax;
	  return *this;
	}

	//! In-place version of the corresponding constructor.
	template<typename T> CImgStats& assign(const CImg<T>& img, const bool compute_variance=true) {
	  if (img.is_empty())
	throw CImgArgumentException("CImgStats::CImgStats() : Specified input image (%u,%u,%u,%u,%p) is empty.",
					img.width,img.height,img.depth,img.dim,img.data);
	  mean = variance = 0;
	  lmin = lmax = -1;
	  T pmin=img[0], pmax=pmin, *ptrmin=img.data, *ptrmax=ptrmin;
	  cimg_map(img,ptr,T) {
	const T& a=*ptr;
	mean+=(double)a;
	if (a<pmin) { pmin=a; ptrmin = ptr; }
	if (a>pmax) { pmax=a; ptrmax = ptr; }
	  }
	  mean/=img.size();
	  min=(double)pmin;
	  max=(double)pmax;
	  unsigned long offmin = (unsigned long)(ptrmin-img.data), offmax = (unsigned long)(ptrmax-img.data);
	  const unsigned long whz = img.width*img.height*img.depth, wh = img.width*img.height;      
	  vmin = offmin/whz; offmin%=whz; zmin = offmin/wh; offmin%=wh; ymin = offmin/img.width; xmin = offmin%img.width;
	  vmax = offmax/whz; offmax%=whz; zmax = offmax/wh; offmax%=wh; ymax = offmax/img.width; xmax = offmax%img.width;
	  if (compute_variance) {
		cimg_map(img,ptr,T) { const double tmpf=(*ptr)-mean; variance+=tmpf*tmpf; }
	const unsigned int siz = img.size();
		if (siz>1) variance/=(siz-1); else variance=0;
	  }
	  return *this;
	}

	//! In-place version of the corresponding constructor.
	template<typename T> CImgStats& assign(const CImgl<T>& list,const bool compute_variance=true) {
	  if (list.is_empty())
	throw CImgArgumentException("CImgStats::CImgStats() : Specified input list (%u,%p) is empty.",
					list.size,list.data);
	  mean = variance = 0;
	  T pmin = list[0][0], pmax = pmin, *ptrmin = list[0].data, *ptrmax = ptrmin;
	  int psize = 0;
	  cimgl_map(list,l) {
		cimg_map(list[l],ptr,T) {
		  const T& a=*ptr;
		  mean+=(double)a;
		  if (a<pmin) { pmin=a; ptrmin = ptr; lmin = l; }
		  if (a>pmax) { pmax=a; ptrmax = ptr; lmax = l; }
		}
		psize+=list[l].size();
	  }
	  mean/=psize;
	  min=(double)pmin;
	  max=(double)pmax;
	  const CImg<T> &imin = list[lmin], &imax = list[lmax];
	  unsigned long offmin = (ptrmin-imin.data), offmax = (ptrmax-imax.data);
	  const unsigned long whz1 = imin.width*imin.height*imin.depth, wh1 = imin.width*imin.height;
	  vmin = offmin/whz1; offmin%=whz1; zmin = offmin/wh1; offmin%=wh1; ymin = offmin/imin.width; xmin = offmin%imin.width;
	  const unsigned long whz2 = imax.width*imax.height*imax.depth, wh2 = imax.width*imax.height;
	  vmax = offmax/whz2; offmax%=whz2; zmax = offmax/wh2; offmax%=wh2; ymax = offmax/imax.width; xmax = offmax%imax.width;
	  if (compute_variance) {
		cimgl_map(list,l) cimg_map(list[l],ptr,T) { const double tmpf=(*ptr)-mean; variance+=tmpf*tmpf; }
	if (psize>1) variance/=(psize-1); else variance=0;
	  }
	  return *this;
	}

	//! Print the current statistics.
	/**
	   Printing is done on the standart error output.
	**/
	const CImgStats& print(const char* title=0) const {
	  if (lmin>=0 && lmax>=0)
	std::fprintf(stderr,"%-8s(this=%p) : { min=%g, mean=%g [var=%g], max=%g, "
			 "pmin=[%d](%d,%d,%d,%d), pmax=[%d](%d,%d,%d,%d) }\n",
			 title?title:"CImgStats",(void*)this,min,mean,variance,max,
			 lmin,xmin,ymin,zmin,vmin,lmax,xmax,ymax,zmax,vmax);
	  else
	std::fprintf(stderr,"%-8s(this=%p) : { min=%g, mean=%g [var=%g], max=%g, "
			 "pmin=(%d,%d,%d,%d), pmax=(%d,%d,%d,%d) }\n",
			 title?title:"CImgStats",(void*)this,min,mean,variance,max,
			 xmin,ymin,zmin,vmin,xmax,ymax,zmax,vmax);
	  return *this;
	}

#ifdef cimgstats_plugin
#include cimgstats_plugin
#endif

  };

  /*
   #-------------------------------------------
   #
   #
   #
   # Definition of the CImgDisplay structure
   #
   #
   #
   #-------------------------------------------
   */

  //! This class represents a window which can display \ref CImg<T> images and handles mouse and keyboard events.
  /**
	 Creating a \c CImgDisplay instance opens a window that can be used to display a \c CImg<T> image
	 of a \c CImgl<T> image list inside. When a display is created, associated window events
	 (such as mouse motion, keyboard and window size changes) are handled and can be easily
	 detected by testing specific \c CImgDisplay data fields.
	 See \ref cimg_displays for a complete tutorial on using the \c CImgDisplay class.
  **/

  struct CImgDisplay {
	
	//! Width of the display
	unsigned int width;
	
	//! Height of the display
	unsigned int height;

	//! Normalization type used for the display
	unsigned int normalization;

	//! Range of events detected by the display
	unsigned int events;

	//! Fullscreen state of the display
	bool is_fullscreen;

	//! Display title
	char* title;
	
	//! X-pos of the display on the screen
	volatile int window_x;
	
	//! Y-pos of the display on the screen
	volatile int window_y;

	//! Width of the underlying window
	volatile unsigned int window_width;

	//! Height of the underlying window
	volatile unsigned int window_height;

	//! X-coordinate of the mouse pointer on the display
	volatile int mouse_x;
	
	//! Y-coordinate of the mouse pointer on the display
	volatile int mouse_y;

	//! Button state of the mouse
	volatile unsigned int button;

	//! Wheel state of the mouse (Linux only)
	volatile int wheel;

	//! Key value if pressed
	volatile unsigned int key;

	//! Closed state of the window
	volatile bool is_closed;
	
	//! Resized state of the window
	volatile bool is_resized;
	
	//! Moved state of the window
	volatile bool is_moved;
	
	//! Event state of the window
	volatile bool is_event;
	
	double min,max;
	unsigned long timer, fps_frames, fps_timer;
	float fps_fps;

	//! Create a display window with a specified size \p pwidth x \p height.
	/** \param dimw       : Width of the display window.
		\param dimh       : Height of the display window.
		\param title      : Title of the display window.
		\param normalization_type  : Normalization type of the display window (see CImgDisplay::normalize).
	\param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
		A black image will be initially displayed in the display window.
	**/
	CImgDisplay(const unsigned int dimw, const unsigned int dimh, const char *title=0,
				const unsigned int normalization_type=1, const unsigned int events_type=3,
				const bool fullscreen_flag=false, const bool closed_flag=false):
	  width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
	  window_x(0),window_y(0),window_width(0),window_height(0),
	  mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
	  min(0),max(0) {
	  assign(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	}

	//! Create a display window from an image.
	/** \param img : Image that will be used to create the display window.
		\param title : Title of the display window
		\param normalization_type : Normalization type of the display window.
		\param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
	**/
	template<typename T> 
	CImgDisplay(const CImg<T>& img,const char *title=0,
				const unsigned int normalization_type=1,const unsigned int events_type=3,
				const bool fullscreen_flag=false,const bool closed_flag=false):
	  width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
	  window_x(0),window_y(0),window_width(0),window_height(0),
	  mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
	  min(0),max(0) {
	  assign(img,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	}
   
	//! Create a display window from an image list.
	/** \param list : The list of images to display.
		\param title : Title of the display window
		\param normalization_type : Normalization type of the display window.
	\param events_type : Type of events handled by the display window.
	\param fullscreen_flag : Fullscreen mode.
	\param closed_flag : Initially visible mode.      
	**/
	template<typename T> 
	CImgDisplay(const CImgl<T>& list,const char *title=0,
				const unsigned int normalization_type=1,const unsigned int events_type=3,
				const bool fullscreen_flag=false,const bool closed_flag=false):
	  width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
	  window_x(0),window_y(0),window_width(0),window_height(0),
	  mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
	  min(0),max(0) {
	  assign(list,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	}

	//! Create a display window by copying another one.
	/** \param win   : Display window to copy.
		\param title : Title of the new display window.
	**/
	CImgDisplay(const CImgDisplay& disp):
	  width(0),height(0),normalization(0),events(0),is_fullscreen(false),title(0),
	  window_x(0),window_y(0),window_width(0),window_height(0),
	  mouse_x(0),mouse_y(0),button(0),wheel(0),key(0),is_closed(true),is_resized(false),is_moved(false),is_event(false),
	  min(0),max(0) {
	  assign(disp);
	}

	//! Destructor
	~CImgDisplay() { 
	  empty(); 
	}

	//! Assignement operator
	CImgDisplay& operator=(const CImgDisplay& disp) {
	  return assign(disp);
	}

	//! Return display width
	int dimx() const { 
	  return (int)width; 
	}

	//! Return display height
	int dimy() const {
	  return (int)height; 
	}

	//! Return display window width
	int window_dimx() const {
	  return (int)window_width; 
	}

	//! Return display window height
	int window_dimy() const {
	  return (int)window_height; 
	}   
	
	//! Return X-coordinate of the window
	int window_posx() const {
	  return window_x;
	}
	
	//! Return Y-coordinate of the window
	int window_posy() const {
	  return window_y;
	}
	
	//! Synchronized waiting function. Same as cimg::wait().
	/** \see cimg::wait()
	 **/
	CImgDisplay& wait(const unsigned int milliseconds) { 
	  cimg::wait(milliseconds, timer);
	  return *this; 
	}

	//! Wait for an event
	CImgDisplay& wait() {
	  wait(*this);
	  return *this;
	}

	static void wait(CImgDisplay& disp1) {
	  disp1.is_event = 0;
	  while (!disp1.is_event) wait_all();
	}

	static void wait(CImgDisplay& disp1, CImgDisplay& disp2) {
	  disp1.is_event = disp2.is_event = 0;
	  while (!disp1.is_event && !disp2.is_event) wait_all();
	}

	static void wait(CImgDisplay& disp1, CImgDisplay& disp2, CImgDisplay& disp3) {
	  disp1.is_event = disp2.is_event = disp3.is_event = 0;
	  while (!disp1.is_event && !disp2.is_event && !disp3.is_event) wait_all();
	}

	static void wait(CImgDisplay& disp1, CImgDisplay& disp2, CImgDisplay& disp3, CImgDisplay& disp4) {
	  disp1.is_event = disp2.is_event = disp3.is_event = disp4.is_event = 0;
	  while (!disp1.is_event && !disp2.is_event && !disp3.is_event && !disp4.is_event) wait_all();
	}

	//! Return the frame per second rate
	float frames_per_second() {
	  if (!fps_timer) fps_timer = cimg::time();
	  const float delta = (cimg::time()-fps_timer)/1000.0f;     
	  fps_frames++;
	  if (delta>=1.0f) {
	fps_fps = fps_frames/delta;
	fps_frames = 0;
	fps_timer = cimg::time();
	  }
	  return fps_fps;
	}
	
	//! Display an image list CImgl<T> into a display window.
	/** First, all images of the list are appended into a single image used for visualization,
		then this image is displayed in the current display window.
		\param list     : The list of images to display.
		\param axe      : The axe used to append the image for visualization. Can be 'x' (default),'y','z' or 'v'.
		\param align : Defines the relative alignment of images when displaying images of different sizes.
		Can be '\p c' (centered, which is the default), '\p p' (top alignment) and '\p n' (bottom aligment).

		\see CImg::get_append()
	**/
	template<typename T> CImgDisplay& display(const CImgl<T>& list,const char axe='x',const char align='c') { 
	  return display(list.get_append(axe,align)); 
	} 

	//! Resize a display window with the size of an image.
	/** \param img    : Input image. \p image.width and \p image.height give the new dimensions of the display window.
		\param redraw : If \p true (default), the current displayed image in the display window will
		be bloc-interpolated to fit the new dimensions. If \p false, a black image will be drawn in the resized window.
		\see CImgDisplay::is_resized, CImgDisplay::resizedimx(), CImgDisplay::resizedimy()
	**/
	template<typename T> CImgDisplay& resize(const CImg<T>& img, const bool redraw=true) { 
	  return resize(img.width,img.height,redraw); 
	}

	//! Resize a display window using the size of the given display \p disp
	CImgDisplay& resize(const CImgDisplay& disp, const bool redraw=true) {
	  return resize(disp.width,disp.height,redraw);
	}

	//! Resize a display window in its current size.
	CImgDisplay& resize(const bool redraw=true) {
	  resize(window_width,window_height,redraw);
	  return *this;
	}

	//! Display a 3d object
	template<typename tp, typename tf, typename T, typename to>
	CImgDisplay& display_object3d(const tp& points, const CImgl<tf>& primitives,
				  const CImgl<T>& colors, const CImg<to>& opacities,
				  const bool centering=true,
				  const int render_static=4, const int render_motion=1,
				  const bool double_sided=false,
				  const float focale=500.0f, const float ambiant_light=0.05f,
				  const bool display_axes = true, float *const pose_matrix = 0) {
	  CImg<T>(width,height,1,3,0).display_object3d(points,primitives,colors,opacities,*this,
						   centering,render_static,render_motion,
						   double_sided,focale,ambiant_light,display_axes,pose_matrix);
	  return *this;
	} 

	//! Display a 3d object
	template<typename tp, typename tf, typename T, typename to>
	CImgDisplay& display_object3d(const tp& points, const CImgl<tf>& primitives,
				  const CImgl<T>& colors, const CImgl<to>& opacities,
				  const bool centering=true,
				  const int render_static=4, const int render_motion=1,
				  const bool double_sided=false,
				  const float focale=500.0f, const float ambiant_light=0.05f,
				  const bool display_axes = true, float *const pose_matrix = 0) {
	  CImg<T>(width,height,1,3,0).display_object3d(points,primitives,colors,opacities,*this,
						   centering,render_static,render_motion,
						   double_sided,focale,ambiant_light,display_axes,pose_matrix);
	  return *this;
	} 

	//! Display a 3D object.
	template<typename tp, typename tf, typename T>
	CImgDisplay& display_object3d(const tp& points, const CImgl<tf>& primitives,
				  const CImgl<T>& colors,
				  const bool centering=true,
				  const int render_static=4, const int render_motion=1,
				  const bool double_sided=false,
				  const float focale=500.0f, const float ambiant_light=0.05f,
				  const float opacity=1.0f, const bool display_axes = true, float *const pose_matrix = 0) {
	  typedef typename cimg::largest<tp,float>::type to;
	  CImg<T>(width,height,1,3,0).display_object3d(points,primitives,colors,
						   CImg<to>(primitives.size)=(to)opacity,*this,
						   centering,render_static,render_motion,
						   double_sided,focale,ambiant_light,display_axes,pose_matrix);
	  return *this;
	} 

	CImgDisplay& toggle_fullscreen() {
	  return assign(width,height,title,normalization,events,!is_fullscreen,is_closed);
	}

	// Inner routine used for fast resizing of buffer to display size.
	template<typename t, typename T> static void _render_resize(const T *ptrs, const unsigned int ws, const unsigned int hs,
								t *ptrd, const unsigned int wd, const unsigned int hd) {
	  unsigned int *const offx = new unsigned int[wd], *const offy = new unsigned int[hd+1], *poffx, *poffy;
	  float s, curr, old;
	  s = (float)ws/wd;
	  poffx = offx; curr=0; for (unsigned int x=0; x<wd; x++) { old=curr; curr+=s; *(poffx++) = (unsigned int)curr-(unsigned int)old; }
	  s = (float)hs/hd;
	  poffy = offy; curr=0; for (unsigned int y=0; y<hd; y++) { old=curr; curr+=s; *(poffy++) = ws*((unsigned int)curr-(unsigned int)old); }
	  *poffy=0;
	  poffy = offy;
	  {for (unsigned int y=0; y<hd; ) {
	const T *ptr = ptrs;
	poffx = offx;
	for (unsigned int x=0; x<wd; x++) { *(ptrd++)=*ptr; ptr+=*(poffx++); }
	y++;
	unsigned int dy=*(poffy++);
	for (;!dy && y<hd; std::memcpy(ptrd, ptrd-wd, sizeof(t)*wd), y++, ptrd+=wd, dy=*(poffy++));
	ptrs+=dy;
	  }}
	  delete[] offx; delete[] offy;
	}
	
	// When no display available
	//---------------------------
#if cimg_display_type==0

	//! Return the width of the screen resolution.
	static int screen_dimx() {
	  return 0; 
	}

	//! Return the height of the screen resolution.
	static int screen_dimy() { 
	  return 0; 
	}

	//! In-place version of the corresponding constructor
	CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
	  throw CImgDisplayException("CImgDisplay() : Display has been required but is not available (cimg_display_type=0)");
	  fps_timer = 0*(unsigned long)(dimw + dimh + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
	  return *this;
	}

	//! In-place version of the corresponding constructor
	template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  fps_timer = 0*(unsigned long)(img.width + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
	  return assign(0,0);
	}
	
	//! In-place version of the corresponding constructor
	template<typename T> CImgDisplay& assign(const CImgl<T>& list, const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  fps_timer = 0*(unsigned long)(list.size + title + normalization_type + events_type + (int)fullscreen_flag + (int)closed_flag);
	  return assign(0,0);
	}
	
	//! In-place version of the corresponding constructor
	CImgDisplay& assign(const CImgDisplay &disp) {
	  return assign(disp.width,disp.height);
	}

	// In-place version of the destructor (should not be used by the user).
	CImgDisplay& empty() {
	  return *this;
	}
	
	//! Display an image in a window.
	template<typename T> CImgDisplay& display(const CImg<T>& img) {
	  fps_timer = 0*img.width;
	  return *this; 
	}

	//! Resize window
	CImgDisplay& resize(const int width, const int height, const bool redraw=true) {
	  fps_timer = 0*width*height*(int)redraw;
	  return *this; 
	}

	//! Move window
	CImgDisplay& move(const int posx, const int posy) { 
	  fps_timer = 0*posx*posy;
	  return *this; 
	}

	//! Wait for a window event in any CImg window
	static void wait_all() {}

	//! Show a closed display
	CImgDisplay& show() {
	  return *this; 
	}

	//! Close a visible display
	CImgDisplay& close() { 
	  return *this; 
	}

	//! Set the window title
	CImgDisplay& set_title(const char *format,...) {
	  fps_timer = 0*(int)format;
	  return *this; 
	}

	//! Re-paint image content in window
	CImgDisplay& paint() {
	  return *this;
	}

	//! Render image buffer into GDI native image format
	template<typename T> CImgDisplay& render(const CImg<T>& img) {
	  fps_timer = 0*img.width;
	  return *this;
	}
	
	// X11-based display
	//-------------------
#elif cimg_display_type==1
	void *data;
	Window window;
	Window background_window;
	XImage *image;
	Colormap colormap;
	Atom wm_delete_window, wm_delete_protocol;
#ifdef cimg_use_xshm
	XShmSegmentInfo *shminfo;
#endif

	static int screen_dimx() { 
	  int res = 0;
	  if (!cimg::X11attr().display) {
	Display *disp = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
	if (!disp) throw CImgDisplayException("CImgDisplay::screen_dimx() : Can't open X11 display");
	res = DisplayWidth(disp,DefaultScreen(disp));
	XCloseDisplay(disp);
	  } else {
#ifdef cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution)
	  res = cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].width;
	else
#endif
	  res = DisplayWidth(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
	  }
	  return res;
	}

	static int screen_dimy() { 
	  int res = 0;
	  if (!cimg::X11attr().display) {
	Display *disp = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
	if (!disp) throw CImgDisplayException("CImgDisplay::screen_dimy() : Can't open X11 display");
	res = DisplayHeight(disp,DefaultScreen(disp));
	XCloseDisplay(disp);
	  } else {
#ifdef cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution)
	  res = cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].height; else
#endif
		res = DisplayHeight(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
	  }
	  return res;
	}
	
	CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (!dimw || !dimh) 
	throw CImgArgumentException("CImgDisplay::assign() : Specified window size (%u,%u) is not valid.",dimw,dimh);
	  assign_lowlevel(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  min = max = 0;
	  std::memset(data,0,(cimg::X11attr().nb_bits==8?sizeof(unsigned char):
			  (cimg::X11attr().nb_bits==16?sizeof(unsigned short):sizeof(unsigned int)))*width*height);
	  return paint();
	}
	
	template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input image (%u,%u,%u,%u,%p) is empty.",
					img.width,img.height,img.depth,img.dim,img.data);
	  CImg<T> tmp;
	  const CImg<T>& nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
	  assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
	  return render(nimg).paint();
	}
	
	template<typename T> CImgDisplay& assign(const CImgl<T>& list, const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (list.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input list (%u,%p) is empty.",
					list.size,list.data);
	  CImg<T> tmp;
	  const CImg<T> img = list.get_append('x'), 
	&nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
	  assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
	  return render(nimg).paint();
	}
	
	CImgDisplay& assign(const CImgDisplay& win) {
	  assign_lowlevel(win.width,win.height,win.title,win.normalization,win.events,win.is_fullscreen,win.is_closed);
	  std::memcpy(data,win.data,(cimg::X11attr().nb_bits==8?sizeof(unsigned char):
				 cimg::X11attr().nb_bits==16?sizeof(unsigned short):
				 sizeof(unsigned int))*width*height);
	  return paint();
	}

	CImgDisplay& empty() {
	  if (width && height) {
	pthread_mutex_lock(cimg::X11attr().mutex);
	
	// Remove display window from event thread list
	unsigned int i;
	for (i=0; i<cimg::X11attr().nb_wins && cimg::X11attr().wins[i]!=this; i++);
	for (; i<cimg::X11attr().nb_wins-1; i++) cimg::X11attr().wins[i]=cimg::X11attr().wins[i+1];
	cimg::X11attr().nb_wins--;
	
	// Destroy window, image, colormap and title
	if (is_fullscreen) _desinit_fullscreen();
	XDestroyWindow(cimg::X11attr().display,window);
	window = 0;
#ifdef cimg_use_xshm
	if (shminfo) {
	  XShmDetach(cimg::X11attr().display, shminfo);
	  XDestroyImage(image);
	  shmdt(shminfo->shmaddr);
	  shmctl(shminfo->shmid,IPC_RMID,0);
	  delete shminfo;
	  shminfo = 0;
	} else
#endif
	  XDestroyImage(image);
	data = 0;
	image = 0;
	if (cimg::X11attr().nb_bits==8) XFreeColormap(cimg::X11attr().display,colormap);
	colormap = 0;
	XSync(cimg::X11attr().display, False);

	// Reset display variables
	if (title) delete[] title;
	width = height = normalization = events = 0;
	is_fullscreen = is_resized = is_moved = is_event = false;
	is_closed = true;
	title = 0;
	window_x = window_y = window_width = window_height = mouse_x = mouse_y = button = wheel = key = 0;
	min = max = 0;
	
	// End event thread and close display if necessary
	if (!cimg::X11attr().nb_wins) {
	  
	  // Kill event thread
	  pthread_cancel(*cimg::X11attr().event_thread);
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  pthread_join(*cimg::X11attr().event_thread,0);
	  delete cimg::X11attr().event_thread;
	  cimg::X11attr().event_thread = 0;
	  pthread_mutex_destroy(cimg::X11attr().mutex);
	  delete cimg::X11attr().mutex;
	  cimg::X11attr().mutex = 0;	  
	  XSync(cimg::X11attr().display, False);
	  XCloseDisplay(cimg::X11attr().display);
	  cimg::X11attr().display=0;
	  delete cimg::X11attr().gc;
	  cimg::X11attr().gc = 0;
	} else pthread_mutex_unlock(cimg::X11attr().mutex);
	  }
	  return *this;
	}
	
	template<typename T> CImgDisplay& display(const CImg<T>& img) {
	  return render(img).paint(false);
	}
	
	CImgDisplay& resize(const int nwidth, const int nheight, const bool redraw=true) {
	  if (!(nwidth && nheight))
	throw CImgArgumentException("CImgDisplay::resize() : Specified window size (%d,%d) is not valid.",
					nwidth,nheight);
	  const unsigned int
	tmpdimx=(nwidth>0)?nwidth:(-nwidth*width/100),
	tmpdimy=(nheight>0)?nheight:(-nheight*height/100),
	dimx = tmpdimx?tmpdimx:1,
	dimy = tmpdimy?tmpdimy:1;
	  const bool
	is_disp_different = (width!=dimx || height!=dimy),
	is_win_different = (window_width!=dimx || window_height!=dimy);
	  pthread_mutex_lock(cimg::X11attr().mutex);
	  if (is_disp_different || is_win_different) {
	XResizeWindow(cimg::X11attr().display,window,dimx,dimy);
	window_width = dimx;
	window_height = dimy;
	is_resized = false;
	if (is_disp_different) {
	  switch (cimg::X11attr().nb_bits) {
	  case 8:  { unsigned char foo=0; _resize(foo,dimx,dimy,redraw);  } break;
	  case 16: { unsigned short foo=0; _resize(foo,dimx,dimy,redraw); } break;
	  default: { unsigned int foo=0; _resize(foo,dimx,dimy,redraw);   } break;
	  }
	  width = dimx;
	  height = dimy;
	}
	  }      
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  if (is_fullscreen) move((screen_dimx()-width)/2,(screen_dimy()-height)/2);
	  return paint();
	}
	
	CImgDisplay& move(const int posx, const int posy) {
	  show();
	  pthread_mutex_lock(cimg::X11attr().mutex);      
	  XMoveWindow(cimg::X11attr().display,window,posx,posy);
	  is_moved = false;
	  window_x = posx;
	  window_y = posy;
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  return paint();
	}
	
	static void wait_all() {
	  pthread_mutex_lock(cimg::X11attr().mutex);
	  XEvent event;
	  bool flag = true;
	  while (flag) {
	for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++) {
	  cimg::X11attr().wins[i]->is_event = false;
	  const unsigned int xevent_type = (cimg::X11attr().wins[i]->events)&3;
	  const unsigned int emask =
		((xevent_type>=1)?ExposureMask|StructureNotifyMask:0)|
		((xevent_type>=2)?ButtonPressMask|KeyPressMask|PointerMotionMask|LeaveWindowMask:0)|
		((xevent_type>=3)?ButtonReleaseMask|KeyReleaseMask:0);
	  XSelectInput(cimg::X11attr().display,cimg::X11attr().wins[i]->window,emask);
	}
	bool event_flag = XCheckTypedEvent(cimg::X11attr().display, ClientMessage, &event);
	if (!event_flag) event_flag = XCheckMaskEvent(cimg::X11attr().display,
							  ExposureMask|StructureNotifyMask|ButtonPressMask|
							  KeyPressMask|PointerMotionMask|LeaveWindowMask|ButtonReleaseMask|
							  KeyReleaseMask,&event);
	if (event_flag) {
	  for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++)
		if (!cimg::X11attr().wins[i]->is_closed && event.xany.window==cimg::X11attr().wins[i]->window) {
		  cimg::X11attr().wins[i]->_handle_events(&event);
		  if (cimg::X11attr().wins[i]->is_event) flag = false;
		}
	}
	cimg::sleep(7);
	  }
	  pthread_mutex_unlock(cimg::X11attr().mutex);	
	}
	
	CImgDisplay& show() {
	  if (is_closed) {
		pthread_mutex_lock(cimg::X11attr().mutex);
	if (is_fullscreen) _init_fullscreen();
	_map_window();
		is_closed = false;
		pthread_mutex_unlock(cimg::X11attr().mutex);
	  }
	  return paint();	
	}

	CImgDisplay& close() {
	  if (!is_closed) {
		pthread_mutex_lock(cimg::X11attr().mutex);
	if (is_fullscreen) _desinit_fullscreen();
		XUnmapWindow(cimg::X11attr().display,window);
	window_x = window_y = -1;
		is_closed = true;
		pthread_mutex_unlock(cimg::X11attr().mutex);
	  }
	  return *this;
	}

	CImgDisplay& set_title(const char *format,...) { 
	  char tmp[1024]={0}; 
	  va_list ap; 
	  va_start(ap, format); 
	  std::vsprintf(tmp,format,ap); 
	  va_end(ap);
	  if (title) delete[] title;
	  const int s = cimg::strlen(tmp)+1;
	  title = new char[s];
	  std::memcpy(title,tmp,s*sizeof(char));
	  pthread_mutex_lock(cimg::X11attr().mutex);
	  XStoreName(cimg::X11attr().display,window,tmp);
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  return *this; 
	}

	CImgDisplay& paint(const bool wait_expose=true) {      
	  pthread_mutex_lock(cimg::X11attr().mutex);
	  _paint(wait_expose);
	  pthread_mutex_unlock(cimg::X11attr().mutex);	  
	  return *this;
	}

	template<typename T> CImgDisplay& render(const CImg<T>& img, const bool flag8=false) {
	  if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::_render_image() : Specified input image (%u,%u,%u,%u,%p) is empty.",
					img.width,img.height,img.depth,img.dim,img.data);
	  if (img.depth!=1) return render(img.get_projections2d(img.width/2,img.height/2,img.depth/2)); 
	  if (cimg::X11attr().nb_bits==8 && (img.width!=width || img.height!=height)) return render(img.get_resize(width,height,1,-100,1));
	  if (cimg::X11attr().nb_bits==8 && !flag8 && img.dim==3) return render(img.get_RGBtoLUT(true),true);

	  const unsigned int xymax = img.width*img.height;
	  const T 
	*data1 = img.ptr(),
	*data2 = (img.dim>=2)?img.ptr(0,0,0,1):data1,
	*data3 = (img.dim>=3)?img.ptr(0,0,0,2):data1;
	  if (cimg::X11attr().blue_first) cimg::swap(data1,data3);
	  pthread_mutex_lock(cimg::X11attr().mutex);
	  
	  if (!normalization) {
	switch (cimg::X11attr().nb_bits) {
	case 8: {
	  _set_colormap(colormap,img.dim);
	  unsigned char *const ndata = (img.width==width && img.height==height)?(unsigned char*)data:new unsigned char[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  switch (img.dim) {
	  case 1: for (unsigned int xy=0; xy<xymax; xy++) (*ptrd++) = (unsigned char)*(data1++);
		break;
	  case 2: for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char R = (unsigned char)*(data1++), G = (unsigned char)*(data2++);
		(*ptrd++) = (R&0xf0)|(G>>4);
	  } break;
	  default: for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char R = (unsigned char)*(data1++), G = (unsigned char)*(data2++), B = (unsigned char)*(data3++);
		(*ptrd++) = (R&0xe0) | ((G>>5)<<2) | (B>>6);
	  } break;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned char*)data,width,height); delete[] ndata; }	
	} break;
	case 16: {
	  unsigned short *const ndata = (img.width==width && img.height==height)?(unsigned short*)data:new unsigned short[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  const unsigned int M = 248;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char G = (unsigned char)*(data2++)>>2;
		*(ptrd++) = (unsigned char)*(data1++)&M | (G>>3);
		*(ptrd++) = (G<<5) | ((unsigned char)*(data3++)>>3);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char G = (unsigned char)*(data2++)>>2;
		*(ptrd++) = (G<<5) | ((unsigned char)*(data3++)>>3);
		*(ptrd++) = (unsigned char)*(data1++)&M | (G>>3);
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned short*)data,width,height); delete[] ndata; } 
	} break;
	default: {
	  unsigned int *const ndata = (img.width==width && img.height==height)?(unsigned int*)data:new unsigned int[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
		*(ptrd++) = 0;
		*(ptrd++) = (unsigned char)*(data1++);
		*(ptrd++) = (unsigned char)*(data2++);
		*(ptrd++) = (unsigned char)*(data3++);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
		*(ptrd++) = (unsigned char)*(data3++);
		*(ptrd++) = (unsigned char)*(data2++);
		*(ptrd++) = (unsigned char)*(data1++);
		*(ptrd++) = 0;
	  }	      
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned int*)data,width,height); delete[] ndata; } 
	} break;
	};
	  } else {
	if ((min>max) || normalization==1) { const CImgStats st(img,false); min=st.min; max=st.max; }
	const T nmin = (T)min, delta = (T)max-nmin, mm=delta?delta:(T)1;
	switch (cimg::X11attr().nb_bits) {
	case 8: {
	  _set_colormap(colormap,img.dim);
	  unsigned char *const ndata = (img.width==width && img.height==height)?(unsigned char*)data:new unsigned char[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  switch (img.dim) {
	  case 1: for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char R = (unsigned char)(255*(*(data1++)-nmin)/mm);
		*(ptrd++) = R;
	  } break;
	  case 2: for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char
		  R = (unsigned char)(255*(*(data1++)-nmin)/mm),
		  G = (unsigned char)(255*(*(data2++)-nmin)/mm);
		(*ptrd++) = (R&0xf0) | (G>>4);
	  } break;
	  default:
		for (unsigned int xy=0; xy<xymax; xy++) {
		  const unsigned char
		R = (unsigned char)(255*(*(data1++)-nmin)/mm),
		G = (unsigned char)(255*(*(data2++)-nmin)/mm),
		B = (unsigned char)(255*(*(data3++)-nmin)/mm);
		  *(ptrd++) = (R&0xe0) | ((G>>5)<<2) | (B>>6);
		} break;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned char*)data,width,height); delete[] ndata; } 
	} break;
	case 16: {
	  unsigned short *const ndata = (img.width==width && img.height==height)?(unsigned short*)data:new unsigned short[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  const unsigned int M = 248;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char G = (unsigned char)(255*(*(data2++)-nmin)/mm)>>2;
		*(ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm)&M | (G>>3);
		*(ptrd++) = (G<<5) | ((unsigned char)(255*(*(data3++)-nmin)/mm)>>3);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
		const unsigned char G = (unsigned char)(255*(*(data2++)-nmin)/mm)>>2;
		*(ptrd++) = (G<<5) | ((unsigned char)(255*(*(data3++)-nmin)/mm)>>3);
		*(ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm)&M | (G>>3);
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned short*)data,width,height); delete[] ndata; } 
	} break;
	default: {
	  unsigned int *const ndata = (img.width==width && img.height==height)?(unsigned int*)data:new unsigned int[img.width*img.height];
	  unsigned char *ptrd = (unsigned char*)ndata;
	  if (cimg::X11attr().byte_order) for (unsigned int xy=0; xy<xymax; xy++) {
		(*ptrd++) = 0;
		(*ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm);
		(*ptrd++) = (unsigned char)(255*(*(data2++)-nmin)/mm);
		(*ptrd++) = (unsigned char)(255*(*(data3++)-nmin)/mm);
	  } else for (unsigned int xy=0; xy<xymax; xy++) {
		(*ptrd++) = (unsigned char)(255*(*(data3++)-nmin)/mm);
		(*ptrd++) = (unsigned char)(255*(*(data2++)-nmin)/mm);
		(*ptrd++) = (unsigned char)(255*(*(data1++)-nmin)/mm);
		(*ptrd++) = 0;
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,(unsigned int*)data,width,height); delete[] ndata; } 
	} break;
	} 
	  }
	  
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	  return *this;
	}

	static int _assign_lowlevel_xshm(Display *dpy, XErrorEvent *error) {      
	  dpy = 0; error = 0;
	  cimg::X11attr().shm_enabled = false;
	  return 0;
	}
	
	void assign_lowlevel(const unsigned int dimw, const unsigned int dimh, const char *ptitle=0,
			 const unsigned int normalization_type=1, const unsigned int events_type=3,
			 const bool fullscreen_flag=false, const bool closed_flag=false) {

	  // Allocate space for window title
	  const int s = cimg::strlen(ptitle)+1;
	  char *tmp_title = s?new char[s]:0;
	  if (s) std::memcpy(tmp_title,ptitle,s*sizeof(char));

	  // Destroy previous display window if existing
	  if (width && height) empty();

	  // Open X11 display if necessary.
	  if (!cimg::X11attr().display) { 
		cimg::X11attr().nb_wins = 0;
		cimg::X11attr().mutex = new pthread_mutex_t;
		pthread_mutex_init(cimg::X11attr().mutex,0);

		cimg::X11attr().display = XOpenDisplay((std::getenv("DISPLAY") ? std::getenv("DISPLAY") : ":0.0"));
		if (!cimg::X11attr().display) 
	  throw CImgDisplayException("CImgDisplay::_create_window() : Can't open X11 display");
		cimg::X11attr().nb_bits = DefaultDepth(cimg::X11attr().display, DefaultScreen(cimg::X11attr().display));
	if (cimg::X11attr().nb_bits!=8 && cimg::X11attr().nb_bits!=16 && cimg::X11attr().nb_bits!=24)
	  throw CImgDisplayException("CImgDisplay::_create_window() : %u bits mode is not supported "
					 "(only 8, 16 and 24 bits modes are supported)",cimg::X11attr().nb_bits);
		cimg::X11attr().gc = new GC;
		*cimg::X11attr().gc = DefaultGC(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
		Visual *visual = DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display));
	XVisualInfo vtemplate;
	vtemplate.visualid = XVisualIDFromVisual(visual);
	int nb_visuals;
	XVisualInfo *vinfo = XGetVisualInfo(cimg::X11attr().display,VisualIDMask,&vtemplate,&nb_visuals);
	if (vinfo && vinfo->red_mask<vinfo->blue_mask) cimg::X11attr().blue_first = true;
	cimg::X11attr().byte_order = ImageByteOrder(cimg::X11attr().display);

		pthread_mutex_lock(cimg::X11attr().mutex);
		cimg::X11attr().event_thread = new pthread_t;
		pthread_create(cimg::X11attr().event_thread,0,_events_thread,0);
	  } else pthread_mutex_lock(cimg::X11attr().mutex);
	  
	  // Set display variables
	  width = dimw;
	  height = dimh;
	  normalization = normalization_type&3;
	  events = events_type&3;
	  is_fullscreen = fullscreen_flag;
	  title = tmp_title;
	  window_x = window_y = 0;
	  mouse_x = mouse_y = -1;
	  button = wheel = key = 0;
	  is_resized = is_moved = is_event = false;
	  is_closed = closed_flag;
	  
	  // Create X11 window and palette (if 8bits display)
	  if (is_fullscreen) {
	_init_fullscreen();
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	XSetWindowAttributes winattr;
	winattr.override_redirect = True;
	window = XCreateWindow(cimg::X11attr().display,
				   RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				   (sx-width)/2,(sy-height)/2,
				   width,height,0,0,InputOutput,CopyFromParent,CWOverrideRedirect,&winattr);
	  } else
	window = XCreateSimpleWindow(cimg::X11attr().display,
					 RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
					 0,0,width,height,2,0,0x0L);
	  XStoreName(cimg::X11attr().display,window,title?title:" ");
	  if (cimg::X11attr().nb_bits==8) {
	colormap = XCreateColormap(cimg::X11attr().display,window,DefaultVisual(cimg::X11attr().display,
										DefaultScreen(cimg::X11attr().display)),AllocAll);
	_set_colormap(colormap,3);
	XSetWindowColormap(cimg::X11attr().display,window,colormap);
	  }

	  // Create XImage
	  const unsigned int bufsize = width*height*(cimg::X11attr().nb_bits==8?1:(cimg::X11attr().nb_bits==16?2:4));
#ifdef cimg_use_xshm
	  shminfo = 0;
	  if (XShmQueryExtension(cimg::X11attr().display)) {
	shminfo = new XShmSegmentInfo;
	image = XShmCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				cimg::X11attr().nb_bits,ZPixmap,0,shminfo,width,height);
	if (!image) { delete shminfo; shminfo = 0; }
	else {
	  shminfo->shmid = shmget(IPC_PRIVATE, bufsize, IPC_CREAT | 0777);	  
	  if (shminfo->shmid==-1) { XDestroyImage(image); delete shminfo; shminfo = 0; }
	  else {
		shminfo->shmaddr = image->data = (char*)(data = shmat(shminfo->shmid,0,0));
		if (shminfo->shmaddr==(char*)-1) { XDestroyImage(image); shmctl(shminfo->shmid,IPC_RMID,0); delete shminfo; shminfo = 0; }
		shminfo->readOnly = False;
		cimg::X11attr().shm_enabled = true;
		XErrorHandler oldXErrorHandler = XSetErrorHandler(_assign_lowlevel_xshm);
		XShmAttach(cimg::X11attr().display, shminfo);
		XSync(cimg::X11attr().display, False);
		XSetErrorHandler(oldXErrorHandler);
		if (!cimg::X11attr().shm_enabled) {
		  XDestroyImage(image);
		  shmdt(shminfo->shmaddr);
		  shmctl(shminfo->shmid,IPC_RMID,0);
		  delete shminfo; shminfo = 0;
		}
	  }
	}
	  }
	  if (!shminfo)
#endif		
	{
	  data = std::malloc(bufsize);
	  image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				   cimg::X11attr().nb_bits,ZPixmap,0,(char*)data,width,height,8,0);
	}
	
	  if (!is_closed) _map_window(); else { window_x = window_y = cimg::type<int>::min(); }
	  
	  if (events) { 
	wm_delete_window = XInternAtom(cimg::X11attr().display, "WM_DELETE_WINDOW", False);
	wm_delete_protocol = XInternAtom(cimg::X11attr().display, "WM_PROTOCOLS", False);
	XSetWMProtocols(cimg::X11attr().display, window, &wm_delete_window, 1); 
	if (is_fullscreen) XGrabKeyboard(cimg::X11attr().display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
	  }
	  window_width = width;
	  window_height = height;
	  mouse_x = mouse_y = -1;
	  button = wheel = key = 0;
	  is_resized = is_moved = is_event = false;
	  fps_timer = fps_frames = timer = 0;
	  fps_fps = 0;

	  cimg::X11attr().wins[cimg::X11attr().nb_wins++]=this;
	  pthread_mutex_unlock(cimg::X11attr().mutex);
	}

	void _map_window() {
	  XWindowAttributes attr;
	  XEvent event;
	  XSelectInput(cimg::X11attr().display,window,StructureNotifyMask);
	  XMapRaised(cimg::X11attr().display,window);
	  do XWindowEvent(cimg::X11attr().display,window,StructureNotifyMask,&event);
	  while (event.type!=MapNotify);
	  do {
	XSync(cimg::X11attr().display, False);
	XGetWindowAttributes(cimg::X11attr().display, window, &attr);
	  } while (attr.map_state != IsViewable);
	  window_x = attr.x;
	  window_y = attr.y;
	}

	void _set_colormap(Colormap& colormap, const unsigned int dim) {
	  XColor palette[256];
	  switch (dim) {
	  case 1:  // palette for greyscale images
	for (unsigned int index=0; index<256; index++) {
	  palette[index].pixel = index;
	  palette[index].red = palette[index].green = palette[index].blue = index<<8;
	  palette[index].flags = DoRed | DoGreen | DoBlue;
	}
	break;
	  case 2: // palette for RG images
	for (unsigned int index=0, r=8; r<256; r+=16)
	  for (unsigned int g=8; g<256; g+=16) {
		palette[index].pixel = index;
		palette[index].red   = palette[index].blue = r<<8;
		palette[index].green = g<<8;
		palette[index++].flags = DoRed | DoGreen | DoBlue;	    
	  }
	break;
	  default: // palette for RGB images
	for (unsigned int index=0, r=16; r<256; r+=32)
	  for (unsigned int g=16; g<256; g+=32)
		for (unsigned int b=32; b<256; b+=64) {
		  palette[index].pixel = index;
		  palette[index].red   = r<<8;
		  palette[index].green = g<<8;
		  palette[index].blue  = b<<8;
		  palette[index++].flags = DoRed | DoGreen | DoBlue;
		}
	break;
	  }      
	  XStoreColors(cimg::X11attr().display,colormap,palette,256);      
	}

	void _paint(const bool wait_expose=true) {
	  if (!is_closed) {
	if (wait_expose) {
	  static XEvent event;
	  event.xexpose.type = Expose;
	  event.xexpose.serial = 0;
	  event.xexpose.send_event = True;
	  event.xexpose.display = cimg::X11attr().display;
	  event.xexpose.window = window;
	  event.xexpose.x = 0;
	  event.xexpose.y = 0;
	  event.xexpose.width = (int)width;
	  event.xexpose.height = (int)height;
	  event.xexpose.count = 0;
	  XSendEvent(cimg::X11attr().display, window, False, 0, &event);
	} else {
#if cimg_use_xshm
	  if (shminfo) XShmPutImage(cimg::X11attr().display,window,*cimg::X11attr().gc,image,0,0,0,0,width,height,False);
	  else
#endif
		XPutImage(cimg::X11attr().display,window,*cimg::X11attr().gc,image,0,0,0,0,width,height); 
	  XSync(cimg::X11attr().display, False);
	}
	  }
	}
	
	template<typename T> void _resize(T foo, const unsigned int ndimx, const unsigned int ndimy, const bool redraw) {
	  foo = 0;
#ifdef cimg_use_xshm
	  if (shminfo) {
	XShmSegmentInfo *nshminfo = new XShmSegmentInfo;
	XImage *nimage = XShmCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
					 cimg::X11attr().nb_bits,ZPixmap,0,nshminfo,ndimx,ndimy);
	nshminfo->shmid = shmget(IPC_PRIVATE, ndimx*ndimy*sizeof(T), IPC_CREAT | 0777);
	nshminfo->shmaddr = nimage->data = (char*)shmat(nshminfo->shmid,0,0);
	nshminfo->readOnly = False;
	XShmAttach(cimg::X11attr().display, nshminfo);
	XSync(cimg::X11attr().display, False);
	T *const ndata = (T*)nimage->data;
	if (redraw) _render_resize((T*)data,width,height,ndata,ndimx,ndimy);
	else std::memset(ndata,0,sizeof(T)*ndimx*ndimy);
	XShmDetach(cimg::X11attr().display, shminfo);
	XDestroyImage(image);
	shmdt(shminfo->shmaddr);
	shmctl(shminfo->shmid,IPC_RMID,0);
	delete shminfo;
	shminfo = nshminfo;
	image = nimage;
	data = (void*)ndata;
	  } else
#endif
	{
	  T *ndata = (T*)std::malloc(ndimx*ndimy*sizeof(T));
	  if (redraw) _render_resize((T*)data,width,height,ndata,ndimx,ndimy);
	  else std::memset(ndata,0,sizeof(T)*ndimx*ndimy);
	  data = (void*)ndata;
	  XDestroyImage(image);
	  image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
				   cimg::X11attr().nb_bits,ZPixmap,0,(char*)data,ndimx,ndimy,8,0);
	}
	}

	void _init_fullscreen() {
	  background_window = 0;
	  if (is_fullscreen && !is_closed) {
#ifdef cimg_use_xrandr     
	int foo;
	if (XRRQueryExtension(cimg::X11attr().display,&foo,&foo)) {	
	  XRRRotations(cimg::X11attr().display, DefaultScreen(cimg::X11attr().display), &cimg::X11attr().curr_rotation);
	  if (!cimg::X11attr().resolutions) {
		cimg::X11attr().resolutions = XRRSizes(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display),&foo);
		cimg::X11attr().nb_resolutions = (unsigned int)foo;
	  }
	  if (cimg::X11attr().resolutions) {
		cimg::X11attr().curr_resolution = 0;
		for (unsigned int i=0; i<cimg::X11attr().nb_resolutions; i++) {
		  const unsigned int
		nw = (unsigned int)(cimg::X11attr().resolutions[i].width),
		nh = (unsigned int)(cimg::X11attr().resolutions[i].height);
		  if (nw>=width && nh>=height &&
		  nw<=(unsigned int)(cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].width) &&
		  nh<=(unsigned int)(cimg::X11attr().resolutions[cimg::X11attr().curr_resolution].height)) 
		cimg::X11attr().curr_resolution = i;
		}
		if (cimg::X11attr().curr_resolution>0) {
		  XRRScreenConfiguration *config = XRRGetScreenInfo(cimg::X11attr().display, DefaultRootWindow(cimg::X11attr().display));
		  XRRSetScreenConfig(cimg::X11attr().display, config, DefaultRootWindow(cimg::X11attr().display),
				 cimg::X11attr().curr_resolution, cimg::X11attr().curr_rotation, CurrentTime);
		  XRRFreeScreenConfigInfo(config);
		  XSync(cimg::X11attr().display, False);
		}
	  }
	}
	cimg::warn(!cimg::X11attr().resolutions,"CImgDisplay::_create_window() : Xrandr extension is not supported by the X server.");	
#endif
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	XSetWindowAttributes winattr;
	winattr.override_redirect = True;
	if (sx!=width || sy!=height) {
	  background_window = XCreateWindow(cimg::X11attr().display,
						RootWindow(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),0,0,
						sx,sy,0,0,InputOutput,CopyFromParent,CWOverrideRedirect,&winattr);
	  const unsigned int bufsize = sx*sy*(cimg::X11attr().nb_bits==8?1:(cimg::X11attr().nb_bits==16?2:4));
	  void *background_data = std::malloc(bufsize);
	  std::memset(background_data,0,bufsize);
	  XImage *background_image = XCreateImage(cimg::X11attr().display,DefaultVisual(cimg::X11attr().display,DefaultScreen(cimg::X11attr().display)),
						  cimg::X11attr().nb_bits,ZPixmap,0,(char*)background_data,sx,sy,8,0);
	  XEvent event;
	  XSelectInput(cimg::X11attr().display,background_window,StructureNotifyMask);
	  XMapRaised(cimg::X11attr().display,background_window);
	  do XWindowEvent(cimg::X11attr().display,background_window,StructureNotifyMask,&event);
	  while (event.type!=MapNotify);
	  XPutImage(cimg::X11attr().display,background_window,*cimg::X11attr().gc,background_image,0,0,0,0,sx,sy); 
	  XWindowAttributes attr;
	  XGetWindowAttributes(cimg::X11attr().display, background_window, &attr);
	  while (attr.map_state != IsViewable) XSync(cimg::X11attr().display, False);
	  XDestroyImage(background_image);
	}
	  }
	}

	void _desinit_fullscreen() {
	  if (is_fullscreen) {
	XUngrabKeyboard(cimg::X11attr().display,CurrentTime);
#if cimg_use_xrandr
	if (cimg::X11attr().resolutions && cimg::X11attr().curr_resolution) {	
	  XRRScreenConfiguration *config = XRRGetScreenInfo(cimg::X11attr().display, DefaultRootWindow(cimg::X11attr().display));
	  XRRSetScreenConfig(cimg::X11attr().display, config, DefaultRootWindow(cimg::X11attr().display),
				 0, cimg::X11attr().curr_rotation, CurrentTime);
	  XRRFreeScreenConfigInfo(config);
	  XSync(cimg::X11attr().display, False);
	  cimg::X11attr().curr_resolution = 0;
	}
#endif
	if (background_window) XDestroyWindow(cimg::X11attr().display,background_window);
	background_window = 0;
	is_fullscreen = false;
	  }
	}
	
	void _handle_events(const XEvent *const pevent) {
	  XEvent event=*pevent;
	  switch (event.type) {
	  case ClientMessage:
	if ((int)event.xclient.message_type==(int)wm_delete_protocol && 
		(int)event.xclient.data.l[0]==(int)wm_delete_window) {
	  XUnmapWindow(cimg::X11attr().display,window);
	  mouse_x = mouse_y = -1; 
	  button = key = 0;
	  is_closed = is_event = true;
	}
		break;
	 case ConfigureNotify: {
		while (XCheckWindowEvent(cimg::X11attr().display,window,StructureNotifyMask,&event));
		const unsigned int
	  nw = event.xconfigure.width,
	  nh = event.xconfigure.height;
	const int
	  nx = event.xconfigure.x,
	  ny = event.xconfigure.y;
		if (nw && nh && (nw!=window_width || nh!=window_height)) { 
		  window_width = nw; 
		  window_height = nh; 
	  mouse_x = mouse_y = -1;
		  XResizeWindow(cimg::X11attr().display,window,window_width,window_height);
		  is_resized = is_event = true;
		}
	if (nx!=window_x || ny!=window_y) {
	  window_x = nx;
	  window_y = ny;
	  is_moved = is_event = true;
	}
	 } break;
	  case Expose: {
		while (XCheckWindowEvent(cimg::X11attr().display,window,ExposureMask,&event));
	_paint(false);
		if (is_fullscreen) {
	  XWindowAttributes attr;
	  XGetWindowAttributes(cimg::X11attr().display, window, &attr);
	  while (attr.map_state != IsViewable) XSync(cimg::X11attr().display, False);
	  XSetInputFocus(cimg::X11attr().display, window, RevertToParent, CurrentTime);
	}
	  } break;
	  case ButtonPress: {
	do {
	  switch (event.xbutton.button) {
	  case 1: button|=1; is_event = true; break;
	  case 2: button|=4; is_event = true; break;
	  case 3: button|=2; is_event = true; break;
	  default: break;
	  }
	} while (XCheckWindowEvent(cimg::X11attr().display,window,ButtonPressMask,&event));
	  } break;
	  case ButtonRelease: {
	do {
	  switch (event.xbutton.button) {
	  case 1: button&=~1U; is_event = true; break;
	  case 2: button&=~4U; is_event = true; break;
	  case 3: button&=~2U; is_event = true; break;
	  case 4: wheel++; is_event = true; break;	    
	  case 5: wheel--; is_event = true; break;
	  default: break;
	  }
	} while (XCheckWindowEvent(cimg::X11attr().display,window,ButtonReleaseMask,&event));	
	  } break;
	  case KeyPress: {
		while (XCheckWindowEvent(cimg::X11attr().display,window,KeyPressMask,&event));
	char tmp;
	KeySym ksym;
	XLookupString(&event.xkey,&tmp,1,&ksym,0);
	key = (unsigned int)ksym;
	is_event = true;
	  } break;
	  case KeyRelease:
		while (XCheckWindowEvent(cimg::X11attr().display,window,KeyReleaseMask,&event));
		key = 0;
	is_event = true;
		break;
	  case LeaveNotify:
		while (XCheckWindowEvent(cimg::X11attr().display,window,LeaveWindowMask,&event));
		mouse_x = mouse_y =-1; 
	is_event = true;
		break;
	  case MotionNotify:
		while (XCheckWindowEvent(cimg::X11attr().display,window,PointerMotionMask,&event));
		mouse_x = event.xmotion.x; 
		mouse_y = event.xmotion.y;
		if (mouse_x<0 || mouse_y<0 || mouse_x>=dimx() || mouse_y>=dimy()) mouse_x = mouse_y = -1; 
	is_event = true;
		break;
	  }
	}
	
	static void* _events_thread(void *arg) {
	  arg = 0;
	  XEvent event;
	  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,0);
	  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,0);
	  for (;;) {
		pthread_mutex_lock(cimg::X11attr().mutex);
		for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++) {
		  const unsigned int xevent_type = (cimg::X11attr().wins[i]->events)&3;
		  const unsigned int emask =
			((xevent_type>=1)?ExposureMask|StructureNotifyMask:0)|
			((xevent_type>=2)?ButtonPressMask|KeyPressMask|PointerMotionMask|LeaveWindowMask:0)|
			((xevent_type>=3)?ButtonReleaseMask|KeyReleaseMask:0);
		  XSelectInput(cimg::X11attr().display,cimg::X11attr().wins[i]->window,emask);
		}
		bool event_flag = XCheckTypedEvent(cimg::X11attr().display, ClientMessage, &event);
		if (!event_flag) event_flag = XCheckMaskEvent(cimg::X11attr().display,
													  ExposureMask|StructureNotifyMask|ButtonPressMask|
													  KeyPressMask|PointerMotionMask|LeaveWindowMask|ButtonReleaseMask|
													  KeyReleaseMask,&event);
		if (event_flag) {
		  for (unsigned int i=0; i<cimg::X11attr().nb_wins; i++)
			if (!cimg::X11attr().wins[i]->is_closed && event.xany.window==cimg::X11attr().wins[i]->window)
		  cimg::X11attr().wins[i]->_handle_events(&event);
		}
		pthread_mutex_unlock(cimg::X11attr().mutex);
	pthread_testcancel();
		cimg::sleep(7);
	  }
	  return 0;
	}

	// Windows-based display
	//-----------------------
#elif cimg_display_type==2
	CLIENTCREATESTRUCT ccs;
	BITMAPINFO bmi;
	unsigned int *data;
	DEVMODE curr_mode;
	HWND window;
	HWND background_window;
	HDC hdc;
	HANDLE thread;
	HANDLE created;
	HANDLE mutex;
	
	static int screen_dimx() { 
	  DEVMODE mode;
	  mode.dmSize = sizeof(DEVMODE);
	  mode.dmDriverExtra = 0;
	  EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	  return mode.dmPelsWidth;      
	}
	
	static int screen_dimy() {
	  DEVMODE mode;
	  mode.dmSize = sizeof(DEVMODE); 
	  mode.dmDriverExtra = 0;
	  EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&mode);
	  return mode.dmPelsHeight;      
	}

	CImgDisplay& assign(const unsigned int dimw, const unsigned int dimh, const char *title=0,
			const unsigned int normalization_type=1, const unsigned int events_type=3,
			const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (!dimw || !dimh) 
	throw CImgArgumentException("CImgDisplay::assign() : Specified window size (%u,%u) is not valid.",dimw,dimh);
	  assign_lowlevel(dimw,dimh,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  std::memset(data,0,sizeof(unsigned int)*width*height);
	  min = max = 0;
	  return paint();
	}

	template<typename T> CImgDisplay& assign(const CImg<T>& img, const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input image (%u,%u,%u,%u,%p) is empty.",
					img.width,img.height,img.depth,img.dim,img.data);
	  CImg<T> tmp;
	  const CImg<T>& nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
	  assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
	  return display(nimg);
	}

	template<typename T> CImgDisplay& assign(const CImgl<T>& list,const char *title=0,
						 const unsigned int normalization_type=1, const unsigned int events_type=3,
						 const bool fullscreen_flag=false, const bool closed_flag=false) {
	  if (list.is_empty())
	throw CImgArgumentException("CImgDisplay::CImgDisplay() : Specified input list (%u,%p) is empty.",
					list.size,list.data);
	  CImg<T> tmp;
	  const CImg<T> img = list.get_append('x'), 
	&nimg = (img.depth==1)?img:(tmp=img.get_projections2d(img.width/2,img.height/2,img.depth/2));
	  assign_lowlevel(nimg.width,nimg.height,title,normalization_type,events_type,fullscreen_flag,closed_flag);
	  if (normalization==2) { const CImgStats st(nimg,false); min=st.min; max=st.max; }
	  return display(nimg);
	}

	CImgDisplay& assign(const CImgDisplay& win) {
	  assign_lowlevel(win.width,win.height,win.title,win.normalization,win.events,win.is_fullscreen,win.is_closed);
	  std::memcpy(data,win.data,sizeof(unsigned int)*width*height);
	  return paint();
	}

	CImgDisplay& empty() {            
	  DestroyWindow(window);
	  if (events) TerminateThread(thread,0);
	  if (data) delete[] data;
	  if (title) delete[] title;
	  if (is_fullscreen) _desinit_fullscreen();
	  width = height = normalization = events = 0;
	  is_fullscreen = is_resized = is_moved = is_event = false;
	  is_closed = true;
	  title = 0;
	  window_x = window_y = window_width = window_height = mouse_x = mouse_y = button = wheel = key = 0;
	  min = max = 0;
	  return *this;
	}

   template<typename T> CImgDisplay& display(const CImg<T>& img) {
	  return render(img).paint();
	}
  
	CImgDisplay& resize(const int nwidth, const int nheight, const bool redraw=true) {
	  if (!(nwidth && nheight))
	throw CImgArgumentException("CImgDisplay::resize() : Specified window size (%d,%d) is not valid.",
					nwidth,nheight);
	  const unsigned int
	tmpdimx=(nwidth>0)?nwidth:(-nwidth*width/100),
	tmpdimy=(nheight>0)?nheight:(-nheight*height/100),
	dimx = tmpdimx?tmpdimx:1,
	dimy = tmpdimy?tmpdimy:1;
	  const bool
	is_disp_different = (width!=dimx || height!=dimy),
	is_win_different = (window_width!=dimx || window_height!=dimy);

	  if (is_disp_different || is_win_different) {
	RECT rect; rect.left=rect.top=0; rect.right = dimx-1; rect.bottom = dimy-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int cwidth = rect.right-rect.left+1, cheight = rect.bottom-rect.top+1;
	SetWindowPos(window,0,0,0,cwidth,cheight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOCOPYBITS);
	window_width  = dimx;
	window_height = dimy;
	is_resized = false;
	if (is_disp_different) {
	  unsigned int *ndata = new unsigned int[dimx*dimy];
	  if (redraw) _render_resize(data,width,height,ndata,dimx,dimy);
	  else std::memset(ndata,0x80,sizeof(unsigned int)*dimx*dimy);
	  delete[] data;
	  data = ndata;
	  bmi.bmiHeader.biWidth = dimx;
	  bmi.bmiHeader.biHeight = -(int)dimy;
	  width = dimx;
	  height = dimy;
	}
	  }
	  if (is_fullscreen) move((screen_dimx()-width)/2,(screen_dimy()-height)/2);
	  return paint();
	}
	
	CImgDisplay& move(const int posx,const int posy) {
	  if (!is_fullscreen) {
	RECT rect; rect.left=rect.top=0; rect.right=window_width-1; rect.bottom=window_height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-width)/2, border2 = rect.bottom-rect.top+1-height-border1;
	SetWindowPos(window,0,posx-border1,posy-border2,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  } else SetWindowPos(window,0,posx,posy,0,0,SWP_NOSIZE | SWP_NOZORDER);
	  window_x = posx;
	  window_y = posy;
	  is_moved = false;
	  return show();
	}

	static void wait_all() {
	  WaitForSingleObject(cimg::Win32attr().wait_event,INFINITE);
	}

	CImgDisplay& show() {
	  if (is_closed) {
	is_closed = false;
	if (is_fullscreen) _init_fullscreen();
		ShowWindow(window,SW_SHOW);	
	RECT rect; 
	rect.left=rect.top=0; rect.right=width-1; rect.bottom=height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-width)/2, border2 = rect.bottom-rect.top+1-height-border1;
	GetWindowRect(window,&rect);
	window_x = rect.left + border1;
	window_y = rect.top + border2;
	  }
	  return paint();
	}

	CImgDisplay& close() {
	  if (!is_closed && !is_fullscreen) {
	if (is_fullscreen) _desinit_fullscreen();
		ShowWindow(window,SW_HIDE);
		is_closed = true;
	window_x = window_y = 0;
	  }
	  return *this;
	}

	CImgDisplay& set_title(const char *format,...) { 
	  char tmp[1024]={0}; 
	  va_list ap; 
	  va_start(ap, title); 
	  std::vsprintf(tmp,title,ap); 
	  va_end(ap); 
	  if (title) delete[] title;
	  const int s = cimg::strlen(tmp)+1;
	  title = new char[s];
	  std::memcpy(title,tmp,s*sizeof(char));      
#if defined(_MSC_VER) && _MSC_VER>1200
	  SetWindowTextA(window, tmp);
#else
	  SetWindowText(window, tmp);
#endif
	  return *this; 
	}
	
	CImgDisplay& paint() {
	  if (!is_closed) {
	WaitForSingleObject(mutex,INFINITE);
	SetDIBitsToDevice(hdc,0,0,width,height,0,0,0,height,data,&bmi,DIB_RGB_COLORS);
	ReleaseMutex(mutex);
	  }
	  return *this;
	}

	template<typename T> CImgDisplay& render(const CImg<T>& img) {
	  if (img.is_empty())
	throw CImgArgumentException("CImgDisplay::_render_image() : Specified input image (%u,%u,%u,%u,%p) is empty.",
					img.width,img.height,img.depth,img.dim,img.data);
	  if (img.depth!=1) return render(img.get_projections2d(img.width/2,img.height/2,img.depth/2));
	  
	  const T 
	*data1 = img.ptr(),
	*data2 = (img.dim>=2)?img.ptr(0,0,0,1):data1,
	*data3 = (img.dim>=3)?img.ptr(0,0,0,2):data1;

	  WaitForSingleObject(mutex,INFINITE);
	  unsigned int 
	*const ndata = (img.width==width && img.height==height)?data:new unsigned int[img.width*img.height],
	*ptrd = ndata;
	  
	  if (!normalization) for (unsigned int xy = img.width*img.height; xy>0; xy--)
	*(ptrd++) = ((unsigned char)*(data1++)<<16) | ((unsigned char)*(data2++)<<8) | (unsigned char)*(data3++);
	  else {
	if ((min>max) || normalization==1) { const CImgStats st(img,false); min=st.min; max=st.max; }
	const T nmin = (T)min, delta = (T)max-nmin, mm = delta?delta:(T)1;
	for (unsigned int xy = img.width*img.height; xy>0; xy--) {
	  const unsigned char
		R = (unsigned char)(255*(*(data1++)-nmin)/mm),
		G = (unsigned char)(255*(*(data2++)-nmin)/mm),
		B = (unsigned char)(255*(*(data3++)-nmin)/mm);
	  *(ptrd++) = (R<<16) | (G<<8) | (B);
	}
	  }
	  if (ndata!=data) { _render_resize(ndata,img.width,img.height,data,width,height); delete[] ndata; }
	  ReleaseMutex(mutex);
	  return *this;
	}
   

	CImgDisplay& assign_lowlevel(const unsigned int dimw,const unsigned int dimh,const char *ptitle=0,
				 const unsigned int normalization_type=1,const unsigned int events_type=3,
				 const bool fullscreen_flag=false,const bool closed_flag=false) {

	  
	  // Allocate space for window title
	  const int s = cimg::strlen(ptitle)+1;
	  char *tmp_title = s?new char[s]:0;
	  if (s) std::memcpy(tmp_title,ptitle,s*sizeof(char));
	  
	  // Destroy previous window if existing
	  if (width && height) empty();

	  // Set display variables
	  width = dimw;
	  height = dimh;
	  normalization = normalization_type&3;
	  events = events_type&3;
	  is_fullscreen = fullscreen_flag;
	  title = tmp_title;
	  window_x = window_y = 0;
	  mouse_x = mouse_y = -1;
	  button = wheel = key = 0;
	  is_resized = is_moved = is_event = false;      
	  is_closed = closed_flag;
	  fps_timer = fps_frames = timer = 0;
	  fps_fps = 0;

	  if (is_fullscreen) _init_fullscreen();
	  
	  // Create event thread
	  void *arg = (void*)(new void*[2]);
	  ((void**)arg)[0]=(void*)this;
	  ((void**)arg)[1]=(void*)title;            
	  if (events) {
	unsigned long ThreadID;
	mutex     = CreateMutex(0,FALSE,0);
	created   = CreateEvent(0,FALSE,FALSE,0);
	thread    = CreateThread(0,0,_events_thread,arg,0,&ThreadID);
	WaitForSingleObject(created,INFINITE);
	  } else _events_thread(arg);
	  
	  return *this;
	}

	static LRESULT APIENTRY _handle_events(HWND window,UINT msg,WPARAM wParam,LPARAM lParam) {
	  CImgDisplay* disp = (CImgDisplay*)GetWindowLong(window,GWL_USERDATA);
	  MSG st_msg;

	  switch(msg) {
	  case WM_CLOSE:
	disp->mouse_x = disp->mouse_y = -1;
	disp->key = disp->button = disp->window_x = disp->window_y = 0;
		disp->is_closed=true;
		ReleaseMutex(disp->mutex);
		ShowWindow(disp->window,SW_HIDE);
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		return 0;
	  case WM_SIZE: {
		while (PeekMessage(&st_msg,window,WM_SIZE,WM_SIZE,PM_REMOVE));
		WaitForSingleObject(disp->mutex,INFINITE);
		const unsigned int nw = LOWORD(lParam),nh = HIWORD(lParam);
		if (nw && nh && (nw!=disp->width || nh!=disp->height)) { 
		  disp->window_width  = nw; 
		  disp->window_height = nh;
	  disp->mouse_x = disp->mouse_y = -1;
		  disp->is_resized = disp->is_event = true;
	  SetEvent(cimg::Win32attr().wait_event);
		}
		ReleaseMutex(disp->mutex);
	  } break;
	  case WM_MOVE: {
		while (PeekMessage(&st_msg,window,WM_SIZE,WM_SIZE,PM_REMOVE));
	WaitForSingleObject(disp->mutex,INFINITE);
	const int nx = (int)(short)(LOWORD(lParam)), ny = (int)(short)(HIWORD(lParam));
	if (nx!=disp->window_x || ny!=disp->window_y) {
	  disp->window_x = nx;
	  disp->window_y = ny;
	  disp->is_moved = disp->is_event = true;
	  SetEvent(cimg::Win32attr().wait_event);
	}
		ReleaseMutex(disp->mutex);
	  } break;
	  case WM_PAINT:
	disp->paint();
		break;
	  }
	  if (disp->events>=2) switch(msg) {
	  case WM_KEYDOWN:
		while (PeekMessage(&st_msg,window,WM_KEYDOWN,WM_KEYDOWN,PM_REMOVE)); 
		disp->key=(int)wParam;
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_MOUSEMOVE: {
		while (PeekMessage(&st_msg,window,WM_MOUSEMOVE,WM_MOUSEMOVE,PM_REMOVE));
		disp->mouse_x = LOWORD(lParam);
		disp->mouse_y = HIWORD(lParam);
		if (disp->mouse_x<0 || disp->mouse_y<0 || disp->mouse_x>=disp->dimx() || disp->mouse_y>=disp->dimy())
	  disp->mouse_x=disp->mouse_y=-1;
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
	  } break;
	  case WM_LBUTTONDOWN: 
		disp->button|=1U; 
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_RBUTTONDOWN: 
		disp->button|=2U; 
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_MBUTTONDOWN: 
		disp->button|=4U; 
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  }
	  if (disp->events>=3) switch(msg) {
	  case WM_KEYUP:
		while (PeekMessage(&st_msg,window,WM_KEYUP,WM_KEYUP,PM_REMOVE));
		disp->key=0;
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_LBUTTONUP:
		disp->button&=~1U; 
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_RBUTTONUP:
		disp->button&=~2U;
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  case WM_MBUTTONUP:
		disp->button&=~4U;
	disp->is_event = true;
		SetEvent(cimg::Win32attr().wait_event);
		break;
	  }
	  return DefWindowProc(window,msg,wParam,lParam);
	}
  
	static DWORD WINAPI _events_thread(void* arg) {
	  CImgDisplay *disp  = (CImgDisplay*)(((void**)arg)[0]);
	  const char *title = (const char*)(((void**)arg)[1]);
	  MSG msg;
	  delete[] (void**)arg;
	  disp->bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	  disp->bmi.bmiHeader.biWidth=disp->width;
	  disp->bmi.bmiHeader.biHeight=-(int)disp->height;
	  disp->bmi.bmiHeader.biPlanes=1;
	  disp->bmi.bmiHeader.biBitCount=32;
	  disp->bmi.bmiHeader.biCompression=BI_RGB;
	  disp->bmi.bmiHeader.biSizeImage=0;
	  disp->bmi.bmiHeader.biXPelsPerMeter=1;
	  disp->bmi.bmiHeader.biYPelsPerMeter=1;
	  disp->bmi.bmiHeader.biClrUsed=0;
	  disp->bmi.bmiHeader.biClrImportant=0;
	  disp->data = new unsigned int[disp->width*disp->height];
	  if (!disp->is_fullscreen) { // Normal window
		RECT rect;
		rect.left=rect.top=0; rect.right=disp->width-1; rect.bottom=disp->height-1;
	AdjustWindowRect(&rect,WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,false);
	const int border1 = (rect.right-rect.left+1-disp->width)/2, border2 = rect.bottom-rect.top+1-disp->height-border1;
	   
#if defined(_MSC_VER) && _MSC_VER>1200
		disp->window = CreateWindowA("MDICLIENT",title?title:" ",
					 WS_OVERLAPPEDWINDOW | (disp->is_closed?0:WS_VISIBLE), CW_USEDEFAULT,CW_USEDEFAULT,
					 disp->width + 2*border1, disp->height + border1 + border2,
					 0,0,0,&(disp->ccs));
#else
		disp->window = CreateWindow("MDICLIENT",title?title:" ",
					WS_OVERLAPPEDWINDOW | (disp->is_closed?0:WS_VISIBLE), CW_USEDEFAULT,CW_USEDEFAULT,
					disp->width + 2*border1, disp->height + border1 + border2,
					0,0,0,&(disp->ccs));
#endif
	if (!disp->is_closed) {
	  GetWindowRect(disp->window,&rect);	
	  disp->window_x = rect.left + border1;
	  disp->window_y = rect.top + border2;
	} else disp->window_x = disp->window_y = 0;
	  } else { // Fullscreen window
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
#if defined(_MSC_VER) && _MSC_VER>1200
	disp->window = CreateWindowA("MDICLIENT",title?title:" ",
					 WS_POPUP | (disp->is_closed?0:WS_VISIBLE), (sx-disp->width)/2, (sy-disp->height)/2,
					 disp->width,disp->height,0,0,0,&(disp->ccs));
#else
	disp->window = CreateWindow("MDICLIENT",title?title:" ",
					WS_POPUP | (disp->is_closed?0:WS_VISIBLE), (sx-disp->width)/2, (sy-disp->height)/2,
					disp->width,disp->height,0,0,0,&(disp->ccs));
#endif
	disp->window_x = disp->window_y = 0;
	  }
	  SetForegroundWindow(disp->window);
	  disp->hdc = GetDC(disp->window);
	  disp->window_width = disp->width;
	  disp->window_height = disp->height;
	  disp->mouse_x = disp->mouse_y = -1;
	  disp->button = disp->wheel = disp->key = 0;
	  disp->is_resized = disp->is_moved = disp->is_event = false;
	  if (disp->events) {
		SetWindowLong(disp->window,GWL_USERDATA,(LONG)disp);
		SetWindowLong(disp->window,GWL_WNDPROC,(LONG)_handle_events);
		SetEvent(disp->created);
		while( GetMessage(&msg,0,0,0) ) DispatchMessage( &msg );
	  }
	  return 0;
	}

	void _init_fullscreen() {
	  background_window = 0;
	  if (is_fullscreen && !is_closed) {	  
	DEVMODE mode;
	unsigned int imode=0, ibest=0, bestbpp=0, bw=~0U, bh=~0U;
	for (mode.dmSize = sizeof(DEVMODE), mode.dmDriverExtra = 0; EnumDisplaySettings(0,imode,&mode); imode++) {
	  const unsigned int nw = mode.dmPelsWidth, nh = mode.dmPelsHeight;
	  if (nw>=width && nh>=height && mode.dmBitsPerPel>=bestbpp && nw<=bw && nh<=bh) {
		bestbpp = mode.dmBitsPerPel;
		ibest = imode; 
		bw = nw; bh = nh;
	  }
	}
	if (bestbpp) {
	  curr_mode.dmSize = sizeof(DEVMODE); curr_mode.dmDriverExtra = 0;
	  EnumDisplaySettings(0,ENUM_CURRENT_SETTINGS,&curr_mode);
	  EnumDisplaySettings(0,ibest,&mode);
	  ChangeDisplaySettings(&mode,0);
	} else curr_mode.dmSize = 0;
	
	const unsigned int sx = screen_dimx(), sy = screen_dimy();
	if (sx!=width || sy!=height) {
	  CLIENTCREATESTRUCT background_ccs;
#if defined(_MSC_VER) && _MSC_VER>1200
	  background_window = CreateWindowA("MDICLIENT"," ",WS_POPUP | WS_VISIBLE, 0,0,sx,sy,0,0,0,&background_ccs);
#else
	  background_window = CreateWindow("MDICLIENT"," ",WS_POPUP | WS_VISIBLE, 0,0,sx,sy,0,0,0,&background_ccs);
#endif
	  SetForegroundWindow(background_window);
	}	    
	  } else curr_mode.dmSize = 0;
	}
	
	void _desinit_fullscreen() {
	  if (is_fullscreen) {
	if (background_window) DestroyWindow(background_window);
	background_window = 0;
	if (curr_mode.dmSize) ChangeDisplaySettings(&curr_mode,0);
	is_fullscreen = false;	
	  }
	}
	  
#endif

#ifdef cimgdisplay_plugin
#include cimgdisplay_plugin
#endif
	
  };

  /*
   #--------------------------------------
   #
   #
   #
   # Definition of the CImg<T> structure
   #
   #
   #
   #--------------------------------------
   */

  //! Class representing an image (up to 4 dimensions wide), each pixel being of type \c T.
  /**
	 This is the main structure of the %CImg Library. It allows the declaration and construction
	 of an image, the access to its pixel values, and the application of various operations on it.

	 \par Image representation

	 A %CImg image is defined as an instance of the container \ref CImg<T>, which contains a regular grid of pixels,
	 each pixel value being of type \c T. The image grid can have up to 4 dimensions : width, height, depth
	 and number of channels.
	 Usually, the three first dimensions are used to describe spatial coordinates <tt>(x,y,z)</tt>, while the number of channels
	 is rather used as a vector-valued dimension (it may describe the R,G,B color channels for instance).
	 If you need a fifth dimension, you can use image lists \ref CImgl<T> rather than simple images \ref CImg<T>.

	 Thus, the \ref CImg<T> class is able to represent volumetric images of vector-valued pixels,
	 as well as images with less dimensions (1D scalar signal, 2D color images, ...). 
	 Most member functions of the class CImg<T> are designed to handle this maximum case of (3+1) dimensions.
	 
	 Concerning the pixel value type \c T :
	 fully supported template types are the basic C++ types : <tt>unsigned char, char, short, unsigned int, int,
	 unsigned long, long, float, double, ... </tt>.
	 Typically, fast image display can be done using <tt>CImg<unsigned char></tt> images,
	 while complex image processing algorithms may be rather coded using <tt>CImg<float></tt> or <tt>CImg<double></tt>
	 images that have floating-point pixel values. The default value for the template T is \c float.
	 Using your own template types may be possible. However, you will certainly have to define the complete set
	 of arithmetic and logical operators for your class.
	 
	 \par Image structure

	 The \ref CImg<\c T> structure contains \a five fields :
	 - \ref width defines the number of \a columns of the image (size along the X-axis).
	 - \ref height defines the number of \a rows of the image (size along the Y-axis).
	 - \ref depth defines the number of \a slices of the image (size along the Z-axis).
	 - \ref dim defines the number of \a channels of the image (size along the V-axis).
	 - \ref data defines a \a pointer to the \a pixel \a data (of type \c T).
	 
	 You can access these fields publicly although it is recommended to use the dedicated functions
	 dimx(), dimy(), dimz(), dimv() and ptr() to do so.     
	 Image dimensions are not limited to a specific range (as long as you got enough available memory).
	 A value of \e 1 usually means that the corresponding dimension is \a flat.
	 If one of the dimensions is \e 0, or if the data pointer is null, the image is considered as \e empty.
	 Empty images should not contain any pixel data and thus, will not be processed by CImg member functions
	 (a CImgInstanceException will be thrown instead).
	 Pixel data are stored in memory, in a non interlaced mode (See \ref cimg_storage).
	 
	 \par Image declaration and construction
	 
	 Declaring an image can be done by using one of the several available constructors.
	 Here is a list of the most used : 

	 - Construct images from arbitrary dimensions :
		 - <tt>CImg<char> img;</tt> declares an empty image.
		 - <tt>CImg<unsigned char> img(128,128);</tt> declares a 128x128 greyscale image with
	 \c unsigned \c char pixel values.
		 - <tt>CImg<double> img(3,3);</tt> declares a 3x3 matrix with \c double coefficients.
		 - <tt>CImg<unsigned char> img(256,256,1,3);</tt> declares a 256x256x1x3 (color) image
	 (colors are stored as an image with three channels).
		 - <tt>CImg<double> img(128,128,128);</tt> declares a 128x128x128 volumetric and greyscale image
	 (with \c double pixel values).
		 - <tt>CImg<> img(128,128,128,3);</tt> declares a 128x128x128 volumetric color image 
	 (with \c float pixels, which is the default value of the template parameter \c T).
	 - \b Note : images pixels are <b>not automatically initialized to 0</b>. You may use the function \ref fill() to
	 do it, or use the specific constructor taking 5 parameters like this :
	 <tt>CImg<> img(128,128,128,3,0);</tt> declares a 128x128x128 volumetric color image with all pixel values to 0.

	 - Construct images from filenames :
		 - <tt>CImg<unsigned char> img("image.jpg");</tt> reads a JPEG color image from the file "image.jpg".
	 - <tt>CImg<float> img("analyze.hdr");</tt> reads a volumetric image (ANALYZE7.5 format) from the file "analyze.hdr".
	 - \b Note : You need to install <a href="http://www.imagemagick.org">ImageMagick</a>
	 to be able to read common compressed image formats (JPG,PNG,...) (See \ref cimg_files_io).

	 - Construct images from C-style arrays :
		 - <tt>CImg<int> img(data_buffer,256,256);</tt> constructs a 256x256 greyscale image from a \c int* buffer 
	 \c data_buffer (of size 256x256=65536).
	 - <tt>CImg<unsigned char> img(data_buffer,256,256,1,3,false);</tt> constructs a 256x256 color image
	 from a \c unsigned \c char* buffer \c data_buffer (where R,G,B channels follow each others).
	 - <tt>CImg<unsigned char> img(data_buffer,256,256,1,3,true);</tt> constructs a 256x256 color image
	 from a \c unsigned \c char* buffer \c data_buffer (where R,G,B channels are multiplexed).

	 The complete list of constructors can be found <a href="#constructors">here</a>.

	 \par Most useful functions
	
	 The \ref CImg<T> class contains a lot of functions that operates on images.
	 Some of the most useful are :
	 
	 - operator()(), operator[]() : allows to access or write pixel values.
	 - display() : displays the image in a new window.
	 
	 \sa CImgl, CImgStats, CImgDisplay, CImgException.
	 
  **/
  template<typename T> struct CImg {
	unsigned int width, height, depth, dim;
	bool is_shared;
	T *data;

	//! Iterator type for CImg<T>.
	/**
	   \remark
	   - An \p iterator is a <tt>T*</tt> pointer (address of a pixel value in the pixel buffer).
	   - Iterators are not directly used in %CImg functions, they have been introduced for compatibility with the STL.
	**/
	typedef T* iterator;

	//! Const iterator type for CImg<T>.
	/**
	   \remark
	   - A \p const_iterator is a <tt>const T*</tt> pointer (address of a pixel value in the pixel buffer).
	   - Iterators are not directly used in %CImg functions, they have been introduced for compatibility with the STL.
	**/
	typedef const T* const_iterator;

	//--------------------------------------
	//
	//! \name Constructors-Destructor-Copy
	//@{
	//--------------------------------------

	//! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T.
	/**
	   \param dx : Width of the image (dimension along the X-axis, i.e. number of image columns).
	   \param dy : Height of the image (dimension along the Y-axis, i.e. number of image lines).
	   \param dz : Depth of the image (dimension along the Z-axis, i.e. number of image slices).
	   \param dv : Vector dimension of the image (dimension along the V-axis, i.e. number of image channels).

	   \remark
	   - If one of the specified dimension is equal to \p 0, the constructed image is \e empty and contains no pixel data.
	   In this case, all image dimensions as well as the pointer to the pixel buffer are set to \p 0.
	   - For non-empty images, pixel buffer allocation/deallocation is automatically performed by the constructor/destructor.
	   - When allocated, pixel data are always contiguous in memory.
	   - Pixel values are \e not initialized by this constructor. You may use CImg<T>::fill() to set a default pixel value
		after construction.
	  
	   \par Example :
	   \code 
	   CImg<float> img_greyscale(200,200);         // Define a 2D grayscale image with float-valued pixels.
	   CImg<unsigned char> img_color(256,256,1,3); // Define a 2D color-valued image with unsigned char-valued pixels.
	   CImg<short> img3d(64,64,64);                // Define a 3D greyscale image with short int-valued pixels.
	   \endcode       
	**/
	explicit CImg(const unsigned int dx=0, const unsigned int dy=1, const unsigned int dz=1, const unsigned int dv=1):
	  is_shared(false) {
	  const unsigned int siz = dx*dy*dz*dv;
	  if (siz) { width = dx; height = dy; depth = dz; dim = dv;	data = new T[siz]; }
	  else { width = height = depth = dim = 0; data = 0; }
	}

	//! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T and set all pixel values to \p val.
	/**
	   \param dx : Width of the image (dimension along the X-axis, i.e. number of image columns).
	   \param dy : Height of the image (dimension along the Y-axis, i.e. number of image lines).
	   \param dz : Depth of the image (dimension along the Z-axis, i.e. number of image slices).
	   \param dv : Vector dimension of the image (dimension along the V-axis, i.e. number of image channels).
	   \param val : Value assigned to all image pixels.
	   
	   \remark
	   - If one of the specified dimension is equal to \p 0, the constructed image is \e empty and contains no pixel data.
	   In this case, all image dimensions as well as the pointer to the pixel buffer are set to \p 0.
	   - For non-empty images, pixel buffer allocation/deallocation is automatically performed by the constructor/destructor.
	   - When allocated, pixel data are always contiguous in memory.
	   - <tt>CImg<T>(dx,dy,dz,dv,N);</tt> is equivalent to <tt>CImg<T>(dx,dy,dz,dv).fill(N);</tt>

	   \par Example :
	   \code
	   CImg<unsigned char> black_image(128,128,1,3,0); // Define a 128x128 black color image.
	   \endcode
	**/
	explicit CImg(const unsigned int dx, const unsigned int dy, const unsigned int dz, const unsigned int dv, const T& val):
	  is_shared(false) {
	  const unsigned int siz = dx*dy*dz*dv;
	  if (siz) { width = dx; height = dy; depth = dz; dim = dv; data = new T[siz]; fill(val); }
	  else { width = height = depth = dim = 0; data = 0; }
	}

	//! Construct an image of size (\p dx,\p dy,\p dz,\p dv) with pixels of type \p T, from a raw pixel buffer \p data_buffer.
	/**
	   \param data_buffer : Raw pixel buffer from where pixel values are copied.
	   \param dx : Width of the image (dimension along the X-axis, i.e. number of image columns).
	   \param dy : Height of the image (dimension along the Y-axis, i.e. number of image lines).
	   \param dz : Depth of the image (dimension along the Z-axis, i.e. number of image slices).
	   \param dv : Vector dimension of the image (dimension along the V-axis, i.e. number of image channels).

	   \remark
	   - If \p data_buffer is a pointer to \p 0 or if one of the specified dimension is equal to \p 0,
	   the constructed image is \e empty and contains no pixel data. In this case, all image dimensions
	   as well as the pointer to the pixel buffer are set to \p 0.
	   - For a non-empty image, a new pixel buffer is allocated containing a copy of the original \p data_buffer content.
	   - If source type \p t and destination type \p T do not match, an implicit cast is performed during construction.
	   
	   \par Example :
	   \code
	   const unsigned char buffer[256*256*3];   // Raw buffer that codes a 256*256 color image.
	   CImg<float> img(buffer,256,256,1,3);     // Construct a float-valued color image from the raw buffer.
	   \endcode
	**/
	template<typename t> CImg(const t *const data_buffer, const unsigned int dx, const unsigned int dy=1,
				  const unsigned int dz=1, const unsigned int dv=1):is_shared(false) {
	  const unsigned int siz = dx*dy*dz*dv;
	  if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; data = new T[siz];
	const t *ptrs = data_buffer+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
	  } else { width = height = depth = dim = 0; data = 0; }
	}

#if ( !defined(_MSC_VER) || _MSC_VER>1300 )
	CImg(const T *const data_buffer, const unsigned int dx, const unsigned int dy=1,
	 const unsigned int dz=1, const unsigned int dv=1):is_shared(false) {
#else
	CImg(const T *const data_buffer, const unsigned int dx, const unsigned int dy,
	 const unsigned int dz, const unsigned int dv):is_shared(false) {
#endif
	  const unsigned int siz = dx*dy*dz*dv;
	  if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; data = new T[siz];
	std::memcpy(data,data_buffer,siz*sizeof(T));
	  } else { width = height = depth = dim = 0; data = 0; }
	}
	
	CImg(const T *const data_buffer, const unsigned int dx, const unsigned int dy,
	 const unsigned int dz, const unsigned int dv, const bool shared) {
	  const unsigned int siz = dx*dy*dz*dv;
	  if (data_buffer && siz) {
	width = dx; height = dy; depth = dz; dim = dv; is_shared = shared;
	if (shared) data = const_cast<T*>(data_buffer);
	else { data = new T[siz]; std::memcpy(data,data_buffer,siz*sizeof(T)); }
	  } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
	}
	
	//! Construct an image from the filename \p filename.
	/**
	   \param filename : Filename of the file containing the image data.
	   
	   \remark
	   - If \p filename is equal to \p 0, an empty image is constructed. In this case, all image dimensions
	   as well as the pointer to the pixel buffer are set to \p 0.
	   - If the file does not exist or is corrupted, a CImgIOException is thrown.
	   
	   \par Example :
	   \code
	   CImg<float> img("lena.png");  // Construct a color float-valued image from a PNG image file.
	   \endcode
	   
	   \see CImg<T>::load().
	**/
	CImg(const char *const filename):width(0),height(0),depth(0),dim(0),is_shared(false),data(0) {
	  assign(filename); 
	}
	
	//! Copy constructor.
	/**
	   \param img : Source image to be copied.
	   
	   \remark
	   - Construction by copy allocates a new pixel buffer containing the copy of the source image \p img.
	   - If source type \p t and destination type \p T do not match, an implicit cast is performed during construction,
	   else a fast memory buffer copy is performed.       
	   - Copy of a shared-memory image is also a shared-memory image.       

	   \par Example :
	   \code
	   CImg<float> source("lena.png"), img(source);  // Construct 'img' as a copy of 'source'.
	   \endcode
	**/
	template<typename t> CImg(const CImg<t>& img):is_shared(false) {
	  const unsigned int siz = img.size();
	  if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; data = new T[siz];
	const t *ptrs = img.data+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs));
	  } else { width = height = depth = dim = 0; data = 0; }
	}
	
	CImg(const CImg& img) {
	  const unsigned int siz = img.size();
	  if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim;
	if (img.is_shared) { is_shared = true; data = img.data; }
	else { is_shared = false; data = new T[siz]; std::memcpy(data,img.data,siz*sizeof(T)); }
	  } else { width = height = depth = dim = 0; is_shared = false; data = 0; }
	}
	
	//! Construct an image with same size as the source image \p img, and copy pixel values if \p pixel_copy is \p true.
	/**
	   \param img : Source image to be copied.
	   
	   \remark
	   - If \pixel_copy is \p true, pixel values of \p img are copied during construction. In this case, this
	   constructor acts a the classical copy-constructor.
	   - If \pixel_copy is \p false, the constructed image gets its size from \p img, allocating its pixel buffer (if non-empty),
	   but does not copy the pixel buffer from \p img.
	   
	   \par Example :
	   \code
	   CImg<float> source("lena.png"), img(source,false);  // 'img' has same size as 'source' but with uninitialized pixel data.
	   \endcode       
	**/
	template<typename t> CImg(const CImg<t>& img, const bool pixel_copy):is_shared(false) {
	  const unsigned int siz = img.size();
	  if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; data = new T[siz];
	if (pixel_copy) { const t *ptrs = img.data+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs)); }
	  } else { width = height = depth = dim = 0; data = 0; }
	}

	CImg(const CImg& img, const bool pixel_copy):is_shared(false) {
	  const unsigned int siz = img.size();
	  if (img.data && siz) {
	width = img.width; height = img.height; depth = img.depth; dim = img.dim; data = new T[siz];
	if (pixel_copy) std::memcpy(data,img.data,siz*sizeof(T));
	  } else { width = height = depth = dim = 0; data = 0; }
	}

	//! Assignement operator.
	/**
	   This is the in-place version of the copy constructor.

	   \par Example :
	   \code
	   CImg<float> source("lena.png"), img;    // Here, 'img' is an empty image.
	   img = source;                           // 'img' is now a copy of the 'source' image.
	   \endcode
	**/
	template<typename t> CImg<T>& operator=(const CImg<t>& img) {
	  return assign(img); 
	}

	CImg& operator=(const CImg& img) {
	  return assign(img); 
	}

	//! Destructor.
	/**
	   \remark
	   - Destruction of a non-shared and non-empty image deallocates the memory used for the pixel buffer.
	**/
	~CImg() {
	  if (!is_shared && data) delete[] data;
	}
	
	//! In-place version of the corresponding constructor.
	/**
	   \par Example :
	   \code
	   CImg<float> img;           // 'img' is an empty image.
	   img.assign(256,256,1,3);   // 'img' is now a 256x256 color image.
	   \endcode;
	**/
	CImg& assign(const unsigned int dx=0, const unsigned int dy=1, const unsigned int dz=1, const unsigned int dv=1) {           
	  const unsigned long siz = dx*dy*dz*dv, curr_siz = size();
	  if (is_shared) {
	if (siz!=curr_siz)
	  throw CImgArgumentException("CImg<%s>::assign() : Cannot assign image (%u,%u,%u,%u) to shared instance image (%u,%u,%u,%u,%p).",
					  pixel_type(),dx,dy,dz,dv,width,height,depth,dim,data);
	  } else {
	if (siz) {
	  if (siz!=curr_siz) { if (data) delete[] data; data = new T[siz]; }
	  width = dx; height = dy; depth = dz; dim = dv;
	} else { 
	  if (data) delete[] data;
	  width = height = depth = dim = 0; data = 0; 
	}	
	  }
	  return *this;
	}

	//! In-place version of the corresponding constructor.
	/**
	   \par Example :
	   \code
	   CImg<float> img;             // 'img' is an empty image.
	   img.assign(256,256,1,3,0);   // 'img' is now a 256x256 black color image.
	   \endcode;
	**/
	CImg& assign(const unsigned int dx, const unsigned int dy, const unsigned int dz, const unsigned int dv, const T& val) {
	  return assign(dx,dy,dz,dv).fill(val);
	}
	
	//! In-place version of the corresponding constructor.
	template<typename t> CImg& assign(const t *const data_buffer, const unsigned int dx, const unsigned int dy=1,
					  const unsigned int dz=1, const unsigned int dv=1) {
	  assign(dx,dy,dz,dv);
	  const unsigned int siz = dx*dy*dz*dv;
	  if (data_buffer && siz) { const t *ptrs = data_buffer+siz; cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs)); }
	  return *this;
	}

	//! In-place version of the corresponding constructor.
	CImg& assign(const char *const filename) { 
	  return load(filename); 
	}
	
	//! In-place version of the corresponding constructor.
	template<typename t> CImg& assign(const CImg<t>& img) {
	  return assign(img.data,img.width,img.height,img.depth,img.dim);
	}
	
	CImg& assign(const CImg& img) { 
	  return assign(img.data,img.width,img.height,img.depth,img.dim);
	}
	
	//! In-place version of the corresponding constructor.
	template<typename t> CImg& assign(const CImg<t>& img, const bool pixel_copy) {
	  if (pixel_copy) assign(img);
	  else assign(img.width,img.height,img.depth,img.dim);
	  return *this;
	}

	//! In-place version of the destructor.
	CImg& empty() {
	  if (is_shared) throw CImgInstanceException("CImg<%s>::empty() : Cannot empty shared image (%u,%u,%u,%u,%p).",
						 pixel_type(),width,height,depth,dim,data);
	  else {
	if (data) delete[] data;
	width = height = depth = dim = 0; data = 0;
	  }
	  return *this;
	}
	
	//! Equivalent to empty() (introduced as a STL-like function name).
	CImg& clear() { 
	  return empty(); 
	}

	//! Return an empty image
	/**
	   \remark
	   - <tt>img = CImg<T>::get_empty()</tt> is equivalent to <tt>img = CImg<T>();</tt>.
	**/
	static CImg get_empty() { 
	  return CImg<T>(); 
	}
	
	// INNER ROUTINE : Swap fields of an image (use it carefully!)
	// If an image is shared, its content is replaced by the non-shared image (which is unchanged).
	CImg& swap(CImg& img) {
	  if (img.is_shared==is_shared) {
	cimg::swap(width,img.width);
	cimg::swap(height,img.height);
	cimg::swap(depth,img.depth);
	cimg::swap(dim,img.dim);
	cimg::swap(data,img.data);
	  } else {
	if (img.is_shared) img.assign(*this);
	if (is_shared) assign(img);
	  }
	  return img;
	}
	
	//@}
	//-------------------------------------
	//
	//! \name Access to image informations
	//@{
	//-------------------------------------
  
	//! Return the type of the pixel values.
	/**
	   \return a string describing the type of the image pixels (template parameter \p T).
	   - The string returned may contains spaces (<tt>"unsigned char"</tt>).
	   - If the template parameter T does not correspond to a registered type, the string <tt>"unknown"</tt> is returned.
	**/
	static const char* pixel_type() { 
	  T val;
	  return cimg::get_type(val); 
	}

	//! Return the total number of pixel values in an image.
	/**
	   - Equivalent to : dimx() * dimy() * dimz() * dimv().

	   \par example:
	   \code
	   CImg<> img(100,100,1,3);
	   if (img.size()==100*100*3) std::fprintf(stderr,"This statement is true");
	   \endcode
	   \sa dimx(), dimy(), dimz(), dimv()
	**/
	unsigned long size() const {
	  return width*height*depth*dim; 
	}  

	//! Return the number of columns of the instance image (size along the X-axis, i.e image width).
	/**
	   \sa width, dimy(), dimz(), dimv(), size().
	**/
	int dimx() const {
	  return (int)width; 
	}  

	//! Return the number of rows of the instance image (size along the Y-axis, i.e image height).
	/**
	   \sa height, dimx(), dimz(), dimv(), size().
	**/
	int dimy() const {
	  return (int)height; 
	}
  
	//! Return the number of slices of the instance image (size along the Z-axis).
	/**
	   \sa depth, dimx(), dimy(), dimv(), size().
	**/
	int dimz() const {
	  return (int)depth; 
	}
  
	//! Return the number of vector channels of the instance image (size along the V-axis).
	/**
	   \sa dim, dimx(), dimy(), dimz(), size().
	**/
	int dimv() const {
	  return (int)dim;
	}
  
	//! Return \c true if images \c (*this) and \c img have same width.
	template<typename t> bool is_sameX(const CImg<t>& img) const {
	  return (width==img.width); 
	}

	//! Return \c true if images \c (*this) and \c img have same height.
	template<typename t> bool is_sameY(const CImg<t>& img) const {
	  return (height==img.height); 
	}

	//! Return \c true if images \c (*this) and \c img have same depth.
	template<typename t> bool is_sameZ(const CImg<t>& img) const {
	  return (depth==img.depth); 
	}

	//! Return \c true if images \c (*this) and \c img have same dim.
	template<typename t> bool is_sameV(const CImg<t>& img) const { 
	  return (dim==img.dim); 
	}

	//! Return \c true if images have same width and same height.
	template<typename t> bool is_sameXY(const CImg<t>& img) const { 
	  return (is_sameX(img) && is_sameY(img)); 
	}

	//! Return \c true if images have same width, same height and same depth.
	template<typename t> bool is_sameXYZ(const CImg<t>& img) const { 
	  return (is_sameXY(img) && is_sameZ(img)); 
	}

	//! Return \c true if images \c (*this) and \c img have same width, same height, same depth and same number of channels.
	template<typename t> bool is_sameXYZV(const CImg<t>& img) const {
	  return (is_sameXYZ(img) && is_sameV(img)); 
	}
	
	//! Return \c true if image is empty.
	bool is_empty() const {
	  return !(data && width && height && depth && dim); 
	}

	//! Return the offset of the pixel coordinates (\p x,\p y,\p z,\p v) with respect to the data pointer \c data.
	/**
	   \param x X-coordinate of the pixel.
	   \param y Y-coordinate of the pixel.
	   \param z Z-coordinate of the pixel.
	   \param v V-coordinate of the pixel.

	   - No checking is done on the validity of the given coordinates.

	   \par example:
	   \code
	   CImg<float> img(100,100,1,3,0);         // Define a 100x100 color image with float-valued black pixels.
	   long off = img.offset(10,10,0,2);       // Get the offset of the blue value of the pixel located at (10,10).
	   float val = img[off];                   // Get the blue value of the pixel.
	   \endcode
	   \sa ptr(), operator()(), operator[](), cimg_storage.
	**/
	long offset(const int x=0, const int y=0, const int z=0, const int v=0) const {      
	  return x + y*width + z*width*height + v*width*height*depth;
	}

	//! Return a pointer to the pixel value located at (\p x,\p y,\p z,\p v).
	/**
	   \param x X-coordinate of the pixel.
	   \param y Y-coordinate of the pixel.
	   \param z Z-coordinate of the pixel.
	   \param v V-coordinate of the pixel.

	   - When called without parameters, ptr() returns a pointer to the begining of the pixel buffer.
	   - If the macro \c cimg_debug == 2, boundary checking is performed and warning messages may appear if 
	   given coordinates are outside the image range (but function performances decrease).

	   \par example:
	   \code
	   CImg<float> img(100,100,1,1,0);   // Define a 100x100 greyscale image with float-valued pixels.
	   float *ptr = ptr(10,10);          // Get a pointer to the pixel located at (10,10).
	   float val = *ptr;                 // Get the pixel value.
	   \endcode
	   \sa data, offset(), operator()(), operator[](), cimg_storage, cimg_environment.
	**/
	T* ptr(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0, const unsigned int v=0) {
	  const long off = offset(x,y,z,v);
#if cimg_debug>1
	  if (off<0 || off>=(long)size()) {
		cimg::warn(true,"CImg<%s>::ptr() : Asked for a pointer at coordinates (%u,%u,%u,%u) (offset=%u), "
		   "outside image range (%u,%u,%u,%u) (size=%u)",
		   pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
		return data;
	  }
#endif
	  return data+off;
	}
	
	const T* ptr(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0, const unsigned int v=0) const {
	  const long off = offset(x,y,z,v);
#if cimg_debug>1
	  if (off<0 || off>=(long)size()) {
		cimg::warn(true,"CImg<%s>::ptr() : Trying to get a pointer at (%u,%u,%u,%u) (offset=%d) which is"
		   "outside the data of the image (%u,%u,%u,%u) (size=%u)",
		   pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
		return data;
	  }
#endif
	  return data+off;
	}

	//! Return an iterator to the first image pixel
	iterator begin() { return data; }
	const_iterator begin() const { return data; }

	//! Return an iterator to the last image pixel
	iterator end() { return data + size(); }
	const_iterator end() const { return data + size(); }
	
	//! Fast access to pixel value for reading or writing.
	/**
	   \param x X-coordinate of the pixel.
	   \param y Y-coordinate of the pixel.
	   \param z Z-coordinate of the pixel.
	   \param v V-coordinate of the pixel.
	   
	   - If one image dimension is equal to 1, it can be omitted in the coordinate list (see example below).
	   - If the macro \c cimg_debug == 2, boundary checking is performed and warning messages may appear
	   (but function performances decrease).
	   
	   \par example:
	   \code
	   CImg<float> img(100,100,1,3,0);                       // Define a 100x100 color image with float-valued black pixels.
	   const float valR = img(10,10,0,0);                    // Read the red component at coordinates (10,10).
	   const float valG = img(10,10,0,1);                    // Read the green component at coordinates (10,10)
	   const float valB = img(10,10,2);                      // Read the blue component at coordinates (10,10) (Z-coordinate omitted here).
	   const float avg = (valR + valG + valB)/3;             // Compute average pixel value. 
	   img(10,10,0) = img(10,10,1) = img(10,10,2) = avg;     // Replace the pixel (10,10) by the average grey value.
	   \endcode
	   
	   \sa operator[](), ptr(), offset(), cimg_storage, cimg_environment.
	**/
	T& operator()(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) {
	  const long off = offset(x,y,z,v);
#if cimg_debug>1
	  if (!data || off>=(long)size()) {
		cimg::warn(true,"CImg<%s>::operator() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
		   "outside the image range (%u,%u,%u,%u) (size=%u)",
				   pixel_type(),x,y,z,v,off,width,height,depth,dim,size());			
		return *data;
	  }
#endif
	  return data[off];
	}

	const T& operator()(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) const {
	  const long off = offset(x,y,z,v);
#if cimg_debug>1
	  if (!data || off>=(long)size()) {
		cimg::warn(true,"CImg<%s>::operator() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
		   "outside the image range (%u,%u,%u,%u) (size=%u)",
				   pixel_type(),x,y,z,v,off,width,height,depth,dim,size());			
		return *data;
	  }
#endif
	  return data[off];
	}

	//! Return pixel value at a given position. Equivalent to operator().
	T& at(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) {
	  const long off = offset(x,y,z,v);
	  if (!data || off>=(long)size())
		throw CImgArgumentException("CImg<%s>::at() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
					"outside the image range (%u,%u,%u,%u) (size=%u)",
					pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
	  return data[off];
	}

	const T& at(const unsigned int x,const unsigned int y=0,const unsigned int z=0,const unsigned int v=0) const {
	  const long off = offset(x,y,z,v);
	  if (!data || off>=(long)size())
		throw CImgArgumentException("CImg<%s>::at() : Pixel access requested at (%u,%u,%u,%u) (offset=%d) "
					"outside the image range (%u,%u,%u,%u) (size=%u)",
					pixel_type(),x,y,z,v,off,width,height,depth,dim,size());
	  return data[off];
	}
	
	//! Fast access to pixel value for reading or writing, using an offset to the image pixel.
	/**
	   \param off Offset of the pixel according to the begining of the pixel buffer, given by ptr().

	   - If the macro \c cimg_debug==2, boundary checking is performed and warning messages may appear
	   (but function performances decrease).
	   - As pixel values are aligned in memory, this operator can sometime useful to access values easier than
	   with operator()() (see example below).
	   
	   \par example:
	   \code
	   CImg<float> vec(1,10);        // Define a vector of float values (10 lines, 1 row).
	   const float val1 = vec(0,4);  // Get the fifth element using operator()().
	   const float val2 = vec[4];    // Get the fifth element using operator[]. Here, val2==val1.       
	   \endcode

	   \sa operator()(), ptr(), offset(), cimg_storage, cimg_environment.
	**/    
	T& operator[](const unsigned long off) {
	  return operator()(off);
	}

	const T& operator[](const unsigned long off) const {
	  return operator()(off);
	}

	//! Return a reference to the last image value
	T& back() {
	  return operator()(size()-1);
	}
	
	const T& back() const {
	  return operator()(size()-1);
	}
	
	//! Return a reference to the first image value
	T& front() {
	  return *data;
	}
	
	const T& front() const {
	  return *data;
	}
	
	//! Read a pixel value with Dirichlet or Neumann boundary conditions.
	/**
	   \param x X-coordinate of the pixel.
	   \param y Y-coordinate of the pixel.
	   \param z Z-coordinate of the pixel.
	   \param v V-coordinate of the pixel.
	   \param out_val Desired value if pixel coordinates are outside the image range (optional parameter).
	   
	   - This function allows to read pixel values with boundary checking on all coordinates.
	   - If given coordinates are outside the image range and the parameter out_val is specified, the value \c out_val is returned.
	   - If given coordinates are outside the image range and the parameter out_val is not specified, the closest pixel value
	   is returned.
	   
	   \par example:
	   \code
	   CImg<float> img(100,100,1,1,128);                     // Define a 100x100 images with all pixel values equal to 128.
	   const float val1 = img.pix4d(10,10,0,0,0);  // Equivalent to val1=img(10,10) (but slower).
	   const float val2 = img.pix4d(-4,5,0,0,0);   // Return 0, since coordinates are outside the image range.
	   const float val3 = img.pix4d(10,10,5,0,64); // Return 64, since coordinates are outside the image range.
	   \endcode
	   
	   \sa operator()(), linear_pix4d(), cubic_pix2d().
	**/
	T pix4d(const int x, const int y, const int z, const int v, const T& out_val) const {
	  return (x<0 || y<0 || z<0 || v<0 || x>=dimx() || y>=dimy() || z>=dimz() || v>=dimv())?out_val:(*this)(x,y,z,v);
	}

	T pix4d(const int x, const int y, const int z, const int v) const {
	  return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),
					 z<0?0:(z>=dimz()?dimz()-1:z), v<0?0:(v>=dimv()?dimv()-1:v));
	}

	//! Read a pixel value with Dirichlet or Neumann boundary conditions for the three first coordinates (\c x,\c y,\c z).
	T pix3d(const int x, const int y, const int z, const int v, const T& out_val) const {
	  return (x<0 || y<0 || z<0 || x>=dimx() || y>=dimy() || z>=dimz())?out_val:(*this)(x,y,z,v);
	}

	const T& pix3d(const int x, const int y, const int z, const int v=0) const {
	  return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),
					 z<0?0:(z>=dimz()?dimz()-1:z),v);
	}

	//! Read a pixel value with Dirichlet or Neumann boundary conditions for the two first coordinates (\c x,\c y).
	T pix2d(const int x, const int y, const int z, const int v, const T& out_val) const {
	  return (x<0 || y<0 || x>=dimx() || y>=dimy())?out_val:(*this)(x,y,z,v);
	}

	const T& pix2d(const int x,const int y,const int z=0,const int v=0) const {
	  return (*this)(x<0?0:(x>=dimx()?dimx()-1:x), y<0?0:(y>=dimy()?dimy()-1:y),z,v);
	}

	//! Read a pixel value with Dirichlet or Neumann boundary conditions for the first coordinate \c x.
	T pix1d(const int x, const int y, const int z, const int v, const T& out_val) const {
	  return (x<0 || x>=dimx())?out_val:(*this)(x,y,z,v);
	}

	const T& pix1d(const int x, const int y=0, const int z=0, const int v=0) const {
	  return (*this)(x<0?0:(x>=dimx()?dimx()-1:x),y,z,v);
	}
	
	//! Read a pixel value using linear interpolation.
	/**
	   \param ffx X-coordinate of the pixel (float-valued).
	   \param ffy Y-coordinate of the pixel (float-valued).
	   \param ffz Z-coordinate of the pixel (float-valued).
	   \param ffv V-coordinate of the pixel (float-valued).
	   \param out_val Out-of-border pixel value
	   
	   - This function allows to read pixel values with boundary checking on all coordinates.
	   - If given coordinates are outside the image range, the value of the nearest pixel inside the image is returned
	   (Neumann boundary conditions).
	   - If given coordinates are float-valued, a linear interpolation is performed in order to compute the returned value.

	   \par example:
	   \code
	   CImg<float> img(2,2);     // Define a greyscale 2x2 image.
	   img(0,0) = 0;             // Fill image with specified pixel values.
	   img(1,0) = 1;
	   img(0,1) = 2;
	   img(1,1) = 3;
	   const double val = img.linear_pix4d(0.5,0.5);  // Return val=1.5, which is the average intensity of the four pixels values.
	   \endcode
	   
	   \sa operator()(), linear_pix3d(), linear_pix2d(), linear_pix1d(), cubic_pix2d().
	**/
	typename cimg::largest<T,float>::type linear_pix4d(const float fx,const float fy,const float fz,const float fv,
							   const T& out_val) const {
	  const int x = (int)fx-(fx>=0?0:1), y = (int)fy-(fy>=0?0:1), z = (int)fz-(fz>=0?0:1), v = (int)fv-(fv>=0?0:1),
	nx = x+1, ny = y+1, nz = z+1, nv = v+1;
	  const float dx = fx-x, dy = fy-y, dz = fz-z, dv = fv-v;
	  const T
	Icccc = pix4d(x,y,z,v,out_val),    Inccc = pix4d(nx,y,z,v,out_val), 
	Icncc = pix4d(x,ny,z,v,out_val),   Inncc = pix4d(nx,ny,z,v,out_val),
	Iccnc = pix4d(x,y,nz,v,out_val),   Incnc = pix4d(nx,y,nz,v,out_val),
	Icnnc = pix4d(x,ny,nz,v,out_val),  Innnc = pix4d(nx,ny,nz,v,out_val),
	Icccn = pix4d(x,y,z,nv,out_val),   Inccn = pix4d(nx,y,z,nv,out_val), 
	Icncn = pix4d(x,ny,z,nv,out_val),  Inncn = pix4d(nx,ny,z,nv,out_val),
	Iccnn = pix4d(x,y,nz,nv,out_val),  Incnn = pix4d(nx,y,nz,nv,out_val), 
	Icnnn = pix4d(x,ny,nz,nv,out_val), Innnn = pix4d(nx,ny,nz,nv,out_val);
	  return Icccc +
	dx*(Inccc-Icccc +
		dy*(Icccc+Inncc-Icncc-Inccc +
		dz*(Iccnc+Innnc+Icncc+Inccc-Icnnc-Incnc-Icccc-Inncc +
			dv*(Iccnn+Innnn+Icncn+Inccn+Icnnc+Incnc+Icccc+Inncc-Icnnn-Incnn-Icccn-Inncn-Iccnc-Innnc-Icncc-Inccc)) +
		dv*(Icccn+Inncn+Icncc+Inccc-Icncn-Inccn-Icccc-Inncc)) +
		dz*(Icccc+Incnc-Iccnc-Inccc +
		dv*(Icccn+Incnn+Iccnc+Inccc-Iccnn-Inccn-Icccc-Incnc)) +
		dv*(Icccc+Inccn-Inccc-Icccn)) +
	dy*(Icncc-Icccc +
		dz*(Icccc+Icnnc-Iccnc-Icncc +
		dv*(Icccn+Icnnn+Iccnc+Icncc-Iccnn-Icncn-Icccc-Icnnc)) +
		dv*(Icccc+Icncn-Icncc-Icccn)) +
	dz*(Iccnc-Icccc +
		dv*(Icccc+Iccnn-Iccnc-Icccn)) +
	dv*(Icccn-Icccc);
	}
	
	typename cimg::largest<T,float>::type linear_pix4d(const float ffx,const float ffy=0,const float ffz=0,const float ffv=0) const {
	  const float
	fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy),
		fz = ffz<0?0:(ffz>depth-1?depth-1:ffz), fv = ffv<0?0:(ffv>dim-1?dim-1:ffv);
	  const unsigned int x = (unsigned int)fx, y = (unsigned int)fy,  z = (unsigned int)fz, v = (unsigned int)fv;
	  const float dx = fx-x, dy = fy-y, dz = fz-z, dv = fv-v;
	  const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y,  nz = dz>0?z+1:z, nv = dv>0?v+1:v;
	  const T
	&Icccc = (*this)(x,y,z,v),   &Inccc = (*this)(nx,y,z,v),   &Icncc = (*this)(x,ny,z,v),   &Inncc = (*this)(nx,ny,z,v),
	&Iccnc = (*this)(x,y,nz,v),  &Incnc = (*this)(nx,y,nz,v),  &Icnnc = (*this)(x,ny,nz,v),  &Innnc = (*this)(nx,ny,nz,v),
	&Icccn = (*this)(x,y,z,nv),  &Inccn = (*this)(nx,y,z,nv),  &Icncn = (*this)(x,ny,z,nv),  &Inncn = (*this)(nx,ny,z,nv),
	&Iccnn = (*this)(x,y,nz,nv), &Incnn = (*this)(nx,y,nz,nv), &Icnnn = (*this)(x,ny,nz,nv), &Innnn = (*this)(nx,ny,nz,nv);   
	  return Icccc +
	dx*(Inccc-Icccc +
		dy*(Icccc+Inncc-Icncc-Inccc +
		dz*(Iccnc+Innnc+Icncc+Inccc-Icnnc-Incnc-Icccc-Inncc +
			dv*(Iccnn+Innnn+Icncn+Inccn+Icnnc+Incnc+Icccc+Inncc-Icnnn-Incnn-Icccn-Inncn-Iccnc-Innnc-Icncc-Inccc)) +
		dv*(Icccn+Inncn+Icncc+Inccc-Icncn-Inccn-Icccc-Inncc)) +
		dz*(Icccc+Incnc-Iccnc-Inccc +
		dv*(Icccn+Incnn+Iccnc+Inccc-Iccnn-Inccn-Icccc-Incnc)) +
		dv*(Icccc+Inccn-Inccc-Icccn)) +
	dy*(Icncc-Icccc +
		dz*(Icccc+Icnnc-Iccnc-Icncc +
		dv*(Icccn+Icnnn+Iccnc+Icncc-Iccnn-Icncn-Icccc-Icnnc)) +
		dv*(Icccc+Icncn-Icncc-Icccn)) +
	dz*(Iccnc-Icccc +
		dv*(Icccc+Iccnn-Iccnc-Icccn)) +
	dv*(Icccn-Icccc);
	}

	//! Read a pixel value using linear interpolation for the three first coordinates (\c cx,\c cy,\c cz).
	/**
	   - Same as linear_pix4d(), except that linear interpolation and boundary checking is performed only on the three first coordinates.

	   \sa operator()(), linear_pix4d(), linear_pix2d(), linear_pix1d(), linear_pix3d(), cubic_pix2d().
	**/
	typename cimg::largest<T,float>::type linear_pix3d(const float fx,const float fy,const float fz,const int v,
							   const T& out_val) const {
	  const int x = (int)fx-(fx>=0?0:1), y = (int)fy-(fy>=0?0:1), z = (int)fz-(fz>=0?0:1), nx = x+1, ny = y+1, nz = z+1;
	  const float dx = fx-x, dy = fy-y, dz = fz-z;
	  const T 
	Iccc = pix3d(x,y,z,v,out_val),  Incc = pix3d(nx,y,z,v,out_val),  Icnc = pix3d(x,ny,z,v,out_val),  Innc = pix3d(nx,ny,z,v,out_val),
	Iccn = pix3d(x,y,nz,v,out_val), Incn = pix3d(nx,y,nz,v,out_val), Icnn = pix3d(x,ny,nz,v,out_val), Innn = pix3d(nx,ny,nz,v,out_val);
	  return Iccc +
	dx*(Incc-Iccc +
		dy*(Iccc+Innc-Icnc-Incc +
		dz*(Iccn+Innn+Icnc+Incc-Icnn-Incn-Iccc-Innc)) +
		dz*(Iccc+Incn-Iccn-Incc)) +
	dy*(Icnc-Iccc +
		dz*(Iccc+Icnn-Iccn-Icnc)) +
	dz*(Iccn-Iccc);
	}
	
	typename cimg::largest<T,float>::type linear_pix3d(const float ffx,const float ffy=0,const float ffz=0,const int v=0) const {
	  const float fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy), fz = ffz<0?0:(ffz>depth-1?depth-1:ffz);
	  const unsigned int x = (unsigned int)fx, y = (unsigned int)fy, z = (unsigned int)fz;
	  const float dx = fx-x, dy = fy-y, dz = fz-z;
	  const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y, nz = dz>0?z+1:z;
	  const T 
	&Iccc = (*this)(x,y,z,v),  &Incc = (*this)(nx,y,z,v),  &Icnc = (*this)(x,ny,z,v),  &Innc = (*this)(nx,ny,z,v),
	&Iccn = (*this)(x,y,nz,v), &Incn = (*this)(nx,y,nz,v), &Icnn = (*this)(x,ny,nz,v), &Innn = (*this)(nx,ny,nz,v);
	  return Iccc +
	dx*(Incc-Iccc +
		dy*(Iccc+Innc-Icnc-Incc +
		dz*(Iccn+Innn+Icnc+Incc-Icnn-Incn-Iccc-Innc)) +
		dz*(Iccc+Incn-Iccn-Incc)) +
	dy*(Icnc-Iccc +
		dz*(Iccc+Icnn-Iccn-Icnc)) +
	dz*(Iccn-Iccc);
	}

	//! Read a pixel value using linear interpolation for the two first coordinates (\c cx,\c cy).
	/**
	   - Same as linear_pix4d(), except that linear interpolation and boundary checking is performed only on the two first coordinates.

	   \sa operator()(), linear_pix4d(), linear_pix3d(), linear_pix1d(), linear_pix2d(), cubic_pix2d().
	**/
	typename cimg::largest<T,float>::type linear_pix2d(const float fx, const float fy, const int z, const int v,
							   const T& out_val) const {
	  const int x = (int)fx-(fx>0?0:1), y = (int)fy-(fy>0?0:1), nx = x+1, ny = y+1;
	  const float dx = fx-x, dy = fy-y;
	  const T 
	Icc = pix2d(x,y,z,v,out_val),  Inc = pix2d(nx,y,z,v,out_val),
	Icn = pix2d(x,ny,z,v,out_val), Inn = pix2d(nx,ny,z,v,out_val);
	  return Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc);
	}

	typename cimg::largest<T,float>::type linear_pix2d(const float ffx, const float ffy=0, const int z=0, const int v=0) const {
	  const float fx = ffx<0?0:(ffx>width-1?width-1:ffx), fy = ffy<0?0:(ffy>height-1?height-1:ffy);
	  const unsigned int x = (unsigned int)fx, y = (unsigned int)fy;
	  const float dx = fx-x, dy = fy-y;
	  const unsigned int nx = dx>0?x+1:x, ny = dy>0?y+1:y;
	  const T &Icc = (*this)(x,y,z,v), &Inc = (*this)(nx,y,z,v), &Icn = (*this)(x,ny,z,v), &Inn = (*this)(nx,ny,z,v);
	  return Icc + dx*(Inc-Icc + dy*(Icc+Inn-Icn-Inc)) + dy*(Icn-Icc);
	}

	//! Read a pixel value using linear interpolation for the first coordinate \c cx.
	/**
	   - Same as linear_pix4d(), except that linear interpolation and boundary checking is performed only on the first coordinate.

	   \sa operator()(), linear_pix4d(), linear_pix3d(), linear_pix2d(), linear_pix1d(), cubic_pix1d().
	**/
	typename cimg::largest<T,float>::type linear_pix1d(const float fx,const int y,const int z,const int v,
							   const T& out_val) const {
	  const int x = (int)fx-(fx>0?0:1), nx = x+1;
	  const float dx = fx-x;
	  const T Ic = pix1d(x,y,z,v,out_val), In = pix2d(nx,y,z,v,out_val);
	  return Ic + dx*(In-Ic);
	}

	typename cimg::largest<T,float>::type linear_pix1d(const float ffx,const int y=0,const int z=0,const int v=0) const {
	  const float fx = ffx<0?0:(ffx>width-1?width-1:ffx);
	  const unsigned int x = (unsigned int)fx;
	  const float dx = fx-x;
	  const unsigned int nx = dx>0?x+1:x;
	  const T &Ic = (*this)(x,y,z,v), &In = (*this)(nx,y,z,v);
	  return Ic + dx*(In-Ic);
	}

	// This function is used as a subroutine for cubic interpolation
	static float _cubic_R(const float x) {
	  const float xp2 = x+2, xp1 = x+1, xm1 = x-1,
	nxp2 = xp2>0?xp2:0, nxp1 = xp1>0?xp1:0, nx = x>0?x:0, nxm1 = xm1>0?xm1:0;
	  return (nxp2*nxp2*nxp2 - 4*nxp1*nxp1*nxp1 + 6*nx*nx*nx - 4*nxm1*nxm1*nxm1)/6.0f;
	}

	//! Read a pixel value using cubic interpolation for the first coordinate \c cx.
	/**
	   - Same as cubic_pix2d(), except that cubic interpolation and boundary checking is performed only on the first coordinate.
	   
	   \sa operator()(), cubic_pix2d(), linear_pix1d().
	**/
	typename cimg::largest<T,float>::type cubic_pix1d(const float fx,const int y,const int z,const int v,
							  const T& out_val) const {
	  const int x = (int)fx-(fx>=0?0:1), px = x-1, nx = x+1, ax = nx+1;
	  const float dx = fx-x;
	  const T a = pix2d(px,y,z,v,out_val), b = pix2d(x,y,z,v,out_val), c = pix2d(nx,y,z,v,out_val), d = pix2d(ax,y,z,v,out_val);
	  const float Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx);
	  return Rxp*a + Rxc*b + Rxn*c + Rxa*d;
	}

	typename cimg::largest<T,float>::type cubic_pix1d(const float pfx,const int y=0,const int z=0,const int v=0) const {
	  const float fx = pfx<0?0:(pfx>width-1?width-1:pfx);
	  const unsigned int x = (unsigned int)fx, px = (int)x-1>=0?x-1:0, nx = x+1<width?x+1:width-1, ax = nx+1<width?nx+1:width-1;
	  const float dx = fx-x;
	  const T& a = (*this)(px,y,z,v), b = (*this)(x,y,z,v), c = (*this)(nx,y,z,v), d = (*this)(ax,y,z,v);
	  const float Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx);
	  return Rxp*a + Rxc*b + Rxn*c + Rxa*d;
	}
   
	//! Read a pixel value using bicubic interpolation.
	/**
	   \param pfx X-coordinate of the pixel (float-valued).
	   \param pfy Y-coordinate of the pixel (float-valued).
	   \param z   Z-coordinate of the pixel.
	   \param v   V-coordinate of the pixel.
	   
	   - This function allows to read pixel values with boundary checking on the two first coordinates.
	   - If given coordinates are outside the image range, the value of the nearest pixel inside the image is returned
	   (Neumann boundary conditions).
	   - If given coordinates are float-valued, a cubic interpolation is performed in order to compute the returned value.
			  
	   \sa operator()(), cubic_pix1d(), linear_pix2d().
	**/
	typename cimg::largest<T,float>::type cubic_pix2d(const float fx,const float fy,const int z,const int v,
							  const T& out_val) const {
	  const int 
		x = (int)fx-(fx>=0?0:1), y = (int)fy-(fy>=0?0:1),
	px = x-1, nx = x+1, ax = nx+1, py = y-1, ny = y+1, ay = ny+1;
	  const float dx = fx-x, dy = fy-y;
	  const T
		a = pix2d(px,py,z,v,out_val), b = pix2d(x,py,z,v,out_val), c = pix2d(nx,py,z,v,out_val), d = pix2d(ax,py,z,v,out_val),
		e = pix2d(px, y,z,v,out_val), f = pix2d(x, y,z,v,out_val), g = pix2d(nx, y,z,v,out_val), h = pix2d(ax, y,z,v,out_val),
		i = pix2d(px,ny,z,v,out_val), j = pix2d(x,ny,z,v,out_val), k = pix2d(nx,ny,z,v,out_val), l = pix2d(ax,ny,z,v,out_val),
		m = pix2d(px,ay,z,v,out_val), n = pix2d(x,ay,z,v,out_val), o = pix2d(nx,ay,z,v,out_val), p = pix2d(ax,ay,z,v,out_val);
	  const float
	Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx),
	Ryp = _cubic_R(dy+1),  Ryc = _cubic_R(dy), Ryn = _cubic_R(dy-1), Rya = _cubic_R(dy-2);    
	  return
	Rxp*Ryp*a + Rxc*Ryp*b + Rxn*Ryp*c + Rxa*Ryp*d +
	Rxp*Ryc*e + Rxc*Ryc*f + Rxn*Ryc*g + Rxa*Ryc*h +
	Rxp*Ryn*i + Rxc*Ryn*j + Rxn*Ryn*k + Rxa*Ryn*l +
	Rxp*Rya*m + Rxc*Rya*n + Rxn*Rya*o + Rxa*Rya*p;
	}

	typename cimg::largest<T,float>::type cubic_pix2d(const float pfx,const float pfy=0,const int z=0,const int v=0) const {
	  const float fx = pfx<0?0:(pfx>width-1?width-1:pfx), fy = pfy<0?0:(pfy>height-1?height-1:pfy);
	  const unsigned int 
		x = (unsigned int)fx,  px = (int)x-1>=0?x-1:0, nx = x+1<width?x+1:width-1, ax = nx+1<width?nx+1:width-1,
		y = (unsigned int)fy,  py = (int)y-1>=0?y-1:0, ny = y+1<height?y+1:height-1, ay = ny+1<height?ny+1:height-1;
	  const float dx = fx-x, dy = fy-y;
	  const T& 
		a = (*this)(px,py,z,v), b = (*this)(x,py,z,v), c = (*this)(nx,py,z,v), d = (*this)(ax,py,z,v),
		e = (*this)(px, y,z,v), f = (*this)(x, y,z,v), g = (*this)(nx, y,z,v), h = (*this)(ax, y,z,v),
		i = (*this)(px,ny,z,v), j = (*this)(x,ny,z,v), k = (*this)(nx,ny,z,v), l = (*this)(ax,ny,z,v),
		m = (*this)(px,ay,z,v), n = (*this)(x,ay,z,v), o = (*this)(nx,ay,z,v), p = (*this)(ax,ay,z,v);
	  const float
	Rxp = _cubic_R(-1-dx), Rxc = _cubic_R(dx), Rxn = _cubic_R(1-dx), Rxa = _cubic_R(2-dx),
	Ryp = _cubic_R(dy+1),  Ryc = _cubic_R(dy), Ryn = _cubic_R(dy-1), Rya = _cubic_R(dy-2);    
	  return
	Rxp*Ryp*a + Rxc*Ryp*b + Rxn*Ryp*c + Rxa*Ryp*d +
	Rxp*Ryc*e + Rxc*Ryc*f + Rxn*Ryc*g + Rxa*Ryc*h +
	Rxp*Ryn*i + Rxc*Ryn*j + Rxn*Ryn*k + Rxa*Ryn*l +
	Rxp*Rya*m + Rxc*Rya*n + Rxn*Rya*o + Rxa*Rya*p;
	}

	//! Display informations about the image on the standard error output.
	/**
	   \param title Name for the considered image (optional).
	   \param print_flag Level of informations to be printed.
	   
	   - The possible values for \c print_flag are :
		   - 0 : print only informations about image size and pixel buffer.
		   - 1 : print also statistics on the image pixels.
	   - 2 : print also the content of the pixel buffer, in a matlab-style.

	   \par example:
	   \code
	   CImg<float> img("foo.jpg");      // Load image from a JPEG file.
	   img.print("Image : foo.jpg",1);  // Print image informations and statistics.
	   \endcode
	   
	   \sa CImgStats	   
	**/
	const CImg& print(const char *title=0,const unsigned int print_flag=1) const {
	  std::fprintf(stderr,"%-8s(this=%p): { size=(%u,%u,%u,%u), data=(%s*)%p (%s)",
		   title?title:"CImg",(void*)this,
		   width,height,depth,dim,pixel_type(),(void*)data,
		   is_shared?"shared":"not shared");
	  if (is_empty()) { std::fprintf(stderr,", [Undefined pixel data] }\n"); return *this; }
	  if (print_flag>=1) { 
		const CImgStats st(*this);
		std::fprintf(stderr,", min=%g, mean=%g [var=%g], max=%g, pmin=(%d,%d,%d,%d), pmax=(%d,%d,%d,%d)",
			 st.min,st.mean,st.variance,st.max,st.xmin,st.ymin,st.zmin,st.vmin,st.xmax,st.ymax,st.zmax,st.vmax); 
	  }     
	  if (print_flag>=2 || size()<=16) {
	std::fprintf(stderr," }\n%s = [ ",title?title:"data");
	cimg_mapXYZV(*this,x,y,z,k) 
	  std::fprintf(stderr,"%g%s",(double)(*this)(x,y,z,k),
			   ((x+1)*(y+1)*(z+1)*(k+1)==(int)size()?" ]\n":(((x+1)%width==0)?" ; ":" ")));
	  } else std::fprintf(stderr," }\n");
	  return *this;
	}

	//! Display informations about the image on the standart output.
	const CImg& print(const unsigned int print_flag) const { return print(0,print_flag); }
  
	//@}
	//------------------------------------------
	//
	//! \name Arithmetic and Boolean operators
	//@{
	//------------------------------------------
  
	//! Assign an image to the instance image.
	/**
	   \param img Image to copy.
	   
	   - Replace the instance image by a copy of the image \c img.
	   - The assignement is faster if input and output images have same template types.
	   - Otherwise, pixel values are casted as in C.
	   
	   \par example:
	   \code
	   CImg<unsigned char> img("foo.jpg");     // Load image from a JPEG file.
	   CImg<unsigned char> dest1;              // Define an empty image of unsigned char pixels.
	   CImg<float> dest2;                      // Define an empty image of float pixels.
	   dest1 = img;                            // Fast copy of img to dest1.
	   dest2 = img;                            // Copy of img to dest2, with conversion of pixel to float values.
	   \endcode
	**/

	//! Assign a value to each image pixel of the instance image.
	/**
	   \param val Value to assign.

	   - Replace all pixel values of the instance image by \c val.
	   - Can be used to easily initialize an image.
	   
	   \par example:
	   \code
	   CImg<float> img(100,100);   // Define a 100x100 greyscale image.
	   img = 3.14f;                // Set all pixel values to 3.14.
	   \endcode
	   
	   \sa fill().
	**/
	CImg& operator=(const T& val) { 
	  return fill(val); 
	}

	//! Assign values of a C-array to the instance image.
	/**
	   \param buf Pointer to a C-style array having a size of (at least) <tt>this->size()</tt>.
	   
	   - Replace pixel values by the content of the array \c buf.
	   - Warning : the value types in the array and in the image must be the same.
	   
	   \par example:
	   \code
	   float tab[4*4] = { 1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16 };  // Define a 4x4 matrix in C-style.
	   CImg<float> matrice(4,4);                                        // Define a 4x4 greyscale image.
	   matrice = tab;                                                   // Fill the image by the values in tab.
	   \endcode
	**/
	CImg& operator=(const T *buf) {
	  if (buf) std::memcpy(data,buf,size()*sizeof(T));
	  else empty();
	  return *this; 
	}
	  
	//! Addition.
	/**
	   \param val Constant value added to each image pixel of the instance image.
	**/
	CImg operator+(const T& val) const { 
	  return CImg<T>(*this,true)+=val; 
	}

	//! Addition.
	/**
	   \param img Image added to the instance image.
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> operator+(const CImg<t>& img) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this)+=img; 
	}

	//! Addition.
	friend CImg operator+(const T& val, const CImg<T>& img) {
	  return CImg<T>(img)+=val; 
	}    

	//! In-place addition.
	/** This is the in-place version of operator+(). **/
	CImg& operator+=(const T& val) { 
	  cimg_map(*this,ptr,T) (*ptr)+=val; 
	  return *this; 
	}

	//! In-place addition.
	/** This is the in-place version of operator+(). **/
	template<typename t> CImg& operator+=(const CImg<t>& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  t *ptrs = img.data+smin;
	  for (T *ptrd = data+smin; ptrd>data; --ptrd, (*ptrd)=(T)((*ptrd)+(*(--ptrs))));
	  return *this;
	}

	//! In-place increment.
	/** Equivalent to \c operator+=(1). **/
	CImg& operator++() { 
	  return (*this)+=1; 
	}

	//! Substraction.
	/**
	   \param val Constant value substracted to each image pixel of the instance image.      
	**/
	CImg operator-(const T& val) const { 
	  return CImg<T>(*this,true)-=val; 
	}

	//! Substraction.
	/**
	   \param img Image substracted to the instance image.
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> operator-(const CImg<t>& img) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this)-=img; 
	}

	//! Substraction.
	friend CImg operator-(const T& val, const CImg<T>& img) { 
	  return CImg<T>(img.width,img.height,img.depth,img.dim,val)-=img; 
	}

	//! In-place substraction.
	/** This is the in-place version of operator-(). **/
	CImg& operator-=(const T& val) {
	  cimg_map(*this,ptr,T) (*ptr)-=val;
	  return *this; 
	}

	//! In-place substraction.
	/** This is the in-place version of operator-(). **/
	template<typename t> CImg& operator-=(const CImg<t>& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  t *ptrs = img.data+smin;
	  for (T *ptrd = data+smin; ptrd>data; --ptrd, (*ptrd)=(T)((*ptrd)-(*(--ptrs))));
	  return *this;
	}

	//! In-place decrement.
	/** Equivalent to \c operator-=(1). **/
	CImg& operator--() {
	  return (*this)-=1; 
	}

	//! Multiplication.
	/**
	   \param val Constant value multiplied to each image pixel of the instance image.
	**/
	CImg operator*(const double val) const {
	  return CImg<T>(*this,true)*=val; 
	}

	//! Multiplication.
	/**
	   Matrix multiplication.
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> operator*(const CImg<t>& img) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  if (width!=img.height) 
		throw CImgArgumentException("CImg<%s>::operator*() : can't multiply a matrix *this = (%ux%u) by a matrix (%ux%u)",
									pixel_type(),width,height,img.width,img.height);
	  CImg<restype> res(img.width,height);
	  restype val;
	  cimg_mapXY(res,i,j) { val=0; cimg_mapX(*this,k) val+=(*this)(k,j)*img(i,k); res(i,j) = val; }
	  return res;
	}

	//! Matrix multiplication with an image list
	template<typename t> CImgl<typename cimg::largest<T,t>::type> operator*(const CImgl<t>& list) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  CImgl<restype> res(list.size);
	  cimgl_map(res,l) res[l] = (*this)*list[l];
	  return res;
	}
	
	//! Multiplication.
	friend CImg operator*(const double val, const CImg &img) {
	  return CImg<T>(img)*=val; 
	}

	//! In-place multiplication.
	/** This is the in-place version of operator*(). **/
	CImg& operator*=(const double val) { 
	  cimg_map(*this,ptr,T) (*ptr)=(T)((*ptr)*val);
	  return *this; 
	}

	//! In-place multiplication.
	/** This is the in-place version of operator*(). **/
	template<typename t> CImg& operator*=(const CImg<t>& img) { 
	  return ((*this)*img).swap(*this); 
	}
  
	//! Division.
	/**
	   \param val Constant value divided to each image pixel of the instance image.
	**/
	CImg operator/(const double val) const {
	  return CImg<T>(*this,true)/=val; 
	}
	
	//! In-place division.
	/** This is the in-place version of operator/(). **/
	CImg& operator/=(const double val) {
	  cimg_map(*this,ptr,T) (*ptr)=(T)((*ptr)/val);
	  return *this; 
	}

	//! Modulo.
	/**
	   \param val Constant valued used for a modulo on each image pixel.
	**/
	CImg operator%(const T& val) const { 
	  return CImg<T>(*this,true)%=val; 
	}  

	//! Modulo.
	/**
	   \param img Image whose values are used for a modulo.
	**/
	CImg operator%(const CImg& img) const { return CImg<T>(*this,true)%=img; }

	//! In-place modulo.
	/** This is the in-place version of operator%(). **/
	CImg& operator%=(const T& val) { cimg_map(*this,ptr,T) (*ptr)%=val; return *this; }
 
	//! In-place modulo.
	/** This is the in-place version of operator%(). **/
	CImg& operator%=(const CImg& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)%=*(--ptrs));
	  return *this;
	}

	//! Bitwise AND.
	/**
	   \param val Constant valued used for a bitwise AND on each image pixel.
	**/
	CImg operator&(const T& val) const { return CImg<T>(*this,true)&=val; }
   
	//! Bitwise AND.
	/**
	   \param img Image whose value are used for the AND.
	**/
	CImg operator&(const CImg& img) const { return CImg<T>(*this,true)&=img; } 

	//! In-place bitwise AND.
	/** This is the in-place version of operator&(). **/
	CImg& operator&=(const T& val) { cimg_map(*this,ptr,T) (*ptr)&=val; return *this; }

	//! In-place bitwise AND.
	/** This is the in-place version of operator&=(). **/
	CImg& operator&=(const CImg& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)&=*(--ptrs));
	  return *this;
	}

	//! Bitwise OR.
	/**
	   \param val Constant valued used for a bitwise OR on each image pixel.
	**/
	CImg operator|(const T& val) const { return CImg<T>(*this,true)|=val; }
	
	//! Bitwise OR.
	/**
	   \param img Image whose values are used for the OR.
	**/
	CImg operator|(const CImg& img) const { return CImg<T>(*this,true)|=img; }

	//! In-place bitwise OR.
	/** This is the in-place version of operator|(). **/
	CImg& operator|=(const T& val) { cimg_map(*this,ptr,T) (*ptr)|=val; return *this; }

	//! In-place bitwise OR.
	/** This is the in-place version of operator|(). **/
	CImg& operator|=(const CImg& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)|=*(--ptrs));
	  return *this;
	}

	//! Bitwise XOR.
	/**
	   \param val Constant valued used for a bitwise XOR on each image pixel.
	**/
	CImg operator^(const T& val) const { return CImg<T>(*this,true)^=val; }

	//! Bitwise XOR.
	/**
	   \param img Image whose values are used for the XOR.
	 **/
	CImg operator^(const CImg& img) const { return CImg<T>(*this,true)^=img; }  

	//! In-place bitwise XOR.
	/** This is the in-place version of operator^(). **/
	CImg& operator^=(const T& val) { cimg_map(*this,ptr,T) (*ptr)^=val; return *this; }

	//! In-place bitwise XOR.
	/** This is the in-place version of operator^(). **/
	CImg& operator^=(const CImg& img) {
	  const unsigned int smin = cimg::min(size(),img.size());
	  for (T *ptrs=img.data+smin, *ptrd=data+smin; ptrd>data; *(--ptrd)^=*(--ptrs));
	  return *this;
	}
   
	//! Boolean NOT.
	CImg operator!() const {
	  CImg<T> res(*this,false);
	  const T *ptrs = end();
	  cimg_map(res,ptrd,T) *ptrd=!(*(--ptrs));
	  return res;
	}

	//! Bitwise NOT.
	CImg operator~() const {
	  CImg<T> res(*this,false);
	  const T *ptrs = end();
	  cimg_map(res,ptrd,T) *ptrd=~(*(--ptrs));
	  return res;
	}

	//! Boolean equality.
	template<typename t> bool operator==(const CImg<t>& img) const {
	  const unsigned int siz = size();
	  bool vequal = true;
	  if (siz!=img.size()) return false;
	  t *ptrs=img.data+siz;
	  for (T *ptrd=data+siz; vequal && ptrd>data; vequal=vequal&&((*(--ptrd))==(*(--ptrs))));
	  return vequal;
	}

	//! Boolean difference.
	template<typename t> bool operator!=(const CImg<t>& img) const { return !((*this)==img); }

	//@}
	//---------------------------------------
	//
	//! \name Usual mathematical operations
	//@{
	//---------------------------------------
	 
	//! Apply a R->R function on all image value.
	template<typename ts, typename td> CImg& apply(td (*func)(ts)) {
	  cimg_map(*this,ptr,T) *ptr = (T)func(*ptr);
	  return *this;
	}  
	
	//! Return an image where each pixel value is equal to func(x).
	template<typename ts, typename td> CImg<typename cimg::largest<T,td>::type> get_apply(td (*func)(ts)) {
	  typedef typename cimg::largest<T,td>::type restype;
	  return CImg<restype>(*this).apply(func);
	}

	//! In-place pointwise multiplication between \c *this and \c img.
	/**
	   This is the in-place version of get_mul().
	   \sa get_mul().
	**/
	template<typename t> CImg& mul(const CImg<t>& img) {
	  t *ptrs = img.data;
	  T *ptrf = data + cimg::min(size(),img.size());
	  for (T* ptrd = data; ptrd<ptrf; ptrd++) (*ptrd)=(T)(*ptrd*(*(ptrs++)));
	  return *this;
	}

	//! Pointwise multiplication between \c *this and \c img.
	/**
	   \param img Argument of the multiplication.
	   - if \c *this and \c img have different size, the multiplication is applied on the maximum possible range.
	   \sa get_div(),mul(),div()
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> get_mul(const CImg<t>& img) const { 
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this).mul(img); 
	}
  
	//! Replace the image by the pointwise division between \p *this and \p img.
	/**
	   This is the in-place version of get_div().
	   \see get_div().
	**/
	template<typename t> CImg& div(const CImg<t>& img) {
	  t *ptrs = img.data;
	  T *ptrf = data + cimg::min(size(),img.size());
	  for (T* ptrd = data; ptrd<ptrf; ptrd++) (*ptrd)=(T)(*ptrd/(*(ptrs++)));
	  return *this;
	}

	//! Return an image from a pointwise division between \p *this and \p img.
	/**
	   \param img = argument of the division.
	   \note if \c *this and \c img have different size, the division is applied
	   only on possible values.
	   \see get_mul(),mul(),div()
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> get_div(const CImg<t>& img) const { 
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this).div(img); 
	}
  
	//! Replace the image by the pointwise max operator between \p *this and \p img
	/**
	   This is the in-place version of get_max().       
	   \see get_max().
	**/
	template<typename t> CImg& max(const CImg<t>& img) {
	  t *ptrs = img.data;
	  T *ptrf = data + cimg::min(size(),img.size());
	  for (T* ptrd = data; ptrd<ptrf; ptrd++) (*ptrd)=cimg::max((T)(*(ptrs++)),*ptrd);
	  return *this;
	}

	//! Return the image corresponding to the max value for each pixel.
	/**
	   \param img = second argument of the max operator (the first one is *this).
	   \see max(), min(), get_min()
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> get_max(const CImg<t>& img) const { 
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this).max(img); 
	}

	//! Replace the image by the pointwise max operator between \p *this and \p val
	/**
	   This is the in-place version of get_max().   
	   \see get_max().
	**/
	CImg& max(const T& val) {
	  cimg_map(*this,ptr,T) (*ptr)=cimg::max(*ptr,val);
	  return *this;
	}

	//! Return the image corresponding to the max value for each pixel.
	/**
	   \param val = second argument of the max operator (the first one is *this).
	   \see max(), min(), get_min()
	**/
	CImg get_max(const T& val) const { 
	  return CImg<T>(*this,true).max(val); 
	}

	//! Replace the image by the pointwise min operator between \p *this and \p img
	/**
	   This is the in-place version of get_min().
	   \see get_min().
	**/
	template<typename t> CImg& min(const CImg<t>& img) {
	  t *ptrs = img.data;
	  T *ptrf = data + cimg::min(size(),img.size());
	  for (T* ptrd = data; ptrd<ptrf; ptrd++) (*ptrd)=cimg::min((T)(*(ptrs++)),*ptrd);
	  return *this;
	}
	//! Return the image corresponding to the min value for each pixel.
	/**
	   \param img = second argument of the min operator (the first one is *this).
	   \see min(), max(), get_max()
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> get_min(const CImg<t>& img) const { 
	  typedef typename cimg::largest<T,t>::type restype;
	  return CImg<restype>(*this).min(img); 
	}

	//! Replace the image by the pointwise min operator between \p *this and \p val
	/**
	   This is the in-place version of get_min().
	   \see get_min().
	**/
	CImg& min(const T& val) {
	  cimg_map(*this,ptr,T) (*ptr)=cimg::min(*ptr,val);
	  return *this;
	}

	//! Return the image corresponding to the min value for each pixel.
	/**
	   \param val = second argument of the min operator (the first one is *this).
	   \see min(), max(), get_max()
	**/
	CImg get_min(const T& val) const { 
	  return CImg<T>(*this,true).min(val); 
	}
	
	//! Replace each image pixel by its square root.
	/**
	   \see get_sqrt()
	**/
	CImg& sqrt() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::sqrt((double)(*ptr));
	  return *this;
	}

	//! Return the image of the square root of the pixel values.
	/**
	   \see sqrt()
	**/
	CImg<typename cimg::largest<T,float>::type> get_sqrt() const { 
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).sqrt(); 
	}
  
	//! Replace each image pixel by its log.
	/**
	   \see get_log(), log10(), get_log10()
	**/
	CImg& log() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::log((double)(*ptr));
	  return *this;
	}

	//! Return the image of the log of the pixel values.
	/**
	   \see log(), log10(), get_log10()
	**/
	CImg<typename cimg::largest<T,float>::type> get_log() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).log(); 
	}

	//! Replace each image pixel by its log10.
	/**
	   \see get_log10(), log(), get_log()
	**/
	CImg& log10() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::log10((double)(*ptr));
	  return *this;
	}

	//! Return the image of the log10 of the pixel values.
	/**
	   \see log10(), log(), get_log()
	**/
	CImg<typename cimg::largest<T,float>::type> get_log10() const { 
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).log10(); 
	}

	//! Replace each image pixel by its power by \p p.
	/**
	   \param p = power
	   \see get_pow(), sqrt(), get_sqrt()
	**/
	CImg& pow(const double p) {
	  if (p==0) return fill(1);
	  if (p==1) return *this;
	  if (p==2) { cimg_map(*this,ptr,T) { const T& val = *ptr; *ptr=val*val; } return *this; }
	  if (p==3) { cimg_map(*this,ptr,T) { const T& val = *ptr; *ptr=val*val*val; } return *this; }
	  if (p==4) { cimg_map(*this,ptr,T) { const T& val = *ptr; *ptr=val*val*val*val; } return *this; }
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::pow((double)(*ptr),p);
	  return *this;
	}
   
	//! Return the image of the square root of the pixel values.
	/**
	   \param p = power
	   \see pow(), sqrt(), get_sqrt()
	**/
	CImg<typename cimg::largest<T,float>::type> get_pow(const double p) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).pow(p); 
	}
	
	//! Return each image pixel (*this)(x,y,z,k) by its power by \p img(x,y,z,k)
	/**
	   In-place version
	**/
	template<typename t> CImg& pow(const CImg<t>& img) {
	  t *ptrs = img.data;
	  T *ptrf = data + cimg::min(size(),img.size());
	  for (T* ptrd = data; ptrd<ptrf; ptrd++) (*ptrd)=(T)std::pow((double)*ptrd,(double)(*(ptrs++)));
	  return *this;
	}

	//! Return each image pixel (*this)(x,y,z,k) by its power by \p img(x,y,z,k)
	template<typename t> CImg<typename cimg::largest<T,float>::type> get_pow(const CImg<t>& img) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).pow(img); 
	}
  
	//! Replace each pixel value by its absolute value.
	/**
	   \see get_abs()
	**/
	CImg& abs() {
	  cimg_map(*this,ptr,T) (*ptr)=cimg::abs(*ptr);
	  return *this;
	}

	//! Return the image of the absolute value of the pixel values.
	/**
	   \see abs()
	**/
	CImg<typename cimg::largest<T,float>::type> get_abs() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).abs(); 
	}
  
	//! Replace each image pixel by its cosinus.
	/**
	   \see get_cos(), sin(), get_sin(), tan(), get_tan()
	**/
	CImg& cos() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::cos((double)(*ptr));
	  return *this;
	}

	//! Return the image of the cosinus of the pixel values.
	/**
	   \see cos(), sin(), get_sin(), tan(), get_tan()
	**/
	CImg<typename cimg::largest<T,float>::type> get_cos() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).cos(); 
	}
 
	//! Replace each image pixel by its sinus.
	/**
	   \see get_sin(), cos(), get_cos(), tan(), get_tan()
	**/
	CImg& sin() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::sin((double)(*ptr));
	  return *this;
	}

	//! Return the image of the sinus of the pixel values.
	/**
	   \see sin(), cos(), get_cos(), tan(), get_tan()
	**/
	CImg<typename cimg::largest<T,float>::type> get_sin() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).sin(); 
	}
  
	//! Replace each image pixel by its tangent.
	/**
	   \see get_tan(), cos(), get_cos(), sin(), get_sin()
	**/
	CImg& tan() {
	  cimg_map(*this,ptr,T) (*ptr)=(T)std::tan((double)(*ptr));
	  return *this;
	}

	//! Return the image of the tangent of the pixel values.
	/**
	   \see tan(), cos(), get_cos(), sin(), get_sin()
	**/
	CImg<typename cimg::largest<T,float>::type> get_tan() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).tan(); 
	}

	//! Return the MSE (Mean-Squared Error) between two images.
	template<typename t> double MSE(const CImg<t>& img) const {
	  if (img.size()!=size())
	throw CImgArgumentException("CImg<%s>::MSE() : Instance image (%u,%u,%u,%u) and given image (%u,%u,%u,%u) have different dimensions.",
					pixel_type(),width,height,depth,dim,img.width,img.height,img.depth,img.dim);
	  
	  double vMSE = 0;
	  const t* ptr2 = img.end();
	  cimg_map(*this,ptr1,T) {
	const double diff = (double)*ptr1 - (double)*(--ptr2);
	vMSE += diff*diff;
	  }
	  vMSE/=img.size();
	  return vMSE;
	}

	//! Return the PSNR between two images.
	template<typename t> double PSNR(const CImg<t>& img, const double valmax=255.0) const {
	  const double vMSE = std::sqrt(MSE(img));
	  return (vMSE!=0)?(20*std::log10(valmax/vMSE)):(cimg::type<double>::max());
	}
  
	//@}
	//-----------------------------------
	//
	//! \name Usual image transformation
	//@{
	//-----------------------------------
	
	//! Fill an image by a value \p val.
	/**
	   \param val = fill value
	   \note All pixel values of the instance image will be initialized by \p val.
	   \see operator=().
	**/
	CImg& fill(const T& val) {
	  if (!is_empty()) {
	if (val!=0 && sizeof(T)!=1) cimg_map(*this,ptr,T) *ptr=val;
	else std::memset(data,(int)val,size()*sizeof(T));
	  }
	  return *this;
	}

	CImg get_fill(const T& val) const {
	  return CImg<T>(*this).fill(val);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 respectively.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	**/
	CImg& fill(const T& val0,const T& val1) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-1;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; }
	if (ptr!=ptr_end+1) *(ptr++)=val0;
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1) const {
	  return CImg<T>(*this).fill(val0,val1);
	}
	
	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-2;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; }     
	ptr_end+=2;
	switch (ptr_end-ptr) {
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2) const {
	  return CImg<T>(*this).fill(val0,val1,val2);
	}
	
	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-3;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; }
	ptr_end+=3;
	switch (ptr_end-ptr) {
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a val4.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-4;
	for (ptr=data; ptr<ptr_end; ) { *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; }
	ptr_end+=4;
	switch (ptr_end-ptr) {
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4);
	}
	
	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a val4 and \a val5
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4,const T& val5) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-5;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	}
	ptr_end+=5;
	switch (ptr_end-ptr) {
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a val4 and \a val5
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
		   const T& val4,const T& val5,const T& val6) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-6; 
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; *(ptr++)=val6;
	}
	ptr_end+=6;
	switch (ptr_end-ptr) {
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val7.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
		   const T& val4,const T& val5,const T& val6,const T& val7) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-7;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3;
	  *(ptr++)=val4; *(ptr++)=val5; *(ptr++)=val6; *(ptr++)=val7;
	}
	ptr_end+=7;
	switch (ptr_end-ptr) {
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val8.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,
		   const T& val3,const T& val4,const T& val5,
		   const T& val6,const T& val7,const T& val8) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-8;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; 
	  *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	  *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8;
	}
	ptr_end+=8;
	switch (ptr_end-ptr) {
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7, const T& val8) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val9.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	   \param val9 = fill value 10
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4,
		   const T& val5,const T& val6,const T& val7,const T& val8,const T& val9) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-9;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4;
	  *(ptr++)=val5; *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8; *(ptr++)=val9;
	}
	ptr_end+=9;
	switch (ptr_end-ptr) {
	case 9: *(--ptr_end)=val8;
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7, const T& val8, const T& val9) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val11.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	   \param val9 = fill value 10
	   \param val10 = fill value 11
	   \param val11 = fill value 12
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
		   const T& val4,const T& val5,const T& val6,const T& val7,
		   const T& val8,const T& val9,const T& val10,const T& val11) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-11;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	  *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8; *(ptr++)=val9; *(ptr++)=val10; *(ptr++)=val11;
	}
	ptr_end+=11;
	switch (ptr_end-ptr) {
	case 11: *(--ptr_end)=val10;
	case 10: *(--ptr_end)=val9;
	case 9: *(--ptr_end)=val8;
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7, const T& val8, const T& val9, const T& val10, const T& val11) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9,val10,val11);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val11.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	   \param val9 = fill value 10
	   \param val10 = fill value 11
	   \param val11 = fill value 12
	   \param val12 = fill value 13
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
		   const T& val4,const T& val5,const T& val6,const T& val7,
		   const T& val8,const T& val9,const T& val10,const T& val11,
		   const T& val12) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-12;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	  *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8; *(ptr++)=val9; *(ptr++)=val10; *(ptr++)=val11;
	  *(ptr++)=val12;
	}
	ptr_end+=12;
	switch (ptr_end-ptr) {
	case 12: *(--ptr_end)=val11;
	case 11: *(--ptr_end)=val10;
	case 10: *(--ptr_end)=val9;
	case 9: *(--ptr_end)=val8;
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7, const T& val8, const T& val9, const T& val10, const T& val11,
		  const T& val12, const T& val13) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9,val10,val11,val12,val13);
	}


	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val15.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	   \param val9 = fill value 10
	   \param val10 = fill value 11
	   \param val11 = fill value 12
	   \param val12 = fill value 13
	   \param val13 = fill value 14
	   \param val14 = fill value 15
	   \param val15 = fill value 16
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,
		   const T& val4,const T& val5,const T& val6,const T& val7,
			   const T& val8,const T& val9,const T& val10,const T& val11,
		   const T& val12,const T& val13,const T& val14,const T& val15) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-15;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4; *(ptr++)=val5; 
	  *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8; *(ptr++)=val9; *(ptr++)=val10; *(ptr++)=val11;
	  *(ptr++)=val12; *(ptr++)=val13; *(ptr++)=val14; *(ptr++)=val15;
	}
	ptr_end+=15;
	switch (ptr_end-ptr) {
	case 15: *(--ptr_end)=val14;
	case 14: *(--ptr_end)=val13;
	case 13: *(--ptr_end)=val12;
	case 12: *(--ptr_end)=val11;
	case 11: *(--ptr_end)=val10;
	case 10: *(--ptr_end)=val9;
	case 9: *(--ptr_end)=val8;
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0, const T& val1, const T& val2, const T& val3, const T& val4, const T& val5,
		  const T& val6, const T& val7, const T& val8, const T& val9, const T& val10, const T& val11,
		  const T& val12, const T& val13, const T& val14, const T& val15) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9,val10,val11,val12,val13,val14,val15);
	}

	//! Fill sequentially all pixel values with values \a val0 and \a val1 and \a val2 and \a val3 and \a ... and \a val24.
	/**
	   \param val0 = fill value 1
	   \param val1 = fill value 2
	   \param val2 = fill value 3
	   \param val3 = fill value 4
	   \param val4 = fill value 5
	   \param val5 = fill value 6
	   \param val6 = fill value 7
	   \param val7 = fill value 8
	   \param val8 = fill value 9
	   \param val9 = fill value 10
	   \param val10 = fill value 11
	   \param val11 = fill value 12
	   \param val12 = fill value 13
	   \param val13 = fill value 14
	   \param val14 = fill value 15
	   \param val15 = fill value 16
	   \param val16 = fill value 17
	   \param val17 = fill value 18
	   \param val18 = fill value 19
	   \param val19 = fill value 20
	   \param val20 = fill value 21
	   \param val21 = fill value 22
	   \param val22 = fill value 23
	   \param val23 = fill value 24
	   \param val24 = fill value 25
	**/
	CImg& fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4,
		   const T& val5,const T& val6,const T& val7,const T& val8,const T& val9,
		   const T& val10,const T& val11,const T& val12,const T& val13,const T& val14,
		   const T& val15,const T& val16,const T& val17,const T& val18,const T& val19,
		   const T& val20,const T& val21,const T& val22,const T& val23,const T& val24) {
	  if (!is_empty()) {
	T *ptr, *ptr_end = end()-24;
	for (ptr=data; ptr<ptr_end; ) {
	  *(ptr++)=val0; *(ptr++)=val1; *(ptr++)=val2; *(ptr++)=val3; *(ptr++)=val4;
	  *(ptr++)=val5; *(ptr++)=val6; *(ptr++)=val7; *(ptr++)=val8; *(ptr++)=val9;
	  *(ptr++)=val10; *(ptr++)=val11; *(ptr++)=val12; *(ptr++)=val13; *(ptr++)=val14; 
	  *(ptr++)=val15; *(ptr++)=val16; *(ptr++)=val17; *(ptr++)=val18; *(ptr++)=val19;
	  *(ptr++)=val20; *(ptr++)=val21; *(ptr++)=val22; *(ptr++)=val23; *(ptr++)=val24;
	}
	ptr_end+=24;
	switch (ptr_end-ptr) {
	case 24: *(--ptr_end)=val23;
	case 23: *(--ptr_end)=val22;
	case 22: *(--ptr_end)=val21;
	case 21: *(--ptr_end)=val20;
	case 20: *(--ptr_end)=val19;
	case 19: *(--ptr_end)=val18;
	case 18: *(--ptr_end)=val17;
	case 17: *(--ptr_end)=val16;
	case 16: *(--ptr_end)=val15;
	case 15: *(--ptr_end)=val14;
	case 14: *(--ptr_end)=val13;
	case 13: *(--ptr_end)=val12;
	case 12: *(--ptr_end)=val11;
	case 11: *(--ptr_end)=val10;
	case 10: *(--ptr_end)=val9;
	case 9: *(--ptr_end)=val8;
	case 8: *(--ptr_end)=val7;
	case 7: *(--ptr_end)=val6;
	case 6: *(--ptr_end)=val5;
	case 5: *(--ptr_end)=val4;
	case 4: *(--ptr_end)=val3;
	case 3: *(--ptr_end)=val2;
	case 2: *(--ptr_end)=val1;
	case 1: *(--ptr_end)=val0;
	}
	  }
	  return *this;
	}

	CImg get_fill(const T& val0,const T& val1,const T& val2,const T& val3,const T& val4,
		  const T& val5,const T& val6,const T& val7,const T& val8,const T& val9,
		  const T& val10,const T& val11,const T& val12,const T& val13,const T& val14,
		  const T& val15,const T& val16,const T& val17,const T& val18,const T& val19,
		  const T& val20,const T& val21,const T& val22,const T& val23,const T& val24) const {
	  return CImg<T>(*this).fill(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9,
				 val10,val11,val12,val13,val14,val15,val16,val17,val18,val19,
				 val20,val21,val22,val23,val24);
	}

	//! Linear normalization of the pixel values between \a a and \a b.
	/**
	   \param a = minimum pixel value after normalization.
	   \param b = maximum pixel value after normalization.
	   \see get_normalize(), cut(), get_cut().
	**/
	CImg& normalize(const T& a,const T& b) {
	  if (!is_empty()) {
	const CImgStats st(*this,false);
	if (st.min==st.max) return fill(0);
	if (st.min!=a || st.max!=b) cimg_map(*this,ptr,T) *ptr=(T)((*ptr-st.min)/(st.max-st.min)*(b-a)+a);
	  }
	  return *this;
	}

	//! Return the image of normalized values.
	/**
	   \param a = minimum pixel value after normalization.
	   \param b = maximum pixel value after normalization.
	   \see normalize(), cut(), get_cut().
	**/
	CImg get_normalize(const T& a,const T& b) const { 
	  return CImg<T>(*this,true).normalize(a,b); 
	}
  
	//! Cut pixel values between \a a and \a b.
	/**
	   \param a = minimum pixel value after cut.
	   \param b = maximum pixel value after cut.
	   \see get_cut(), normalize(), get_normalize().
	**/
	CImg& cut(const T& a, const T& b) {
	  if (!is_empty())
	cimg_map(*this,ptr,T) *ptr = (*ptr<a)?a:((*ptr>b)?b:*ptr);
	  return *this;
	}

	//! Return the image of cutted values.
	/**
	   \param a = minimum pixel value after cut.
	   \param b = maximum pixel value after cut.
	   \see cut(), normalize(), get_normalize().
	**/
	CImg get_cut(const T& a, const T& b) const { return CImg<T>(*this,true).cut(a,b); }

	//! Quantize pixel values into \n levels.
	/**
	   \param n = number of quantification levels
	   \see get_quantize().
	**/
	CImg& quantize(const unsigned int n=256) {
	  if (!is_empty()) {
	if (!n) throw CImgArgumentException("CImg<%s>::quantize() : Cannot quantize image to 0 values.",
						pixel_type());       
	const CImgStats st(*this,false);
	const double range = st.max-st.min;
	if (range>0) cimg_map(*this,ptr,T) {
	  const unsigned int val = (unsigned int)((*ptr-st.min)*n/range);
	  *ptr = (T)(st.min + cimg::min(val,n-1)*range);
	}
	  }
	  return *this;
	}
	
	//! Return a quantified image, with \n levels.
	/**
	   \param n = number of quantification levels
	   \see quantize().
	**/
	CImg get_quantize(const unsigned int n=256) const { return CImg<T>(*this,true).quantize(n); }

	//! Threshold the image.
	/**
	   \param thres = threshold
	   \see get_threshold().
	**/	
	CImg& threshold(const T& thres) {
	  if (!is_empty())
	cimg_map(*this,ptr,T) *ptr = *ptr<=thres?(T)0:(T)1;
	  return *this;
	}

	//! Return a thresholded image.
	/**
	   \param thres = threshold.
	   \see threshold().
	**/
	CImg get_threshold(const T& thres) const {
	  return CImg<T>(*this,true).threshold(thres); 
	}
  
	//! Return a rotated image.
	/**
	   \param angle = rotation angle (in degrees).
	   \param cond = rotation type. can be :
	   - 0 = zero-value at borders
	   - 1 = repeat image at borders
	   - 2 = zero-value at borders and linear interpolation
	   \note Returned image will probably have a different size than the instance image *this.
	   \see rotate()
	**/
	CImg get_rotate(const float angle, const unsigned int cond=3) const {
	  if (is_empty()) return CImg<T>();
	  CImg dest;
	  const float nangle = cimg::mod(angle,360.0f), rad = (float)((nangle*cimg::PI)/180.0),
	ca=(float)std::cos(rad), sa=(float)std::sin(rad);
	  if (cond!=1 && cimg::mod(nangle,90.0f)==0) { // optimized version for orthogonal angles
	const int wm1 = dimx()-1, hm1 = dimy()-1;
	const int iangle = (int)nangle/90;
	switch (iangle) {
	case 1: {
	  dest.assign(height,width,depth,dim); 
	  cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = (*this)(y,hm1-x,z,v); 
	} break; 
	case 2: {
	  dest.assign(*this,false);
	  cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = (*this)(wm1-x,hm1-y,z,v); 
	} break;
	case 3: {
	  dest.assign(height,width,depth,dim); 
	  cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = (*this)(wm1-y,x,z,v); 
	} break;
	default: 
	  return *this;        
	}
	  } else { // generic version
	const float 
	  ux  = (float)(cimg::abs(width*ca)),  uy  = (float)(cimg::abs(width*sa)),
	  vx  = (float)(cimg::abs(height*sa)), vy  = (float)(cimg::abs(height*ca)),
	  w2  = 0.5f*width,           h2  = 0.5f*height,
	  dw2 = 0.5f*(ux+vx),         dh2 = 0.5f*(uy+vy);
	dest.assign((int)(ux+vx), (int)(uy+vy),depth,dim);
	switch (cond) {
	case 0: { 
	  cimg_mapXY(dest,x,y)
		cimg_mapZV(*this,z,v) 
		dest(x,y,z,v) = pix2d((int)(w2 + (x-dw2)*ca + (y-dh2)*sa),(int)(h2 - (x-dw2)*sa + (y-dh2)*ca),z,v,0);
	} break;
	case 1: {
	  cimg_mapXY(dest,x,y)
		cimg_mapZV(*this,z,v) 
		dest(x,y,z,v) = (*this)(cimg::mod((int)(w2 + (x-dw2)*ca + (y-dh2)*sa),width),
					cimg::mod((int)(h2 - (x-dw2)*sa + (y-dh2)*ca),height),z,v);
	} break;
	case 2: {
	  cimg_mapXY(dest,x,y) {
		const float X = w2 + (x-dw2)*ca + (y-dh2)*sa, Y = h2 - (x-dw2)*sa + (y-dh2)*ca;
		cimg_mapZV(*this,z,v) dest(x,y,z,v) = (T)linear_pix2d(X,Y,z,v,0);
	  }
	} break; 
	default: {
	  cimg_mapXY(dest,x,y) {
		const float X = w2 + (x-dw2)*ca + (y-dh2)*sa, Y = h2 - (x-dw2)*sa + (y-dh2)*ca;
		cimg_mapZV(*this,z,v) dest(x,y,z,v) = (T)cubic_pix2d(X,Y,z,v,0);
	  }	  
	} break;	  
	}
	  }
	  return dest;
	}
  
	//! Rotate the image 
	/**
	   \param angle = rotation angle (in degrees).
	   \param cond = rotation type. can be :
	   - 0 = zero-value at borders
	   - 1 = repeat image at borders
	   - 2 = zero-value at borders and linear interpolation
	   \see get_rotate()
	**/
	CImg& rotate(const float angle,const unsigned int cond=3) { return get_rotate(angle,cond).swap(*this); }
  
	//! Return a rotated image around the point (\c cx,\c cy).
	/**
	   \param angle = rotation angle (in degrees).
	   \param cx = X-coordinate of the rotation center.
	   \param cy = Y-coordinate of the rotation center.
	   \param zoom = zoom.
	   \param cond = rotation type. can be :
	   - 0 = zero-value at borders
	   - 1 = repeat image at borders
	   - 2 = zero-value at borders and linear interpolation
	   \see rotate()
	**/
	CImg get_rotate(const float angle,const float cx,const float cy,const float zoom=1,const unsigned int cond=3) const {
	  if (is_empty()) return CImg<T>();
	  CImg dest(*this,false);
	  const float nangle = cimg::mod(angle,360.0f), rad = (float)((nangle*cimg::PI)/180.0),
	ca=(float)std::cos(rad)/zoom, sa=(float)std::sin(rad)/zoom;
	  if (cond!=1 && zoom==1 && cimg::mod(nangle,90.0f)==0) { // optimized version for orthogonal angles
	const int iangle = (int)nangle/90;
	switch (iangle) {
	case 1: {
	  dest.fill(0);
	  const unsigned int
		xmin = cimg::max(0,(dimx()-dimy())/2), xmax = cimg::min(width,xmin+height),
		ymin = cimg::max(0,(dimy()-dimx())/2), ymax = cimg::min(height,ymin+width),
		xoff = xmin + cimg::min(0,(dimx()-dimy())/2),
		yoff = ymin + cimg::min(0,(dimy()-dimx())/2);
	  cimg_mapZV(dest,z,v) for (unsigned int y=ymin; y<ymax; y++) for (unsigned int x=xmin; x<xmax; x++)
		dest(x,y,z,v) = (*this)(y-yoff,height-1-x+xoff,z,v);
	} break;
	case 2: {
	  cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = (*this)(width-1-x,height-1-y,z,v); 
	} break;
	case 3: {
	  dest.fill(0);
	  const unsigned int
		xmin = cimg::max(0,(dimx()-dimy())/2), xmax = cimg::min(width,xmin+height),
		ymin = cimg::max(0,(dimy()-dimx())/2), ymax = cimg::min(height,ymin+width),
		xoff = xmin + cimg::min(0,(dimx()-dimy())/2),
		yoff = ymin + cimg::min(0,(dimy()-dimx())/2);
	  cimg_mapZV(dest,z,v) for (unsigned int y=ymin; y<ymax; y++) for (unsigned int x=xmin; x<xmax; x++)
		dest(x,y,z,v) = (*this)(width-1-y+yoff,x-xoff,z,v);
	} break;
	default: 
	  return *this;        
	}
	  } else 
	switch (cond) { // generic version
	case 0: { 
	  cimg_mapXY(dest,x,y)
		cimg_mapZV(*this,z,v) 
		dest(x,y,z,v) = pix2d((int)(cx + (x-cx)*ca + (y-cy)*sa),(int)(cy - (x-cx)*sa + (y-cy)*ca),z,v,0);
	} break;
	case 1: {
	  cimg_mapXY(dest,x,y)
		cimg_mapZV(*this,z,v) 
		dest(x,y,z,v) = (*this)(cimg::mod((int)(cx + (x-cx)*ca + (y-cy)*sa),width),
					cimg::mod((int)(cy - (x-cx)*sa + (y-cy)*ca),height),z,v);
	} break;
	case 2: {
	  cimg_mapXY(dest,x,y) {
		const float X = cx + (x-cx)*ca + (y-cy)*sa, Y = cy - (x-cx)*sa + (y-cy)*ca;
		cimg_mapZV(*this,z,v) dest(x,y,z,v) = (T)linear_pix2d(X,Y,z,v,0);
	  }
	} break; 
	default: {
	  cimg_mapXY(dest,x,y) {
		const float X = cx + (x-cx)*ca + (y-cy)*sa, Y = cy - (x-cx)*sa + (y-cy)*ca;
		cimg_mapZV(*this,z,v) dest(x,y,z,v) = (T)cubic_pix2d(X,Y,z,v,0);
	  }
	} break; 
	}
	  return dest;
	}
  
	//! Rotate the image around the point (\c cx,\c cy).
	/**
	   \param angle = rotation angle (in degrees).
	   \param cx = X-coordinate of the rotation center.
	   \param cy = Y-coordinate of the rotation center.
	   \param zoom = zoom.
	   \param cond = rotation type. can be :
	   - 0 = zero-value at borders
	   - 1 = repeat image at borders
	   - 2 = zero-value at borders and linear interpolation
	   \note Rotation does not change the image size. If you want to get an image with a new size, use get_rotate() instead.
	   \see get_rotate()
	**/
	CImg& rotate(const float angle,const float cx,const float cy,const float zoom=1,const unsigned int cond=3) {
	  return get_rotate(angle,cx,cy,zoom,cond).swap(*this);
	}
 
	//! Return a resized image.
	/**
	   \param pdx = Number of columns (new size along the X-axis).
	   \param pdy = Number of rows (new size along the Y-axis).
	   \param pdz = Number of slices (new size along the Z-axis).
	   \param pdv = Number of vector-channels (new size along the V-axis).
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
	**/
	CImg get_resize(const int pdx=-100,const int pdy=-100,const int pdz=-100,const int pdv=-100,const unsigned int interp=1) const {
	  if (!pdx || !pdy || !pdz || !pdv) return CImg<T>();
	  const unsigned int 
	dx = pdx<0?-pdx*width/100:pdx,
	dy = pdy<0?-pdy*height/100:pdy,
	dz = pdz<0?-pdz*depth/100:pdz, 
	dv = pdv<0?-pdv*dim/100:pdv;
	  CImg res(dx?dx:1,dy?dy:1,dz?dz:1,dv?dv:1);
	  if (is_empty()) return res.fill(0);
	  if (width==res.width && height==res.height && depth==res.depth && dim==res.dim) return *this;
	  switch (interp) {
	  case 0:  // Zero filling
	res.fill(0).draw_image(*this,0,0,0,0);
	break;
	  case 1: { // Nearest-neighbor interpolation
	unsigned int 
	  *const offx = new unsigned int[res.width],
	  *const offy = new unsigned int[res.height+1],
	  *const offz = new unsigned int[res.depth+1],
	  *const offv = new unsigned int[res.dim+1],
	  *poffx, *poffy, *poffz, *poffv;
	const unsigned int 
	  wh = width*height,
	  whd = width*height*depth,
	  rwh = res.width*res.height,
	  rwhd = res.width*res.height*res.depth;
	float s, curr, old;
	s = (float)width/res.width;
	poffx = offx; curr=0; { cimg_mapX(res,x) { old=curr; curr+=s; *(poffx++) = (unsigned int)curr-(unsigned int)old; }}
	s = (float)height/res.height;
	poffy = offy; curr=0; { cimg_mapY(res,y) { old=curr; curr+=s; *(poffy++) = width*((unsigned int)curr-(unsigned int)old); }} *poffy=0;
	s = (float)depth/res.depth;
	poffz = offz; curr=0; { cimg_mapZ(res,z) { old=curr; curr+=s; *(poffz++) = wh*((unsigned int)curr-(unsigned int)old); }} *poffz=0;
	s = (float)dim/res.dim;
	poffv = offv; curr=0; { cimg_mapV(res,v) { old=curr; curr+=s; *(poffv++) = whd*((unsigned int)curr-(unsigned int)old); }} *poffv=0;
	   
	T *ptrd = res.ptr();	
	const T* ptrv = ptr();
	poffv = offv;
	for (unsigned int k=0; k<res.dim; ) {
	  const T *ptrz = ptrv;
	  poffz = offz;
	  for (unsigned int z=0; z<res.depth; ) {
		const T *ptry = ptrz;
		poffy = offy;
		for (unsigned int y=0; y<res.height; ) {
		  const T *ptrx = ptry;
		  poffx = offx;
		  cimg_mapX(res,x) { *(ptrd++)=*ptrx; ptrx+=*(poffx++); }
		  y++;
		  unsigned int dy=*(poffy++);
		  for (;!dy && y<res.height; std::memcpy(ptrd, ptrd-res.width, sizeof(T)*res.width), y++, ptrd+=res.width, dy=*(poffy++));
		  ptry+=dy;
		}
		z++;
		unsigned int dz=*(poffz++);
		for (;!dz && z<res.depth; std::memcpy(ptrd, ptrd-rwh, sizeof(T)*rwh), z++, ptrd+=rwh, dz=*(poffz++));
		ptrz+=dz;
	  }
	  k++;
	  unsigned int dv=*(poffv++);
	  for (;!dv && k<res.dim; std::memcpy(ptrd, ptrd-rwhd, sizeof(T)*rwhd), k++, ptrd+=rwhd, dv=*(poffv++));
	  ptrv+=dv;
	}
	delete[] offx; delete[] offy; delete[] offz;
	  }	break;
	  case 2: { // Mosaic filling
	for (unsigned int k=0; k<res.dim; k+=dim)
	  for (unsigned int z=0; z<res.depth; z+=depth)
		for (unsigned int y=0; y<res.height; y+=height)
		  for (unsigned int x=0; x<res.width; x+=width) res.draw_image(*this,x,y,z,k);
	  } break;
	  case 3: { // Linear interpolation
	const bool bx = (res.width<width), by = (res.height<height), bz = (res.depth<depth), bk = (res.dim<dim);
	const float
	  sx = bx?(width-1.0f)/(res.width+1):(res.width>1?(width-1.0f)/(res.width-1):0),
	  sy = by?(height-1.0f)/(res.height+1):(res.height>1?(height-1.0f)/(res.height-1):0),
	  sz = bz?(depth-1.0f)/(res.depth+1):(res.depth>1?(depth-1.0f)/(res.depth-1):0),
	  sk = bk?(dim-1.0f)/(res.dim+1):(res.dim>1?(dim-1.0f)/(res.dim-1):0),
	  dx = bx?sx:0, dy = by?sy:0, dz = bz?sz:0, dk = bk?sk:0;       
	float cx,cy,cz,ck=dk;
	cimg_mapV(res,k) { cz = dz;
	cimg_mapZ(res,z) { cy = dy;
	cimg_mapY(res,y) { cx = dx;
	cimg_mapX(res,x) { res(x,y,z,k) = (T)linear_pix4d(cx,cy,cz,ck); cx+=sx;
	} cy+=sy;
	} cz+=sz;
	} ck+=sk;
	}
	  } break;
	  case 4: { // Grid filling
	res.fill(0);
	cimg_mapXYZV(*this,x,y,z,k) res(x*res.width/width,
					y*res.height/height,
					z*res.depth/depth,
					k*res.dim/dim) = (*this)(x,y,z,k);
	  } break;
	  case 5: { // Cubic interpolation
	const bool bx = (res.width<width), by = (res.height<height), bz = (res.depth<depth), bk = (res.dim<dim);
	const float
	  sx = bx?(width-1.0f)/(res.width+1):(res.width>1?(width-1.0f)/(res.width-1):0),
	  sy = by?(height-1.0f)/(res.height+1):(res.height>1?(height-1.0f)/(res.height-1):0),
	  sz = bz?(depth-1.0f)/(res.depth+1):(res.depth>1?(depth-1.0f)/(res.depth-1):0),
	  sk = bk?(dim-1.0f)/(res.dim+1):(res.dim>1?(dim-1.0f)/(res.dim-1):0),
	  dx = bx?sx:0, dy = by?sy:0, dz = bz?sz:0, dk = bk?sk:0;       
	float cx,cy,cz,ck=dk;
	cimg_mapV(res,k) { cz = dz;
	cimg_mapZ(res,z) { cy = dy;
	cimg_mapY(res,y) { cx = dx;
	cimg_mapX(res,x) { res(x,y,z,k) = (T)cubic_pix2d(cx,cy,(int)cz,(int)ck); cx+=sx;
	} cy+=sy;
	} cz+=sz;
	} ck+=sk;
	}
	  } break;      
	  }
	  return res;
	}

	//! Return a resized image.
	/**
	   \param src = Image giving the geometry of the resize.
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
	**/
	template<typename t> CImg get_resize(const CImg<t>& src,const unsigned int interp=1) const {
	  return get_resize(src.width,src.height,src.depth,src.dim,interp); 
	}  

	//! Return a resized image.
	/**
	   \param disp = Display giving the geometry of the resize.
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
	**/
	CImg get_resize(const CImgDisplay& disp,const unsigned int interp=1) const {
	  return get_resize(disp.width,disp.height,depth,dim,interp);
	}

	//! Resize the image.
	/**
	   \param pdx = Number of columns (new size along the X-axis).
	   \param pdy = Number of rows (new size along the Y-axis).
	   \param pdz = Number of slices (new size along the Z-axis).
	   \param pdv = Number of vector-channels (new size along the V-axis).
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).       
	**/
	CImg& resize(const int pdx=-100,const int pdy=-100,const int pdz=-100,const int pdv=-100,const unsigned int interp=1) {
	  if (!pdx || !pdy || !pdz || !pdv) return empty();
	  const unsigned int 
	dx = pdx<0?-pdx*width/100:pdx,
	dy = pdy<0?-pdy*height/100:pdy,
	dz = pdz<0?-pdz*depth/100:pdz, 
	dv = pdv<0?-pdv*dim/100:pdv;
	  if (width==dx && height==dy && depth==dz && dim==dv) return *this;
	  return get_resize(dx,dy,dz,dv,interp).swap(*this);
	}

	//! Resize the image.
	/**
	   \param src = Image giving the geometry of the resize.
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
	**/
	template<typename t> CImg& resize(const CImg<t>& src,const unsigned int interp=1) { 
	  return resize(src.width,src.height,src.depth,src.dim,interp); 
	}

	//! Resize the image
	/**
	   \param disp = Display giving the geometry of the resize.
	   \param interp = Resizing type :
	   - 0 = no interpolation : additionnal space is filled with 0.
	   - 1 = bloc interpolation (nearest point).
	   - 2 = mosaic : image is repeated if necessary.
	   - 3 = linear interpolation.
	   - 4 = grid interpolation.
	   - 5 = bi-cubic interpolation.
	   \note If pd[x,y,z,v]<0, it corresponds to a percentage of the original size (the default value is -100).
	**/
	CImg& resize(const CImgDisplay& disp,const unsigned int interp=1) {
	  return resize(disp.width,disp.height,depth,dim,interp);
	}

	//! Return an half-resized image, using a special filter.
	/**
	   \see resize_halfXY(), resize(), get_resize().
	**/
	CImg get_resize_halfXY() const {
	  typedef typename cimg::largest<T,float>::type ftype;
	  if (is_empty()) return CImg<T>();
	  CImg<ftype> mask = CImg<ftype>::matrix(0.07842776544f, 0.1231940459f, 0.07842776544f,
						 0.1231940459f,  0.1935127547f, 0.1231940459f,
						 0.07842776544f, 0.1231940459f, 0.07842776544f);
	  CImg_3x3x1(I,ftype);
	  CImg dest(width/2,height/2,depth,dim);
	  cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) 
	if (x%2 && y%2) dest(x/2,y/2,z,k) = (T)cimg_conv3x3x1(I,mask);
	  return dest;
	}

	//! Half-resize the image, using a special filter
	/**
	   \see get_resize_halfXY(), resize(), get_resize().
	**/
	CImg& resize_halfXY() {
	  return get_resize_halfXY().swap(*this); 
	}

	//! Return a square region of the image, as a new image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param z0 = Z-coordinate of the upper-left crop rectangle corner.
	   \param v0 = V-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param z1 = Z-coordinate of the lower-right crop rectangle corner.
	   \param v1 = V-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = Dirichlet (false) or Neumann border conditions.
	   \see crop()
	**/
	CImg get_crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,const unsigned int v0,
		  const unsigned int x1,const unsigned int y1,const unsigned int z1,const unsigned int v1,
		  const bool border_condition = false) const {
	  if (is_empty()) 
		throw CImgInstanceException("CImg<%s>::get_crop() : Instance image (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  const unsigned int dx=x1-x0+1, dy=y1-y0+1, dz=z1-z0+1, dv=v1-v0+1;
	  CImg dest(dx,dy,dz,dv);
	  if (x0>=width || x1>=width || y0>=height || y1>=height || z0>=depth || z1>=depth ||
	  v0>=dim || v1>=dim || x1<x0 || y1<y0 || z1<z0 || v1<v0)
	switch (border_condition) {
	case false: { cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = pix4d(x0+x,y0+y,z0+z,v0+v,0); } break;
	default: { cimg_mapXYZV(dest,x,y,z,v) dest(x,y,z,v) = pix4d(x0+x,y0+y,z0+z,v0+v); } break;
	} else {
	  const T *psrc = ptr(x0,y0,z0,v0);
	  T *pdest = dest.ptr(0,0,0,0);
	  if (dx!=width)
		for (unsigned int k=0; k<dv; k++) {
		  for (unsigned int z=0; z<dz; z++) {
		for (unsigned int y=0; y<dy; y++) {
		  std::memcpy(pdest,psrc,dx*sizeof(T));
		  pdest+=dx;
		  psrc+=width;
		}
		psrc+=width*(height-dy);
		  }
		  psrc+=width*height*(depth-dz);
		}
	  else {
		if (dy!=height)         
		  for (unsigned int k=0; k<dv; k++) {
		for (unsigned int z=0; z<dz; z++) {
		  std::memcpy(pdest,psrc,dx*dy*sizeof(T));
		  pdest+=dx*dy;
		  psrc+=width*height;
		}
		psrc+=width*height*(depth-dz);
		  }
		else {
		  if (dz!=depth)
		for (unsigned int k=0; k<dv; k++) {
		  std::memcpy(pdest,psrc,dx*dy*dz*sizeof(T));
		  pdest+=dx*dy*dz;
		  psrc+=width*height*depth;
		}
		  else std::memcpy(pdest,psrc,dx*dy*dz*dv*sizeof(T));
		}
	  }
	}
	  return dest;
	}
	
	//! Return a square region of the image, as a new image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param z0 = Z-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param z1 = Z-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see crop()   
	**/
	CImg get_crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,
		  const unsigned int x1,const unsigned int y1,const unsigned int z1,
		  const bool border_condition=false) const {
	  return get_crop(x0,y0,z0,0,x1,y1,z1,dim-1,border_condition);
	}

	//! Return a square region of the image, as a new image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see crop()   
	**/
	CImg get_crop(const unsigned int x0,const unsigned int y0,
		  const unsigned int x1,const unsigned int y1,
		  const bool border_condition=false) const {
	  return get_crop(x0,y0,0,0,x1,y1,depth-1,dim-1,border_condition);
	}

	//! Return a square region of the image, as a new image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see crop()   
	**/
	CImg get_crop(const unsigned int x0,const unsigned int x1,const bool border_condition=false) const {
	  return get_crop(x0,0,0,0,x1,height-1,depth-1,dim-1,border_condition); 
	}

	//! Replace the image by a square region of the image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param z0 = Z-coordinate of the upper-left crop rectangle corner.
	   \param v0 = V-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param z1 = Z-coordinate of the lower-right crop rectangle corner.
	   \param v1 = V-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see get_crop()
	**/
	CImg& crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,const unsigned int v0,
		   const unsigned int x1,const unsigned int y1,const unsigned int z1,const unsigned int v1,
		   const bool border_condition=false) {
	  return get_crop(x0,y0,z0,v0,x1,y1,z1,v1,border_condition).swap(*this);
	}

	//! Replace the image by a square region of the image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param z0 = Z-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param z1 = Z-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see get_crop()
	**/
	CImg& crop(const unsigned int x0,const unsigned int y0,const unsigned int z0,
		   const unsigned int x1,const unsigned int y1,const unsigned int z1,
		   const bool border_condition=false) {
	  return crop(x0,y0,z0,0,x1,y1,z1,dim-1,border_condition);
	}

	//! Replace the image by a square region of the image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param y0 = Y-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param y1 = Y-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see get_crop()
	**/
	CImg& crop(const unsigned int x0,const unsigned int y0,
		   const unsigned int x1,const unsigned int y1,
		   const bool border_condition=false) { 
	  return crop(x0,y0,0,0,x1,y1,depth-1,dim-1,border_condition); 
	}

	//! Replace the image by a square region of the image
	/**
	   \param x0 = X-coordinate of the upper-left crop rectangle corner.
	   \param x1 = X-coordinate of the lower-right crop rectangle corner.
	   \param border_condition = determine the type of border condition if
	   some of the desired region is outside the image.
	   \see get_crop()
	**/
	CImg& crop(const unsigned int x0,const unsigned int x1,const bool border_condition=false) { 
	  return crop(x0,0,0,0,x1,height-1,depth-1,dim-1,border_condition);
	}

	//! Get a shared-memory image referencing a set of points of the instance image.
	CImg get_shared_points(const unsigned int x0, const unsigned int x1,
			   const unsigned int y0=0, const unsigned int z0=0, const unsigned int v0=0) {
	  const unsigned long beg = offset(x0,y0,z0,v0), end = offset(x1,y0,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_points() : Cannot return a shared-memory subset (%u->%u,%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),x0,x1,y0,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,x1-x0+1,1,1,1,true);
	}
	
	//! Get a shared-memory image referencing a set of points of the instance image (const version).
	const CImg get_shared_points(const unsigned int x0, const unsigned int x1,
				 const unsigned int y0=0, const unsigned int z0=0, const unsigned int v0=0) const {
	  const unsigned long beg = offset(x0,y0,z0,v0), end = offset(x1,y0,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_points() : Cannot return a shared-memory subset (%u->%u,%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),x0,x1,y0,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,x1-x0+1,1,1,1,true);
	}

	//! Get a copy of a set of points of the instance image.
	CImg get_points(const unsigned int x0, const unsigned int x1,
			const unsigned int y0=0, const unsigned int z0=0, const unsigned int v0=0) const {      
	  const unsigned long beg = offset(x0,y0,z0,v0), end = offset(x1,y0,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_points() : Cannot return a shared-memory subset (%u->%u,%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),x0,x1,y0,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,x1-x0+1,1,1,1);
	}

	//! Return a shared-memory image referencing a set of lines of the instance image.
	CImg get_shared_lines(const unsigned int y0, const unsigned int y1,
			  const unsigned int z0=0, const unsigned int v0=0) {
	  const unsigned long beg = offset(0,y0,z0,v0), end = offset(0,y1,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_lines() : Cannot return a shared-memory subset (0->%u,%u->%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,y0,y1,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,y1-y0+1,1,1,true);
	}
	
	//! Return a shared-memory image referencing a set of lines of the instance image (const version).
	const CImg get_shared_lines(const unsigned int y0, const unsigned int y1,
				const unsigned int z0=0, const unsigned int v0=0) const {
	  const unsigned long beg = offset(0,y0,z0,v0), end = offset(0,y1,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_lines() : Cannot return a shared-memory subset (0->%u,%u->%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,y0,y1,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,y1-y0+1,1,1,true);
	}
	
	//! Get a copy of a set of lines of the instance image.
	CImg get_lines(const unsigned int y0, const unsigned int y1,
		   const unsigned int z0=0, const unsigned int v0=0) const {
	  const unsigned long beg = offset(0,y0,z0,v0), end = offset(0,y1,z0,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_lines() : Cannot return a shared-memory subset (0->%u,%u->%u,%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,y0,y1,z0,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,y1-y0+1,1,1);
	}

	//! Replace the instance image by a set of lines of the instance image.
	CImg& lines(const unsigned int y0, const unsigned int y1,
		const unsigned int z0=0, const unsigned int v0=0) const {
	  return get_lines(y0,y1,z0,v0).swap(*this);
	}
	
	//! Return a shared-memory image referencing one particular line (y0,z0,v0) of the instance image.
	CImg get_shared_line(const unsigned int y0, const unsigned int z0=0, const unsigned int v0=0) { 
	  return get_shared_lines(y0,y0,z0,v0);
	}

	//! Return a shared-memory image referencing one particular line (y0,z0,v0) of the instance image (const version).
	const CImg get_shared_line(const unsigned int y0,const unsigned int z0=0,const unsigned int v0=0) const { 
	  return get_shared_lines(y0,y0,z0,v0);
	}
	
	//! Get a copy of a line of the instance image.
	CImg get_line(const unsigned int y0,
		  const unsigned int z0=0, const unsigned int v0=0) const {
	  return get_lines(y0,y0,z0,v0);
	}
	
	//! Replace the instance image by one of its line.
	CImg& line(const unsigned int y0, const unsigned int z0=0, const unsigned int v0=0) {
	  return get_line(y0,z0,v0).swap(*this);
	}
	
	//! Return a shared memory image referencing a set of planes (z0->z1,v0) of the instance image.
	CImg get_shared_planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) {
	  const unsigned long beg = offset(0,0,z0,v0), end = offset(0,0,z1,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_planes() : Cannot return a shared-memory subset (0->%u,0->%u,%u->%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,z0,z1,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,z1-z0+1,1,true);
	}

	//! Return a shared-memory image referencing a set of planes (z0->z1,v0) of the instance image (const version).
	const CImg get_shared_planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) const {
	  const unsigned long beg = offset(0,0,z0,v0), end = offset(0,0,z1,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_planes() : Cannot return a shared-memory subset (0->%u,0->%u,%u->%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,z0,z1,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,z1-z0+1,1,true);
	}
	
	//! Return a copy of a set of planes of the instance image.
	CImg get_planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) const {
	  const unsigned long beg = offset(0,0,z0,v0), end = offset(0,0,z1,v0);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_planes() : Cannot return a shared-memory subset (0->%u,0->%u,%u->%u,%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,z0,z1,v0,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,z1-z0+1,1);
	}

	//! Replace the instance image by a set of planes of the instance image.
	CImg& planes(const unsigned int z0, const unsigned int z1, const unsigned int v0=0) {
	  return get_planes(z0,z1,v0).swap(*this);
	}
	
	//! Return a shared-memory image referencing one plane (z0,v0) of the instance image.
	CImg get_shared_plane(const unsigned int z0, const unsigned int v0=0) { 
	  return get_shared_planes(z0,z0,v0);
	}

	//! Return a shared-memory image referencing one plane (z0,v0) of the instance image (const version).
	const CImg get_shared_plane(const unsigned int z0, const unsigned int v0=0) const { 
	  return get_shared_planes(z0,z0,v0);
	}
	
	//! Return a copy of a plane of the instance image.
	CImg get_plane(const unsigned int z0, const unsigned int v0=0) const {
	  return get_planes(z0,z0,v0);
	}
	
	//! Replace the instance image by one plane of the instance image.
	CImg& plane(const unsigned int z0, const unsigned int v0=0) {
	  return get_plane(z0,v0).swap(*this);
	}

	//! Return a shared-memory image referencing a set of channels (v0->v1) of the instance image.
	CImg get_shared_channels(const unsigned int v0, const unsigned int v1) { 
	  const unsigned long beg = offset(0,0,0,v0), end = offset(0,0,0,v1);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_channels() : Cannot return a shared-memory subset (0->%u,0->%u,0->%u,%u->%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,depth-1,v0,v1,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,depth,v1-v0+1,true);
	}
	
	//! Return a shared-memory image referencing a set of channels (v0->v1) of the instance image (const version).
	const CImg get_shared_channels(const unsigned int v0, const unsigned int v1) const { 
	  const unsigned long beg = offset(0,0,0,v0), end = offset(0,0,0,v1);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_shared_channels() : Cannot return a shared-memory subset (0->%u,0->%u,0->%u,%u->%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,depth-1,v0,v1,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,depth,v1-v0+1,true);
	}
	
	//! Return a copy of a set of channels of the instance image.
	CImg get_channels(const unsigned int v0, const unsigned int v1) const {
	  const unsigned long beg = offset(0,0,0,v0), end = offset(0,0,0,v1);
	  if (beg>end || beg>=size() || end>=size())
	throw CImgArgumentException("CImg<%s>::get_channels() : Cannot return a shared-memory subset (0->%u,0->%u,0->%u,%u->%u) from "
					"a (%u,%u,%u,%u) image.",pixel_type(),width-1,height-1,depth-1,v0,v1,width,height,depth,dim);
	  return CImg<T>(data+beg,width,height,depth,v1-v0+1);
	}
	
	//! Replace the instance image by a set of channels of the instance image.
	CImg& channels(const unsigned int v0, const unsigned int v1) {
	  return get_channels(v0,v1).swap(*this);
	}

	//! Return a shared-memory image referencing one channel v0 of the instance image.
	CImg get_shared_channel(const unsigned int v0) { 
	  return get_shared_channels(v0,v0);
	}

	//! Return a shared-memory image referencing one channel v0 of the instance image (const version).
	const CImg get_shared_channel(const unsigned int v0) const { 
	  return get_shared_channels(v0,v0);
	}

	//! Return a copy of a channel of the instance image.
	CImg get_channel(const unsigned int v0) const {
	  return get_channels(v0,v0);
	}

	//! Replace the instance image by one of its channel.
	CImg& channel(const unsigned int v0) {
	  return get_channel(v0).swap(*this);
	}

	//! Return a shared version of the instance image.
	CImg get_shared() {
	  return CImg<T>(data,width,height,depth,dim,true);
	}

	//! Return a shared version of the instance image (const version).
	const CImg get_shared() const {
	  return CImg<T>(data,width,height,depth,dim,true);
	}

	//! Get the z-slice \a z of *this, as a new image.
	/**
	   \param z0 = Z-slice to return.
	   \see slice(), get_channel(), channel(), get_plane(), plane().
	**/
	CImg get_slice(const unsigned int z0=0) const {
	  return get_crop(0,0,z0,0,width-1,height-1,z0,dim-1); 
	}

	//! Replace the image by one of its slice.
	CImg& slice(const unsigned int z0) { 
	  return get_slice(z0).swap(*this); 
	}

	//! Mirror an image along the specified axis.
	/**
	   This is the in-place version of get_mirror().
	   \sa get_mirror().
	**/
	CImg& mirror(const char axe='x') {
	  if (!is_empty()) {
	T *pf,*pb,*buf=0;
	switch (cimg::uncase(axe)) {
	case 'x': {
	  pf = ptr(); pb = ptr(width-1);
	  for (unsigned int yzv=0; yzv<height*depth*dim; yzv++) { 
		for (unsigned int x=0; x<width/2; x++) { const T val = *pf; *(pf++)=*pb; *(pb--)=val; }
		pf+=width-width/2;
		pb+=width+width/2;
	  }
	} break;
	case 'y': {
	  buf = new T[width];
	  pf = ptr(); pb = ptr(0,height-1);
	  for (unsigned int zv=0; zv<depth*dim; zv++) {
		for (unsigned int y=0; y<height/2; y++) {
		  std::memcpy(buf,pf,width*sizeof(T));
		  std::memcpy(pf,pb,width*sizeof(T));
		  std::memcpy(pb,buf,width*sizeof(T));
		  pf+=width;
		  pb-=width;
		}
		pf+=width*(height-height/2);
		pb+=width*(height+height/2);
	  }
	} break;
	case 'z': {
	  buf = new T[width*height];
	  pf = ptr(); pb = ptr(0,0,depth-1);
	  cimg_mapV(*this,v) {
		for (unsigned int z=0; z<depth/2; z++) {
		  std::memcpy(buf,pf,width*height*sizeof(T));
		  std::memcpy(pf,pb,width*height*sizeof(T));
		  std::memcpy(pb,buf,width*height*sizeof(T));
		  pf+=width*height;
		  pb-=width*height;
		}
		pf+=width*height*(depth-depth/2);
		pb+=width*height*(depth+depth/2);
	  }
	} break;
	case 'v': {
	  buf = new T[width*height*depth];
	  pf = ptr(); pb = ptr(0,0,0,dim-1);
	  for (unsigned int v=0; v<dim/2; v++) {
		std::memcpy(buf,pf,width*height*depth*sizeof(T));
		std::memcpy(pf,pb,width*height*depth*sizeof(T));
		std::memcpy(pb,buf,width*height*depth*sizeof(T));
		pf+=width*height*depth;
		pb-=width*height*depth;
	  }
	} break;
	default:
	  throw CImgArgumentException("CImg<%s>::mirror() : unknow axe '%c', must be 'x','y','z' or 'v'",pixel_type(),axe);
	}
	if (buf) delete[] buf;
	  }    
	  return *this;
	}
  
	//! Get a mirrored version of the image, along the specified axis.
	/**
	   \param axe Axe used to mirror the image. Can be \c 'x', \c 'y', \c 'z' or \c 'v'.
	   \sa mirror().
	**/
	CImg get_mirror(const char axe='x') {
	  return CImg<T>(*this,true).mirror(axe); 
	}
	
	//! Scroll the image
	/**
	   This is the in-place version of get_scroll().
	   \sa get_scroll().
	**/
	CImg& scroll(const int scrollx,const int scrolly=0,const int scrollz=0,const int scrollv=0,const int border_condition=0) {
	  if (!is_empty()) {

	if (scrollx) // Scroll along X-axis
	  switch (border_condition) {
	  case 0:
		if (cimg::abs(scrollx)>=(int)width) return fill(0);
		if (scrollx>0) cimg_mapYZV(*this,y,z,k) {
		  std::memmove(ptr(0,y,z,k),ptr(scrollx,y,z,k),(width-scrollx)*sizeof(T));
		  std::memset(ptr(width-scrollx,y,z,k),0,scrollx*sizeof(T));
		} else cimg_mapYZV(*this,y,z,k) {
		  std::memmove(ptr(-scrollx,y,z,k),ptr(0,y,z,k),(width+scrollx)*sizeof(T));
		  std::memset(ptr(0,y,z,k),0,-scrollx*sizeof(T));
		}
		break;   
	  case 1:
		if (scrollx>0) {
		  const int nscrollx = (scrollx>=(int)width)?width-1:scrollx;
		  if (!nscrollx) return *this;
		  cimg_mapYZV(*this,y,z,k) {
		std::memmove(ptr(0,y,z,k),ptr(nscrollx,y,z,k),(width-nscrollx)*sizeof(T));
		T *ptrd = ptr(width-1,y,z,k);
		const T &val = *ptrd;
		for (int l=0; l<nscrollx-1; l++) *(--ptrd) = val;
		  }
		} else {
		  const int nscrollx = (-scrollx>=(int)width)?width-1:-scrollx;
		  if (!nscrollx) return *this;
		  cimg_mapYZV(*this,y,z,k) {
		std::memmove(ptr(nscrollx,y,z,k),ptr(0,y,z,k),(width-nscrollx)*sizeof(T));
		T *ptrd = ptr(0,y,z,k);
		const T &val = *ptrd;
		for (int l=0; l<nscrollx-1; l++) *(++ptrd) = val;
		  }
		}    
		break; 
	  case 2: {
		const int ml = cimg::mod(scrollx,width), nscrollx = (ml<=(int)width/2)?ml:(ml-(int)width);
		if (!nscrollx) return *this;
		T* buf = new T[cimg::abs(nscrollx)];
		if (nscrollx>0) cimg_mapYZV(*this,y,z,k) {
		  std::memcpy(buf,ptr(0,y,z,k),nscrollx*sizeof(T));
		  std::memmove(ptr(0,y,z,k),ptr(nscrollx,y,z,k),(width-nscrollx)*sizeof(T));
		  std::memcpy(ptr(width-nscrollx,y,z,k),buf,nscrollx*sizeof(T));
		} else cimg_mapYZV(*this,y,z,k) {
		  std::memcpy(buf,ptr(width+nscrollx,y,z,k),-nscrollx*sizeof(T));
		  std::memmove(ptr(-nscrollx,y,z,k),ptr(0,y,z,k),(width+nscrollx)*sizeof(T));
		  std::memcpy(ptr(0,y,z,k),buf,-nscrollx*sizeof(T));
		}
		delete[] buf;
	  } break;
	  }

	if (scrolly) // Scroll along Y-axis
	  switch (border_condition) {
	  case 0:
		if (cimg::abs(scrolly)>=(int)height) return fill(0);
		if (scrolly>0) cimg_mapZV(*this,z,k) {
		  std::memmove(ptr(0,0,z,k),ptr(0,scrolly,z,k),width*(height-scrolly)*sizeof(T));
		  std::memset(ptr(0,height-scrolly,z,k),0,width*scrolly*sizeof(T));
		} else cimg_mapZV(*this,z,k) {
		  std::memmove(ptr(0,-scrolly,z,k),ptr(0,0,z,k),width*(height+scrolly)*sizeof(T));
		  std::memset(ptr(0,0,z,k),0,-scrolly*width*sizeof(T));
		}
		break;      
	  case 1:
		if (scrolly>0) {
		  const int nscrolly = (scrolly>=(int)height)?height-1:scrolly;
		  if (!nscrolly) return *this;
		  cimg_mapZV(*this,z,k) {
		std::memmove(ptr(0,0,z,k),ptr(0,nscrolly,z,k),width*(height-nscrolly)*sizeof(T));
		T *ptrd = ptr(0,height-nscrolly,z,k), *ptrs = ptr(0,height-1,z,k);
		for (int l=0; l<nscrolly-1; l++) { std::memcpy(ptrd,ptrs,width*sizeof(T)); ptrd+=width; }
		  }
		} else {
		  const int nscrolly = (-scrolly>=(int)height)?height-1:-scrolly;
		  if (!nscrolly) return *this;
		  cimg_mapZV(*this,z,k) {
		std::memmove(ptr(0,nscrolly,z,k),ptr(0,0,z,k),width*(height-nscrolly)*sizeof(T));
		T *ptrd = ptr(0,1,z,k), *ptrs = ptr(0,0,z,k);
		for (int l=0; l<nscrolly-1; l++) { std::memcpy(ptrd,ptrs,width*sizeof(T)); ptrd+=width; }
		  }
		}    
		break;  
	  case 2: {
		const int ml = cimg::mod(scrolly,height), nscrolly = (ml<=(int)height/2)?ml:(ml-(int)height);
		if (!nscrolly) return *this;
		T* buf = new T[width*cimg::abs(nscrolly)];
		if (nscrolly>0) cimg_mapZV(*this,z,k) {
		  std::memcpy(buf,ptr(0,0,z,k),width*nscrolly*sizeof(T));
		  std::memmove(ptr(0,0,z,k),ptr(0,nscrolly,z,k),width*(height-nscrolly)*sizeof(T));
		  std::memcpy(ptr(0,height-nscrolly,z,k),buf,width*nscrolly*sizeof(T));
		} else cimg_mapZV(*this,z,k) {
		  std::memcpy(buf,ptr(0,height+nscrolly,z,k),-nscrolly*width*sizeof(T));
		  std::memmove(ptr(0,-nscrolly,z,k),ptr(0,0,z,k),width*(height+nscrolly)*sizeof(T));
		  std::memcpy(ptr(0,0,z,k),buf,-nscrolly*width*sizeof(T));
		}
		delete[] buf;
	  } break;    
	  }
	
	if (scrollz) // Scroll along Z-axis
	  switch (border_condition) {
	  case 0:
		if (cimg::abs(scrollz)>=(int)depth) return fill(0);
		if (scrollz>0) cimg_mapV(*this,k) {
		  std::memmove(ptr(0,0,0,k),ptr(0,0,scrollz,k),width*height*(depth-scrollz)*sizeof(T));
		  std::memset(ptr(0,0,depth-scrollz,k),0,width*height*scrollz*sizeof(T));
		} else cimg_mapV(*this,k) {
		  std::memmove(ptr(0,0,-scrollz,k),ptr(0,0,0,k),width*height*(depth+scrollz)*sizeof(T));
		  std::memset(ptr(0,0,0,k),0,-scrollz*width*height*sizeof(T));
		}
		break;      
	  case 1:
		if (scrollz>0) {
		  const int nscrollz = (scrollz>=(int)depth)?depth-1:scrollz;
		  if (!nscrollz) return *this;
		  cimg_mapV(*this,k) {
		std::memmove(ptr(0,0,0,k),ptr(0,0,nscrollz,k),width*height*(depth-nscrollz)*sizeof(T));
		T *ptrd = ptr(0,0,depth-nscrollz,k), *ptrs = ptr(0,0,depth-1,k);
		for (int l=0; l<nscrollz-1; l++) { std::memcpy(ptrd,ptrs,width*height*sizeof(T)); ptrd+=width*height; }
		  }
		} else {
		  const int nscrollz = (-scrollz>=(int)depth)?depth-1:-scrollz;
		  if (!nscrollz) return *this;
		  cimg_mapV(*this,k) {
		std::memmove(ptr(0,0,nscrollz,k),ptr(0,0,0,k),width*height*(depth-nscrollz)*sizeof(T));
		T *ptrd = ptr(0,0,1,k), *ptrs = ptr(0,0,0,k);
		for (int l=0; l<nscrollz-1; l++) { std::memcpy(ptrd,ptrs,width*height*sizeof(T)); ptrd+=width*height; }
		  }
		}    
		break;      
	  case 2: {
		const int ml = cimg::mod(scrollz,depth), nscrollz = (ml<=(int)depth/2)?ml:(ml-(int)depth);
		if (!nscrollz) return *this;
		T* buf = new T[width*height*cimg::abs(nscrollz)];
		if (nscrollz>0) cimg_mapV(*this,k) {
		  std::memcpy(buf,ptr(0,0,0,k),width*height*nscrollz*sizeof(T));
		  std::memmove(ptr(0,0,0,k),ptr(0,0,nscrollz,k),width*height*(depth-nscrollz)*sizeof(T));
		  std::memcpy(ptr(0,0,depth-nscrollz,k),buf,width*height*nscrollz*sizeof(T));
		} else cimg_mapV(*this,k) {
		  std::memcpy(buf,ptr(0,0,depth+nscrollz,k),-nscrollz*width*height*sizeof(T));
		  std::memmove(ptr(0,0,-nscrollz,k),ptr(0,0,0,k),width*height*(depth+nscrollz)*sizeof(T));
		  std::memcpy(ptr(0,0,0,k),buf,-nscrollz*width*height*sizeof(T));
		}
		delete[] buf;
	  } break;    
	  }
	
	if (scrollv) // Scroll along V-axis
	  switch (border_condition) {
	  case 0:
		if (cimg::abs(scrollv)>=(int)dim) return fill(0);
		if (scrollv>0) {
		  std::memmove(data,ptr(0,0,0,scrollv),width*height*depth*(dim-scrollv)*sizeof(T));
		  std::memset(ptr(0,0,0,dim-scrollv),0,width*height*depth*scrollv*sizeof(T));
		} else cimg_mapV(*this,k) {
		  std::memmove(ptr(0,0,0,-scrollv),data,width*height*depth*(dim+scrollv)*sizeof(T));
		  std::memset(data,0,-scrollv*width*height*depth*sizeof(T));
		}
		break;      
	  case 1:
		if (scrollv>0) {
		  const int nscrollv = (scrollv>=(int)dim)?dim-1:scrollv;
		  if (!nscrollv) return *this;
		  std::memmove(data,ptr(0,0,0,nscrollv),width*height*depth*(dim-nscrollv)*sizeof(T));
		  T *ptrd = ptr(0,0,0,dim-nscrollv), *ptrs = ptr(0,0,0,dim-1);
		  for (int l=0; l<nscrollv-1; l++) { std::memcpy(ptrd,ptrs,width*height*depth*sizeof(T)); ptrd+=width*height*depth; }      
		} else {
		  const int nscrollv = (-scrollv>=(int)dim)?dim-1:-scrollv;
		  if (!nscrollv) return *this;
		  std::memmove(ptr(0,0,0,nscrollv),data,width*height*depth*(dim-nscrollv)*sizeof(T));
		  T *ptrd = ptr(0,0,0,1);
		  for (int l=0; l<nscrollv-1; l++) { std::memcpy(ptrd,data,width*height*depth*sizeof(T)); ptrd+=width*height*depth; }      
		}    
		break;      
	  case 2: {
		const int ml = cimg::mod(scrollv,dim), nscrollv = (ml<=(int)dim/2)?ml:(ml-(int)dim);
		if (!nscrollv) return *this;
		T* buf = new T[width*height*depth*cimg::abs(nscrollv)];
		if (nscrollv>0) {
		  std::memcpy(buf,data,width*height*depth*nscrollv*sizeof(T));
		  std::memmove(data,ptr(0,0,0,nscrollv),width*height*depth*(dim-nscrollv)*sizeof(T));
		  std::memcpy(ptr(0,0,0,dim-nscrollv),buf,width*height*depth*nscrollv*sizeof(T));
		} else {
		  std::memcpy(buf,ptr(0,0,0,dim+nscrollv),-nscrollv*width*height*depth*sizeof(T));
		  std::memmove(ptr(0,0,0,-nscrollv),data,width*height*depth*(dim+nscrollv)*sizeof(T));
		  std::memcpy(data,buf,-nscrollv*width*height*depth*sizeof(T));
		}
		delete[] buf;
	  } break;    
	  }
	  }
	  return *this;
	}

	//! Return a scrolled image.
	/**
	   \param scrollx Amount of displacement along the X-axis.
	   \param scrolly Amount of displacement along the Y-axis.
	   \param scrollz Amount of displacement along the Z-axis.
	   \param scrollv Amount of displacement along the V-axis.
	   \param border_condition Border condition.
	   
	   - \c border_condition can be :
		  - 0 : Zero border condition (Dirichlet).
	  - 1 : Nearest neighbors (Neumann).
	  - 2 : Repeat Pattern (Fourier style).
	**/
	CImg get_scroll(const int scrollx,const int scrolly=0,const int scrollz=0,const int scrollv=0,
			const int border_condition=0) const {
	  return CImg<T>(*this,true).scroll(scrollx,scrolly,scrollz,scrollv,border_condition);
	}
	
	//! Return a 2D representation of a 3D image, with three slices.
	CImg get_projections2d(const unsigned int px0,const unsigned int py0,const unsigned int pz0) const {
	  if (is_empty()) return CImg<T>();
	  const unsigned int
	x0=(px0>=width)?width-1:px0,
	y0=(py0>=height)?height-1:py0,
	z0=(pz0>=depth)?depth-1:pz0;
	  CImg res(width+depth,height+depth,1,dim);
	  res.fill((*this)[0]);
	  { cimg_mapXYV(*this,x,y,k) res(x,y,0,k)        = (*this)(x,y,z0,k); }
	  { cimg_mapYZV(*this,y,z,k) res(width+z,y,0,k)  = (*this)(x0,y,z,k); }
	  { cimg_mapXZV(*this,x,z,k) res(x,height+z,0,k) = (*this)(x,y0,z,k); }
	  return res;
	}

	//! Return the image histogram.
	/**
	   The histogram H of an image I is a 1D-function where H(x) is the number of
	   occurences of the value x in I.
	   \param nblevels = Number of different levels of the computed histogram.
	   For classical images, this value is 256 (default value). You should specify more levels
	   if you are working with CImg<float> or images with high range of pixel values.
	   \param val_min = Minimum value considered for the histogram computation. All pixel values lower than val_min
	   won't be counted.
	   \param val_max = Maximum value considered for the histogram computation. All pixel values higher than val_max
	   won't be counted.
	   \note If val_min==val_max==0 (default values), the function first estimates the minimum and maximum
	   pixel values of the current image, then uses these values for the histogram computation.
	   \result The histogram is returned as a 1D CImg<float> image H, having a size of (nblevels,1,1,1) such that
	   H(0) and H(nblevels-1) are respectively equal to the number of occurences of the values val_min and val_max in I.
	   \note Histogram computation always returns a 1D function. Histogram of multi-valued (such as color) images
	   are not multi-dimensional.
	   \see get_equalize_histogram(), equalize_histogram()
	**/
	CImg<float> get_histogram(const unsigned int nblevels=256,const T val_min=(T)0,const T val_max=(T)0) const {
	  if (is_empty()) return CImg<float>();
	  if (nblevels<1)
	throw CImgArgumentException("CImg<%s>::get_histogram() : Can't compute an histogram with %u levels",
					pixel_type(),nblevels);
	  T vmin=val_min, vmax=val_max;
	  CImg<float> res(nblevels,1,1,1,0);
	  if (vmin==vmax && vmin==0) { const CImgStats st(*this,false); vmin = (T)st.min; vmax = (T)st.max; }
	  cimg_map(*this,ptr,T) {
	const int pos = (int)((*ptr-vmin)*(nblevels-1)/(vmax-vmin));
	if (pos>=0 && pos<(int)nblevels) res[pos]++; 
	  }
	  return res;
	}
	
	//! Equalize the image histogram
	/** This is the in-place version of \ref get_equalize_histogram() **/
	CImg& equalize_histogram(const unsigned int nblevels=256,const T val_min=(T)0,const T val_max=(T)0) {
	  if (!is_empty()) {
	T vmin=val_min, vmax=val_max;
	if (vmin==vmax && vmin==0) { const CImgStats st(*this,false); vmin = (T)st.min; vmax = (T)st.max; }
	CImg<float> hist = get_histogram(nblevels,vmin,vmax);
	float cumul=0;
	cimg_mapX(hist,pos) { cumul+=hist[pos]; hist[pos]=cumul; }
	cimg_map(*this,ptr,T) {
	  const int pos = (unsigned int)((*ptr-vmin)*(nblevels-1)/(vmax-vmin));
	  if (pos>=0 && pos<(int)nblevels) *ptr = (T)(vmin + (vmax-vmin)*hist[pos]/size());
	}
	  }
	  return *this;
	}

	//! Return the histogram-equalized version of the current image.
	/**
	   The histogram equalization is a classical image processing algorithm that enhances the image contrast
	   by expanding its histogram.
	   \param nblevels = Number of different levels of the computed histogram.
	   For classical images, this value is 256 (default value). You should specify more levels
	   if you are working with CImg<float> or images with high range of pixel values.
	   \param val_min = Minimum value considered for the histogram computation. All pixel values lower than val_min
	   won't be changed.
	   \param val_max = Maximum value considered for the histogram computation. All pixel values higher than val_max
	   won't be changed.
	   \note If val_min==val_max==0 (default values), the function acts on all pixel values of the image.
	   \return A new image with same size is returned, where pixels have been equalized.
	   \see get_histogram(), equalize_histogram()
	**/
	CImg get_equalize_histogram(const unsigned int nblevels=256,const T val_min=(T)0,const T val_max=(T)0) const { 
	  return CImg<T>(*this,true).equalize_histogram(nblevels,val_min,val_max); 
	}

	//! Return the scalar image of vector norms.
	/**
	   When dealing with vector-valued images (i.e images with dimv()>1), this function computes the L1,L2 or Linf norm of each
	   vector-valued pixel.
	   \param norm_type = Type of the norm being computed (1 = L1, 2 = L2, -1 = Linf).
	   \return A scalar-valued image CImg<float> with size (dimx(),dimy(),dimz(),1), where each pixel is the norm
	   of the corresponding pixels in the original vector-valued image.
	   \see get_orientation_pointwise, orientation_pointwise, norm_pointwise.
	**/
	CImg<typename cimg::largest<T,float>::type> get_norm_pointwise(int norm_type=2) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  if (is_empty()) return CImg<restype>();
	  CImg<restype> res(width,height,depth);
	  switch(norm_type) {
	  case -1: {             // Linf norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) {
		const restype tmp = (restype)cimg::abs((*this)(x,y,z,v));
		if (tmp>n) n=tmp; res(x,y,z) = n;
	  }
	}
	  } break;
	  case 1: {              // L1 norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) n+=cimg::abs((*this)(x,y,z,v)); res(x,y,z) = n;
	}
	  } break;
	  default: {             // L2 norm
	cimg_mapXYZ(*this,x,y,z) {
	  restype n=0; cimg_mapV(*this,v) n+=(*this)(x,y,z,v)*(*this)(x,y,z,v); res(x,y,z) = (restype)std::sqrt((double)n);
	}
	  } break;
	  }
	  return res;
	}

	//! Replace each pixel value with its vector norm.
	/**
	   This is the in-place version of \ref get_norm_pointwise().
	   \note Be careful when using this function on CImg<T> with T=char, unsigned char,unsigned int or int. The vector norm
	   is usually a floating point value, and a rough cast will be done here.
	**/
	CImg& norm_pointwise() { 
	  return CImg<T>(get_norm_pointwise()).swap(*this); 
	}
	
	//! Return the image of normalized vectors
	/**
	   When dealing with vector-valued images (i.e images with dimv()>1), this function return the image of normalized vectors
	   (unit vectors). Null vectors are unchanged. The L2-norm is computed for the normalization.
	   \return A new vector-valued image with same size, where each vector-valued pixels have been normalized.
	   \see get_norm_pointwise, norm_pointwise, orientation_pointwise.
	**/
	CImg<typename cimg::largest<T,float>::type> get_orientation_pointwise() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  if (is_empty()) return CImg<restype>();
	  return CImg<restype>(*this).orientation_pointwise(); 
	}

	//! Replace each pixel value by its normalized vector
	/** This is the in-place version of \ref get_orientation_pointwise() **/
	CImg& orientation_pointwise() {
	  cimg_mapXYZ(*this,x,y,z) {
		float n = 0.0f;
		cimg_mapV(*this,v) n+=(float)((*this)(x,y,z,v)*(*this)(x,y,z,v));
		n = (float)std::sqrt(n);
		if (n>0) cimg_mapV(*this,v) (*this)(x,y,z,v)=(T)((*this)(x,y,z,v)/n);
	else cimg_mapV(*this,v) (*this)(x,y,z,v)=0;
	  }
	  return *this;
	}

	//! Split image into a list CImgl<>.
	CImgl<T> get_split(const char axe,const unsigned int nb=0) const {
	  if (is_empty()) return CImgl<T>();
	  CImgl<T> res;
	  switch (cimg::uncase(axe)) {
	  case 'x': {
	if (nb>width)
	  throw CImgArgumentException("CImg<%s>::get_split() : Cannot split instance image (%u,%u,%u,%u,%p) along 'x' into %u images.",
					  pixel_type(),width,height,depth,dim,data,nb);
	res.assign(nb?nb:width);
	const unsigned int delta = width/res.size + ((width%res.size)?1:0);	
	unsigned int l,x;
	for (l=0, x=0; l<res.size-1; l++, x+=delta) res[l] = get_crop(x,0,0,0,x+delta-1,height-1,depth-1,dim-1);	
	res[res.size-1] = get_crop(x,0,0,0,width-1,height-1,depth-1,dim-1);
	  } break;
	  case 'y': {
	if (nb>height)
	  throw CImgArgumentException("CImg<%s>::get_split() : Cannot split instance image (%u,%u,%u,%u,%p) along 'y' into %u images.",
					  pixel_type(),width,height,depth,dim,data,nb);
	res.assign(nb?nb:height);
	const unsigned int delta = height/res.size + ((height%res.size)?1:0);	
	unsigned int l,x;
	for (l=0, x=0; l<res.size-1; l++, x+=delta) res[l] = get_crop(0,x,0,0,width-1,x+delta-1,depth-1,dim-1);	
	res[res.size-1] = get_crop(0,x,0,0,width-1,height-1,depth-1,dim-1);
	  } break;
	  case 'z': {
	if (nb>depth)
	  throw CImgArgumentException("CImg<%s>::get_split() : Cannot split instance image (%u,%u,%u,%u,%p) along 'z' into %u images.",
					  pixel_type(),width,height,depth,dim,data,nb);
	res.assign(nb?nb:depth);
	const unsigned int delta = depth/res.size + ((depth%res.size)?1:0);	
	unsigned int l,x;
	for (l=0, x=0; l<res.size-1; l++, x+=delta) res[l] = get_crop(0,0,x,0,width-1,height-1,x+delta-1,dim-1);	
	res[res.size-1] = get_crop(0,0,x,0,width-1,height-1,depth-1,dim-1);
	  } break;
	  case 'v': {
	if (nb>dim)
	  throw CImgArgumentException("CImg<%s>::get_split() : Cannot split instance image (%u,%u,%u,%u,%p) along 'v' into %u images.",
					  pixel_type(),width,height,depth,dim,data,nb);
	res.assign(nb?nb:dim);
	const unsigned int delta = dim/res.size + ((dim%res.size)?1:0);	
	unsigned int l,x;
	for (l=0, x=0; l<res.size-1; l++, x+=delta) res[l] = get_crop(0,0,0,x,width-1,height-1,depth-1,x+delta-1);	
	res[res.size-1] = get_crop(0,0,0,x,width-1,height-1,depth-1,dim-1);
	  } break;
	  default:
	throw CImgArgumentException("CImg<%s>::get_split() : Unknow axe '%c', must be 'x','y','z' or 'v'",pixel_type(),axe);
	break;
	  }           
	  return res;
	}

	//! Append an image to another one
	CImg get_append(const CImg<T>& img,const char axis='x',const char align='c') const {
	  if (img.is_empty()) return *this;
	  if (is_empty()) return img;
	  CImgl<T> temp(2);
	  temp[0].width = width; temp[0].height = height; temp[0].depth = depth;
	  temp[0].dim = dim; temp[0].data = data;
	  temp[1].width = img.width; temp[1].height = img.height; temp[1].depth = img.depth;
	  temp[1].dim = img.dim; temp[1].data = img.data;
	  const CImg<T> res = temp.get_append(axis,align);
	  temp[0].width = temp[0].height = temp[0].depth = temp[0].dim = 0; temp[0].data = 0;
	  temp[1].width = temp[1].height = temp[1].depth = temp[1].dim = 0; temp[1].data = 0;
	  return res;
	}

	//! Append an image to another one (in-place version)
	CImg& append(const CImg<T>& img,const char axis='x', const char align='c') {
	  if (img.is_empty()) return *this;
	  if (is_empty()) return (*this=img);
	  return get_append(img,axis,align).swap(*this);
	}

	//! Append an image to another one (in-place operator<< version)
	CImg& operator<<(const CImg<T>& img) {
	  return append(img);
	}
	
	//! Return a list of images, corresponding to the XY-gradients of an image.
	/**
	   \param scheme = Numerical scheme used for the gradient computation :
	   - -1 = Backward finite differences
	   - 0 = Centered finite differences
	   - 1 = Forward finite differences
	   - 2 = Using Sobel masks
	   - 3 = Using rotation invariant masks
	   - 4 = Using Deriche recusrsive filter.
	**/
	CImgl<typename cimg::largest<T,float>::type> get_gradientXY(const int scheme=0) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  if (is_empty()) return CImgl<restype>(2);
	  CImgl<restype> res(2,width,height,depth,dim);
	  switch(scheme) {
	  case -1: { // backward finite differences
	CImg_3x3x1(I,restype);
	cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) { res[0](x,y,z,k) = Icc-Ipc; res[1](x,y,z,k) = Icc-Icp; } 
	  } break;
	  case 1: { // forward finite differences
	CImg_2x2x1(I,restype);
	cimg_mapZV(*this,z,k) cimg_map2x2x1(*this,x,y,z,k,I) { res[0](x,y,0,k) = Inc-Icc; res[1](x,y,z,k) = Icn-Icc; }
	  } break;
	  case 2: { // using Sobel mask
	CImg_3x3x1(I,restype);
	const float a = 1, b = 2;
	cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) {
	  res[0](x,y,z,k) = -a*Ipp-b*Ipc-a*Ipn+a*Inp+b*Inc+a*Inn;
	  res[1](x,y,z,k) = -a*Ipp-b*Icp-a*Inp+a*Ipn+b*Icn+a*Inn;
	}
	  } break;
	  case 3: { // using rotation invariant mask
	CImg_3x3x1(I,restype);     
	const float a = (float)(0.25*(2-std::sqrt(2.0))), b = (float)(0.5f*(std::sqrt(2.0)-1));
	cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) {
	  res[0](x,y,z,k) = -a*Ipp-b*Ipc-a*Ipn+a*Inp+b*Inc+a*Inn;
	  res[1](x,y,z,k) = -a*Ipp-b*Icp-a*Inp+a*Ipn+b*Icn+a*Inn;
	}
	  } break;
	  case 4: { // using Deriche filter with low standard variation
	res[0] = get_deriche(0,1,'x');
	res[1] = get_deriche(0,1,'y');
	  } break;
	  default: { // central finite differences
	CImg_3x3x1(I,restype);
	cimg_mapZV(*this,z,k) cimg_map3x3x1(*this,x,y,z,k,I) { 
	  res[0](x,y,z,k) = 0.5f*(Inc-Ipc);
	  res[1](x,y,z,k) = 0.5f*(Icn-Icp); 
	} 
	  } break;
	  }
	  return res;
	}
	   
	//! Return a list of images, corresponding to the XYZ-gradients of an image.
	/**
	   \see get_gradientXY().
	**/
	CImgl<typename cimg::largest<T,float>::type> get_gradientXYZ(const int scheme=0) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  if (is_empty()) return CImgl<restype>(3);
	  CImgl<restype> res(3,width,height,depth,dim);
	  CImg_3x3x3(I,restype);
	  switch(scheme) {
	  case -1: { // backward finite differences
		cimg_mapV(*this,k) cimg_map3x3x3(*this,x,y,z,k,I) { 
		  res[0](x,y,z,k) = Iccc-Ipcc;
		  res[1](x,y,z,k) = Iccc-Icpc;
		  res[2](x,y,z,k) = Iccc-Iccp; 
		}
	  } break;
	  case 1: { // forward finite differences
		cimg_mapV(*this,k) cimg_map3x3x3(*this,x,y,z,k,I) {
		  res[0](x,y,z,k) = Incc-Iccc; 
		  res[1](x,y,z,k) = Icnc-Iccc;
		  res[2](x,y,z,k) = Iccn-Iccc; 
		} 
	  } break;
	  case 4: { // using Deriche filter with low standard variation
	res[0] = get_deriche(0,1,'x');
	res[1] = get_deriche(0,1,'y');
	res[2] = get_deriche(0,1,'z');
	  } break;
	  default: { // central finite differences
		cimg_mapV(*this,k) cimg_map3x3x3(*this,x,y,z,k,I) {
		  res[0](x,y,z,k) = 0.5f*(Incc-Ipcc);
		  res[1](x,y,z,k) = 0.5f*(Icnc-Icpc); 
		  res[2](x,y,z,k) = 0.5f*(Iccn-Iccp); 
		} 
	  } break;
	  }
	  return res;
	}
	
	struct _marching_cubes_func {
	  const CImg<T>& ref;
	  _marching_cubes_func(const CImg<T>& pref):ref(pref) {}
	  float operator()(const float x, const float y, const float z) const {
	return (float)ref((int)x,(int)y,(int)z);
	  }
	};
	
	//! Get a triangularization of an implicit function defined by the instance image
	template<typename tp, typename tf>
	const CImg& marching_cubes(const float isovalue,CImgl<tp>& points, CImgl<tf>& primitives,			   
				   const bool invert_faces = false) const {
	  if (depth<=1 || dim>1)
	throw CImgInstanceException("CImg<%s>::marching_cubes() : Instance image (%u,%u,%u,%u,%p) is not a 3D scalar image.",
					pixel_type(),width,height,depth,dim,data);
	  const _marching_cubes_func func(*this);
	  cimg::marching_cubes(func,isovalue,0.0f,0.0f,0.0f,dimx()-1.0f,dimy()-1.0f,dimz()-1.0f,
			   1.0f,1.0f,1.0f,points,primitives,invert_faces);
	  return *this;
	}

	struct _marching_cubes_func_float {
	  const CImg<T>& ref;
	  _marching_cubes_func_float(const CImg<T>& pref):ref(pref) {}
	  float operator()(const float x, const float y, const float z) const {
	return (float)ref.linear_pix3d(x,y,z);
	  }
	};
	
	//! Get a triangularization of an implicit function defined by the instance image
	/**
	   This version allows to specify the marching cube resolution along x,y and z.
	**/
	template<typename tp, typename tf>
	const CImg& marching_cubes(const float isovalue,
				   const float resx, const float resy, const float resz,
				   CImgl<tp>& points, CImgl<tf>& primitives,
				   const bool invert_faces = false) const {
	  if (depth<=1 || dim>1)
	throw CImgInstanceException("CImg<%s>::marching_cubes() : Instance image (%u,%u,%u,%u,%p) is not a 3D scalar image.",
					pixel_type(),width,height,depth,dim,data);
	  const _marching_cubes_func_float func(*this);
	  cimg::marching_cubes(func,isovalue,0.0f,0.0f,0.0f,dimx()-1.0f,dimy()-1.0f,dimz()-1.0f,
			   resx,resy,resz,points,primitives,invert_faces);
	  return *this;
	}

	struct _marching_squares_func {
	  const CImg<T>& ref;
	  _marching_squares_func(const CImg<T>& pref):ref(pref) {}
	  float operator()(const float x, const float y) const {
	return (float)ref((int)x,(int)y);
	  }
	};

	//! Get a vectorization of an implicit function defined by the instance image.
	template<typename tp, typename tf>
	const CImg& marching_squares(const float isovalue,CImgl<tp>& points, CImgl<tf>& primitives) const {
	  if (height<=1 || depth>1 || dim>1)
	throw CImgInstanceException("CImg<%s>::marching_squares() : Instance image (%u,%u,%u,%u,%p) is not a 2D scalar image.",
					pixel_type(),width,height,depth,dim,data);
	  const _marching_squares_func func(*this);
	  cimg::marching_squares(func,isovalue,0.0f,0.0f,dimx()-1.0f,dimy()-1.0f,1.0f,1.0f,points,primitives);
	  return *this;
	}

	struct _marching_squares_func_float {
	  const CImg<T>& ref;
	  _marching_squares_func_float(const CImg<T>& pref):ref(pref) {}
	  float operator()(const float x, const float y) const {
	return (float)ref.linear_pix2d(x,y);
	  }
	};

	//! Get a vectorization of an implicit function defined by the instance image.
	/**
	   This version allows to specify the marching squares resolution along x,y, and z.
	**/   
	template<typename tp, typename tf>
	const CImg& marching_squares(const float isovalue,
				 const float resx, const float resy,
				 CImgl<tp>& points, CImgl<tf>& primitives) const {
	  if (height<=1 || depth>1 || dim>1)
	throw CImgInstanceException("CImg<%s>::marching_squares() : Instance image (%u,%u,%u,%u,%p) is not a 2D scalar image.",
					pixel_type(),width,height,depth,dim,data);
	  const _marching_squares_func_float func(*this);
	  cimg::marching_squares(func,isovalue,0.0f,0.0f,dimx()-1.0f,dimy()-1.0f,resx,resy,points,primitives);
	  return *this;
	}

	//@}
	//
	//
	//
	//! \name Color conversion functions
	//@{
	//
	//

	//! Return the default 256 colors palette.
	/**
	   The default color palette is used by %CImg when displaying images on 256 colors displays.
	   It consists in the quantification of the (R,G,B) color space using 3:3:2 bits for color coding
	   (i.e 8 levels for the Red and Green and 4 levels for the Blue).
	   \return A 256x1x1x3 color image defining the palette entries.
	**/
	static CImg<T> get_default_LUT8() {
	  static CImg<T> palette;
	  if (!palette.data) {
	palette.assign(256,1,1,3);
	for (unsigned int index=0, r=16; r<256; r+=32)
	  for (unsigned int g=16; g<256; g+=32)
		for (unsigned int b=32; b<256; b+=64) {
		  palette(index,0) = r;
		  palette(index,1) = g;
		  palette(index++,2) = b;
		}
	  }
	  return palette;
	}
	
	//! Convert color pixels from (R,G,B) to match a specified palette.
	/**
	   This function return a (R,G,B) image where colored pixels are constrained to match entries
	   of the specified color \c palette.
	   \param palette User-defined palette that will constraint the color conversion.
	   \param dithering Enable/Disable Floyd-Steinberg dithering.
	   \param indexing If \c true, each resulting image pixel is an index to the given color palette.
	   Otherwise, (R,G,B) values of the palette are copied instead.
	**/
	template<typename t> CImg<t> get_RGBtoLUT(const CImg<t>& palette, const bool dithering=true,const bool indexing=false) const {
	  if (is_empty()) return CImg<t>();
	  if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoLUT() : Input image dimension is dim=%u, "
						  "should be a (R,G,B) image.",pixel_type(),dim);
	  if (palette.data && palette.dim!=3)
	throw CImgArgumentException("CImg<%s>::RGBtoLUT() : Given palette dimension is dim=%u, "
					"should be a (R,G,B) palette",pixel_type(),palette.dim);
	  CImg<t> res(width,height,depth,indexing?1:3), pal = palette.data?palette:CImg<t>::get_default_LUT8();
	  float *line1 = new float[3*width], *line2 = new float[3*width], *pline1 = line1, *pline2 = line2;
	  cimg_mapZ(*this,z) {
	float *ptr=pline2; cimg_mapX(*this,x) { *(ptr++)=(*this)(x,0,z,0); *(ptr++)=(*this)(x,0,z,1); *(ptr++)=(*this)(x,0,z,2); }
	cimg_mapY(*this,y) {
	  cimg::swap(pline1,pline2);
	  if (y<dimy()-1) {
		const int ny = y+1;
		float *ptr=pline2; cimg_mapX(*this,x) { *(ptr++)=(*this)(x,ny,z,0); *(ptr++)=(*this)(x,ny,z,1); *(ptr++)=(*this)(x,ny,z,2); }
	  }
	  float *ptr1=pline1, *ptr2=pline2;
	  cimg_mapX(*this,x) {
		float R = *(ptr1++), G = *(ptr1++), B = *(ptr1++);
		R = R<0?0:(R>255?255:R); G = G<0?0:(G>255?255:G); B = B<0?0:(B>255?255:B);
		int best_index = 0;
		t Rbest=0,Gbest=0,Bbest=0;
		if (palette.data) {	// find best match in given color palette
		  float min = cimg::type<float>::max();
		  cimg_mapX(palette,off) {
		const t Rp = palette(off,0), Gp = palette(off,1), Bp = palette(off,2);
		const float error = (float)((Rp-R)*(Rp-R) + (Gp-G)*(Gp-G) + (Bp-B)*(Bp-B));
		if (error<min) { min=error; best_index=off; Rbest=Rp; Gbest=Gp; Bbest=Bp; }
		  }
		} else {
		  Rbest = (t)((unsigned char)R&0xe0); Gbest = (t)((unsigned char)G&0xe0); Bbest = (t)((unsigned char)B&0xc0);
		  best_index = (unsigned char)Rbest | ((unsigned char)Gbest>>3) | ((unsigned char)Bbest>>6);
		}
		if (indexing) res(x,y,z) = best_index;
		else { res(x,y,z,0) = Rbest; res(x,y,z,1) = Gbest; res(x,y,z,2) = Bbest; }
		if (dithering) { // apply dithering to neighborhood pixels if needed
		  const float dR = (float)(R-Rbest), dG = (float)(G-Gbest),	dB = (float)(B-Bbest);
		  if (x<dimx()-1) { *(ptr1++)+= dR*7/16; *(ptr1++)+= dG*7/16; *(ptr1++)+= dB*7/16; ptr1-=3; }
		  if (y<dimy()-1) {	
		*(ptr2++)+= dR*5/16; *(ptr2++)+= dG*5/16; *ptr2+= dB*5/16; ptr2-=2;
		if (x>0) { *(--ptr2)+= dB*3/16; *(--ptr2)+= dG*3/16; *(--ptr2)+= dR*3/16; ptr2+=3; }
		if (x<dimx()-1) { ptr2+=3; *(ptr2++)+= dR/16; *(ptr2++)+= dG/16; *ptr2+= dB/16; ptr2-=5; }
		  }
		}
		ptr2+=3;
	  }
	}
	  }
	  delete[] line1; delete[] line2;
	  return res;
	}

	//! Convert color pixels from (R,G,B) to match the default 256 colors palette.
	/**
	   Same as get_RGBtoLUT() with the default color palette given by get_default_LUT8().
	**/
	CImg<T> get_RGBtoLUT(const bool dithering=true, const bool indexing=false) const {
	  CImg<T> foo;
	  return get_RGBtoLUT(foo,dithering,indexing); 
	}
	
	//! Convert color pixels from (R,G,B) to match the specified color palette.
	/** This is the in-place version of get_RGBtoLUT(). **/
	CImg& RGBtoLUT(const CImg<T>& palette,const bool dithering=true,const bool indexing=false) {
	  return get_RGBtoLUT(palette,dithering,indexing).swap(*this);
	}

	//! Convert color pixels from (R,G,B) to match the specified color palette.
	/** This is the in-place version of get_RGBtoLUT(). **/
	CImg& RGBtoLUT(const bool dithering=true,const bool indexing=false) {
	  CImg<T> foo;
	  return get_RGBtoLUT(foo,dithering,indexing).swap(*this); 
	}
	
	//! Convert an indexed image to a (R,G,B) image using the specified color palette.    
	template<typename t> CImg<t> get_LUTtoRGB(const CImg<t>& palette) const {
	  if (is_empty()) return CImg<t>();
	  if (dim!=1) throw CImgInstanceException("CImg<%s>::LUTtoRGB() : Input image dimension is dim=%u, "
						  "should be a LUT image",pixel_type(),dim);
	  if (palette.data && palette.dim!=3) 
	throw CImgArgumentException("CImg<%s>::LUTtoRGB() : Given palette dimension is dim=%u, "
					"should be a (R,G,B) palette",pixel_type(),palette.dim);
	  CImg<t> res(width,height,depth,3);
	  CImg<t> pal = palette.data?palette:get_default_LUT8();      
	  cimg_mapXYZ(*this,x,y,z) {
	const unsigned int index = (unsigned int)(*this)(x,y,z);
	res(x,y,z,0) = pal(index,0);
	res(x,y,z,1) = pal(index,1);
	res(x,y,z,2) = pal(index,2);
	  }
	  return res;
	}

	//! Convert an indexed image (with the default palette) to a (R,G,B) image.
	CImg<T> get_LUTtoRGB() const { 
	  CImg<T> foo;
	  return get_LUTtoRGB(foo); 
	}

	//! In-place version of get_LUTtoRGB().
	CImg& LUTtoRGB(const CImg<T>& palette) { 
	  return get_LUTtoRGB(palette).swap(*this); 
	}

	//! In-place version of get_LUTroRGB().
	CImg& LUTtoRGB() { 
	  CImg<T> foo;
	  return get_LUTtoRGB(foo).swap(*this); 
	}

	//! Convert color pixels from (R,G,B) to (H,S,V).
	CImg& RGBtoHSV() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoHSV() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image.",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
		R = (float)((*this)(x,y,z,0)/255.0f),
		G = (float)((*this)(x,y,z,1)/255.0f),
		B = (float)((*this)(x,y,z,2)/255.0f);
	  const float m = cimg::min(R,G,B), v = cimg::max(R,G,B);
	  float h,s;
	  if (v==m) { h=-1; s=0; } else {
		const float 
		  f = (R==m)?(G-B):((G==m)?(B-R):(R-G)),
		  i = (R==m)?3.0f:((G==m)?5.0f:1.0f);
		h = (i-f/(v-m));
		s = (v-m)/v;
		if (h>=6.0f) h-=6.0f;
		h*=(float)cimg::PI/3.0f;
	  }
	  (*this)(x,y,z,0) = (T)h;
	  (*this)(x,y,z,1) = (T)s;
	  (*this)(x,y,z,2) = (T)v;
	}
	  }
	  return *this;
	}

	//! Convert color pixels from (H,S,V) to (R,G,B).
	CImg& HSVtoRGB() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::HSVtoRGB() : Input image dimension is dim=%u, "
						"should be a (H,S,V) image",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  float
		H = (float)((*this)(x,y,z,0)),
		S = (float)((*this)(x,y,z,1)),
		V = (float)((*this)(x,y,z,2));
	  float R=0,G=0,B=0;
	  if (H<0) R=G=B=V;
	  else {
		H/=(float)cimg::PI/3.0f;
		const int i = (int)std::floor(H);
		const float
		  f = (i&1)?(H-i):(1.0f-H+i),
		  m = V*(1.0f-S),
		  n = V*(1.0f-S*f);
		switch(i) {
		case 6:
		case 0: R=V; G=n; B=m; break;
		case 1: R=n; G=V; B=m; break;
		case 2: R=m; G=V; B=n; break;
		case 3: R=m; G=n; B=V; break;
		case 4: R=n; G=m; B=V; break;
		case 5: R=V; G=m; B=n; break;
		}
	  }
	  (*this)(x,y,z,0) = (T)(R*255.0f);
	  (*this)(x,y,z,1) = (T)(G*255.0f);
	  (*this)(x,y,z,2) = (T)(B*255.0f);
	}
	  }
	  return *this;
	}
	
	//! Convert color pixels from (R,G,B) to (Y,Cb,Cr)_8 (Thanks to Chen Wang).
	CImg& RGBtoYCbCr() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoYCbCr() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const int 
		R = (int)((*this)(x,y,z,0)),
		G = (int)((*this)(x,y,z,1)),
		B = (int)((*this)(x,y,z,2));
	  const int
		Y  = ((66*R+129*G+25*B+128)>>8) + 16,
		Cb = ((-38*R-74*G+112*B+128)>>8) + 128,
		Cr = ((112*R-94*G-18*B+128)>>8) + 128;
	  (*this)(x,y,z,0) = (T)(Y<0?0:(Y>255?255:Y));
	  (*this)(x,y,z,1) = (T)(Cb<0?0:(Cb>255?255:Cb));
	  (*this)(x,y,z,2) = (T)(Cr<0?0:(Cr>255?255:Cr));
	}
	  }
	  return *this;
	}
	
	//! Convert color pixels from (Y,Cb,Cr)_8 to (R,G,B).
	CImg& YCbCrtoRGB() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::YCbCrtoRGB() : Input image dimension is dim=%u, "
						"should be a (Y,Cb,Cr)_8 image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const int 
		Y  = (int)((*this)(x, y, z, 0)-16), 
		Cb = (int)((*this)(x, y, z, 1)-128),
		Cr = (int)((*this)(x, y, z, 2)-128);
	  const int 
		R = ((298*Y + 409*Cr + 128) >> 8 ),
		G = ((298*Y - 100*Cb - 208*Cr + 128) >> 8 ),
		B = ((298*Y + 516*Cb + 128) >> 8 );
	  (*this)(x,y,z,0) = (T)(R<0?0:(R>255?255:R));
	  (*this)(x,y,z,1) = (T)(G<0?0:(G>255?255:G));
	  (*this)(x,y,z,2) = (T)(B<0?0:(B>255?255:B));
	}
	  }
	  return *this;
	}
	  
	//! Convert color pixels from (R,G,B) to (Y,U,V).
	CImg& RGBtoYUV() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoYUV() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
		R = (*this)(x,y,z,0)/255.0f,
		G = (*this)(x,y,z,1)/255.0f,
		B = (*this)(x,y,z,2)/255.0f,
		Y = (T)(0.299*R + 0.587*G + 0.114*B);
	  (*this)(x,y,z,0) = Y;
	  (*this)(x,y,z,1) = (T)(0.492*(B-Y));
	  (*this)(x,y,z,2) = (T)(0.877*(R-Y));
	}
	  }
	  return *this;
	}

	//! Convert color pixels from (Y,U,V) to (R,G,B).
	CImg& YUVtoRGB() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::YUVtoRGB() : Input image dimension is dim=%u, "
						"should be a (Y,U,V) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T Y = (*this)(x,y,z,0), U = (*this)(x,y,z,1), V = (*this)(x,y,z,2);
	  (*this)(x,y,z,0) = (T)((Y + 1.140*V)*255.0f);
	  (*this)(x,y,z,1) = (T)((Y - 0.395*U - 0.581*V)*255.0f);
	  (*this)(x,y,z,2) = (T)((Y + 2.032*U)*255.0f);
	}
	  }
	  return *this;
	}  

	//! Convert color pixels from (R,G,B) to (X,Y,Z)_709.    
	CImg& RGBtoXYZ() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::RGBtoXYZ() : Input image dimension is dim=%u, "
						"should be a (R,G,B) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float
		R = (float)((*this)(x,y,z,0)/255.0f),
		G = (float)((*this)(x,y,z,1)/255.0f),
		B = (float)((*this)(x,y,z,2)/255.0f);
	  (*this)(x,y,z,0) = (T)(0.412453*R + 0.357580*G + 0.180423*B);
	  (*this)(x,y,z,1) = (T)(0.212671*R + 0.715160*G + 0.072169*B);
	  (*this)(x,y,z,2) = (T)(0.019334*R + 0.119193*G + 0.950227*B);
	}
	  }
	  return *this;
	}

	//! Convert (X,Y,Z)_709 pixels of a color image into the (R,G,B) color space.
	CImg& XYZtoRGB() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoRGB() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const float 
		X = (float)(255.0f*(*this)(x,y,z,0)),
		Y = (float)(255.0f*(*this)(x,y,z,1)),
		Z = (float)(255.0f*(*this)(x,y,z,2));
	  (*this)(x,y,z,0) = (T)(3.240479*X  - 1.537150*Y - 0.498535*Z);
	  (*this)(x,y,z,1) = (T)(-0.969256*X + 1.875992*Y + 0.041556*Z);
	  (*this)(x,y,z,2) = (T)(0.055648*X  - 0.204043*Y + 1.057311*Z);
	}
	  }
	  return *this;
	}

	//! Convert (X,Y,Z)_709 pixels of a color image into the (L*,a*,b*) color space.
#define cimg_Labf(x)  ((x)>=0.008856?(std::pow(x,1/3.0)):(7.787*(x)+16.0/116.0))
#define cimg_Labfi(x) ((x)>=0.206893?((x)*(x)*(x)):(((x)-16.0/116.0)/7.787))

	CImg& XYZtoLab() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoLab() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	const double
	  Xn = 0.412453 + 0.357580 + 0.180423,
	  Yn = 0.212671 + 0.715160 + 0.072169,
	  Zn = 0.019334 + 0.119193 + 0.950227;
	cimg_mapXYZ(*this,x,y,z) {
	  const T X = (*this)(x,y,z,0), Y = (*this)(x,y,z,1), Z = (*this)(x,y,z,2);
	  const double
		XXn = X/Xn, YYn = Y/Yn, ZZn = Z/Zn,
		fX = cimg_Labf(XXn), fY = cimg_Labf(YYn), fZ = cimg_Labf(ZZn);
	  (*this)(x,y,z,0) = (T)(116*fY-16);
	  (*this)(x,y,z,1) = (T)(500*(fX-fY));
	  (*this)(x,y,z,2) = (T)(200*(fY-fZ));
	}
	  }
	  return *this;
	}

	//! Convert (L,a,b) pixels of a color image into the (X,Y,Z) color space.
	CImg& LabtoXYZ() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::LabtoXYZ() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	const double
	  Xn = 0.412453 + 0.357580 + 0.180423,
	  Yn = 0.212671 + 0.715160 + 0.072169,
	  Zn = 0.019334 + 0.119193 + 0.950227;
	cimg_mapXYZ(*this,x,y,z) {
	  const T L = (*this)(x,y,z,0), a = (*this)(x,y,z,1), b = (*this)(x,y,z,2);
	  const double
		cY = (L+16)/116.0,
		Y = Yn*cimg_Labfi(cY),
		pY = std::pow(Y/Yn,1.0/3),
		cX = a/500+pY,
		X = Xn*cX*cX*cX,
		cZ = pY-b/200,
		Z = Zn*cZ*cZ*cZ;
	  (*this)(x,y,z,0) = (T)(X);
	  (*this)(x,y,z,1) = (T)(Y);
	  (*this)(x,y,z,2) = (T)(Z);
	}
	  }
	  return *this;
	}

	//! Convert (X,Y,Z)_709 pixels of a color image into the (x,y,Y) color space.
	CImg& XYZtoxyY() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::XYZtoxyY() : Input image dimension is dim=%u, "
						"should be a (X,Y,Z) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T X = (*this)(x,y,z,0), Y = (*this)(x,y,z,1), Z = (*this)(x,y,z,2), sum = (X+Y+Z), nsum = sum>0?sum:1;
	  (*this)(x,y,z,0) = X/nsum;
	  (*this)(x,y,z,1) = Y/nsum;
	  (*this)(x,y,z,2) = Y;
	}
	  }
	  return *this;
	}
	
	//! Convert (x,y,Y) pixels of a color image into the (X,Y,Z)_709 color space.
	CImg& xyYtoXYZ() {
	  if (!is_empty()) {
	if (dim!=3) throw CImgInstanceException("CImg<%s>::xyYtoXYZ() : Input image dimension is dim=%u, "
						"should be a (x,y,Y) image (dim=3)",pixel_type(),dim);
	cimg_mapXYZ(*this,x,y,z) {
	  const T px = (*this)(x,y,z,0), py = (*this)(x,y,z,1), Y = (*this)(x,y,z,2), ny = py>0?py:1;
	  (*this)(x,y,z,0) = (T)(px*Y/ny);
	  (*this)(x,y,z,1) = Y;
	  (*this)(x,y,z,2) = (T)((1-px-py)*Y/ny);
	}
	  }
	  return *this;
	}
	
	//! In-place version of get_RGBtoLab().
	CImg& RGBtoLab() { 
	  return RGBtoXYZ().XYZtoLab(); 
	}

	//! In-place version of get_LabtoRGb().
	CImg& LabtoRGB() {
	  return LabtoXYZ().XYZtoRGB(); 
	}

	//! In-place version of get_RGBtoxyY().
	CImg& RGBtoxyY() { 
	  return RGBtoXYZ().XYZtoxyY(); 
	}

	//! In-place version of get_xyYtoRGB().
	CImg& xyYtoRGB() {
	  return xyYtoXYZ().XYZtoRGB(); 
	}

	//! Convert a (R,G,B) image to a (H,S,V) one.
	CImg get_RGBtoHSV() const { 
	  return CImg<T>(*this,true).RGBtoHSV(); 
	}

	//! Convert a (H,S,V) image to a (R,G,B) one.
	CImg get_HSVtoRGB() const { 
	  return CImg<T>(*this,true).HSVtoRGB(); 
	}

	//! Convert a (R,G,B) image to a (Y,Cb,Cr) one.
	CImg get_RGBtoYCbCr() const { 
	  return CImg<T>(*this,true).RGBtoYCbCr(); 
	}

	//! Convert a (Y,Cb,Cr) image to a (R,G,B) one.
	CImg get_YCbCrtoRGB() const {
	  return CImg<T>(*this,true).YCbCrtoRGB(); 
	}

	//! Convert a (R,G,B) image into a (Y,U,V) one.
	CImg<typename cimg::largest<T,float>::type> get_RGBtoYUV() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).RGBtoYUV();
	}
	
	//! Convert a (Y,U,V) image into a (R,G,B) one.
	CImg get_YUVtoRGB() const {
	  return CImg<T>(*this,true).YUVtoRGB(); 
	}

	//! Convert a (R,G,B) image to a (X,Y,Z) one.
	CImg<typename cimg::largest<T,float>::type> get_RGBtoXYZ() const { 
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).RGBtoXYZ(); 
	}
	
	//! Convert a (X,Y,Z) image to a (R,G,B) one.
	CImg get_XYZtoRGB() const { 
	  return CImg<T>(*this,true).XYZtoRGB(); 
	}

	//! Convert a (X,Y,Z) image to a (L,a,b) one.
	CImg get_XYZtoLab() const {
	  return CImg<T>(*this,true).XYZtoLab(); 
	}

	//! Convert a (L,a,b) image to a (X,Y,Z) one.
	CImg get_LabtoXYZ() const {
	  return CImg<T>(*this,true).LabtoXYZ(); 
	}

	//! Convert a (X,Y,Z) image to a (x,y,Y) one.
	CImg get_XYZtoxyY() const {
	  return CImg<T>(*this,true).XYZtoxyY(); 
	}

	//! Convert a (x,y,Y) image to a (X,Y,Z) one.
	CImg get_xyYtoXYZ() const {
	  return CImg<T>(*this,true).xyYtoXYZ(); 
	}

	//! Convert a (R,G,B) image to a (L,a,b) one.
	CImg get_RGBtoLab() const {
	  return CImg<T>(*this,true).RGBtoLab(); 
	}

	//! Convert a (L,a,b) image to a (R,G,B) one.
	CImg get_LabtoRGB() const { 
	  return CImg<T>(*this,true).LabtoRGB(); 
	}

	//! Convert a (R,G,B) image to a (x,y,Y) one.
	CImg get_RGBtoxyY() const {
	  return CImg<T>(*this,true).RGBtoxyY(); 
	}

	//! Convert a (x,y,Y) image to a (R,G,B) one.
	CImg get_xyYtoRGB() const {
	  return CImg<T>(*this,true).xyYtoRGB(); 
	}
	
	//@}
	//
	//
	//
	//! \name Drawing functions
	//@{
	//
	//

	// Should be used only by member functions. Not an user-friendly function.
	// Pre-requisites : x0<x1, y-coordinate is valid, col is valid.
	CImg& draw_scanline(const int x0,const int x1,const int y,const T *const color,
			const float opacity=1, const float brightness=1, const bool init=false) {
	  static float nopacity=0, copacity=0;
	  static unsigned int whz=0;
	  static const T* col = 0;
	  if (init) { 
	nopacity = cimg::abs(opacity);
	copacity = 1-cimg::max(opacity,0.0f); 
	whz = width*height*depth;
	col = color;
	  } else {
	const int nx0 = cimg::max(x0,0), nx1 = cimg::min(x1,(int)width-1), dx = nx1-nx0;
	T *ptrd = ptr(0,y) + nx0;  // avoid problems when cimg_debug=2
	if (dx>=0) {
	  if (opacity>=1) {
		int off = whz-dx-1;
		if (sizeof(T)!=1) cimg_mapV(*this,k) {
		  const T val = (T)(*(col++)*brightness);
		  for (int x=dx; x>=0; x--) *(ptrd++)=val;
		  ptrd+=off;
		} else cimg_mapV(*this,k) { std::memset(ptrd,(int)(*(col++)*brightness),dx+1); ptrd+=whz; }
		col-=dim;
	  } else {
		int off = whz-dx-1;
		cimg_mapV(*this,k) {
		  const T val = (T)(*(col++)*brightness);
		  for (int x=dx; x>=0; x--) { *ptrd = (T)(val*nopacity + *ptrd*copacity); ptrd++; }
		  ptrd+=off;
		}
		col-=dim;
	  }
	}
	  }
	  return *this;
	}
	
	CImg& draw_scanline(const T *const color,const float opacity=1) { return draw_scanline(0,0,0,color,opacity,1.0f,true); }

	//! Draw a colored point in the instance image, at coordinates (\c x0,\c y0,\c z0).
	/**
	   \param x0 = X-coordinate of the vector-valued pixel to plot.
	   \param y0 = Y-coordinate of the vector-valued pixel to plot.
	   \param z0 = Z-coordinate of the vector-valued pixel to plot.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_point(const int x0,const int y0,const int z0,
					 const T *const color,const float opacity=1) {
	  if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_point() : Specified color is (null)",pixel_type());
	if (x0>=0 && y0>=0 && z0>=0 && x0<dimx() && y0<dimy() && z0<dimz()) {
	  const T *col=color;
	  const unsigned int whz = width*height*depth;
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  T *ptrd = ptr(x0,y0,z0,0);
	  if (opacity>=1) cimg_mapV(*this,k) { *ptrd = *(col++); ptrd+=whz; }
	  else cimg_mapV(*this,k) { *ptrd=(T)(*(col++)*nopacity + *ptrd*copacity); ptrd+=whz; }
	}
	  }
	  return *this;
	}

	//! Draw a colored point in the instance image, at coordinates (\c x0,\c y0).
	/**
	   \param x0 = X-coordinate of the vector-valued pixel to plot.
	   \param y0 = Y-coordinate of the vector-valued pixel to plot.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_point(const int x0,const int y0,const T *const color,const float opacity=1) { 
	  return draw_point(x0,y0,0,color,opacity); 
	}

	//! Draw a 2D colored line in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
	/**
	   \param x0 = X-coordinate of the starting point of the line.
	   \param y0 = Y-coordinate of the starting point of the line.
	   \param x1 = X-coordinate of the ending point of the line.
	   \param y1 = Y-coordinate of the ending point of the line.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param pattern = An integer whose bits describes the line pattern.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_line(const int x0,const int y0,const int x1,const int y1,
					const T *const color,const unsigned int pattern=~0L,const float opacity=1) {
	  if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_line() : Specified color is (null)",pixel_type());
	const T* col=color;
	unsigned int hatch=1;     
	int nx0=x0, nx1=x1, ny0=y0, ny1=y1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { ny0-=nx0*(ny1-ny0)/(nx1-nx0); nx0=0; }
	if (nx1>=dimx()) { ny1+=(nx1-dimx())*(ny0-ny1)/(nx1-nx0); nx1=dimx()-1;}
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { nx0-=ny0*(nx1-nx0)/(ny1-ny0); ny0=0; }
	if (ny1>=dimy()) { nx1+=(ny1-dimy())*(nx0-nx1)/(ny1-ny0); ny1=dimy()-1;}
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),ny1-ny0), whz = width*height*depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0;
	if (opacity>=1) for (unsigned int t=0; t<=dmax; t++) {
	  if (!(~pattern) || (~pattern && pattern&hatch)) {
		T* ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);      
		cimg_mapV(*this,k) { *ptrd=*(col++); ptrd+=whz; }
		col-=dim;
	  }
	  x+=px; y+=py; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	} else {
	  const float nopacity = cimg::abs(opacity), copacity=1-cimg::max(opacity,0.0f);
	  for (unsigned int t=0; t<=dmax; t++) {
		if (!(~pattern) || (~pattern && pattern&hatch)) {
		  T* ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
		  cimg_mapV(*this,k) { *ptrd = (T)(*(col++)*nopacity + copacity*(*ptrd)); ptrd+=whz; }
		  col-=dim;
		}
		x+=px; y+=py; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	  }
	}
	  }
	  return *this;
	}
  
	//! Draw a 3D colored line in the instance image, at coordinates (\c x0,\c y0,\c z0)-(\c x1,\c y1,\c z1).
	/**
	   \param x0 = X-coordinate of the starting point of the line.
	   \param y0 = Y-coordinate of the starting point of the line.
	   \param z0 = Z-coordinate of the starting point of the line.
	   \param x1 = X-coordinate of the ending point of the line.
	   \param y1 = Y-coordinate of the ending point of the line.
	   \param z1 = Z-coordinate of the ending point of the line.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param pattern = An integer whose bits describes the line pattern.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_line(const int x0,const int y0,const int z0,const int x1,const int y1,const int z1,
					const T *const color,const unsigned int pattern=~0L,const float opacity=1) {
	  if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_line() : Specified color is (null)",pixel_type());
	const T* col=color;
	unsigned int hatch=1;
	int nx0=x0, ny0=y0, nz0=z0, nx1=x1, ny1=y1, nz1=z1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { const int D=nx1-nx0; ny0-=nx0*(ny1-ny0)/D; nz0-=nx0*(nz1-nz0)/D; nx0=0; }
	if (nx1>=dimx()) { const int d=nx1-dimx(), D=nx1-nx0; ny1+=d*(ny0-ny1)/D; nz1+=d*(nz0-nz1)/D; nx1=dimx()-1;}
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { const int D=ny1-ny0; nx0-=ny0*(nx1-nx0)/D; nz0-=ny0*(nz1-nz0)/D; ny0=0; }
	if (ny1>=dimy()) { const int d=ny1-dimy(), D=ny1-ny0; nx1+=d*(nx0-nx1)/D; nz1+=d*(nz0-nz1)/D; ny1=dimy()-1;}
	if (nz0>nz1) cimg::swap(nx0,nx1,ny0,ny1,nz0,nz1);
	if (nz1<0 || nz0>=dimz()) return *this;
	if (nz0<0) { const int D=nz1-nz0; nx0-=nz0*(nx1-nx0)/D; ny0-=nz0*(ny1-ny0)/D; nz0=0; }
	if (nz1>=dimz()) { const int d=nz1-dimz(), D=nz1-nz0; nx1+=d*(nx0-nx1)/D; ny1+=d*(ny0-ny1)/D; nz1=dimz()-1;}
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),cimg::abs(ny1-ny0),nz1-nz0), whz = width*height*depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0, pz = dmax?(nz1-nz0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0, z = (float)nz0;
	if (opacity>=1) for (unsigned int t=0; t<=dmax; t++) { 
	  if (!(~pattern) || (~pattern && pattern&hatch)) {
		T* ptrd = ptr((unsigned int)x,(unsigned int)y,(unsigned int)z,0);
		cimg_mapV(*this,k) { *ptrd=*(col++); ptrd+=whz; }        
		col-=dim; 
	  }
	  x+=px; y+=py; z+=pz; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	} else {
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  for (unsigned int t=0; t<=dmax; t++) { 
		if (!(~pattern) || (~pattern && pattern&hatch)) {
		  T* ptrd = ptr((unsigned int)x,(unsigned int)y,(unsigned int)z,0);
		  cimg_mapV(*this,k) { *ptrd = (T)(*(col++)*nopacity + copacity*(*ptrd)); ptrd+=whz; }
		  col-=dim; 
		}
		x+=px; y+=py; z+=pz; if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));        
	  }
	}
	  }
	  return *this;
	}

	//! Draw a 2D textured line in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
	/**
	   \param x0 = X-coordinate of the starting point of the line.
	   \param y0 = Y-coordinate of the starting point of the line.
	   \param x1 = X-coordinate of the ending point of the line.
	   \param y1 = Y-coordinate of the ending point of the line.
	   \param texture = a colored texture image used to draw the line color.
	   \param tx0 = X-coordinate of the starting point of the texture.
	   \param ty0 = Y-coordinate of the starting point of the texture.
	   \param tx1 = X-coordinate of the ending point of the texture.
	   \param ty1 = Y-coordinate of the ending point of the texture.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported, but texture coordinates do not support clipping.
	**/
	template<typename t> CImg& draw_line(const int x0,const int y0,const int x1,const int y1,
										 const CImg<t>& texture,
										 const int tx0,const int ty0,const int tx1,const int ty1,
										 const float opacity=1) {
	  if (!is_empty()) {
	if (texture.is_empty() || texture.dim<dim)
	  throw CImgArgumentException("CImg<%s>::draw_line() : specified texture (%u,%u,%u,%u,%p) has wrong dimensions.",
					  pixel_type(),texture.width,texture.height,texture.depth,texture.dim,texture.data);
	int nx0=x0, ny0=y0, nx1=x1, ny1=y1, ntx0=tx0, nty0=ty0, ntx1=tx1, nty1=ty1;
	if (nx0>nx1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1);
	if (nx1<0 || nx0>=dimx()) return *this;
	if (nx0<0) { const int D=nx1-nx0; ny0-=nx0*(ny1-ny0)/D; ntx0-=nx0*(ntx1-ntx0)/D; nty0-=nx0*(nty1-nty0)/D; nx0=0; }
	if (nx1>=dimx()) { const int d=nx1-dimx(),D=nx1-nx0; ny1+=d*(ny0-ny1)/D; ntx1+=d*(ntx0-ntx1)/D; nty1+=d*(nty0-nty1)/D; nx1=dimx()-1; }
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1);
	if (ny1<0 || ny0>=dimy()) return *this;
	if (ny0<0) { const int D=ny1-ny0; nx0-=ny0*(nx1-nx0)/D; ntx0-=ny0*(ntx1-ntx0)/D; nty0-=ny0*(nty1-nty0)/D; ny0=0; }
	if (ny1>=dimy()) { const int d=ny1-dimy(),D=ny1-ny0; nx1+=d*(nx0-nx1)/D; ntx1+=d*(ntx0-ntx1)/D; nty1+=d*(nty0-nty1)/D; ny1=dimy()-1; }
	const unsigned int dmax = (unsigned int)cimg::max(cimg::abs(nx1-nx0),ny1-ny0), 
	  whz = width*height*depth, twhz = texture.width*texture.height*texture.depth;
	const float px = dmax?(nx1-nx0)/(float)dmax:0, py = dmax?(ny1-ny0)/(float)dmax:0,
	  tpx = dmax?(ntx1-ntx0)/(float)dmax:0, tpy = dmax?(nty1-nty0)/(float)dmax:0;
	float x = (float)nx0, y = (float)ny0, tx = (float)ntx0, ty = (float)nty0;
	if (opacity>=1) for (unsigned int tt=0; tt<=dmax; tt++) { 
	  T *ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
	  const t *ptrs = texture.ptr((unsigned int)tx,(unsigned int)ty,0,0);
	  cimg_mapV(*this,k) { *ptrd = (T)(*ptrs); ptrd+=whz; ptrs+=twhz; }
	  x+=px; y+=py; tx+=tpx; ty+=tpy;
	} else {
	  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	  for (unsigned int tt=0; tt<=dmax; tt++) { 
		T *ptrd = ptr((unsigned int)x,(unsigned int)y,0,0);
		const t *ptrs = texture.ptr((unsigned int)tx,(unsigned int)ty,0,0);
		cimg_mapV(*this,k) { *ptrd = (T)(nopacity*(*ptrs) + copacity*(*ptrd)); ptrd+=whz; ptrs+=twhz; }
		x+=px; y+=py; tx+=tpx; ty+=tpy;
	  }
	}
	  }
	  return *this;
	}

	//! Draw a 2D colored arrow in the instance image, at coordinates (\c x0,\c y0)->(\c x1,\c y1).
	/**
	   \param x0 = X-coordinate of the starting point of the arrow (tail).
	   \param y0 = Y-coordinate of the starting point of the arrow (tail).
	   \param x1 = X-coordinate of the ending point of the arrow (head).
	   \param y1 = Y-coordinate of the ending point of the arrow (head).
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param angle = aperture angle of the arrow head
	   \param length = length of the arrow head. If <0, described as a percentage of the arrow length.
	   \param pattern = An integer whose bits describes the line pattern.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_arrow(const int x0,const int y0,const int x1,const int y1,
					 const T *const color,
					 const float angle=30,const float length=-10,const unsigned int pattern=~0L,const float opacity=1) {
	  if (!is_empty()) {
	const float u = (float)(x0-x1), v = (float)(y0-y1), sq = u*u+v*v,
	  deg = (float)(angle*cimg::PI/180), ang = (sq>0)?(float)std::atan2(v,u):0.0f,
	  l = (length>=0)?length:-length*(float)std::sqrt(sq)/100;
	if (sq>0) {
	  const double cl = std::cos(ang-deg), sl = std::sin(ang-deg), cr = std::cos(ang+deg), sr = std::sin(ang+deg);        
	  const int 
		xl = x1+(int)(l*cl), yl = y1+(int)(l*sl),
		xr = x1+(int)(l*cr), yr = y1+(int)(l*sr),
		xc = x1+(int)((l+1)*(cl+cr))/2, yc = y1+(int)((l+1)*(sl+sr))/2;
	  draw_line(x0,y0,xc,yc,color,pattern,opacity).draw_triangle(x1,y1,xl,yl,xr,yr,color,opacity);
	} else draw_point(x0,y0,color,opacity);
	  }
	  return *this;
	}

	//! Draw a sprite image in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0).
	/**
	   \param sprite = sprite image.
	   \param x0 = X-coordinate of the sprite position in the instance image.
	   \param y0 = Y-coordinate of the sprite position in the instance image.
	   \param z0 = Z-coordinate of the sprite position in the instance image.
	   \param v0 = V-coordinate of the sprite position in the instance image.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	template<typename t> CImg& draw_image(const CImg<t>& sprite,
										  const int x0=0,const int y0=0,const int z0=0,const int v0=0,const float opacity=1) {
	  if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int 
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);
	const t *ptrs = sprite.ptr()-(bx?x0:0)-(by?y0*sprite.dimx():0)+(bz?z0*sprite.dimx()*sprite.dimy():0)+
	  (bv?v0*sprite.dimx()*sprite.dimy()*sprite.dimz():0);
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
		for (int z=0; z<lZ; z++) {
		  for (int y=0; y<lY; y++) {
		if (opacity>=1) for (int x=0; x<lX; x++) *(ptrd++) = (T)(*(ptrs++));
		else for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*(*(ptrs++)) + copacity*(*ptrd)); ptrd++; }
		ptrd+=offX; ptrs+=soffX;
		  }
		  ptrd+=offY; ptrs+=soffY;
		}
		ptrd+=offZ; ptrs+=soffZ;
	  }
	  }
	  return *this;
	}

#if ( !defined(_MSC_VER) || _MSC_VER>1300 )
	CImg& draw_image(const CImg& sprite,const int x0=0,const int y0=0,const int z0=0,const int v0=0,const float opacity=1) {
	  if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	if (this==&sprite) return draw_image(CImg<T>(sprite),x0,y0,z0,v0,opacity);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int 
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);
	const T *ptrs = sprite.ptr()-(bx?x0:0)-(by?y0*sprite.dimx():0)+(bz?z0*sprite.dimx()*sprite.dimy():0)+
	  (bv?v0*sprite.dimx()*sprite.dimy()*sprite.dimz():0);
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ),
	  slX = lX*sizeof(T);    
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
		for (int z=0; z<lZ; z++) {
		  if (opacity>=1) for (int y=0; y<lY; y++) { std::memcpy(ptrd,ptrs,slX); ptrd+=width; ptrs+=sprite.width; }
		  else for (int y=0; y<lY; y++) {
		for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*(*(ptrs++)) + copacity*(*ptrd)); ptrd++; }
		ptrd+=offX; ptrs+=soffX;
		  }
		  ptrd+=offY; ptrs+=soffY;
		}
		ptrd+=offZ; ptrs+=soffZ;
	  }
	  }
	  return *this;
	}
#endif

	//! Draw a masked sprite image in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0).
	/**
	   \param sprite = sprite image.
	   \param mask = mask image.
	   \param x0 = X-coordinate of the sprite position in the instance image.
	   \param y0 = Y-coordinate of the sprite position in the instance image.
	   \param z0 = Z-coordinate of the sprite position in the instance image.
	   \param v0 = V-coordinate of the sprite position in the instance image.
	   \param mask_valmax = Maximum pixel value of the mask image \c mask.
	   \param opacity = opacity of the drawing.
	   \note Pixel values of \c mask set the opacity of the corresponding pixels in \c sprite.
	   \note Clipping is supported.
	   \note Dimensions along x,y and z of \c sprite and \c mask must be the same.
	**/
	template<typename ti,typename tm> CImg& draw_image(const CImg<ti>& sprite,const CImg<tm>& mask,
													   const int x0=0,const int y0=0,const int z0=0,const int v0=0,
													   const tm mask_valmax=1,const float opacity=1) {
	  if (!is_empty()) {
	if (sprite.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified sprite image (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),sprite.width,sprite.height,sprite.depth,sprite.dim,sprite.data);
	if (mask.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_image() : Specified mask image (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	if ((void*)this==(void*)&sprite) return draw_image(CImg<T>(sprite),mask,x0,y0,z0,v0);
	if(mask.width!=sprite.width || mask.height!=sprite.height || mask.depth!=sprite.depth)
	  throw CImgArgumentException("CImg<%s>::draw_image() : Mask dimension is (%u,%u,%u,%u), while sprite is (%u,%u,%u,%u)",
					  pixel_type(),mask.width,mask.height,mask.depth,mask.dim,sprite.width,sprite.height,sprite.depth,sprite.dim);
	const bool bx=(x0<0), by=(y0<0), bz=(z0<0), bv=(v0<0);
	const int
	  lX = sprite.dimx() - (x0+sprite.dimx()>dimx()?x0+sprite.dimx()-dimx():0) + (bx?x0:0),
	  lY = sprite.dimy() - (y0+sprite.dimy()>dimy()?y0+sprite.dimy()-dimy():0) + (by?y0:0),
	  lZ = sprite.dimz() - (z0+sprite.dimz()>dimz()?z0+sprite.dimz()-dimz():0) + (bz?z0:0),      
	  lV = sprite.dimv() - (v0+sprite.dimv()>dimv()?v0+sprite.dimv()-dimv():0) + (bv?v0:0);    
	const int coff = -(bx?x0:0)-(by?y0*mask.dimx():0)-(bz?z0*mask.dimx()*mask.dimy():0)-
	  (bv?v0*mask.dimx()*mask.dimy()*mask.dimz():0),
	  ssize = mask.dimx()*mask.dimy()*mask.dimz();
	const ti *ptrs = sprite.ptr() + coff;
	const tm *ptrm = mask.ptr() + coff;
	const unsigned int
	  offX = width-lX, soffX = sprite.width-lX,
	  offY = width*(height-lY), soffY = sprite.width*(sprite.height-lY),
	  offZ = width*height*(depth-lZ), soffZ = sprite.width*sprite.height*(sprite.depth-lZ);
	T *ptrd = ptr(x0<0?0:x0,y0<0?0:y0,z0<0?0:z0,v0<0?0:v0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
		ptrm = mask.data + (ptrm - mask.data)%ssize;
		for (int z=0; z<lZ; z++) {
		  for (int y=0; y<lY; y++) {
		for (int x=0; x<lX; x++) {
		  const float mopacity = *(ptrm++)*opacity,
			nopacity = cimg::abs(mopacity), copacity = mask_valmax-cimg::max(mopacity,0.0f);
		  *ptrd = (T)((nopacity*(*(ptrs++))+copacity*(*ptrd))/mask_valmax);
		  ptrd++;
		}
		ptrd+=offX; ptrs+=soffX; ptrm+=soffX;
		  }
		  ptrd+=offY; ptrs+=soffY; ptrm+=soffY;
		}
		ptrd+=offZ; ptrs+=soffZ; ptrm+=soffZ;
	  }
	  }
	  return *this;
	}

	//! Draw a 4D filled rectangle in the instance image, at coordinates (\c x0,\c y0,\c z0,\c v0)-(\c x1,\c y1,\c z1,\c v1).
	/**
	   \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
	   \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
	   \param z0 = Z-coordinate of the upper-left rectangle corner in the instance image.
	   \param v0 = V-coordinate of the upper-left rectangle corner in the instance image.
	   \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
	   \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
	   \param z1 = Z-coordinate of the lower-right rectangle corner in the instance image.
	   \param v1 = V-coordinate of the lower-right rectangle corner in the instance image.
	   \param val = scalar value used to fill the rectangle area.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_rectangle(const int x0,const int y0,const int z0,const int v0,
						 const int x1,const int y1,const int z1,const int v1,
						 const T& val,const float opacity=1.0f) {
	  if (!is_empty()) {	
	const bool bx=(x0<x1), by=(y0<y1), bz=(z0<z1), bv=(v0<v1);
	const int nx0=bx?x0:x1, nx1=bx?x1:x0, ny0=by?y0:y1, ny1=by?y1:y0, nz0=bz?z0:z1, nz1=bz?z1:z0, nv0=bv?v0:v1, nv1=bv?v1:v0;
	const int 
	  lX = (1+nx1-nx0) + (nx1>=dimx()?dimx()-1-nx1:0) + (nx0<0?nx0:0),
	  lY = (1+ny1-ny0) + (ny1>=dimy()?dimy()-1-ny1:0) + (ny0<0?ny0:0),
	  lZ = (1+nz1-nz0) + (nz1>=dimz()?dimz()-1-nz1:0) + (nz0<0?nz0:0),
	  lV = (1+nv1-nv0) + (nv1>=dimv()?dimv()-1-nv1:0) + (nv0<0?nv0:0);
	const unsigned int offX = width-lX, offY = width*(height-lY), offZ = width*height*(depth-lZ);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	T *ptrd = ptr(nx0<0?0:nx0,ny0<0?0:ny0,nz0<0?0:nz0,nv0<0?0:nv0);
	if (lX>0 && lY>0 && lZ>0 && lV>0)
	  for (int v=0; v<lV; v++) {
		for (int z=0; z<lZ; z++) {
		  for (int y=0; y<lY; y++) {
		if (opacity>=1) {
		  if (sizeof(T)!=1) { for (int x=0; x<lX; x++) *(ptrd++) = val; ptrd+=offX; }
		  else { std::memset(ptrd,(int)val,lX); ptrd+=width; }
		} else { for (int x=0; x<lX; x++) { *ptrd = (T)(nopacity*val+copacity*(*ptrd)); ptrd++; } ptrd+=offX; }
		  }
		  ptrd+=offY;
		}
		ptrd+=offZ;
	  }  
	  }
	  return *this;
	}

	//! Draw a 3D filled colored rectangle in the instance image, at coordinates (\c x0,\c y0,\c z0)-(\c x1,\c y1,\c z1).
	/**
	   \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
	   \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
	   \param z0 = Z-coordinate of the upper-left rectangle corner in the instance image.
	   \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
	   \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
	   \param z1 = Z-coordinate of the lower-right rectangle corner in the instance image.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_rectangle(const int x0,const int y0,const int z0,
						 const int x1,const int y1,const int z1,
						 const T *const color,const float opacity=1) {
	  if (!color) throw CImgArgumentException("CImg<%s>::draw_rectangle : specified color is (null)",pixel_type());
	  cimg_mapV(*this,k) draw_rectangle(x0,y0,z0,k,x1,y1,z1,k,color[k],opacity);
	  return *this;
	}

	//! Draw a 2D filled colored rectangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1).
	/**
	   \param x0 = X-coordinate of the upper-left rectangle corner in the instance image.
	   \param y0 = Y-coordinate of the upper-left rectangle corner in the instance image.
	   \param x1 = X-coordinate of the lower-right rectangle corner in the instance image.
	   \param y1 = Y-coordinate of the lower-right rectangle corner in the instance image.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_rectangle(const int x0,const int y0,const int x1,const int y1,
						 const T *const color,const float opacity=1) {
	  draw_rectangle(x0,y0,0,x1,y1,depth-1,color,opacity);
	  return *this;
	}
  
	//! Draw a 2D filled colored triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing (<1)
	   \param brightness = brightness of the drawing (in [0,1])
	   \note Clipping is supported.
	**/
	CImg& draw_triangle(const int x0,const int y0,
						const int x1,const int y1,
						const int x2,const int y2,
						const T *const color,
			const float opacity=1,
			const float brightness=1) {
	  draw_scanline(color,opacity);
	  int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2;
	  if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1);
	  if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2);
	  if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2);
	  if (ny0>=dimy() || ny2<0) return *this;
	  const float 
		p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
		p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
		p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1);
	  float xleft = (float)nx0, xright = xleft, pleft = (p1<p2)?p1:p2, pright = (p1<p2)?p2:p1;
	  if (ny0<0) { xleft-=ny0*pleft; xright-=ny0*pright; }
	  const int ya = ny1>dimy()?height:ny1;
	  for (int y=ny0<0?0:ny0; y<ya; y++) {
	draw_scanline((int)xleft,(int)xright,y,color,opacity,brightness); 
	xleft+=pleft; xright+=pright; 
	  }
	  if (p1<p2) { xleft=(float)nx1;  pleft=p3;  if (ny1<0) xleft-=ny1*pleft; } 
	  else       { xright=(float)nx1; pright=p3; if (ny1<0) xright-=ny1*pright; }
	  const int yb = ny2>=dimy()?height-1:ny2;
	  for (int yy=ny1<0?0:ny1; yy<=yb; yy++) { 
	draw_scanline((int)xleft,(int)xright,yy,color,opacity,brightness);
	xleft+=pleft; xright+=pright; 
	  }
	  return *this;
	}

	//! Draw a 2D Gouraud-filled triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param color = array of dimv() values of type \c T, defining the global drawing color.
	   \param c0 = brightness of the first corner.
	   \param c1 = brightness of the second corner.
	   \param c2 = brightness of the third corner.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	CImg& draw_triangle(const int x0,const int y0,
			const int x1,const int y1,
			const int x2,const int y2,
			const T *const color,
			const float c0,const float c1,const float c2,
			const float opacity=1) {
	  if (!is_empty()) {
	int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2,whz=width*height*depth;
	float nc0=c0,nc1=c1,nc2=c2;
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,nc0,nc1);
	if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2,nc0,nc2);
	if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2,nc1,nc2);
	if (ny0>=dimy() || ny2<0) return *this;
	const float 
	  p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
	  p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
	  p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1),
	  cp1 = (ny1-ny0)?(nc1-nc0)/(float)(ny1-ny0):0,
	  cp2 = (ny2-ny0)?(nc2-nc0)/(float)(ny2-ny0):0,
	  cp3 = (ny2-ny1)?(nc2-nc1)/(float)(ny2-ny1):0;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	float pleft,pright,cpleft,cpright,xleft=(float)nx0,xright=xleft,cleft=nc0,cright=cleft;
	if (p1<p2) { pleft=p1; pright=p2; cpleft=cp1; cpright=cp2; } 
	else       { pleft=p2; pright=p1; cpleft=cp2; cpright=cp1; }
	if (ny0<0) { xleft-=ny0*pleft; xright-=ny0*pright; cleft-=ny0*cpleft; cright-=ny0*cpright; }
	const int ya = ny1<dimy()?ny1:height;
	for (int y=(ny0<0?0:ny0); y<ya; y++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		cp = dx?(cright-cleft)/dx:0,
		ci = (xleft>=0)?cleft:(cleft-xleft*cp);
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  const T col = color[k];
		  float c=ci;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(c*col); c+=cp; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  const T col = color[k];
		  float c=ci;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*c*col+copacity*(*ptrd)); ptrd++; c+=cp; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; cleft+=cpleft; cright+=cpright;
	}
	
	if (p1<p2) {
	  xleft=(float)nx1; pleft=p3; cleft=nc1; cpleft=cp3;
	  if (ny1<0) { xleft-=ny1*pleft; cleft-=ny1*cpleft; }
	} else { 
	  xright=(float)nx1; pright=p3; cright=nc1; cpright=cp3;
	  if (ny1<0) { xright-=ny1*pright; cright-=ny1*cpright; }
	}    
	const int yb = ny2>=dimy()?(height-1):ny2;
	for (int yy=(ny1<0?0:ny1); yy<=yb; yy++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		cp = dx?(cright-cleft)/dx:0,
		ci = (xleft>=0)?cleft:(cleft-xleft*cp);
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,yy,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  const T col = color[k];
		  float c=ci;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(c*col); c+=cp; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  const T col = color[k];
		  float c=ci;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*c*col+copacity*(*ptrd)); ptrd++; c+=cp; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; cleft+=cpleft; cright+=cpright;
	}
	  }
	  return *this;
	}

	//! Draw a 2D phong-shaded triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param color = array of dimv() values of type \c T, defining the global drawing color.
	   \param light = light image.
	   \param lx0 = X-coordinate of the first corner in the light image.
	   \param ly0 = Y-coordinate of the first corner in the light image.
	   \param lx1 = X-coordinate of the second corner in the light image.
	   \param ly1 = Y-coordinate of the second corner in the light image.
	   \param lx2 = X-coordinate of the third corner in the light image.
	   \param ly2 = Y-coordinate of the third corner in the light image.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported, but texture coordinates do not support clipping.
	**/
	template<typename t> CImg& draw_triangle(const int x0,const int y0,
											 const int x1,const int y1,
											 const int x2,const int y2,
						 const T *const color,
											 const CImg<t>& light,
											 const int lx0,const int ly0,
											 const int lx1,const int ly1,
											 const int lx2,const int ly2,
											 const float opacity=1.0f) {
	  if (!is_empty()) {
	if (light.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_triangle() : Specified light texture (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),light.width,light.height,light.depth,light.dim,light.data);
	int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2,nlx0=lx0,nly0=ly0,nlx1=lx1,nly1=ly1,nlx2=lx2,nly2=ly2,whz=width*height*depth;
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,nlx0,nlx1,nly0,nly1);
	if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2,nlx0,nlx2,nly0,nly2);
	if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2,nlx1,nlx2,nly1,nly2);
	if (ny0>=dimy() || ny2<0) return *this;
	const float 
	  p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
	  p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
	  p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1),
	  lpx1 = (ny1-ny0)?(nlx1-nlx0)/(float)(ny1-ny0):0,
	  lpy1 = (ny1-ny0)?(nly1-nly0)/(float)(ny1-ny0):0,
	  lpx2 = (ny2-ny0)?(nlx2-nlx0)/(float)(ny2-ny0):0,
	  lpy2 = (ny2-ny0)?(nly2-nly0)/(float)(ny2-ny0):0,
	  lpx3 = (ny2-ny1)?(nlx2-nlx1)/(float)(ny2-ny1):0,
	  lpy3 = (ny2-ny1)?(nly2-nly1)/(float)(ny2-ny1):0;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	float pleft,pright,lpxleft,lpyleft,lpxright,lpyright,
	  xleft=(float)nx0,xright=xleft,lxleft=(float)nlx0,lyleft=(float)nly0,lxright=lxleft,lyright=lyleft;
	if (p1<p2) { pleft=p1; pright=p2; lpxleft=lpx1; lpyleft=lpy1; lpxright=lpx2; lpyright=lpy2; } 
	else       { pleft=p2; pright=p1; lpxleft=lpx2; lpyleft=lpy2; lpxright=lpx1; lpyright=lpy1; }
	if (ny0<0) { xleft-=ny0*pleft; xright-=ny0*pright; lxleft-=ny0*lpxleft; lyleft-=ny0*lpyleft;
		lxright-=ny0*lpxright; lyright-=ny0*lpyright; }
	const int ya = ny1<dimy()?ny1:height;
	for (int y=(ny0<0?0:ny0); y<ya; y++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		lpx = dx?((int)lxright-(int)lxleft)/(float)dx:0,
		lpy = dx?((int)lyright-(int)lyleft)/(float)dx:0,        
		lxi = (float)((xleft>=0)?(int)lxleft:(int)(lxleft-(int)xleft*lpx)),
		lyi = (float)((xleft>=0)?(int)lyleft:(int)(lyleft-(int)xleft*lpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  float lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(light((unsigned int)lx,(unsigned int)ly)*color[k]); lx+=lpx; ly+=lpy; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  float lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*light((unsigned int)lx,(unsigned int)ly)*color[k]+copacity*(*ptrd)); ptrd++; lx+=lpx; ly+=lpy; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; lxleft+=lpxleft; lyleft+=lpyleft; lxright+=lpxright; lyright+=lpyright;
	}
	
	if (p1<p2) {
	  xleft=(float)nx1; pleft=p3; lxleft=(float)nlx1; lyleft=(float)nly1; lpxleft=lpx3; lpyleft=lpy3;
	  if (ny1<0) { xleft-=ny1*pleft; lxleft-=ny1*lpxleft; lyleft-=ny1*lpyleft; }
	} else { 
	  xright=(float)nx1; pright=p3; lxright=(float)nlx1; lyright=(float)nly1; lpxright=lpx3; lpyright=lpy3;
	  if (ny1<0) { xright-=ny1*pright; lxright-=ny1*lpxright; lyright-=ny1*lpyright; }
	}    
	const int yb = ny2>=dimy()?(height-1):ny2;
	for (int yy=(ny1<0?0:ny1); yy<=yb; yy++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		lpx = dx?((int)lxright-(int)lxleft)/(float)dx:0,
		lpy = dx?((int)lyright-(int)lyleft)/(float)dx:0,        
		lxi = (float)((xleft>=0)?(int)lxleft:(int)(lxleft-(int)xleft*lpx)),
		lyi = (float)((xleft>=0)?(int)lyleft:(int)(lyleft-(int)xleft*lpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,yy,0,0);
		if (opacity>=1) cimg_mapV(*this,k) { 
		  float lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(light((unsigned int)lx,(unsigned int)ly)*color[k]); lx+=lpx; ly+=lpy; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) { 
		  float lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*light((unsigned int)lx,(unsigned int)ly)*color[k]+copacity*(*ptrd)); ptrd++; lx+=lpx; ly+=lpy; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; lxleft+=lpxleft; lyleft+=lpyleft; lxright+=lpxright; lyright+=lpyright;
	}
	  }
	  return *this;
	}
		
	//! Draw a 2D textured triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param texture = texture image used to fill the triangle.
	   \param tx0 = X-coordinate of the first corner in the texture image.
	   \param ty0 = Y-coordinate of the first corner in the texture image.
	   \param tx1 = X-coordinate of the second corner in the texture image.
	   \param ty1 = Y-coordinate of the second corner in the texture image.
	   \param tx2 = X-coordinate of the third corner in the texture image.
	   \param ty2 = Y-coordinate of the third corner in the texture image.
	   \param opacity = opacity of the drawing.
	   \param brightness = brightness of the drawing.
	   \note Clipping is supported, but texture coordinates do not support clipping.
	**/
	template<typename t> CImg& draw_triangle(const int x0,const int y0,
											 const int x1,const int y1,
											 const int x2,const int y2,
											 const CImg<t>& texture,
											 const int tx0,const int ty0,
											 const int tx1,const int ty1,
											 const int tx2,const int ty2,
											 const float opacity=1.0f, const float brightness=1.0f) {
	  if (!is_empty()) {
	if (texture.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_triangle() : Specified texture (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),texture.width,texture.height,texture.depth,texture.dim,texture.data);
	int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2,ntx0=tx0,nty0=ty0,ntx1=tx1,nty1=ty1,ntx2=tx2,nty2=ty2,whz=width*height*depth;
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1);
	if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2,ntx0,ntx2,nty0,nty2);
	if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2,ntx1,ntx2,nty1,nty2);
	if (ny0>=dimy() || ny2<0) return *this;
	const float 
	  p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
	  p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
	  p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1),
	  tpx1 = (ny1-ny0)?(ntx1-ntx0)/(float)(ny1-ny0):0,
	  tpy1 = (ny1-ny0)?(nty1-nty0)/(float)(ny1-ny0):0,
	  tpx2 = (ny2-ny0)?(ntx2-ntx0)/(float)(ny2-ny0):0,
	  tpy2 = (ny2-ny0)?(nty2-nty0)/(float)(ny2-ny0):0,
	  tpx3 = (ny2-ny1)?(ntx2-ntx1)/(float)(ny2-ny1):0,
	  tpy3 = (ny2-ny1)?(nty2-nty1)/(float)(ny2-ny1):0;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	float pleft,pright,tpxleft,tpyleft,tpxright,tpyright,
	  xleft=(float)nx0,xright=xleft,txleft=(float)ntx0,tyleft=(float)nty0,txright=txleft,tyright=tyleft;
	if (p1<p2) { pleft=p1; pright=p2; tpxleft=tpx1; tpyleft=tpy1; tpxright=tpx2; tpyright=tpy2; } 
	else       { pleft=p2; pright=p1; tpxleft=tpx2; tpyleft=tpy2; tpxright=tpx1; tpyright=tpy1; }
	if (ny0<0) { xleft-=ny0*pleft; xright-=ny0*pright; txleft-=ny0*tpxleft; tyleft-=ny0*tpyleft;
		txright-=ny0*tpxright; tyright-=ny0*tpyright; }
	const int ya = ny1<dimy()?ny1:height;
	for (int y=(ny0<0?0:ny0); y<ya; y++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,        
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(brightness*texture((unsigned int)tx,(unsigned int)ty,0,k)); tx+=tpx; ty+=tpy; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*brightness*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; tx+=tpx; ty+=tpy; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright;
	}
	
	if (p1<p2) {
	  xleft=(float)nx1; pleft=p3; txleft=(float)ntx1; tyleft=(float)nty1; tpxleft=tpx3; tpyleft=tpy3;
	  if (ny1<0) { xleft-=ny1*pleft; txleft-=ny1*tpxleft; tyleft-=ny1*tpyleft; }
	} else { 
	  xright=(float)nx1; pright=p3; txright=(float)ntx1; tyright=(float)nty1; tpxright=tpx3; tpyright=tpy3;
	  if (ny1<0) { xright-=ny1*pright; txright-=ny1*tpxright; tyright-=ny1*tpyright; }
	}    
	const int yb = ny2>=dimy()?(height-1):ny2;
	for (int yy=(ny1<0?0:ny1); yy<=yb; yy++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,        
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,yy,0,0);
		if (opacity>=1) cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(brightness*texture((unsigned int)tx,(unsigned int)ty,0,k)); tx+=tpx; ty+=tpy; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*brightness*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; tx+=tpx; ty+=tpy; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright;
	}
	  }
	  return *this;
	}

	//! Draw a 2D textured triangle with Gouraud-Shading in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param texture = texture image used to fill the triangle.
	   \param tx0 = X-coordinate of the first corner in the texture image.
	   \param ty0 = Y-coordinate of the first corner in the texture image.
	   \param tx1 = X-coordinate of the second corner in the texture image.
	   \param ty1 = Y-coordinate of the second corner in the texture image.
	   \param tx2 = X-coordinate of the third corner in the texture image.
	   \param ty2 = Y-coordinate of the third corner in the texture image.
	   \param c0 = brightness value of the first corner.
	   \param c1 = brightness value of the second corner.
	   \param c2 = brightness value of the third corner.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported, but texture coordinates do not support clipping.
	**/
	template<typename t> CImg& draw_triangle(const int x0,const int y0,
											 const int x1,const int y1,
											 const int x2,const int y2,
											 const CImg<t>& texture,
											 const int tx0,const int ty0,
											 const int tx1,const int ty1,
											 const int tx2,const int ty2,
						 const float c0,const float c1,const float c2,
											 const float opacity=1) {
	  if (!is_empty()) {
	if (texture.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_triangle() : Specified texture (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),texture.width,texture.height,texture.depth,texture.dim,texture.data);
	int nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2,ntx0=tx0,nty0=ty0,ntx1=tx1,nty1=ty1,ntx2=tx2,nty2=ty2,whz=width*height*depth;
	float nc0=c0,nc1=c1,nc2=c2;
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1,nc0,nc1);
	if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2,ntx0,ntx2,nty0,nty2,nc0,nc2);
	if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2,ntx1,ntx2,nty1,nty2,nc1,nc2);
	if (ny0>=dimy() || ny2<0) return *this;
	const float 
	  p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
	  p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
	  p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1),
	  tpx1 = (ny1-ny0)?(ntx1-ntx0)/(float)(ny1-ny0):0,
	  tpy1 = (ny1-ny0)?(nty1-nty0)/(float)(ny1-ny0):0,
	  tpx2 = (ny2-ny0)?(ntx2-ntx0)/(float)(ny2-ny0):0,
	  tpy2 = (ny2-ny0)?(nty2-nty0)/(float)(ny2-ny0):0,
	  tpx3 = (ny2-ny1)?(ntx2-ntx1)/(float)(ny2-ny1):0,
	  tpy3 = (ny2-ny1)?(nty2-nty1)/(float)(ny2-ny1):0,
	  cp1 = (ny1-ny0)?(nc1-nc0)/(float)(ny1-ny0):0,
	  cp2 = (ny2-ny0)?(nc2-nc0)/(float)(ny2-ny0):0,
	  cp3 = (ny2-ny1)?(nc2-nc1)/(float)(ny2-ny1):0;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	float pleft,pright,tpxleft,tpyleft,tpxright,tpyright,cpleft,cpright,
	  xleft=(float)nx0,xright=xleft,txleft=(float)ntx0,tyleft=(float)nty0,txright=txleft,tyright=tyleft,cleft=nc0,cright=cleft;
	if (p1<p2) { pleft=p1; pright=p2; tpxleft=tpx1; tpyleft=tpy1; tpxright=tpx2; tpyright=tpy2; cpleft=cp1; cpright=cp2; } 
	else       { pleft=p2; pright=p1; tpxleft=tpx2; tpyleft=tpy2; tpxright=tpx1; tpyright=tpy1; cpleft=cp2, cpright=cp1; }
	if (ny0<0) {
	  xleft-=ny0*pleft; xright-=ny0*pright; txleft-=ny0*tpxleft; tyleft-=ny0*tpyleft; cleft-=ny0*cpleft; 
	  txright-=ny0*tpxright; tyright-=ny0*tpyright; cright-=ny0*cpright;
	}
	const int ya = ny1<dimy()?ny1:height;
	for (int y=(ny0<0?0:ny0); y<ya; y++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,
		cp = dx?(cright-cleft)/dx:0,
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy)),
		ci = (xleft>=0)?cleft:(cleft-xleft*cp);
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi, c=ci;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(c*texture((unsigned int)tx,(unsigned int)ty,0,k)); tx+=tpx; ty+=tpy; c+=cp; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi, c=ci;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*c*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; tx+=tpx; ty+=tpy; c+=cp; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright; cleft+=cpleft; cright+=cpright;
	}
	
	if (p1<p2) {
	  xleft=(float)nx1; pleft=p3; txleft=(float)ntx1; tyleft=(float)nty1; tpxleft=tpx3; tpyleft=tpy3; cleft=nc1; cpleft=cp3;
	  if (ny1<0) { xleft-=ny1*pleft; txleft-=ny1*tpxleft; tyleft-=ny1*tpyleft; cleft-=ny1*cpleft; }
	} else { 
	  xright=(float)nx1; pright=p3; txright=(float)ntx1; tyright=(float)nty1; tpxright=tpx3; tpyright=tpy3; cright=nc1; cpright=cp3;
	  if (ny1<0) { xright-=ny1*pright; txright-=ny1*tpxright; tyright-=ny1*tpyright; cright-=ny1*cpright; }
	}    
	const int yb = ny2>=dimy()?(height-1):ny2;
	for (int yy=(ny1<0?0:ny1); yy<=yb; yy++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,        
		cp = dx?(cright-cleft)/dx:0,
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy)),
		ci = (xleft>=0)?cleft:(cleft-xleft*cp);
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,yy,0,0);
		if (opacity>=1) cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi, c=ci;
		  for (int x=xmin; x<=xmax; x++) { *(ptrd++)=(T)(c*texture((unsigned int)tx,(unsigned int)ty,0,k)); tx+=tpx; ty+=tpy; c+=cp; }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi, c=ci;
		  for (int x=xmin; x<=xmax; x++) { *ptrd=(T)(nopacity*c*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; tx+=tpx; ty+=tpy; c+=ci; }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright; txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright; cleft+=cpleft; cright+=cpright;
	}
	  }
	  return *this;
	}

	//! Draw a phong-shaded 2D textured triangle in the instance image, at coordinates (\c x0,\c y0)-(\c x1,\c y1)-(\c x2,\c y2).
	/**
	   \param x0 = X-coordinate of the first corner in the instance image.
	   \param y0 = Y-coordinate of the first corner in the instance image.
	   \param x1 = X-coordinate of the second corner in the instance image.
	   \param y1 = Y-coordinate of the second corner in the instance image.
	   \param x2 = X-coordinate of the third corner in the instance image.
	   \param y2 = Y-coordinate of the third corner in the instance image.
	   \param texture = texture image used to fill the triangle.
	   \param tx0 = X-coordinate of the first corner in the texture image.
	   \param ty0 = Y-coordinate of the first corner in the texture image.
	   \param tx1 = X-coordinate of the second corner in the texture image.
	   \param ty1 = Y-coordinate of the second corner in the texture image.
	   \param tx2 = X-coordinate of the third corner in the texture image.
	   \param ty2 = Y-coordinate of the third corner in the texture image.
	   \param light = light image.
	   \param lx0 = X-coordinate of the first corner in the light image.
	   \param ly0 = Y-coordinate of the first corner in the light image.
	   \param lx1 = X-coordinate of the second corner in the light image.
	   \param ly1 = Y-coordinate of the second corner in the light image.
	   \param lx2 = X-coordinate of the third corner in the light image.
	   \param ly2 = Y-coordinate of the third corner in the light image.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported, but texture coordinates do not support clipping.
	**/
	template<typename t, typename tl> CImg& draw_triangle(const int x0,const int y0,
							  const int x1,const int y1,
							  const int x2,const int y2,
							  const CImg<t>& texture,
							  const int tx0,const int ty0,
							  const int tx1,const int ty1,
							  const int tx2,const int ty2,
							  const CImg<tl>& light,
							  const int lx0,const int ly0,
							  const int lx1,const int ly1,
							  const int lx2,const int ly2,
							  const float opacity=1.0f) {
	  if (!is_empty()) {
	if (texture.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_triangle() : Specified texture (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),texture.width,texture.height,texture.depth,texture.dim,texture.data);
	if (light.is_empty())
	  throw CImgArgumentException("CImg<%s>::draw_triangle() : Specified light (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),light.width,light.height,light.depth,light.dim,light.data);
	int 
	  nx0=x0,ny0=y0,nx1=x1,ny1=y1,nx2=x2,ny2=y2,
	  ntx0=tx0,nty0=ty0,ntx1=tx1,nty1=ty1,ntx2=tx2,nty2=ty2,
	  nlx0=lx0,nly0=ly0,nlx1=lx1,nly1=ly1,nlx2=lx2,nly2=ly2,
	  whz=width*height*depth;
	if (ny0>ny1) cimg::swap(nx0,nx1,ny0,ny1,ntx0,ntx1,nty0,nty1,nlx0,nlx1,nly0,nly1);
	if (ny0>ny2) cimg::swap(nx0,nx2,ny0,ny2,ntx0,ntx2,nty0,nty2,nlx0,nlx2,nly0,nly2);
	if (ny1>ny2) cimg::swap(nx1,nx2,ny1,ny2,ntx1,ntx2,nty1,nty2,nlx1,nlx2,nly1,nly2);
	if (ny0>=dimy() || ny2<0) return *this;
	const float 
	  p1 = (ny1-ny0)?(nx1-nx0)/(float)(ny1-ny0):(nx1-nx0),
	  p2 = (ny2-ny0)?(nx2-nx0)/(float)(ny2-ny0):(nx2-nx0),
	  p3 = (ny2-ny1)?(nx2-nx1)/(float)(ny2-ny1):(nx2-nx1),
	  tpx1 = (ny1-ny0)?(ntx1-ntx0)/(float)(ny1-ny0):0,
	  tpy1 = (ny1-ny0)?(nty1-nty0)/(float)(ny1-ny0):0,
	  tpx2 = (ny2-ny0)?(ntx2-ntx0)/(float)(ny2-ny0):0,
	  tpy2 = (ny2-ny0)?(nty2-nty0)/(float)(ny2-ny0):0,
	  tpx3 = (ny2-ny1)?(ntx2-ntx1)/(float)(ny2-ny1):0,
	  tpy3 = (ny2-ny1)?(nty2-nty1)/(float)(ny2-ny1):0,
	  lpx1 = (ny1-ny0)?(nlx1-nlx0)/(float)(ny1-ny0):0,
	  lpy1 = (ny1-ny0)?(nly1-nly0)/(float)(ny1-ny0):0,
	  lpx2 = (ny2-ny0)?(nlx2-nlx0)/(float)(ny2-ny0):0,
	  lpy2 = (ny2-ny0)?(nly2-nly0)/(float)(ny2-ny0):0,
	  lpx3 = (ny2-ny1)?(nlx2-nlx1)/(float)(ny2-ny1):0,
	  lpy3 = (ny2-ny1)?(nly2-nly1)/(float)(ny2-ny1):0;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	float pleft,pright,tpxleft,tpyleft,tpxright,tpyright,lpxleft,lpyleft,lpxright,lpyright,
	  xleft=(float)nx0,xright=xleft,
	  txleft=(float)ntx0,tyleft=(float)nty0,txright=txleft,tyright=tyleft,
	  lxleft=(float)nlx0,lyleft=(float)nly0,lxright=lxleft,lyright=lyleft;
	if (p1<p2) { 
	  pleft=p1; pright=p2;
	  tpxleft=tpx1; tpyleft=tpy1; tpxright=tpx2; tpyright=tpy2; 
	  lpxleft=lpx1; lpyleft=lpy1; lpxright=lpx2; lpyright=lpy2; 
	} else { 
	  pleft=p2; pright=p1; 
	  tpxleft=tpx2; tpyleft=tpy2; tpxright=tpx1; tpyright=tpy1; 
	  lpxleft=tpx2; lpyleft=tpy2; lpxright=tpx1; lpyright=tpy1; 
	}
	if (ny0<0) {
	  xleft-=ny0*pleft; xright-=ny0*pright;
	  txleft-=ny0*tpxleft; tyleft-=ny0*tpyleft; txright-=ny0*tpxright; tyright-=ny0*tpyright; 
	  lxleft-=ny0*lpxleft; lyleft-=ny0*lpyleft; lxright-=ny0*lpxright; lyright-=ny0*lpyright; 
	}
	const int ya = ny1<dimy()?ny1:height;
	for (int y=(ny0<0?0:ny0); y<ya; y++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,        
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy)),
		lpx = dx?((int)lxright-(int)lxleft)/(float)dx:0,
		lpy = dx?((int)lyright-(int)lyleft)/(float)dx:0,        
		lxi = (float)((xleft>=0)?(int)lxleft:(int)(lxleft-(int)xleft*lpx)),
		lyi = (float)((xleft>=0)?(int)lyleft:(int)(lyleft-(int)xleft*lpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi, lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { 
		*(ptrd++)=(T)(light((unsigned int)lx,(unsigned int)ly)*texture((unsigned int)tx,(unsigned int)ty,0,k));
		tx+=tpx; ty+=tpy; lx+=lpx; ly+=lpy; 
		  }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) {
		  float tx=txi, ty=tyi, lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) {
		*ptrd=(T)(nopacity*light((unsigned int)lx,(unsigned int)ly)*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; 
		tx+=tpx; ty+=tpy; lx+=lpx; ly+=lpy; 
		  }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright;
	  txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright;
	  lxleft+=lpxleft; lyleft+=lpyleft; lxright+=lpxright; lyright+=lpyright;
	}
	
	if (p1<p2) {
	  xleft=(float)nx1; pleft=p3; 
	  txleft=(float)ntx1; tyleft=(float)nty1; tpxleft=tpx3; tpyleft=tpy3;
	  lxleft=(float)nlx1; lyleft=(float)nly1; lpxleft=lpx3; lpyleft=lpy3;
	  if (ny1<0) { xleft-=ny1*pleft; txleft-=ny1*tpxleft; tyleft-=ny1*tpyleft; lxleft-=ny1*lpxleft; lyleft-=ny1*lpyleft; }
	} else { 
	  xright=(float)nx1; pright=p3; 
	  txright=(float)ntx1; tyright=(float)nty1; tpxright=tpx3; tpyright=tpy3;
	  lxright=(float)nlx1; lyright=(float)nly1; lpxright=lpx3; lpyright=lpy3;
	  if (ny1<0) { xright-=ny1*pright; txright-=ny1*tpxright; tyright-=ny1*tpyright; lxright-=ny1*lpxright; lyright-=ny1*lpyright; }
	}    
	const int yb = ny2>=dimy()?(height-1):ny2;
	for (int yy=(ny1<0?0:ny1); yy<=yb; yy++) {
	  const int dx = (int)xright-(int)xleft;
	  const float
		tpx = dx?((int)txright-(int)txleft)/(float)dx:0,
		tpy = dx?((int)tyright-(int)tyleft)/(float)dx:0,        
		txi = (float)((xleft>=0)?(int)txleft:(int)(txleft-(int)xleft*tpx)),
		tyi = (float)((xleft>=0)?(int)tyleft:(int)(tyleft-(int)xleft*tpy)),
		lpx = dx?((int)lxright-(int)lxleft)/(float)dx:0,
		lpy = dx?((int)lyright-(int)lyleft)/(float)dx:0,        
		lxi = (float)((xleft>=0)?(int)lxleft:(int)(lxleft-(int)xleft*lpx)),
		lyi = (float)((xleft>=0)?(int)lyleft:(int)(lyleft-(int)xleft*lpy));
	  const int xmin=(xleft>=0)?(int)xleft:0, xmax=(xright<dimx())?(int)xright:(width-1);
	  if (xmin<=xmax) {
		const int offx=whz-xmax+xmin-1;
		T* ptrd = ptr(xmin,yy,0,0);
		if (opacity>=1) cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi, lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) {
		*(ptrd++)=(T)(light((unsigned int)lx,(unsigned int)ly)*texture((unsigned int)tx,(unsigned int)ty,0,k));
		tx+=tpx; ty+=tpy; lx+=lpx; ly+=lpy; 
		  }
		  ptrd+=offx;
		} else cimg_mapV(*this,k) { 
		  float tx=txi, ty=tyi, lx=lxi, ly=lyi;
		  for (int x=xmin; x<=xmax; x++) { 
		*ptrd=(T)(nopacity*light((unsigned int)lx,(unsigned int)ly)*texture((unsigned int)tx,(unsigned int)ty,0,k)+copacity*(*ptrd)); ptrd++; 
		tx+=tpx; ty+=tpy; lx+=lpx; ly+=lpy; 
		  }
		  ptrd+=offx;
		}
	  }
	  xleft+=pleft; xright+=pright;
	  txleft+=tpxleft; tyleft+=tpyleft; txright+=tpxright; tyright+=tpyright;
	  lxleft+=lpxleft; lyleft+=lpyleft; lxright+=lpxright; lyright+=lpyright;
	}
	  }
	  return *this;
	}


	//! Draw an ellipse on the instance image
	/**
	   \param x0 = X-coordinate of the ellipse center.
	   \param y0 = Y-coordinate of the ellipse center.
	   \param r1 = First radius of the ellipse.
	   \param r2 = Second radius of the ellipse.
	   \param ru = X-coordinate of the orientation vector related to the first radius.
	   \param rv = Y-coordinate of the orientation vector related to the first radius.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param pattern = If zero, the ellipse is filled, else pattern is an integer whose bits describe the outline pattern.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_ellipse(const int x0,const int y0,const float r1,const float r2,const float ru,const float rv,
			   const T *const color,const unsigned int pattern=0L, const float opacity=1) {
	  if (!is_empty()) {
	draw_scanline(color,opacity);
	if (!color) throw CImgArgumentException("CImg<%s>::draw_ellipse : Specified color is (null).",pixel_type());
	unsigned int hatch=1;
	const float
	  nr1 = cimg::abs(r1), nr2 = cimg::abs(r2),
	  norm = (float)std::sqrt(ru*ru+rv*rv),
	  u = norm>0?ru/norm:1,
	  v = norm>0?rv/norm:0,
	  rmax = cimg::max(nr1,nr2),
	  l1 = (float)std::pow(rmax/(nr1>0?nr1:1e-6),2),
	  l2 = (float)std::pow(rmax/(nr2>0?nr2:1e-6),2),
	  a = l1*u*u + l2*v*v,
	  b = u*v*(l1-l2),
	  c = l1*v*v + l2*u*u;
	const int
	  yb = (int)std::sqrt(a*rmax*rmax/(a*c-b*b)),
	  ymin = (y0-yb<0)?0:(y0-yb),
	  ymax = (1+y0+yb>=dimy())?height-1:(1+y0+yb);
	int oxmin=0, oxmax=0;
	bool first_line = true;
	for (int y=ymin; y<ymax; y++) {
	  const float 
		Y = (float)(y-y0)+0.25f,
		delta = b*b*Y*Y-a*(c*Y*Y-rmax*rmax),
		sdelta = (float)((delta>0?std::sqrt(delta):0)),
		fxmin = x0-(b*Y+sdelta)/a,
		fxmax = x0-(b*Y-sdelta)/a;
	  const int xmin = (int)fxmin, xmax = (int)fxmax;
	  if (!pattern) draw_scanline(xmin,xmax,y,color,opacity);
	  else {
		if (!(~pattern) || (~pattern && pattern&hatch)) {
		  if (first_line) { draw_scanline(xmin,xmax,y,color,opacity); first_line = false; }
		  else {
		if (xmin<oxmin) draw_scanline(xmin,oxmin-1,y,color,opacity); 
		else draw_scanline(oxmin+(oxmin==xmin?0:1),xmin,y,color,opacity);
		if (xmax<oxmax) draw_scanline(xmax,oxmax-1,y,color,opacity); 
		else draw_scanline(oxmax+(oxmax==xmax?0:1),xmax,y,color,opacity);	
		  }
		}
	  }
	  oxmin = xmin; oxmax = xmax;
	  if (pattern) hatch=(hatch<<1)+(hatch>>(sizeof(unsigned int)*8-1));
	}
	  }
	  return *this;
	}
	
	//! Draw an ellipse on the instance image
	/**
	   \param x0 = X-coordinate of the ellipse center.
	   \param y0 = Y-coordinate of the ellipse center.
	   \param tensor = Diffusion tensor describing the ellipse.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param pattern = If zero, the ellipse is filled, else pattern is an integer whose bits describe the outline pattern.
	   \param opacity = opacity of the drawing.
	**/
	template<typename t> CImg& draw_ellipse(const int x0,const int y0,const CImg<t> &tensor,
						const T *color,const unsigned int pattern=0L,const float opacity=1) {
	  CImgl<t> eig = tensor.get_symmetric_eigen();
	  const CImg<t> &val = eig[0], &vec = eig[1];
	  return draw_ellipse(x0,y0,val(0),val(1),vec(0,0),vec(0,1),color,pattern,opacity);
	}
	
	//! Draw a circle on the instance image
	/**
	   \param x0 = X-coordinate of the circle center.
	   \param y0 = Y-coordinate of the circle center.
	   \param r = radius of the circle.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param pattern = If zero, the circle is filled, else pattern is an integer whose bits describe the outline pattern.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_circle(const int x0,const int y0,float r,const T *const color,const unsigned int pattern=0L,const float opacity=1) {
	  return draw_ellipse(x0,y0,r,r,1,0,color,pattern,opacity);
	}
  
	//! Draw a text into the instance image.
	/**
	   \param text = a C-string containing the text to display.
	   \param x0 = X-coordinate of the text in the instance image.
	   \param y0 = Y-coordinate of the text in the instance image.
	   \param fgcolor = an array of dimv() values of type \c T, defining the foreground color (0 means 'transparent').
	   \param bgcolor = an array of dimv() values of type \c T, defining the background color (0 means 'transparent').
	   \param font = List of font characters used for the drawing.
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	   \see get_font().
	**/
	template<typename t> CImg& draw_text(const char *const text,
										 const int x0,const int y0,
										 const T *const fgcolor,const T *const bgcolor,
										 const CImgl<t>& font,const float opacity=1) {
	  if (!text)
	throw CImgArgumentException("CImg<%s>::draw_text() : Specified input string is (null).",pixel_type());
	  if (font.is_empty())
	throw CImgArgumentException("CImg<%s>::draw_text() : Specified font (%u,%p) is empty.",
					pixel_type(),font.size,font.data);

	  if (is_empty()) {
	// If needed, pre-compute needed size of the image
	int x=0, y=0, w=0;
	for (int i=0; i<cimg::strlen(text); i++) {
	  const unsigned char c = text[i];
	  switch (c) {
	  case '\n': y+=font[' '].height; if (x>w) w=x; x=0; break;
	  case '\t': x+=4*font[' '].width; break;
	  default: if (c<font.size) x+=font[c].width;
	  }
	}	
	if (x!=0) {
	  if (x>w) w=x;
	  y+=font[' '].height;
	}
	assign(x0+w,y0+y,1,font[' '].dim,0);
	if (bgcolor) cimg_mapV(*this,k) get_shared_channel(k).fill(bgcolor[k]);
	  }
	  
	  int x = x0, y = y0;
	  CImg<T> letter;
	  for (int i=0; i<cimg::strlen(text); i++) {
		const unsigned char c = text[i];
		switch (c) {
		case '\n': y+=font[' '].height; x=x0; break;
		case '\t': x+=4*font[' '].width; break;
		default: if (c<font.size) {
	  letter = font[c];
	  const CImg& mask = (c+256)<(int)font.size?font[c+256]:font[c];
	  if (fgcolor) for (unsigned int p=0; p<letter.width*letter.height; p++) 
		if (mask(p)) cimg_mapV(*this,k) letter(p,0,0,k) = (T)(letter(p,0,0,k)*fgcolor[k]);
	  if (bgcolor) for (unsigned int p=0; p<letter.width*letter.height; p++)
		if (!mask(p)) cimg_mapV(*this,k) letter(p,0,0,k) = bgcolor[k];
	  if (!bgcolor && font.size>=512) draw_image(letter,mask,x,y,0,0,(T)1,opacity); 
	  else draw_image(letter,x,y,0,0,opacity);
	  x+=letter.width;
	}
		  break;
		}
	  }
	  return *this;
	}

	//! Draw a text into the instance image.
	/**
	   \param text = a C-string containing the text to display.
	   \param x0 = X-coordinate of the text in the instance image.
	   \param y0 = Y-coordinate of the text in the instance image.
	   \param fgcolor = an array of dimv() values of type \c T, defining the foreground color (0 means 'transparent').
	   \param bgcolor = an array of dimv() values of type \c T, defining the background color (0 means 'transparent').
	   \param font_size = Height of the desired font (11,13,24,38 or 57)
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	   \see get_font().
	**/
	CImg& draw_text(const char *const text,
			const int x0,const int y0,
			const T *const fgcolor,const T *const bgcolor=0,
			const unsigned int font_size=11,const float opacity=1.0f) {
	  return draw_text(text,x0,y0,fgcolor,bgcolor,CImgl<T>::get_font(font_size),opacity);
	}
	
  
	//! Draw a text into the instance image.
	/**
	   \param x0 = X-coordinate of the text in the instance image.
	   \param y0 = Y-coordinate of the text in the instance image.
	   \param fgcolor = an array of dimv() values of type \c T, defining the foreground color (0 means 'transparent').
	   \param bgcolor = an array of dimv() values of type \c T, defining the background color (0 means 'transparent').
	   \param opacity = opacity of the drawing.
	   \param format = a 'printf'-style format, followed by arguments.
	   \note Clipping is supported.
	**/
	CImg& draw_text(const int x0,const int y0,
					const T *const fgcolor,const T *const bgcolor, const unsigned int font_size,
					const float opacity,const char *format,...) {
	  char tmp[2048]; 
	  std::va_list ap;
	  va_start(ap,format);
	  std::vsprintf(tmp,format,ap);
	  va_end(ap);
	  return draw_text(tmp,x0,y0,fgcolor,bgcolor,font_size,opacity);
	}

	template<typename t> CImg& draw_text(const int x0,const int y0,
										 const T *const fgcolor,const T *const bgcolor,
										 const CImgl<t>& font, const float opacity, const char *format,...) {
	  char tmp[2048]; 
	  std::va_list ap;
	  va_start(ap,format); 
	  std::vsprintf(tmp,format,ap);
	  va_end(ap);
	  return draw_text(tmp,x0,y0,fgcolor,bgcolor,font,opacity);
	}


	//! Draw a vector field in the instance image.
	/**
	   \param flow = a 2d image of 2d vectors used as input data.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param sampling = length (in pixels) between each arrow.
	   \param factor = length factor of each arrow (if <0, computed as a percentage of the maximum length).
	   \param quiver_type = type of plot. Can be 0 (arrows) or 1 (segments).
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	template<typename t> 
	CImg& draw_quiver(const CImg<t>& flow,const T *const color,const unsigned int sampling=25,const float factor=-20,
					  const int quiver_type=0,const float opacity=1) {
	  if (!is_empty()) {
	if (flow.is_empty() || flow.dim!=2)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified flow (%u,%u,%u,%u,%p) has wrong dimensions.",
					  pixel_type(),flow.width,flow.height,flow.depth,flow.dim,flow.data);
	if (!color) 
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified color is (null)",
					  pixel_type());
	if (sampling<=0)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Incorrect sampling value = %g",
					  pixel_type(),sampling);

	float vmax,fact;
	if (factor<=0) {
	  const CImgStats st(flow.get_norm_pointwise(2),false);
	  vmax = (float)cimg::max(cimg::abs(st.min),cimg::abs(st.max));
	  fact = -factor;
	} else { fact = factor; vmax = 1; }
	
	for (unsigned int y=sampling/2; y<height; y+=sampling)
	  for (unsigned int x=sampling/2; x<width; x+=sampling) {
		const unsigned int X = x*flow.width/width, Y = y*flow.height/height;
		float u = (float)flow(X,Y,0,0)*fact/vmax, v = (float)flow(X,Y,0,1)*fact/vmax;
		if (!quiver_type) {
		  const int xx = x+(int)u, yy = y+(int)v;
		  draw_arrow(x,y,xx,yy,color,45.0f,sampling/5.0f,~0L,opacity);
		} else draw_line((int)(x-0.5*u),(int)(y-0.5*v),(int)(x+0.5*u),(int)(y+0.5*v),color,~0L,opacity);
	  }
	  }
	  return *this; 
	}

	//! Draw a vector field in the instance image, using a colormap.
	/**
	   \param flow = a 2d image of 2d vectors used as input data.
	   \param color = a 2d image of dimv()-D vectors corresponding to the color of each arrow.
	   \param sampling = length (in pixels) between each arrow.
	   \param factor = length factor of each arrow (if <0, computed as a percentage of the maximum length).
	   \param quiver_type = type of plot. Can be 0 (arrows) or 1 (segments).
	   \param opacity = opacity of the drawing.
	   \note Clipping is supported.
	**/
	template<typename t1,typename t2>
	  CImg& draw_quiver(const CImg<t1>& flow,const CImg<t2>& color,const unsigned int sampling=25,const float factor=-20,
						const int quiver_type=0,const float opacity=1) {
	  if (!is_empty()) {
	if (flow.is_empty() || flow.dim!=2)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified flow (%u,%u,%u,%u,%p) has wrong dimensions.",
					  pixel_type(),flow.width,flow.height,flow.depth,flow.dim,flow.data);
	if (color.is_empty() || color.width!=flow.width || color.height!=flow.height)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Specified color (%u,%u,%u,%u,%p) has wrong dimensions.",
					  pixel_type(),color.width,color.height,color.depth,color.dim,color.data);
	if (sampling<=0)
	  throw CImgArgumentException("CImg<%s>::draw_quiver() : Incorrect sampling value = %g",pixel_type(),sampling);

	float vmax,fact;
	if (factor<=0) {
	  const CImgStats st(flow.get_norm_pointwise(2),false);
	  vmax = (float)cimg::max(cimg::abs(st.min),cimg::abs(st.max));
	  fact = -factor;
	} else { fact = factor; vmax = 1; }
	
	for (unsigned int y=sampling/2; y<height; y+=sampling)
	  for (unsigned int x=sampling/2; x<width; x+=sampling) {
		const unsigned int X = x*flow.width/width, Y = y*flow.height/height;
		float u = (float)flow(X,Y,0,0)*fact/vmax, v = (float)flow(X,Y,0,1)*fact/vmax;
		if (!quiver_type) {
		  const int xx = x+(int)u, yy = y+(int)v;
		  draw_arrow(x,y,xx,yy,color.get_vector_at(X,Y).data,45,sampling/5,~0L,opacity);
		} else draw_line((int)(x-0.5*u),(int)(y-0.5*v),(int)(x+0.5*u),(int)(y+0.5*v),color(X,Y),~0L,opacity);
	  }
	  }
	  return *this; 
	}

	//! Draw a 1D graph on the instance image.
	/**
	   \param data = an image containing the graph values I = f(x).
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param gtype = define the type of the plot :
					  - 0 = Plot using linear interpolation (segments).
			  - 1 = Plot with bars.
			  - 2 = Plot using cubic interpolation (3-polynomials).
	   \param ymin = lower bound of the y-range.
	   \param ymax = upper bound of the y-range.
	   \param opacity = opacity of the drawing.
	   \note
		 - if \c ymin==ymax==0, the y-range is computed automatically from the input sample.
	   \see draw_axe().
	**/
	template<typename t>
	CImg& draw_graph(const CImg<t>& data,const T *const color,const unsigned int gtype=0,
					 const double ymin=0,const double ymax=0,const float opacity=1) {
	  if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_graph() : Specified color is (null)",pixel_type());
	T *color1 = new T[dim], *color2 = new T[dim];
	cimg_mapV(*this,k) { color1[k]=(T)(color[k]*0.6f); color2[k]=(T)(color[k]*0.3f); }
	CImgStats st;
	if (ymin==ymax) { st = CImgStats(data,false); cimg::swap(st.min,st.max); } else { st.min = ymin; st.max = ymax; }
	if (st.min==st.max) { st.min--; st.max++; }
	const float ca = height>1?(float)(st.max-st.min)/(height-1):0, cb = (float)st.min;
	const int Y0 = (int)(-cb/ca);
	int pY=0;
	cimg_mapoff(data,off) {     
	  const int Y = (int)((data[off]-cb)/ca);
	  switch (gtype) {
	  case 0: // plot with segments
		if (off>0) draw_line((int)((off-1)*width/data.size()),pY,(int)(off*width/data.size()),Y,color,~0L,opacity);
		break;
	  case 1: { // plot with bars
		const unsigned int X = off*width/data.size(), nX = (off+1)*width/data.size()-1;
		draw_rectangle(X,(int)Y0,nX,Y,color1,opacity);
		draw_line(X,Y,X,(int)Y0,color2,~0L,opacity);
		draw_line(X,(int)Y0,nX,(int)Y0,Y<=Y0?color2:color,~0L,opacity);
		draw_line(nX,Y,nX,(int)Y0,color,~0L,opacity);
		draw_line(X,Y,nX,Y,Y<=Y0?color:color2,~0L,opacity);
	  } break;
	  }        
	  pY=Y;
	}
	if (gtype==2) { // plot with cubic interpolation
	  const CImg<t> ndata = data.get_shared_points(0,data.size()-1);
	  cimg_mapX(*this,x) {
		const int Y = (int)((ndata.cubic_pix1d((float)x*ndata.width/width)-cb)/ca);
		if (x>0) draw_line(x,pY,x+1,Y,color,~0L,opacity);
		pY=Y;
	  }
	}
	delete[] color1; delete[] color2;
	  }
	  return *this;     
	}

	//! Draw a labelled horizontal axis on the instance image.
	/** 
	   \param x0 = lower bound of the x-range.
	   \param x1 = upper bound of the x-range.
	   \param y = Y-coordinate of the horizontal axis in the instance image.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param precision = precision of the labels.
	   \param opacity = opacity of the drawing.
	   \note if \c precision==0, precision of the labels is automatically computed.
	   \see draw_graph().
	**/
	template<typename t> CImg& draw_axe(const CImg<t>& xvalues, const int y, const T *const color,
					const int precision=-1, const float opacity=1.0f) {
	  if (!is_empty()) {	
	int siz = (int)xvalues.size()-1;
	if (siz<=0) draw_line(0,y,width-1,y,color,~0L,opacity);
	else {
	  if (xvalues[0]<xvalues[siz]) draw_arrow(0,y,width-1,y,color,30,5,~0L,opacity);
	  else draw_arrow(width-1,y,0,y,color,30,5,~0L,opacity);
	  const int yt = (y+14)<dimy()?(y+3):(y-14);
	  char txt[32];
	  cimg_mapoff(xvalues,x) {
		if (precision>=0) std::sprintf(txt,"%.*g",precision,(double)xvalues(x));
		else std::sprintf(txt,"%g",(double)xvalues(x));
		const int xi=(int)(x*(width-1)/siz), xt = xi-(int)std::strlen(txt)*3;
		draw_point(xi,y-1,color,opacity).draw_point(xi,y+1,color,opacity).
		  draw_text(txt,xt<0?0:xt,yt,color,0,11,opacity);
	  }
	}
	  }
	  return *this;
	}

	template<typename t> CImg& draw_axe(const int x, const CImg<t>& yvalues, const T *const color, 
					const int precision=-1, const float opacity=1.0f) {
	  if (!is_empty()) {
	int siz = (int)yvalues.size()-1;
	if (siz<=0) draw_line(x,0,x,height-1,color,~0L,opacity);
	else {
	  if (yvalues[0]<yvalues[siz]) draw_arrow(x,0,x,height-1,color,30,5,~0L,opacity);
	  else draw_arrow(x,height-1,x,0,color,30,5,~0L,opacity);
	  char txt[32];
	  cimg_mapoff(yvalues,y) {
		if (precision>=0) std::sprintf(txt,"%.*g",precision,(double)yvalues(y));
		else std::sprintf(txt,"%g",(double)yvalues(y));
		const int
		  yi = (int)(y*(height-1)/siz),
		  tmp = yi-5,
		  nyi = tmp<0?0:(tmp>=(int)height-11?(int)height-11:tmp),
		  xt = x-(int)std::strlen(txt)*7;
		draw_point(x-1,yi,color,opacity).draw_point(x+1,yi,color,opacity);
		if (xt>0) draw_text(txt,xt,nyi,color,0,11,opacity);
		else draw_text(txt,x+3,nyi,color,0,11,opacity);
	  }
	}
	  }
	  return *this;
	}
	
	template<typename tx, typename ty> CImg& draw_axe(const CImg<tx>& xvalues, const CImg<ty>& yvalues, const T *const color,
							  const int precisionx=-1, const int precisiony=-1, const float opacity=1.0f) {
	  if (!is_empty()) {
	const CImg<tx> nxvalues(xvalues.data,xvalues.size(),1,1,1,true);
	const int sizx = (int)xvalues.size()-1, wm1 = (int)(width)-1;
	if (sizx>0) {
	  float ox = (float)nxvalues[0];
	  for (unsigned int x=1; x<width; x++) {	   
		const float nx = nxvalues.linear_pix1d((float)x*sizx/wm1);
		if (nx*ox<=0) { draw_axe(nx==0?x:x-1,yvalues,color,precisiony,opacity); break; }
		ox = nx;
	  }
	}
	const CImg<ty> nyvalues(yvalues.data,yvalues.size(),1,1,1,true);		
	const int sizy = (int)yvalues.size()-1, hm1 = (int)(height)-1;
	if (sizy>0) {
	  float oy = (float)nyvalues[0];
	  for (unsigned int y=1; y<height; y++) {
		const float ny = nyvalues.linear_pix1d((float)y*sizy/hm1);
		if (ny*oy<=0) { draw_axe(xvalues,ny==0?y:y-1,color,precisionx,opacity); break; }
		oy = ny;
	  }
	}
	  }
	  return *this;
	}

	template<typename tx, typename ty> CImg& draw_axe(const tx& x0, const tx& x1, const ty& y0, const ty& y1,
							  const T *const color,
							  const int subdivisionx=-60, const int subdivisiony=-60,
							  const int precisionx=-1, const int precisiony=-1,
							  const float opacity=1.0f) {
	  return draw_axe(CImg<tx>::sequence(subdivisionx>0?subdivisionx:-(int)width/subdivisionx,x0,x1),
			  CImg<ty>::sequence(subdivisiony>0?subdivisiony:-(int)height/subdivisiony,y0,y1),
			  color,precisionx,precisiony,opacity);
	}


	// INNER CLASS used by function CImg<>::draw_fill()
	template<typename T1,typename T2> struct _draw_fill {
	  const T1 *const color;
	  const float sigma,opacity;
	  const CImg<T1> value;
	  CImg<T2> region;      

	  _draw_fill(const CImg<T1>& img,const int x,const int y,const int z,
				 const T *const pcolor,const float psigma,const float popacity):
		color(pcolor),sigma(psigma),opacity(popacity),
		value(img.get_vector_at(x,y,z)), region(CImg<T2>(img.width,img.height,img.depth,1,(T2)false)) {
	  }
	  
	  _draw_fill& operator=(const _draw_fill& d) {
	color = d.color;
	sigma = d.sigma;
	opacity = d.opacity;
	value = d.value;
	region = d.region;
	return *this;
	  }
	  
	  bool comp(const CImg<T1>& A,const CImg<T1>& B) const {
		bool res=true;
		const T *pA=A.data+A.size();
		for (const T *pB=B.data+B.size(); res && pA>A.data; res=(cimg::abs(*(--pA)-(*(--pB)))<=sigma) );
		return res;
	  }

	  void fill(CImg<T1>& img,const int x,const int y,const int z) {
		if (x<0 || x>=img.dimx() || y<0 || y>=img.dimy() || z<0 || z>=img.dimz()) return;
		if (!region(x,y,z) && comp(value,img.get_vector_at(x,y,z))) {
		  const T *col=color;
		  const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
		  int xmin,xmax;
		  if (opacity>=1) cimg_mapV(img,k) img(x,y,z,k)=*(col++);
		  else cimg_mapV(img,k) img(x,y,z,k)=(T1)(*(col++)*opacity+copacity*img(x,y,z,k));
		  col-=img.dim;
		  region(x,y,z) = (T2)true;
		  for (xmin=x-1; xmin>=0 && comp(value,img.get_vector_at(xmin,y,z)); xmin--) {
			if (opacity>=1) cimg_mapV(img,k) img(xmin,y,z,k) = *(col++);
			else cimg_mapV(img,k) img(xmin,y,z,k)=(T1)(*(col++)*nopacity+copacity*img(xmin,y,z,k)); 
			col-=img.dim;
			region(xmin,y,z)=(T2)true;
		  }
		  for (xmax=x+1; xmax<img.dimx() && comp(value,img.get_vector_at(xmax,y,z)); xmax++) {
			if (opacity>=1) cimg_mapV(img,k) img(xmax,y,z,k) = *(col++);
			else cimg_mapV(img,k) img(xmax,y,z,k)=(T1)(*(col++)*nopacity+copacity*img(xmax,y,z,k));
			col-=img.dim;
			region(xmax,y,z)=(T2)true; 
		  }
		  xmin++; xmax--;
		  for (; xmin<=xmax; xmin++) { 
			fill(img,xmin,y-1,z); 
			fill(img,xmin,y+1,z);
			fill(img,xmin,y,z-1); 
			fill(img,xmin,y,z+1);
		  }
		}
	  }        
	};

	//! Draw a 3D filled region starting from a point (\c x,\c y,\ z) in the instance image.
	/**
	   \param x = X-coordinate of the starting point of the region to fill.
	   \param y = Y-coordinate of the starting point of the region to fill.
	   \param z = Z-coordinate of the starting point of the region to fill.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param region = image that will contain the mask of the filled region mask, as an output.
	   \param sigma = tolerance concerning neighborhood values.
	   \param opacity = opacity of the drawing.

	   \return \p region is initialized with the binary mask of the filled region.
	**/
	template<typename t> CImg& draw_fill(const int x,const int y,const int z,
										 const T *const color, CImg<t>& region,const float sigma=0,
										 const float opacity=1) {
	  _draw_fill<T,t> F(*this,x,y,z,color,sigma,opacity);
	  F.fill(*this,x,y,z);
	  region = F.region;
	  return *this;
	}

	//! Draw a 3D filled region starting from a point (\c x,\c y,\ z) in the instance image.
	/**
	   \param x = X-coordinate of the starting point of the region to fill.
	   \param y = Y-coordinate of the starting point of the region to fill.
	   \param z = Z-coordinate of the starting point of the region to fill.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param sigma = tolerance concerning neighborhood values.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_fill(const int x,const int y,const int z,const T *const color,const float sigma=0,const float opacity=1) {
	  CImg<bool> tmp;
	  return draw_fill(x,y,z,color,tmp,sigma,opacity);
	}

	//! Draw a 2D filled region starting from a point (\c x,\c y) in the instance image.
	/**
	   \param x = X-coordinate of the starting point of the region to fill.
	   \param y = Y-coordinate of the starting point of the region to fill.
	   \param color = an array of dimv() values of type \c T, defining the drawing color.
	   \param sigma = tolerance concerning neighborhood values.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_fill(const int x,const int y,const T *const color,const float sigma=0,const float opacity=1) {      
	  CImg<bool> tmp;
	  return draw_fill(x,y,0,color,tmp,sigma,opacity);
	}

	//! Draw a plasma square in the instance image.
	/**
	   \param x0 = X-coordinate of the upper-left corner of the plasma.
	   \param y0 = Y-coordinate of the upper-left corner of the plasma.
	   \param x1 = X-coordinate of the lower-right corner of the plasma.
	   \param y1 = Y-coordinate of the lower-right corner of the plasma.
	   \param alpha = Alpha-parameter of the plasma.
	   \param beta = Beta-parameter of the plasma.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_plasma(const int x0, const int y0, const int x1, const int y1,
					  const double alpha=1.0, const double beta=1.0, const float opacity=1) {
	  if (!is_empty()) {
	int nx0=x0,nx1=x1,ny0=y0,ny1=y1;
	if (nx1<nx0) cimg::swap(nx0,nx1);
	if (ny1<ny0) cimg::swap(ny0,ny1);
	if (nx0<0) nx0=0;
	if (nx1>=dimx()) nx1=width-1;
	if (ny0<0) ny0=0;
	if (ny1>=dimy()) ny1=height-1;
	const int xc = (nx0+nx1)/2, yc = (ny0+ny1)/2, dx=(xc-nx0), dy=(yc-ny0);
	const double dc = std::sqrt((double)(dx*dx+dy*dy))*alpha + beta;
	float val = 0;
	cimg_mapV(*this,k) {
	  if (opacity>=1) {
		(*this)(xc,ny0,0,k) = (T)(0.5f*((*this)(nx0,ny0,0,k)+(*this)(nx1,ny0,0,k)));
		(*this)(xc,ny1,0,k) = (T)(0.5f*((*this)(nx0,ny1,0,k)+(*this)(nx1,ny1,0,k)));
		(*this)(nx0,yc,0,k) = (T)(0.5f*((*this)(nx0,ny0,0,k)+(*this)(nx0,ny1,0,k)));
		(*this)(nx1,yc,0,k) = (T)(0.5f*((*this)(nx1,ny0,0,k)+(*this)(nx1,ny1,0,k)));
		do {
		  val = (float)(0.25f*((*this)(nx0,ny0,0,k)+(*this)(nx1,ny0,0,k) +
				   (*this)(nx1,ny1,0,k)+(*this)(nx0,ny1,0,k)) + dc*cimg::grand());
		} while (val<(float)cimg::type<T>::min() || val>(float)cimg::type<T>::max());
		(*this)(xc,yc,0,k)  = (T)val;
	  } else {
		const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
		(*this)(xc,ny0,0,k) = (T)(0.5f*((*this)(nx0,ny0,0,k)+(*this)(nx1,ny0,0,k))*nopacity + copacity*(*this)(xc,ny0,0,k));
		(*this)(xc,ny1,0,k) = (T)(0.5f*((*this)(nx0,ny1,0,k)+(*this)(nx1,ny1,0,k))*nopacity + copacity*(*this)(xc,ny1,0,k));
		(*this)(nx0,yc,0,k) = (T)(0.5f*((*this)(nx0,ny0,0,k)+(*this)(nx0,ny1,0,k))*nopacity + copacity*(*this)(nx0,yc,0,k));
		(*this)(nx1,yc,0,k) = (T)(0.5f*((*this)(nx1,ny0,0,k)+(*this)(nx1,ny1,0,k))*nopacity + copacity*(*this)(nx1,yc,0,k));
		do {
		  val = (float)(0.25f*(((*this)(nx0,ny0,0,k)+(*this)(nx1,ny0,0,k) +
						 (*this)(nx1,ny1,0,k)+(*this)(nx0,ny1,0,k)) + dc*cimg::grand())*nopacity
				+ copacity*(*this)(xc,yc,0,k));
		} while (val<(float)cimg::type<T>::min() || val>(float)cimg::type<T>::max());
		(*this)(xc,yc,0,k)  = (T)val;
	  }
	}
	if (xc!=nx0 || yc!=ny0) { 
	  draw_plasma(nx0,ny0,xc,yc,alpha,beta,opacity);
	  draw_plasma(xc,ny0,nx1,yc,alpha,beta,opacity);
	  draw_plasma(nx0,yc,xc,ny1,alpha,beta,opacity);
	  draw_plasma(xc,yc,nx1,ny1,alpha,beta,opacity); 
	}
	  }
	  return *this;
	}

	//! Draw a plasma in the instance image.
	/**
	   \param alpha = Alpha-parameter of the plasma.
	   \param beta = Beta-parameter of the plasma.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_plasma(const double alpha=1.0,const double beta=1.0,const float opacity=1) {
	  return draw_plasma(0,0,width-1,height-1,alpha,beta,opacity);
	}
  
	//! Draw a 1D gaussian function in the instance image.
	/**
	   \param xc = X-coordinate of the gaussian center.
	   \param sigma = Standard variation of the gaussian distribution.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_gaussian(const float xc,const double sigma,const T *const color,const float opacity=1) {
	  if (!is_empty()) {
	if (!color) throw CImgArgumentException("CImg<%s>::draw_gaussian() : Specified color is (null)",pixel_type());
	const double sigma2 = 2*sigma*sigma;
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	const unsigned int whz = width*height*depth;
	const T *col = color;
	cimg_mapX(*this,x) {
	  const float dx = (x-xc);
	  const double val = std::exp( -dx*dx/sigma2 );
	  T *ptrd = ptr(x,0,0,0);
	  if (opacity>=1) cimg_mapV(*this,k) { *ptrd = (T)(val*(*col++)); ptrd+=whz; }
	  else cimg_mapV(*this,k) { *ptrd = (T)(nopacity*val*(*col++) + copacity*(*ptrd)); ptrd+=whz; } 
	  col-=dim;
	}
	  }
	  return *this;
	}

	//! Draw an anisotropic 2D gaussian function in the instance image.
	/**
	   \param xc = X-coordinate of the gaussian center.
	   \param yc = Y-coordinate of the gaussian center.       
	   \param tensor = 2x2 covariance matrix.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	**/
	template<typename t> CImg& draw_gaussian(const float xc,const float yc,const CImg<t>& tensor,
						 const T *const color,const float opacity=1) {
	  if (!is_empty()) {
	if (tensor.width!=2 || tensor.height!=2 || tensor.depth!=1 || tensor.dim!=1) 
	  throw CImgArgumentException("CImg<%s>::draw_gaussian() : Tensor parameter (%u,%u,%u,%u,%p) is not a 2x2 matrix.",
					  pixel_type(),tensor.width,tensor.height,tensor.depth,tensor.dim,tensor.data);
	if (!color) throw CImgArgumentException("CImg<%s>::draw_gaussian() : Specified color is (null)",pixel_type());
	const CImg<t> invT = tensor.get_inverse(), invT2 = (invT*invT)/(-2.0);
	const t &a=invT2(0,0), &b=2*invT2(1,0), &c=invT2(1,1);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	const unsigned int whz = width*height*depth;
	const T *col = color;
	float dy = -yc;	
	cimg_mapY(*this,y) {
	  float dx = -xc;
	  cimg_mapX(*this,x) {
		const float val = (float)std::exp(a*dx*dx + b*dx*dy + c*dy*dy);
		T *ptrd = ptr(x,y,0,0);
		if (opacity>=1) cimg_mapV(*this,k) { *ptrd = (T)(val*(*col++)); ptrd+=whz; }
		else cimg_mapV(*this,k) { *ptrd = (T)(nopacity*val*(*col++) + copacity*(*ptrd)); ptrd+=whz; }
		col-=dim;
		dx++;
	  }
	  dy++;
	}
	  }
	  return *this;
	}

	//! Draw an isotropic 2D gaussian function in the instance image
	/**
	   \param xc = X-coordinate of the gaussian center.
	   \param yc = Y-coordinate of the gaussian center.       
	   \param sigma = standard variation of the gaussian distribution.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_gaussian(const float xc,const float yc,const float sigma,const T *const color,const float opacity=1) {
	  return draw_gaussian(xc,yc,CImg<float>::diagonal(sigma,sigma),color,opacity);
	}
	
	//! Draw an anisotropic 3D gaussian function in the instance image.
	/**
	   \param xc = X-coordinate of the gaussian center.
	   \param yc = Y-coordinate of the gaussian center.
	   \param zc = Z-coordinate of the gaussian center.
	   \param tensor = 3x3 covariance matrix.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	**/
	template<typename t> CImg& draw_gaussian(const float xc,const float yc,const float zc,const CImg<t>& tensor,
						 const T *const color,const float opacity=1) {
	  if (!is_empty()) {
	if (tensor.width!=3 || tensor.height!=3 || tensor.depth!=1 || tensor.dim!=1)
	  throw CImgArgumentException("CImg<%s>::draw_gaussian() : Tensor parameter (%u,%u,%u,%u,%p) is not a 3x3 matrix.",
					  pixel_type(),tensor.width,tensor.height,tensor.depth,tensor.dim,tensor.data);
	const CImg<t> invT = tensor.get_inverse(), invT2 = (invT*invT)/(-2.0);
	const t a=invT(0,0), b=2*invT(1,0), c=2*invT(2,0), d=invT(1,1), e=2*invT(2,1), f=invT(2,2);
	const float nopacity = cimg::abs(opacity), copacity = 1-cimg::max(opacity,0.0f);
	const unsigned int whz = width*height*depth; 
	const T *col = color;
	cimg_mapXYZ(*this,x,y,z) {
	  const float dx = (x-xc), dy = (y-yc), dz = (z-zc);
	  const double val = std::exp(a*dx*dx + b*dx*dy + c*dx*dz + d*dy*dy + e*dy*dz + f*dz*dz);
	  T *ptrd = ptr(x,y,z,0);
	  if (opacity>=1) cimg_mapV(*this,k) { *ptrd = (T)(val*(*col++)); ptrd+=whz; }
	  else cimg_mapV(*this,k) { *ptrd = (T)(nopacity*val*(*col++) + copacity*(*ptrd)); ptrd+=whz; }
	  col-=dim;
	}
	  }
	  return *this;
	}
	
	//! Draw an isotropic 3D gaussian function in the instance image
   /**
	   \param xc = X-coordinate of the gaussian center.
	   \param yc = Y-coordinate of the gaussian center.
	   \param zc = Z-coordinate of the gaussian center.
	   \param sigma = standard variation of the gaussian distribution.
	   \param color = array of dimv() values of type \c T, defining the drawing color.
	   \param opacity = opacity of the drawing.
	**/
	CImg& draw_gaussian(const float xc,const float yc,const float zc,
			const double sigma,const T *const color,const float opacity=1) {
	  return draw_gaussian(xc,yc,zc,CImg<float>::diagonal(sigma,sigma,sigma),color,opacity);
	}
	
	//! Draw a 3D object in the instance image
	/**
	   \param X = X-coordinate of the 3d object position
	   \param Y = Y-coordinate of the 3d object position
	   \param Z = Z-coordinate of the 3d object position
	   \param points = Image N*3 describing 3D point coordinates
	   \param primitives = List of P primitives
	   \param colors = List of P color (or textures)
	   \param opacities = Image of P opacities
	   \param render_type = Render type (0=Points, 1=Lines, 2=Faces (no light), 3=Faces (flat), 4=Faces(Gouraud)
	   \param double_sided = Tell if object faces have two sides or are oriented.
	   \param focale = length of the focale
	   \param lightx = X-coordinate of the light
	   \param lighty = Y-coordinate of the light
	   \param lightz = Z-coordinate of the light
	   \param ambiant_light = Brightness (between 0..1) of the ambiant light
	**/
	template<typename tp, typename tf, typename to>
	CImg& draw_object3d(const float X, const float Y, const float Z,
			const CImg<tp>& points, const CImgl<tf>& primitives,
			const CImgl<T>& colors,	const CImg<to>& opacities,
			const unsigned int render_type=4,   
			const bool double_sided=false, const float focale=500,
			const float lightx=0, const float lighty=0, const float lightz=-5000,
			const float ambiant_light = 0.05f) {
	  
	  static CImg<float> light_texture;
	  if (is_empty() || points.is_empty() || primitives.is_empty() || colors.is_empty()) return *this;
	  if (opacities.is_empty())
	return draw_object3d(X,Y,Z,points,primitives,colors,CImg<to>(primitives.size,1,1,1,(to)1),
				 render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);
	  if (points.height<3)
	return draw_object3d(X,Y,Z,points.get_resize(-100,3,1,1,0),primitives,colors,opacities,
				 render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);
	  if (colors.size<primitives.size)
	throw CImgArgumentException("CImg<%s>::draw_object3d() : Not enough defined colors (size=%u) regarding primitives (size=%u).",
					pixel_type(),colors.size,primitives.size);
	  if (opacities.width<primitives.size)
	throw CImgArgumentException("CImg<%s>::draw_object3d() : Not enough defined opacities (size=%u) regarding primitives (size=%u).",
					pixel_type(),opacities.width,primitives.size);

	  // Create light texture for phong-like rendering
	  if (render_type==5) {
	if (colors.size>primitives.size) light_texture = colors[primitives.size];
	else {
	  static float olightx=0, olighty=0, olightz=0, oambiant_light=0;
	  if (light_texture.is_empty() || lightx!=olightx || lighty!=olighty || lightz!=olightz || ambiant_light!=oambiant_light) {
		light_texture.assign(512,512);
		const float white[1]={ 1.0f },
		  dlx = lightx-X, dly = lighty-Y, dlz = lightz-Z,
		nl = (float)std::sqrt(dlx*dlx+dly*dly+dlz*dlz),
		nlx = light_texture.width/2*(1+dlx/nl),
		nly = light_texture.height/2*(1+dly/nl);
		  (light_texture.draw_gaussian(nlx,nly,light_texture.width/3.0f,white)+=ambiant_light).cut(0.0f,1.0f);
		  olightx = lightx; olighty = lighty; olightz = lightz; oambiant_light = ambiant_light;
	  }
	}
	  }
	  
	  // Compute 3D to 2D projection
	  CImg<float> projected(points.width,2);
	  cimg_mapX(points,l) {
	const float
	  x = (float)points(l,0),
	  y = (float)points(l,1),
	  z = (float)points(l,2);
	const float projectedz = z + Z + focale;
	projected(l,1) = Y + focale*y/projectedz;
	projected(l,0) = X + focale*x/projectedz;
	  }
  
	  // Compute and sort visible primitives
	  CImg<unsigned int> visibles(primitives.size);
	  CImg<float> zrange(primitives.size);
	  unsigned int nb_visibles = 0;
	  const float zmin = -focale+1.5f;
	  { cimgl_map(primitives,l) {
	const CImg<tf>& primitive = primitives[l];
	switch (primitive.size()) {
	case 1: { // Point
	  const unsigned int i0 = (unsigned int)primitive(0);
	  const float z0 = (float)(Z+points(i0,2));
	  if (z0>zmin) {
		visibles(nb_visibles) = (unsigned int)l;
		zrange(nb_visibles++) = z0;
	  }
	} break;
	case 2: // Line or textured line
	case 6: {
	  const unsigned int 
		i0 = (unsigned int)primitive(0),
		i1 = (unsigned int)primitive(1);
	  const float
		z0 = (float)(Z+points(i0,2)),
		z1 = (float)(Z+points(i1,2));
	  if (z0>zmin && z1>zmin) {
		visibles(nb_visibles) = (unsigned int)l;
		zrange(nb_visibles++) = 0.5f*(z0+z1);
	  }
	} break;
	case 3:  // Triangle or textured triangle
	case 9: {
	  const unsigned int
		i0 = (unsigned int)primitive(0), 
		i1 = (unsigned int)primitive(1),
		i2 = (unsigned int)primitive(2);
	  const float
		z0 = (float)(Z+points(i0,2)),
		z1 = (float)(Z+points(i1,2)),
		z2 = (float)(Z+points(i2,2));
	  if (z0>zmin && z1>zmin && z2>zmin) {
		const float
		  x0 = projected(i0,0), y0 = projected(i0,1),
		  x1 = projected(i1,0), y1 = projected(i1,1),
		  x2 = projected(i2,0), y2 = projected(i2,1),
		  dx1 = x1-x0, dy1 = y1-y0, dx2 = x2-x0, dy2 = y2-y0;	
		if (double_sided || dx1*dy2-dx2*dy1<0) {
		  visibles(nb_visibles) = (unsigned int)l;
		  zrange(nb_visibles++) = (z0+z1+z2)/3;
		}
	  }	      
	} break;
	case 4: // Rectangle or textured rectangle
	case 12: {
	  const unsigned int
		i0 = (unsigned int)primitive(0), 
		i1 = (unsigned int)primitive(1),
		i2 = (unsigned int)primitive(2),
		i3 = (unsigned int)primitive(3);
	  const float
		z0 = (float)(Z+points(i0,2)),
		z1 = (float)(Z+points(i1,2)),
		z2 = (float)(Z+points(i2,2)),
		z3 = (float)(Z+points(i3,2));	    
	  if (z0>zmin && z1>zmin && z2>zmin && z3>zmin) {
		const float
		  x0 = projected(i0,0), y0 = projected(i0,1),
		  x1 = projected(i1,0), y1 = projected(i1,1),
		  x2 = projected(i2,0), y2 = projected(i2,1),
		  dx1 = x1-x0, dy1 = y1-y0, dx2 = x2-x0, dy2 = y2-y0;
		if (double_sided || dx1*dy2-dx2*dy1<0) {
		  visibles(nb_visibles) = (unsigned int)l;
		  zrange(nb_visibles++) = (z0+z1+z2+z3)/4;
		}
	  }
	} break;
	default:
	  throw CImgArgumentException("CImg<%s>::draw_object3d() : Primitive %u is invalid (size = %u, can be 1,2,3,4,6,9 or 12)",
					  pixel_type(),l,primitive.size());
	}}
	  }
	  if (nb_visibles<=0) return *this;
	  CImg<unsigned int> permutations;
	  CImg<float>(zrange.data,nb_visibles,1,1,1,true).sort(permutations,false);
  
	  // Compute light properties
	  CImg<float> lightprops;
	  switch (render_type) {
	  case 3: { // Flat Shading
	lightprops.assign(nb_visibles);
	cimg_mapX(lightprops,l) {
	  const CImg<tf>& primitive = primitives(visibles(permutations(l)));
	  const unsigned int psize = primitive.size();
	  if (psize==3 || psize==4 || psize==9 || psize==12) {
		const unsigned int
		  i0 = (unsigned int)primitive(0), 
		  i1 = (unsigned int)primitive(1),
		  i2 = (unsigned int)primitive(2);
		const float
		  x0 = (float)points(i0,0), y0 = (float)points(i0,1), z0 = (float)points(i0,2),
		  x1 = (float)points(i1,0), y1 = (float)points(i1,1), z1 = (float)points(i1,2),
		  x2 = (float)points(i2,0), y2 = (float)points(i2,1), z2 = (float)points(i2,2),
		  dx1 = x1-x0, dy1 = y1-y0, dz1 = z1-z0,
		  dx2 = x2-x0, dy2 = y2-y0, dz2 = z2-z0,
		  nx = dy1*dz2-dz1*dy2,
		  ny = dz1*dx2-dx1*dz2,
		  nz = dx1*dy2-dy1*dx2,
		  norm = (float)std::sqrt(1e-5f+nx*nx+ny*ny+nz*nz),
		  lx = X+(x0+x1+x2)/3-lightx,
		  ly = Y+(y0+y1+y2)/3-lighty,
		  lz = Z+(z0+z1+z2)/3-lightz,
		  nl = (float)std::sqrt(1e-5f+lx*lx+ly*ly+lz*lz),
		  factor = (-lx*nx-ly*ny-lz*nz)/(norm*nl),
		  nfactor = double_sided?cimg::abs(factor):cimg::max(factor,0.0f);
		lightprops[l] = cimg::min(nfactor+ambiant_light,1.0f);
	  } else lightprops[l] = 1.0f;
	}
	  } break;
  
	  case 4: // Gouraud Shading
	  case 5: { // Phong-Shading
	CImg<float> points_normals(points.width,3,1,1,0);
	cimgl_map(primitives,l) {
	  const CImg<tf>& primitive = primitives[l];
	  const unsigned int psize = primitive.size();
	  const bool 
		triangle_flag = (psize==3) || (psize==9),
		rectangle_flag = (psize==4) || (psize==12);
	  if (triangle_flag || rectangle_flag) {
		const unsigned int
		  i0 = (unsigned int)primitive(0), 
		  i1 = (unsigned int)primitive(1),
		  i2 = (unsigned int)primitive(2),
		  i3 = rectangle_flag?(unsigned int)primitive(3):0;
		const float
		  x0 = (float)points(i0,0), y0 = (float)points(i0,1), z0 = (float)points(i0,2)+Z,
		  x1 = (float)points(i1,0), y1 = (float)points(i1,1), z1 = (float)points(i1,2)+Z,
		  x2 = (float)points(i2,0), y2 = (float)points(i2,1), z2 = (float)points(i2,2)+Z,
		  dx1 = x1-x0, dy1 = y1-y0, dz1 = z1-z0,
		  dx2 = x2-x0, dy2 = y2-y0, dz2 = z2-z0,
		  nx = dy1*dz2-dz1*dy2,
		  ny = dz1*dx2-dx1*dz2,
		  nz = dx1*dy2-dy1*dx2,
		  norm = (float)std::sqrt(1e-5f+nx*nx+ny*ny+nz*nz),
		  nnx = nx/norm,
		  nny = ny/norm,
		  nnz = nz/norm;
		points_normals(i0,0)+=nnx; points_normals(i0,1)+=nny; points_normals(i0,2)+=nnz;
		points_normals(i1,0)+=nnx; points_normals(i1,1)+=nny; points_normals(i1,2)+=nnz;
		points_normals(i2,0)+=nnx; points_normals(i2,1)+=nny; points_normals(i2,2)+=nnz;
		if (rectangle_flag) {
		  points_normals(i3,0)+=nnx; points_normals(i3,1)+=nny; points_normals(i3,2)+=nnz; 
		}
	  }
	}
	
	if (render_type==4) {
	  lightprops.assign(points.width);
	  cimg_mapX(points,ll) {
		const float 
		  nx = points_normals(ll,0),
		  ny = points_normals(ll,1),
		  nz = points_normals(ll,2),
		  norm = (float)std::sqrt(1e-5f+nx*nx+ny*ny+nz*nz),
		  lx = (float)(X+points(ll,0)-lightx),
		  ly = (float)(Y+points(ll,1)-lighty),
		  lz = (float)(Z+points(ll,2)-lightz),
		  nl = (float)std::sqrt(1e-5f+lx*lx+ly*ly+lz*lz),
		  factor = (-lx*nx-ly*ny-lz*nz)/(norm*nl),
		  nfactor = double_sided?cimg::abs(factor):cimg::max(factor,0.0f);
		lightprops[ll] = cimg::min(nfactor+ambiant_light,1.0f);
	  }
	} else {
	  lightprops.assign(points.width,2);
	  cimg_mapX(points,ll) {
		const float 
		  nx = points_normals(ll,0),
		  ny = points_normals(ll,1),
		  nz = points_normals(ll,2),
		  norm = (float)std::sqrt(1e-5f+nx*nx+ny*ny+nz*nz),
		  nnx = nx/norm, nny = ny/norm;
		lightprops(ll,0) = (light_texture.width/2-1)*(1+nnx);
		lightprops(ll,1) = (light_texture.height/2-1)*(1+nny);
	  }
	}
	  } break;
	  }
	  
	  // Draw visible primitives
	  { for (unsigned int l=0; l<nb_visibles; l++) {
	const unsigned int n_primitive = visibles(permutations(l));
	const CImg<tf>& primitive = primitives[n_primitive];
	const CImg<T>& color = colors[n_primitive];
	const float opacity = (float)opacities(n_primitive,0);

	switch (primitive.size()) {
	case 1: { // colored point	 
	  const unsigned int n0 = (unsigned int)primitive[0];
	  const int x0 = (int)projected(n0,0), y0 = (int)projected(n0,1);
	  draw_point(x0,y0,color.ptr(),opacity);
	} break;
	case 2: { // colored line
	  const unsigned int 
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1);
	  if (render_type) draw_line(x0,y0,x1,y1,color.ptr(),~0L,opacity);
	  else draw_point(x0,y0,color.ptr(),opacity).draw_point(x1,y1,color.ptr(),opacity);	
	} break;
	case 6: { // textured line
	  const unsigned int 
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1],
		tx0 = (unsigned int)primitive[2],
		ty0 = (unsigned int)primitive[3],
		tx1 = (unsigned int)primitive[4],
		ty1 = (unsigned int)primitive[5];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1);
	  if (render_type) draw_line(x0,y0,x1,y1,color,tx0,ty0,tx1,ty1,opacity);
	  else draw_point(x0,y0,color.get_vector_at(tx0,ty0).ptr(),opacity).
		 draw_point(x1,y1,color.get_vector_at(tx1,ty1).ptr(),opacity);	  
	} break;
	case 3: { // colored triangle
	  const unsigned int
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1],
		n2 = (unsigned int)primitive[2];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1),
		x2 = (int)projected(n2,0), y2 = (int)projected(n2,1);
	  switch(render_type) {
	  case 0:
		draw_point(x0,y0,color.ptr(),opacity).draw_point(x1,y1,color.ptr(),opacity).draw_point(x2,y2,color.ptr(),opacity);
		break;
	  case 1:
		draw_line(x0,y0,x1,y1,color.ptr(),~0L,opacity).draw_line(x0,y0,x2,y2,color.ptr(),~0L,opacity).
		  draw_line(x1,y1,x2,y2,color.ptr(),~0L,opacity);
		break;
	  case 2:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),opacity); 
		break;
	  case 3:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),opacity,lightprops(l)); 
		break;
	  case 4:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),lightprops(n0),lightprops(n1),lightprops(n2),opacity);
		break;
	  case 5:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),light_texture,
			  (unsigned int)lightprops(n0,0), (unsigned int)lightprops(n0,1),
			  (unsigned int)lightprops(n1,0), (unsigned int)lightprops(n1,1),
			  (unsigned int)lightprops(n2,0), (unsigned int)lightprops(n2,1),
			  opacity);
		break;
	  }
	} break;
	case 4: { // colored rectangle
	  const unsigned int
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1],
		n2 = (unsigned int)primitive[2],
		n3 = (unsigned int)primitive[3];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1),
		x2 = (int)projected(n2,0), y2 = (int)projected(n2,1),
		x3 = (int)projected(n3,0), y3 = (int)projected(n3,1);
	  switch(render_type) {
	  case 0:
		draw_point(x0,y0,color.ptr(),opacity).draw_point(x1,y1,color.ptr(),opacity).
		  draw_point(x2,y2,color.ptr(),opacity).draw_point(x3,y3,color.ptr(),opacity);
		break;
	  case 1:
		draw_line(x0,y0,x1,y1,color.ptr(),~0L,opacity).draw_line(x1,y1,x2,y2,color.ptr(),~0L,opacity).
		  draw_line(x2,y2,x3,y3,color.ptr(),~0L,opacity).draw_line(x3,y3,x0,y0,color.ptr(),~0L,opacity);
		break;
	  case 2:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),opacity).draw_triangle(x0,y0,x2,y2,x3,y3,color.ptr(),opacity);
		break;
	  case 3:
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),opacity,lightprops(l)).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color.ptr(),opacity,lightprops(l)); 
		break;
	  case 4: {
		const float 
		  lightprop0 = lightprops(n0), lightprop1 = lightprops(n1),
		  lightprop2 = lightprops(n2), lightprop3 = lightprops(n3);
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),lightprop0,lightprop1,lightprop2,opacity).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color.ptr(),lightprop0,lightprop2,lightprop3,opacity);
	  } break;
	  case 5: {
		const unsigned int
		  lx0 = (unsigned int)lightprops(n0,0), ly0 = (unsigned int)lightprops(n0,1),
		  lx1 = (unsigned int)lightprops(n1,0), ly1 = (unsigned int)lightprops(n1,1),
		  lx2 = (unsigned int)lightprops(n2,0), ly2 = (unsigned int)lightprops(n2,1),
		  lx3 = (unsigned int)lightprops(n3,0), ly3 = (unsigned int)lightprops(n3,1);
		draw_triangle(x0,y0,x1,y1,x2,y2,color.ptr(),light_texture,lx0,ly0,lx1,ly1,lx2,ly2,opacity).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color.ptr(),light_texture,lx0,ly0,lx2,ly2,lx3,ly3,opacity);
	  } break;
	  }
	} break;
	case 9: { // Textured triangle
	  const unsigned int
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1],
		n2 = (unsigned int)primitive[2],
		tx0 = (unsigned int)primitive[3],
		ty0 = (unsigned int)primitive[4],
		tx1 = (unsigned int)primitive[5],
		ty1 = (unsigned int)primitive[6],
		tx2 = (unsigned int)primitive[7],
		ty2 = (unsigned int)primitive[8];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1),
		x2 = (int)projected(n2,0), y2 = (int)projected(n2,1);
	  switch(render_type) {
	  case 0:
		draw_point(x0,y0,color.get_vector_at(tx0,ty0).ptr(),opacity).
		  draw_point(x1,y1,color.get_vector_at(tx1,ty1).ptr(),opacity).
		  draw_point(x2,y2,color.get_vector_at(tx2,ty2).ptr(),opacity);
		break;
	  case 1:
		draw_line(x0,y0,x1,y1,color,tx0,ty0,tx1,ty1,opacity).
		  draw_line(x0,y0,x2,y2,color,tx0,ty0,tx2,ty2,opacity).
		  draw_line(x1,y1,x2,y2,color,tx1,ty1,tx2,ty2,opacity);
		break;
	  case 2:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,opacity);
		break;
	  case 3:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,opacity,lightprops(l));
		break;
	  case 4:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,lightprops(n0),lightprops(n1),lightprops(n2),opacity);	    
		break;
	  case 5:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,light_texture,
			  (unsigned int)lightprops(n0,0), (unsigned int)lightprops(n0,1),
			  (unsigned int)lightprops(n1,0), (unsigned int)lightprops(n1,1),
			  (unsigned int)lightprops(n2,0), (unsigned int)lightprops(n2,1),
			  opacity);
		break;
	  }
	} break;
	case 12: { // Textured rectangle
	  const unsigned int
		n0 = (unsigned int)primitive[0],
		n1 = (unsigned int)primitive[1],
		n2 = (unsigned int)primitive[2],
		n3 = (unsigned int)primitive[3],
		tx0 = (unsigned int)primitive[4],
		ty0 = (unsigned int)primitive[5],
		tx1 = (unsigned int)primitive[6],
		ty1 = (unsigned int)primitive[7],
		tx2 = (unsigned int)primitive[8],
		ty2 = (unsigned int)primitive[9],
		tx3 = (unsigned int)primitive[10],
		ty3 = (unsigned int)primitive[11];
	  const int
		x0 = (int)projected(n0,0), y0 = (int)projected(n0,1),
		x1 = (int)projected(n1,0), y1 = (int)projected(n1,1),
		x2 = (int)projected(n2,0), y2 = (int)projected(n2,1),
		x3 = (int)projected(n3,0), y3 = (int)projected(n3,1);
	  switch(render_type) {
	  case 0:
		draw_point(x0,y0,color.get_vector_at(tx0,ty0).ptr(),opacity).
		  draw_point(x1,y1,color.get_vector_at(tx1,ty1).ptr(),opacity).
		  draw_point(x2,y2,color.get_vector_at(tx2,ty2).ptr(),opacity).
		  draw_point(x3,y3,color.get_vector_at(tx3,ty3).ptr(),opacity);
		break;
	  case 1:
		draw_line(x0,y0,x1,y1,color,tx0,ty0,tx1,ty1,opacity).
		  draw_line(x1,y1,x2,y2,color,tx1,ty1,tx2,ty2,opacity).
		  draw_line(x2,y2,x3,y3,color,tx2,ty2,tx3,ty3,opacity).
		  draw_line(x3,y3,x0,y0,color,tx3,ty3,tx0,ty0,opacity);
		break;
	  case 2:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,opacity).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color,tx0,ty0,tx2,ty2,tx3,ty3,opacity);
		break;
	  case 3:
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,opacity,lightprops(l)).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color,tx0,ty0,tx2,ty2,tx3,ty3,opacity,lightprops(l));
		break;
	  case 4: {
		const float
		  lightprop0 = lightprops(n0), lightprop1 = lightprops(n1), 
		  lightprop2 = lightprops(n2), lightprop3 = lightprops(n3);
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,lightprop0,lightprop1,lightprop2,opacity).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color,tx0,ty0,tx2,ty2,tx3,ty3,lightprop0,lightprop2,lightprop3,opacity);
	  } break;
	  case 5: {
		const unsigned int
		  lx0 = (unsigned int)lightprops(n0,0), ly0 = (unsigned int)lightprops(n0,1),
		  lx1 = (unsigned int)lightprops(n1,0), ly1 = (unsigned int)lightprops(n1,1),
		  lx2 = (unsigned int)lightprops(n2,0), ly2 = (unsigned int)lightprops(n2,1),
		  lx3 = (unsigned int)lightprops(n3,0), ly3 = (unsigned int)lightprops(n3,1);
		draw_triangle(x0,y0,x1,y1,x2,y2,color,tx0,ty0,tx1,ty1,tx2,ty2,light_texture,lx0,ly0,lx1,ly1,lx2,ly2,opacity).
		  draw_triangle(x0,y0,x2,y2,x3,y3,color,tx0,ty0,tx1,ty1,tx2,ty2,light_texture,lx0,ly0,lx2,ly2,lx3,ly3,opacity);
	  } break;
	  }	
	} break;	
	}
	  }
	  }
	  return *this;
	}
	
	//! Draw a 3D object in the instance image
	template<typename tp, typename tf, typename to>
	CImg& draw_object3d(const float X, const float Y, const float Z,
			const CImg<tp>& points, const CImgl<tf>& primitives,
			const CImgl<T>& colors,	const CImgl<to>& opacities,
			const unsigned int render_type=4,   
			const bool double_sided=false, const float focale=500,
			const float lightx=0, const float lighty=0, const float lightz=-5000,
			const float ambiant_light = 0.05f) {
	  if (opacities.is_empty())
	return draw_object3d(X,Y,Z,points,primitives,colors,CImg<to>(),
				 render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);     
	  CImg<to> nopacities(opacities.size);
	  to *ptrd = nopacities.ptr();
	  cimg_mapoff(nopacities,l) if (opacities(l).size()) *(ptrd++) = opacities(l,0);
	  else 
	throw CImgArgumentException("CImg<%s>::draw_object3d() : Given opacities (size=%u) contains a null element at "
					"position %u.",pixel_type(),opacities.size,l);
	  return draw_object3d(X,Y,Z,points,primitives,colors,nopacities,
			   render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);
	}

	//! Draw a 3D object in the instance image
	template<typename tp, typename tf, typename to>
	CImg& draw_object3d(const float X, const float Y, const float Z,
			const CImgl<tp>& points, const CImgl<tf>& primitives,
			const CImgl<T>& colors,	 const CImg<to>& opacities,
			const unsigned int render_type=4,   
			const bool double_sided=false, const float focale=500,
			const float lightx=0, const float lighty=0, const float lightz=-5000,
			const float ambiant_light = 0.05f) {
	  if (points.is_empty()) return *this;
	  CImg<tp> npoints(points.size,3,1,1,0);
	  tp *ptrX = npoints.ptr(), *ptrY = npoints.ptr(0,1), *ptrZ = npoints.ptr(0,2);
	  cimg_mapX(npoints,l) {
	const CImg<tp>& point = points[l];
	const unsigned int siz = point.size();
	if (!siz)
	  throw CImgArgumentException("CImg<%s>::draw_object3d() : Given points (size=%u) contains a null element at "
					  "position %u.",pixel_type(),points.size,l);
	*(ptrZ++) = (siz>2)?point(2):0;
	*(ptrY++) = (siz>1)?point(1):0;
	*(ptrX++) = point(0);
	  }
	  return draw_object3d(X,Y,Z,npoints,primitives,colors,opacities,
			   render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);
	}

	//! Draw a 3D object in the instance image
	template<typename tp, typename tf, typename to>
	CImg& draw_object3d(const float X, const float Y, const float Z,
			const CImgl<tp>& points, const CImgl<tf>& primitives,
			const CImgl<T>& colors,	 const CImgl<to>& opacities,
			const unsigned int render_type=4,   
			const bool double_sided=false, const float focale=500,
			const float lightx=0, const float lighty=0, const float lightz=-5000,
			const float ambiant_light = 0.05f) {
	  if (opacities.is_empty())
	return draw_object3d(X,Y,Z,points,primitives,colors,CImg<to>(),
				 render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);     
	  CImg<to> nopacities(opacities.size);
	  to *ptrd = nopacities.ptr();
	  cimg_mapoff(nopacities,l) if (opacities(l).size()) *(ptrd++) = opacities(l,0);
	  else 
	throw CImgArgumentException("CImg<%s>::draw_object3d() : Given opacities (size=%u) contains a null element at "
					"position %u.",pixel_type(),opacities.size,l);
	  return draw_object3d(X,Y,Z,points,primitives,colors,nopacities,
			   render_type,double_sided,focale,lightx,lighty,lightz,ambiant_light);
	}
	
	//! Draw a 3D object in the instance image
	template<typename tp, typename tf>
	CImg& draw_object3d(const float X, const float Y, const float Z,
			const tp& points, const CImgl<tf>& primitives,
			const CImgl<T>& colors,
			const unsigned int render_type=4,   
			const bool double_sided=false, const float focale=500,
			const float lightx=0, const float lighty=0, const float lightz=-5000,
			const float ambiant_light = 0.05f, const float opacity=1.0f) {
	  return draw_object3d(X,Y,Z,points,primitives,colors,
			   CImg<float>(primitives.size,1,1,1,opacity),
			   render_type,double_sided,focale,lightx,lighty,lightz,
			   ambiant_light);
	}

	//@}
	//----------------------------
	//
	//! \name Filtering functions
	//@{
	//----------------------------
  
	//! Return the correlation of the image by a mask.
	/**
	   The result \p res of the correlation of an image \p img by a mask \p mask is defined to be :
	   
	   res(x,y,z) = sum_{i,j,k} img(x+i,y+j,z+k)*mask(i,j,k)

	   \param mask = the correlation kernel.
	   \param cond = the border condition type (0=zero, 1=dirichlet)
	   \param weighted_correl = enable local normalization.
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type> 
	get_correlate(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_correl=false) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  typedef typename cimg::largest<t,float>::type fftype;
	  typedef typename cimg::largest<T,fftype>::type ftype;
	  
	  if (is_empty()) return CImg<restype>();
	  if (mask.is_empty() || mask.dim!=1)
	throw CImgArgumentException("CImg<%s>::get_correlate() : Specified mask (%u,%u,%u,%u,%p) is not scalar.",
					pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	  CImg<restype> dest(*this,false);
	  if (cond && mask.width==mask.height && ((mask.depth==1 && mask.width<=5) || (mask.depth==mask.width && mask.width<=3))) {
		// A special optimization is done for 2x2,3x3,4x4,5x5,2x2x2 and 3x3x3 mask (with cond=1)
		switch (mask.depth) {
		case 3: {
		  CImg_3x3x3(I,T);
		  if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map3x3x3(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr3x3x3(I,mask);
		  else cimg_mapZV(*this,z,v) cimg_map3x3x3(*this,x,y,z,v,I) {
			const double norm = (double)cimg_squaresum3x3x3(I);
			dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr3x3x3(I,mask)/std::sqrt(norm)):0;
		  }
		} break;
		case 2: {
		  CImg_2x2x2(I,T);
		  if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map2x2x2(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr2x2x2(I,mask);
		  else cimg_mapZV(*this,z,v) cimg_map2x2x2(*this,x,y,z,v,I) {
			const double norm = (double)cimg_squaresum2x2x2(I);
			dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr2x2x2(I,mask)/std::sqrt(norm)):0;
		  }
		} break;
		default:
		case 1:
		  switch (mask.width) {
		  case 5: {
			CImg_5x5x1(I,T);
			if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map5x5x1(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr5x5x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map5x5x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum5x5x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr5x5x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;          
		  case 4: {
			CImg_4x4x1(I,T);
			if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map4x4x1(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr4x4x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map4x4x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum4x4x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr4x4x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;              
		  case 3: {
			CImg_3x3x1(I,T);
			if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map3x3x1(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr3x3x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map3x3x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum3x3x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr3x3x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;   
		  case 2: {
			CImg_2x2x1(I,T);
			if (!weighted_correl) cimg_mapZV(*this,z,v) cimg_map2x2x1(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_corr2x2x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map2x2x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum2x2x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_corr2x2x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;  
		  case 1: dest = mask(0)*(*this); break;
		  }
		}
	  } else { 
		// Generic version for other masks      
		const int cxm=mask.width/2, cym=mask.height/2, czm=mask.depth/2, fxm=cxm-1+(mask.width%2), fym=cym-1+(mask.height%2), fzm=czm-1+(mask.depth%2);
		cimg_mapV(*this,v) 
		  if (!weighted_correl) {	// Classical correlation
			for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
			  ftype val = 0;
			  for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
				val+= (*this)(x+xm,y+ym,z+zm,v)*mask(cxm+xm,cym+ym,czm+zm,0);
			  dest(x,y,z,v)=(restype)val;
			}
			if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
				  val+= pix3d(x+xm,y+ym,z+zm,v)*mask(cxm+xm,cym+ym,czm+zm,0);
				dest(x,y,z,v)=(restype)val;
			  }
			else cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++)  for (int xm=-cxm; xm<=fxm; xm++)
				  val+= pix3d(x+xm,y+ym,z+zm,v,0)*mask(cxm+xm,cym+ym,czm+zm,0);
				dest(x,y,z,v)=(restype)val;
			  }
		  } else {	// Weighted correlation
			for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
			  ftype val = 0, norm = 0;
			  for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++) {
				const T cval = (*this)(x+xm,y+ym,z+zm,v);
				val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				norm+= cval*cval;
			  }
			  dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt((double)norm)):0;
			}
			if (cond) cimg_mapYZV(*this,y,z,v)
		 for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0, norm = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++) {
				  const T cval = pix3d(x+xm,y+ym,z+zm,v);
				  val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				  norm+=cval*cval;
				}
				dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt((double)norm)):0;
			  }
			else cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0, norm = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++) {
				  const T cval = pix3d(x+xm,y+ym,z+zm,v,0);
				  val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				  norm+= cval*cval;
				}
				dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt((double)norm)):0;
			  }
		  }
	  }
	  return dest;
	}


	//! Correlate the image by a mask
	/**
	   This is the in-place version of get_correlate.
	   \see get_correlate
	**/
	template<typename t> CImg& correlate(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_correl=false) { 
	  return get_correlate(mask,cond,weighted_correl).swap(*this); 
	}
  
	//! Return the convolution of the image by a mask
	/**
	   The result \p res of the convolution of an image \p img by a mask \p mask is defined to be :
	   
	   res(x,y,z) = sum_{i,j,k} img(x-i,y-j,z-k)*mask(i,j,k)

	   \param mask = the correlation kernel.
	   \param cond = the border condition type (0=zero, 1=dirichlet)
	   \param weighted_convol = enable local normalization.
	**/
	template<typename t> CImg<typename cimg::largest<T,t>::type>
	get_convolve(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_convol=false) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  typedef typename cimg::largest<t,float>::type fftype;
	  typedef typename cimg::largest<T,fftype>::type ftype;
	  
	  if (is_empty()) return CImg<restype>();
	  if (mask.is_empty() || mask.dim!=1)
	throw CImgArgumentException("CImg<%s>::get_convolve() : Specified mask (%u,%u,%u,%u,%p) is not scalar.",
					pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	  CImg<restype> dest(*this,false);
	  if (cond && mask.width==mask.height && ((mask.depth==1 && mask.width<=5) || (mask.depth==mask.width && mask.width<=3))) { // optimized version
		switch (mask.depth) {
		case 3: {
		  CImg_3x3x3(I,T);
		  if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map3x3x3(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_conv3x3x3(I,mask);
		  else cimg_mapZV(*this,z,v) cimg_map3x3x3(*this,x,y,z,v,I) {
			const double norm = (double)cimg_squaresum3x3x3(I);
			dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv3x3x3(I,mask)/std::sqrt(norm)):0;
		  }
		} break;
		case 2: {
		  CImg_2x2x2(I,T);
		  if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map2x2x2(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_conv2x2x2(I,mask);
		  else cimg_mapZV(*this,z,v) cimg_map2x2x2(*this,x,y,z,v,I) {
			const double norm = (double)cimg_squaresum2x2x2(I);
			dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv2x2x2(I,mask)/std::sqrt(norm)):0;
		  }
		} break;
		default:
		case 1:
		  switch (mask.width) {
		  case 5: {
			CImg_5x5x1(I,T);
			if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map5x5x1(*this,x,y,z,v,I) dest(x,y,z,v) = cimg_conv5x5x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map5x5x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum5x5x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv5x5x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;          
		  case 4: {
			CImg_4x4x1(I,T);
			if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map4x4x1(*this,x,y,z,v,I) dest(x,y,z,v) = (T)cimg_conv4x4x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map4x4x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum4x4x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv4x4x1(I,mask)/std::sqrt(norm)):0;
			}
		  } break;              
		  case 3: {
			CImg_3x3x1(I,T);
			if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map3x3x1(*this,x,y,z,v,I) dest(x,y,z,v) = (T)cimg_conv3x3x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map3x3x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum3x3x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv3x3x1(I,mask)/std::sqrt(norm)):0;
			}            
		  } break;   
		  case 2: {
			CImg_2x2x1(I,T);
			if (!weighted_convol) cimg_mapZV(*this,z,v) cimg_map2x2x1(*this,x,y,z,v,I) dest(x,y,z,v) = (T)cimg_conv2x2x1(I,mask);
			else cimg_mapZV(*this,z,v) cimg_map2x2x1(*this,x,y,z,v,I) {
			  const double norm = (double)cimg_squaresum2x2x1(I);
			  dest(x,y,z,v) = (norm!=0)?(restype)(cimg_conv2x2x1(I,mask)/std::sqrt(norm)):0;
			} 
		  } break;  
		  case 1: dest = mask(0)*(*this); break;
		  }
		}
	  } else { // generic version
		  
		const int cxm=mask.width/2, cym=mask.height/2, czm=mask.depth/2, fxm=cxm-1+(mask.width%2), fym=cym-1+(mask.height%2), fzm=czm-1+(mask.depth%2);
		cimg_mapV(*this,v) 
		  if (!weighted_convol) {	// Classical convolution
			for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
			  ftype val = 0;
			  for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
				val+= (*this)(x-xm,y-ym,z-zm,v)*mask(cxm+xm,cym+ym,czm+zm,0);
			  dest(x,y,z,v)=(restype)val;
			}
			if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
				  val+= pix3d(x-xm,y-ym,z-zm,v)*mask(cxm+xm,cym+ym,czm+zm,0);
				dest(x,y,z,v)=(restype)val;
			  }
			else cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++)  for (int xm=-cxm; xm<=fxm; xm++)
				  val+= pix3d(x-xm,y-ym,z-zm,v,0)*mask(cxm+xm,cym+ym,czm+zm,0);
				dest(x,y,z,v)=(restype)val;
			  }
		  } else {	// Weighted convolution
			for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
			  ftype val = 0, norm = 0;
			  for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++) {
				const T cval = (*this)(x-xm,y-ym,z-zm,v);
				val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				norm+= cval*cval;
			  }
			  dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt(norm)):0;
			}
			if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				ftype val = 0, norm = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++) {
				  const T cval = pix3d(x-xm,y-ym,z-zm,v);
				  val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				  norm+=cval*cval;
				}
				dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt(norm)):0;
			  }
			else cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
				double val = 0, norm = 0;
				for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++)  for (int xm=-cxm; xm<=fxm; xm++) {
				  const T cval = pix3d(x-xm,y-ym,z-zm,v,0);
				  val+= cval*mask(cxm+xm,cym+ym,czm+zm,0);
				  norm+= cval*cval;
				}
				dest(x,y,z,v)=(norm!=0)?(restype)(val/std::sqrt(norm)):0;
			  }
		  }
	  }
	  return dest;
	}
  
	//! Convolve the image by a mask
	/**
	   This is the in-place version of get_convolve().
	   \see get_convolve()
	**/
	template<typename t> CImg& convolve(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_convol=false) {
	  return get_convolve(mask,cond,weighted_convol).swap(*this); 
	}

	//! Return the erosion of the image by a structuring element.
	template<typename t> CImg<typename cimg::largest<T,t>::type> 
	get_erode(const CImg<t>& mask, const unsigned int cond=1, const bool weighted_erosion=false) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  if (is_empty()) return CImg<restype>();
	  if (mask.is_empty() || mask.dim!=1)
	throw CImgArgumentException("CImg<%s>::get_erosion() : Specified mask (%u,%u,%u,%u,%p) is not a scalar image.",
					pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	  CImg<restype> dest(*this,false);
	  const int cxm=mask.width/2, cym=mask.height/2, czm=mask.depth/2,
	fxm=cxm-1+(mask.width%2), fym=cym-1+(mask.height%2), fzm=czm-1+(mask.depth%2);
	  cimg_mapV(*this,v) 
	if (!weighted_erosion) {	// Classical erosion
	  for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
		restype min_val = cimg::type<restype>::max();
		for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
		  if (mask(cxm+xm,cym+ym,czm+zm,0)) min_val = cimg::min((restype)(*this)(x+xm,y+ym,z+zm,v),min_val);
		dest(x,y,z,v)=min_val;
	  }
	  if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
			restype min_val = cimg::type<restype>::max();
			for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			  if (mask(cxm+xm,cym+ym,czm+zm,0)) min_val = cimg::min((restype)pix3d(x+xm,y+ym,z+zm,v),min_val);
			dest(x,y,z,v)=min_val;
			  }
	  else cimg_mapYZV(*this,y,z,v)
		 for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
		   restype min_val = cimg::type<restype>::max();
		   for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			 if (mask(cxm+xm,cym+ym,czm+zm,0)) min_val = cimg::min((restype)pix3d(x+xm,y+ym,z+zm,v,0),min_val);
		   dest(x,y,z,v)=min_val;
		 }
	} else { // Weighted erosion
	  t mval=0;
	  for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
		restype min_val = cimg::type<restype>::max();
		for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
		  if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) min_val = cimg::min((restype)((*this)(x+xm,y+ym,z+zm,v)+mval),min_val);
		dest(x,y,z,v)=min_val;
	  }
	  if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
			restype min_val = cimg::type<restype>::max();
			for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			  if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) min_val = cimg::min((restype)(pix3d(x+xm,y+ym,z+zm,v)+mval),min_val);
			dest(x,y,z,v)=min_val;
			  }
	  else cimg_mapYZV(*this,y,z,v)
		 for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
		   restype min_val = cimg::type<restype>::max();
		   for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			 if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) min_val = cimg::min((restype)(pix3d(x+xm,y+ym,z+zm,v,0)+mval),min_val);
		   dest(x,y,z,v)=min_val;
		 }
	}    
	  return dest;
	}
	
	//! Erode the image by a structuring element
	/**
	   This is the in-place version of get_erode().
	   \see get_erode()
	**/
	template<typename t> CImg& erode(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_erosion=false) {
	  return get_erode(mask,cond,weighted_erosion).swap(*this); 
	}

	//! Erode the image by a square structuring element of size n
	CImg get_erode(const unsigned int n=1, const unsigned int cond=1) const {
	  static const CImg<T> mask(n,n,1,1,1);
	  return get_erode(mask,cond,false);      
	}

	//! Erode the image by a square structuring element of size n
	CImg& erode(const unsigned int n=1, const unsigned int cond=1) {
	  return get_erode(n,cond).swap(*this);
	}

	//! Return the dilatation of the image by a structuring element.
	template<typename t> CImg<typename cimg::largest<T,t>::type> 
	get_dilate(const CImg<t>& mask, const unsigned int cond=1, const bool weighted_dilatation=false) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  if (is_empty()) return CImg<restype>();
	  if (mask.is_empty() || mask.dim!=1)
	throw CImgArgumentException("CImg<%s>::get_dilate() : Specified mask (%u,%u,%u,%u,%p) is not a scalar image.",
					pixel_type(),mask.width,mask.height,mask.depth,mask.dim,mask.data);
	  CImg<restype> dest(*this,false);
	  const int cxm=mask.width/2, cym=mask.height/2, czm=mask.depth/2,
	fxm=cxm-1+(mask.width%2), fym=cym-1+(mask.height%2), fzm=czm-1+(mask.depth%2);
	  cimg_mapV(*this,v) 
	if (!weighted_dilatation) { // Classical dilatation
	  for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
		restype max_val = cimg::type<restype>::min();
		for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
		  if (mask(cxm+xm,cym+ym,czm+zm,0)) max_val = cimg::max((restype)(*this)(x+xm,y+ym,z+zm,v),max_val);
		dest(x,y,z,v)=max_val;
	  }
	  if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
			restype max_val = cimg::type<restype>::min();
			for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			  if (mask(cxm+xm,cym+ym,czm+zm,0)) max_val = cimg::max((restype)pix3d(x+xm,y+ym,z+zm,v),max_val);
			dest(x,y,z,v)=max_val;
			  }
	  else cimg_mapYZV(*this,y,z,v)
		 for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
		   restype max_val = cimg::type<restype>::min();
		   for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			 if (mask(cxm+xm,cym+ym,czm+zm,0)) max_val = cimg::max((restype)pix3d(x+xm,y+ym,z+zm,v,0),max_val);
		   dest(x,y,z,v)=max_val;
		 }
	} else { // Weighted dilatation
	  t mval=0;
	  for (int z=czm; z<dimz()-czm; z++) for (int y=cym; y<dimy()-cym; y++) for (int x=cxm; x<dimx()-cxm; x++) {
		restype max_val = cimg::type<restype>::min();
		for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
		  if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) max_val = cimg::max((restype)((*this)(x+xm,y+ym,z+zm,v)-mval),max_val);
		dest(x,y,z,v)=max_val;
	  }
	  if (cond) cimg_mapYZV(*this,y,z,v)
			  for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
			restype max_val = cimg::type<restype>::min();
			for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			  if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) max_val = cimg::max((restype)(pix3d(x+xm,y+ym,z+zm,v)-mval),max_val);
			dest(x,y,z,v)=max_val;
			  }
	  else cimg_mapYZV(*this,y,z,v)
		 for (int x=0; x<dimx(); (y<cym || y>=dimy()-cym || z<czm || z>=dimz()-czm)?x++:((x<cxm-1 || x>=dimx()-cxm)?x++:(x=dimx()-cxm))) {
		   restype max_val = cimg::type<restype>::min();
		   for (int zm=-czm; zm<=fzm; zm++) for (int ym=-cym; ym<=fym; ym++) for (int xm=-cxm; xm<=fxm; xm++)
			 if ((mval=mask(cxm+xm,cym+ym,czm+zm,0))!=0) max_val = cimg::max((restype)(pix3d(x+xm,y+ym,z+zm,v,0)-mval),max_val);
		   dest(x,y,z,v)=max_val;
		 }
	}    
	  return dest;
	}
	
	//! Dilate the image by a structuring element
	/**
	   This is the in-place version of get_dilate().
	   \see get_dilate()
	**/
	template<typename t> CImg& dilate(const CImg<t>& mask,const unsigned int cond=1,const bool weighted_dilatation=false) {
	  return get_dilate(mask,cond,weighted_dilatation).swap(*this); 
	}

	//! Dilate the image by a square structuring element of size n
	CImg get_dilate(const unsigned int n=1, const unsigned int cond=1) const {
	  static const CImg<T> mask(n,n,1,1,1);
	  return get_dilate(mask,cond,false);      
	}

	//! Dilate the image by a square structuring element of size n
	CImg& dilate(const unsigned int n=1, const unsigned int cond=1) {
	  return get_dilate(n,cond).swap(*this);
	}

	//! Add noise to the image
	/**
	   This is the in-place version of get_noise.
	   \see get_noise.
	**/
	CImg& noise(const double sigma=-20, const unsigned int ntype=0) {
	  if (!is_empty()) {
	double nsigma = sigma, max = (double)cimg::type<T>::max(), min = (double)cimg::type<T>::min();
	static bool first_time = true;
	if (first_time) { std::srand((unsigned int)::time(0)); first_time = false; }
	CImgStats st;
	if (nsigma==0) return *this;
	if (nsigma<0 || ntype==2) st = CImgStats(*this,false);
	if (nsigma<0) nsigma = -nsigma*(st.max-st.min)/100.0;
	switch (ntype) {
	case 0: { // Gaussian noise
	  cimg_map(*this,ptr,T) {
		double val = *ptr+nsigma*cimg::grand(); 
		if (val>max) val = max;
		if (val<min) val = min;
		*ptr = (T)val;
	  }
	} break;
	case 1: { // Uniform noise
	  cimg_map(*this,ptr,T) {
		double val = *ptr+nsigma*cimg::crand();
		if (val>max) val = max;
		if (val<min) val = min;
		*ptr = (T)val;
	  }
	} break;
	case 2: { // Salt & Pepper noise
	  if (st.max==st.min) { st.min=0; st.max=255; }
	  cimg_map(*this,ptr,T) if (cimg::rand()*100<nsigma) *ptr=(T)(cimg::rand()<0.5?st.max:st.min);
	} break;

	case 3: { // Poisson Noise (contribution by J�rome Boulanger)
	  cimg_map(*this,ptr,T) {
		const double z = (double)*ptr;
		if (z<=1.0e-10) *ptr=(T)0;
		else {
		  if (z>100.0) *ptr = (T)(unsigned int)((std::sqrt(z) * cimg::grand()) + z);
		  else {
		unsigned int k = 0;            const double y=std::exp(-z);
		for (double s=1.0; s>=y; k++) s *= cimg::rand();
		*ptr=(T)(k-1);
		  }
		}
	  }
	} break; 
	}
	  }
	  return *this;
	}
	
	//! Return a noisy image
	/**
	   \param sigma = power of the noise. if sigma<0, it corresponds to the percentage of the maximum image value.
	   \param ntype = noise type. can be 0=gaussian, 1=uniform or 2=Salt and Pepper.
	   \return A noisy version of the instance image.
	**/
	CImg get_noise(const double sigma=-20,const unsigned int ntype=0) const { return CImg<T>(*this,true).noise(sigma,ntype); }

#define cimg_deriche_map(x0,y0,z0,k0,nb,offset,T) { \
	ima = ptr(x0,y0,z0,k0); \
	I2 = *ima; ima+=offset; I1 = *ima; ima+=offset; \
	Y2 = *(Y++) = sumg0*I2; Y1 = *(Y++) = g0*I1 + sumg1*I2; \
	for (i=2; i<(nb); i++) { I1 = *ima; ima+=offset; \
		Y0 = *(Y++) = a1*I1 + a2*I2 + b1*Y1 + b2*Y2; \
		I2=I1; Y2=Y1; Y1=Y0; } \
	ima-=offset; I2 = *ima; Y2 = Y1 = parity*sumg1*I2; *ima = (T)(*(--Y)+Y2); \
	ima-=offset; I1 = *ima; *ima = (T)(*(--Y)+Y1); \
	for (i=(nb)-3; i>=0; i--) { Y0=a3*I1+a4*I2+b1*Y1+b2*Y2; ima-=offset; \
	  I2=I1; I1=*ima; *ima=(T)(*(--Y)+Y0); Y2=Y1; Y1=Y0; } \
  }

	//! Apply a deriche filter on the image
	/**
	   This is the in-place version of get_deriche
	   \see get_deriche.
	**/
	CImg& deriche(const float sigma=1,const int order=0,const char axe='x',const unsigned int cond=1) {
	  if (!is_empty()) {
	if (sigma<0 || order<0 || order>2)
	  throw CImgArgumentException("CImg<%s>::deriche() : Bad arguments (sigma=%g, order=%d)",pixel_type(),sigma,order);
	const float alpha=sigma>0?(1.695f/sigma):20,ea=(float)std::exp(alpha),ema=(float)std::exp(-alpha),em2a=ema*ema,b1=2*ema,b2=-em2a;
	float ek,ekn,parity,a1,a2,a3,a4,g0,sumg1,sumg0;
	double *Y,Y0,Y1,Y2;
	int i,offset,nb;
	T *ima,I1,I2;
	switch(order) {
	case 1:                 // first derivative
	  ek = -(1-ema)*(1-ema)*(1-ema)/(2*(ema+1)*ema); a1 = a4 = 0;  a2 = ek*ema; a3 = -ek*ema; parity =-1;
	  if (cond) { sumg1 = (ek*ea) / ((ea-1)*(ea-1)); g0 = 0; sumg0 = g0+sumg1; }
	  else g0 = sumg0 = sumg1 = 0;
	  break;
	case 2:               // second derivative
	  ekn = ( -2*(-1+3*ea-3*ea*ea+ea*ea*ea)/(3*ea+1+3*ea*ea+ea*ea*ea) );
	  ek = -(em2a-1)/(2*alpha*ema); a1 = ekn;  a2 = -ekn*(1+ek*alpha)*ema; a3 = ekn*(1-ek*alpha)*ema; a4 = -ekn*em2a; parity =1;
	  if (cond) { sumg1 = ekn/2; g0 = ekn; sumg0 = g0+sumg1; }
	  else g0=sumg0=sumg1=0;
	  break;
	default:              // smoothing
	  ek = (1-ema)*(1-ema) / (1+2*alpha*ema - em2a); a1 = ek;  a2 = ek*ema*(alpha-1); a3 = ek*ema*(alpha+1); a4 = -ek*em2a; parity = 1;
	  if (cond) { sumg1 = ek*(alpha*ea+ea-1) / ((ea-1)*(ea-1)); g0 = ek; sumg0 = g0+sumg1; }
	  else  g0=sumg0=sumg1=0;
	  break;
	}
	// filter init
	Y = new double[cimg::max(width,height,depth)];
	switch(cimg::uncase(axe)) {
	case 'x': if (width>1)  { offset = 1;            nb = width;  cimg_mapYZV(*this,y,z,k) cimg_deriche_map(0,y,z,k,nb,offset,T); }	break;
	case 'y': if (height>1) { offset = width;        nb = height; cimg_mapXZV(*this,x,z,k) cimg_deriche_map(x,0,z,k,nb,offset,T); }	break;
	case 'z': if (depth>1)  { offset = width*height; nb = depth;  cimg_mapXYV(*this,x,y,k) cimg_deriche_map(x,y,0,k,nb,offset,T); }	break;
	default: throw CImgArgumentException("CImg<%s>::deriche() : unknow axe '%c', must be 'x','y' or 'z'",pixel_type(),axe);
	}
	delete[] Y;
	  }
	  return *this;
	}

	//! Return the result of the Deriche filter
	/**
	   The Canny-Deriche filter is a recursive algorithm allowing to compute blurred derivatives of
	   order 0,1 or 2 of an image.
	   \see blur
	**/
	CImg get_deriche(const float sigma=1,const int order=0,const char axe='x',const unsigned int cond=1) const {
	  return CImg<T>(*this,true).deriche(sigma,order,axe,cond);
	}

	//! Blur the image with a Deriche filter (anisotropically)
	/**
	   This is the in-place version of get_blur().
	   \see get_blur().
	**/
	CImg& blur(const float sigmax,const float sigmay,const float sigmaz,const unsigned int cond=1) {
	  if (!is_empty()) {
	if (width>1  && sigmax>0) deriche(sigmax,0,'x',cond);
	if (height>1 && sigmay>0) deriche(sigmay,0,'y',cond);
	if (depth>1  && sigmaz>0) deriche(sigmaz,0,'z',cond);
	  }
	  return *this;
	}
	
	//! Blur the image with a Canny-Deriche filter.
	/** This is the in-place version of get_blur(). **/
	CImg& blur(const float sigma,const unsigned int cond=1) { return blur(sigma,sigma,sigma,cond); }

	//! Return a blurred version of the image, using a Canny-Deriche filter.
	/**
	   Blur the image with an anisotropic exponential filter (Deriche filter of order 0).
	**/
	CImg get_blur(const float sigmax,const float sigmay,const float sigmaz,const unsigned int cond=1) const {
	  return CImg<T>(*this,true).blur(sigmax,sigmay,sigmaz,cond); 
	}
	
	//! Return a blurred version of the image, using a Canny-Deriche filter.
	CImg get_blur(const float sigma,const unsigned int cond=1) const { return CImg<T>(*this,true).blur(sigma,cond); }

	//! Blur an image following a field of diffusion tensors.
	/** This is the in-place version of get_blur_anisotropic(). **/
	template<typename t> 
	CImg& blur_anisotropic(const CImg<t>& G, const float amplitude=30.0f, const float dl=0.8f,const float da=30.0f,
			   const float gauss_prec=2.0f, const unsigned int interpolation=0, const bool fast_approx=true) {
	  
	  // Check arguments and init variables
	  if (!is_empty() && amplitude>0) {
	if (G.is_empty() || (G.dim!=3 && G.dim!=6) || G.width!=width || G.height!=height || G.depth!=depth)
	  throw CImgArgumentException("CImg<%s>::blur_anisotropic() : Specified tensor field (%u,%u,%u,%u) is not valid.",
					  pixel_type(),G.width,G.height,G.depth,G.dim);
	
	const int dx1 = dimx()-1, dy1 = dimy()-1, dz1 = dimz()-1;
	const bool threed = (G.dim>=6);
	CImg<t> dest(width,height,depth,dim,0), tmp(dim), W(width,height,depth,threed?4:3);
	int N = 0;
	const float sqrt2amplitude = (float)std::sqrt(2*amplitude);
	
	if (threed)
	  // 3D version of the algorithm
	  for (float phi=(180%(int)da)/2.0f; phi<=180; phi+=da) {
		const float phir = (float)(phi*cimg::PI/180), datmp = (float)(da/std::cos(phir)), da2 = datmp<1?360.0f:datmp;
		for (float theta=0; theta<360; (theta+=da2),N++) {
		  const float thetar = (float)(theta*cimg::PI/180),
		vx = (float)(std::cos(thetar)*std::cos(phir)),
		vy = (float)(std::sin(thetar)*std::cos(phir)),
		vz = (float)std::sin(phir);
		  const t 
		*pa = G.ptr(0,0,0,0), *pb = G.ptr(0,0,0,1), *pc = G.ptr(0,0,0,2),
		*pd = G.ptr(0,0,0,3), *pe = G.ptr(0,0,0,4), *pf = G.ptr(0,0,0,5);
		  t *pd0 = W.ptr(0,0,0,0), *pd1 = W.ptr(0,0,0,1), *pd2 = W.ptr(0,0,0,2), *pd3 = W.ptr(0,0,0,3);
		  cimg_mapXYZ(G,xg,yg,zg) {
		const t
		  a = *(pa++), b = *(pb++), c = *(pc++),
		  d = *(pd++), e = *(pe++), f = *(pf++),
		  u = a*vx + b*vy + c*vz,
		  v = b*vx + d*vy + e*vz,
		  w = c*vx + e*vy + f*vz,
		  n = (t)std::sqrt(1e-5+u*u+v*v+w*w),
		  dln = dl/n;
		*(pd0++) = u*dln;
		*(pd1++) = v*dln;
		*(pd2++) = w*dln;
		*(pd3++) = n;
		  }
		  
		  cimg_mapXYZ(*this,x,y,z) {
		tmp.fill(0);
		const t cu = W(x,y,z,0), cv = W(x,y,z,1), cw = W(x,y,z,2), n = W(x,y,z,3);
		const float
		  fsigma = (float)(n*sqrt2amplitude),
		  length = gauss_prec*fsigma,
		  fsigma2 = 2*fsigma*fsigma;
		float l, S=0, pu=cu, pv=cv, pw=cw, X=(float)x, Y=(float)y, Z=(float)z;
		if (fast_approx) switch (interpolation) {
		case 0: // Nearest neighbor interpolation
		  for (l=0; l<length; l+=dl) {
			const float 
			  Xn = X<0?0:(X>=dx1?dx1:X),
			  Yn = Y<0?0:(Y>=dy1?dy1:Y),
			  Zn = Z<0?0:(Z>=dz1?dz1:Z);
			const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f), zi = (int)(Zn+0.5f);
			t u = W(xi,yi,zi,0), v = W(xi,yi,zi,1), w = W(xi,yi,zi,2);
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)(*this)(xi,yi,zi,k);
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
			t u = (t)(W.linear_pix3d(X,Y,Z,0)), v = (t)(W.linear_pix3d(X,Y,Z,1)), w = (t)(W.linear_pix3d(X,Y,Z,2));
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)linear_pix3d(X,Y,Z,k);
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		default: // 2nd order Runge Kutta interpolation
		  for (l=0; l<length; l+=dl) {
			t u0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,0)), v0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,1)), w0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,2));
			if ((pu*u0+pv*v0+pw*w0)<0) { u0=-u0; v0=-v0; w0=-w0; }
			t u = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,0)), v = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,1)), w = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,2));
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)linear_pix3d(X,Y,Z,k);
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S++;
		  } break;
		}
		else switch (interpolation) {
		case 0: // Nearest neighbor interpolation
		  for (l=0; l<length; l+=dl) {
			const float 
			  coef = (float)std::exp(-l*l/fsigma2),
			  Xn = X<0?0:(X>=dx1?dx1:X),
			  Yn = Y<0?0:(Y>=dy1?dy1:Y),
			  Zn = Z<0?0:(Z>=dz1?dz1:Z);
			const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f), zi = (int)(Zn+0.5f);
			t u = W(xi,yi,zi,0), v = W(xi,yi,zi,1), w = W(xi,yi,zi,2);
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*(*this)(xi,yi,zi,k));
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
			const float coef = (float)std::exp(-l*l/fsigma2);
			t u = (t)(W.linear_pix3d(X,Y,Z,0)), v = (t)(W.linear_pix3d(X,Y,Z,1)), w = (t)(W.linear_pix3d(X,Y,Z,2));
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix3d(X,Y,Z,k));
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		default: // 2nd order Runge Kutta interpolation
		  for (l=0; l<length; l+=dl) {
			const float coef = (float)std::exp(-l*l/fsigma2);
			t u0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,0)), v0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,1)), w0 = (t)(0.5f*dl*W.linear_pix3d(X,Y,Z,2));
			if ((pu*u0+pv*v0+pw*w0)<0) { u0=-u0; v0=-v0; w0=-w0; }
			t u = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,0)), v = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,1)), w = (t)(W.linear_pix3d(X+u0,Y+v0,Z+w0,2));
			if ((pu*u+pv*v+pw*w)<0) { u=-u; v=-v; w=-w; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix3d(X,Y,Z,k));
			X+=(pu=u); Y+=(pv=v); Z+=(pw=w); S+=coef;
		  } break;
		}
		if (S>0) cimg_mapV(dest,k) dest(x,y,z,k)+=tmp[k]/S;
		else cimg_mapV(dest,k) dest(x,y,z,k)+=(t)((*this)(x,y,z,k));
		  }
		}
	  } else
		// 2D version of the algorithm
		for (float theta=(360%(int)da)/2.0f; theta<360; (theta+=da),N++) {
		  const float thetar = (float)(theta*cimg::PI/180), vx = (float)(std::cos(thetar)), vy = (float)(std::sin(thetar));
		  
		  const t *pa = G.ptr(0,0,0,0), *pb = G.ptr(0,0,0,1), *pc = G.ptr(0,0,0,2);
		  t *pd0 = W.ptr(0,0,0,0), *pd1 = W.ptr(0,0,0,1), *pd2 = W.ptr(0,0,0,2);
		  cimg_mapXY(G,xg,yg) {
		const t
		  a = *(pa++), b = *(pb++), c = *(pc++), 
		  u = a*vx + b*vy, v = b*vx + c*vy,
		  n = (t)std::sqrt(1e-5+u*u+v*v),
		  dln = dl/n;
		*(pd0++) = u*dln;
		*(pd1++) = v*dln;
		*(pd2++) = n;
		  }
		  
		  cimg_mapXY(*this,x,y) {
		tmp.fill(0);
		const t cu = W(x,y,0,0), cv = W(x,y,0,1), n = W(x,y,0,2);
		const float
		  fsigma = (float)(n*sqrt2amplitude),
		  length = gauss_prec*fsigma,
		  fsigma2 = 2*fsigma*fsigma;
		float l, S=0, pu=cu, pv=cv, X=(float)x, Y=(float)y;
		if (fast_approx) switch (interpolation) {		  
		case 0: // Nearest-neighbor interpolation
		  for (l=0; l<length; l+=dl) {
			const float 
			  Xn = X<0?0:(X>=dx1?dx1:X),
			  Yn = Y<0?0:(Y>=dy1?dy1:Y);
			const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f);
			t u = W(xi,yi,0,0), v = W(xi,yi,0,1);
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)(*this)(xi,yi,0,k);
			X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
			t u = (t)(W.linear_pix2d(X,Y,0,0)), v = (t)(W.linear_pix2d(X,Y,0,1));
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)linear_pix2d(X,Y,0,k);
			X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		default: // 2nd-order Runge-kutta interpolation
		  for (l=0; l<length; l+=dl) {
			t u0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,0)), v0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,1));
			if ((pu*u0+pv*v0)<0) { u0=-u0; v0=-v0; }
			t u = (t)(W.linear_pix2d(X+u0,Y+v0,0,0)), v = (t)(W.linear_pix2d(X+u0,Y+v0,0,1));
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)linear_pix2d(X,Y,0,k);
			X+=(pu=u); Y+=(pv=v); S++;
		  } break;
		}
		else switch (interpolation) {		  
		case 0: // Nearest-neighbor interpolation
		  for (l=0; l<length; l+=dl) {
			const float 
			  coef = (float)std::exp(-l*l/fsigma2),
			  Xn = X<0?0:(X>=dx1?dx1:X),
			  Yn = Y<0?0:(Y>=dy1?dy1:Y);
			const int xi = (int)(Xn+0.5f), yi = (int)(Yn+0.5f);
			t u = W(xi,yi,0,0), v = W(xi,yi,0,1);
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*(*this)(xi,yi,0,k));
			X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		case 1: // Linear interpolation
		  for (l=0; l<length; l+=dl) {
			const float coef = (float)std::exp(-l*l/fsigma2);
			t u = (t)(W.linear_pix2d(X,Y,0,0)), v = (t)(W.linear_pix2d(X,Y,0,1));
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix2d(X,Y,0,k));
			X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		default: // 2nd-order Runge-kutta interpolation
		  for (l=0; l<length; l+=dl) {
			const float coef = (float)std::exp(-l*l/fsigma2);
			t u0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,0)), v0 = (t)(0.5f*dl*W.linear_pix2d(X,Y,0,1));
			if ((pu*u0+pv*v0)<0) { u0=-u0; v0=-v0; }
			t u = (t)(W.linear_pix2d(X+u0,Y+v0,0,0)), v = (t)(W.linear_pix2d(X+u0,Y+v0,0,1));
			if ((pu*u+pv*v)<0) { u=-u; v=-v; }
			cimg_mapV(*this,k) tmp[k]+=(t)(coef*linear_pix2d(X,Y,0,k));
			X+=(pu=u); Y+=(pv=v); S+=coef;
		  } break;
		}
		if (S>0) cimg_mapV(dest,k) dest(x,y,0,k)+=tmp[k]/S;
		else cimg_mapV(dest,k) dest(x,y,0,k)+=(t)((*this)(x,y,0,k));
		  }
		}
	const float *ptrs = dest.data+dest.size(); cimg_map(*this,ptrd,T) *ptrd = (T)(*(--ptrs)/N);
	  }
	  return *this;
	}

	//! Get a blurred version of an image following a field of diffusion tensors.
	/**
	   \param G = Field of square roots of diffusion tensors used to drive the smoothing.
	   \param amplitude = amplitude of the smoothing.
	   \param dl = spatial discretization.
	   \param da = angular discretization.
	   \param gauss_prec = precision of the gaussian function.
	   \param interpolation Used interpolation scheme (0 = nearest-neighbor, 1 = linear, 2 = Runge-Kutta)
	   \param fast_approx = Tell to use the fast approximation or not.
	**/
	template<typename t>
	CImg get_blur_anisotropic(const CImg<t>& G, const float amplitude=30.0f, const float dl=0.8f,const float da=30.0f,
				  const float gauss_prec=2.0f, const unsigned int interpolation=0, const bool fast_approx=true) const {
	  return CImg<T>(*this,true).blur_anisotropic(G,amplitude,dl,da,gauss_prec,interpolation,fast_approx);
	}
	
	//! Blur an image following a field of diffusion tensors.
	CImg& blur_anisotropic(const float amplitude, const float sharpness=0.8f, const float anisotropy=0.3f,
			   const float alpha=1.2f,const float sigma=0.8f, const float dl=0.8f,const float da=30.0f,
			   const float gauss_prec=2.0f, const unsigned int interpolation=0, const bool fast_approx=true) {
	  if (!is_empty() && amplitude>0) {
	if (amplitude==0) return *this;
	if (amplitude<0 || sharpness<0 || anisotropy<0 || anisotropy>1 || alpha<0 || sigma<0 || dl<0 || da<0 || gauss_prec<0)
	  throw CImgArgumentException("CImg<%s>::blur_anisotropic() : Given parameters are amplitude(%g), sharpness(%g), "
					  "anisotropy(%g), alpha(%g), sigma(%g), dl(%g), da(%g), gauss_prec(%g).\n"
					  "Admissible parameters are in the range : amplitude>0, sharpness>0, anisotropy in [0,1], "
					  "alpha>0, sigma>0, dl>0, da>0, gauss_prec>0.",
					  pixel_type(),amplitude,sharpness,anisotropy,alpha,sigma,dl,da,gauss_prec);
	const bool threed = (depth>1);
	CImg<float> G(width,height,depth,(threed?6:3),0);
	const float power1 = 0.5f*sharpness, power2 = power1/(1e-7f+1.0f-anisotropy);
	const CImgStats stats(*this,false);
	const float valmin = (float)stats.min, valmax = (float)stats.max;
	const CImg<float> blurred = CImg<float>(*this).blur(alpha).normalize(valmin,valmax);
	
	if (threed) { // Field for 3D volumes    
	  CImg<float> val(3),vec(3,3);
	  CImg_3x3x3(I,float);
	  cimg_mapV(*this,k) cimg_map3x3x3(blurred,x,y,z,k,I) {
		const float 
		  ixf = Incc-Iccc, iyf = Icnc-Iccc, izf = Iccn-Iccc,
		  ixb = Iccc-Ipcc, iyb = Iccc-Icpc, izb = Iccc-Iccp;
		G(x,y,z,0) += 0.5f*(ixf*ixf + ixb*ixb);
		G(x,y,z,1) += 0.25f*(ixf*iyf + ixf*iyb + ixb*iyf + ixb*iyb);
		G(x,y,z,2) += 0.25f*(ixf*izf + ixf*izb + ixb*izf + ixb*izb);
		G(x,y,z,3) += 0.5f*(iyf*iyf + iyb*iyb);
		G(x,y,z,4) += 0.25f*(iyf*izf + iyf*izb + iyb*izf + iyb*izb);
		G(x,y,z,5) += 0.5f*(izf*izf + izb*izb);
	  }
	  if (sigma>0) G.blur(sigma);
	  cimg_mapXYZ(*this,x,y,z) {
		G.get_tensor_at(x,y,z).symmetric_eigen(val,vec);
		const float l1 = val[2], l2 = val[1], l3 = val[0],
		  ux = vec(0,0), uy = vec(0,1), uz = vec(0,2),
		  vx = vec(1,0), vy = vec(1,1), vz = vec(1,2),
		  wx = vec(2,0), wy = vec(2,1), wz = vec(2,2),
		  n1 = (float)std::pow(1.0f+l1+l2+l3,-power1),
		  n2 = (float)std::pow(1.0f+l1+l2+l3,-power2);
		G(x,y,z,0) = n1*(ux*ux + vx*vx) + n2*wx*wx;
		G(x,y,z,1) = n1*(ux*uy + vx*vy) + n2*wx*wy;
		G(x,y,z,2) = n1*(ux*uz + vx*vz) + n2*wx*wz;
		G(x,y,z,3) = n1*(uy*uy + vy*vy) + n2*wy*wy;
		G(x,y,z,4) = n1*(uy*uz + vy*vz) + n2*wy*wz;
		G(x,y,z,5) = n1*(uz*uz + vz*vz) + n2*wz*wz;
	  }
	} else { // Field for 2D images
	  CImg<float> val(2),vec(2,2);
	  CImg_3x3x1(I,float);
	  cimg_mapV(*this,k) cimg_map3x3x1(blurred,x,y,0,k,I) {
		const float
		  ixf = Inc-Icc, iyf = Icn-Icc,
		  ixb = Icc-Ipc, iyb = Icc-Icp;
		G(x,y,0,0) += 0.5f*(ixf*ixf+ixb*ixb);
		G(x,y,0,1) += 0.25f*(ixf*iyf+ixf*iyb+ixb*iyf+ixb*iyb);
		G(x,y,0,2) += 0.5f*(iyf*iyf+iyb*iyb);
	  }
	  if (sigma>0) G.blur(sigma);
	  cimg_mapXY(*this,x,y) {
		G.get_tensor_at(x,y).symmetric_eigen(val,vec);
		const float l1 = val[1], l2 = val[0],
		  ux = vec(1,0), uy = vec(1,1),
		  vx = vec(0,0), vy = vec(0,1),
		  n1 = (float)std::pow(1.0f+l1+l2,-power1),
		  n2 = (float)std::pow(1.0f+l1+l2,-power2);
		G(x,y,0,0) = n1*ux*ux + n2*vx*vx;
		G(x,y,0,1) = n1*ux*uy + n2*vx*vy;
		G(x,y,0,2) = n1*uy*uy + n2*vy*vy;
	  }
	}
	blur_anisotropic(G,amplitude,dl,da,gauss_prec,interpolation,fast_approx);
	  }
	  return *this;
	}

	//! Blur an image in an anisotropic way.
	/**
	   \param amplitude = amplitude of the anisotropic blur.
	   \param sharpness = define the contour preservation.
	   \param anisotropy = define the smoothing anisotropy.
	   \param alpha = image pre-blurring (gaussian).
	   \param sigma = regularity of the tensor-valued geometry.
	   \param dl = spatial discretization.
	   \param da = angular discretization.
	   \param gauss_prec = precision of the gaussian function.
	   \param interpolation Used interpolation scheme (0 = nearest-neighbor, 1 = linear, 2 = Runge-Kutta)
	   \param fast_approx = Tell to use the fast approximation or not
	**/
	CImg get_blur_anisotropic(const float amplitude, const float sharpness=0.8f, const float anisotropy=0.3f,
				  const float alpha=1.2f, const float sigma=0.8f, const float dl=0.8f,
				  const float da=30.0f, const float gauss_prec=2.0f, const unsigned int interpolation=0,
				  const bool fast_approx=true) const {
	  
	  return CImg<T>(*this,true).blur_anisotropic(amplitude,sharpness,anisotropy,alpha,sigma,dl,da,gauss_prec,interpolation,fast_approx);
	}

	//! Return the Fast Fourier Transform of an image (along a specified axis)
	CImgl<typename cimg::largest<T,float>::type> get_FFT(const char axe,const bool inverse=false) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImgl<restype>(*this,CImg<restype>(width,height,depth,dim,0)).FFT(axe,inverse);
	}

	//! Return the Fast Fourier Transform on an image
	CImgl<typename cimg::largest<T,float>::type> get_FFT(const bool inverse=false) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImgl<restype>(*this,CImg<restype>(width,height,depth,dim,0)).FFT(inverse);
	}

	//! Apply a median filter.
	CImg get_blur_median(const unsigned int n=3) {
	  CImg<T> res(*this,false);
	  if (!n || n==1) return *this;
	  const int hl=n/2, hr=hl-1+n%2;
	  if (res.depth!=1) {  // 3D median filter
	CImg<T> vois;
	cimg_mapXYZV(*this,x,y,z,k) {
	  vois = get_crop(x-hl,y-hl,z-hl,k,x+hr,y+hr,z+hr,k);
	  res(x,y,z,k) = vois.median();
	}
	  } else { // 2D median filter
#define _median_sort(a,b) if ((a)>(b)) cimg::swap(a,b)
	switch (n) {
	case 3: {
	  CImg_3x3(I,T);
	  CImg_3x3(J,T);
	  cimg_mapV(*this,k) cimg_map3x3(*this,x,y,0,k,I) {
		cimg_copy3x3x1(I,J);
		_median_sort(Jcp, Jnp); _median_sort(Jcc, Jnc); _median_sort(Jcn, Jnn);
		_median_sort(Jpp, Jcp); _median_sort(Jpc, Jcc); _median_sort(Jpn, Jcn);
		_median_sort(Jcp, Jnp); _median_sort(Jcc, Jnc); _median_sort(Jcn, Jnn);
		_median_sort(Jpp, Jpc); _median_sort(Jnc, Jnn); _median_sort(Jcc, Jcn);
		_median_sort(Jpc, Jpn); _median_sort(Jcp, Jcc); _median_sort(Jnp, Jnc);
		_median_sort(Jcc, Jcn); _median_sort(Jcc, Jnp); _median_sort(Jpn, Jcc);
		_median_sort(Jcc, Jnp);
		res(x,y,0,k) = Jcc;
	  }
	} break;
	case 5: {
	  CImg_5x5(I,T);
	  CImg_5x5(J,T);
	  cimg_mapV(*this,k) cimg_map5x5(*this,x,y,0,k,I) {
		cimg_copy5x5x1(I,J);
		_median_sort(Jbb, Jpb); _median_sort(Jnb, Jab); _median_sort(Jcb, Jab); _median_sort(Jcb, Jnb);
		_median_sort(Jpp, Jcp); _median_sort(Jbp, Jcp); _median_sort(Jbp, Jpp); _median_sort(Jap, Jbc);
		_median_sort(Jnp, Jbc); _median_sort(Jnp, Jap); _median_sort(Jcc, Jnc); _median_sort(Jpc, Jnc);
		_median_sort(Jpc, Jcc); _median_sort(Jbn, Jpn); _median_sort(Jac, Jpn); _median_sort(Jac, Jbn);
		_median_sort(Jnn, Jan); _median_sort(Jcn, Jan); _median_sort(Jcn, Jnn); _median_sort(Jpa, Jca);
		_median_sort(Jba, Jca); _median_sort(Jba, Jpa); _median_sort(Jna, Jaa); _median_sort(Jcb, Jbp);
		_median_sort(Jnb, Jpp); _median_sort(Jbb, Jpp); _median_sort(Jbb, Jnb); _median_sort(Jab, Jcp);
		_median_sort(Jpb, Jcp); _median_sort(Jpb, Jab); _median_sort(Jpc, Jac); _median_sort(Jnp, Jac);
		_median_sort(Jnp, Jpc); _median_sort(Jcc, Jbn); _median_sort(Jap, Jbn); _median_sort(Jap, Jcc);
		_median_sort(Jnc, Jpn); _median_sort(Jbc, Jpn); _median_sort(Jbc, Jnc); _median_sort(Jba, Jna);
		_median_sort(Jcn, Jna); _median_sort(Jcn, Jba); _median_sort(Jpa, Jaa); _median_sort(Jnn, Jaa);
		_median_sort(Jnn, Jpa); _median_sort(Jan, Jca); _median_sort(Jnp, Jcn); _median_sort(Jap, Jnn);
		_median_sort(Jbb, Jnn); _median_sort(Jbb, Jap); _median_sort(Jbc, Jan); _median_sort(Jpb, Jan);
		_median_sort(Jpb, Jbc); _median_sort(Jpc, Jba); _median_sort(Jcb, Jba); _median_sort(Jcb, Jpc);
		_median_sort(Jcc, Jpa); _median_sort(Jnb, Jpa); _median_sort(Jnb, Jcc); _median_sort(Jnc, Jca);
		_median_sort(Jab, Jca); _median_sort(Jab, Jnc); _median_sort(Jac, Jna); _median_sort(Jbp, Jna);
		_median_sort(Jbp, Jac); _median_sort(Jbn, Jaa); _median_sort(Jpp, Jaa); _median_sort(Jpp, Jbn);
		_median_sort(Jcp, Jpn); _median_sort(Jcp, Jan); _median_sort(Jnc, Jpa); _median_sort(Jbn, Jna);
		_median_sort(Jcp, Jnc); _median_sort(Jcp, Jbn); _median_sort(Jpb, Jap); _median_sort(Jnb, Jpc);
		_median_sort(Jbp, Jcn); _median_sort(Jpc, Jcn); _median_sort(Jap, Jcn); _median_sort(Jab, Jbc);
		_median_sort(Jpp, Jcc); _median_sort(Jcp, Jac); _median_sort(Jab, Jpp); _median_sort(Jab, Jcp);
		_median_sort(Jcc, Jac); _median_sort(Jbc, Jac); _median_sort(Jpp, Jcp); _median_sort(Jbc, Jcc);
		_median_sort(Jpp, Jbc); _median_sort(Jpp, Jcn); _median_sort(Jcc, Jcn); _median_sort(Jcp, Jcn);
		_median_sort(Jcp, Jbc); _median_sort(Jcc, Jnn); _median_sort(Jcp, Jcc); _median_sort(Jbc, Jnn);
		_median_sort(Jcc, Jba); _median_sort(Jbc, Jba); _median_sort(Jbc, Jcc);
		res(x,y,0,k) = Jcc;
	  }
	} break;
	default: {
	  CImg<T> vois;
	  cimg_mapXYV(*this,x,y,k) {
		vois = get_crop(x-hl,y-hl,0,k,x+hr,y+hr,0,k);
		res(x,y,0,k) = vois.median();
	  }  
	} break;
	}
	  }
	  return res;      
	}

	//! Apply a median filter
	CImg& blur_median(const unsigned int n=3) { return get_blur_median(n).swap(*this); }
   
	//@}
	//
	//
	//
	//! \name Matrix and vector computation
	//@{
	//
	//

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1) {
	  return CImg<T>(1,1).fill(a1); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2) {
	  return CImg<T>(1,2).fill(a1,a2); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3) { 
	  return CImg<T>(1,3).fill(a1,a2,a3); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4) {
	  return CImg<T>(1,4).fill(a1,a2,a3,a4); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5) {
	  return CImg<T>(1,5).fill(a1,a2,a3,a4,a5); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,const T& a6) { 
	  return CImg<T>(1,6).fill(a1,a2,a3,a4,a5,a6); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7) {
	  return CImg<T>(1,7).fill(a1,a2,a3,a4,a5,a6,a7); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8) { 
	  return CImg<T>(1,8).fill(a1,a2,a3,a4,a5,a6,a7,a8); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,const T& a9) { 
	  return CImg<T>(1,9).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,
			   const T& a9,const T& a10) { 
	  return CImg<T>(1,10).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,
			   const T& a9,const T& a10, const T& a11) { 
	  return CImg<T>(1,11).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11); 
	}

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,
			   const T& a9,const T& a10, const T& a11, const T& a12) { 
	  return CImg<T>(1,12).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12); 
	}  

	//! Return a vector with specified coefficients
	static CImg vector(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,
			   const T& a9,const T& a10, const T& a11, const T& a12,
			   const T& a13) { 
	  return CImg<T>(1,13).fill(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13); 
	}

	//! Unroll all image values into a vector.
	CImg& vector() {
	  const unsigned int siz = size();
	  width = depth = dim = 1;
	  height = siz;
	  return *this;
	}
	
	CImg get_vector() const {
	  return CImg<T>(*this).vector();
	}

	//! Return a 1x1 square matrix with specified coefficients
	static CImg matrix(const T& a1) {
	  return vector(a1); 
	}

	//! Return a 2x2 square matrix with specified coefficients
	static CImg matrix(const T& a1,const T& a2,
			   const T& a3,const T& a4) { 
	  return CImg<T>(2,2).fill(a1,a2,
				   a3,a4);
	}

	//! Return a 3x3 square matrix with specified coefficients
	static CImg matrix(const T& a1,const T& a2,const T& a3,
			   const T& a4,const T& a5,const T& a6,
			   const T& a7,const T& a8,const T& a9) {
	  return CImg<T>(3,3).fill(a1,a2,a3,
				   a4,a5,a6,
				   a7,a8,a9);
	}

	//! Return a 4x4 square matrix with specified coefficients
	static CImg matrix(const T& a1,const T& a2,const T& a3,const T& a4,
			   const T& a5,const T& a6,const T& a7,const T& a8,
			   const T& a9,const T& a10,const T& a11,const T& a12,
			   const T& a13,const T& a14,const T& a15,const T& a16) {
	  return CImg<T>(4,4).fill(a1,a2,a3,a4,
				   a5,a6,a7,a8,
				   a9,a10,a11,a12,
				   a13,a14,a15,a16);
	}

	//! Return a 5x5 square matrix with specified coefficients
	static CImg matrix(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,
			   const T& a6,const T& a7,const T& a8,const T& a9,const T& a10,
			   const T& a11,const T& a12,const T& a13,const T& a14,const T& a15,
			   const T& a16,const T& a17,const T& a18,const T& a19,const T& a20,
			   const T& a21,const T& a22,const T& a23,const T& a24,const T& a25) {
	  return CImg<T>(5,5).fill(a1,a2,a3,a4,a5,
				   a6,a7,a8,a9,a10,
				   a11,a12,a13,a14,a15,
				   a16,a17,a18,a19,a20,
				   a21,a22,a23,a24,a25);
	}

	CImg& matrix() {
	  const unsigned int siz = size();
	  switch (siz) {
	  case 1: break;
	  case 4: width = height = 2; break;
	  case 9: width = height = 3; break;
	  case 16: width = height = 4; break;
	  case 25: width = height = 5; break;
	  case 36: width = height = 6; break;
	  case 49: width = height = 7; break;
	  case 64: width = height = 8; break;
	  case 81: width = height = 9; break;
	  case 100: width = height = 10; break;
	  default: {
	unsigned int i=11, i2=i*i;
	while (i2<siz) { i2+=2*i+1; i++; }
	if (i2==siz) width = height = i;
	else throw CImgInstanceException("CImg<%s>::matrix() : Image size = %u is not a square number",pixel_type(),siz);
	  } break;
	  }
	  return *this;
	}

	CImg get_matrix() const {
	  return CImg<T>(*this).matrix();
	}

	//! Return a 1x1 symmetric matrix with specified coefficients
	static CImg tensor(const T& a1) { 
	  return matrix(a1); 
	}

	//! Return a 2x2 symmetric matrix tensor with specified coefficients
	static CImg tensor(const T& a1,const T& a2,const T& a3) { 
	  return matrix(a1,a2,
			a2,a3); 
	}

	//! Return a 3x3 symmetric matrix with specified coefficients
	static CImg tensor(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5,const T& a6) {
	  return matrix(a1,a2,a3,
			a2,a4,a5,
			a3,a5,a6);
	}

	CImg get_tensor() const {
	  CImg<T> res;
	  const unsigned int siz = size();
	  switch (siz) {
	  case 1: break;
	  case 3: 
	res.assign(2,2);
	res(0,0) = (*this)(0);
	res(1,0) = res(0,1) = (*this)(1);
	res(1,1) = (*this)(2);
	break;
	  case 6:
	res.assign(3,3);
	res(0,0) = (*this)(0);
	res(1,0) = res(0,1) = (*this)(1);
	res(2,0) = res(0,2) = (*this)(2);
	res(1,1) = (*this)(3);
	res(2,1) = res(1,2) = (*this)(4);
	res(2,2) = (*this)(5);
	break;
	  default:
	throw CImgInstanceException("CImg<%s>::get_tensor() : Wrong vector dimension = %u in instance image.",
					pixel_type(), dim);
	break;
	  }
	  return res;
	}

	CImg& tensor() {
	  return get_tensor().swap(*this);
	}

	//! Return a 1x1 diagonal matrix with specified coefficients
	static CImg diagonal(const T& a1) { 
	  return matrix(a1); 
	}

	//! Return a 2x2 diagonal matrix with specified coefficients
	static CImg diagonal(const T& a1,const T& a2) { 
	  return matrix(a1,0,
			0,a2); 
	}

	//! Return a 3x3 diagonal matrix with specified coefficients
	static CImg diagonal(const T& a1,const T& a2,const T& a3) { 
	  return matrix(a1,0,0,
			0,a2,0,
			0,0,a3); 
	}

	//! Return a 4x4 diagonal matrix with specified coefficients
	static CImg diagonal(const T& a1,const T& a2,const T& a3,const T& a4) { 
	  return matrix(a1,0,0,0,
			0,a2,0,0,
			0,0,a3,0,
			0,0,0,a4); 
	}

	//! Return a 5x5 diagonal matrix with specified coefficients
	static CImg diagonal(const T& a1,const T& a2,const T& a3,const T& a4,const T& a5) { 
	  return matrix(a1,0,0,0,0,
			0,a2,0,0,0,
			0,0,a3,0,0,
			0,0,0,a4,0,
			0,0,0,0,a5);
	}

	//! Get a diagonal matrix, whose diagonal coefficients are the coefficients of the input image
	CImg get_diagonal() const {
	  if (is_empty()) return CImg<T>();
	  CImg res(size(),size(),1,1,0);
	  cimg_mapoff(*this,off) res(off,off)=(*this)(off);
	  return res;
	}

	//! Replace a vector by a diagonal matrix containing the original vector coefficients.
	CImg& diagonal() {
	  return get_diagonal().swap(*this); 
	}

	//! Return a NxN identity matrix
	static CImg identity_matrix(const unsigned int N) {      
	  CImg<T> res(N,N,1,1,0);
	  cimg_mapX(res,x) res(x,x)=1;
	  return res;
	}

	CImg& identity_matrix() {
	  return get_identity_matrix(cimg::max(width,height)).swap(*this);
	}

	CImg get_identity_matrix() const {
	  return identity_matrix(cimg::max(width,height));
	}

	//! Return a N-numbered sequence vector from \p a0 to \p a1
	CImg& sequence(const T& a0, const T& a1) {
	  const unsigned int siz = size()-1;
	  const float delta = (float)a1-a0;
	  T* ptr = data;      
	  cimg_mapoff(*this,l) *(ptr++) = (T)(a0 + delta*l/siz);
	  return *this;
	}
	
	CImg get_sequence(const T& a0, const T& a1) {
	  return CImg<T>(*this).sequence(a0,a1);
	}
	
	static CImg sequence(const unsigned N, const T& a0, const T& a1) {
	  return CImg<T>(N).sequence(a0,a1);
	}
	
	//! Return a 3x3 rotation matrix along the (x,y,z)-axis with an angle w.
	static CImg rotation_matrix(const float x, const float y, const float z, const float w, const bool quaternion_data=false) {
	  float X,Y,Z,W;      
	  if (!quaternion_data) {
	const float norm = (float)std::sqrt(x*x + y*y + z*z),
	  nx = norm>0?x/norm:0,
	  ny = norm>0?y/norm:0,
	  nz = norm>0?z/norm:1,
	  nw = norm>0?w:0,
	  sina = (float)std::sin(nw/2),
	  cosa = (float)std::cos(nw/2);
	X = nx*sina;
	Y = ny*sina;
	Z = nz*sina;
	W = cosa;
	  } else { 
	const float norm = (float)std::sqrt(x*x + y*y + z*z + w*w);
	if (norm>0) { X=x/norm; Y=y/norm; Z=z/norm; W=w/norm; }
	else { X=Y=Z=0; W=1; }
	  }
	  const float xx=X*X, xy=X*Y, xz=X*Z, xw=X*W, yy=Y*Y, yz=Y*Z, yw=Y*W, zz=Z*Z, zw=Z*W;
	  return CImg<T>::matrix(1-2*(yy+zz),   2*(xy+zw),   2*(xz-yw),
				 2*(xy-zw), 1-2*(xx+zz),   2*(yz+xw),
				 2*(xz+yw),   2*(yz-xw), 1-2*(xx+yy));
	}

	//! Return a new image corresponding to the vector located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg get_vector_at(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0) const {
	  CImg dest(1,dim);
	  cimg_mapV(*this,k) dest[k]=(*this)(x,y,z,k);
	  return dest;
	}
  
	//! Return a new image corresponding to the \a square \a matrix located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg get_matrix_at(const unsigned int x=0, const unsigned int y=0, const unsigned int z=0) const {
	  const int n = (int)std::sqrt((double)dim);
	  CImg dest(n,n);
	  cimg_mapV(*this,k) dest[k]=(*this)(x,y,z,k);
	  return dest;
	}
  
	//! Return a new image corresponding to the \a diffusion \a tensor located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg get_tensor_at(const unsigned int x=0,const unsigned int y=0,const unsigned int z=0) const {      
	  if (dim==6) return tensor((*this)(x,y,z,0),(*this)(x,y,z,1),(*this)(x,y,z,2),
					  (*this)(x,y,z,3),(*this)(x,y,z,4),(*this)(x,y,z,5));
	  if (dim==3) return tensor((*this)(x,y,z,0),(*this)(x,y,z,1),(*this)(x,y,z,2));
	  return tensor((*this)(x,y,z,0));
	}

	//! Set the image \p vec as the \a vector \a valued pixel located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg& set_vector_at(const CImg& vec,const unsigned int x=0,const unsigned int y=0,const unsigned int z=0) {
	  return draw_point(x,y,z,vec.data,1);
	}

	//! Set the image \p vec as the \a square \a matrix-valued pixel located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg& set_matrix_at(const CImg& mat,const unsigned int x=0,const unsigned int y=0,const unsigned int z=0) {
	  return set_vector_at(mat,x,y,z);
	}

	//! Set the image \p vec as the \a tensor \a valued pixel located at (\p x,\p y,\p z) of the current vector-valued image.
	CImg& set_tensor_at(const CImg& ten,const unsigned int x=0,const unsigned int y=0,const unsigned int z=0) {
	  if (ten.height==2) {
		(*this)(x,y,z,0)=ten[0];
		(*this)(x,y,z,1)=ten[1];
		(*this)(x,y,z,2)=ten[3];
	  }
	  else {
		(*this)(x,y,z,0)=ten[0];
		(*this)(x,y,z,1)=ten[1];
		(*this)(x,y,z,2)=ten[2];
		(*this)(x,y,z,3)=ten[4];
		(*this)(x,y,z,4)=ten[5];
		(*this)(x,y,z,5)=ten[8];
	  }
	  return *this;
	}

	//! Return the transpose version of the current matrix.
	CImg get_transpose() const {
	  CImg<T> res(height,width,depth,dim);
	  cimg_mapXYZV(*this,x,y,z,v) res(y,x,z,v) = (*this)(x,y,z,v);      
	  return res;
	}
	
	//! Replace the current matrix by its transpose.
	CImg& transpose() {
	  if (width==1) { width=height; height=1; return *this; }
	  if (height==1) { height=width; width=1; return *this; }
	  if (width==height) {
	cimg_mapYZV(*this,y,z,v) for (int x=y; x<(int)width; x++) cimg::swap((*this)(x,y,z,v),(*this)(y,x,z,v));
	return *this;
	  }
	  return (*this)=get_transpose();
	}

	//! Inverse the current matrix.
	CImg& inverse(const bool use_LU=true) {
	  if (!is_empty()) {
	if (width!=height || depth!=1 || dim!=1)
	  throw CImgInstanceException("CImg<%s>::inverse() : Instance matrix (%u,%u,%u,%u,%p) is not square.",
					  pixel_type(),width,height,depth,dim,data);
	const double dete = width>3?-1.0:det();
	if (dete!=0.0 && width==2) {
	  const double 
		a = data[0], c = data[1],
		b = data[2], d = data[3];
	  data[0] = (T)(d/dete);  data[1] = (T)(-c/dete);
	  data[2] = (T)(-b/dete), data[3] = (T)(a/dete); 
	} else if (dete!=0.0 && width==3) {
	  const double
		a = data[0], d = data[1], g = data[2],
		b = data[3], e = data[4], h = data[5],
		c = data[6], f = data[7], i = data[8];
	  data[0] = (T)((i*e-f*h)/dete), data[1] = (T)((g*f-i*d)/dete), data[2] = (T)((d*h-g*e)/dete);
	  data[3] = (T)((h*c-i*b)/dete), data[4] = (T)((i*a-c*g)/dete), data[5] = (T)((g*b-a*h)/dete);
	  data[6] = (T)((b*f-e*c)/dete), data[7] = (T)((d*c-a*f)/dete), data[8] = (T)((a*e-d*b)/dete);
	} else {
	  if (use_LU) { // LU-based inverse computation
		CImg<T> A(*this), indx, col(1,width);
		bool d;
		A._LU(indx,d);
		cimg_mapX(*this,j) {
		  col.fill(0); col(j)=1;
		  col._solve(A,indx);
		  cimg_mapX(*this,i) (*this)(j,i) = col(i);
		}
	  } else { // SVD-based inverse computation
		CImg<T> U(width,width),S(1,width),V(width,width);
		SVD(U,S,V,false);
		U.transpose();
		cimg_mapY(S,k) if (S[k]!=0) S[k]=1/S[k];
		S.diagonal();
		*this = V*S*U;
	  }
	}
	  }
	  return *this;
	}

	//! Return the inverse of the current matrix.
	CImg<typename cimg::largest<T,float>::type> get_inverse(const bool use_LU=true) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).inverse(use_LU); 
	}

	//! Return the pseudo-inverse (Moore-Penrose) of the matrix
	CImg<typename cimg::largest<T,float>::type> get_pseudoinverse() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  CImg<restype> At = get_transpose(), At2(At);
	  return (((At*=*this).inverse())*=At2);
	}
	
	//! Replace the matrix by its pseudo-inverse
	CImg& pseudoinverse() {
	  typedef typename cimg::largest<T,float>::type restype;
	  CImg<restype> At = get_transpose(), At2(At);
	  ((At*=*this).inverse())*=At2;
	  return ((*this)=At);
	}
	
	//! Return the trace of the current matrix.
	double trace() const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::trace() : Instance matrix (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  double res=0;
	  cimg_mapX(*this,k) res+=(*this)(k,k);
	  return res;
	}
	
	//! Return the kth smallest element of the image
	// (Adapted from the numerical recipies for CImg)
	const T kth_smallest(const unsigned int k) const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::kth_smallest() : Instance image (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  CImg<T> arr(*this);
	  unsigned long l=0,ir=size()-1;
	  for (;;) {
	if (ir<=l+1) {
	  if (ir==l+1 && arr[ir]<arr[l]) cimg::swap(arr[l],arr[ir]);
	  return arr[k];
	} else {
	  const unsigned long mid = (l+ir)>>1;
	  cimg::swap(arr[mid],arr[l+1]);
	  if (arr[l]>arr[ir]) cimg::swap(arr[l],arr[ir]);
	  if (arr[l+1]>arr[ir]) cimg::swap(arr[l+1],arr[ir]);
	  if (arr[l]>arr[l+1]) cimg::swap(arr[l],arr[l+1]);
	  unsigned long i = l+1, j = ir;
	  const T pivot = arr[l+1];
	  for (;;) {
		do i++; while (arr[i]<pivot);
		do j--; while (arr[j]>pivot);
		if (j<i) break;
		cimg::swap(arr[i],arr[j]);
	  }
	  arr[l+1] = arr[j];
	  arr[j] = pivot;
	  if (j>=k) ir=j-1;
	  if (j<=k) l=i;
	}
	  }    
	  return 0;
	}
	
	//! Return the median of the image
	const T median() const {
	  const unsigned int s = size();
	  const T res = kth_smallest(s>>1);
	  return (s%2)?res:((res+kth_smallest((s>>1)-1))/2);
	}
  
	//! Return the dot product of the current vector/matrix with the vector/matrix \p img.
	double dot(const CImg& img) const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::dot() : Instance object (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  if (img.is_empty())
	throw CImgArgumentException("CImg<%s>::trace() : Specified argument (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),img.width,img.height,img.depth,img.dim,img.data);
	  const unsigned long nb = cimg::min(size(),img.size());
	  double res=0;
	  for (unsigned long off=0; off<nb; off++) res+=data[off]*img[off];
	  return res;
	}
	
	//! Return the cross product between two 3d vectors
	CImg& cross(const CImg& img) {
	  if (width!=1 || height<3 || img.width!=1 || img.height<3)
		throw CImgInstanceException("CImg<%s>::cross() : Arguments (%u,%u,%u,%u,%p) and (%u,%u,%u,%u,%p) must be both 3d vectors.",
									pixel_type(),width,height,depth,dim,data,img.width,img.height,img.depth,img.dim,img.data);
	  const T x = (*this)[0], y = (*this)[1], z = (*this)[2];
	  (*this)[0] = y*img[2]-z*img[1];
	  (*this)[1] = z*img[0]-x*img[2];
	  (*this)[2] = x*img[1]-y*img[0];
	  return *this;
	}

	//! Return the cross product between two 3d vectors
	CImg get_cross(const CImg& img) const {
	  return CImg<T>(*this,true).cross(img); 
	}

	//! Return the determinant of the current matrix.
	double det() const {
	  if (is_empty() || width!=height || depth!=1 || dim!=1)
	throw CImgInstanceException("CImg<%s>::det() : Instance matrix (%u,%u,%u,%u,%p) is not square or is empty.",
					pixel_type(),width,height,depth,dim,data);
	  switch (width) {
	  case 1: return (*this)(0,0);
	  case 2: return (*this)(0,0)*(*this)(1,1)-(*this)(0,1)*(*this)(1,0);
	  case 3: {
	const double
	  a = data[0], d = data[1], g = data[2],
	  b = data[3], e = data[4], h = data[5],
	  c = data[6], f = data[7], i = data[8];
	return i*a*e-a*h*f-i*b*d+b*g*f+c*d*h-c*g*e;
	  }
	  default: {
	typedef typename cimg::largest<T,float>::type ftype;
	CImg<ftype> lu(*this);
	CImg<unsigned int> indx;
	bool d;
	lu._LU(indx,d);
	double res = d?1.0:-1.0;
	cimg_mapX(lu,i) res*=lu(i,i);
	return res;
	  }
	  }
	  return 0;
	}

	//! Return the norm of the current vector/matrix. \p ntype = norm type (0=L2, 1=L1, -1=Linf).
	double norm(const int ntype=2) const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::norm() : Instance object (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  double res = 0;
	  switch (ntype) {
	  case -1: {
	cimg_mapoff(*this,off) {
	  const double tmp = cimg::abs((double)data[off]);
	  if (tmp>res) res = tmp;
	}
	return res; 
	  } break;
	  case 1 : { 
	cimg_mapoff(*this,off) res+=cimg::abs((double)data[off]); 
	return res;
	  } break;
	  default: { return std::sqrt(dot(*this)); }
	  }
	  return 0;
	}

	//! Return the sum of all the pixel values in an image.
	double sum() const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::sum() : Instance object (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  double res=0;
	  cimg_map(*this,ptr,T) res+=*ptr;
	  return res;
	}

	//! Compute the SVD of a general matrix.   
	template<typename t> const CImg& SVD(CImg<t>& U, CImg<t>& S, CImg<t>& V,
					 const bool sorting=true, const unsigned int max_iter=40) const {
	  if (is_empty()) { U.empty(); S.empty(); V.empty(); }
	  else {
	U = *this;
	if (S.size()<width) S.assign(1,width);
	if (V.width<width || V.height<height) V.assign(width,width);
	CImg<t> rv1(width);  
	t anorm=0,c,f,g=0,h,s,scale=0;
	int l=0,nm=0;
	
	cimg_mapX(U,i) {
	  l = i+1; rv1[i] = scale*g; g = s = scale = 0;
	  if (i<dimy()) {
		for (int k=i; k<dimy(); k++) scale+= cimg::abs(U(i,k));
		if (scale) {
		  for (int k=i; k<dimy(); k++) { U(i,k)/=scale; s+= U(i,k)*U(i,k); }
		  f = U(i,i); g = (t)((f>=0?-1:1)*std::sqrt(s)); h=f*g-s; U(i,i)=f-g;
		  for (int j=l; j<dimx(); j++) {
		s = 0; for (int k=i; k<dimy(); k++) s+= U(i,k)*U(j,k);
		f = s/h;
		{ for (int k=i; k<dimy(); k++) U(j,k)+= f*U(i,k); }
		  }
		  { for (int k=i; k<dimy(); k++) U(i,k)*= scale; }
		}
	  }
	  S[i]=scale*g;
	  
	  g = s = scale = 0;
	  if (i<dimy() && i!=dimx()-1) {
		for (int k=l; k<dimx(); k++) scale += cimg::abs(U(k,i));
		if (scale) {
		  for (int k=l; k<dimx(); k++) { U(k,i)/= scale; s+= U(k,i)*U(k,i); }
		  f = U(l,i); g = (t)((f>=0?-1:1)*std::sqrt(s)); h = f*g-s; U(l,i) = f-g;
		  { for (int k=l; k<dimx(); k++) rv1[k]=U(k,i)/h; }
		  for (int j=l; j<dimy(); j++) {
		s=0; for (int k=l; k<dimx(); k++) s+= U(k,j)*U(k,i);
		{ for (int k=l; k<dimx(); k++) U(k,j)+= s*rv1[k]; }
		  }
		  { for (int k=l; k<dimx(); k++) U(k,i)*= scale; }
		}
	  }
	  anorm=cimg::max((t)anorm,(cimg::abs(S[i])+cimg::abs(rv1[i])));
	}
	
	{ for (int i=dimx()-1; i>=0; i--) {
	  if (i<dimx()-1) {
		if (g) {
		  { for (int j=l; j<dimx(); j++) V(i,j) =(U(j,i)/U(l,i))/g; }
		  for (int j=l; j<dimx(); j++) {
		s=0; for (int k=l; k<dimx(); k++) s+= U(k,i)*V(j,k);
		{ for (int k=l; k<dimx(); k++) V(j,k)+= s*V(i,k); }
		  }
		}
		for (int j=l; j<dimx(); j++) V(j,i)=V(i,j)=0.0;
	  }
	  V(i,i) = 1.0; g = rv1[i]; l = i;
	}
	}
	
	{ for (int i=cimg::min(dimx(),dimy())-1; i>=0; i--) {
	  l = i+1; g = S[i];
	  for (int j=l; j<dimx(); j++) U(j,i) = 0;
	  if (g) {
		g = 1/g;
		for (int j=l; j<dimx(); j++) {
		  s=0; for (int k=l; k<dimy(); k++) s+= U(i,k)*U(j,k);
		  f = (s/U(i,i))*g;
		  { for (int k=i; k<dimy(); k++) U(j,k)+= f*U(i,k); }
		}
		{ for (int j=i; j<dimy(); j++) U(i,j)*= g; }
	  } else for (int j=i; j<dimy(); j++) U(i,j)=0;
	  U(i,i)++;
	}
	}
	
	for (int k=dimx()-1; k>=0; k--) {
	  for (unsigned int its=0; its<max_iter; its++) {
		bool flag = true;
		for (l=k; l>=0; l--) {
		  nm = l-1;
		  if ((cimg::abs(rv1[l])+anorm)==anorm) { flag = false; break; }
		  if ((cimg::abs(S[nm])+anorm)==anorm) break;
		}
		if (flag) {
		  c = 0; s = 1;
		  for (int i=l; i<=k; i++) {
		f = s*rv1[i]; rv1[i] = c*rv1[i];
		if ((cimg::abs(f)+anorm)==anorm) break;
		g = S[i]; h = (t)cimg::pythagore(f,g); S[i] = h; h = 1/h; c = g*h; s = -f*h;
		cimg_mapY(U,j) { const t y = U(nm,j), z = U(i,j); U(nm,j) = y*c+z*s; U(i,j) = z*c-y*s; }
		  }
		}
		const t& z = S[k];
		if (l==k) { if (z<0) { S[k] = -z; cimg_mapX(U,j) V(k,j) = -V(k,j); } break; }
		nm = k-1; 
		t x = S[l], y = S[nm]; 
		g = rv1[nm]; h = rv1[k];
		f = ((y-z)*(y+z)+(g-h)*(g+h))/(2*h*y);
		g = (t)cimg::pythagore(f,1.0);
		f = ((x-z)*(x+z)+h*((y/(f+ (f>=0?g:-g)))-h))/x;
		c = s = 1;
		for (int j=l; j<=nm; j++) {
		  const int i = j+1;
		  g = rv1[i]; h = s*g; g = c*g;
		  t y = S[i];
		  t z = (t)cimg::pythagore(f,h); 
		  rv1[j] = z; c = f/z; s = h/z;
		  f = x*c+g*s; g = g*c-x*s; h = y*s; y*=c;
		  cimg_mapX(U,jj) { const t x = V(j,jj), z = V(i,jj); V(j,jj) = x*c+z*s; V(i,jj) = z*c-x*s; }
		  z = (t)cimg::pythagore(f,h); S[j] = z;
		  if (z) { z = 1/z; c = f*z; s = h*z; }
		  f = c*g+s*y; x = c*y-s*g;
		  { cimg_mapY(U,jj) { const t y = U(j,jj); z = U(i,jj); U(j,jj) = y*c+z*s; U(i,jj) = z*c-y*s; }}
		}
		rv1[l] = 0; rv1[k]=f; S[k]=x;
	  }
	}
	
	if (sorting) {
	  CImg<int> permutations(width);
	  CImg<t> tmp(width);
	  S.sort(permutations,false);
	  cimg_mapY(U,k) {
		cimg_mapX(permutations,x) tmp(x) = U(permutations(x),k);
		std::memcpy(U.ptr(0,k),tmp.data,sizeof(t)*width);
	  }
	  { cimg_mapY(V,k) {
		cimg_mapX(permutations,x) tmp(x) = V(permutations(x),k);
		std::memcpy(V.ptr(0,k),tmp.data,sizeof(t)*width);
	  }}	  
	}
	  }
	  return *this;
	}

	//! Compute the SVD of a general matrix.
	template<typename t> const CImg& SVD(CImgl<t>& USV) const {
	  if (USV.size<3) USV.assign(3);
	  return SVD(USV[0],USV[1],USV[2]);      
	}
	
	//! Compute the SVD of a general matrix.
	CImgl<typename cimg::largest<T,float>::type> get_SVD(const bool sorting=true) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  CImgl<restype> res(3);
	  SVD(res[0],res[1],res[2],sorting);
	  return res;
	}

	// INNER ROUTINE : Compute the LU decomposition of a permuted matrix (c.f. numerical recipies)
	template<typename t> CImg& _LU(CImg<t>& indx, bool& d) {
	  typedef typename cimg::largest<T,float>::type ftype;
	  const int N = dimx();      
	  int imax=0;
	  CImg<ftype> vv(N);
	  indx.assign(N);
	  d=true;      
	  cimg_mapX(*this,i) {
	ftype vmax=0.0;
	cimg_mapX(*this,j) {
	  const ftype tmp = cimg::abs((*this)(j,i));
	  if (tmp>vmax) vmax = tmp;
	}
	if (vmax==0) return fill(0);
	vv[i] = 1/vmax;
	  }
	  cimg_mapX(*this,j) {
	for (int i=0; i<j; i++) {
	  ftype sum=(*this)(j,i);
	  for (int k=0; k<i; k++) sum-=(*this)(k,i)*(*this)(j,k);
	  (*this)(j,i) = (T)sum;
	}
	ftype vmax=0;
	{ for (int i=j; i<dimx(); i++) {
	  ftype sum=(*this)(j,i);
	  for (int k=0; k<j; k++) sum-=(*this)(k,i)*(*this)(j,k);
	  (*this)(j,i) = (T)sum;
	  const ftype tmp = vv[i]*cimg::abs(sum);
	  if (tmp>=vmax) { vmax=tmp; imax=i; }
	}}
	if (j!=imax) {
	  cimg_mapX(*this,k) cimg::swap((*this)(k,imax),(*this)(k,j));
	  d =!d;
	  vv[imax] = vv[j];
	}
	indx[j] = (t)imax;
	if ((*this)(j,j)==0) (*this)(j,j)=(T)1e-20;
	if (j<N) {
	  const ftype tmp = 1/(ftype)(*this)(j,j);
	  for (int i=j+1; i<N; i++) (*this)(j,i)*=tmp;
	}	
	  }            
	  return *this;
	}
	
	// INNER ROUTINE : Solve a linear system, using the LU decomposition
	template<typename t> CImg& _solve(const CImg<T>& A, const CImg<t>& indx) {
	  typedef typename cimg::largest<T,float>::type ftype;
	  const int N = size();
	  int ii=-1;
	  ftype sum;     
	  for (int i=0; i<N; i++) {
	const int ip = (int)indx[i];
	ftype sum = (*this)(ip);
	(*this)(ip) = (*this)(i);
	if (ii>=0) for (int j=ii; j<=i-1; j++) sum-=A(j,i)*(*this)(j);
	else if (sum!=0) ii=i;
	(*this)(i)=sum;
	  }
	  { for (int i=N-1; i>=0; i--) {
	sum = (*this)(i);
	for (int j=i+1; j<N; j++) sum-=A(j,i)*(*this)(j);
	(*this)(i)=sum/A(i,i);
	  }}
	  return *this;
	}

	//! Solve a linear system AX=B where B=*this. (in-place version)
	CImg& solve(const CImg& A) {
	  if (width!=1 || depth!=1 || dim!=1 || height!=A.height || A.depth!=1 || A.dim!=1)
	throw CImgArgumentException("CImg<%s>::solve() : Instance matrix size is (%u,%u,%u,%u) while "
					"size of given matrix A is (%u,%u,%u,%u).",
					pixel_type(),width,height,depth,dim,A.width,A.height,A.depth,A.dim);
	  if (A.width==A.height) {
	CImg<T> lu(A);
	CImg<T> indx;
	bool d;
	lu._LU(indx,d);
	_solve(lu,indx);
	  } else assign(A.get_pseudoinverse()*(*this));
	  return *this;
	}
	
	//! Solve a linear system AX=B where B=*this.
	CImg<typename cimg::largest<T,float>::type> get_solve(const CImg& A) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImg<restype>(*this).solve(A);
	}
  
	//! Compute the eigenvalues and eigenvectors of a matrix.
	template<typename t> const CImg<T>& eigen(CImg<t>& val, CImg<t> &vec) const {
	  if (is_empty()) { val.empty(); vec.empty(); }
	  else {
	if (width!=height || depth>1 || dim>1)
	  throw CImgInstanceException("CImg<%s>::eigen() : Instance object (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),width,height,depth,dim,data);
	if (val.size()<width) val.assign(1,width);
	if (vec.size()<width*width) vec.assign(width,width);
	switch(width) {
	case 1: { val[0]=(t)(*this)[0]; vec[0]=(t)1; } break;
	case 2: {
	  const double a = (*this)[0], b = (*this)[1], c = (*this)[2], d = (*this)[3], e = a+d;
	  double f = e*e-4*(a*d-b*c);
	  cimg::warn(f<0,"CImg<%s>::eigen() : Complex eigenvalues",pixel_type());
	  f = std::sqrt(f);
	  const double l1 = 0.5*(e-f), l2 = 0.5*(e+f);
	  const double theta1 = std::atan2(l2-a,b), theta2 = std::atan2(l1-a,b);
	  val[0]=(t)l2;
	  val[1]=(t)l1;
	  vec(0,0) = (t)std::cos(theta1);
	  vec(0,1) = (t)std::sin(theta1);
	  vec(1,0) = (t)std::cos(theta2);
	  vec(1,1) = (t)std::sin(theta2);
	} break;
	default: 
	  throw CImgInstanceException("CImg<%s>::eigen() : Eigenvalues computation of general matrices is limited"
					  "to 2x2 matrices (given is %ux%u)", pixel_type(),width,height);
	}
	  }
	  return *this;
	}

	//! Return the eigenvalues and eigenvectors of a matrix.
	CImgl<typename cimg::largest<T,float>::type> get_eigen() const { 
	  typedef typename cimg::largest<T,float>::type restype;
	  CImgl<restype> res(2); 
	  eigen(res[0],res[1]);
	  return res; 
	}
	
	//! Compute the eigenvalues and eigenvectors of a matrix.
	template<typename t> const CImg<T>& eigen(CImgl<t>& eig) const {
	  if (eig.size<2) eig.assign(2);
	  eigen(eig[0],eig[1]);
	  return *this; 
	}
	
	//! Compute the eigenvalues and eigenvectors of a symmetric matrix.
	template<typename t> const CImg<T>& symmetric_eigen(CImg<t>& val, CImg<t>& vec) const {
	  if (is_empty()) { val.empty(); vec.empty(); }
	  else {
	if (width!=height || depth>1 || dim>1)
	  throw CImgInstanceException("CImg<%s>::eigen() : Instance object (%u,%u,%u,%u,%p) is empty.",
					  pixel_type(),width,height,depth,dim,data);
	
	if (val.size()<width) val.assign(1,width);
	if (vec.data && vec.size()<width*width) vec.assign(width,width);
	if (width<3) return eigen(val,vec);     
	CImg<t> V(width,width);
	SVD(vec,val,V,false);
	cimg_mapX(vec,x) {       // check for negative eigenvalues
	  t scal=0;
	  cimg_mapY(vec,y) scal+=vec(x,y)*V(x,y);
	  if (scal<0) val[x]=-val[x];
	}
	CImg<int> permutations(width);  // sort eigenvalues in decreasing order
	CImg<t> tmp(width);
	val.sort(permutations,false);
	cimg_mapY(vec,k) {
	  cimg_mapX(permutations,x) tmp(x) = vec(permutations(x),k);
	  std::memcpy(vec.ptr(0,k),tmp.data,sizeof(t)*width);
	}
	  }
	  return *this;
	}
	
	//! Compute the eigenvalues and eigenvectors of a symmetric matrix.
	CImgl<typename cimg::largest<T,float>::type> get_symmetric_eigen() const {
	  typedef typename cimg::largest<T,float>::type restype;
	  CImgl<restype> res(2);
	  symmetric_eigen(res[0],res[1]);
	  return res; 
	}

	//! Compute the eigenvalues and eigenvectors of a symmetric matrix.
	template<typename t> const CImg<T>& symmetric_eigen(CImgl<t>& eig) const {
	  if (eig.size<2) eig.assign(2);
	  symmetric_eigen(eig[0],eig[1]);
	  return *this;
	}

	template<typename t> CImg<T>& _quicksort(const int min,const int max,CImg<t>& permutations,const bool increasing) {
	  if (min<max) {      
	const int mid = (min+max)/2;
	if (increasing) {
	  if ((*this)[min]>(*this)[mid]) {
		cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	  if ((*this)[mid]>(*this)[max]) {
		cimg::swap((*this)[max],(*this)[mid]); cimg::swap(permutations[max],permutations[mid]); }
	  if ((*this)[min]>(*this)[mid]) {
		cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	} else {
	  if ((*this)[min]<(*this)[mid]) {
		cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	  if ((*this)[mid]<(*this)[max]) {
		cimg::swap((*this)[max],(*this)[mid]); cimg::swap(permutations[max],permutations[mid]); }
	  if ((*this)[min]<(*this)[mid]) {
		cimg::swap((*this)[min],(*this)[mid]); cimg::swap(permutations[min],permutations[mid]); }
	}
	if (max-min>=3) {
	  const T pivot = (*this)[mid];
	  int i = min, j = max;
	  if (increasing) {
		do {
		  while ((*this)[i]<pivot) i++;
		  while ((*this)[j]>pivot) j--;
		  if (i<=j) {
		cimg::swap((*this)[i],(*this)[j]);
		cimg::swap(permutations[i++],permutations[j--]);
		  }
		} while (i<=j);
	  } else {
		do {
		  while ((*this)[i]>pivot) i++;
		  while ((*this)[j]<pivot) j--;
		  if (i<=j) {
		cimg::swap((*this)[i],(*this)[j]);
		cimg::swap(permutations[i++],permutations[j--]);
		  }
		} while (i<=j);
	  }
	  if (min<j) _quicksort(min,j,permutations,increasing);
	  if (i<max) _quicksort(i,max,permutations,increasing);
	}
	  }
	  return *this;
	}

	//! Sort values of a vector and get permutations.
	template<typename t>
	CImg<T>& sort(CImg<t>& permutations,const bool increasing=true) {
	  if (is_empty()) permutations.empty();
	  else {
	if (permutations.size()!=size()) permutations.assign(size());
	cimg_mapoff(permutations,off) permutations[off] = off;
	_quicksort(0,size()-1,permutations,increasing); 
	  }
	  return *this;
	}

	//! Sort values of a vector.
	CImg<T>& sort(const bool increasing=true) { CImg<T> foo; return sort(foo,increasing); }

	//! Get a sorted version a of vector, with permutations.
	template<typename t> CImg<T> get_sort(CImg<t>& permutations,const bool increasing=true) {
	  return CImg<T>(*this,true).sort(permutations,increasing);
	}

	//! Get a sorted version of a vector.
	CImg<T> get_sort(const bool increasing=true) { 
	  return CImg<T>(*this,true).sort(increasing); 
	}
	
	//@}
	//---------------------------
	//
	//! \name Display functions
	//@{
	//---------------------------

  
	//! Display an image into a CImgDisplay window.
	const CImg& display(CImgDisplay& disp) const { disp.display(*this); return *this; }

	//! Display an image in a window with a title \p title, and wait a 'is_closed' or 'keyboard' event.\n
	//! Parameters \p min_size and \p max_size set the minimum and maximum dimensions of the display window.
	//! If negative, they corresponds to a percentage of the original image size.
	const CImg& display(const char* title,const int min_size=128,const int max_size=1024) const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::display() : Instance image (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);
	  CImgDisplay *disp;
	  unsigned int w = width+(depth>1?depth:0), h = height+(depth>1?depth:0), XYZ[3];
	  print(title);
	  const unsigned int dmin = cimg::min(w,h), minsiz = min_size>=0?min_size:(-min_size)*dmin/100;
	  if (dmin<minsiz) { w=w*minsiz/dmin; w+=(w==0); h=h*minsiz/dmin; h+=(h==0); }
	  const unsigned int dmax = cimg::max(w,h), maxsiz = max_size>=0?max_size:(-max_size)*dmax/100;
	  if (dmax>maxsiz) { w=w*maxsiz/dmax; w+=(w==0); h=h*maxsiz/dmax; h+=(h==0); }
	  disp = new CImgDisplay(CImg<unsigned char>(w,h,1,1,0),title,1,3);
	  XYZ[0] = width/2; XYZ[1] = height/2; XYZ[2] = depth/2;
	  while (!disp->is_closed && !disp->key) feature_selection(0,1,*disp,XYZ);
	  delete disp;
	  return *this;
	}

	//! Display an image in a window, with a default title. See also \see display() for details on parameters.
	const CImg& display(const int min_size=128,const int max_size=1024) const { return display(" ",min_size,max_size); }
  
	//! High-level interface to select features from images
	const CImg& feature_selection(int* const selection, const int feature_type, CImgDisplay &disp,
								  unsigned int *const XYZ=0,const unsigned char *const color=0) const {
	  if (is_empty())
	throw CImgInstanceException("CImg<%s>::feature_selection() : Instance image (%u,%u,%u,%u,%p) is empty.",
					pixel_type(),width,height,depth,dim,data);

	  const unsigned int old_events = disp.events, old_normalization = disp.normalization;
	  bool old_is_resized = disp.is_resized;
	  disp.events = 3; disp.normalization = 0;
	  disp.show().key = 0;

	  unsigned char fgcolor[3] = { 255,255,105 }, bgcolor[3] = { 0,0,0 };
	  if (color) std::memcpy(fgcolor,color,sizeof(unsigned char)*cimg::min(3,dimv()));

	  int area = 0, clicked_area = 0, phase = 0,
	X0 = (int)((XYZ?XYZ[0]:width/2)%width), Y0 = (int)((XYZ?XYZ[1]:height/2)%height), Z0 = (int)((XYZ?XYZ[2]:depth/2)%depth),
	X1 =-1, Y1 = -1, Z1 = -1,
		X = -1, Y = -1, Z = -1,
	oX = X, oY = Y, oZ = Z;
	  unsigned int old_button = 0, hatch = feature_type?0xF0F0F0F0:~0L;
	  bool feature_selected = false, text_down = false;
	  CImg<unsigned char> visu, visu0;
	  char text[1024] = { 0 };
	  
	  while (!disp.key && !disp.is_closed && !feature_selected) {

		// Handle mouse motion and selection
	oX = X; oY = Y; oZ = Z;
	int mx = disp.mouse_x, my = disp.mouse_y;
	const int mX = mx*(width+(depth>1?depth:0))/disp.width, mY = my*(height+(depth>1?depth:0))/disp.height;
	
	area = 0;
	if (mX<dimx() && mY<dimy())  { area = 1; X = mX; Y = mY; Z = phase?Z1:Z0; }
	if (mX<dimx() && mY>=dimy()) { area = 2; X = mX; Z = mY-height; Y = phase?Y1:Y0; }
	if (mX>=dimx() && mY<dimy()) { area = 3; Y = mY; Z = mX-width; X = phase?X1:X0; }

	if (!area) mx = my = X = Y = Z = -1;
	else {
	  if (disp.button&1 && phase<2) { X1 = X; Y1 = Y; Z1 = Z; visu0.empty(); }
	  if (!(disp.button&1) && phase>=2) {
		switch (clicked_area) {
		case 1: Z1 = Z; break;
		case 2: Y1 = Y; break;
		case 3: X1 = X; break;
		}
	  visu0.empty();
	  }
	  if (disp.button&2) { visu0.empty(); if (phase) { X1 = X; Y1 = Y; Z1 = Z; } else { X0 = X; Y0 = Y; Z0 = Z; } }
	  if (disp.button&4) { oX = X = X0; oY = Y = Y0; oZ = Z = Z0; visu0.empty(); phase = 0; }
	  if (disp.wheel) {
		switch (area) {
		case 1: if (phase) Z = (Z1+=disp.wheel); else Z = (Z0+=disp.wheel); break;
		case 2: if (phase) Y = (Y1+=disp.wheel); else Y = (Y0+=disp.wheel); break;
		case 3: if (phase) X = (X1+=disp.wheel); else X = (X0+=disp.wheel); break;
		default: break;
		}
		disp.wheel = 0;
		visu0.empty();
	  }
	  if ((disp.button&1)!=old_button) {	    
		if (phase++) { X1 = X; Y1 = Y; Z1 = Z; } else { X0 = X1 = X; Y0 = Y1 = Y; Z0 = Z1 = Z; clicked_area = area; }
		old_button = disp.button&1;
	  }
	}

	if (phase) {
	  if (!feature_type) feature_selected = phase?true:false;
	  else {
		if (depth>1) feature_selected = (phase==3)?true:false;
		else feature_selected = (phase==2)?true:false;
	  }   	  
		}

	if (X0<0) X0 = 0; if (X0>=(int)width) X0 = (int)width-1; if (Y0<0) Y0 = 0; if (Y0>=(int)height) Y0 = (int)height-1;
	if (Z0<0) Z0 = 0; if (Z0>=(int)depth) Z0 = (int)depth-1;
	if (X1<1) X1 = 0; if (X1>=(int)width) X1 = (int)width-1; if (Y1<0) Y1 = 0; if (Y1>=(int)height) Y1 = (int)height-1;
	if (Z1<0) Z1 = 0; if (Z1>=(int)depth) Z1 = (int)depth-1;

	// Draw visualization image on the display
	if (oX!=X || oY!=Y || oZ!=Z || phase || visu0.is_empty()) {
	  hatch=cimg::ror(hatch);
	  if (visu0.is_empty())
		visu0 = ((depth==1)?
			 (old_normalization?get_normalize(0,(T)255):*this):
			 (old_normalization?
			  (!phase?get_projections2d(X0,Y0,Z0):get_projections2d(X1,Y1,Z1)).normalize(0,(T)255):
			  (!phase?get_projections2d(X0,Y0,Z0):get_projections2d(X1,Y1,Z1)))).
		  get_resize(disp.width, disp.height, 1, cimg::min(3,dimv()));
	  
	  visu = visu0;

	  const int d=(depth>1)?depth:0;
	  if (phase) switch (feature_type) {
	  case 1: {
		const int
		  x0=(int)((X0+0.5f)*disp.width/(width+d)), y0=(int)((Y0+0.5f)*disp.height/(height+d)),
		  x1=(int)((X1+0.5f)*disp.width/(width+d)), y1=(int)((Y1+0.5f)*disp.height/(height+d));
		visu.draw_arrow(x0,y0,x1,y1,fgcolor,30.0f,5.0f,hatch);
		if (d) {
		  const int zx0=(int)((width+Z0+0.5f)*disp.width/(width+d)), zx1=(int)((width+Z1+0.5f)*disp.width/(width+d)),
		zy0=(int)((height+Z0+0.5f)*disp.height/(height+d)), zy1=(int)((height+Z1+0.5f)*disp.height/(height+d));
		  visu.draw_arrow(zx0,y0,zx1,y1,fgcolor,30.0f,5.0f,hatch).draw_arrow(x0,zy0,x1,zy1,fgcolor,30.0f,5.0f,hatch);
		}
	  } break;
	  case 2: {
		const int
		  x0=(X0<X1?X0:X1)*disp.width/(width+d),
		  y0=(Y0<Y1?Y0:Y1)*disp.height/(height+d),
		  x1=((X0<X1?X1:X0)+1)*disp.width/(width+d)-1,
		  y1=((Y0<Y1?Y1:Y0)+1)*disp.height/(height+d)-1;
		visu.draw_rectangle(x0,y0,x1,y1,fgcolor,0.2f).draw_line(x0,y0,x1,y0,fgcolor,hatch).
		  draw_line(x1,y0,x1,y1,fgcolor,hatch).draw_line(x1,y1,x0,y1,fgcolor,hatch).draw_line(x0,y1,x0,y0,fgcolor,hatch);
		if (d) {
		  const int
		zx0=(int)((width+(Z0<Z1?Z0:Z1))*disp.width/(width+d)),
		zy0=(int)((height+(Z0<Z1?Z0:Z1))*disp.height/(height+d)),
		zx1=(int)((width+(Z0<Z1?Z1:Z0)+1)*disp.width/(width+d))-1,
		zy1=(int)((height+(Z0<Z1?Z1:Z0)+1)*disp.height/(height+d))-1;
		  visu.draw_rectangle(zx0,y0,zx1,y1,fgcolor,0.2f).draw_line(zx0,y0,zx1,y0,fgcolor,hatch).
		draw_line(zx1,y0,zx1,y1,fgcolor,hatch).draw_line(zx1,y1,zx0,y1,fgcolor,hatch).draw_line(zx0,y1,zx0,y0,fgcolor,hatch);
		  visu.draw_rectangle(x0,zy0,x1,zy1,fgcolor,0.2f).draw_line(x0,zy0,x1,zy0,fgcolor,hatch).
		draw_line(x1,zy0,x1,zy1,fgcolor,hatch).draw_line(x1,zy1,x0,zy1,fgcolor,hatch).draw_line(x0,zy1,x0,zy0,fgcolor,hatch);
		}
	  } break;
	  case 3: {
		const int
			  x0=X0*disp.width/(width+d),
			  y0=Y0*disp.height/(height+d),
			  x1=X1*disp.width/(width+d)-1,
			  y1=Y1*disp.height/(height+d)-1;
		visu.draw_ellipse(x0,y0,(float)(x1-x0),(float)(y1-y0),1.0f,0.0f,fgcolor,0L,0.2f).
		  draw_ellipse(x0,y0,(float)(x1-x0),(float)(y1-y0),1.0f,0.0f,fgcolor,hatch);
			if (d) {
			  const int
				zx0=(int)((width+Z0)*disp.width/(width+d)),
				zy0=(int)((height+Z0)*disp.height/(height+d)),
				zx1=(int)((width+Z1+1)*disp.width/(width+d))-1,
				zy1=(int)((height+Z1+1)*disp.height/(height+d))-1;
			  visu.draw_ellipse(zx0,y0,(float)(zx1-zx0),(float)(y1-y0),1.0f,0.0f,fgcolor,0L,0.2f).
		draw_ellipse(zx0,y0,(float)(zx1-zx0),(float)(y1-y0),1.0f,0.0f,fgcolor,hatch).
		draw_ellipse(x0,zy0,(float)(x1-x0),(float)(zy1-zy0),1.0f,0.0f,fgcolor,0L,0.2f).
		draw_ellipse(x0,zy0,(float)(x1-x0),(float)(zy1-zy0),1.0f,0.0f,fgcolor,hatch);
			}
	  } break;
		  }

	  if (my<12) text_down = true;
	  if (my>=visu.dimy()-11) text_down = false;	    
	  if (!feature_type || !phase) {
		if (X>=0 && Y>=0 && Z>=0 && X<(int)width && Y<(int)height && Z<(int)depth) {
		  if (depth>1) std::sprintf(text,"Coords (%d,%d,%d)={ ",X,Y,Z); else std::sprintf(text,"Coords (%d,%d)={ ",X,Y);
		  cimg_mapV(*this,k) std::sprintf(text+cimg::strlen(text),"%g ",(double)(*this)(X,Y,Z,k));
		  std::sprintf(text+cimg::strlen(text),"}");
		}
	  } else switch (feature_type) {
	  case 1: {
		const double dX=(double)(X0-X1), dY=(double)(Y0-Y1), dZ=(double)(Z0-Z1), norm = std::sqrt(dX*dX+dY*dY+dZ*dZ);
		if (depth>1) std::sprintf(text,"Vect (%d,%d,%d)-(%d,%d,%d), norm=%g",X0,Y0,Z0,X1,Y1,Z1,norm);
		else std::sprintf(text,"Vect (%d,%d)-(%d,%d), norm=%g",X0,Y0,X1,Y1,norm);
	  } break;
	  case 2:
		if (depth>1) std::sprintf(text,"Box (%d,%d,%d)-(%d,%d,%d), Size=(%d,%d,%d)",
					  X0<X1?X0:X1,Y0<Y1?Y0:Y1,Z0<Z1?Z0:Z1,
					  X0<X1?X1:X0,Y0<Y1?Y1:Y0,Z0<Z1?Z1:Z0,
					  1+cimg::abs(X0-X1),1+cimg::abs(Y0-Y1),1+cimg::abs(Z0-Z1));
		else  std::sprintf(text,"Box (%d,%d)-(%d,%d), Size=(%d,%d)",
				   X0<X1?X0:X1,Y0<Y1?Y0:Y1,X0<X1?X1:X0,Y0<Y1?Y1:Y0,1+cimg::abs(X0-X1),1+cimg::abs(Y0-Y1));
		break;
	  default:
		if (depth>1) std::sprintf(text,"Ellipse (%d,%d,%d)-(%d,%d,%d), Radii=(%d,%d,%d)",
					  X0,Y0,Z0,X1,Y1,Z1,1+cimg::abs(X0-X1),1+cimg::abs(Y0-Y1),1+cimg::abs(Z0-Z1));
		else  std::sprintf(text,"Ellipse (%d,%d)-(%d,%d), Radii=(%d,%d)",
				   X0,Y0,X1,Y1,1+cimg::abs(X0-X1),1+cimg::abs(Y0-Y1));
		
		break;
	  }	    
	  if (phase || (mx>=0 && my>=0)) visu.draw_text(text,0,text_down?visu.dimy()-11:0,fgcolor,bgcolor,11,0.7f);
		
	  disp.display(visu).wait(25);
	} else disp.wait();
	
	if (disp.is_resized) { disp.resize(false); old_is_resized = true; visu0.empty(); }
	  }	

	  // Return result
	  if (XYZ) { XYZ[0] = (unsigned int)X; XYZ[1] = (unsigned int)Y; XYZ[2] = (unsigned int)Z; }
	  if (feature_selected) {
		if (feature_type==2) {
		  if (X0>X1) cimg::swap(X0,X1);
		  if (Y0>Y1) cimg::swap(Y0,Y1);
		  if (Z0>Z1) cimg::swap(Z0,Z1);
		}
		if (selection) {
		  if (X1<0 || Y1<0 || Z1<0) X0=Y0=Z0=X1=Y1=Z1=-1;
		  switch(feature_type) {
		  case 1:
		  case 2:  selection[3] = X1; selection[4] = Y1; selection[5] = Z1;
		  default: selection[0] = X0; selection[1] = Y0; selection[2] = Z0;
		  }
		}
	  } else if (selection) selection[0]=selection[1]=selection[2]=selection[3]=selection[4]=selection[5]=-1;
	  disp.button = 0;
	  disp.events = old_events;
	  disp.normalization = old_normalization;
	  disp.is_resized = old_is_resized;
	  return *this;
	}

	//! High-level interface to select features in images
	const CImg& feature_selection(int *const selection, const int feature_type,
								  unsigned int *const XYZ=0,const unsigned char *const color=0) const {
	  unsigned int w = width + (depth>1?depth:0), h = height + (depth>1?depth:0);
	  const unsigned int dmin = cimg::min(w,h), minsiz = 256;
	  if (dmin<minsiz) { w=w*minsiz/dmin; h=h*minsiz/dmin; }
	  const unsigned int dmax = cimg::max(w,h), maxsiz = 1024;
	  if (dmax>maxsiz) { w=w*maxsiz/dmax; h=h*maxsiz/dmax; }
	  CImgDisplay disp(w,h," ",0,3);
	  return feature_selection(selection,feature_type,disp,XYZ,color);
	}
  
	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf, typename to>
	const CImg& display_object3d(const CImg<tp>& points,const CImgl<tf>& primitives,
				 const CImgl<T>& colors, const CImg<to>& opacities, CImgDisplay& disp,			 
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const bool display_axes=true, float *const pose_matrix = 0) const {

	  // Check input arguments      
	  if (points.is_empty())
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given points are empty.",
					pixel_type());
	  if (primitives.is_empty())
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given primitives are empty.",
					pixel_type());
	  if (is_empty())
	return CImg<T>(disp.width,disp.height,1,colors[0].size(),0).
	  display_object3d(points,primitives,colors,opacities,disp,centering,
			   render_static,render_motion,double_sided,focale,ambiant_light);
	  if (opacities.is_empty())
	return display_object3d(points,primitives,colors,CImg<to>(primitives.size,1,1,1,(to)1),disp,
				centering,render_static,render_motion,double_sided,focale,ambiant_light);
	  if (points.height<3)
	return display_object3d(points.get_resize(-100,3,1,1,0),primitives,colors,opacities,disp,
				centering,render_static,render_motion,double_sided,focale,ambiant_light);
	  if (colors.size!=primitives.size)
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given colors (size=%u) and primitives (size=%u) have "
					"different sizes.",pixel_type(),colors.size,primitives.size);
	  if (opacities.width!=primitives.size)
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given opacities (size=%u) and primitives (size=%u) have "
					"different sizes", pixel_type(),opacities.width,primitives.size);

	  // Init 3D objects and compute object statistics
	  CImg<float> pose,
	centered_points = centering?CImg<float>(points.width,3):CImg<float>(),
	rotated_points(points.width,3),
	bbox_points, rotated_bbox_points, bbox_opacities,
	axes_points, rotated_axes_points, axes_opacities;
	  CImgl<T> bbox_colors, axes_colors;
	  CImgl<tf> bbox_primitives, axes_primitives;
	  float dx=0, dy=0, dz=0, ratio=1;
	  const T valmax = cimg::type<T>::max();

	  const CImgStats 
	sx(points.get_shared_line(0),false),
	sy(points.get_shared_line(1),false),
	sz(points.get_shared_line(2),false);
	  const float
	xm = (float)sx.min, xM = (float)sx.max,
	ym = (float)sy.min, yM = (float)sy.max,
	zm = (float)sz.min, zM = (float)sz.max,
	delta = cimg::max(xM-xm,yM-ym,zM-zm);

	  if (display_axes) {
	axes_points.assign(7,3);
	rotated_axes_points.assign(7,3);
	axes_opacities.assign(3,1,1,1,1.0f);
	axes_colors.assign(3,dim,1,1,1,valmax);
	axes_points(0,0) = 0; axes_points(0,1) = 0; axes_points(0,2) = 0;
	axes_points(1,0) = 20; axes_points(1,1) = 0; axes_points(1,2) = 0;
	axes_points(2,0) = 0; axes_points(2,1) = 20; axes_points(2,2) = 0;
	axes_points(3,0) = 0; axes_points(3,1) = 0; axes_points(3,2) = 20;
	axes_points(4,0) = 22; axes_points(4,1) = -6; axes_points(4,2) = 0;
	axes_points(5,0) = -6; axes_points(5,1) = 22; axes_points(5,2) = 0;
	axes_points(6,0) = -6; axes_points(6,1) = -6; axes_points(6,2) = 22;
	axes_primitives.insert(CImg<tf>::vector(0,1));
	axes_primitives.insert(CImg<tf>::vector(0,2));
	axes_primitives.insert(CImg<tf>::vector(0,3));
	  }

	  // Begin user interaction loop
	  CImg<T> visu0(*this), visu;
	  bool init = true, clicked = false, redraw = true, stopflag = false;
	  int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	  const unsigned int old_events = disp.events;
	  disp.show().button = disp.key = 0;
	  disp.events = 3;

	  while (!disp.is_closed && !stopflag) {
	
	// Init object position and scale if necessary
	if (init) {
	  ratio = delta>0?(2.0f*cimg::min(disp.width,disp.height)/(3.0f*delta)):0;
	  dx = 0.5f*(xM+xm); dy = 0.5f*(yM+ym); dz = 0.5f*(zM+zm);
	  if (centering) {
		cimg_mapX(centered_points,l) {
		  centered_points(l,0) = (float)((points(l,0)-dx)*ratio);
		  centered_points(l,1) = (float)((points(l,1)-dy)*ratio);
		  centered_points(l,2) = (float)((points(l,2)-dz)*ratio);
		}
	  }
	  
	  if (render_static<0 || render_motion<0) {
		bbox_colors.assign(12,dim,1,1,1,valmax);
		bbox_primitives.assign(12,1,2);
		bbox_points.assign(8,3);
		rotated_bbox_points.assign(8,3);
		bbox_points(0,0) = xm; bbox_points(0,1) = ym; bbox_points(0,2) = zm;
		bbox_points(1,0) = xM; bbox_points(1,1) = ym; bbox_points(1,2) = zm;
		bbox_points(2,0) = xM; bbox_points(2,1) = yM; bbox_points(2,2) = zm;
		bbox_points(3,0) = xm; bbox_points(3,1) = yM; bbox_points(3,2) = zm;
		bbox_points(4,0) = xm; bbox_points(4,1) = ym; bbox_points(4,2) = zM;
		bbox_points(5,0) = xM; bbox_points(5,1) = ym; bbox_points(5,2) = zM;
		bbox_points(6,0) = xM; bbox_points(6,1) = yM; bbox_points(6,2) = zM;
		bbox_points(7,0) = xm; bbox_points(7,1) = yM; bbox_points(7,2) = zM;
		bbox_primitives[0].fill(0,1); bbox_primitives[1].fill(1,2); bbox_primitives[2].fill(2,3);  bbox_primitives[3].fill(3,0);
		bbox_primitives[4].fill(4,5); bbox_primitives[5].fill(5,6); bbox_primitives[6].fill(6,7);  bbox_primitives[7].fill(7,4);
		bbox_primitives[8].fill(0,4); bbox_primitives[9].fill(1,5); bbox_primitives[10].fill(2,6); bbox_primitives[11].fill(3,7);
		bbox_opacities.assign(bbox_primitives.size,1,1,1,1.0f);
	  }
	  
	  if (pose_matrix) pose = CImg<float>(pose_matrix,4,4,1,1); else pose = CImg<float>::identity_matrix(4);
	  init = false;
	  redraw = true;
	}

		// Rotate and Draw 3D object
	if (redraw) {
	  const float 
		r00 = pose(0,0), r10 = pose(1,0), r20 = pose(2,0), r30 = pose(3,0),
		r01 = pose(0,1), r11 = pose(1,1), r21 = pose(2,1), r31 = pose(3,1),
		r02 = pose(0,2), r12 = pose(1,2), r22 = pose(2,2), r32 = pose(3,2);
	  if ((clicked && render_motion>=0) || (!clicked && render_static>=0)) {
		if (centering) cimg_mapX(centered_points,l) {
		  const float x = centered_points(l,0), y = centered_points(l,1), z = centered_points(l,2);
		  rotated_points(l,0) = r00*x + r10*y + r20*z + r30;
		  rotated_points(l,1) = r01*x + r11*y + r21*z + r31;
		  rotated_points(l,2) = r02*x + r12*y + r22*z + r32;
		} else cimg_mapX(points,l) {
		  const float x = (float)points(l,0), y = (float)points(l,1), z = (float)points(l,2);
		  rotated_points(l,0) = r00*x + r10*y + r20*z + r30;
		  rotated_points(l,1) = r01*x + r11*y + r21*z + r31;
		  rotated_points(l,2) = r02*x + r12*y + r22*z + r32;
		}
	  } else {
		if (!centering) cimg_mapX(bbox_points,l) {
		  const float x = bbox_points(l,0),	y = bbox_points(l,1), z = bbox_points(l,2);
		  rotated_bbox_points(l,0) = r00*x + r10*y + r20*z + r30;
		  rotated_bbox_points(l,1) = r01*x + r11*y + r21*z + r31;
		  rotated_bbox_points(l,2) = r02*x + r12*y + r22*z + r32;
		} else cimg_mapX(bbox_points,l) {
		  const float x = (bbox_points(l,0)-dx)*ratio, y = (bbox_points(l,1)-dy)*ratio, z = (bbox_points(l,2)-dz)*ratio;
		  rotated_bbox_points(l,0) = r00*x + r10*y + r20*z + r30;
		  rotated_bbox_points(l,1) = r01*x + r11*y + r21*z + r31;
		  rotated_bbox_points(l,2) = r02*x + r12*y + r22*z + r32;
		}	    
	  }
	  
	  // Draw object
	  visu = visu0;
	  if ((clicked && render_motion<0) || (!clicked && render_static<0))
		visu.draw_object3d(visu.width/2.0f, visu.height/2.0f, 0,
				   rotated_bbox_points,bbox_primitives,bbox_colors,bbox_opacities,1,
				   false,focale,visu.dimx()/2.0f,visu.dimy()/2.0f,-5000.0f,0.2f);
	  else visu.draw_object3d(visu.width/2.0f, visu.height/2.0f, 0,
				  rotated_points,primitives,colors,opacities,clicked?render_motion:render_static,
				  double_sided,focale,visu.dimx()/2.0f,visu.dimy()/2.0f,-5000.0f,ambiant_light);

	  // Draw axes
	  if (display_axes) {
		const float Xaxes = 25.0f, Yaxes = visu.height-35.0f;
		cimg_mapX(axes_points,l) {
		  const float x = axes_points(l,0), y = axes_points(l,1), z = axes_points(l,2);
		  rotated_axes_points(l,0) = r00*x + r10*y + r20*z;
		  rotated_axes_points(l,1) = r01*x + r11*y + r21*z;
		  rotated_axes_points(l,2) = r02*x + r12*y + r22*z;
		}
		axes_opacities(0) = (rotated_axes_points(1,2)>0)?0.5f:1.0f;
		axes_opacities(1) = (rotated_axes_points(2,2)>0)?0.5f:1.0f;
		axes_opacities(2) = (rotated_axes_points(3,2)>0)?0.5f:1.0f;
		visu.draw_object3d(Xaxes, Yaxes, 0, rotated_axes_points,axes_primitives,axes_colors,axes_opacities,1,false,focale,0,0,0,0).
		  draw_text("X",(int)(Xaxes+rotated_axes_points(4,0)), (int)(Yaxes+rotated_axes_points(4,1)), axes_colors[0].ptr(), 0, 11, axes_opacities(0)).
		  draw_text("Y",(int)(Xaxes+rotated_axes_points(5,0)), (int)(Yaxes+rotated_axes_points(5,1)), axes_colors[1].ptr(), 0, 11, axes_opacities(1)).
		  draw_text("Z",(int)(Xaxes+rotated_axes_points(6,0)), (int)(Yaxes+rotated_axes_points(6,1)), axes_colors[2].ptr(), 0, 11, axes_opacities(2));
	  }
	  
	  visu.display(disp);
	  if (!clicked || render_motion==render_static) redraw = false;
	}
	  
	// Handle user interaction
	if ((disp.button || disp.wheel) && disp.mouse_x>=0 && disp.mouse_y>=0) {
	  redraw = true;
	  if (!clicked) { x0 = x1 = disp.mouse_x; y0 = y1 = disp.mouse_y; clicked = true; }
	  else { x1 = disp.mouse_x; y1 = disp.mouse_y; }
	  if (disp.button&1) {
		const float
		  R = 0.4f*cimg::min(disp.width,disp.height),
		  R2 = R*R,
		  u0 = (float)(x0-disp.dimx()/2),
		  v0 = (float)(y0-disp.dimy()/2),
		  u1 = (float)(x1-disp.dimx()/2),
		  v1 = (float)(y1-disp.dimy()/2),
		  n0 = (float)std::sqrt(u0*u0+v0*v0),
		  n1 = (float)std::sqrt(u1*u1+v1*v1),
		  nu0 = n0>R?(u0*R/n0):u0,
		  nv0 = n0>R?(v0*R/n0):v0,
		  nw0 = (float)std::sqrt(cimg::max(0.0f,R2-nu0*nu0-nv0*nv0)),
		  nu1 = n1>R?(u1*R/n1):u1,
		  nv1 = n1>R?(v1*R/n1):v1,
		  nw1 = (float)std::sqrt(cimg::max(0.0f,R2-nu1*nu1-nv1*nv1)),
		  u = nv0*nw1-nw0*nv1,					
		  v = nw0*nu1-nu0*nw1,
		  w = nv0*nu1-nu0*nv1,
		  n = (float)std::sqrt(u*u+v*v+w*w),
		  alpha = (float)std::asin(n/R2);
		pose.draw_image(CImg<float>::rotation_matrix(u,v,w,alpha)*pose.get_crop(0,0,2,2),0,0);
		x0=x1; y0=y1;
	  }      
	  if (disp.button&2) { pose(3,2)+=(y1-y0); x0=x1; y0=y1; }
	  if (disp.wheel) { pose(3,2)-=15*disp.wheel; disp.wheel=0; }
	  if (disp.button&4) { pose(3,0)+=(x1-x0); pose(3,1)+=(y1-y0); x0=x1; y0=y1; }
	  if ((disp.button&1) && (disp.button&2)) { init = true; disp.button = 0; x0 = x1; y0 = y1; pose = CImg<float>::identity_matrix(4); }
	} else if (clicked) { x0=x1; y0=y1; clicked = false; redraw = true; }
	if (disp.key) { redraw = false; stopflag = true; }
	if (disp.is_resized) { disp.resize(false); visu0 = get_resize(disp,1); redraw = true; }	
	  }      
	  if (pose_matrix) std::memcpy(pose_matrix,pose.data,16*sizeof(float));      
	  disp.events = old_events;
	  disp.button = 0;
	  return *this;
	}

	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf, typename to>
	const CImg& display_object3d(const CImg<tp>& points, const CImgl<tf>& primitives,
				 const CImgl<T>& colors, const CImgl<to>& opacities, CImgDisplay& disp,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const bool display_axes=true, float *const pose_matrix = 0) const {
	  if (opacities.is_empty())
	return display_object3d(points,primitives,colors,CImg<to>(),disp,centering,
				render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);
	  CImg<to> nopacities(opacities.size);
	  to *ptrd = nopacities.ptr();
	  cimg_mapoff(nopacities,l) if (opacities(l).size()) *(ptrd++) = opacities(l,0);
	  else 
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given opacities (size=%u) contains a null element at "
					"position %u.",pixel_type(),opacities.size,l);
	  return display_object3d(points,primitives,colors,nopacities,disp,centering,
				  render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);

	}

	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf, typename to>
	const CImg& display_object3d(const CImgl<tp>& points,const CImgl<tf>& primitives,
				 const CImgl<T>& colors, const CImg<to>& opacities, CImgDisplay& disp,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const bool display_axes=true, float *const pose_matrix = 0) const {
	  if (points.is_empty())
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given points are empty.",
					pixel_type());
	  CImg<tp> npoints(points.size,3,1,1,0);
	  tp *ptrX = npoints.ptr(), *ptrY = npoints.ptr(0,1), *ptrZ = npoints.ptr(0,2);
	  cimg_mapX(npoints,l) {
	const CImg<tp>& point = points[l];
	const unsigned int siz = point.size();
	if (!siz)
	  throw CImgArgumentException("CImg<%s>::display_object3d() : Given points (size=%u) contains a null element at "
					  "position %u.",pixel_type(),points.size,l);
	*(ptrZ++) = (siz>2)?point(2):0;
	*(ptrY++) = (siz>1)?point(1):0;
	*(ptrX++) = point(0);
	  }
	  return display_object3d(npoints,primitives,colors,opacities,disp,centering,
				  render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);
	}

	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf, typename to>
	const CImg& display_object3d(const CImgl<tp>& points,const CImgl<tf>& primitives,
				 const CImgl<T>& colors, const CImgl<to>& opacities, CImgDisplay &disp,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const bool display_axes=true, float *const pose_matrix = 0) const {
	  if (opacities.is_empty())
	return display_object3d(points,primitives,colors,CImg<to>(),disp,centering,
				render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);
	  CImg<to> nopacities(opacities.size);
	  to *ptrd = nopacities.ptr();
	  cimg_mapoff(nopacities,l) if (opacities(l).size()) *(ptrd++) = opacities(l,0);
	  else 
	throw CImgArgumentException("CImg<%s>::display_object3d() : Given opacities (size=%u) contains a null element at "
					"position %u.",pixel_type(),opacities.size,l);
	  return display_object3d(points,primitives,colors,nopacities,disp,centering,
				  render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);
	}
	
	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf, typename to>
	const CImg& display_object3d(const tp& points, const CImgl<tf>& primitives,
				 const CImgl<T> colors, const to& opacities,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const bool display_axes=true, float *const pose_matrix = 0) const {
	  CImgDisplay disp(width,height," ",0);
	  return display_object3d(points,primitives,colors,opacities,disp,centering,
				  render_static,render_motion,double_sided,focale,ambiant_light,display_axes,pose_matrix);
	}

	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf>
	const CImg& display_object3d(const tp& points, const CImgl<tf>& primitives,
				 const CImgl<T> colors,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const float opacity=1.0f, const bool display_axes=true, float *const pose_matrix = 0) const {
	  CImgDisplay disp(width,height," ",0);
	  return display_object3d(points,primitives,colors,CImg<float>(primitives.size,1,1,1,opacity),
				  disp,centering,render_static,render_motion,double_sided,
				  focale,ambiant_light,display_axes,pose_matrix);
	}

	//! High-level interface for displaying a 3d object
	template<typename tp, typename tf>
	const CImg& display_object3d(const tp& points, const CImgl<tf>& primitives,
				 const CImgl<T> colors, CImgDisplay &disp,
				 const bool centering=true,
				 const int render_static=4, const int render_motion=1,
				 const bool double_sided=false,
				 const float focale=500.0f, const float ambiant_light=0.05f,
				 const float opacity=1.0f, const bool display_axes=true, float *const pose_matrix = 0) const {
	  return display_object3d(points,primitives,colors,CImg<float>(primitives.size,1,1,1,opacity),
				  disp,centering,render_static,render_motion,double_sided,
				  focale,ambiant_light,display_axes,pose_matrix);
	}
	
	//@}
	//--------------------------------
	//
	//! \name Input-Output functions
	//@{
	//--------------------------------

	//! Load an image from a file.
	/**
	   \param filename = name of the image file to load.
	   \return A CImg<T> instance containing the pixel data defined in the image file.
	   \note The extension of \c filename defines the file format. If no filename
	   extension is provided, CImg<T>::get_load() will try to load a CRAW file (CImg Raw file).
	**/
	static CImg get_load(const char *filename) {
	  const char *ext = cimg::filename_split(filename);
	  if (!cimg::strcasecmp(ext,"asc")) return get_load_ascii(filename);
	  if (!cimg::strcasecmp(ext,"dlm")) return get_load_dlm(filename);
	  if (!cimg::strcasecmp(ext,"inr")) return get_load_inr(filename);
	  if (!cimg::strcasecmp(ext,"hdr")) return get_load_analyze(filename);
	  if (!cimg::strcasecmp(ext,"par") ||
	  !cimg::strcasecmp(ext,"rec")) return get_load_parrec(filename);
	  if (!cimg::strcasecmp(ext,"pan")) return get_load_pandore(filename);
	  if (!cimg::strcasecmp(ext,"bmp")) return get_load_bmp(filename);
	  if (!cimg::strcasecmp(ext,"png")) return get_load_png(filename);
	  if (!cimg::strcasecmp(ext,"jpg") ||
	  !cimg::strcasecmp(ext,"jpeg")) return get_load_jpeg(filename);
	  if (!cimg::strcasecmp(ext,"ppm") || 
	  !cimg::strcasecmp(ext,"pgm") ||
	  !cimg::strcasecmp(ext,"pnm")) return get_load_pnm(filename);
	  if (!cimg::strcasecmp(ext,"cimg") || ext[0]=='\0') return get_load_cimg(filename);
	  if (!cimg::strcasecmp(ext,"dcm") ||
	  !cimg::strcasecmp(ext,"dicom")) return get_load_dicom(filename);
#ifdef cimg_use_magick
	  return get_load_magick(filename);
#else
	  return get_load_convert(filename);
#endif
	}

	//! Load an image from a file
	/** This is the in-place version of get_load(). **/
	CImg& load(const char *filename) { 
	  return get_load(filename).swap(*this); 
	}

	//! Load an image from an ASCII file.
	static CImg get_load_ascii(const char *filename) {
	  std::FILE *file = cimg::fopen(filename,"rb");
	  char line[256] = {0};
	  std::fscanf(file,"%255[^\n]",line);
	  unsigned int off;
	  int err=1, dx=0, dy=1, dz=1, dv=1;
	  std::sscanf(line,"%d %d %d %d",&dx,&dy,&dz,&dv);
	  if (!dx || !dy || !dz || !dv)
	throw CImgIOException("CImg<%s>::get_load_ascii() : File '%s' does not appear to be a valid ASC file.\n"
				  "Specified image dimensions are (%d,%d,%d,%d)",pixel_type(),filename,dx,dy,dz,dv);
	  CImg dest(dx,dy,dz,dv);
	  double val;
	  T *ptr = dest.data;
	  for (off=0; off<dest.size() && err==1; off++) {
	err = std::fscanf(file,"%lf%*[^0-9.eE+-]",&val); 
	*(ptr++)=(T)val; 
	  }
	  cimg::warn(off<dest.size(),"CImg<%s>::get_load_ascii() : File '%s', only %u values read, instead of %u",
		 pixel_type(),filename,off,dest.size());
	  cimg::fclose(file);
	  return dest;
	}
	
	//! Load an image from an ASCII file (in-place version).
	/** This is the in-place version of get_load_ascii(). **/
	CImg& load_ascii(const char *filename) {
	  return get_load_ascii(filename).swap(*this); 
	}

	//! Load an image from a DLM file
	static CImg get_load_dlm(const char *filename) {
	  std::FILE *file = cimg::fopen(filename,"r");
	  CImg<T> dest(256,256);
	  unsigned int cdx=0,dx=0,dy=0;
	  double val;
	  char c, delimiter[256]={0}, tmp[256];
	  int oerr=0, err;
	  while ((err = std::fscanf(file,"%lf%255[^0-9.eE+-]",&val,delimiter))!=EOF) {
	oerr = err;
	if (err>0) dest(cdx++,dy) = (T)val;
	if (cdx>=dest.width) dest.resize(dest.width+256,1,1,1,0);
	c=0; if (!std::sscanf(delimiter,"%255[^\n]%c",tmp,&c) || c=='\n') { 
	  dx = cimg::max(cdx,dx);
	  dy++;
	  if (dy>=dest.height) dest.resize(dest.width,dest.height+256,1,1,0);
	  cdx=0; 
	}
	  }
	  if (cdx && oerr==1) { dx=cdx; dy++; }
	  if (!dx || !dy) throw CImgIOException("CImg<%s>::get_load_dlm() : File '%s' does not appear to be a "
						"valid DLM file (width = %d, height = %d)\n",pixel_type(),filename,dx,dy);
	  dest.resize(dx,dy,1,1,0);
	  cimg::fclose(file);
	  return dest;
	}

	//! Load an image from a DLM file (in-place version).
	/** This is the in-place version of get_load_dlm(). **/
	CImg& load_dlm(const char *filename) { 
	  return get_load_dlm(filename).swap(*this); 
	}

	//! Load an image from a PNM file
	static CImg get_load_pnm(const char *filename) {
	  std::FILE *file=cimg::fopen(filename,"rb");
	  char item[1024]={0};
	  unsigned int ppm_type,width,height,colormax=255;
	  int err;
	  
	  while ((err=std::fscanf(file,"%1023[^\n]",item))!=EOF && (item[0]=='#' || !err)) std::fgetc(file);
	  if(std::sscanf(item," P%u",&ppm_type)!=1) 
		throw CImgIOException("CImg<%s>::get_load_pnm() : file '%s',PPM header 'P?' not found",pixel_type(),filename);
	  while ((err=std::fscanf(file," %1023[^\n]",item))!=EOF && (item[0]=='#' || !err)) std::fgetc(file);
	  if ((err=std::sscanf(item," %u %u %u",&width,&height,&colormax))<2)
		throw CImgIOException("CImg<%s>::get_load_pnm() : file '%s',WIDTH and HEIGHT not defined",pixel_type(),filename);
	  if (err==2) {
	while ((err=std::fscanf(file," %1023[^\n]",item))!=EOF && (item[0]=='#' || !err)) std::fgetc(file);
	cimg::warn(std::sscanf(item,"%u",&colormax)!=1,
		   "CImg<%s>::get_load_pnm() : file '%s',COLORMAX not defined",pixel_type(),filename);
	  }
	  std::fgetc(file);
	  
	  CImg dest;
	  int rval,gval,bval;

	  switch (ppm_type) {
	  case 2: { // Grey Ascii
	dest.assign(width,height,1,1);
	T* rdata = dest.ptr();
	cimg_mapoff(dest,off) { std::fscanf(file,"%d",&rval); *(rdata++)=(T)rval; }
	  } break;
	  case 3: { // Color Ascii
	dest.assign(width,height,1,3);
	T *rdata = dest.ptr(0,0,0,0), *gdata = dest.ptr(0,0,0,1), *bdata = dest.ptr(0,0,0,2);
	cimg_mapXY(dest,x,y) { 
	  std::fscanf(file,"%d %d %d",&rval,&gval,&bval);
	  *(rdata++)=(T)rval; 
	  *(gdata++)=(T)gval; 
	  *(bdata++)=(T)bval; }
	  } break;
	  case 5: { // Grey Binary
	if (colormax<256) { // 8 bits
	  CImg<unsigned char> raw(width,height,1,1);
	  cimg::fread(raw.data,width*height,file);
	  dest=raw;
	} else { // 16 bits
	  CImg<unsigned short> raw(width,height,1,1);
	  cimg::fread(raw.data,width*height,file);
	  if (!cimg::endian()) cimg::endian_swap(raw.data,width*height);
	  dest=raw;
	}
	  } break;
	  case 6: { // Color Binary
	if (colormax<256) { // 8 bits
	  CImg<unsigned char> raw(width,height,1,3);
	  cimg::fread(raw.data,width*height*3,file);
	  dest.assign(width,height,1,3);
	  T *rdata = dest.ptr(0,0,0,0), *gdata = dest.ptr(0,0,0,1), *bdata = dest.ptr(0,0,0,2);
	  const unsigned char *ptrs = raw.ptr();
	  for (unsigned int off = raw.width*raw.height; off; --off) {
		*(rdata++) = (T)(*(ptrs++));
		*(gdata++) = (T)(*(ptrs++));
		*(bdata++) = (T)(*(ptrs++));
	  }
	} else { // 16 bits
	  CImg<unsigned short> raw(width,height,1,3);
	  cimg::fread(raw.data,width*height*3,file);
	  if (!cimg::endian()) cimg::endian_swap(raw.data,width*height*3);
	  dest.assign(width,height,1,3);
	  T *rdata = dest.ptr(0,0,0,0), *gdata = dest.ptr(0,0,0,1), *bdata = dest.ptr(0,0,0,2);
	  const unsigned short *ptrs = raw.ptr();
	  for (unsigned int off = raw.width*raw.height; off; --off) {
		*(rdata++) = (T)(*(ptrs++));
		*(gdata++) = (T)(*(ptrs++));
		*(bdata++) = (T)(*(ptrs++));
	  }
	}
	  } break;
	  default:
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::get_load_pnm() : file '%s', PPM type 'P%d' not supported",pixel_type(),filename,ppm_type);
	  }
	  cimg::fclose(file);
	  return dest;
	}

	//! Load an image from a PNM file (in-place version).
	CImg& load_pnm(const char *filename) { 
	  return get_load_pnm(filename).swap(*this); 
	}
	
	//! Load a YUV image sequence file.
	static CImg get_load_yuv(const char *filename,
			 const unsigned int sizex, const unsigned int sizey, 
			 const unsigned int first_frame=0, const int last_frame=-1,
			 const bool yuv2rgb = true) {
	  return CImgl<T>::get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).get_append('z','c');
	}

	//! Load a YUV image sequence file (in-place).
	CImg& load_yuv(const char *filename,
		   const unsigned int sizex, const unsigned int sizey, 
		   const unsigned int first_frame=0, const int last_frame=-1,
		   const bool yuv2rgb = true) { 
	  return get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).swap(*this); 
	}
	
	//! Load an image from a BMP file.
	static CImg get_load_bmp(const char *filename) {
	  unsigned char header[64];
	  std::FILE *file = cimg::fopen(filename,"rb");
	  cimg::fread(header,54,file);
	  if (header[0]!='B' || header[1]!='M')
	throw CImgIOException("CImg<%s>::get_load_bmp() : filename '%s' does not appear to be a valid BMP file",
				  pixel_type(),filename);
	  
	  // Read header and pixel buffer
	  int
	file_size   = header[0x02] + (header[0x03]<<8) + (header[0x04]<<16) + (header[0x05]<<24),
	offset      = header[0x0A] + (header[0x0B]<<8) + (header[0x0C]<<16) + (header[0x0D]<<24),
	dx          = header[0x12] + (header[0x13]<<8) + (header[0x14]<<16) + (header[0x15]<<24),
	dy          = header[0x16] + (header[0x17]<<8) + (header[0x18]<<16) + (header[0x19]<<24),
	compression = header[0x1E] + (header[0x1F]<<8) + (header[0x20]<<16) + (header[0x21]<<24),
	nb_colors   = header[0x2E] + (header[0x2F]<<8) + (header[0x30]<<16) + (header[0x31]<<24),
	bpp         = header[0x1C] + (header[0x1D]<<8),
	*palette    = 0;
	  const int 
	dx_bytes   = (bpp==1)?(dx/8+(dx%8?1:0)):((bpp==4)?(dx/2+(dx%2?1:0)):(dx*bpp/8)),
	align      = (4-dx_bytes%4)%4,
	buf_size   = cimg::min(cimg::abs(dy)*(dx_bytes+align),file_size-offset);

	  if (bpp<16) { if (!nb_colors) nb_colors=1<<bpp; } else nb_colors=0;
	  if (nb_colors) { palette = new int[nb_colors]; cimg::fread(palette,nb_colors,file); }
	  const int	xoffset = offset-54-4*nb_colors;      
	  if (xoffset>0) std::fseek(file,xoffset,SEEK_CUR);
	  unsigned char *buffer  = new unsigned char[buf_size], *ptrs = buffer;
	  cimg::fread(buffer,buf_size,file);
	  cimg::fclose(file);

	  // Decompress buffer (if necessary)
	  if (compression) return get_load_convert(filename);
	  
	  // Read pixel data
	  CImg res(dx,cimg::abs(dy),1,3);
	  switch (bpp) {
	  case 1: { // Monochrome
	for (int y=res.height-1; y>=0; y--) { 
	  unsigned char mask = 0x80, val = 0;
	  cimg_mapX(res,x) {
		if (mask==0x80) val = *(ptrs++);
		const unsigned char *col = (unsigned char*)(palette+(val&mask?1:0));
		res(x,y,2) = (T)*(col++);
		res(x,y,1) = (T)*(col++);
		res(x,y,0) = (T)*(col++);
		mask = cimg::ror(mask);
	  } ptrs+=align; }
	  } break;
	  case 4: { // 16 colors
	for (int y=res.height-1; y>=0; y--) { 
	  unsigned char mask = 0xF0, val = 0;
	  cimg_mapX(res,x) {
		if (mask==0xF0) val = *(ptrs++);
		const unsigned char color = (mask<16)?(val&mask):((val&mask)>>4);
		unsigned char *col = (unsigned char*)(palette+color);
		res(x,y,2) = (T)*(col++);
		res(x,y,1) = (T)*(col++);
		res(x,y,0) = (T)*(col++);
		mask = cimg::ror(mask,4);
	  } ptrs+=align; }
	  } break;
	  case 8: { //  256 colors
	for (int y=res.height-1; y>=0; y--) { cimg_mapX(res,x) {
	  const unsigned char *col = (unsigned char*)(palette+*(ptrs++));
	  res(x,y,2) = (T)*(col++);
	  res(x,y,1) = (T)*(col++);
	  res(x,y,0) = (T)*(col++);
	} ptrs+=align; }
	  } break;
	  case 16: { // 16 bits colors
	for (int y=res.height-1; y>=0; y--) { cimg_mapX(res,x) {
	  const unsigned char c1 = *(ptrs++), c2 = *(ptrs++);
	  const unsigned short col = c1+(c2<<8);
	  res(x,y,2) = (T)(col&0x1F);
	  res(x,y,1) = (T)((col>>5)&0x1F);
	  res(x,y,0) = (T)((col>>10)&0x1F);
	} ptrs+=align; }
	  } break;	
	  case 24: { // 24 bits colors
	for (int y=res.height-1; y>=0; y--) { cimg_mapX(res,x) {
	  res(x,y,2) = (T)*(ptrs++);
	  res(x,y,1) = (T)*(ptrs++);
	  res(x,y,0) = (T)*(ptrs++);
	} ptrs+=align; }
	  } break;
	  case 32: { // 32 bits colors
	for (int y=res.height-1; y>=0; y--) { cimg_mapX(res,x) {
	  res(x,y,2) = (T)*(ptrs++);
	  res(x,y,1) = (T)*(ptrs++);
	  res(x,y,0) = (T)*(ptrs++);
	  ptrs++;
	} ptrs+=align; }
	  } break;
	  }
	  
	  if (palette) delete[] palette;
	  delete[] buffer;
	  if (dy<0) res.mirror('y');
	  return res;
	}
	
	//! Load an image from a BMP file
	CImg& load_bmp(const char *filename) {
	  return get_load_bmp(filename).swap(*this); 
	}

	//! Load an image from a PNG file.
	// Note : Most of this function has been written by Eric Fausett
	static CImg get_load_png(const char *filename) {
#ifndef cimg_use_png
#ifndef cimg_use_magick
	  return get_load_convert(filename);
#else
	  return get_load_magick(filename);
#endif
#else
	  // Open file and check for PNG validity
	  unsigned char pngCheck[8];
	  std::FILE *file = cimg::fopen(filename,"rb");
	  cimg::fread(pngCheck,8,file);
	  if(png_sig_cmp(pngCheck,0,8)){
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::get_load_png() : filename '%s' does not appear to be a valid PNG file",pixel_type(),filename);
	  }
	  
	  // Setup PNG structures for read
	  png_voidp user_error_ptr=0;
	  png_error_ptr user_error_fn=0, user_warning_fn=0;
	  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,	// Verifies libpng version correct
						   user_error_ptr, user_error_fn, user_warning_fn);
	  if(!png_ptr){
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::get_load_png() : trouble initializing 'png_ptr' data structure",pixel_type());
	  }
	  png_infop info_ptr = png_create_info_struct(png_ptr);
	  if(!info_ptr){
	cimg::fclose(file);
	png_destroy_read_struct(&png_ptr, (png_infopp)0, (png_infopp)0);			
	throw CImgIOException("CImg<%s>::get_load_png() : trouble initializing 'info_ptr' data structure",pixel_type());
	  }
	  png_infop end_info = png_create_info_struct(png_ptr);
	  if(!end_info){
	cimg::fclose(file);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);			
	throw CImgIOException("CImg<%s>::get_load_png() : trouble initializing 'end_info' data structure",pixel_type());
	  }
	  
	  // Error handling callback for png file reading
	  if (setjmp(png_jmpbuf(png_ptr))){
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::get_load_png() : unspecified error reading PNG file '%s'",pixel_type(),filename);
	  }      
	  png_init_io(png_ptr, file);      
	  png_set_sig_bytes(png_ptr, 8);
	  
	  // Get PNG Header Info up to data block
	  png_read_info(png_ptr, info_ptr);      
	  png_uint_32 width, height;
	  int bit_depth, color_type, interlace_type;
	  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type,
		   int_p_NULL, int_p_NULL);
	  int new_bit_depth = bit_depth;
	  int new_color_type = color_type;
	  
	  // Transforms to unify image data		
	  if (new_color_type == PNG_COLOR_TYPE_PALETTE){
	png_set_palette_to_rgb(png_ptr);
	new_color_type -= PNG_COLOR_MASK_PALETTE;
	new_bit_depth = 8;
	  }
	  if (new_color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
	png_set_gray_1_2_4_to_8(png_ptr);
	new_bit_depth = 8;
	  }
	  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	png_set_tRNS_to_alpha(png_ptr);
	  if (new_color_type == PNG_COLOR_TYPE_GRAY || new_color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
	png_set_gray_to_rgb(png_ptr);
	new_color_type |= PNG_COLOR_MASK_COLOR;
	  }
	  if (new_color_type == PNG_COLOR_TYPE_RGB)	png_set_filler(png_ptr, 0xffffU, PNG_FILLER_AFTER);      
	  png_read_update_info(png_ptr, info_ptr);      
	  if (!(new_bit_depth==8 || new_bit_depth==16))
	throw CImgIOException("CImg<%s>::get_load_png() : Wrong bit coding 'bit_depth=%u'",pixel_type(),new_bit_depth);
	  const int byte_depth = new_bit_depth>>3;
	  
	  // Allocate Memory for Image Read
	  png_bytep *imgData = new png_bytep[height];
	  for (unsigned int row=0; row < height; row++) imgData[row] = new png_byte[byte_depth * 4 * width];
	  png_read_image(png_ptr, imgData);
	  png_read_end(png_ptr, end_info);
	  
	  // Read pixel data
	  if (!(new_color_type==PNG_COLOR_TYPE_RGB || new_color_type==PNG_COLOR_TYPE_RGB_ALPHA))
	throw CImgIOException("CImg<%s>::get_load_png() : Wrong color coding new_color_type=%u",pixel_type(),new_color_type);
	  const bool no_alpha_channel = (new_color_type==PNG_COLOR_TYPE_RGB);
	  CImg res(width,height,1,no_alpha_channel?3:4);
	  const unsigned long off = width*height;
	  T *ptr1 = res.data, *ptr2 = ptr1+off, *ptr3 = ptr2+off, *ptr4 = ptr3+off;
	  switch(new_bit_depth){
	  case 8: {
	cimg_mapY(res,y){
	  const unsigned char *ptrs = (unsigned char*)imgData[y];
	  cimg_mapX(res,x){
		*(ptr1++) = (T)*(ptrs++);
		*(ptr2++) = (T)*(ptrs++);
		*(ptr3++) = (T)*(ptrs++);
		if (no_alpha_channel) ptrs++; else *(ptr4++) = (T)*(ptrs++);
	  }
	}
	  } break;
	  case 16: {
	cimg_mapY(res,y){
	  const unsigned short *ptrs = (unsigned short*)(imgData[y]);
	  cimg_mapX(res,x){
		*(ptr1++) = (T)*(ptrs++);
		*(ptr2++) = (T)*(ptrs++);
		*(ptr3++) = (T)*(ptrs++);
		if (no_alpha_channel) ptrs++; else *(ptr4++) = (T)*(ptrs++);
	  }
	}
	  } break;
	  }
	  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	  
	  // Deallocate Image Read Memory
	  for (unsigned int n=0; n<height; n++) delete[] imgData[n];
	  delete[] imgData;      
	  cimg::fclose(file);
	  return res;      
#endif      
	}

	//! Load an image from a PNG file
	CImg& load_png(const char *filename) {
	  return get_load_png(filename).swap(*this); 
	}

	//! Load a file in JPEG format.
	static CImg get_load_jpeg(const char *filename) {
#ifndef cimg_use_jpeg
#ifndef cimg_use_magick
	  return get_load_convert(filename);
#else
	  return get_load_magick(filename);
#endif
#else
	  struct jpeg_decompress_struct cinfo;
	  struct jpeg_error_mgr jerr;
	  std::FILE *file = cimg::fopen(filename,"rb");
	  cinfo.err = jpeg_std_error(&jerr);
	  jpeg_create_decompress(&cinfo);
	  jpeg_stdio_src(&cinfo,file);
	  jpeg_read_header(&cinfo,TRUE);
	  jpeg_start_decompress(&cinfo);
	  
	  if (cinfo.output_components!=1 && cinfo.output_components!=3 && cinfo.output_components!=4) {
	cimg::warn(true,"CImg<%s>::get_load_jpeg() : Don't know how to read image '%s' with libpeg,"
		   "trying ImageMagick's convert",
		   pixel_type(),filename);
	return get_load_convert(filename);
	  }
	  
	  const unsigned int row_stride = cinfo.output_width * cinfo.output_components;
	  unsigned char *buf = new unsigned char[cinfo.output_width*cinfo.output_height*cinfo.output_components], *buf2 = buf;
	  JSAMPROW row_pointer[1];
	  while (cinfo.output_scanline < cinfo.output_height) {
	row_pointer[0] = &buf[cinfo.output_scanline*row_stride];
	jpeg_read_scanlines(&cinfo,row_pointer,1);
	  }
	  jpeg_finish_decompress(&cinfo);
	  jpeg_destroy_decompress(&cinfo);
	  cimg::fclose(file);
	  
	  CImg<T> dest(cinfo.output_width,cinfo.output_height,1,cinfo.output_components);
	  switch (dest.dim) {
	  case 1: {
	T *ptr_g = dest.ptr();
	cimg_mapXY(dest,x,y) *(ptr_g++) = (T)*(buf2++);
	  } break;
	  case 3: {
	T *ptr_r = dest.ptr(0,0,0,0), *ptr_g = dest.ptr(0,0,0,1), *ptr_b = dest.ptr(0,0,0,2);
	cimg_mapXY(dest,x,y) {
	  *(ptr_r++) = (T)*(buf2++);
	  *(ptr_g++) = (T)*(buf2++);
	  *(ptr_b++) = (T)*(buf2++);
	}
	  } break;
	  case 4: {
	T *ptr_r = dest.ptr(0,0,0,0), *ptr_g = dest.ptr(0,0,0,1), 
	  *ptr_b = dest.ptr(0,0,0,2), *ptr_a = dest.ptr(0,0,0,3);
	cimg_mapXY(dest,x,y) {
	  *(ptr_r++) = (T)*(buf2++);
	  *(ptr_g++) = (T)*(buf2++);
	  *(ptr_b++) = (T)*(buf2++);
	  *(ptr_a++) = (T)*(buf2++);
	}
	  } break;
	  }
	  delete[] buf;
	  return dest;
#endif
	}

	//! Load an image from a JPEG file
	CImg& load_jpeg(const char *filename) {
	  return get_load_jpeg(filename).swap(*this); 
	}

#ifdef cimg_use_magick
	//! Load an image using builtin ImageMagick++ Library
	/**
	   Added April/may 2006 by Christoph Hormann <chris_hormann@gmx.de>
	   This is experimental code, not much tested, use with care.
	**/
	static CImg get_load_magick(const char *filename) {
	  CImg dest;
	  Magick::Image image(filename);
	  const unsigned int width = image.size().width(), height = image.size().height();
	  switch (image.type()) {
	  case Magick::PaletteMatteType:
	  case Magick::TrueColorMatteType:
	  case Magick::ColorSeparationType: {
	dest.assign(width,height,1,4);
	T *rdata = dest.ptr(0,0,0,0), *gdata = dest.ptr(0,0,0,1), *bdata = dest.ptr(0,0,0,2), *adata = dest.ptr(0,0,0,3);
	Magick::PixelPacket *pixels = image.getPixels(0,0,width,height);
	for (unsigned int off = width*height; off; --off) {
	  *(rdata++) = (T)(pixels->red);
	  *(gdata++) = (T)(pixels->green);
	  *(bdata++) = (T)(pixels->blue);
	  *(adata++) = (T)(pixels->opacity);
	  pixels++;
	}
	  }	break;
	  case Magick::PaletteType:
	  case Magick::TrueColorType: {
	dest.assign(width,height,1,3);
	T *rdata = dest.ptr(0,0,0,0), *gdata = dest.ptr(0,0,0,1), *bdata = dest.ptr(0,0,0,2);
	Magick::PixelPacket *pixels = image.getPixels(0,0,width,height);
	for (unsigned int off = width*height; off; --off) {
	  *(rdata++) = (T)(pixels->red);
	  *(gdata++) = (T)(pixels->green);
	  *(bdata++) = (T)(pixels->blue);
	  pixels++;
	}
	  } break;
	  case Magick::GrayscaleMatteType: {
	dest.assign(width,height,1,2);
	T *data = dest.ptr(0,0,0,0), *adata = dest.ptr(0,0,0,1);
	Magick::PixelPacket *pixels = image.getPixels(0,0,width,height);
	for (unsigned int off = width*height; off; --off) {
	  *(data++) = (T)(pixels->red);
	  *(adata++) = (T)(pixels->opacity);
	  pixels++;
	}
	  } break;
	  default: {
	dest.assign(width,height,1,1);
	T *data = dest.ptr(0,0,0,0);
	Magick::PixelPacket *pixels = image.getPixels(0,0,width,height);
	for (unsigned int off = width*height; off; --off) {
	  *(data++) = (T)(pixels->red);
	  pixels++;
	}
	  } break;
	  }
	  return dest;
	}

	//! Load an image using builtin ImageMagick++ Library (in-place version).
	CImg& load_magick(const char *filename) { 
	  return get_load_magick(filename).swap(*this); 
	}
#endif
	
	//! Load an image from a RAW file.
	static CImg get_load_raw(const char *filename,
				 const unsigned int sizex, const unsigned int sizey=1,
				 const unsigned int sizez=1, const unsigned int sizev=1,
				 const bool multiplexed = false, const bool endian_swap = false) {
	  CImg<T> res(sizex,sizey,sizez,sizev,0);
	  if (res.is_empty()) return res;
	  std::FILE *file = cimg::fopen(filename,"rb");
	  if (!multiplexed) {
	cimg::fread(res.ptr(),res.size(),file);
	if (endian_swap) cimg::endian_swap(res.ptr(),res.size());
	  }
	  else {
	CImg<T> buf(1,1,1,sizev);
	cimg_mapXYZ(res,x,y,z) {
	  cimg::fread(buf.ptr(),sizev,file);
	  if (endian_swap) cimg::endian_swap(buf.ptr(),sizev);
	  res.set_vector_at(buf,x,y,z); }
	  }
	  cimg::fclose(file);
	  return res;      
	}

	//! In-place version of get_load_raw()
	CImg& load_raw(const char *filename,
		   const unsigned int sizex, const unsigned int sizey=1,
		   const unsigned int sizez=1, const unsigned int sizev=1, 
		   const bool multiplexed = false, const bool endian_swap = false) {
	  return get_load_raw(filename,sizex,sizey,sizez,sizev,multiplexed,endian_swap).swap(*this);
	}

	//! Load an image from a RGBA file.
	static CImg get_load_rgba(const char *filename,const unsigned int dimw,const unsigned int dimh) {
	  std::FILE *file = cimg::fopen(filename,"rb");
	  unsigned char *buffer = new unsigned char[dimw*dimh*4];
	  cimg::fread(buffer,dimw*dimh*4,file);
	  cimg::fclose(file);     
	  CImg res(dimw,dimh,1,4);
	  T *pR = res.ptr(0,0,0,0), *pG = res.ptr(0,0,0,1), *pB = res.ptr(0,0,0,2), *pA = res.ptr(0,0,0,3);
	  const unsigned char *ptrs = buffer;
	  for (unsigned int off=res.width*res.height; off>0; --off) {
	*(pR++) = *(ptrs++);
	*(pG++) = *(ptrs++);
	*(pB++) = *(ptrs++);
	*(pA++) = *(ptrs++);
	  }      
	  delete[] buffer;
	  return res;      
	}

	//! In-place version of get_load_rgba()
	CImg& load_rgba(const char *filename,const unsigned int dimw,const unsigned int dimh) {
	  return get_load_rgba(filename,dimw,dimh).swap(*this);
	}

	//! Load an image from a RGB file.
	static CImg get_load_rgb(const char *filename,const unsigned int dimw,const unsigned int dimh) {
	  std::FILE *file = cimg::fopen(filename,"rb");
	  unsigned char *buffer = new unsigned char[dimw*dimh*3];
	  cimg::fread(buffer,dimw*dimh*3,file);
	  cimg::fclose(file);
	  CImg res(dimw,dimh,1,3);
	  T *pR = res.ptr(0,0,0,0), *pG = res.ptr(0,0,0,1), *pB=res.ptr(0,0,0,2);
	  const unsigned char *ptrs = buffer;
	  for (unsigned int off=res.width*res.height; off>0; --off) {
	*(pR++) = *(ptrs++);
	*(pG++) = *(ptrs++);
	*(pB++) = *(ptrs++);
	  }
	  delete[] buffer;
	  return res;
	}

	//! In-place version of get_load_rgb()
	CImg& load_rgb(const char *filename,const unsigned int dimw,const unsigned int dimh) {
	  return get_load_rgb(filename,dimw,dimh).swap(*this);
	}
	
#define cimg_load_inr_case(Tf,sign,pixsize,Ts) \
  if (!loaded && fopt[6]==pixsize && fopt[4]==Tf && fopt[5]==sign) { \
	  Ts *xval, *val = new Ts[fopt[0]*fopt[3]]; \
	  cimg_mapYZ(dest,y,z) { \
		  cimg::fread(val,fopt[0]*fopt[3],file); \
		  if (fopt[7]!=endian) cimg::endian_swap(val,fopt[0]*fopt[3]); \
		  xval = val; cimg_mapX(dest,x) cimg_mapV(dest,k) \
						  dest(x,y,z,k) = (T)*(xval++); \
		} \
	  delete[] val; \
	  loaded = true; \
	}
	
	static void _load_inr(std::FILE *file,int out[8],float *voxsize=0) {
	  char item[1024],tmp1[64],tmp2[64];
	  out[0]=out[1]=out[2]=out[3]=out[5]=1; out[4]=out[6]=out[7]=-1;
	  std::fscanf(file,"%63s",item);
	  if(cimg::strncasecmp(item,"#INRIMAGE-4#{",13)!=0) 
	throw CImgIOException("CImg<%s>::get_load_inr() : File does not appear to be a valid INR file.\n"
				  "(INRIMAGE-4 identifier not found)",pixel_type());
	  while (std::fscanf(file," %63[^\n]%*c",item)!=EOF && cimg::strncmp(item,"##}",3)) {
		std::sscanf(item," XDIM%*[^0-9]%d",out);
		std::sscanf(item," YDIM%*[^0-9]%d",out+1);
		std::sscanf(item," ZDIM%*[^0-9]%d",out+2);
		std::sscanf(item," VDIM%*[^0-9]%d",out+3);
		std::sscanf(item," PIXSIZE%*[^0-9]%d",out+6);
		if (voxsize) {
		  std::sscanf(item," VX%*[^0-9.eE+-]%f",voxsize);
		  std::sscanf(item," VY%*[^0-9.eE+-]%f",voxsize+1);
		  std::sscanf(item," VZ%*[^0-9.eE+-]%f",voxsize+2);
		}
		if (std::sscanf(item," CPU%*[ =]%s",tmp1)) out[7]=cimg::strncasecmp(tmp1,"sun",3)?0:1;
		switch(std::sscanf(item," TYPE%*[ =]%s %s",tmp1,tmp2)) {
		case 0: break;
		case 2: out[5] = cimg::strncasecmp(tmp1,"unsigned",8)?1:0; std::strcpy(tmp1,tmp2);
		case 1:
		  if (!cimg::strncasecmp(tmp1,"int",3)   || !cimg::strncasecmp(tmp1,"fixed",5))  out[4]=0;
		  if (!cimg::strncasecmp(tmp1,"float",5) || !cimg::strncasecmp(tmp1,"double",6)) out[4]=1;
		  if (!cimg::strncasecmp(tmp1,"packed",6))                                       out[4]=2;
		  if (out[4]>=0) break;
		default: throw CImgIOException("cimg::inr_header_read() : Invalid TYPE '%s'",tmp2);
		}
	  }
	  if(out[0]<0 || out[1]<0 || out[2]<0 || out[3]<0)
		throw CImgIOException("CImg<%s>::get_load_inr() : Bad dimensions in .inr file = ( %d , %d , %d , %d )",
							  pixel_type(),out[0],out[1],out[2],out[3]);
	  if(out[4]<0 || out[5]<0) throw CImgIOException("CImg<%s>::get_load_inr() : TYPE is not fully defined",pixel_type());
	  if(out[6]<0) throw CImgIOException("CImg<%s>::get_load_inr() : PIXSIZE is not fully defined",pixel_type());
	  if(out[7]<0) throw CImgIOException("CImg<%s>::get_load_inr() : Big/Little Endian coding type is not defined",pixel_type());
	}
	
	//! Load an image from an INRIMAGE-4 file.
	static CImg get_load_inr(const char *filename, float *voxsize = 0) {
	  std::FILE *file = cimg::fopen(filename,"rb");
	  int fopt[8], endian=cimg::endian()?1:0;
	  bool loaded = false;
	  if (voxsize) voxsize[0]=voxsize[1]=voxsize[2]=1;
	  _load_inr(file,fopt,voxsize);
	  CImg<T> dest(fopt[0],fopt[1],fopt[2],fopt[3]);
	  cimg_load_inr_case(0,0,8, unsigned char);
	  cimg_load_inr_case(0,1,8, char);
	  cimg_load_inr_case(0,0,16,unsigned short);
	  cimg_load_inr_case(0,1,16,short);
	  cimg_load_inr_case(0,0,32,unsigned int);
	  cimg_load_inr_case(0,1,32,int);
	  cimg_load_inr_case(1,0,32,float);
	  cimg_load_inr_case(1,1,32,float);
	  cimg_load_inr_case(1,0,64,double);
	  cimg_load_inr_case(1,1,64,double);
	  if (!loaded) throw CImgIOException("CImg<%s>::get_load_inr() : File '%s', can't read images of the type specified in the file",
					 pixel_type(),filename);
	  cimg::fclose(file);
	  return dest;
	}

	//! In-place version of get_load_inr()
	CImg& load_inr(const char *filename, float *voxsize = 0) { 
	  return get_load_inr(filename,voxsize).swap(*this); 
	}
   
#define cimg_load_pandore_case(nid,nbdim,nwidth,nheight,ndepth,ndim,stype) \
  case nid: { \
	cimg::fread(dims,nbdim,file); \
	if (endian) cimg::endian_swap(dims,nbdim); \
	dest.assign(nwidth,nheight,ndepth,ndim); \
	stype *buffer = new stype[dest.size()]; \
	cimg::fread(buffer,dest.size(),file); \
	if (endian) cimg::endian_swap(buffer,dest.size()); \
	T *ptrd = dest.ptr(); \
	cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++)); \
	buffer-=dest.size(); \
	delete[] buffer; \
   } \
   break;

	//! Load an image from a PANDORE-5 file.
	static CImg get_load_pandore(const char *filename) {
	  std::FILE *file = cimg::fopen(filename,"rb");
	  typedef unsigned char uchar;
	  typedef unsigned short ushort;
	  typedef unsigned int uint;  
	  typedef unsigned long ulong; 
	  CImg dest;
	  char tmp[32];
	  cimg::fread(tmp,12,file);
	  if (cimg::strncasecmp("PANDORE",tmp,7)) 
	throw CImgIOException("CImg<%s>::get_load_pandore() : File '%s' does not appear to be a valid PANDORE file.\n"
				  "(PANDORE identifier not found)",pixel_type(),filename);
	  unsigned int imageid,dims[8];
	  int ptbuf[4];
	  cimg::fread(&imageid,1,file);
	  const bool endian = (imageid>255);
	  if (endian) cimg::endian_swap(imageid);
	 
	  cimg::fread(tmp,20,file);
	  switch (imageid) {
	cimg_load_pandore_case(2,2,dims[1],1,1,1,uchar);
	cimg_load_pandore_case(3,2,dims[1],1,1,1,long);
	cimg_load_pandore_case(4,2,dims[1],1,1,1,float);
	cimg_load_pandore_case(5,3,dims[2],dims[1],1,1,uchar);
	cimg_load_pandore_case(6,3,dims[2],dims[1],1,1,long);
	cimg_load_pandore_case(7,3,dims[2],dims[1],1,1,float);
	cimg_load_pandore_case(8,4,dims[3],dims[2],dims[1],1,uchar);
	cimg_load_pandore_case(9,4,dims[3],dims[2],dims[1],1,long);
	cimg_load_pandore_case(10,4,dims[3],dims[2],dims[1],1,float);

	  case 11: { // Region 1D
	cimg::fread(dims,3,file);
	if (endian) cimg::endian_swap(dims,3);
	dest.assign(dims[1],1,1,1);
	if (dims[2]<256) {
	  unsigned char *buffer = new unsigned char[dest.size()];
	  cimg::fread(buffer,dest.size(),file);
	  T *ptrd = dest.ptr();
	  cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
	  buffer-=dest.size();
	  delete[] buffer;
	} else {
	  if (dims[2]<65536) {
		unsigned short *buffer = new unsigned short[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  } else {
		unsigned int *buffer = new unsigned int[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  }
	}	
	  }
	break;
	  case 12: { // Region 2D
	cimg::fread(dims,4,file);
	if (endian) cimg::endian_swap(dims,4);
	dest.assign(dims[2],dims[1],1,1);
	if (dims[3]<256) {
	  unsigned char *buffer = new unsigned char[dest.size()];
	  cimg::fread(buffer,dest.size(),file);
	  T *ptrd = dest.ptr();
	  cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
	  buffer-=dest.size();
	  delete[] buffer;
	} else {
	  if (dims[3]<65536) {
		unsigned short *buffer = new unsigned short[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  } else {
		unsigned long *buffer = new unsigned long[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  }
	}	
	  }
	break;
	  case 13: { // Region 3D
	cimg::fread(dims,5,file);
	if (endian) cimg::endian_swap(dims,5);
	dest.assign(dims[3],dims[2],dims[1],1);
	if (dims[4]<256) {
	  unsigned char *buffer = new unsigned char[dest.size()];
	  cimg::fread(buffer,dest.size(),file);
	  T *ptrd = dest.ptr();
	  cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
	  buffer-=dest.size();
	  delete[] buffer;
	} else {
	  if (dims[4]<65536) {
		unsigned short *buffer = new unsigned short[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  } else {
		unsigned int *buffer = new unsigned int[dest.size()];
		cimg::fread(buffer,dest.size(),file);
		if (endian) cimg::endian_swap(buffer,dest.size());
		T *ptrd = dest.ptr();
		cimg_mapoff(dest,off) *(ptrd++) = (T)(*(buffer++));
		buffer-=dest.size();
		delete[] buffer;
	  }
	}	
	  }
	break;
	cimg_load_pandore_case(16,4,dims[2],dims[1],1,3,uchar);
	cimg_load_pandore_case(17,4,dims[2],dims[1],1,3,long);
	cimg_load_pandore_case(18,4,dims[2],dims[1],1,3,float);
	cimg_load_pandore_case(19,5,dims[3],dims[2],dims[1],3,uchar);
	cimg_load_pandore_case(20,5,dims[3],dims[2],dims[1],3,long);
	cimg_load_pandore_case(21,5,dims[3],dims[2],dims[1],3,float);
	cimg_load_pandore_case(22,2,dims[1],1,1,dims[0],uchar);
	cimg_load_pandore_case(23,2,dims[1],1,1,dims[0],long);
	cimg_load_pandore_case(24,2,dims[1],1,1,dims[0],ulong);
	cimg_load_pandore_case(25,2,dims[1],1,1,dims[0],float);
	cimg_load_pandore_case(26,3,dims[2],dims[1],1,dims[0],uchar);
	cimg_load_pandore_case(27,3,dims[2],dims[1],1,dims[0],long);
	cimg_load_pandore_case(28,3,dims[2],dims[1],1,dims[0],ulong);
	cimg_load_pandore_case(29,3,dims[2],dims[1],1,dims[0],float);
	cimg_load_pandore_case(30,4,dims[3],dims[2],dims[1],dims[0],uchar);
	cimg_load_pandore_case(31,4,dims[3],dims[2],dims[1],dims[0],long);
	cimg_load_pandore_case(32,4,dims[3],dims[2],dims[1],dims[0],ulong);
	cimg_load_pandore_case(33,4,dims[3],dims[2],dims[1],dims[0],float);	
	  case 34: // Points 1D	
	cimg::fread(ptbuf,1,file);
	if (endian) cimg::endian_swap(ptbuf,1);
	dest.assign(1); dest[0]=(T)ptbuf[0];
	break;
	  case 35: // Points 2D
	cimg::fread(ptbuf,2,file);
	if (endian) cimg::endian_swap(ptbuf,2);
	dest.assign(2); dest[0]=(T)ptbuf[1]; dest[1]=(T)ptbuf[0];
	break;
	  case 36: // Points 3D
	cimg::fread(ptbuf,3,file);
	if (endian) cimg::endian_swap(ptbuf,3);
	dest.assign(3); dest[0]=(T)ptbuf[2]; dest[1]=(T)ptbuf[1]; dest[2]=(T)ptbuf[0];
	break;
	  default:
	throw CImgIOException("CImg<%s>::get_load_pandore() : File '%s', can't read images with ID_type=%u",pixel_type(),filename,imageid);
	  }
	  return dest;
	}

	//! In-place version of get_load_pandore()
	CImg& load_pandore(const char *filename) {
	  return get_load_pandore(filename).swap(*this); 
	}

	//! Load an image from an ANALYZE7.5 file
	static CImg get_load_analyze(const char *filename, float *voxsize = 0) {

	  // Open header and data files
	  std::FILE *file_header=0, *file=0;
	  char body[1024];
	  const char *ext = cimg::filename_split(filename,body);
	  if (!cimg::strcasecmp(ext,"hdr") || !cimg::strcasecmp(ext,"img")) {
	std::sprintf(body+cimg::strlen(body),".hdr");
	file_header = cimg::fopen(body,"rb");
	std::sprintf(body+cimg::strlen(body)-3,"img");
	file = cimg::fopen(body,"rb");
	  } else throw CImgIOException("CImg<%s>::get_load_analyze() : Cannot load filename '%s' as an analyze format",pixel_type(),filename);

	  // Read header
	  bool endian = false;
	  unsigned int header_size;
	  cimg::fread(&header_size,1,file_header);
	  if (header_size>=4096) { endian = true; cimg::endian_swap(header_size); }
	  unsigned char *header = new unsigned char[header_size];
	  cimg::fread(header+4,header_size-4,file_header);
	  cimg::fclose(file_header);
	  if (endian) {
	cimg::endian_swap((short*)(header+40),5);
		cimg::endian_swap((short*)(header+70),1);
		cimg::endian_swap((short*)(header+72),1);
		cimg::endian_swap((float*)(header+76),4);
		cimg::endian_swap((float*)(header+112),1);
	  }
	  unsigned short *dim = (unsigned short*)(header+40), dimx=1, dimy=1, dimz=1, dimv=1;
	  cimg::warn(!dim[0],"CImg<%s>::get_load_analyze() : Specified image has zero dimensions.",pixel_type());
	  cimg::warn(dim[0]>4,"CImg<%s>::get_load_analyze() : Number of image dimension is %d, reading only the 4 first dimensions",
		 pixel_type(),dim[0]);
	  if (dim[0]>=1) dimx = dim[1];
	  if (dim[0]>=2) dimy = dim[2];
	  if (dim[0]>=3) dimz = dim[3];
	  if (dim[0]>=4) dimv = dim[4];
	  
	  float scalefactor = *(float*)(header+112); if (scalefactor==0) scalefactor=1;
	  const unsigned short datatype = *(short*)(header+70);
	  if (voxsize) { const float *vsize = (float*)(header+76); voxsize[0] = vsize[1]; voxsize[1] = vsize[2]; voxsize[2] = vsize[3]; }
	  delete[] header;

	  // Read pixel data
	  CImg dest(dimx,dimy,dimz,dimv);
	  switch (datatype) {
	  case 2: {
	unsigned char *buffer = new unsigned char[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
	  } break;
	  case 4: {
	short *buffer = new short[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
	  } break;
	  case 8: {
	int *buffer = new int[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
	  } break;
	  case 16: {
	float *buffer = new float[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
	  } break;
	  case 64: {
	double *buffer = new double[dimx*dimy*dimz*dimv];
	cimg::fread(buffer,dimx*dimy*dimz*dimv,file);
	if (endian) cimg::endian_swap(buffer,dimx*dimy*dimz*dimv);
	cimg_mapoff(dest,off) dest.data[off] = (T)(buffer[off]*scalefactor);
	delete[] buffer;
	  } break;
	  default: throw CImgIOException("CImg<%s>::get_load_analyze() : Cannot read images width 'datatype = %d'",pixel_type(),datatype);
	  }
	  cimg::fclose(file);
	  return dest;
	}

	//! In-place version of get_load_analyze()
	CImg& load_analyze(const char *filename, float *voxsize = 0) {
	  return get_load_analyze(filename,voxsize).swap(*this); 
	}

	//! Load PAR-REC (Philips) image file
	static CImg get_load_parrec(const char *filename,const char axe='v',const char align='p') {
	  return CImgl<T>::get_load_parrec(filename).get_append(axe,align);
	}

	//! In-place version of get_load_parrec()
	CImg& load_parrec(const char *filename, const char axis='v', const char align='p') {
	  return get_load_parrec(filename,axis,align).swap(*this);
	}
	
	//! Load an image from a CImg RAW file
	static CImg get_load_cimg(const char *filename, const char axis='v', const char align='p') { 
	  return CImgl<T>(filename).get_append(axis,align); 
	}

	//! In-place version of get_load_cimg()
	CImg& load_cimg(const char* filename, const char axis='v', const char align='p') {
	  return get_load_cimg(filename,axis,align).swap(*this);
	}

	//! Function that loads the image for other file formats that are not natively handled by CImg, using the tool 'convert' from the ImageMagick package.\n
	//! This is the case for all compressed image formats (GIF,PNG,JPG,TIF,...). You need to install the ImageMagick package in order to get
	//! this function working properly (see http://www.imagemagick.org ).
	static CImg get_load_convert(const char *filename) {
	  static bool first_time = true;
	  char command[1024], filetmp[512];
	  if (first_time) { std::srand((unsigned int)::time(0)); first_time = false; }
	  std::FILE *file = 0;
	  do { 
	if (file) std::fclose(file);
	std::sprintf(filetmp,"%s/CImg%.4d.ppm",cimg::temporary_path(),std::rand()%10000);
	file = std::fopen(filetmp,"rb");
	  } while (file);
	  std::sprintf(command,"\"%s\" \"%s\" %s",cimg::convert_path(),filename,filetmp);
	  cimg::system(command);
	  file = std::fopen(filetmp,"rb");
	  if (!file) {
		std::fclose(cimg::fopen(filename,"r"));
		throw CImgIOException("CImg<%s>::get_load_convert() : Failed to open image '%s' with 'convert'.\n"
				  "Check that you have installed the ImageMagick package in a standard directory.",
				  pixel_type(),filename);
	  } else cimg::fclose(file);
	  const CImg dest = CImg<T>::get_load_pnm(filetmp);
	  std::remove(filetmp);
	  return dest;
	}

	//! In-place version of get_load_convert()
	CImg& load_convert(const char *filename) {
	  return get_load_convert(filename).swap(*this); 
	}

	//! Load an image from a Dicom file (need '(X)Medcon' : http://xmedcon.sourceforge.net )
	static CImg get_load_dicom(const char *filename) {
	  static bool first_time = true;
	  char command[1024], filetmp[512], body[512];
	  if (first_time) { std::srand((unsigned int)::time(0)); first_time = false; }
	  cimg::fclose(cimg::fopen(filename,"r")); 
	  std::FILE *file = 0;
	  do { 
	if (file) std::fclose(file);
	std::sprintf(filetmp,"CImg%.4d.hdr",std::rand()%10000);
	file = std::fopen(filetmp,"rb");
	  } while (file);
	  std::sprintf(command,"\"%s\" -w -c anlz -o \"%s\" -f \"%s\"",cimg::medcon_path(),filetmp,filename);
	  cimg::system(command);
	  cimg::filename_split(filetmp,body);
	  std::sprintf(command,"m000-%s.hdr",body);
	  file = std::fopen(command,"rb");
	  if (!file) {
		std::fclose(cimg::fopen(filename,"r"));
		throw CImgIOException("CImg<%s>::get_load_dicom() : Failed to open image '%s' with 'medcon'.\n"
				  "Check that you have installed the XMedCon package in a standard directory.",
				  pixel_type(),filename);
	  } else cimg::fclose(file);
	  const CImg dest = CImg<T>::get_load_analyze(command);
	  std::remove(command);
	  std::sprintf(command,"m000-%s.img",body);
	  std::remove(command);
	  return dest;
	}

	//! In-place version of get_load_dicom()
	CImg& load_dicom(const char *filename) {
	  return get_load_dicom(filename).swap(*this); 
	}

	//! Load OFF files (GeomView 3D object files)
	template<typename tf,typename tc>
	static CImg<T> get_load_off(const char *filename, CImgl<tf>& primitives, CImgl<tc>& colors, const bool invert_faces=false) {
	  std::FILE *file=cimg::fopen(filename,"r");
	  unsigned int nb_points=0, nb_triangles=0;
	  int err;
	  if ((err = std::fscanf(file,"OFF%u%u%*[^\n]",&nb_points,&nb_triangles))!=2)
	throw CImgIOException("CImg<%s>::get_load_off() : File '%s' does not appear to be a valid OFF file.\n",
				  pixel_type(),filename);
	  
	  // Read points data
	  CImg<T> points(nb_points,3);
	  float X=0,Y=0,Z=0;
	  cimg_mapX(points,l) {
	if ((err = std::fscanf(file,"%f%f%f%*[^\n]",&X,&Y,&Z))!=3)
	  throw CImgIOException("CImg<%s>::get_load_off() : File '%s', cannot read point %u.\n",
				pixel_type(),filename,l);
	points(l,0) = (T)X; points(l,1) = (T)Y; points(l,2) = (T)Z;
	  }
	  
	  // Read primitive data
	  primitives.empty();
	  colors.empty();
	  bool stopflag = false;
	  while (!stopflag) {
	unsigned int prim=0, i0=0, i1=0, i2=0, i3=0;
	char s_colors[256] = {'\0'};
	if ((err = std::fscanf(file,"%u",&prim))!=1) stopflag=true;
	else switch (prim) {
	case 3: {
	  if ((err = std::fscanf(file,"%u%u%u%255[^\n]",&i0,&i1,&i2,s_colors))<3) stopflag = true;
	  else {
		float c0=0.5, c1=0.5, c2=0.5;
		std::sscanf(s_colors,"%f%f%f",&c0,&c1,&c2);
		if (invert_faces) primitives.insert(CImg<tf>::vector(i0,i1,i2));
		else primitives.insert(CImg<tf>::vector(i0,i2,i1));
		colors.insert(CImg<tc>::vector((tc)(c0*255),(tc)(c1*255),(tc)(c2*255)));
	  }
	} break;
	case 4: {
	  if ((err = std::fscanf(file,"%u%u%u%u%255[^\n]",&i0,&i1,&i2,&i3,s_colors))<4) stopflag = true;
	  else {
		float c0=0.5, c1=0.5, c2=0.5;
		std::sscanf(s_colors,"%f%f%f",&c0,&c1,&c2);
		if (invert_faces) primitives.insert(CImg<tf>::vector(i0,i1,i2,i3));
		else primitives.insert(CImg<tf>::vector(i0,i3,i2,i1));
		colors.insert(CImg<tc>::vector((tc)(c0*255),(tc)(c1*255),(tc)(c2*255),(tc)(c2*255)));
	  }
	} break;
	default: stopflag = true;	  
	}
	  }
	  cimg::fclose(file);
	  cimg::warn(primitives.size!=nb_triangles,
		 "CImg<%s>::get_load_off() : File '%s' contained %u triangles instead of %u as claimed in the header.",
		 pixel_type(),filename,primitives.size,nb_triangles);
	  return points;
	}

	//! In-place version of get_load_off()
	template<typename tf,typename tc>
	CImg& load_off(const char *filename, CImgl<tf>& primitives, CImgl<tc>& colors, const bool invert_faces=false) {
	  return get_load_off(filename,primitives,colors,invert_faces).swap(*this);
	}
	
	//! Save the image as a file. 
	/**
	   The used file format is defined by the file extension in the filename \p filename.\n
	   Parameter \p number can be used to add a 6-digit number to the filename before saving.\n
	   If \p normalize is true, a normalized version of the image (between [0,255]) is saved.
	**/
	const CImg& save(const char *filename,const int number=-1) const {
	  if (!filename) throw CImgArgumentException("CImg<%s>::save() : Specified filename is (null).",pixel_type());
	  const char *ext = cimg::filename_split(filename);
	  char nfilename[1024];
	  if (number>=0) filename = cimg::filename_number(filename,number,6,nfilename);
	  if (!cimg::strcasecmp(ext,"asc")) return save_ascii(filename);
	  if (!cimg::strcasecmp(ext,"dlm")) return save_dlm(filename);
	  if (!cimg::strcasecmp(ext,"inr")) return save_inr(filename);
	  if (!cimg::strcasecmp(ext,"hdr")) return save_analyze(filename);
	  if (!cimg::strcasecmp(ext,"dcm")) return save_dicom(filename);
	  if (!cimg::strcasecmp(ext,"pan")) return save_pandore(filename);
	  if (!cimg::strcasecmp(ext,"bmp")) return save_bmp(filename);
	  if (!cimg::strcasecmp(ext,"png")) return save_png(filename);
	  if (!cimg::strcasecmp(ext,"jpg") ||
	  !cimg::strcasecmp(ext,"jpeg")) return save_jpeg(filename);
	  if (!cimg::strcasecmp(ext,"rgba")) return save_rgba(filename);
	  if (!cimg::strcasecmp(ext,"rgb")) return save_rgb(filename);
	  if (!cimg::strcasecmp(ext,"raw")) return save_raw(filename);
	  if (!cimg::strcasecmp(ext,"cimg") || ext[0]=='\0') return save_cimg(filename);
	  if (!cimg::strcasecmp(ext,"pgm") || 
	  !cimg::strcasecmp(ext,"ppm") || 
	  !cimg::strcasecmp(ext,"pnm")) return save_pnm(filename);
	  if (!cimg::strcasecmp(ext,"yuv")) return save_yuv(filename,true);
#ifdef cimg_use_magick
	  return save_magick(filename);
#else
	  return save_convert(filename);
#endif
	}
  
	//! Save the image as an ASCII file.
	const CImg& save_ascii(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_ascii() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_ascii() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"w");
	  std::fprintf(file,"%u %u %u %u\n",width,height,depth,dim);
	  const T* ptrs = data;
	  cimg_mapYZV(*this,y,z,v) {
	cimg_mapX(*this,x) std::fprintf(file,"%g ",(double)*(ptrs++));
	std::fputc('\n',file);
	  }
	  cimg::fclose(file);
	  return *this;
	}

	//! Save the image as a DLM file.
	const CImg& save_dlm(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_dlm() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_dlm() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"w");
	  const T* ptrs = data;
	  cimg_mapYZV(*this,y,z,v) {
	cimg_mapX(*this,x) std::fprintf(file,"%g%s",(double)*(ptrs++),(x==(int)width-1)?"":",");
	std::fputc('\n',file);
	  }
	  cimg::fclose(file);
	  return *this;
	}

	//! Save the image as a PNM file.
	const CImg& save_pnm(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_pnm() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_pnm() : Specified filename is (null).",pixel_type());
	  const char *ext = cimg::filename_split(filename);
	  const CImgStats st(*this,false);

	  if (dim>1 && !cimg::strcasecmp(ext,"pgm")) {
			get_norm_pointwise().normalize(0.0f,(float)st.max).save_pnm(filename); 
			return *this; 
	  }
	  std::FILE *file = cimg::fopen(filename,"wb");
	  const T 
	*ptrR = ptr(0,0,0,0),
	*ptrG = (dim>=2)?ptr(0,0,0,1):ptrR,
	*ptrB = (dim>=3)?ptr(0,0,0,2):ptrR;
	  const unsigned int buf_size = width*height*(dim==1?1:3);

	  std::fprintf(file,"P%c\n# CREATOR: CImg : Original size=%ux%ux%ux%u\n%u %u\n%u\n",
		   (dim==1?'5':'6'),width,height,depth,dim,width,height,(st.max)<256?255:65535);
	  
	  switch(dim) {
	  case 1: {
	if ((st.max)<256) { // Binary PGM 8 bits
	  unsigned char *ptrd = new unsigned char[buf_size], *xptrd = ptrd;
	  cimg_mapXY(*this,x,y) *(xptrd++) = (unsigned char)*(ptrR++);
	  cimg::fwrite(ptrd,buf_size,file);
	  delete[] ptrd;
	} else {             // Binary PGM 16 bits
	  unsigned short *ptrd = new unsigned short[buf_size], *xptrd = ptrd;
	  cimg_mapXY(*this,x,y) *(xptrd++) = (unsigned short)*(ptrR++);
	  if (!cimg::endian()) cimg::endian_swap(ptrd,buf_size);
	  cimg::fwrite(ptrd,buf_size,file);
	  delete[] ptrd;
	}
	  } break;
	  default: {
	if ((st.max)<256) { // Binary PPM 8 bits
	  unsigned char *ptrd = new unsigned char[buf_size], *xptrd = ptrd;
	  cimg_mapXY(*this,x,y) {
		*(xptrd++) = (unsigned char)*(ptrR++);
		*(xptrd++) = (unsigned char)*(ptrG++);
		*(xptrd++) = (unsigned char)*(ptrB++);
	  }
	  cimg::fwrite(ptrd,buf_size,file);
	  delete[] ptrd;
	} else {             // Binary PPM 16 bits
	  unsigned short *ptrd = new unsigned short[buf_size], *xptrd = ptrd;
	  cimg_mapXY(*this,x,y) {
		*(xptrd++) = (unsigned short)*(ptrR++);
		*(xptrd++) = (unsigned short)*(ptrG++);
		*(xptrd++) = (unsigned short)*(ptrB++);
	  }
	  if (!cimg::endian()) cimg::endian_swap(ptrd,buf_size);
	  cimg::fwrite(ptrd,buf_size,file);
	  delete[] ptrd;
	}
	  } break;
	  }
	  cimg::fclose(file);
	  
	  return *this;
	}

	//! Save an image as a Dicom file (need '(X)Medcon' : http://xmedcon.sourceforge.net )
	const CImg& save_dicom(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_dicom() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_dicom() : Specified filename is (null).",pixel_type());
	  static bool first_time = true;
	  char command[1024], filetmp[512], body[512];
	  if (first_time) { std::srand((unsigned int)::time(0)); first_time = false; }
	  std::FILE *file = 0;
	  do { 
	if (file) std::fclose(file);
	std::sprintf(filetmp,"CImg%.4d.hdr",std::rand()%10000);
	file = std::fopen(filetmp,"rb");
	  } while (file);
	  save_analyze(filetmp);
	  std::sprintf(command,"\"%s\" -w -c dicom -o \"%s\" -f \"%s\"",cimg::medcon_path(),filename,filetmp);
	  cimg::system(command);
	  std::remove(filetmp);
	  cimg::filename_split(filetmp,body);
	  std::sprintf(filetmp,"%s.img",body);
	  std::remove(filetmp);
	  std::sprintf(command,"m000-%s",filename);
	  file = std::fopen(command,"rb");
	  if (!file) {
		std::fclose(cimg::fopen(filename,"r"));
		throw CImgIOException("CImg<%s>::save_dicom() : Failed to save image '%s' with 'medcon'.\n"
				  "Check that you have installed the XMedCon package in a standard directory.",
				  pixel_type(),filename);
	  } else cimg::fclose(file);
	  std::rename(command,filename);
	  return *this;
	}
	
	//! Save the image as an ANALYZE7.5 file.
	const CImg& save_analyze(const char *filename,const float *const voxsize=0) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_analyze() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_analyze() : Specified filename is (null).",pixel_type());
	  std::FILE *file;
	  char header[348],hname[1024],iname[1024];
	  const char *ext = cimg::filename_split(filename);
	  short datatype=-1;
	  std::memset(header,0,348);
	  if (!ext[0]) { std::sprintf(hname,"%s.hdr",filename); std::sprintf(iname,"%s.img",filename); }
	  if (!cimg::strncasecmp(ext,"hdr",3)) { 
	std::strcpy(hname,filename); std::strcpy(iname,filename); std::sprintf(iname+cimg::strlen(iname)-3,"img"); 
	  }
	  if (!cimg::strncasecmp(ext,"img",3)) {
	std::strcpy(hname,filename); std::strcpy(iname,filename); std::sprintf(hname+cimg::strlen(iname)-3,"hdr"); 
	  }
	  ((int*)(header))[0] = 348;
	  std::sprintf(header+4,"CImg");
	  std::sprintf(header+14," ");
	  ((short*)(header+36))[0] = 4096;
	  ((char*)(header+38))[0] = 114;
	  ((short*)(header+40))[0] = 4;
	  ((short*)(header+40))[1] = width;
	  ((short*)(header+40))[2] = height;
	  ((short*)(header+40))[3] = depth;
	  ((short*)(header+40))[4] = dim;      
	  if (!cimg::strcasecmp(pixel_type(),"bool"))           datatype = 2;
	  if (!cimg::strcasecmp(pixel_type(),"unsigned char"))  datatype = 2;
	  if (!cimg::strcasecmp(pixel_type(),"char"))           datatype = 2;
	  if (!cimg::strcasecmp(pixel_type(),"unsigned short")) datatype = 4;
	  if (!cimg::strcasecmp(pixel_type(),"short"))          datatype = 4;
	  if (!cimg::strcasecmp(pixel_type(),"unsigned int"))   datatype = 8;
	  if (!cimg::strcasecmp(pixel_type(),"int"))            datatype = 8;
	  if (!cimg::strcasecmp(pixel_type(),"unsigned long"))  datatype = 8;
	  if (!cimg::strcasecmp(pixel_type(),"long"))           datatype = 8;
	  if (!cimg::strcasecmp(pixel_type(),"float"))          datatype = 16;
	  if (!cimg::strcasecmp(pixel_type(),"double"))         datatype = 64;
	  if (datatype<0)
	throw CImgIOException("CImg<%s>::save_analyze() : Cannot save image '%s' since pixel type (%s)"
				  "is not handled in Analyze7.5 specifications.\n",
				  pixel_type(),filename,pixel_type());
	  ((short*)(header+70))[0] = datatype;
	  ((short*)(header+72))[0] = sizeof(T);
	  ((float*)(header+112))[0] = 1;
	  ((float*)(header+76))[0] = 0;
	  if (voxsize) {
		((float*)(header+76))[1] = voxsize[0];
		((float*)(header+76))[2] = voxsize[1];
		((float*)(header+76))[3] = voxsize[2];
	  } else ((float*)(header+76))[1] = ((float*)(header+76))[2] = ((float*)(header+76))[3] = 1;
	  file = cimg::fopen(hname,"wb");
	  cimg::fwrite(header,348,file);
	  cimg::fclose(file);
	  file = cimg::fopen(iname,"wb");
	  cimg::fwrite(data,size(),file);
	  cimg::fclose(file);
	  return *this;
	}

	//! Save the image as a CImg RAW file
	const CImg& save_cimg(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_cimg() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_cimg() : Specified filename is (null).",pixel_type());
	  CImgl<T> tmp(1);
	  tmp[0].width = width;
	  tmp[0].height = height;
	  tmp[0].depth = depth;
	  tmp[0].dim = dim;
	  tmp[0].data = data;
	  tmp.save_cimg(filename);
	  tmp[0].width = tmp[0].height = tmp[0].depth = tmp[0].dim = 0;
	  tmp[0].data = 0;
	  return *this;
	}

	//! Save the image as a RAW file
	const CImg& save_raw(const char *filename, const bool multiplexed=false) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_raw() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_raw() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  if (!multiplexed) cimg::fwrite(data,size(),file);
	  else {
	CImg<T> buf(dim);
	cimg_mapXYZ(*this,x,y,z) {
	  cimg_mapV(*this,k) buf[k] = (*this)(x,y,z,k);
	  cimg::fwrite(buf.data,dim,file);
	}
	  }
	  cimg::fclose(file);
	  return *this;
	}
	
	//! Save the image using ImageMagick's convert.    
	/** Function that saves the image for other file formats that are not natively handled by CImg,
	using the tool 'convert' from the ImageMagick package.\n
	This is the case for all compressed image formats (GIF,PNG,JPG,TIF,...). You need to install 
	the ImageMagick package in order to get
	this function working properly (see http://www.imagemagick.org ).
	**/
	const CImg& save_convert(const char *filename, const unsigned int quality=100) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_convert() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_convert() : Specified filename is (null).",pixel_type());
	  static bool first_time = true;
	  char command[512],filetmp[512];
	  if (first_time) { std::srand((unsigned int)::time(0)); first_time = false; }
	  std::FILE *file = 0;
	  do {
	if (file) std::fclose(file);
	std::sprintf(filetmp,"%s/CImg%.4d.ppm",cimg::temporary_path(),std::rand()%10000);
	file = std::fopen(filetmp,"rb");
	  } while (file);
	  save_pnm(filetmp);
	  std::sprintf(command,"\"%s\" -quality %u%% %s \"%s\"",cimg::convert_path(),quality,filetmp,filename);
	  cimg::system(command);
	  file = std::fopen(filename,"rb");
	  if (!file) throw CImgIOException("CImg<%s>::save_convert() : Failed to save image '%s' with 'convert'.\n"
					   "Check that you have installed the ImageMagick package in a standard directory.",
					   pixel_type(),filename);
	  if (file) cimg::fclose(file);
	  std::remove(filetmp);
	  return *this;
	}
	
	//! Save the image as an INRIMAGE-4 file.
	const CImg& save_inr(const char *filename,const float *const voxsize = 0) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_inr() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_inr() : Specified filename is (null).",pixel_type());
	  int inrpixsize=-1;
	  const char *inrtype = "unsigned fixed\nPIXSIZE=8 bits\nSCALE=2**0";
	  if (!cimg::strcasecmp(pixel_type(),"unsigned char"))  { inrtype = "unsigned fixed\nPIXSIZE=8 bits\nSCALE=2**0"; inrpixsize = 1; }
	  if (!cimg::strcasecmp(pixel_type(),"char"))           { inrtype = "fixed\nPIXSIZE=8 bits\nSCALE=2**0"; inrpixsize = 1; }
	  if (!cimg::strcasecmp(pixel_type(),"unsigned short")) { inrtype = "unsigned fixed\nPIXSIZE=16 bits\nSCALE=2**0";inrpixsize = 2; }
	  if (!cimg::strcasecmp(pixel_type(),"short"))          { inrtype = "fixed\nPIXSIZE=16 bits\nSCALE=2**0"; inrpixsize = 2; }
	  if (!cimg::strcasecmp(pixel_type(),"unsigned int"))   { inrtype = "unsigned fixed\nPIXSIZE=32 bits\nSCALE=2**0";inrpixsize = 4; }
	  if (!cimg::strcasecmp(pixel_type(),"int"))            { inrtype = "fixed\nPIXSIZE=32 bits\nSCALE=2**0"; inrpixsize = 4; }
	  if (!cimg::strcasecmp(pixel_type(),"float"))          { inrtype = "float\nPIXSIZE=32 bits"; inrpixsize = 4; }
	  if (!cimg::strcasecmp(pixel_type(),"double"))         { inrtype = "float\nPIXSIZE=64 bits"; inrpixsize = 8; }
	  if (inrpixsize<=0) throw CImgIOException("CImg<%s>::save_inr() : Don't know how to save images of '%s'",pixel_type(),pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  char header[257];      
	  int err = std::sprintf(header,"#INRIMAGE-4#{\nXDIM=%u\nYDIM=%u\nZDIM=%u\nVDIM=%u\n",width,height,depth,dim);
	  if (voxsize) err += std::sprintf(header+err,"VX=%g\nVY=%g\nVZ=%g\n",voxsize[0],voxsize[1],voxsize[2]);
	  err += std::sprintf(header+err,"TYPE=%s\nCPU=%s\n",inrtype,cimg::endian()?"sun":"decm");
	  std::memset(header+err,'\n',252-err);
	  std::memcpy(header+252,"##}\n",4);
	  cimg::fwrite(header,256,file);
	  cimg_mapXYZ(*this,x,y,z) cimg_mapV(*this,k) cimg::fwrite(&((*this)(x,y,z,k)),1,file);
	  cimg::fclose(file);
	  return *this;
	}

#define cimg_save_pandore_case(sy,sz,sv,stype,id) \
   if (!saved && (sy?(sy==height):true) && (sz?(sz==depth):true) && (sv?(sv==dim):true) && !strcmp(stype,pixel_type())) { \
	  unsigned int *iheader = (unsigned int*)(header+12); \
	  nbdims = _save_pandore_header_length((*iheader=id),dims,colorspace); \
	  cimg::fwrite(header,36,file); \
	  cimg::fwrite(dims,nbdims,file); \
	  if (id==2 || id==5 || id==8 || id==16 || id==19 || id==22 || id==26 || id==30) { \
	unsigned char *buffer = new unsigned char[size()]; \
	const T *ptrs = ptr(); \
	cimg_mapoff(*this,off) *(buffer++)=(unsigned char)(*(ptrs++)); \
	buffer-=size(); \
	cimg::fwrite(buffer,size(),file); \
	delete[] buffer; \
	  } \
	  if (id==3 || id==6 || id==9 || id==17 || id==20 || id==23 || id==27 || id==31) { \
	unsigned long *buffer = new unsigned long[size()]; \
	const T *ptrs = ptr(); \
	cimg_mapoff(*this,off) *(buffer++)=(long)(*(ptrs++)); \
	buffer-=size(); \
	cimg::fwrite(buffer,size(),file); \
	delete[] buffer; \
	  } \
	  if (id==4 || id==7 || id==10 || id==18 || id==21 || id==25 || id==29 || id==33) { \
	float *buffer = new float[size()]; \
	const T *ptrs = ptr(); \
	cimg_mapoff(*this,off) *(buffer++)=(float)(*(ptrs++)); \
	buffer-=size(); \
	cimg::fwrite(buffer,size(),file); \
	delete[] buffer; \
	  } \
	  saved = true; \
	}

	unsigned int _save_pandore_header_length(unsigned int id,unsigned int *dims,const unsigned int colorspace=0) const {
	  unsigned int nbdims=0;
	  if (id==2 || id==3 || id==4)    { dims[0]=1; dims[1]=width; nbdims=2; }
	  if (id==5 || id==6 || id==7)    { dims[0]=1; dims[1]=height; dims[2]=width; nbdims=3; }
	  if (id==8 || id==9 || id==10)   { dims[0]=dim; dims[1]=depth; dims[2]=height; dims[3]=width; nbdims=4; }
	  if (id==16 || id==17 || id==18) { dims[0]=3; dims[1]=height; dims[2]=width; dims[3]=colorspace; nbdims=4; }
	  if (id==19 || id==20 || id==21) { dims[0]=3; dims[1]=depth; dims[2]=height; dims[3]=width; dims[4]=colorspace; nbdims=5; }
	  if (id==22 || id==23 || id==25) { dims[0]=dim; dims[1]=width; nbdims=2; }
	  if (id==26 || id==27 || id==29) { dims[0]=dim; dims[1]=height; dims[2]=width; nbdims=3; }
	  if (id==30 || id==31 || id==33) { dims[0]=dim; dims[1]=depth; dims[2]=height; dims[3]=width; nbdims=4; }
	  return nbdims;
	}    

	//! Save the image as a PANDORE-5 file.
	const CImg& save_pandore(const char* filename, const unsigned int colorspace=0) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_pandore() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_pandore() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  unsigned char header[36] = { 'P','A','N','D','O','R','E','0','4',0,0,0,
				   0,0,0,0,
				   'C','I','m','g',0,0,0,0,0,
				   'N','o',' ','d','a','t','e',0,0,0,
				   0 };
	  unsigned int nbdims,dims[5];
	  bool saved=false;
	  cimg_save_pandore_case(1,1,1,"unsigned char",2);
	  cimg_save_pandore_case(1,1,1,"char",3);
	  cimg_save_pandore_case(1,1,1,"short",3);
	  cimg_save_pandore_case(1,1,1,"unsigned short",3);
	  cimg_save_pandore_case(1,1,1,"unsigned int",3);
	  cimg_save_pandore_case(1,1,1,"int",3);
	  cimg_save_pandore_case(1,1,1,"unsigned long",4);
	  cimg_save_pandore_case(1,1,1,"long",3);
	  cimg_save_pandore_case(1,1,1,"float",4);
	  cimg_save_pandore_case(1,1,1,"double",4);
 
	  cimg_save_pandore_case(0,1,1,"unsigned char",5);
	  cimg_save_pandore_case(0,1,1,"char",6);
	  cimg_save_pandore_case(0,1,1,"short",6);
	  cimg_save_pandore_case(0,1,1,"unsigned short",6);
	  cimg_save_pandore_case(0,1,1,"unsigned int",6);
	  cimg_save_pandore_case(0,1,1,"int",6);
	  cimg_save_pandore_case(0,1,1,"unsigned long",7);
	  cimg_save_pandore_case(0,1,1,"long",6);
	  cimg_save_pandore_case(0,1,1,"float",7);
	  cimg_save_pandore_case(0,1,1,"double",7);

	  cimg_save_pandore_case(0,0,1,"unsigned char",8);
	  cimg_save_pandore_case(0,0,1,"char",9);
	  cimg_save_pandore_case(0,0,1,"short",9);
	  cimg_save_pandore_case(0,0,1,"unsigned short",9);
	  cimg_save_pandore_case(0,0,1,"unsigned int",9);
	  cimg_save_pandore_case(0,0,1,"int",9);
	  cimg_save_pandore_case(0,0,1,"unsigned long",10);
	  cimg_save_pandore_case(0,0,1,"long",9);
	  cimg_save_pandore_case(0,0,1,"float",10);
	  cimg_save_pandore_case(0,0,1,"double",10);
	  
	  cimg_save_pandore_case(0,1,3,"unsigned char",16);
	  cimg_save_pandore_case(0,1,3,"char",17);
	  cimg_save_pandore_case(0,1,3,"short",17);
	  cimg_save_pandore_case(0,1,3,"unsigned short",17);
	  cimg_save_pandore_case(0,1,3,"unsigned int",17);
	  cimg_save_pandore_case(0,1,3,"int",17);
	  cimg_save_pandore_case(0,1,3,"unsigned long",18);
	  cimg_save_pandore_case(0,1,3,"long",17);
	  cimg_save_pandore_case(0,1,3,"float",18);
	  cimg_save_pandore_case(0,1,3,"double",18);

	  cimg_save_pandore_case(0,0,3,"unsigned char",19);
	  cimg_save_pandore_case(0,0,3,"char",20);
	  cimg_save_pandore_case(0,0,3,"short",20);
	  cimg_save_pandore_case(0,0,3,"unsigned short",20);
	  cimg_save_pandore_case(0,0,3,"unsigned int",20);
	  cimg_save_pandore_case(0,0,3,"int",20);
	  cimg_save_pandore_case(0,0,3,"unsigned long",21);
	  cimg_save_pandore_case(0,0,3,"long",20);
	  cimg_save_pandore_case(0,0,3,"float",21);
	  cimg_save_pandore_case(0,0,3,"double",21);
	 
	  cimg_save_pandore_case(1,1,0,"unsigned char",22);
	  cimg_save_pandore_case(1,1,0,"char",23);
	  cimg_save_pandore_case(1,1,0,"short",23);
	  cimg_save_pandore_case(1,1,0,"unsigned short",23);
	  cimg_save_pandore_case(1,1,0,"unsigned int",23);
	  cimg_save_pandore_case(1,1,0,"int",23);
	  cimg_save_pandore_case(1,1,0,"unsigned long",25);
	  cimg_save_pandore_case(1,1,0,"long",23);
	  cimg_save_pandore_case(1,1,0,"float",25);
	  cimg_save_pandore_case(1,1,0,"double",25);
 
	  cimg_save_pandore_case(0,1,0,"unsigned char",26);
	  cimg_save_pandore_case(0,1,0,"char",27);
	  cimg_save_pandore_case(0,1,0,"short",27);
	  cimg_save_pandore_case(0,1,0,"unsigned short",27);
	  cimg_save_pandore_case(0,1,0,"unsigned int",27);
	  cimg_save_pandore_case(0,1,0,"int",27);
	  cimg_save_pandore_case(0,1,0,"unsigned long",29);
	  cimg_save_pandore_case(0,1,0,"long",27);
	  cimg_save_pandore_case(0,1,0,"float",29);
	  cimg_save_pandore_case(0,1,0,"double",29);

	  cimg_save_pandore_case(0,0,0,"unsigned char",30);
	  cimg_save_pandore_case(0,0,0,"char",31);
	  cimg_save_pandore_case(0,0,0,"short",31);
	  cimg_save_pandore_case(0,0,0,"unsigned short",31);
	  cimg_save_pandore_case(0,0,0,"unsigned int",31);
	  cimg_save_pandore_case(0,0,0,"int",31);
	  cimg_save_pandore_case(0,0,0,"unsigned long",33);
	  cimg_save_pandore_case(0,0,0,"long",31);
	  cimg_save_pandore_case(0,0,0,"float",33);
	  cimg_save_pandore_case(0,0,0,"double",33);

	  cimg::fclose(file);
	  return *this;
	}

	//! Save the image as a YUV file
	const CImg& save_yuv(const char *filename, const bool rgb2yuv=true) const {
	  CImgl<T>(*this).save_yuv(filename,rgb2yuv);
	  return *this;
	}
	
	//! Save the image as a BMP file
	const CImg& save_bmp(const char* filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_bmp() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_bmp() : Specified filename is (null).",pixel_type());

	  std::FILE *file = cimg::fopen(filename,"wb");
	  unsigned char header[54]={0}, align_buf[4]={0};
	  const unsigned int 
	align     = (4-(3*width)%4)%4,
	buf_size  = (3*width+align)*dimy(),
	file_size = 54+buf_size;
	  header[0] = 'B'; header[1] = 'M';
	  header[0x02]=file_size&0xFF; header[0x03]=(file_size>>8)&0xFF;
	  header[0x04]=(file_size>>16)&0xFF; header[0x05]=(file_size>>24)&0xFF;
	  header[0x0A]=0x36;
	  header[0x0E]=0x28;
	  header[0x12]=width&0xFF; header[0x13]=(width>>8)&0xFF;
	  header[0x14]=(width>>16)&0xFF; header[0x15]=(width>>24)&0xFF;
	  header[0x16]=height&0xFF; header[0x17]=(height>>8)&0xFF;
	  header[0x18]=(height>>16)&0xFF; header[0x19]=(height>>24)&0xFF;
	  header[0x1A]=1;  header[0x1B]=0;
	  header[0x1C]=24; header[0x1D]=0;
	  header[0x22]=buf_size&0xFF; header[0x23]=(buf_size>>8)&0xFF;
	  header[0x24]=(buf_size>>16)&0xFF; header[0x25]=(buf_size>>24)&0xFF;
	  header[0x27]=0x1; header[0x2B]=0x1;
	  cimg::fwrite(header,54,file);

	  const T
	*pR = ptr(0,height-1,0,0),
	*pG = (dim>=2)?ptr(0,height-1,0,1):pR, 
	*pB = (dim>=3)?ptr(0,height-1,0,2):pR;

	  cimg_mapY(*this,y) {
	cimg_mapX(*this,x) {
	  std::fputc((unsigned char)(*(pB++)),file);
	  std::fputc((unsigned char)(*(pG++)),file);
	  std::fputc((unsigned char)(*(pR++)),file);
	}
	std::fwrite(align_buf,1,align,file);
	pR-=2*width; pG-=2*width; pB-=2*width;	
	  }      
	  cimg::fclose(file);
	  return *this;
	}
	
	//! Save an image to a PNG file.
	// Most of this function has been written by Eric Fausett
	/**
	   \param filename = name of the png image file to load
	   \return *this
	   \note The png format specifies a variety of possible data formats.  Grey scale, Grey
	   scale with Alpha, RGB color, RGB color with Alpha, and Palletized color are supported.
	   Per channel bit depths of 1, 2, 4, 8, and 16 are natively supported. The
	   type of file saved depends on the number of channels in the CImg file.  If there is 4 or more
	   channels, the image will be saved as an RGB color with Alpha image using the bottom 4 channels.
	   If there are 3 channels, the saved image will be an RGB color image.  If 2 channels then the
	   image saved will be Grey scale with Alpha, and if 1 channel will be saved as a Grey scale
	   image.
	**/
	const CImg& save_png(const char* filename) const {
#ifndef cimg_use_png
#ifndef cimg_use_magick
	  return save_convert(filename);
#else
	  return save_magick(filename);
#endif
#else
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_png() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_png() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  
	  // Setup PNG structures for write
	  png_voidp user_error_ptr=0;
	  png_error_ptr user_error_fn=0, user_warning_fn=0;
	  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
							user_error_ptr, user_error_fn, user_warning_fn);
	  if(!png_ptr){
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::save_png() : trouble initializing 'png_ptr' data structure",pixel_type());
	  }
	  png_infop info_ptr = png_create_info_struct(png_ptr);
	  if(!info_ptr){
	png_destroy_write_struct(&png_ptr,(png_infopp)0);
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::save_png() : trouble initializing 'info_ptr' data structure",pixel_type());
	  }
	  if (setjmp(png_jmpbuf(png_ptr))){
	png_destroy_write_struct(&png_ptr, &info_ptr);
	cimg::fclose(file);
	throw CImgIOException("CImg<%s>::save_png() : unspecified error reading PNG file '%s'",pixel_type(),filename);
	  }
	  
	  png_init_io(png_ptr, file);      
	  png_uint_32 width = dimx();
	  png_uint_32 height = dimy();
	  const CImgStats stats(*this,false);
	  const float vmin = (float)stats.min, vmax = (float)stats.max;
	  const int bit_depth = (vmin<0 || vmax>=256)?16:8;
	  int color_type;
	  switch (dimv()) {
	  case 1: color_type = PNG_COLOR_TYPE_GRAY; break;
	  case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
	  case 3: color_type = PNG_COLOR_TYPE_RGB; break;
	  default: color_type = PNG_COLOR_TYPE_RGB_ALPHA;
	  }
	  const int interlace_type = PNG_INTERLACE_NONE;
	  const int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
	  const int filter_method = PNG_FILTER_TYPE_DEFAULT;
	  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type,
		   compression_type, filter_method);
	  png_write_info(png_ptr, info_ptr);
	  const int byte_depth = bit_depth>>3;
	  const int numChan = dimv()>4?4:dimv();
	  const int pixel_bit_depth_flag = numChan * (bit_depth-1);
	  
	  // Allocate Memory for Image Save and Fill pixel data
	  png_bytep *imgData = new png_byte*[height];
	  for(unsigned int row=0; row<height; row++) imgData[row] = new png_byte[byte_depth * numChan * width];
	  const T *pC0 = ptr(0,0,0,0);
	  switch(pixel_bit_depth_flag) {
	  case 7 :	{ // Gray 8-bit
	cimg_mapY(*this,y) {
	  unsigned char *ptrs = imgData[y];
	  cimg_mapX(*this,x) *(ptrs++) = (unsigned char)*(pC0++);
	}
	  } break;
	  case 14: { // Gray w/ Alpha 8-bit
	const T *pC1 = ptr(0,0,0,1);
	cimg_mapY(*this,y) {
	  unsigned char *ptrs = imgData[y];
	  cimg_mapX(*this,x) {
		*(ptrs++) = (unsigned char)*(pC0++);
		*(ptrs++) = (unsigned char)*(pC1++);
	  }
	}
	  } break;
	  case 21:	{ // RGB 8-bit
	const T *pC1 = ptr(0,0,0,1);
	const T *pC2 = ptr(0,0,0,2);
	cimg_mapY(*this,y) {
	  unsigned char *ptrs = imgData[y];
	  cimg_mapX(*this,x) {
		*(ptrs++) = (unsigned char)*(pC0++);
		*(ptrs++) = (unsigned char)*(pC1++);
		*(ptrs++) = (unsigned char)*(pC2++);
	  }
	}
	  }	break;
	  case 28: { // RGB x/ Alpha 8-bit
	const T *pC1 = ptr(0,0,0,1);
	const T *pC2 = ptr(0,0,0,2);
	const T *pC3 = ptr(0,0,0,3);
	cimg_mapY(*this,y){
	  unsigned char *ptrs = imgData[y];
	  cimg_mapX(*this,x){
		*(ptrs++) = (unsigned char)*(pC0++);
		*(ptrs++) = (unsigned char)*(pC1++);
		*(ptrs++) = (unsigned char)*(pC2++);
		*(ptrs++) = (unsigned char)*(pC3++);
	  }
	}
	  }	break;
	  case 15: { // Gray 16-bit
	cimg_mapY(*this,y){
	  unsigned short *ptrs = reinterpret_cast<unsigned short*>(imgData[y]);
	  cimg_mapX(*this,x) *(ptrs++) = (unsigned short)*(pC0++);
	}
	  }	break;
	  case 30: { // Gray w/ Alpha 16-bit
	const T *pC1 = ptr(0,0,0,1);
	cimg_mapY(*this,y){
	  unsigned short *ptrs = reinterpret_cast<unsigned short*>(imgData[y]);
	  cimg_mapX(*this,x) {
		*(ptrs++) = (unsigned short)*(pC0++);
		*(ptrs++) = (unsigned short)*(pC1++);
	  }
	}
	  }	break;
	  case 45: { // RGB 16-bit
	const T *pC1 = ptr(0,0,0,1);
	const T *pC2 = ptr(0,0,0,2);
	cimg_mapY(*this,y) {
	  unsigned short *ptrs = reinterpret_cast<unsigned short*>(imgData[y]);
	  cimg_mapX(*this,x) {
		*(ptrs++) = (unsigned short)*(pC0++);
		*(ptrs++) = (unsigned short)*(pC1++);
		*(ptrs++) = (unsigned short)*(pC2++);
	  }
	}
	  }	break;
	  case 60: { // RGB w/ Alpha 16-bit
	const T *pC1 = ptr(0,0,0,1);
	const T *pC2 = ptr(0,0,0,2);
	const T *pC3 = ptr(0,0,0,3);
	cimg_mapY(*this,y) {
	  unsigned short *ptrs = reinterpret_cast<unsigned short*>(imgData[y]);
	  cimg_mapX(*this,x) {
		*(ptrs++) = (unsigned short)*(pC0++);
		*(ptrs++) = (unsigned short)*(pC1++);
		*(ptrs++) = (unsigned short)*(pC2++);
		*(ptrs++) = (unsigned short)*(pC3++);
	  }
	}
	  }	break;
	  default:
	throw CImgIOException("CImg<%s>::save_png() : unspecified error reading PNG file '%s'",pixel_type(),filename);
	break;
	  }
	  png_write_image(png_ptr, imgData);      
	  png_write_end(png_ptr, info_ptr);      
	  png_destroy_write_struct(&png_ptr, &info_ptr);
	  
	  // Deallocate Image Write Memory
	  for (unsigned int n=0; n<height; n++) delete[] imgData[n];
	  delete[] imgData;      
	  cimg::fclose(file);
	  return *this;
#endif
	}

	//! Save a file in JPEG format.
	const CImg<T>& save_jpeg(const char *filename, const unsigned int quality=100) const {
#ifndef cimg_use_jpeg
#ifndef cimg_use_magick
	  return save_convert(filename,quality);
#else
	  return save_magick(filename);
#endif
#else
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_jpeg() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_jpeg() : Specified filename is (null).",pixel_type());

	  // Fill pixel buffer
	  unsigned char *buf;      
	  unsigned int dimbuf=0;
	  J_COLOR_SPACE colortype=JCS_RGB;
	  switch (dim) {
	  case 1: { // Greyscale images
	unsigned char *buf2 = buf = new unsigned char[width*height*(dimbuf=1)];
	colortype = JCS_GRAYSCALE;
	const T *ptr_g = ptr();
	cimg_mapXY(*this,x,y) *(buf2++) = (unsigned char)*(ptr_g++);
	  } break;
	  case 2:
	  case 3: { // RGB images
	unsigned char *buf2 = buf = new unsigned char[width*height*(dimbuf=3)];
	const T *ptr_r = ptr(0,0,0,0), *ptr_g = ptr(0,0,0,1), *ptr_b = ptr(0,0,0,dim>2?2:0);
	colortype = JCS_RGB;
	cimg_mapXY(*this,x,y) {
	  *(buf2++) = (unsigned char)*(ptr_r++);
	  *(buf2++) = (unsigned char)*(ptr_g++);
	  *(buf2++) = (unsigned char)*(ptr_b++);
	}
	  } break;
	  default: { // YCMYK images
	unsigned char *buf2 = buf = new unsigned char[width*height*(dimbuf=4)];
	const T *ptr_r = ptr(0,0,0,0), *ptr_g = ptr(0,0,0,1), *ptr_b = ptr(0,0,0,2), *ptr_a = ptr(0,0,0,3);
	colortype = JCS_CMYK;
	cimg_mapXY(*this,x,y) {
	  *(buf2++) = (unsigned char)*(ptr_r++);
	  *(buf2++) = (unsigned char)*(ptr_g++);
	  *(buf2++) = (unsigned char)*(ptr_b++);
	  *(buf2++) = (unsigned char)*(ptr_a++);
	}
	  } break;
	  }
	  
	  // Call libjpeg functions      
	  struct jpeg_compress_struct cinfo;
	  struct jpeg_error_mgr jerr;
	  cinfo.err = jpeg_std_error(&jerr);
	  jpeg_create_compress(&cinfo);
	  std::FILE *file = cimg::fopen(filename,"wb");
	  jpeg_stdio_dest(&cinfo,file);
	  cinfo.image_width = width;
	  cinfo.image_height = height;
	  cinfo.input_components = dimbuf;
	  cinfo.in_color_space = colortype;
	  jpeg_set_defaults(&cinfo);
	  jpeg_set_quality(&cinfo,quality<100?quality:100,TRUE);
	  jpeg_start_compress(&cinfo,TRUE);
	  
	  const unsigned int row_stride = width*dimbuf;
	  JSAMPROW row_pointer[1];
	  while (cinfo.next_scanline < cinfo.image_height) {
	row_pointer[0] = &buf[cinfo.next_scanline*row_stride];
	jpeg_write_scanlines(&cinfo,row_pointer,1);
	  }
	  jpeg_finish_compress(&cinfo);

	  delete[] buf;
	  cimg::fclose(file);
	  jpeg_destroy_compress(&cinfo);
	  return *this;
#endif
	}
   
	//! Save the image using built-in ImageMagick++ library
#ifdef cimg_use_magick
	const CImg& save_magick(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_magick() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_magick() : Specified filename is (null).",pixel_type());
	  Magick::Image image(Magick::Geometry(width,height),"black");
	  image.type(Magick::TrueColorType);
	  const T *rdata = ptr(0,0,0,0), *gdata = dim>1?ptr(0,0,0,1):rdata, *bdata = dim>2?ptr(0,0,0,2):gdata;
	  cimg_mapXY(*this,x,y) image.pixelColor(x,y,Magick::ColorRGB(*(rdata++)/255.0,*(gdata++)/255.0,*(bdata++)/255.0));
	  image.syncPixels();
	  image.write(filename);
	  return *this;
	}
#endif

	//! Save the image as a RGBA file
	const CImg& save_rgba(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_rgba() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_rgba() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  const unsigned int wh = width*height;
	  unsigned char *buffer = new unsigned char[4*wh], *nbuffer=buffer;
	  const T 
	*ptr1 = ptr(0,0,0,0),
	*ptr2 = dim>1?ptr(0,0,0,1):ptr1,
	*ptr3 = dim>2?ptr(0,0,0,2):ptr1,
	*ptr4 = dim>3?ptr(0,0,0,3):0;
	  for (unsigned int k=0; k<wh; k++) {
	*(nbuffer++) = (unsigned char)(*(ptr1++));
	*(nbuffer++) = (unsigned char)(*(ptr2++));
	*(nbuffer++) = (unsigned char)(*(ptr3++));
	*(nbuffer++) = (unsigned char)(ptr4?(*(ptr4++)):255);
	  }
	  cimg::fwrite(buffer,4*wh,file);
	  cimg::fclose(file);
	  delete[] buffer;
	  return *this;
	}
	

	//! Save the image as a RGB file
	const CImg& save_rgb(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_rgb() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_rgb() : Specified filename is (null).",pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  const unsigned int wh = width*height;
	  unsigned char *buffer = new unsigned char[3*wh], *nbuffer=buffer;
	  const T 
	*ptr1 = ptr(0,0,0,0),
	*ptr2 = dim>1?ptr(0,0,0,1):ptr1,
	*ptr3 = dim>2?ptr(0,0,0,2):ptr1;
	  for (unsigned int k=0; k<wh; k++) {
	*(nbuffer++) = (unsigned char)(*(ptr1++));
	*(nbuffer++) = (unsigned char)(*(ptr2++));
	*(nbuffer++) = (unsigned char)(*(ptr3++));
	  }
	  cimg::fwrite(buffer,3*wh,file);
	  cimg::fclose(file);
	  delete[] buffer;
	  return *this;
	}
	
	//! Get a 40x38 color logo of a 'danger' item
	static CImg get_logo40x38() { 
	  static bool first_time = true;
	  static CImg<T> res(40,38,1,3);
	  if (first_time) {
	const unsigned char *ptrs = cimg::logo40x38;
	T *ptr1 = res.ptr(0,0,0,0), *ptr2 = res.ptr(0,0,0,1), *ptr3 = res.ptr(0,0,0,2);
	for (unsigned int off = 0; off<res.width*res.height;) {
	  const unsigned char n = *(ptrs++), r = *(ptrs++), g = *(ptrs++), b = *(ptrs++);
	  for (unsigned int l=0; l<n; off++,l++) { *(ptr1++) = (T)r; *(ptr2++) = (T)g; *(ptr3++) = (T)b; }
	}
	first_time = false;
	  }      
	  return res;
	}       

	//! Save OFF files (GeomView 3D object files)
	template<typename tf, typename tc>
	const CImg& save_off(const char *filename, const CImgl<tf>& primitives, const CImgl<tc>& colors, const bool invert_faces=false) const {
	  if (is_empty()) throw CImgInstanceException("CImg<%s>::save_off() : Instance image (%u,%u,%u,%u,%p) is empty.",
						  pixel_type(),width,height,depth,dim,data);
	  if (!filename) throw CImgArgumentException("CImg<%s>::save_off() : Specified filename is (null).",pixel_type());
	  std::FILE *file=cimg::fopen(filename,"w");  
	  std::fprintf(file,"OFF\n%u %u %u\n",width,primitives.size,3*primitives.size);
	  cimg_mapX(*this,i) std::fprintf(file,"%f %f %f\n",(float)((*this)(i,0)),(float)((*this)(i,1)),(float)((*this)(i,2)));
	  cimgl_map(primitives,l) {
	const unsigned int prim = primitives[l].size();
	switch (prim) {
	case 3: {
	  if (invert_faces) 
		std::fprintf(file,"3 %u %u %u %f %f %f\n",
			 (unsigned int)primitives(l,0),(unsigned int)primitives(l,1),(unsigned int)primitives(l,2),
			 (float)(colors(l,0)/255.0f),(float)(colors(l,1)/255.0f),(float)(colors(l,2)/255.0f));
	  else
		std::fprintf(file,"3 %u %u %u %f %f %f\n",
			 (unsigned int)primitives(l,0),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1),
			 (float)(colors(l,0)/255.0f),(float)(colors(l,1)/255.0f),(float)(colors(l,2)/255.0f));
	} break;
	case 4: {
	  if (invert_faces)
		std::fprintf(file,"4 %u %u %u %u %f %f %f\n",
			 (unsigned int)primitives(l,0),(unsigned int)primitives(l,1),(unsigned int)primitives(l,2),(unsigned int)primitives(l,3),
			 (float)(colors(l,0)/255.0f),(float)(colors(l,1)/255.0f),(float)(colors(l,2)/255.0f));
	  else
		std::fprintf(file,"4 %u %u %u %u %f %f %f\n",
			 (unsigned int)primitives(l,0),(unsigned int)primitives(l,3),(unsigned int)primitives(l,2),(unsigned int)primitives(l,1),
			 (float)(colors(l,0)/255.0f),(float)(colors(l,1)/255.0f),(float)(colors(l,2)/255.0f));
	} break;
	}
	  }
	  cimg::fclose(file);      
	  return *this;
	}

	//@}
	//---------------------------
	//
	//! \name Plugins functions
	//@{
	//---------------------------
#ifdef cimg_plugin
#include cimg_plugin
#endif
	//@}
  };


  /*
   #-----------------------------------------
   #
   #
   #
   # Definition of the CImgl<> structure
   #
   #
   #
   #------------------------------------------
   */

  //! Class representing list of images CImg<T>.
  template<typename T> struct CImgl {       

	unsigned int size, allocsize;
	const bool is_shared;
	CImg<T> *data;

	//! Define a CImgl<T>::iterator
	typedef CImg<T>* iterator;

	//! Define a CImgl<T>::const_iterator
	typedef const CImg<T>* const_iterator;
	
	//------------------------------------------
	//
	//! \name Constructors - Destructor - Copy
	//@{
	//------------------------------------------

	//! Construct an image list containing n images with specified size
	CImgl(const unsigned int n=0, const unsigned int width=0, const unsigned int height=1,
	  const unsigned int depth=1, const unsigned int dim=1):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(n,width,height,depth,dim);
	}

	//! Construct an image list containing n images with specified size, filled with val
	CImgl(const unsigned int n, const unsigned int width, const unsigned int height,
	  const unsigned int depth, const unsigned int dim, const T& val):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(n,width,height,depth,dim,val);
	}

	//! Construct an image containing n copies of the image img
	template<typename t> CImgl(const unsigned int n, const CImg<t>& img):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(n,img);
	}

	//! Construct an image list from a filename
	CImgl(const char* filename):
	  size(0),allocsize(0),is_shared(false),data(0) { 
	  assign(filename); 
	}
	
	//! Construct an image list from one image
	CImgl(const CImg<T>& img):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img);
	}

	//! Construct an image list from two images
	CImgl(const CImg<T>& img1, const CImg<T>& img2):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2);
	}

	//! Construct an image list from three images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3);
	}

	//! Construct an image list from four images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4);
	}

	//! Construct an image list from five images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5);
	}

	//! Construct an image list from six images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
	  const CImg<T>& img6):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5,img6);
	}

	//! Construct an image list from seven images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
	  const CImg<T>& img6, const CImg<T>& img7):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5,img6,img7);
	}

	//! Construct an image list from eight images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
	  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5,img6,img7,img8);
	}

	//! Construct an image list from nine images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
	  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8, const CImg<T>& img9):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5,img6,img7,img8,img9);
	}

	//! Construct an image list from ten images
	CImgl(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
	  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8, const CImg<T>& img9, const CImg<T>& img10):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(img1,img2,img3,img4,img5,img6,img7,img8,img9,img10);
	}

	//! Construct an image list from a raw image buffer
	template<typename t> CImgl(const CImg<t> *const list_buffer, const unsigned int n):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(list_buffer,n);
	}
	
	CImgl(const CImg<T> *const list_buffer, const unsigned int n, const bool shared_flag):
	  size(0),allocsize(0),is_shared(shared_flag),data(0) {
	  if (is_shared) { size = n; allocsize = 0; data = const_cast<CImg<T>*>(list_buffer); }
	  else assign(list_buffer,n);
	}

	//! Copy constructor
	template<typename t> CImgl(const CImgl<t>& _list):
	  size(0),allocsize(0),is_shared(false),data(0) {
	  assign(_list);
	}

	CImgl(const CImgl& _list):
	  size(0),allocsize(0),is_shared(_list.is_shared),data(0) {
	  if (is_shared) { data = _list.data; size = _list.size; allocsize = 0; }
	  else assign(_list);
	}

	//! Assignement operator
	template<typename t> CImgl& operator=(const CImgl<t>& _list) { 
	  return assign(_list);
	}

	CImgl& operator=(const CImgl& _list) { 
	  return assign(_list);
	}

	//! Destructor
	~CImgl() {
	  if (!is_shared) empty();
	}
	
	//! In-place version of the corresponding constructor
	CImgl& assign(const unsigned int n=0, const unsigned int width=0, const unsigned int height=1,
		  const unsigned int depth=1, const unsigned int dim=1) {
	  if (is_shared) {
	if (n!=size)
	  throw CImgArgumentException("CImgl<%s>::assign() : Cannot assign list with size %u to shared list (%u,%p).",
					  pixel_type(),n,size,data);
	  } else {
	if (n) {
	  if (allocsize<n || allocsize>(n<<2)) {
		if (data) delete[] data;
		data = new CImg<T>[allocsize=cimg::nearest_pow2(n)];
	  }
	  size = n;
	  const unsigned int siz = width*height*depth*dim;
	  if (siz) cimgl_map(*this,l) data[l].assign(width,height,depth,dim);
	} else return empty();
	  }
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const unsigned int n,const unsigned int width,const unsigned int height,
		  const unsigned int depth, const unsigned int dim,const T& val) {
	  assign(n,width,height,depth,dim);
	  cimgl_map(*this,l) data[l].fill(val);
	  return *this;
	}

	//! In-place version of the corresponding constructor
	template<typename t> CImgl& assign(const unsigned int n, const CImg<t>& img) {
	  assign(n,img.width,img.height,img.depth,img.dim);
	  cimgl_map(*this,l) data[l].assign(img);
	  return *this;
	}
	
	//! In-place version of the corresponding constructor
	CImgl& assign(const char *filename) {
	  return load(filename);
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img) { 
	  return assign(1,img);
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2) { 
	  assign(2);
	  data[0] = img1; data[1] = img2;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3) {
	  assign(3);
	  data[0] = img1; data[1] = img2; data[2] = img3;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4) {
	  assign(4);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5) {
	  assign(5);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
		  const CImg<T>& img6) {
	  assign(6);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  data[5] = img6;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
		  const CImg<T>& img6, const CImg<T>& img7) {
	  assign(7);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  data[5] = img6; data[6] = img7;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
		  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8) {
	  assign(8);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  data[5] = img6; data[6] = img7; data[7] = img8;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
		  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8, const CImg<T>& img9) {
	  assign(9);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  data[5] = img6; data[6] = img7; data[7] = img8; data[8] = img9;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	CImgl& assign(const CImg<T>& img1, const CImg<T>& img2, const CImg<T>& img3, const CImg<T>& img4, const CImg<T>& img5,
		  const CImg<T>& img6, const CImg<T>& img7, const CImg<T>& img8, const CImg<T>& img9, const CImg<T>& img10) {
	  assign(10);
	  data[0] = img1; data[1] = img2; data[2] = img3; data[3] = img4; data[4] = img5;
	  data[5] = img6; data[6] = img7; data[7] = img8; data[8] = img9; data[9] = img10;
	  return *this;
	}

	//! In-place version of the corresponding constructor
	template<typename t> CImgl& assign(const CImg<t> *const list_buffer, const unsigned int n) {
	  assign(n);
	  cimgl_map(*this,l) data[l]=list_buffer[l];
	  return *this;
	}

	//! In-place version of the corresponding constructor
	template<typename t> CImgl& assign(const CImgl<t>& list) {
	  return assign(list.data,list.size);
	}

	//! In-place version of the destructor
	CImgl& empty() { 
	  if (is_shared) throw CImgInstanceException("CImgl<%s>::empty() : Cannot empty shared list (%u,%p).",
						 pixel_type(),size,data);
	  else {
	if (data) delete[] data;
	size = allocsize = 0;
	data = 0;
	  }
	  return *this;
	}

	//! Equivalent to empty() (STL-compliant name)
	CImgl& clear() { 
	  return empty(); 
	}
		
	//! Return an empty list
	static CImgl get_empty() {
	  return CImgl<T>(); 
	}

	//@}
	//------------------------------
	//
	//! \name Arithmetics operators
	//@{
	//------------------------------

	//! Return a string describing the type of the image pixels in the list (template parameter \p T).
	static const char* pixel_type() { T val; return cimg::get_type(val); }
	
	//! Return \p true if list is empty
	bool is_empty() const { 
	  return (!data || !size); 
	}

	//! Add each image of the current list with the corresponding image in the list \p list.
	template<typename t> CImgl& operator+=(const CImgl<t>& list) {
	  const unsigned int sizemax = min(size,list.size);
	  for (unsigned int l=0; l<sizemax; l++) (*this)[l]+=list[l];
	  return *this;
	}
	
	//! Subtract each image of the current list with the corresponding image in the list \p list.
	template<typename t> CImgl& operator-=(const CImgl<t>& list) {
	  const unsigned int sizemax = min(size,list.size);
	  for (unsigned int l=0; l<sizemax; l++) (*this)[l]-=list[l];
	  return *this;
	}
	
	//! Add each image of the current list with a value \p val.
	CImgl& operator+=(const T& val) { 
	  cimgl_map(*this,l) (*this)[l]+=val;
	  return *this; 
	}

	//! Substract each image of the current list with a value \p val.
	CImgl& operator-=(const T& val) { 
	  cimgl_map(*this,l) (*this)[l]-=val;
	  return *this; 
	}
	
	//! Multiply each image of the current list by a value \p val.
	CImgl& operator*=(const double val) { 
	  cimgl_map(*this,l) (*this)[l]*=val;
	  return *this; 
	}
	
	//! Divide each image of the current list by a value \p val.
	CImgl& operator/=(const double val) { 
	  cimgl_map(*this,l) (*this)[l]/=val;
	  return *this; 
	}
	
	//! Return a new image list corresponding to the addition of each image of the current list with a value \p val.
	CImgl operator+(const T& val) const { 
	  return CImgl<T>(*this)+=val;
	}
	
	//! Return a new image list corresponding to the multiplication of each image of the current list by a value \p val.
	CImgl operator*(const double val) const {
	  return CImgl<T>(*this)*=val;  
	}
	
	//! Return a new image list corresponding to the substraction of each image of the current list with a value \p val.
	CImgl operator-(const T& val) const { 
	  return CImgl<T>(*this)-=val;  
	}
	
	//! Return a new image list corresponding to the division of each image of the current list by a value \p val.
	CImgl operator/(const double val) const { 
	  return CImgl<T>(*this)/=val;  
	}
	
	//! Return a new image list corresponding to the addition of each image of the current list with the corresponding image in the list \p list.
	CImgl operator+(const CImgl& list) const { 
	  return CImgl<T>(*this)+=list; 
	}

	//! Return a new image list corresponding to the substraction of each image of the current list with the corresponding image in the list \p list.
	CImgl operator-(const CImgl& list) const {
	  return CImgl<T>(*this)-=list; 
	}
	
	//! Return a new image list corresponding to the addition of each image of the current list with a value \p val;
	friend CImgl operator+(const T& val, const CImgl& list) {
	  return CImgl<T>(list)+=val; 
	}
	
	//! Return a new image list corresponding to the scalar multiplication of each image of the current list by a value \p val.
	friend CImgl operator*(const double val, const CImgl& list) {
	  return CImgl<T>(list)*=val; 
	}
	
	//! Return a new image list corresponding to the matrix multiplication of each image of the current list by the matrix \p img
	template<typename t> CImgl<typename cimg::largest<T,t>::type> operator*(const CImg<t>& img) const {
	  typedef typename cimg::largest<T,t>::type restype;
	  CImgl<restype> res(size);
	  cimgl_map(res,l) res[l] = data[l]*img;
	  return res;
	}
  
	//@}
	//-------------------------
	//
	//! \name List operations
	//@{
	//-------------------------
	
	//! Return a reference to the i-th element of the image list.
	CImg<T>& operator[](const unsigned int pos) {
#if cimg_debug>1
	  if (pos>=size) {
	cimg::warn(true,"CImgl<%s>::operator[] : bad list position %u, in a list of %u images",pixel_type(),pos,size);
	return *data;
	  }
#endif
	  return data[pos];
	}
	
	const CImg<T>& operator[](const unsigned int pos) const {
#if cimg_debug>1
	  if (pos>=size) {
	cimg::warn(true,"CImgl<%s>::operator[] : bad list position %u, in a list of %u images",pixel_type(),pos,size);
	return *data;
	  }
#endif
	  return data[pos];
	}
	
	//! Equivalent to CImgl<T>::operator[]
	CImg<T>& operator()(const unsigned int pos) { return (*this)[pos]; }
	const CImg<T>& operator()(const unsigned int pos) const { return (*this)[pos]; }

	//! Return a reference to (x,y,z,v) pixel of the pos-th image of the list
	T& operator()(const unsigned int pos, const unsigned int x, const unsigned int y=0,
		  const unsigned int z=0, const unsigned int v=0) {
	  return (*this)[pos](x,y,z,v);
	}
	const T& operator()(const unsigned int pos, const unsigned int x, const unsigned int y=0,
			const unsigned int z=0, const unsigned int v=0) const {
	  return (*this)[pos](x,y,z,v);
	}

	//! Equivalent to CImgl<T>::operator[], with boundary checking
	CImg<T>& at(const unsigned int pos) {
	  if (pos>=size)
	throw CImgArgumentException("CImgl<%s>::at() : bad list position %u, in a list of %u images",
					pixel_type(),pos,size);
	  return data[pos];
	}

	const CImg<T>& at(const unsigned int pos) const {
	  if (pos>=size)
	throw CImgArgumentException("CImgl<%s>::at() : bad list position %u, in a list of %u images",
					pixel_type(),pos,size);
	  return data[pos];
	}
	
	//! Returns a reference to last element
	CImg<T>& back() { 
	  return (*this)(size-1); 
	}

	const CImg<T>& back() const {
	  return (*this)(size-1); 
	}

	//! Returns a reference to the first element
	CImg<T>& front() { 
	  return *data; 
	}

	const CImg<T>& front() const { 
	  return *data; 
	}
	
	//! Returns an iterator to the beginning of the vector.
	iterator begin() { 
	  return data; 
	}

	const_iterator begin() const {
	  return data; 
	}

	//! Returns an iterator just past the last element.
	iterator end() { 
	  return data + size; 
	}

	const_iterator end() const { 
	  return data + size; 
	}

	//! Insert a copy of the image \p img into the current image list, at position \p pos.
	CImgl& insert(const CImg<T>& img, const unsigned int pos, const bool shared=false) {
	  if (is_shared) 
	throw CImgInstanceException("CImgl<%s>::insert() : Insertion in a shared list is not possible",pixel_type());      
	  if (pos>size) 
	throw CImgArgumentException("CImgl<%s>::insert() : Can't insert at position %u into a list with %u elements",
					pixel_type(),pos,size);
	  CImg<T> *new_data = (++size>allocsize)?new CImg<T>[allocsize?(allocsize<<=1):(allocsize=1)]:0;
	  if (!size || !data) { data = new_data; *data = img; }
	  else {
	if (new_data) { // Insert with reallocation
	  if (pos) std::memcpy(new_data,data,sizeof(CImg<T>)*pos);
	  if (pos!=size-1) std::memcpy(new_data+pos+1,data+pos,sizeof(CImg<T>)*(size-1-pos));
	  std::memset(data,0,sizeof(CImg<T>)*(size-1));
	  delete[] data;
	  data = new_data;
	}
	else if (pos!=size-1) memmove(data+pos+1,data+pos,sizeof(CImg<T>)*(size-1-pos));
	if (shared && !img.is_empty()) {
	  data[pos].width = img.width; data[pos].height = img.height; data[pos].depth = img.depth; data[pos].dim = img.dim;
	  data[pos].is_shared = true; data[pos].data = img.data;
	} else {
	  data[pos].width = data[pos].height = data[pos].depth = data[pos].dim = 0; data[pos].data = 0;
	  data[pos] = img;
	}
	  }
	  return *this;
	}

	//! Insert a copy of the image \p img at the current image list.
	CImgl& insert(const CImg<T>& img) { 
	  return insert(img,size); 
	}
	
	//! Insert n copies of the image \p img into the current image list, at position \p pos.
	CImgl& insert(const unsigned int n, const CImg<T>& img, const unsigned int pos) { 
	  for (unsigned int i=0; i<n; i++) insert(img,pos);
	  return *this;
	}

	//! Insert n copies of the image \p img at the end of the list.
	CImgl& insert(const unsigned int n, const CImg<T>& img) { 
	  for (unsigned int i=0; i<n; i++) insert(img);
	  return *this;
	}

	//! Insert a copy of the image list \p list into the current image list, starting from position \p pos.
	CImgl& insert(const CImgl<T>& list, const unsigned int pos) { 
	  if (this!=&list) cimgl_map(list,l) insert(list[l],pos+l);
	  else insert(CImgl<T>(list),pos);
	  return *this; 
	}

	//! Append a copy of the image list \p list at the current image list.
	CImgl& insert(const CImgl<T>& list) {
	  return insert(list,size); 
	}

	//! Insert n copies of the list \p list at position \p pos of the current list.
	CImgl& insert(const unsigned int n, const CImgl<T>& list, const unsigned int pos) {
	  for (unsigned int i=0; i<n; i++) insert(list,pos);
	  return *this;
	}

	//! Insert n copies of the list at the end of the current list
	CImgl& insert(const unsigned int n, const CImgl<T>& list) {
	  for (unsigned int i=0; i<n; i++) insert(list);
	  return *this;
	}

	//! Insert image \p img at the end of the list.
	CImgl& push_back(const CImg<T>& img) { 
	  return insert(img); 
	}
	
	//! Insert image \p img at the front of the list.
	CImgl& push_front(const CImg<T>& img) { 
	  return insert(img,0); 
	}

	//! Insert list \p list at the end of the current list.
	CImgl& push_back(const CImgl<T>& list) { 
	  return insert(list); 
	}

	//! Insert list \p list at the front of the current list.
	CImgl& push_front(const CImgl<T>& list) {
	  return insert(list,0); 
	}
	
	//! Insert a shared copy of the image \p img into the current image list, at position \p pos.
	CImgl& insert_shared(const CImg<T>& img, const unsigned int pos) {
	  return insert(img,pos,true);
	}

	//! Insert a shared copy of the image \p img at the current image list.
	CImgl& insert_shared(const CImg<T>& img) { 
	  return insert_shared(img,size); 
	}

	//! Insert n shared copies of the image \p img into the current image list, at position \p pos.
	CImgl& insert_shared(const unsigned int n, const CImg<T>& img, const unsigned int pos) { 
	  for (unsigned int i=0; i<n; i++) insert_shared(img,pos);
	  return *this;
	}
	
	//! Insert n shared copies of the image \p img at the end of the list.
	CImgl& insert_shared(const unsigned int n, const CImg<T>& img) { 
	  for (unsigned int i=0; i<n; i++) insert_shared(img);
	  return *this;
	}

	//! Insert a shared copy of all image of the list \p list into the current image list, starting from position \p pos.
	CImgl& insert_shared(const CImgl<T>& list, const unsigned int pos) { 
	  if (this!=&list) cimgl_map(list,l) insert_shared(list[l],pos+l);
	  else insert_shared(CImgl<T>(list),pos);
	  return *this; 
	}

	//! Append a shared copy of the image list \p list at the current image list.
	CImgl& insert_shared(const CImgl<T>& list) {
	  return insert_shared(list,size); 
	}

	//! Insert n shared copies of the list \p list at position \p pos of the current list.
	CImgl& insert_shared(const unsigned int n, const CImgl<T>& list, const unsigned int pos) {
	  for (unsigned int i=0; i<n; i++) insert_shared(list,pos);
	  return *this;
	}
	
	//! Insert n shared copies of the list \p list at the end of the list
	CImgl& insert_shared(const unsigned int n, const CImgl<T>& list) {
	  return insert_shared(n,list,size);
	}

	//! Remove the image at position \p pos from the image list.
	CImgl& remove(const unsigned int pos) {
	  if (is_shared)
	throw CImgInstanceException("CImgl<%s>::remove() : Removing from a shared list is not allowed.",pixel_type());
	  if (pos>=size)
	cimg::warn(true,"CImgl<%s>::remove() : Cannot remove an image from a list (%p,%u), at position %u.",
		   pixel_type(),data,size,pos);
	  else {
	data[pos].empty();
	if (!(--size)) return empty();
	if (size<8 || size>(allocsize>>2)) { // Removing item without reallocation.
	  if (pos!=size) { 
		std::memmove(data+pos,data+pos+1,sizeof(CImg<T>)*(size-pos));
		CImg<T> &tmp = data[size];
		tmp.width = tmp.height = tmp.depth = tmp.dim = 0; tmp.data = 0;
	  }
	} else { // Removing item with reallocation.
	  allocsize>>=2;
	  CImg<T> *new_data = new CImg<T>[allocsize];
	  if (pos) std::memcpy(new_data,data,sizeof(CImg<T>)*pos);
	  if (pos!=size) std::memcpy(new_data+pos,data+pos+1,sizeof(CImg<T>)*(size-pos));
	  std::memset(data,0,sizeof(CImg<T>)*(size+1));
	  delete[] data;
	  data = new_data;	
	}
	  }
	  return *this;
	}

	//! Remove last element of the list;
	CImgl& pop_back() { return remove(size-1); }

	//! Remove first element of the list;
	CImgl& pop_front() { return remove(0); }

	//! Remove the element pointed by iterator \p iter;
	CImgl& erase(const iterator iter) { return remove(iter-data); }

	//! Remove the last image from the image list.
	CImgl& remove() { 
	  if (size) return remove(size-1); 
	  else cimg::warn(true,"CImgl<%s>::remove() : List is empty",pixel_type());
	  return *this;
	}

	//! Reverse list order
	CImgl& reverse() {
	  for (unsigned int l=0; l<size/2; l++) (*this)[l].swap((*this)[size-1-l]);
	  return *this;
	}
	
	//! Get reversed list
	CImgl& get_reverse() const { return CImgl<T>(*this).reverse(); }

	//! Insert image at the end of the list
	CImgl& operator<<(const CImg<T>& img) { 
	  return insert(img);
	}
	
	//! Remove last image of the list
	CImgl& operator>>(CImg<T>& img) {
	  if (size) { img = (*this)[size-1]; return remove(size-1); }
	  cimg::warn(true,"CImgl<%s>::operator>>() : List is empty",pixel_type());
	  img.empty();
	  return *this;
	}

	//@}
	//----------------------------
	//
	//! \name Fourier transforms
	//@{
	//----------------------------
	
	//! Compute the Fast Fourier Transform (along the specified axis).
	CImgl& FFT(const char axe, const bool inverse=false) {
	  if (size<2) throw CImgInstanceException("CImg<%s>::FFT() : Instance list have less than 2 images",pixel_type());
	  CImg<T> &Ir = data[0], &Ii = data[1];
	  if (Ir.is_empty())
	throw CImgInstanceException("CImg<%s>::FFT() : Real part (first image of the instance list) is empty.",pixel_type());
	  if (Ii.is_empty())
	throw CImgInstanceException("CImg<%s>::FFT() : Imaginary part (second image of the instance list) is empty.",pixel_type());
	  if (Ir.width!=Ii.width || Ir.height!=Ii.height || Ir.depth!=Ii.depth || Ir.dim!=Ii.dim)
	throw CImgInstanceException("CImg<%s>::FFT() : Real and Imaginary parts of the instance have different dimensions",pixel_type());

	  switch (cimg::uncase(axe)) {
	  case 'x': { // Fourier along X
	const unsigned int N = Ir.width, N2 = (N>>1);
	if (((N-1)&N) && N!=1) throw CImgInstanceException("CImg<%s>::FFT() : Dimension of instance image along 'x' is %d != 2^N",
							   pixel_type(),N);
	for (unsigned int i=0,j=0; i<N2; i++) {
	  if (j>i) cimg_mapYZV(Ir,y,z,v) { cimg::swap(Ir(i,y,z,v),Ir(j,y,z,v)); cimg::swap(Ii(i,y,z,v),Ii(j,y,z,v));
	  if (j<N2) { 
		const unsigned int ri = N-1-i, rj = N-1-j;
		cimg::swap(Ir(ri,y,z,v),Ir(rj,y,z,v)); cimg::swap(Ii(ri,y,z,v),Ii(rj,y,z,v)); 
	  }}
	  for (unsigned int m=N, n=N2; (j+=n)>=m; j-=m, m=n, n>>=1);
	}
	for (unsigned int delta=2; delta<=N; delta<<=1) {
	  const unsigned int delta2 = (delta>>1);
	  for (unsigned int i=0; i<N; i+=delta) {
		float wr = 1, wi = 0;
		const float angle = (float)((inverse?+1:-1)*2*cimg::PI/delta),
			ca = (float)std::cos(angle),
			sa = (float)std::sin(angle);
		for (unsigned int k=0; k<delta2; k++) {
		  const unsigned int j = i + k, nj = j + delta2;
		  cimg_mapYZV(Ir,y,z,k) {
		T &ir = Ir(j,y,z,k), &ii = Ii(j,y,z,k), &nir = Ir(nj,y,z,k), &nii = Ii(nj,y,z,k);	       
		const T tmpr = wr*nir - wi*nii, tmpi = wr*nii + wi*nir;
		nir = ir - tmpr; nii = ii - tmpi;
		ir += tmpr; ii += tmpi;
		  }
		  const float nwr = wr*ca-wi*sa;
		  wi = wi*ca + wr*sa;
		  wr = nwr;	      
		}
	  }	  
	}
	if (inverse) (*this)/=N;
	  } break;

	  case 'y': { // Fourier along Y
	const unsigned int N = Ir.height, N2 = (N>>1);
	if (((N-1)&N) && N!=1) throw CImgInstanceException("CImg<%s>::FFT() : Dimension of instance image(s) along 'y' is %d != 2^N",
							   pixel_type(),N);
	for (unsigned int i=0,j=0; i<N2; i++) {
	  if (j>i) cimg_mapXZV(Ir,x,z,v) { cimg::swap(Ir(x,i,z,v),Ir(x,j,z,v)); cimg::swap(Ii(x,i,z,v),Ii(x,j,z,v));
	  if (j<N2) { 
		const unsigned int ri = N-1-i, rj = N-1-j;
		cimg::swap(Ir(x,ri,z,v),Ir(x,rj,z,v)); cimg::swap(Ii(x,ri,z,v),Ii(x,rj,z,v)); 
	  }}
	  for (unsigned int m=N, n=N2; (j+=n)>=m; j-=m, m=n, n>>=1);
	}
	for (unsigned int delta=2; delta<=N; delta<<=1) {
	  const unsigned int delta2 = (delta>>1);
	  for (unsigned int i=0; i<N; i+=delta) {
		float wr = 1, wi = 0;
		const float angle = (float)((inverse?+1:-1)*2*cimg::PI/delta),
			ca = (float)std::cos(angle), sa = (float)std::sin(angle);
		for (unsigned int k=0; k<delta2; k++) {
		  const unsigned int j = i + k, nj = j + delta2;
		  cimg_mapXZV(Ir,x,z,k) {
		T &ir = Ir(x,j,z,k), &ii = Ii(x,j,z,k), &nir = Ir(x,nj,z,k), &nii = Ii(x,nj,z,k);	       
		const T tmpr = wr*nir - wi*nii, tmpi = wr*nii + wi*nir;
		nir = ir - tmpr; nii = ii - tmpi;
		ir += tmpr; ii += tmpi;
		  }
		  const float nwr = wr*ca-wi*sa;
		  wi = wi*ca + wr*sa;
		  wr = nwr;	      
		}
	  }	  
	}
	if (inverse) (*this)/=N;
	  } break;

	  case 'z': { // Fourier along Z
	const unsigned int N = Ir.depth, N2 = (N>>1);
	if (((N-1)&N) && N!=1) throw CImgInstanceException("CImg<%s>::FFT() : Dimension of instance image(s) along 'z' is %d != 2^N",
							   pixel_type(),N);
	for (unsigned int i=0,j=0; i<N2; i++) {
	  if (j>i) cimg_mapXYV(Ir,x,y,v) { cimg::swap(Ir(x,y,i,v),Ir(x,y,j,v)); cimg::swap(Ii(x,y,i,v),Ii(x,y,j,v));
	  if (j<N2) { 
		const unsigned int ri = N-1-i, rj = N-1-j;
		cimg::swap(Ir(x,y,ri,v),Ir(x,y,rj,v)); cimg::swap(Ii(x,y,ri,v),Ii(x,y,rj,v)); 
	  }}
	  for (unsigned int m=N, n=N2; (j+=n)>=m; j-=m, m=n, n>>=1);
	}
	for (unsigned int delta=2; delta<=N; delta<<=1) {
	  const unsigned int delta2 = (delta>>1);
	  for (unsigned int i=0; i<N; i+=delta) {
		float wr = 1, wi = 0;
		const float angle = (float)((inverse?+1:-1)*2*cimg::PI/delta),
			ca = (float)std::cos(angle), sa = (float)std::sin(angle);
		for (unsigned int k=0; k<delta2; k++) {
		  const unsigned int j = i + k, nj = j + delta2;
		  cimg_mapXYV(Ir,x,y,k) {
		T &ir = Ir(x,y,j,k), &ii = Ii(x,y,j,k), &nir = Ir(x,y,nj,k), &nii = Ii(x,y,nj,k);	       
		const T tmpr = wr*nir - wi*nii, tmpi = wr*nii + wi*nir;
		nir = ir - tmpr; nii = ii - tmpi;
		ir += tmpr; ii += tmpi;
		  }
		  const float nwr = wr*ca-wi*sa;
		  wi = wi*ca + wr*sa;
		  wr = nwr;	      
		}
	  }	  
	}
	if (inverse) (*this)/=N;
	  } break;

	  default: throw CImgArgumentException("CImg<%s>::FFT() : unknown axe '%c', must be 'x','y' or 'z'");
	  }
	  return *this; 
	}

	//! Compute the Fast Fourier Transform of a complex image.
	CImgl& FFT(const bool inverse=false) {
	  if (size<2) throw CImgInstanceException("CImg<%s>::FFT() : Instance have less than 2 images",pixel_type());
	  CImg<T> &Ir = data[0];
	  if (Ir.depth>1)  FFT('z',inverse);
	  if (Ir.height>1) FFT('y',inverse);
	  if (Ir.width>1)  FFT('x',inverse);
	  return *this;
	}
	
	//! Return the Fast Fourier Transform of a complex image
	CImgl<typename cimg::largest<T,float>::type> get_FFT(const bool inverse=false) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImgl<restype>(*this).FFT(inverse); 
	}

	//! Return the Fast Fourier Transform of a complex image (along a specified axis).
	CImgl<typename cimg::largest<T,float>::type> get_FFT(const char axe,const bool inverse=false) const {
	  typedef typename cimg::largest<T,float>::type restype;
	  return CImgl<restype>(*this).FFT(axe,inverse); 
	}
		
	//@}
	//----------------------------------
	//
	//! \name IO and display functions
	//@{
	//----------------------------------
	
	//! Print informations about the list on the standard error stream.
	const CImgl& print(const char* title=0,const int print_flag=1) const { 
	  char tmp[1024];
	  std::fprintf(stderr,"%-8s(this=%p) : { size=%u, data=%p (%s) }\n",title?title:"CImgl",
		   (void*)this,size,(void*)data,is_shared?"shared":"not shared");
	  if (print_flag>0)	cimgl_map(*this,l) {
	std::sprintf(tmp,"%s[%d]",title?title:"CImgl",l);
	data[l].print(tmp,print_flag);
	  }
	  return *this;
	}

	//! Load an image list from a file.
	static CImgl get_load(const char *filename) {
	  CImgl res;
	  const char *ext = cimg::filename_split(filename);
	  if (!cimg::strcasecmp(ext,"cimg") || !ext[0]) return get_load_cimg(filename);
	  if (!cimg::strcasecmp(ext,"rec") || !cimg::strcasecmp(ext,"par")) return get_load_parrec(filename);
	  return CImg<T>(filename);
	}

	//! In-place version of load().
	CImgl& load(const char* filename) {
	  return get_load(filename).swap(*this); 
	}

#define cimg_load_cimg_case(Ts,Tss) \
  if (!loaded && !cimg::strcasecmp(Ts,tmp2)) for (unsigned int l=0; l<n; l++) { \
	  const bool endian = cimg::endian(); \
	  j=0; while((i=std::fgetc(file))!='\n') tmp[j++]=(char)i; tmp[j]='\0'; \
	  std::sscanf(tmp,"%u %u %u %u",&w,&h,&z,&k);\
	  if (w*h*z*k>0) { \
		Tss *buf = new Tss[w*h*z*k]; cimg::fread(buf,w*h*z*k,file); \
		if (endian) cimg::endian_swap(buf,w*h*z*k); \
		CImg<T> idest(w,h,z,k); cimg_mapoff(idest,off) \
						  idest[off] = (T)(buf[off]); idest.swap(res[l]); \
		delete[] buf; \
	   } \
	  loaded = true; \
	}

	//! Load an image list from a file (.raw format).
	static CImgl get_load_cimg(const char *filename) {
	  typedef unsigned char uchar;
	  typedef unsigned short ushort;
	  typedef unsigned int uint;  
	  typedef unsigned long ulong; 
	  std::FILE *file = cimg::fopen(filename,"rb");
	  char tmp[256],tmp2[256];
	  int i;
	  bool loaded = false;
	  unsigned int n,j,w,h,z,k,err;
	  j=0; while((i=std::fgetc(file))!='\n' && i!=EOF && j<256) tmp[j++]=i; tmp[j]='\0';
	  err=std::sscanf(tmp,"%u%*c%255[A-Za-z ]",&n,tmp2);           
	  if (err!=2) throw CImgIOException("CImgl<%s>::get_load_cimg() : file '%s', Unknow CImg RAW header",pixel_type(),filename);
	  CImgl<T> res(n);
	  cimg_load_cimg_case("unsigned char",uchar);
	  cimg_load_cimg_case("uchar",uchar);
	  cimg_load_cimg_case("char",char);
	  cimg_load_cimg_case("unsigned short",ushort);
	  cimg_load_cimg_case("ushort",ushort);
	  cimg_load_cimg_case("short",short);
	  cimg_load_cimg_case("unsigned int",uint);
	  cimg_load_cimg_case("uint",uint);
	  cimg_load_cimg_case("int",int);
	  cimg_load_cimg_case("unsigned long",ulong);
	  cimg_load_cimg_case("ulong",ulong);
	  cimg_load_cimg_case("long",long);
	  cimg_load_cimg_case("float",float);
	  cimg_load_cimg_case("double",double);
	  if (!loaded) throw CImgIOException("CImgl<%s>::get_load_cimg() : file '%s', can't read images of %s",
					 pixel_type(),filename,tmp2);
	  cimg::fclose(file);
	  return res;
	}

	//! In-place version of get_load_cimg().
	CImgl& load_cimg(const char *filename) {
	  return get_load_cimg(filename).swap(*this); 
	}

	//! Load PAR-REC (Philips) image file
	static CImgl get_load_parrec(const char *filename) {
	  char body[1024], filenamepar[1024], filenamerec[1024];
	  const char *ext = cimg::filename_split(filename,body);
	  if (!cimg::strncmp(ext,"par",3)) { std::strcpy(filenamepar,filename); std::sprintf(filenamerec,"%s.rec",body); }
	  if (!cimg::strncmp(ext,"PAR",3)) { std::strcpy(filenamepar,filename); std::sprintf(filenamerec,"%s.REC",body); }
	  if (!cimg::strncmp(ext,"rec",3)) { std::strcpy(filenamerec,filename); std::sprintf(filenamepar,"%s.par",body); }
	  if (!cimg::strncmp(ext,"REC",3)) { std::strcpy(filenamerec,filename); std::sprintf(filenamepar,"%s.PAR",body); }
	  std::FILE *file = cimg::fopen(filenamepar,"r");

	  // Parse header file
	  CImgl<float> st_slices;
	  CImgl<unsigned int> st_global;
	  int err;
	  char line[256]={0};
	  do { err=std::fscanf(file,"%255[^\n]%*c",line); } while (err!=EOF && (line[0]=='#' || line[0]=='.'));
	  do { 
	unsigned int sn,sizex,sizey,pixsize;
	float rs,ri,ss;
	err=std::fscanf(file,"%u%*u%*u%*u%*u%*u%*u%u%*u%u%u%g%g%g%*[^\n]",&sn,&pixsize,&sizex,&sizey,&ri,&rs,&ss);
	if (err==7) {
	  st_slices.insert(CImg<float>::vector((float)sn,(float)pixsize,(float)sizex,(float)sizey,ri,rs,ss,0));	  
	  unsigned int i; for (i=0; i<st_global.size && sn<=st_global[i][2]; i++);
	  if (i==st_global.size) st_global.insert(CImg<unsigned int>::vector(sizex,sizey,sn));
	  else {
		CImg<unsigned int> &vec = st_global[i];
		if (sizex>vec[0]) vec[0] = sizex;
		if (sizey>vec[1]) vec[1] = sizey;
		vec[2] = sn;
	  }
	  st_slices[st_slices.size-1][7] = (float)i;
	}
	  } while (err==7);
	  
	  // Read data
	  std::FILE *file2 = cimg::fopen(filenamerec,"rb");
	  CImgl<T> dest;
	  {	cimgl_map(st_global,l) {
	const CImg<unsigned int>& vec = st_global[l];
	dest.insert(CImg<T>(vec[0],vec[1],vec[2]));
	  }}
	  
	  cimgl_map(st_slices,l) {
	const CImg<float>& vec = st_slices[l];
	const unsigned int
	  sn = (unsigned int)vec[0]-1,
	  pixsize = (unsigned int)vec[1],
	  sizex = (unsigned int)vec[2],
	  sizey = (unsigned int)vec[3],
	  imn = (unsigned int)vec[7];
	const float ri = vec[4], rs = vec[5], ss = vec[6];
	switch (pixsize) {
	case 8: {
	  CImg<unsigned char> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	case 16: {
	  CImg<unsigned short> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	case 32: {
	  CImg<unsigned int> buf(sizex,sizey);
	  cimg::fread(buf.data,sizex*sizey,file2);
	  if (cimg::endian()) cimg::endian_swap(buf.data,sizex*sizey);
	  CImg<T>& img = dest[imn];
	  cimg_mapXY(img,x,y) img(x,y,sn) = (T)(( buf(x,y)*rs + ri )/(rs*ss));
	} break;
	default:
	  throw CImgIOException("CImg<%s>::get_load_parrec() : Cannot handle image with pixsize = %d bits\n",
				pixel_type(),pixsize);
	  
	}
	  }
	  
	  cimg::fclose(file);
	  cimg::fclose(file2);  
	  if (!dest.size)
	throw CImgIOException("CImg<%s>::get_load_parrec() : filename '%s' does not appear to be a valid PAR-REC file",
				  pixel_type(),filename);
	  return dest;
	}

	//! In-place version of get_load_parrec().
	CImgl& load_parrec(const char *filename) {
	  return get_load_parrec(filename).swap(*this); 
	}

	//! Load YUV image sequence.
	static CImgl get_load_yuv(const char *filename,
			  const unsigned int sizex, const unsigned int sizey,
			  const unsigned int first_frame=0, const int last_frame=-1,
			  const bool yuv2rgb=true) {
	  if (sizex%2 || sizey%2)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Image dimensions along X and Y must be even numbers (given are %ux%u)\n",
					pixel_type(),sizex,sizey);
	  if (!sizex || !sizey)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given image sequence size (%u,%u) is invalid",
					pixel_type(),sizex,sizey);      
	  if (last_frame>0 && first_frame>(unsigned int)last_frame)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given first frame %u is posterior to last frame %d.",
				pixel_type(),first_frame,last_frame);
	  if (!sizex || !sizey)
	throw CImgArgumentException("CImgl<%s>::get_load_yuv() : Given frame size (%u,%u) is invalid.",
					pixel_type(),sizex,sizey);
	  CImgl<T> res;
	  CImg<unsigned char> tmp(sizex,sizey,1,3), UV(sizex/2,sizey/2,1,2);
	  std::FILE *file = cimg::fopen(filename,"rb");
	  bool stopflag = false;
	  int err;
	  if (first_frame) {
	err = std::fseek(file,first_frame*(sizex*sizey + sizex*sizey/2),SEEK_CUR);
	if (err) throw CImgIOException("CImgl<%s>::get_load_yuv() : File '%s' doesn't contain frame number %u "
					   "(out of range error).",pixel_type(),filename,first_frame);
	  }
	  unsigned int frame;
	  for (frame = first_frame; !stopflag && (last_frame<0 || frame<=(unsigned int)last_frame); frame++) {
	tmp.fill(0);
	// TRY to read the luminance, don't replace by cimg::fread !
	err = (int)std::fread((void*)(tmp.ptr()),1,(size_t)(tmp.width*tmp.height),file); 
	if (err!=(int)(tmp.width*tmp.height)) {
	  stopflag = true;
	  cimg::warn(err>0,"CImgl<%s>::get_load_yuv() : File '%s' contains incomplete data,"
			 " or given image dimensions (%u,%u) are incorrect.",pixel_type(),filename,sizex,sizey);
	} else {
	  UV.fill(0);
	  // TRY to read the luminance, don't replace by cimg::fread !
	  err = (int)std::fread((void*)(UV.ptr()),1,(size_t)(UV.size()),file);
	  if (err!=(int)(UV.size())) {
		stopflag = true;
		cimg::warn(err>0,"CImgl<%s>::get_load_yuv() : File '%s' contains incomplete data,"
			   " or given image dimensions (%u,%u) are incorrect.",pixel_type(),filename,sizex,sizey);
	  } else {
		cimg_mapXY(UV,x,y) {
		  const int x2=2*x, y2=2*y;
		  tmp(x2,y2,1) = tmp(x2+1,y2,1) = tmp(x2,y2+1,1) = tmp(x2+1,y2+1,1) = UV(x,y,0);
		  tmp(x2,y2,2) = tmp(x2+1,y2,2) = tmp(x2,y2+1,2) = tmp(x2+1,y2+1,2) = UV(x,y,1);
		}
		if (yuv2rgb) tmp.YCbCrtoRGB();
		res.insert(tmp);
	  }
	}
	  }
	  cimg::warn(stopflag && last_frame>=0 && frame!=(unsigned int)last_frame,
		 "CImgl<%s>::get_load_yuv() : Frame %d not reached, only %u frames found in the file '%s'.",
		 pixel_type(),last_frame,frame-1,filename);
	  return res;
	}

	//! In-place version of get_load_yuv().
	CImgl& load_yuv(const char *filename,
			const unsigned int sizex, const unsigned int sizey,
			const unsigned int first_frame=0, const int last_frame=-1,
			const bool yuv2rgb=true) {
	  return get_load_yuv(filename,sizex,sizey,first_frame,last_frame,yuv2rgb).swap(*this);
	}

	//! Save an image sequence into a YUV file   
	const CImgl& save_yuv(const char *filename, const bool rgb2yuv=true) const {
	  if (is_empty())
	throw CImgInstanceException("CImgl<%s>::save_yuv() : Instance list (%u,%p) is empty.",
						  pixel_type(),size,data);
	  if (!filename) 
	throw CImgArgumentException("CImgl<%s>::save_yuv() : Specified filename is (null).",
					pixel_type());
	  if ((*this)[0].dimx()%2 || (*this)[0].dimy()%2)
	throw CImgInstanceException("CImgl<%s>::save_yuv() : Image dimensions along X and Y must be even (current are %ux%u)\n",
					pixel_type(),(*this)[0].dimx(),(*this)[0].dimy());

	  std::FILE *file = cimg::fopen(filename,"wb");
	  cimgl_map(*this,l) {	
	CImg<unsigned char> YCbCr((*this)[l]);
	if (rgb2yuv) YCbCr.RGBtoYCbCr();
	cimg::fwrite(YCbCr.ptr(),YCbCr.width*YCbCr.height,file);
	cimg::fwrite(YCbCr.get_resize(YCbCr.width/2, YCbCr.height/2,1,3,3).ptr(0,0,0,1),
			 YCbCr.width*YCbCr.height/2,file);
	  }
	  cimg::fclose(file);
	  return *this;
	}
	
	//! Save an image list into a file.
	/**
	   Depending on the extension of the given filename, a file format is chosen for the output file.       
	**/    
	const CImgl& save(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImgl<%s>::save() : Instance list (%u,%p) is empty.",
						  pixel_type(),size,data);
	  if (!filename) throw CImgArgumentException("CImgl<%s>::save() : Specified filename is (null).",pixel_type());
	  const char *ext = cimg::filename_split(filename);
	  if (!cimg::strcasecmp(ext,"cimg") || !ext[0]) return save_cimg(filename);
	  if (!cimg::strcasecmp(ext,"yuv")) return save_yuv(filename,true);      
	  if (size==1) data[0].save(filename,-1);
	  else cimgl_map(*this,l) data[l].save(filename,l);      
	  return *this;
	}
	
	//! Save an image list into a CImg RAW file.
	/**
	   A CImg RAW file is a simple uncompressed binary file that may be used to save list of CImg<T> images.
	   \param filename : name of the output file.
	   \return A reference to the current CImgl instance is returned.
	**/
	const CImgl& save_cimg(const char *filename) const {
	  if (is_empty()) throw CImgInstanceException("CImgl<%s>::save_cimg() : Instance list (%u,%p) is empty.",
						  pixel_type(),size,data);
	  if (!filename) throw CImgArgumentException("CImgl<%s>::save_cimg() : Specified filename is (null).",
						 pixel_type());
	  std::FILE *file = cimg::fopen(filename,"wb");
	  std::fprintf(file,"%u %s\n",size,pixel_type());
	  cimgl_map(*this,l) {
	const CImg<T>& img = data[l];
	std::fprintf(file,"%u %u %u %u\n",img.width,img.height,img.depth,img.dim);
	if (img.data) {
	  if (cimg::endian()) {
		CImg<T> tmp(img);
		cimg::endian_swap(tmp.data,tmp.size());
		cimg::fwrite(tmp.data,img.width*img.height*img.depth*img.dim,file);
	  } else cimg::fwrite(img.data,img.width*img.height*img.depth*img.dim,file);
	}
	  }
	  cimg::fclose(file);
	  return *this;
	}  


	//! Load from OFF file format
	template<typename tf,typename tc>
	static CImgl<T> get_load_off(const char *filename, CImgl<tf>& primitives, CImgl<tc>& colors, const bool invert_faces=false) {
	  return CImg<T>::get_load_off(filename,primitives,colors,invert_faces).get_split('x');
	}
	
	//! In-place version of get_load_off()
	template<typename tf,typename tc>
	CImgl& load_off(const char *filename, CImgl<tf>& primitives, CImgl<tc>& colors, const bool invert_faces=false) {
	  return get_load_off(filename,primitives,colors,invert_faces).swap(*this);
	}

	//! Save an image list into a OFF file.
	template<typename tf, typename tc>
	const CImgl& save_off(const char *filename, const CImgl<tf>& primitives, const CImgl<tc>& colors, const bool invert_faces=false) const {
	  get_append('x').save_off(filename,primitives,colors,invert_faces);
	  return *this;
	}

	//! Return a single image which is the concatenation of all images of the current CImgl instance.
	/**
	   \param axe : specify the axe for image concatenation. Can be 'x','y','z' or 'v'.
	   \param align : specify the alignment for image concatenation. Can be 'p' (top), 'c' (center) or 'n' (bottom).
	   \return A CImg<T> image corresponding to the concatenation is returned.
	**/
	CImg<T> get_append(const char axe='x',const char align='c') const {
	  if (is_empty()) return CImg<T>();
	  unsigned int dx=0,dy=0,dz=0,dv=0,pos=0;
	  CImg<T> res;
	  switch(cimg::uncase(axe)) {
	  case 'x': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx += img.width;
	  dy = cimg::max(dy,img.height);
	  dz = cimg::max(dz,img.depth);
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p' : { cimgl_map(*this,ll) { res.draw_image((*this)[ll],pos,0,0,0); pos+=(*this)[ll].width; }} break;
	case 'n' : { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],pos,dy-(*this)[ll].height,dz-(*this)[ll].depth,dv-(*this)[ll].dim); pos+=(*this)[ll].width;
		}} break;
	default  : { cimgl_map(*this,ll) {
		  res.draw_image((*this)[ll],pos,(dy-(*this)[ll].height)/2,(dz-(*this)[ll].depth)/2,(dv-(*this)[ll].dim)/2);
		  pos+=(*this)[ll].width; 
		}} break;
	}
	  }	break;
	  case 'y': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy += img.height;
	  dz = cimg::max(dz,img.depth);
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,pos,0,0); pos+=(*this)[ll].height; }} break;
	case 'n': { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],dx-(*this)[ll].width,pos,dz-(*this)[ll].depth,dv-(*this)[ll].dim); pos+=(*this)[ll].height;
		}} break;
	default : { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,pos,(dz-(*this)[ll].depth)/2,(dv-(*this)[ll].dim)/2);
		  pos+=(*this)[ll].height; 
		}} break;
	}
	  }	break;
	  case 'z': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy = cimg::max(dy,img.height);
	  dz += img.depth;
	  dv = cimg::max(dv,img.dim);
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,0,pos,0); pos+=(*this)[ll].depth; }} break;
	case 'n': { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],dx-(*this)[ll].width,dy-(*this)[ll].height,pos,dv-(*this)[ll].dim); pos+=(*this)[ll].depth;
		}} break;
	case 'c': { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,(dy-(*this)[ll].height)/2,pos,(dv-(*this)[ll].dim)/2);
		  pos+=(*this)[ll].depth; 
		}} break;
	}
	  }	break;
	  case 'v': {
	cimgl_map(*this,l) {
	  const CImg<T>& img = (*this)[l];
	  dx = cimg::max(dx,img.width);
	  dy = cimg::max(dy,img.height);
	  dz = cimg::max(dz,img.depth);
	  dv += img.dim;
	}
	res.assign(dx,dy,dz,dv,0);
	switch (cimg::uncase(align)) {
	case 'p': { cimgl_map(*this,ll) { res.draw_image((*this)[ll],0,0,0,pos); pos+=(*this)[ll].dim; }} break;
	case 'n': { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],dx-(*this)[ll].width,dy-(*this)[ll].height,dz-(*this)[ll].depth,pos); pos+=(*this)[ll].dim;
		}} break;
	case 'c': { cimgl_map(*this,ll) { 
		  res.draw_image((*this)[ll],(dx-(*this)[ll].width)/2,(dy-(*this)[ll].height)/2,(dz-(*this)[ll].depth)/2,pos);
		  pos+=(*this)[ll].dim; 
		}} break;
	}
	  } break;
	  default: throw CImgArgumentException("CImg<%s>::get_append() : unknow axe '%c', must be 'x','y','z' or 'v'",pixel_type(),axe);
	  }
	  return res;
	}

	// Create an auto-cropped font (along the X axis) from a input font \p font.
	CImgl<T> get_crop_font() const {
	  CImgl<T> res;
	  cimgl_map(*this,l) {
	const CImg<T>& letter = (*this)[l];
		int xmin = letter.width, xmax = 0;
		cimg_mapXY(letter,x,y) if (letter(x,y)) { if (x<xmin) xmin=x; if (x>xmax) xmax=x; }
		if (xmin>xmax) res.insert(CImg<T>(letter.width,letter.height,1,letter.dim,0));
	else res.insert(letter.get_crop(xmin,0,xmax,letter.height-1));
	  }
	  res[' '].resize(res['f'].width);
	  res[' '+256].resize(res['f'].width);
	  return res;
	}
	
	CImgl<T>& crop_font() {
	  return get_crop_font().swap(*this);
	}
	
	static CImgl<T> get_font(const unsigned int *const font,const unsigned int w,const unsigned int h,
				 const unsigned int paddingx, const unsigned int paddingy, const bool variable_size=true) {
	  CImgl<T> res = CImgl<T>(256,w,h,1,3).insert(CImgl<T>(256,w,h,1,1));
	  const unsigned int *ptr = font;
	  unsigned int m = 0, val = 0;
	  for (unsigned int y=0; y<h; y++)
	for (unsigned int x=0; x<256*w; x++) {
	  m>>=1; if (!m) { m=0x80000000; val = *(ptr++); }
	  CImg<T>& img = res[x/w], &mask = res[x/w+256];
	  unsigned int xm = x%w;
	  img(xm,y,0) = img(xm,y,1) = img(xm,y,2) = mask(xm,y,0) = (T)((val&m)?1:0);
	}
	  if (variable_size) res.crop_font();
	  if (paddingx || paddingy) cimgl_map(res,l) res[l].resize(res[l].dimx()+paddingx, res[l].dimy()+paddingy,1,-100,0);
	  return res;
	}

	//! Return a CImg pre-defined font with desired size
	/**
	   \param font_height = height of the desired font (can be 11,13,24,38 or 57)
	   \param fixed_size = tell if the font has a fixed or variable width.
	**/
	static CImgl<T> get_font(const unsigned int font_width, const bool variable_size=true) {
	  if (font_width<=11) {
	static CImgl<T> font7x11, nfont7x11;
	if (!variable_size && font7x11.is_empty()) font7x11 = get_font(cimg::font7x11,7,11,1,0,false);
	if (variable_size && nfont7x11.is_empty()) nfont7x11 = get_font(cimg::font7x11,7,11,1,0,true);
	return variable_size?nfont7x11:font7x11;
	  }
	  if (font_width<=13) {
	static CImgl<T> font10x13, nfont10x13;
	if (!variable_size && font10x13.is_empty()) font10x13 = get_font(cimg::font10x13,10,13,1,0,false);
	if (variable_size && nfont10x13.is_empty()) nfont10x13 = get_font(cimg::font10x13,10,13,1,0,true);
	return variable_size?nfont10x13:font10x13;
	  } 
	  if (font_width<=17) {
	static CImgl<T> font8x17, nfont8x17;
	if (!variable_size && font8x17.is_empty()) font8x17 = get_font(cimg::font8x17,8,17,1,0,false);
	if (variable_size && nfont8x17.is_empty()) nfont8x17 = get_font(cimg::font8x17,8,17,1,0,true);
	return variable_size?nfont8x17:font8x17;
	  }
	  if (font_width<=19) {
	static CImgl<T> font10x19, nfont10x19;
	if (!variable_size && font10x19.is_empty()) font10x19 = get_font(cimg::font10x19,10,19,2,0,false);
	if (variable_size && nfont10x19.is_empty()) nfont10x19 = get_font(cimg::font10x19,10,19,2,0,true);
	return variable_size?nfont10x19:font10x19;
	  } 
	  if (font_width<=24) {
	static CImgl<T> font12x24, nfont12x24;
	if (!variable_size && font12x24.is_empty()) font12x24 = get_font(cimg::font12x24,12,24,2,0,false);
	if (variable_size && nfont12x24.is_empty()) nfont12x24 = get_font(cimg::font12x24,12,24,2,0,true);
	return variable_size?nfont12x24:font12x24;
	  } 
	  if (font_width<=32) {
	static CImgl<T> font16x32, nfont16x32;
	if (!variable_size && font16x32.is_empty()) font16x32 = get_font(cimg::font16x32,16,32,2,0,false);
	if (variable_size && nfont16x32.is_empty()) nfont16x32 = get_font(cimg::font16x32,16,32,2,0,true);
	return variable_size?nfont16x32:font16x32;
	  } 
	  if (font_width<=38) {
	static CImgl<T> font19x38, nfont19x38;
	if (!variable_size && font19x38.is_empty()) font19x38 = get_font(cimg::font19x38,19,38,3,0,false);
	if (variable_size && nfont19x38.is_empty()) nfont19x38 = get_font(cimg::font19x38,19,38,3,0,true);
	return variable_size?nfont19x38:font19x38;
	  }
	  static CImgl<T> font29x57, nfont29x57;
	  if (!variable_size && font29x57.is_empty()) font29x57 = get_font(cimg::font29x57,29,57,5,0,false);
	  if (variable_size && nfont29x57.is_empty()) nfont29x57 = get_font(cimg::font29x57,29,57,5,0,true);
	  return variable_size?nfont29x57:font29x57;
	}
	
	//! Display the current CImgl instance in an existing CImgDisplay window (by reference).
	/**
	   This function displays the list images of the current CImgl instance into an existing CImgDisplay window.
	   Images of the list are concatenated in a single temporarly image for visualization purposes.
	   The function returns immediately.
	   \param disp : reference to an existing CImgDisplay instance, where the current image list will be displayed.
	   \param axe : specify the axe for image concatenation. Can be 'x','y','z' or 'v'.
	   \param align : specify the alignment for image concatenation. Can be 'p' (top), 'c' (center) or 'n' (bottom).
	   \return A reference to the current CImgl instance is returned.
	**/
	const CImgl& display(CImgDisplay& disp,const char axe='x',const char align='c') const { 
	  get_append(axe,align).display(disp);
	  return *this; 
	}

	//! Display the current CImgl instance in a new display window.
	/**
	   This function opens a new window with a specific title and displays the list images of the current CImgl instance into it.
	   Images of the list are concatenated in a single temporarly image for visualization purposes.
	   The function returns when a key is pressed or the display window is closed by the user.
	   \param title : specify the title of the opening display window.
	   \param axe : specify the axe for image concatenation. Can be 'x','y','z' or 'v'.
	   \param align : specify the alignment for image concatenation. Can be 'p' (top), 'c' (center) or 'n' (bottom).
	   \param min_size : specify the minimum size of the opening display window. Images having dimensions below this
	   size will be upscaled.
	   \param max_size : specify the maximum size of the opening display window. Images having dimensions above this
	   size will be downscaled.
	   \return A reference to the current CImgl instance is returned.
	**/
	const CImgl& display(const char* title,const char axe='x',const char align='c',
			 const int min_size=128,const int max_size=1024) const {
	  get_append(axe,align).display(title,min_size,max_size);
	  return *this;
	}

	//! Display the current CImgl instance in a new display window.
	/**
	   This function opens a new window and displays the list images of the current CImgl instance into it.
	   Images of the list are concatenated in a single temporarly image for visualization purposes.
	   The function returns when a key is pressed or the display window is closed by the user.
	   \param axe : specify the axe for image concatenation. Can be 'x','y','z' or 'v'.
	   \param align : specify the alignment for image concatenation. Can be 'p' (top), 'c' (center) or 'n' (bottom).
	   \param min_size : specify the minimum size of the opening display window. Images having dimensions below this
	   size will be upscaled.
	   \param max_size : specify the maximum size of the opening display window. Images having dimensions above this
	   size will be downscaled.
	   \return A reference to the current CImgl instance is returned.
	**/
	const CImgl& display(const char axe='x',const char align='c',
			 const int min_size=128,const int max_size=1024) const {
	  return display(" ",axe,align,min_size,max_size); 
	}

	// Swap fields of two CImgl instances.
	CImgl& swap(CImgl& list) {
	  if (list.is_shared==is_shared) {
	cimg::swap(size,list.size);
	cimg::swap(allocsize,list.allocsize);
	cimg::swap(data,list.data);
	  } else {
	if (list.is_shared) list=*this;
	if (is_shared) *this=list;
	  }
	  return list;
	}

	//! Return a reference to a set of images (I0->I1) of the list.
	const CImgl get_shared_images(const unsigned int i0, const unsigned int i1) const {
	  if (i0>i1 || i0>=size || i1>=size)
	throw CImgArgumentException("CImgl<%s>::get_shared_images() : Cannot get a subset (%u->%u) from a list of size %u",
					pixel_type(),i0,i1,size);
	  return CImgl<T>(data+i0,i1-i0+1,true);
	}

	CImgl get_shared_images(const unsigned int i0, const unsigned int i1) {
	  if (i0>i1 || i0>=size || i1>=size)
	throw CImgArgumentException("CImgl<%s>::get_shared_images() : Cannot get a subset (%u->%u) from a list of size %u",
					pixel_type(),i0,i1,size);
	  return CImgl<T>(data+i0,i1-i0+1,true);
	}
	
	//! Return a sublist
	CImgl get_images(const unsigned int i0, const unsigned int i1) const {
	  const CImgl<T> sh = get_shared_images(i0,i1);
	  return CImgl<T>(data,sh.size);
	}

	//@}
	//---------------------------
	//
	//! \name Plugins functions
	//@{
	//---------------------------
#ifdef cimgl_plugin
#include cimgl_plugin
#endif
	//@}
  };

  /*
   #-----------------------------------------
   #
   #
   #
   # Complete some already defined functions
   #
   #
   #
   #------------------------------------------
  */

namespace cimg {
  
  //! Display a dialog box, where a user can click standard buttons.
  /**
	 Up to 6 buttons can be defined in the dialog window.
	 This function returns when a user clicked one of the button or closed the dialog window.
	 \param title = Title of the dialog window.
	 \param msg = Main message displayed inside the dialog window.
	 \param button1_txt = Label of the 1st button.
	 \param button2_txt = Label of the 2nd button.
	 \param button3_txt = Label of the 3rd button.
	 \param button4_txt = Label of the 4th button.
	 \param button5_txt = Label of the 5th button.
	 \param button6_txt = Label of the 6th button.
	 \param logo = Logo image displayed at the left of the main message. This parameter is optional.
	 \param centering = Tell to center the dialog window on the screen.
	 \return The button number (from 0 to 5), or -1 if the dialog window has been closed by the user.
	 \note If a button text is set to 0, then the corresponding button (and the followings) won't appear in
	 the dialog box. At least one button is necessary.
  **/

  template<typename t>
  inline int dialog(const char *title,const char *msg,
			const char *button1_txt,const char *button2_txt,
			const char *button3_txt,const char *button4_txt,
			const char *button5_txt,const char *button6_txt,
			const CImg<t>& logo, const bool centering = false) {
#if cimg_display_type!=0
	const unsigned char
	  black[3]={0,0,0}, white[3]={255,255,255}, gray[3]={200,200,200}, gray2[3]={150,150,150};
	  
	  // Create buttons and canvas graphics
	  CImgl<unsigned char> buttons, cbuttons, sbuttons;
	  if (button1_txt) { buttons.insert(CImg<unsigned char>().draw_text(button1_txt,0,0,black,gray,13));
	  if (button2_txt) { buttons.insert(CImg<unsigned char>().draw_text(button2_txt,0,0,black,gray,13));
	  if (button3_txt) { buttons.insert(CImg<unsigned char>().draw_text(button3_txt,0,0,black,gray,13));
	  if (button4_txt) { buttons.insert(CImg<unsigned char>().draw_text(button4_txt,0,0,black,gray,13));
	  if (button5_txt) { buttons.insert(CImg<unsigned char>().draw_text(button5_txt,0,0,black,gray,13));
	  if (button6_txt) { buttons.insert(CImg<unsigned char>().draw_text(button6_txt,0,0,black,gray,13));
	  }}}}}}
	  if (!buttons.size) throw CImgArgumentException("cimg::dialog() : No buttons have been defined. At least one is necessary");
	  
	  unsigned int bw=0, bh=0;
	  cimgl_map(buttons,l) { bw = cimg::max(bw,buttons[l].width); bh = cimg::max(bh,buttons[l].height); }
	  bw+=8; bh+=8;
	  if (bw<64) bw=64;
	  if (bw>128) bw=128;
	  if (bh<24) bh=24;
	  if (bh>48) bh=48;
	  
	  CImg<unsigned char> button = CImg<unsigned char>(bw,bh,1,3).
	draw_rectangle(0,0,bw-1,bh-1,gray).
	draw_line(0,0,bw-1,0,white).draw_line(0,bh-1,0,0,white).
	draw_line(bw-1,0,bw-1,bh-1,black).draw_line(bw-1,bh-1,0,bh-1,black).
	draw_line(1,bh-2,bw-2,bh-2,gray2).draw_line(bw-2,bh-2,bw-2,1,gray2);
	  CImg<unsigned char> sbutton = CImg<unsigned char>(bw,bh,1,3).
	draw_rectangle(0,0,bw-1,bh-1,gray).
	draw_line(0,0,bw-1,0,black).draw_line(bw-1,0,bw-1,bh-1,black).
	draw_line(bw-1,bh-1,0,bh-1,black).draw_line(0,bh-1,0,0,black).
	draw_line(1,1,bw-2,1,white).draw_line(1,bh-2,1,1,white).
	draw_line(bw-2,1,bw-2,bh-2,black).draw_line(bw-2,bh-2,1,bh-2,black).
	draw_line(2,bh-3,bw-3,bh-3,gray2).draw_line(bw-3,bh-3,bw-3,2,gray2).
	draw_line(4,4,bw-5,4,black,0xAAAAAAAA).draw_line(bw-5,4,bw-5,bh-5,black,0xAAAAAAAA).
	draw_line(bw-5,bh-5,4,bh-5,black,0xAAAAAAAA).draw_line(4,bh-5,4,4,black,0xAAAAAAAA);
	  CImg<unsigned char> cbutton = CImg<unsigned char>(bw,bh,1,3).
	draw_rectangle(0,0,bw-1,bh-1,black).draw_rectangle(1,1,bw-2,bh-2,gray2).draw_rectangle(2,2,bw-3,bh-3,gray).
	draw_line(4,4,bw-5,4,black,0xAAAAAAAA).draw_line(bw-5,4,bw-5,bh-5,black,0xAAAAAAAA).
	draw_line(bw-5,bh-5,4,bh-5,black,0xAAAAAAAA).draw_line(4,bh-5,4,4,black,0xAAAAAAAA);

	cimgl_map(buttons,ll) {
	  cbuttons.insert(CImg<unsigned char>(cbutton).draw_image(buttons[ll],1+(bw-buttons[ll].dimx())/2,1+(bh-buttons[ll].dimy())/2));
	  sbuttons.insert(CImg<unsigned char>(sbutton).draw_image(buttons[ll],(bw-buttons[ll].dimx())/2,(bh-buttons[ll].dimy())/2));
	  buttons[ll] = CImg<unsigned char>(button).draw_image(buttons[ll],(bw-buttons[ll].dimx())/2,(bh-buttons[ll].dimy())/2);
	}
	
	CImg<unsigned char> canvas;
	if (msg) canvas = CImg<unsigned char>().draw_text(msg,0,0,black,gray,13);
	const unsigned int 
	  bwall = (buttons.size-1)*(12+bw) + bw,
	  w = cimg::max(196U,36+logo.width+canvas.width, 24+bwall),
	  h = cimg::max(96U,36+canvas.height+bh,36+logo.height+bh),
	  lx = 12 + (canvas.data?0:((w-24-logo.width)/2)),
	  ly = (h-12-bh-logo.height)/2,
	  tx = lx+logo.width+12,
	  ty = (h-12-bh-canvas.height)/2,
	  bx = (w-bwall)/2,
	  by = h-12-bh;
	
	if (canvas.data)
	  canvas = CImg<unsigned char>(w,h,1,3).
		draw_rectangle(0,0,w-1,h-1,gray).
		draw_line(0,0,w-1,0,white).draw_line(0,h-1,0,0,white).
		draw_line(w-1,0,w-1,h-1,black).draw_line(w-1,h-1,0,h-1,black).
		draw_image(canvas,tx,ty);
	else 
	  canvas = CImg<unsigned char>(w,h,1,3).
		draw_rectangle(0,0,w-1,h-1,gray).
		draw_line(0,0,w-1,0,white).draw_line(0,h-1,0,0,white).
		draw_line(w-1,0,w-1,h-1,black).draw_line(w-1,h-1,0,h-1,black);
	if (logo.data) canvas.draw_image(logo,lx,ly);
	
	unsigned int xbuttons[6];
	cimgl_map(buttons,lll) { xbuttons[lll] = bx+(bw+12)*lll; canvas.draw_image(buttons[lll],xbuttons[lll],by); }
	
	// Open window and enter events loop  
	CImgDisplay disp(canvas,title?title:" ",0,3,false,centering?true:false);
	if (centering) disp.move((CImgDisplay::screen_dimx()-disp.dimx())/2,
				 (CImgDisplay::screen_dimy()-disp.dimy())/2);
	bool stopflag = false, refresh = false;
	int oselected = -1, oclicked = -1, selected = -1, clicked = -1;
	while (!disp.is_closed && !stopflag) {
	  if (refresh) {
		if (clicked>=0) CImg<unsigned char>(canvas).draw_image(cbuttons[clicked],xbuttons[clicked],by).display(disp);
		else {
		  if (selected>=0) CImg<unsigned char>(canvas).draw_image(sbuttons[selected],xbuttons[selected],by).display(disp);
		  else canvas.display(disp);
		}
		refresh = false;
	  }
	  disp.wait(15);
	  if (disp.is_resized) disp.resize(disp);
	  
	  if (disp.button&1)  {
		oclicked = clicked;
		clicked = -1;
		cimgl_map(buttons,l)
		  if (disp.mouse_y>=(int)by && disp.mouse_y<(int)(by+bh) &&
		  disp.mouse_x>=(int)xbuttons[l] && disp.mouse_x<(int)(xbuttons[l]+bw)) {
		clicked = selected = l;
		refresh = true;
		  }
		if (clicked!=oclicked) refresh = true;
	  } else if (clicked>=0) stopflag = true;
	  
	  if (disp.key) {
		oselected = selected;
		switch (disp.key) {
		case cimg::keyESC: selected=-1; stopflag=true; break;
		case cimg::keyENTER: if (selected<0) selected=0; stopflag = true; break;
		case cimg::keyTAB:
		case cimg::keyARROWRIGHT:
		case cimg::keyARROWDOWN: selected = (selected+1)%buttons.size; break;
		case cimg::keyARROWLEFT:
		case cimg::keyARROWUP: selected = (selected+buttons.size-1)%buttons.size; break;
		}
		disp.key=0;
		if (selected!=oselected) refresh = true;
	  }
	}
	if (disp.is_closed) selected = -1;
	return selected;
#else
	std::fprintf(stderr,"<%s>\n\n%s\n\n",title,msg);	
	return -1+0*(int)(button1_txt-button2_txt+button3_txt-button4_txt+button5_txt-button6_txt+logo.width+(int)centering);
#endif
  }

  inline int dialog(const char *title,const char *msg,
			const char *button1_txt,const char *button2_txt,const char *button3_txt,
			const char *button4_txt,const char *button5_txt,const char *button6_txt,
			const bool centering) {
	return dialog(title,msg,button1_txt,button2_txt,button3_txt,button4_txt,button5_txt,button6_txt,
		  CImg<unsigned char>::get_logo40x38(),centering);
  }


  // Inner routine used by the Marching cube algorithm
  template<typename t> inline int _marching_cubes_indice(const unsigned int edge, const CImg<t>& indices1, const CImg<t>& indices2,
							 const unsigned int x, const unsigned int y, const unsigned int nx, const unsigned int ny) {
	switch (edge) {
	case 0: return indices1(x,y,0);
	case 1: return indices1(nx,y,1);
	case 2: return indices1(x,ny,0);
	case 3: return indices1(x,y,1);
	case 4: return indices2(x,y,0);
	case 5: return indices2(nx,y,1);
	case 6: return indices2(x,ny,0);
	case 7: return indices2(x,y,1);
	case 8: return indices1(x,y,2);
	case 9: return indices1(nx,y,2);
	case 10: return indices1(nx,ny,2);
	case 11: return indices1(x,ny,2);
	}
	return 0;
  }
	
  //! Polygonize an implicit function
  // This function uses the Marching Cubes Tables published on the web page :
  // http://astronomy.swin.edu.au/~pbourke/modelling/polygonise/
  template<typename tfunc, typename tp, typename tf>
  inline void marching_cubes(const tfunc& func, const float isovalue,
				 const float x0,const float y0,const float z0,
				 const float x1,const float y1,const float z1,
				 const float resx,const float resy,const float resz,
				 CImgl<tp>& points, CImgl<tf>& primitives,
				 const bool invert_faces) {

	static unsigned int edges[256]={
	  0x000, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	  0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	  0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c, 0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	  0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	  0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	  0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	  0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	  0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc , 0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	  0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	  0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	  0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	  0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	  0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac, 0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	  0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	  0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	  0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x000 };

	static int triangles[256][16] =
	  {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	   {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	   {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	   {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1}, {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	   {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1}, {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	   {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	   {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1}, {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	   {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1}, {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	   {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1}, {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	   {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	   {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1}, {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	   {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	   {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1}, {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	   {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1}, {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	   {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1}, {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	   {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	   {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1}, {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1}, {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	   {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1}, {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	   {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1}, {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	   {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1}, {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	   {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1}, {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	   {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1}, {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	   {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	   {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1}, {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	   {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	   {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1}, {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	   {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	   {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1}, {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	   {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	   {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1}, {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	   {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1}, {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	   {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1}, {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	   {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1}, {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	   {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1}, {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	   {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1}, {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	   {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1}, {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	   {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	   {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1}, {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	   {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1}, {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	   {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	   {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	   {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1}, {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	   {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1}, {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	   {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1}, {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1}, {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	   {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1}, {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1}, {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	   {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1}, {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1}, {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	   {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1}, {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	   {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1}, {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1}, {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	   {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	   {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1}, {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	   {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	   {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1}, {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	   {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1}, {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	   {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1}, {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	   {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	   {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1}, {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	   {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1}, {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	   {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1}, {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	   {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1}, {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1}, {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	   {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1}, {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	   {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1}, {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	   {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1}, {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	   {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1}, {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	   {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1}, {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	   {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1}, {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	   {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1}, {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	   {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1}, {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	   {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1}, {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	   {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1}, {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	   {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1}, {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1}, {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	   {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1}, {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	   {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1}, {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	   {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1}, {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1}, {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	   {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	   {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1}, {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	   {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	   {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1}, {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	   {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1}, {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	   {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1}, {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	   {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	   {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1}, {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1}, {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	   {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1}, {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	   {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1}, {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	   {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1}, {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1}, {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	   {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1}, {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	   {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1}, {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1}, {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1}, {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	   {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1}, {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	   {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1}, {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	   {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1}, {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	   {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1}, {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	   {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1}, {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1}, {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	   {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	   {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1}, {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1}, {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	   {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1}, {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1}, {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	   {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
  
	const unsigned int 
	  nx = (unsigned int)((x1-x0+1)/resx), nxm1 = nx-1,
	  ny = (unsigned int)((y1-y0+1)/resy), nym1 = ny-1,
	  nz = (unsigned int)((z1-z0+1)/resz), nzm1 = nz-1;      

	if (!nxm1 || !nym1 || !nzm1) return;

	CImg<int> indices1(nx,ny,1,3,-1), indices2(indices1);
	CImg<float> values1(nx,ny), values2(nx,ny);
	float X=0, Y=0, Z=0, nX=0, nY=0, nZ=0;

	// Fill the first plane with function values
	Y=y0;
	cimg_mapY(values1,y) {
	  X = x0;
	  cimg_mapX(values1,x) { values1(x,y) = (float)func(X,Y,z0); X+=resx; }
	  Y+=resy;
	}
	
	// Run Marching Cubes algorithm
	Z = z0; nZ = Z + resz;
	for (unsigned int zi=0; zi<nzm1; ++zi, Z=nZ, nZ+=resz) {
	  Y = y0; nY = Y + resy;
	  indices2.fill(-1);
	  for (unsigned int yi=0, nyi=1; yi<nym1; ++yi, ++nyi, Y=nY, nY+=resy) {
	X = x0; nX = X + resx;
	for (unsigned int xi=0, nxi=1; xi<nxm1; ++xi, ++nxi, X=nX, nX+=resx) {

	  // Determine cube configuration
	  const float
		val0 = values1(xi,yi), val1 = values1(nxi,yi), val2 = values1(nxi,nyi), val3 = values1(xi,nyi),
		val4 = values2(xi,yi) = (float)func(X,Y,nZ),
		val5 = values2(nxi,yi) = (float)func(nX,Y,nZ),
		val6 = values2(nxi,nyi) = (float)func(nX,nY,nZ),
		val7 = values2(xi,nyi) = (float)func(X,nY,nZ);

	  const unsigned int configuration = 
		(val0<isovalue?1:0)  | (val1<isovalue?2:0)  | (val2<isovalue?4:0)  | (val3<isovalue?8:0) |
		(val4<isovalue?16:0) | (val5<isovalue?32:0) | (val6<isovalue?64:0) | (val7<isovalue?128:0),
		edge = edges[configuration];
	
	  // Compute intersection points
	  if (edge) {
		if ((edge&1) && indices1(xi,yi,0)<0) {
		  const float Xi = X + (isovalue-val0)*resx/(val1-val0);
		  indices1(xi,yi,0) = points.size;
		  points.insert(CImg<tp>::vector(Xi,Y,Z));
		}
		if ((edge&2) && indices1(nxi,yi,1)<0) {
		  const float Yi = Y + (isovalue-val1)*resy/(val2-val1); 
		  indices1(nxi,yi,1) = points.size;
		  points.insert(CImg<tp>::vector(nX,Yi,Z));
		}
		if ((edge&4) && indices1(xi,nyi,0)<0) { 
		  const float Xi = X + (isovalue-val3)*resx/(val2-val3);
		  indices1(xi,nyi,0) = points.size;
		  points.insert(CImg<tp>::vector(Xi,nY,Z));
		}
		if ((edge&8) && indices1(xi,yi,1)<0) { 
		  const float Yi = Y + (isovalue-val0)*resy/(val3-val0);
		  indices1(xi,yi,1) = points.size;
		  points.insert(CImg<tp>::vector(X,Yi,Z));
		}
		if ((edge&16) && indices2(xi,yi,0)<0) {
		  const float Xi = X + (isovalue-val4)*resx/(val5-val4);
		  indices2(xi,yi,0) = points.size;
		  points.insert(CImg<tp>::vector(Xi,Y,nZ));
		}
		if ((edge&32) && indices2(nxi,yi,1)<0) {
		  const float Yi = Y + (isovalue-val5)*resy/(val6-val5); 
		  indices2(nxi,yi,1) = points.size; 
		  points.insert(CImg<tp>::vector(nX,Yi,nZ));
		}
		if ((edge&64) && indices2(xi,nyi,0)<0) { 
		  const float Xi = X + (isovalue-val7)*resx/(val6-val7);
		  indices2(xi,nyi,0) = points.size; 
		  points.insert(CImg<tp>::vector(Xi,nY,nZ));
		}
		if ((edge&128) && indices2(xi,yi,1)<0)  { 
		  const float Yi = Y + (isovalue-val4)*resy/(val7-val4);
		  indices2(xi,yi,1) = points.size;
		  points.insert(CImg<tp>::vector(X,Yi,nZ));
		}
		if ((edge&256) && indices1(xi,yi,2)<0) {
		  const float Zi = Z+ (isovalue-val0)*resz/(val4-val0);
		  indices1(xi,yi,2) = points.size;
		  points.insert(CImg<tp>::vector(X,Y,Zi));
		}
		if ((edge&512) && indices1(nxi,yi,2)<0)  {
		  const float Zi = Z + (isovalue-val1)*resz/(val5-val1); 
		  indices1(nxi,yi,2) = points.size;
		  points.insert(CImg<tp>::vector(nX,Y,Zi));
		}
		if ((edge&1024) && indices1(nxi,nyi,2)<0) { 
		  const float Zi = Z + (isovalue-val2)*resz/(val6-val2);
		  indices1(nxi,nyi,2) = points.size; 
		  points.insert(CImg<tp>::vector(nX,nY,Zi));
		}
		if ((edge&2048) && indices1(xi,nyi,2)<0) { 
		  const float Zi = Z + (isovalue-val3)*resz/(val7-val3);
		  indices1(xi,nyi,2) = points.size;
		  points.insert(CImg<tp>::vector(X,nY,Zi));
		}

		// Create triangles
		for (int *triangle=triangles[configuration]; *triangle!=-1; ) {
		  const unsigned int p0 = *(triangle++), p1 = *(triangle++), p2 = *(triangle++);
		  const tf 
		i0 = (tf)(_marching_cubes_indice(p0,indices1,indices2,xi,yi,nxi,nyi)),
		i1 = (tf)(_marching_cubes_indice(p1,indices1,indices2,xi,yi,nxi,nyi)),
		i2 = (tf)(_marching_cubes_indice(p2,indices1,indices2,xi,yi,nxi,nyi));	    
		  if (invert_faces) primitives.insert(CImg<tf>::vector(i0,i1,i2));
		  else primitives.insert(CImg<tf>::vector(i0,i2,i1));
		}
	  }
	}
	  }
	  cimg::swap(values1,values2);
	  cimg::swap(indices1,indices2); 
	}
  }
	
  // Inner routine used by the Marching square algorithm
  template<typename t> inline int _marching_squares_indice(const unsigned int edge, const CImg<t>& indices1, const CImg<t>& indices2,
							   const unsigned int x, const unsigned int nx) {
	switch (edge) {
	case 0: return (int)indices1(x,0);
	case 1: return (int)indices1(nx,1);
	case 2: return (int)indices2(x,0);
	case 3: return (int)indices1(x,1);
	}
	return 0;
  }

  //! Polygonize an implicit 2D function by the marching squares algorithm  
  template<typename tfunc, typename tp, typename tf>
  inline void marching_squares(const tfunc& func, const float isovalue,
				   const float x0,const float y0,
				   const float x1,const float y1,
				   const float resx,const float resy,
				   CImgl<tp>& points, CImgl<tf>& primitives) {
	
	static unsigned int edges[16]={ 0x0, 0x9, 0x3, 0xa, 0x6, 0xf, 0x5, 0xc, 0xc, 0x5, 0xf, 0x6, 0xa, 0x3, 0x9, 0x0 };
	static int segments[16][4] = { { -1,-1,-1,-1 }, { 0,3,-1,-1 }, { 0,1,-1,-1 }, { 1,3,-1,-1 },
				   { 1,2,-1,-1 },   { 0,1,2,3 },   { 0,2,-1,-1 }, { 2,3,-1,-1 },
				   { 2,3,-1,-1 },   { 0,2,-1,-1},  { 0,3,1,2 },   { 1,2,-1,-1 },
				   { 1,3,-1,-1 },   { 0,1,-1,-1},  { 0,3,-1,-1},  { -1,-1,-1,-1 } };
	const unsigned int 
	  nx = (unsigned int)((x1-x0+1)/resx), nxm1 = nx-1,
	  ny = (unsigned int)((y1-y0+1)/resy), nym1 = ny-1;

	if (!nxm1 || !nym1) return;

	CImg<int> indices1(nx,1,1,2,-1), indices2(nx,1,1,2);
	CImg<float> values1(nx), values2(nx);
	float X = 0, Y = 0, nX = 0, nY = 0;

	// Fill first line with values
	cimg_mapX(values1,x) { values1(x) = (float)func(X,Y); X+=resx; }

	// Run the marching squares algorithm
	Y = y0; nY = Y + resy;
	for (unsigned int yi=0, nyi=1; yi<nym1; ++yi, ++nyi, Y=nY, nY+=resy) {
	  X = x0; nX = X + resx;
	  indices2.fill(-1);
	  for (unsigned int xi=0, nxi=1; xi<nxm1; ++xi, ++nxi, X=nX, nX+=resx) {

	// Determine cube configuration
	const float
	  val0 = values1(xi), val1 = values1(nxi),
	  val2 = values2(nxi) = (float)func(nX,nY),
	  val3 = values2(xi) = (float)func(X,nY);

	const unsigned int configuration = (val0<isovalue?1:0)  | (val1<isovalue?2:0)  | (val2<isovalue?4:0)  | (val3<isovalue?8:0),
	  edge = edges[configuration];
	
	// Compute intersection points
	if (edge) {
	  if ((edge&1) && indices1(xi,0)<0) {
		const float Xi = X + (isovalue-val0)*resx/(val1-val0);
		indices1(xi,0) = points.size;
		points.insert(CImg<tp>::vector(Xi,Y));
	  }
	  if ((edge&2) && indices1(nxi,1)<0) {
		const float Yi = Y + (isovalue-val1)*resy/(val2-val1); 
		indices1(nxi,1) = points.size;
		points.insert(CImg<tp>::vector(nX,Yi));
	  }
	  if ((edge&4) && indices2(xi,0)<0) { 
		const float Xi = X + (isovalue-val3)*resx/(val2-val3);
		indices2(xi,0) = points.size;
		points.insert(CImg<tp>::vector(Xi,nY));
	  }
	  if ((edge&8) && indices1(xi,1)<0) { 
		const float Yi = Y + (isovalue-val0)*resy/(val3-val0);
		indices1(xi,1) = points.size; 
		points.insert(CImg<tp>::vector(X,Yi));
	  }
	  
	  // Create segments
	  for (int *segment=segments[configuration]; *segment!=-1; ) {
		const unsigned int p0 = *(segment++), p1 = *(segment++);
		const tf 
		  i0 = (tf)(_marching_squares_indice(p0,indices1,indices2,xi,nxi)),
		  i1 = (tf)(_marching_squares_indice(p1,indices1,indices2,xi,nxi));
		primitives.insert(CImg<tf>::vector(i0,i1));
	  }
	}
	  }
	  values1.swap(values2);
	  indices1.swap(indices2);
	}
  }
  
  // End of cimg:: namespace  
}
  
  
  // End of cimg_library:: namespace
}

// Overcome VisualC++ 6.0 and DMC compilers namespace bug
#if ( defined(_MSC_VER) || defined(__DMC__) ) && defined(std)
#undef std
#endif

/*
 #------------------------------------------------------------------------------------
 #
 #
 # Additional documentation for the generation of the reference page (using doxygen)
 #
 #
 #------------------------------------------------------------------------------------
 */

/**
   \mainpage
   
   This is the reference documentation of <a href="http://cimg.sourceforge.net">the CImg Library</a>.
   These HTML pages have been generated using <a href="http://www.doxygen.org">doxygen</a>.
   It contains a detailed description of all classes and functions of the %CImg Library.
   If you have downloaded the CImg package, you actually have a local copy of these pages in the
   \c CImg/documentation/reference/ directory.
  
   Use the menu above to navigate through the documentation pages.
   As a first step, you may look at the list of <a href="modules.html">available modules</a>.

   A complete PDF version of this reference documentation is
   <a href="../CImg_reference.pdf">available here</a> for off-line reading.
   
**/

/** \addtogroup cimg_structure Introduction to the CImg Library */
/*@{*/
/**
  \page foo2

  The <b>CImg Library</b> is an image processing library, designed for C++ programmers.
  It provides useful classes and functions to load/save, display and process various types of images.
  
  \section s1 Library structure

  The %CImg Library consists in a <b>single header file</b> CImg.h providing a set of C++ template classes that
  can be used in your own sources, to load/save, process and display images or list of images.
  Very portable (Unix/X11,Windows, MacOS X, FreeBSD,..), efficient, simple to use, it's a pleasant toolkit
  for coding image processing stuffs in C++.

  The header file CImg.h contains all the classes and functions that compose the library itself.
  This is one originality of the %CImg Library. This particularly means that :
  - No pre-compilation of the library is needed, since the compilation of the CImg functions is done at the same time as
  the compilation of your own C++ code.
  - No complex dependencies have to be handled : Just include the CImg.h file, and you get a working C++ image processing toolkit.
  - The compilation is done on the fly : only CImg functionalities really used by your program are compiled and appear in the 
  compiled executable program. This leads to very compact code, without any unused stuffs.
  - Class members and functions are inlined, leading to better performance during the program execution.

  The %CImg Library is structured as follows :
  
  - All library classes and functions are defined in the namespace \ref cimg_library. This namespace
  encapsulates the library functionalities and avoid any class name collision that could happen with
  other includes. Generally, one uses this namespace as a default namespace :
  \code
  #include "CImg.h"
  using namespace cimg_library;
  ...
  \endcode

  - The namespace \ref cimg_library::cimg defines a set of \e low-level functions and variables used by the library.
  Documented functions in this namespace can be safely used in your own program. But, \b never use the
  \ref cimg_library::cimg namespace as a default namespace, since it contains functions whose names are already
  defined in the standard C/C++ library.

  - The class \ref cimg_library::CImg<T> represents images up to 4-dimensions wide, containing pixels of type \c T
  (template parameter). This is actually the main class of the library.

  - The class \ref cimg_library::CImgl<T> represents lists of cimg_library::CImg<T> images. It can be used for instance
  to store different frames of an image sequence.

  - The class \ref cimg_library::CImgDisplay is able to display images or image lists into graphical display windows.
  As you may guess, the code of this class is highly system-dependent but this is transparent for the programmer,
  as environment variables are automatically set by the CImg library (see also \ref cimg_environment).

  - The class \ref cimg_library::CImgStats represents image statistics. Use it to compute the
  minimum, maximum, mean and variance of pixel values of images, as well as the corresponding min/max pixel location.

  - The class \ref cimg_library::CImgException (and its subclasses) are used by the library to throw exceptions
  when errors occur. Those exceptions can be catched with a bloc <tt>try { ..} catch (CImgException) { .. }</tt>.
  Subclasses define precisely the type of encountered errors.

  Knowing these five classes is \b enough to get benefit of the %CImg Library functionalities.


  \section s2 CImg version of "Hello world".

  Below is a very simple code that creates a "Hello World" image. This shows you basically how a CImg program looks like.

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
	CImg<unsigned char> img(640,400,1,3);        // Define a 640x400 color image with 8 bits per color component.
	img.fill(0);                                 // Set pixel values to 0 (color : black)
	unsigned char purple[3]={255,0,255};         // Define a purple color
	img.draw_text("Hello World",100,100,purple); // Draw a purple "Hello world" at coordinates (100,100).
	img.display("My first CImg code");           // Display the image in a display window.
	return 0;
  }
  \endcode

  Which can be also written in a more compact way as :

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
	const unsigned char purple[3]={255,0,255};
	CImg<unsigned char>(640,400,1,3,0).draw_text("Hello World",100,100,purple).display("My first CImg code");
	return 0;
  }
  \endcode

  Generally, you can write very small code that performs complex image processing tasks. The %CImg Library is very simple
  to use and provide a lot of interesting algorithms for image manipulation.
  
  \section s3 How to compile ?

  The CImg library is a very light and user-friendly library : only standard system libraries are used.
  It avoid to handle complex dependancies and problems with library compatibility.
  The only thing you need is a (quite modern) C++ compiler :
  
  - <b>Microsoft Visual C++ 6.0 and Visual Studio.NET</b> : Use project files and solution files provided in the 
  %CImg Library package (directory 'compilation/') to see how it works.
  - <b>Intel ICL compiler</b> : Use the following command to compile a CImg-based program with ICL :
  \code
  icl /Ox hello_world.cpp user32.lib gdi32.lib
  \endcode
  - <b>g++ (MingW windows version)</b> : Use the following command to compile a CImg-based program with g++, on Windows :
  \code
  g++ -o hello_word.exe hello_word.cpp -O2 -lgdi32
  \endcode
  - <b>g++ (Linux version)</b> : Use the following command to compile a CImg-based program with g++, on Linux :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
  \endcode
  - <b>g++ (Solaris version)</b> : Use the following command to compile a CImg-based program with g++, on Solaris :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -lm -lpthread -R/usr/X11R6/lib -lrt -lnsl -lsocket
  \endcode
  - <b>g++ (Mac OS X version)</b> : Use the following command to compile a CImg-based program with g++, on Mac OS X :
  \code
  g++ -o hello_word.exe hello_world.cpp -O2 -lm -lpthread -L/usr/X11R6/lib -lm -lpthread -lX11
  \endcode
  - <b>Dev-Cpp</b> : Use the project file provided in the CImg library package to see how it works.

  If you are using another compilers and encounter problems, please
  <a href="http://www.greyc.ensicaen.fr/~dtschump">write me</a> since maintaining compatibility is one
  of the priority of the %CImg Library. Nevertheless, old compilers that does not respect the C++ norm will not
  support the %CImg Library.

  \section s4 What's next ?

  If you are ready to get more, and to start writing more serious programs
  with CImg, you are invited to go to the \ref cimg_tutorial section.

**/
/*@}*/

/** \addtogroup cimg_environment Setting Environment Variables */
/*@{*/
/**
  \page foo1
  
  The CImg library is a multiplatform library, working on a wide variety of systems.
  This implies the existence of some \e environment \e variables that must be correctly defined
  depending on your current system.
  Most of the time, the %CImg Library defines these variables automatically
  (for popular systems). Anyway, if your system is not recognized, you will have to set the environment
  variables by hand. Here is a quick explanations of environment variables.\n
  
  Setting the environment variables is done with the <tt>#define</tt> keyword.
  This setting must be done <i>before including the file CImg.h</i> in your source code.
  For instance,
  defining the environment variable \c cimg_display_type would be done like this :
  \code
  #define cimg_display_type 0
  #include "CImg.h"
  ...
  \endcode

  Here are the different environment variables used by the %CImg Library :
  
  - \b \c cimg_OS : This variable defines the type of your Operating System. It can be set to \b 1 (\e Unix),
  \b 2 (\e Windows), or \b 0 (\e Other \e configuration).
  It should be actually auto-detected by the CImg library. If this is not the case (<tt>cimg_OS=0</tt>), you
  will probably have to tune the environment variables described below.

  - \b \c cimg_display_type : This variable defines the type of graphical library used to
  display images in windows. It can be set to 0 (no display library available), \b 1 (X11-based display) or
  \b 2 (Windows-GDI display).
  If you are running on a system without X11 or Windows-GDI ability, please set this variable to \c 0.
  This will disable the display support, since the %CImg Library doesn't contain the necessary code to display
  images on systems other than X11 or Windows GDI.

  - \b \c cimg_color_terminal : This variable tells the library if the system terminal has VT100 color capabilities.
  It can be \e defined or \e not \e defined. Define this variable to get colored output on your terminal, 
  when using the %CImg Library.
  
  - \b \c cimg_debug : This variable defines the level of run-time debug messages that will be displayed by
  the %CImg Library. It can be set to 0 (no debug messages), 1 (normal debug messages, which is
  the default value), or 2 (high debug messages). Note that setting this value to 2 may slow down your
  program since more debug tests are made by the library (particularly to check if pixel access is made outside
  image boundaries). See also CImgException to better understand how debug messages are working.
  
  - \b \c cimg_convert_path : This variables tells the library where the ImageMagick's \e convert tool is located.
  Setting this variable should not be necessary if ImageMagick is installed on a standard directory, or
  if \e convert is in your system PATH variable. This macro should be defined only if the ImageMagick's 
  \e convert tool is not found automatically, when trying to read compressed image format (GIF,PNG,...). 
  See also cimg_library::CImg::get_load_convert() and cimg_library::CImg::save_convert() for more informations.

  - \b \c cimg_temporary_path : This variable tells the library where it can find a directory to store
  temporary files. Setting this variable should not be necessary if you are running on a standard system.
  This macro should be defined only when troubles are encountered when trying to read
  compressed image format (GIF,PNG,...).
  See also cimg_library::CImg::get_load_convert() and cimg_library::CImg::save_convert() for more informations.

  - \b \c cimg_plugin : This variable tells the library to use a plugin file to add features to the CImg<T> class.
  Define it with the path of your plugin file, if you want to add member functions to the CImg<T> class,
  without having to modify directly the \c "CImg.h" file. An include of the plugin file is performed in the CImg<T>
  class. If \c cimg_plugin if not specified (default), no include is done.
  
  - \b \c cimgl_plugin : Same as \c cimg_plugin, but to add features to the CImgl<T> class.
  
  - \b \c cimgdisplay_plugin : Same as \c cimg_plugin, but to add features to the CImgDisplay<T> class.

  - \b \c cimgstats_plugin : Same as \c cimg_plugin, but to add features to the CImgStats<T> class.

  All these compilation variables can be checked, using the function cimg_library::cimg::info(), which
  displays a list of the different configuration variables and their values on the standard error output.
**/
/*@}*/

/** \addtogroup cimg_tutorial Tutorial : Getting Started. */
/*@{*/
/**
  \page foo3
  
  Let's start to write our first program to get the idea. This will demonstrate how to load and create images, as well as handle image 
  display and mouse events.
  Assume we want to load a color image <tt>lena.jpg</tt>, smooth it, display it in a windows, and enter an event loop so that clicking a
  point in the image with the mouse will draw the intensity profiles of (R,G,B) of the corresponding image line (in another window).
  Yes, that sounds quite complex for a first code, but don't worry, it will be very simple using the CImg library ! Well, just look
  at the code below, it does the task :

  \code
  #include "CImg.h"
  using namespace cimg_library;

  int main() {
	CImg<unsigned char> image("lena.jpg"), visu(500,400,1,3,0);
	const unsigned char red[3]={255,0,0}, green[3]={0,255,0}, blue[3]={0,0,255};
	image.blur(2.5);
	CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile");
	while (!main_disp.is_closed && !draw_disp.is_closed) {
	  main_disp.wait();
	  if (main_disp.button && main_disp.mouse_y>=0) {
		const int y = main_disp.mouse_y;
	visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.dimx()-1,y,0,0),red,0,256,0);
	visu.draw_graph(image.get_crop(0,y,0,1,image.dimx()-1,y,0,1),green,0,256,0);
	visu.draw_graph(image.get_crop(0,y,0,2,image.dimx()-1,y,0,2),blue,0,256,0).display(draw_disp);
	}
	  }
	return 0;
  }
  \endcode
  
  Here is a screenshot of the resulting program :

  <img SRC="../img/tutorial.jpg">

  And here is the detailled explanation of the source, line by line :

  \code #include "CImg.h" \endcode
  Include the main and only header file of the CImg library.
  \code using namespace cimg_library; \endcode
  Use the library namespace to ease the declarations afterward.
  \code int main() { \endcode
  Definition of the main function.
  \code CImg<unsigned char> image("lena.jpg"), visu(500,400,1,3,0); \endcode
  Creation of two instances of images of \c unsigned \c char pixels.
  The first image \c image is initialized by reading an image file from the disk. 
  Here, <tt>lena.jpg</tt> must be in the same directory than the current program.
  Note that you must also have installed the \e ImageMagick package in order to be able to read JPG images.
  The second image \c visu is initialized as a black color image with dimension <tt>dx=500</tt>, <tt>dy=400</tt>, 
  <tt>dz=1</tt> (here, it is a 2D image, not a 3D one), and <tt>dv=3</tt> (each pixel has 3 'vector' channels R,G,B).
  The last argument in the constructor defines the default value of the pixel values
  (here \c 0, which means that \c visu will be initially black).
  \code const unsigned char red[3]={255,0,0}, green[3]={0,255,0}, blue[3]={0,0,255}; \endcode
  Definition of three different colors as array of unsigned char. This will be used to draw plots with different colors.
  \code image.blur(2.5); \endcode
  Blur the image, with a gaussian blur and a standard variation of 2.5. Note that most of the CImg functions have two versions :
  one that acts in-place (which is the case of blur), and one that returns the result as a new image (the name of the function 
  begins then with <tt>get_</tt>&nbsp;). In this case, one could have also written <tt>image = image.get_blur(2.5);</tt>
  (more expensive, since it needs an additional copy operation).
  \code CImgDisplay main_disp(image,"Click a point"), draw_disp(visu,"Intensity profile"); \endcode
  Creation of two display windows, one for the input image image, and one for the image visu which will be display intensity profiles.
  By default, CImg displays handles events (mouse,keyboard,..). On Windows, there is a way to create fullscreen displays.
  \code while (!main_disp.is_closed && !draw_disp.is_closed) { \endcode
  Enter the event loop, the code will exit when one of the two display windows is closed.
  \code main_disp.wait(); \endcode
  Wait for an event (mouse, keyboard,..) in the display window \c main_disp.
  \code if (main_disp.button && main_disp.mouse_y>=0) { \endcode
  Test if the mouse button has been clicked on the image area.
  One may distinguish between the 3 different mouse buttons,
  but in this case it is not necessary
  \code const int y = main_disp.mouse_y; \endcode
  Get the image line y-coordinate that has been clicked.
  \code visu.fill(0).draw_graph(image.get_crop(0,y,0,0,image.dimx()-1,y,0,0),red,0,256,0); \endcode
  This line illustrates the pipeline property of most of the CImg class functions. The first function <tt>fill(0)</tt> simply sets
  all pixel values with 0 (i.e. clear the image \c visu). The interesting thing is that it returns a reference to
  \c visu and then, can be pipelined with the function \c draw_graph() which draws a plot in the image \c visu.
  The plot data are given by another image (the first argument of \c draw_graph()). In this case, the given image is 
  the red-component of the line y of the original image, retrieved by the function \c get_crop() which returns a
  sub-image of the image \c image. Remember that images coordinates are 4D (x,y,z,v) and for color images,
  the R,G,B channels are respectively given by <tt>v=0, v=1</tt> and <tt>v=2</tt>.
  \code visu.draw_graph(image.get_crop(0,y,0,1,image.dimx()-1,y,0,1),green,0,256,0); \endcode
  Plot the intensity profile for the green channel of the clicked line.
  \code visu.draw_graph(image.get_crop(0,y,0,2,image.dimx()-1,y,0,2),blue,0,256,0).display(draw_disp); \endcode
  Same thing for the blue channel. Note how the function (which return a reference to \c visu) is pipelined with the function
  \c display() that just paints the image visu in the corresponding display window.
  \code ...till the end \endcode
  I don't think you need more explanations !

  As you have noticed, the CImg library allows to write very small and intuitive code. Note also that this source will perfectly 
  work on Unix and Windows systems. Take also a look to the examples provided in the CImg package (
  directory \c examples/ ). It will show you how CImg-based code can be surprisingly small. 
  Moreover, there is surely one example close to what you want to do.
  A good start will be to look at the file <tt>CImg_demo.cpp</tt> which contains small and various examples of what you can do
  with the %CImg Library. All CImg classes are used in this source, and the code can be easily modified to see what happens. 

**/
/*@}*/

/** \addtogroup cimg_drawing Using Drawing Functions. */
/*@{*/
/**
  \page foo5

  \section s5 Using Drawing Functions.

  This section tells more about drawing features in CImg images.
  Drawing functions list can be found in <a href="structCImg.html">the CImg functions list</a>
  (section \b Drawing Functions),
  and are all defined on a common basis. Here are the important points to understand before using
  drawing functions :
  
  - Drawing is performed on the instance image. Drawing functions parameters
  are defined as \e const variables and return a reference to the current instance <tt>(*this)</tt>,
  so that drawing functions can be pipelined (see examples below).
  Drawing is usually done in 2D color images but can be performed in 3D images with any vector-valued dimension,
  and with any possible pixel type.

  - A color parameter is always needed to draw features in an image. The color must be defined as a C-style array
  whose dimension is at least

**/
/*@}*/

/** \addtogroup cimg_loops Using Image Loops. */
/*@{*/
/**
  \page foo_lo
  The %CImg Library provides different macros that define useful iterative loops over an image.
  Basically, it can be used to replace one or several <tt>for(..)</tt> instructions, but it also proposes
  interesting extensions to classical loops.
  Below is a list of all existing loop macros, classified in four different categories :
  - \ref lo1
  - \ref lo4
  - \ref lo5
  - \ref lo6

  \section lo1 Loops over the pixel buffer

  Loops over the pixel buffer are really basic loops that iterate a pointer on the pixel data buffer
  of a \c cimg_library::CImg image. Two macros are defined for this purpose :
  
  - \b cimg_map(img,ptr,T) :
  This macro loops over the pixel data buffer of the image \c img, using a pointer <tt>T* ptr</tt>,
  starting from the end of the buffer (last pixel) till the beginning of the buffer (first pixel).
	  - \c img must be a (non empty) \c cimg_library::CImg image of pixels \c T.
	  - \c ptr is a pointer of type \c T*.
  This kind of loop should not appear a lot in your own source code, since this is a low-level loop
  and many functions of the CImg class may be used instead. Here is an example of use :
  \code
  CImg<float> img(320,200);
  cimg_map(img,ptr,float) { *ptr=0; }      // Equivalent to 'img.fill(0);'
  \endcode

  - \b cimg_mapoff(img,off) :
  This macro loops over the pixel data buffer of the image \c img, using an offset \c ,
  starting from the beginning of the buffer (first pixel, \c off=0)
  till the end of the buffer (last pixel value, <tt>off = img.size()-1</tt>).
	  - \c img must be a (non empty) cimg_library::CImg<T> image of pixels \c T.
	  - \c off is an inner-loop variable, only defined inside the scope of the loop.

  Here is an example of use :
  \code
  CImg<float> img(320,200);
  cimg_mapoff(img,off) { img[off]=0; }  // Equivalent to 'img.fill(0);'
  \endcode

  \section lo4 Loops over image dimensions

  The following loops are probably the most used loops in image processing programs.
  They allow to loop over the image along one or several dimensions, along a raster scan course.
  Here is the list of such loop macros for a single dimension :
  - \b cimg_mapX(img,x) : equivalent to : <tt>for (int x=0; x<img.dimx(); x++)</tt>.
  - \b cimg_mapY(img,y) : equivalent to : <tt>for (int y=0; y<img.dimy(); y++)</tt>.
  - \b cimg_mapZ(img,z) : equivalent to : <tt>for (int z=0; z<img.dimz(); z++)</tt>.
  - \b cimg_mapV(img,v) : equivalent to : <tt>for (int v=0; v<img.dimv(); v++)</tt>.

  Combinations of these macros are also defined as other loop macros, allowing to loop directly over 2D, 3D or 4D images :
  - \b cimg_mapXY(img,x,y) : equivalent to : \c cimg_mapY(img,y) \c cimg_mapX(img,x).
  - \b cimg_mapXZ(img,x,z) : equivalent to : \c cimg_mapZ(img,z) \c cimg_mapX(img,x).
  - \b cimg_mapYZ(img,y,z) : equivalent to : \c cimg_mapZ(img,z) \c cimg_mapY(img,y).
  - \b cimg_mapXV(img,x,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapX(img,x).
  - \b cimg_mapYV(img,y,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapY(img,y).
  - \b cimg_mapZV(img,z,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapZ(img,z).
  - \b cimg_mapXYZ(img,x,y,z) : equivalent to : \c cimg_mapZ(img,z) \c cimg_mapXY(img,x,y).
  - \b cimg_mapXYV(img,x,y,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapXY(img,x,y).
  - \b cimg_mapXZV(img,x,z,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapXZ(img,x,z).
  - \b cimg_mapYZV(img,y,z,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapYZ(img,y,z).
  - \b cimg_mapXYZV(img,x,y,z,v) : equivalent to : \c cimg_mapV(img,v) \c cimg_mapXYZ(img,x,y,z).

  - For all these loops, \c x,\c y,\c z and \c v are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  - \c img must be a (non empty) cimg_library::CImg image.

  Here is an example of use that creates an image with a smooth color gradient :
  \code
  CImg<unsigned char> img(256,256,1,3);       // Define a 256x256 color image
  cimg_mapXYV(img,x,y,v) { img(x,y,v) = (x+y)*(v+1)/6; }
  img.display("Color gradient");
  \endcode

  \section lo5 Loops over interior regions and borders.

  Similar macros are also defined to loop only on the border of an image, or inside the image (excluding the border).
  The border may be several pixel wide :

  - \b cimg_imapX(img,x,n) : Loop along the x-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_imapY(img,y,n) : Loop along the y-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_imapZ(img,z,n) : Loop along the z-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_imapV(img,v,n) : Loop along the v-axis, except for pixels inside a border of \p n pixels wide.
  - \b cimg_imapXY(img,x,y,n) : Loop along the (x,y)-axes, excepted for pixels inside a border of \p n pixels wide.
  - \b cimg_imapXYZ(img,x,y,z,n) : Loop along the (x,y,z)-axes, excepted for pixels inside a border of \p n pixels wide.

  And also :

  - \b cimg_bmapX(img,x,n) : Loop along the x-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_bmapY(img,y,n) : Loop along the y-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_bmapZ(img,z,n) : Loop along the z-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_bmapV(img,v,n) : Loop along the z-axis, only for pixels inside a border of \p n pixels wide.
  - \b cimg_bmapXY(img,x,y,n) : Loop along the (x,y)-axes, only for pixels inside a border of \p n pixels wide.
  - \b cimg_bmapXYZ(img,x,y,z,n) : Loop along the (x,y,z)-axes, only for pixels inside a border of \p n pixels wide.

  - For all these loops, \c x,\c y,\c z and \c v are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  - \c img must be a (non empty) cimg_library::CImg image.
  - The constant \c n stands for the size of the border.

  Here is an example of use, to create a 2d grayscale image with two different intensity gradients :
  \code
  CImg<> img(256,256);
  cimg_imapXY(img,x,y,50) img(x,y) = x+y;
  cimg_bmapXY(img,x,y,50) img(x,y) = x-y;
  img.display();
  \endcode

  \section lo6 Loops using neighborhoods.
  
  Inside an image loop, it is often useful to get values of neighborhood pixels of the
  current pixel at the loop location.
  The %CImg Library provides a very smart and fast mechanism for this purpose, with the definition
  of several loop macros that remember the neighborhood values of the pixels.
  The use of these macros can highly optimize your code, and also simplify your program.

  \subsection lo7 Neighborhood-based loops for 2D images

  For 2D images, the neighborhood-based loop macros are : 

  - \b cimg_map2x2x1(img,x,y,z,v,I) : Loop along the (x,y)-axes using a centered 2x2 neighborhood.
  - \b cimg_map3x3x1(img,x,y,z,v,I) : Loop along the (x,y)-axes using a centered 3x3 neighborhood.
  - \b cimg_map4x4x1(img,x,y,z,v,I) : Loop along the (x,y)-axes using a centered 4x4 neighborhood.
  - \b cimg_map5x5x1(img,x,y,z,v,I) : Loop along the (x,y)-axes using a centered 5x5 neighborhood.

  For all these loops, \c x and \c y are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  \c img is a non empty CImg<T> image. \c z and \c v are constants that define on which image slice and
  vector channel the loop must apply (usually both 0 for grayscale 2D images).
  Finally, \c I is the 2x2, 3x3, 4x4 or 5x5 neighborhood that will be updated with the correct pixel values
  during the loop (see \ref lo9).

  \subsection lo8 Neighborhood-based loops for 3D images

  For 3D images, the neighborhood-based loop macros are : 

  - \b cimg_map2x2x2(img,x,y,z,v,I) : Loop along the (x,y,z)-axes using a centered 2x2x2 neighborhood.
  - \b cimg_map3x3x3(img,x,y,z,v,I) : Loop along the (x,y,z)-axes using a centered 3x3x3 neighborhood.

  For all these loops, \c x, \c y and \c z are inner-defined variables only visible inside the scope of the loop.
  They don't have to be defined before the call of the macro.
  \c img is a non empty CImg<T> image. \c v is a constant that defines on which image channel
  the loop must apply (usually 0 for grayscale 3D images).
  Finally, \c I is the 2x2x2 or 3x3x3 neighborhood that will be updated with the correct pixel values
  during the loop (see \ref lo9).

  \subsection lo9 Defining neighborhoods

  The CImg library defines a neighborhood as a set of named \e variables or \e references, declared
  using specific CImg macros :

  - \b CImg_2x2x1(I,type) : Define a 2x2 neighborhood named \c I, of type \c type.
  - \b CImg_3x3x1(I,type) : Define a 3x3 neighborhood named \c I, of type \c type.
  - \b CImg_4x4x1(I,type) : Define a 4x4 neighborhood named \c I, of type \c type.
  - \b CImg_5x5x1(I,type) : Define a 5x5 neighborhood named \c I, of type \c type.
  - \b CImg_2x2x2(I,type) : Define a 2x2x2 neighborhood named \c I, of type \c type.
  - \b CImg_3x3x3(I,type) : Define a 3x3x3 neighborhood named \c I, of type \c type.

  Actually, \c I is a \e generic \e name for the neighborhood. In fact, these macros declare
  a \e set of new variables.
  For instance, defining a 3x3 neighborhood \c CImg_3x3x1(I,float) declares 9 different float variables
  \c Ipp,\c Icp,\c Inp,\c Ipc,\c Icc,\c Inc,\c Ipn,\c Icn,\c Inn which correspond to each pixel value of
  a 3x3 neighborhood.
  Variable indices are \c p,\c c or \c n, and stand respectively for \e 'previous', \e 'current' and \e 'next'.
  First indice denotes the \c x-axis, second indice denotes the \c y-axis.
  Then, the names of the variables are directly related to the position of the corresponding pixels
  in the neighborhood. For 3D neighborhoods, a third indice denotes the \c z-axis.
  Then, inside a neighborhood loop, you will have the following equivalence :
  - <tt>Ipp = img(x-1,y-1)</tt>
  - <tt>Icn = img(x,y+1)</tt>
  - <tt>Inp = img(x+1,y-1)</tt>
  - <tt>Inpc = img(x+1,y-1,z)</tt>
  - <tt>Ippn = img(x-1,y-1,z+1)</tt>
  - and so on...

  For bigger neighborhoods, such as 4x4 or 5x5 neighborhoods, two additionnal indices are introduced :
  \c a (stands for \e 'after') and \c b (stands for \e 'before'), so that :
  - <tt>Ibb = img(x-2,y-2)</tt>
  - <tt>Ina = img(x+1,y+2)</tt>
  - and so on...

  The value of a neighborhood pixel outside the image range (image border problem) is automatically set to the same
  values than the nearest valid pixel in the image (this is also called the \e Neumann \e border \e condition).

  \subsection lo10 Neighborhood as a reference
  It is also possible to define neighborhood variables as references to classical C-arrays or CImg<T> images, instead of
  allocating new variables. This is done by adding \c _ref to the macro names used for the neighborhood definition :

  - \b CImg_2x2x1_ref(I,type,tab) : Define a 2x2 neighborhood named \c I, of type \c type, as a reference to \c tab.
  - \b CImg_3x3x1_ref(I,type,tab) : Define a 3x3 neighborhood named \c I, of type \c type, as a reference to \c tab.
  - \b CImg_4x4x1_ref(I,type,tab) : Define a 4x4 neighborhood named \c I, of type \c type, as a reference to \c tab.
  - \b CImg_5x5x1_ref(I,type,tab) : Define a 5x5 neighborhood named \c I, of type \c type, as a reference to \c tab.
  - \b CImg_2x2x2_ref(I,type,tab) : Define a 2x2x2 neighborhood named \c I, of type \c type, as a reference to \c tab.
  - \b CImg_3x3x3_ref(I,type,tab) : Define a 3x3x3 neighborhood named \c I, of type \c type, as a reference to \c tab.

  \c tab can be a one-dimensionnal C-style array, or a non empty \c CImg<T> image. Both objects must have
  same sizes as the considered neighborhoods.

  \subsection lo11 Example codes
  More than a long discussion, the above example will demonstrate how to compute the gradient norm of a 3D volume
  using the \c cimg_map3x3x3() loop macro :

  \code
  CImg<float> volume("IRM.hdr");        // Load an IRM volume from an Analyze7.5 file
  CImg_3x3x3(I,float);                  // Define a 3x3x3 neighborhood
  CImg<float> gradnorm(volume,false);   // Create an image with same size as 'volume'
  cimg_map3x3x3(volume,x,y,z,0,I) {     // Loop over the volume, using the neighborhood I
	const float ix = 0.5f*(Incc-Ipcc);  // Compute the derivative along the x-axis.
	const float iy = 0.5f*(Icnc-Icpc);  // Compute the derivative along the y-axis.
	const float iz = 0.5f*(Iccn-Iccp);  // Compute the derivative along the z-axis.
	gradnorm(x,y,z) = std::sqrt(ix*ix+iy*iy+iz*iz);  // Set the gradient norm in the destination image
  }
  gradnorm.display("Gradient norm");
  \endcode
  
  And the following example shows how to deal with neighborhood references to blur a color image by averaging
  pixel values on a 5x5 neighborhood.

  \code
  CImg<unsigned char> src("image_color.jpg"), dest(src,false), neighbor(5,5);  // Image definitions.
  typedef unsigned char uchar;             // Avoid space in the second parameter of the macro CImg_5x5x1 below.
  CImg_5x5x1_ref(N,uchar,neighbor);        // Define a 5x5 neighborhood as a reference to the 5x5 image neighbor.
  cimg_mapV(src,k)                         // Standard loop on color channels
	 cimg_map5x5x1(src,x,y,0,k,N)          // 5x5 neighborhood loop.
	   dest(x,y,k) = neighbor.sum()/(5*5); // Averaging pixels to filter the color image.
  CImgl<unsigned char> visu(src,dest);
  visu.display("Original + Filtered");     // Display both original and filtered image.
  \endcode
  
  Note that in this example, we didn't use directly the variables Nbb,Nbp,..,Ncc,... since
  there are only references to the neighborhood image \c neighbor. We rather used a member function of \c neighbor.

  As you can see, explaining the use of the CImg neighborhood macros is actually more difficult than using them !

**/
/*@}*/

/** \addtogroup cimg_displays Using Display Windows. */
/*@{*/
/**
  \page foo_di

  When opening a display window, you can choose the way the pixel values will be normalized
  before being displayed on the screen. Screen displays only support color values between [0,255],
  and some
  
  When displaying an image into the display window using CImgDisplay::display(), values of
  the image pixels can be eventually linearly normalized between [0,255] for visualization purposes.
  This may be useful for instance when displaying \p CImg<double> images with pixel values
  between [0,1].
  The normalization behavior depends on the value of \p normalize which can be either \p 0,\p 1 or \p 2 :
  - \p 0 : No pixel normalization is performed when displaying an image. This is the fastest
  process, but you must be sure your displayed image have pixel values inside the range [0,255].
  - \p 1 : Pixel value normalization is done for each new image display. Image pixels are
  not modified themselves, only displayed pixels are normalized.
  - \p 2 : Pixel value normalization is done for the first image display, then the
  normalization parameters are kept and used for all the next image displays.
  
**/
/*@}*/

/** \addtogroup cimg_storage How pixel data are stored with CImg. */
/*@{*/
/**
  \page foo_store

  TODO
**/
/*@}*/

/** \addtogroup cimg_files_io Files IO in CImg. */
/*@{*/
/**
  \page foo_fi

  The %CImg Library can NATIVELY handle the following file formats :
  - RAW : consists in a very simple header (in ascii), then the image data.
  - ASC (Ascii)
  - HDR (Analyze 7.5)
  - INR (Inrimage)
  - PPM/PGM (Portable Pixmap)
  - BMP (uncompressed)
  - PAN (Pandore-5)
  - DLM (Matlab ASCII)
 
  If ImageMagick is installed, The %CImg Library can save image in formats handled by ImageMagick : JPG, GIF, PNG, TIF,...

**/
/*@}*/

/** \addtogroup cimg_options Retrieving Command Line Arguments. */
/*@{*/
/**
  \page foo_so

   The CImg library offers facilities to retrieve command line arguments in a console-based
   program, as it is a commonly needed operation.
   Two macros \c cimg_usage() and \c cimg_option() are defined for this purpose.
   Using these macros allows to easily retrieve options values from the command line.
   Moreover, invoking the corresponding executable with the option \c -h or \c --help will
   automatically display the program usage, followed by the list of requested options.

   \section so1 The cimg_usage() macro

   The macro \c cimg_usage(usage) may be used to describe the program goal and usage.
   It is generally inserted one time after the <tt>int main(int argc,char **argv)</tt> definition.
  
   \param usage : A string describing the program goal and usage.
   \pre The function where \c cimg_usage() is used must have correctly defined \c argc and \c argv variables.

   \section so2 The cimg_option() macro

   The macro \c cimg_option(name,default,usage) may be used to retrieve an option value from the command line.

   \param name    : The name of the option to be retrieved from the command line.
   \param default : The default value returned by the macro if no options \p name has been specified when running the program.
   \param usage   : A brief explanation of the option. If \c usage==0, the option won't appear on the option list
					when invoking the executable with options \c -h or \c --help (hidden option).
				  
   \return \c cimg_option() returns an object that has the \e same \e type than the default value \c default.
   The return value is equal to the one specified on the command line. If no such option have been specified,
   the return value is equal to the default value \c default.
   Warning, this can be confusing in some situations (look at the end of the next section).
   \pre The function where \c cimg_option() is used must have correctly defined \c argc and \c argv variables.

   \section so3 Example of use

   The code below uses the macros \c cimg_usage() and \c cimg_option().
   It loads an image, smoothes it an quantifies it with a specified number of values.
   \code
   #include "CImg.h"
   using namespace cimg_library;
   int main(int argc,char **argv) {
	 cimg_usage("Retrieve command line arguments");
	 const char* filename = cimg_option("-i","image.gif","Input image file");
	 const char* output   = cimg_option("-o",(const char*)0,"Output image file");
	 const double sigma   = cimg_option("-s",1.0,"Standard variation of the gaussian smoothing");
	 const  int nblevels  = cimg_option("-n",16,"Number of quantification levels");
	 const bool hidden    = cimg_option("-hidden",false,0);      // This is a hidden option

	 CImg<unsigned char> img(filename);
	 img.blur(sigma).quantize(nblevels);
	 if (output) img.save(output); else img.display("Output image");
	 if (hidden) std::fprintf(stderr,"You found me !\n");
	 return 0;
   }
   \endcode

   Invoking the corresponding executable with <tt>test -h -hidden -n 20 -i foo.jpg</tt> will display :
   \verbatim
   ./test -h -hidden -n 20 -i foo.jpg

 test : Retrieve command line arguments (Oct 16 2004, 12:34:26)

	-i       = foo.jpg      : Input image file
	-o       = 0            : Output image file
	-s       = 1            : Standard variation of the gaussian smoothing
	-n       = 20           : Number of quantification levels

   You found me !
\endverbatim

   \warning As the type of object returned by the macro \c cimg_option(option,default,usage) 
   is defined by the type of \c default, undesired casts may appear when writting code such as :
   \code
   const double sigma = cimg_option("-val",0,"A floating point value");
   \endcode
   In this case, \c sigma will always be equal to an integer (since the default value \c 0 is an integer).
   When passing a float value on the command line, a \e float \e to \e integer cast is then done,
   truncating the given parameter to an integer value (this is surely not a desired behavior).
   You must specify <tt>0.0</tt> as the default value in this case.

   \section so4 How to learn more about command line options ?
   You should take a look at the examples <tt>examples/inrcast.cpp</tt> provided in the %CImg Library package.
   This is a command line based image converter which intensively uses the \c cimg_option() and \c cimg_usage()
   macros to retrieve command line parameters.
**/
/*@}*/

#endif

// Local Variables:
// mode: c++
// End:
