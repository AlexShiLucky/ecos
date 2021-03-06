//=============================================================================
//
//      m25p05.c
//
//      eCos SPI test.
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2008, 2009 Free Software Foundation, Inc.
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Kolb Alexandr
// Date:        2011-11-25
// Purpose:     Microblaze SPI test
// Description: eCos SPI test.
// Usage:
//
//####DESCRIPTIONEND####
//
//=============================================================================

//=============================================================================
// This is a quick loopback test for the STM32 SPI driver.  It only checks
// the data transfer functionality - chip select handling will require
// testing with external devices.  In order to run the test, the MOSI and
// MISO pins for the test port need to be shorted together to provide an
// external loopback.  Don't do this on a bus which has external devices
// attached unless you first make sure that none of them are connected to
// the chip select used by the test harness.
// The default port and chip select used for this test are SPI bus 1,
// chip select 0.  These can be changed by editing the loopback_device
// data structure directly.
// Note that this is intended to be run as a standalone test and not as part
// of the standard board tests since it requires a hardware modification.
//=============================================================================

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/testcase.h>         // Test macros
#include <cyg/infra/cyg_ass.h>          // Assertion macros
#include <cyg/infra/diag.h>             // Diagnostic output

#include <pkgconf/devs_spi_microblaze.h>            // at91 data structures

#include <cyg/hal/hal_arch.h>           // CYGNUM_HAL_STACK_SIZE_TYPICAL
#include <cyg/kernel/kapi.h>

#include <cyg/io/spi.h>                 // Common SPI API
#include <cyg/io/spi_microblaze.h>            // at91 data structures

#include <string.h>

//---------------------------------------------------------------------------
// Thread data structures.

cyg_uint8 stack [CYGNUM_HAL_STACK_SIZE_TYPICAL];
cyg_thread thread_data;
cyg_handle_t thread_handle;

#ifdef CYGPKG_DEVS_SPI_MICROBLAZE_BUS0
externC cyg_spi_microblaze_bus_t cyg_spi_microblaze_bus0;
#endif

//---------------------------------------------------------------------------
// SPI loopback device driver data structures.

 cyg_spi_microblaze_dev_t loopback_device =
  {
    .spi_device.spi_bus = &cyg_spi_microblaze_bus0.spi_bus,
    .spi_cpol = 1,
    .spi_cpha = 1,
    .spi_lsbf = 1,
 };



//---------------------------------------------------------------------------

char tx_data[] = "Testing, testing, 12, 123.";
char tx_data1[] = "Testing FLASH 25p05 write/read..";

//! Define FLASH commands
#define CMD_WREN 0x06
#define CMD_WRDI 0x04
#define CMD_RDID 0xAB
#define CMD_PP   0x02
#define CMD_RDSR 0x05
#define CMD_RD   0x03
#define CMD_SERR 0xD8
#define CMD_BERR 0xC7

//const char tx_wren[] = {}; // write enable 98
//const char tx_wrdi[] = {0x04}; // write disable 80
//const char tx_[] = {0xc0}; // read cmd + 6 bit addr



char rx_data [sizeof(tx_data)];
char rx_data1 [sizeof(tx_data1)];
//char rx_data2 [sizeof(tx_data2)];

//---------------------------------------------------------------------------

void run_test_1 ()
{
//! Read chip ID command
    tx_data[0] = CMD_RDID;
    tx_data[1] = 0xff;
    tx_data[2] = 0xff;
    tx_data[3] = 0xff;
    tx_data[4] = 0xff;

    cyg_spi_transfer (&loopback_device.spi_device, 0, 5, 
    (const cyg_uint8*) tx_data, (cyg_uint8*) rx_data);
//! Output 4-th byte because 3 first bytes used for command transfer and only 4-th byte contain chip ID
    diag_printf("FLASH ID : 0x%x\n\n", rx_data[4]);

}

//---------------------------------------------------------------------------

void run_test_2 ()
{
    char comm_code;
    diag_printf ("Test 2 : Extended API test.\n");


//!Write enable section

    comm_code = CMD_WREN;

    cyg_spi_transaction_begin (&loopback_device.spi_device);
    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, 1, 
        (const cyg_uint8*) &comm_code, (cyg_uint8*) &rx_data1[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);
//! Use sleep for providing program time tpp
    sleep(1);
//////////////////////////




//! Erase sector section
    diag_printf("Erase sector: address = 0x00\n\n");
    tx_data1[0] = CMD_SERR;
    tx_data1[1] = 0x00;
    tx_data1[2] = 0x00;
    tx_data1[3] = 0x00;

    cyg_spi_transaction_begin (&loopback_device.spi_device);
    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, 4, 
        (const cyg_uint8*) &tx_data1[0], (cyg_uint8*) &rx_data1[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);
//! Use sleep for providing program time tpp
    sleep(1);
/////////////////////



//! Read section
    diag_printf("Reading after erase: address 0x00\n");
    cyg_spi_transaction_begin (&loopback_device.spi_device);
    tx_data1[0] = CMD_RD;
    tx_data1[1] = 0x00;
    tx_data1[2] = 0x00;
    tx_data1[3] = 0x00;
    
    
    tx_data1[4] = 0xff;
    tx_data1[5] = 0xff;
    tx_data1[6] = 0xff;
    tx_data1[7] = 0xff;
    tx_data1[8] = 0xff;
    tx_data1[9] = 0xff;
    tx_data1[10] = 0xff;
    tx_data1[11] = 0xff;

    tx_data1[12] = 0xff;
    tx_data1[13] = 0xff;
    tx_data1[14] = 0xff;
    tx_data1[15] = 0xff;
    tx_data1[16] = 0xff;
    tx_data1[17] = 0xff;
    tx_data1[18] = 0xff;
    tx_data1[19] = 0xff;

    tx_data1[20] = 0xff;
    tx_data1[21] = 0xff;
    tx_data1[22] = 0xff;
    tx_data1[23] = 0xff;
    tx_data1[24] = 0xff;
    tx_data1[25] = 0xff;
    tx_data1[26] = 0xff;
    tx_data1[27] = 0xff;

    tx_data1[28] = 0xff;
    tx_data1[29] = 0xff;
    tx_data1[30] = 0xff;
    tx_data1[31] = 0xff;
    tx_data1[32] = 0xff;
    tx_data1[33] = 0xff;
    tx_data1[34] = 0xff;
    tx_data1[35] = 0xff;

    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, 36, 
        (const cyg_uint8*) &tx_data1[0], (cyg_uint8*) &rx_data1[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);


    diag_printf ("    Rx data 1 : 0x%X, 0x%X,  0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n\n\n",
     rx_data1[4],rx_data1[5],rx_data1[6],rx_data1[7],rx_data1[8],rx_data1[9],rx_data1[10],rx_data1[11]);


///////////////////











//!Write enable section

    comm_code = CMD_WREN;

    cyg_spi_transaction_begin (&loopback_device.spi_device);
    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, 1, 
        (const cyg_uint8*) &comm_code, (cyg_uint8*) &rx_data1[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);
//! Use sleep for providing program time tpp
    sleep(1);
//////////////////////////




//! Program page section
    diag_printf("Program page : address 0x00\n");
    char tx_data2[] = "    Test FLASH 25p05 write/read.";
    char rx_data2[sizeof(tx_data2)];
    diag_printf ("    Tx data : %s\n", &tx_data2[4]);
    tx_data2[0] = CMD_PP;
    tx_data2[1] = 0x00;
    tx_data2[2] = 0x00;
    tx_data2[3] = 0x00;

    cyg_spi_transaction_begin (&loopback_device.spi_device);
    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, sizeof(tx_data2), 
        (const cyg_uint8*) &tx_data2[0], (cyg_uint8*) &rx_data2[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);
//! Use sleep for providing program time tpp
    sleep(1);
///////////////////////////////



//! Read section
    cyg_spi_transaction_begin (&loopback_device.spi_device);
    tx_data1[0] = CMD_RD;
    tx_data1[1] = 0x00;
    tx_data1[2] = 0x00;
    tx_data1[3] = 0x00;
    
    
    tx_data1[4] = 0xff;
    tx_data1[5] = 0xff;
    tx_data1[6] = 0xff;
    tx_data1[7] = 0xff;
    tx_data1[8] = 0xff;
    tx_data1[9] = 0xff;
    tx_data1[10] = 0xff;
    tx_data1[11] = 0xff;

    tx_data1[12] = 0xff;
    tx_data1[13] = 0xff;
    tx_data1[14] = 0xff;
    tx_data1[15] = 0xff;
    tx_data1[16] = 0xff;
    tx_data1[17] = 0xff;
    tx_data1[18] = 0xff;
    tx_data1[19] = 0xff;

    tx_data1[20] = 0xff;
    tx_data1[21] = 0xff;
    tx_data1[22] = 0xff;
    tx_data1[23] = 0xff;
    tx_data1[24] = 0xff;
    tx_data1[25] = 0xff;
    tx_data1[26] = 0xff;
    tx_data1[27] = 0xff;

    tx_data1[28] = 0xff;
    tx_data1[29] = 0xff;
    tx_data1[30] = 0xff;
    tx_data1[31] = 0xff;
    tx_data1[32] = 0xff;
    tx_data1[33] = 0xff;
    tx_data1[34] = 0xff;
    tx_data1[35] = 0xff;

    cyg_spi_transaction_transfer (&loopback_device.spi_device, true, 36, 
        (const cyg_uint8*) &tx_data1[0], (cyg_uint8*) &rx_data1[0], true);
    cyg_spi_transaction_end (&loopback_device.spi_device);





    diag_printf ("    Rx data : %s\n",&rx_data1[4]);

///////////////////


}

//---------------------------------------------------------------------------

void run_tests (void)
{
    diag_printf ("Running Microblaze SPI driver tests.\n");
    run_test_1 ();
    run_test_2 (); 
    CYG_TEST_PASS_FINISH ("SPI driver tests OK");
}

//---------------------------------------------------------------------------
//! User startup - tests are run in their own thread.

void cyg_user_start(void)
{
    CYG_TEST_INIT();
#ifdef CYGPKG_DEVS_SPI_MICROBLAZE_BUS0
    cyg_thread_create(
        10,                                   // Arbitrary priority
        (cyg_thread_entry_t*) run_tests,      // Thread entry point
        0,                                    // 
        "test_thread",                        // Thread name
        &stack[0],                            // Stack
        CYGNUM_HAL_STACK_SIZE_TYPICAL,        // Stack size
        &thread_handle,                       // Thread handle
        &thread_data                          // Thread data structure
    );
    cyg_thread_resume(thread_handle);
    cyg_scheduler_start();
#else
    diag_printf("ERROR: There are no any SPI buses.... \n");
#endif
}

//=============================================================================
