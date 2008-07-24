/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
* 
* Distributable under the terms of either the Apache License (Version 2.0) or 
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#ifndef lucene_sharedheader_h
#define lucene_sharedheader_h

/**
* This header contains definitions and macros for helping create cross-platform code.
* It is primarily for use by the clucene-core library, but is split off so that
* other applications such as the demo, test, benchmarks, etc can benefit from the
* cross platform code. Cross platform code is not distributed with the clucene-core
* and is not available through the shared library.
*/

#include "CLucene/clucene-config.h"

//some early definitions
#if defined(_MSC_VER)
    //this has to be included very early, or visual studio will ignore it
    #pragma warning(disable : 4786) //ignore: identifier was truncated to '255' characters in the debug information
    #define _LUCENE_PRAGMA_WARNINGS //tell lucene to display warnings using pragmas instead of #warning
#endif
#if defined(__BORLANDC__)
    #define _LUCENE_PRAGMA_WARNINGS //tell lucene to display warnings using pragmas instead of #warning
#endif


////////////////////////////////////////////////////////
//Are we in unicode mode?
////////////////////////////////////////////////////////
#if defined(_MBCS) || defined(_ASCII)
 #undef _ASCII
 #undef _UCS2
 #define _ASCII
#elif defined(_UNICODE)
 #ifndef _UCS2
  #define _UCS2
 #endif
#elif !defined(_UCS2)
 #define _UCS2
#endif

//msvc needs unicode define so that it uses unicode library
#ifdef _UCS2
	#undef _UNICODE
	#define _UNICODE
	#undef _ASCII
#else
	#undef _UNICODE
	#undef _UCS2
#endif
////////////////////////////////////////////////////////



////////////////////////////////////////////////////////
//platform includes that MUST be included for the public headers to work...
////////////////////////////////////////////////////////
#include <cstddef> //need this for wchar_t, size_t, NULL
#ifdef _CL_HAVE_STDINT_H
    #include <stdint.h> //need this for int32_t, etc
#endif
#include <math.h> //required for float_t
#include <string> //need to include this really early...

#ifdef _CL_HAVE_TCHAR_H
    #include <tchar.h> //required for _T and TCHAR
#endif

////////////////////////////////////////////////////////
//namespace helper
////////////////////////////////////////////////////////
#if defined(_LUCENE_DONTIMPLEMENT_NS_MACROS)
    //do nothing
#elif !defined(DISABLE_NAMESPACE) && defined(_CL_HAVE_NAMESPACES)
	#define CL_NS_DEF(sub) namespace lucene{ namespace sub{
	#define CL_NS_DEF2(sub,sub2) namespace lucene{ namespace sub{ namespace sub2 {

	#define CL_NS_END }}
	#define CL_NS_END2 }}}

	#define CL_NS_USE(sub) using namespace lucene::sub;
	#define CL_NS_USE2(sub,sub2) using namespace lucene::sub::sub2;

	#define CL_NS(sub) lucene::sub
	#define CL_NS2(sub,sub2) lucene::sub::sub2
	    
	#define CL_STRUCT_DEF(sub,clazz) namespace lucene { namespace sub{ struct clazz; } }
	#define CL_CLASS_DEF(sub,clazz) namespace lucene { namespace sub{ class clazz; } }
#else
	#define CL_NS_DEF(sub)
	#define CL_NS_DEF2(sub, sub2)
	#define CL_NS_END
	#define CL_NS_END2
	#define CL_NS_USE(sub)
	#define CL_NS_USE2(sub,sub2)
	#define CL_NS(sub)
	#define CL_NS2(sub,sub2)
	#define CL_CLASS_DEF(sub,clazz) class clazz;
#endif

#if defined(LUCENE_NO_STDC_NAMESPACE)
   //todo: haven't actually tested this on a non-stdc compliant compiler
   #define CL_NS_STD(func) ::func
#else
   #define CL_NS_STD(func) std::func
#endif
//
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// EXPORTS definition
////////////////////////////////////////////////////////
#ifdef MAKE_CLUCENE_CORE_LIB
	#ifdef CLUCENE_EXPORT_SYMBOLS
		#if defined(_WIN32)
			#define CLUCENE_EXPORT __declspec(dllexport)
		#elif defined(_CL_HAVE_GCCVISIBILITYPATCH)
			#define CLUCENE_EXPORT __attribute__ ((visibility("default")))
			#define CLUCENE_LOCAL __attribute__ ((visibility("hidden")))
		#endif
    #endif
#else
	#if defined(_WIN32) && defined(CLUCENE_IMPORT_SYMBOLS)
		#define CLUCENE_EXPORT __declspec(dllimport)
	#endif
#endif
#ifndef CLUCENE_EXPORT
	#define CLUCENE_EXPORT
#endif
#ifndef CLUCENE_LOCAL
	#define CLUCENE_LOCAL
#endif
////////////////////////////////////////////////////////


#define _CL_CONST_CAST(typ,var) const_cast<typ>(var)
#define _CL_REINTERPRET_CAST(typ,var) reinterpret_cast<typ>(var)

//todo: put this logic in cmake
#if defined(_MSC_VER)
	#if _MSC_FULL_VER >= 140050320
	    #define _CL_DEPRECATE_TEXT(_Text) __declspec(deprecated(_Text))
	#elif _MSC_VER >= 1300
	    #define _CL_DEPRECATE_TEXT(_Text) __declspec(deprecated)
	#else
	    #define _CL_DEPRECATE_TEXT(_Text)
	#endif
#elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
	#define _CL_DEPRECATE_TEXT(_Text)  __attribute__((__deprecated__))
#else
    #define _CL_DEPRECATE_TEXT(_Text)
#endif
#define _CL_DEPRECATED(_NewItem) _CL_DEPRECATE_TEXT("This function or variable has been superceded by newer library or operating system functionality. Consider using " #_NewItem " instead. See online help for details.")


////////////////////////////////////////////////////////
//Class interfaces
////////////////////////////////////////////////////////
#include "CLucene/debug/lucenebase.h"
////////////////////////////////////////////////////////

//error handling macros/functions
#include "CLucene/debug/error.h"

//memory handling macros/functions
#include "CLucene/debug/mem.h"

#endif //lucene_sharedheader_h
