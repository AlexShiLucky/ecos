#ifndef CYGONCE_DEVS_FLASH_IXDP425_STRATAFLASH_INL
#define CYGONCE_DEVS_FLASH_IXDP425_STRATAFLASH_INL
//==========================================================================
//
//      ixdp425_strataflash.inl
//
//      Flash programming - device constants, etc.
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    gthomas, hmt
// Contributors: gthomas
// Date:         2001-02-24
// Purpose:      
// Description:  
//              
//####DESCRIPTIONEND####
//
//==========================================================================

// The ixdp425 has one 16-bit device.
// StrataFlash 28F128.

#define CYGNUM_FLASH_DEVICES 	(1)
#define CYGNUM_FLASH_BASE_MASK  (0xFF000000u) // 16Mb

#define CYGNUM_FLASH_BASE 	(0x8C000000u)
#define CYGNUM_FLASH_WIDTH 	(16)
#define CYGNUM_FLASH_BLANK      (1)

#include <pkgconf/hal.h>  // for CYGHWR_HAL_ARM_BIGENDIAN

// We have to do some address gymnastics in little-endian mode

// ------------------------------------------------------------------------
// EOF ixdp425_strataflash.inl
