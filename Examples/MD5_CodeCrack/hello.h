

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Feb 12 21:27:21 2011
 */
/* Compiler settings for hello.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, oldnames, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __hello_h__
#define __hello_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __hello_INTERFACE_DEFINED__
#define __hello_INTERFACE_DEFINED__

/* interface hello */
/* [implicit_handle][unique][version][uuid] */ 

void GetPermutationRange( 
    /* [in] */ handle_t h1,
    /* [out] */ unsigned long start[ 1 ],
    /* [out] */ unsigned long end[ 1 ]);

void GetHash( 
    /* [in] */ handle_t h1,
    /* [string][out] */ unsigned char breakHash[ 33 ]);

void HashDecoded( 
    /* [in] */ handle_t h1,
    /* [string][in] */ unsigned char *decodedHash);


extern handle_t hello_IfHandle;


extern RPC_IF_HANDLE hello_ClientIfHandle;
extern RPC_IF_HANDLE hello_ServerIfHandle;
#endif /* __hello_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

