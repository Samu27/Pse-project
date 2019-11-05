//----------------------------------------------------------
//--
//-- Author: Stefano Centomo
//--
//-- Date:16/09/2017
//-- Controller Software, runned on a m6502 cpu
//--
//----------------------------------------------------------
#include "stdio.h"
#include <stdint.h>

//---------------------------------------
//---------------------------------------
//
//	M6502 
//	SUPPORTED TYPES & DIMENSIONS
//
//  int                 =>	16 bits	
//  char                =>	8 bits
//  uint8_t/int8_t      =>	8 bits
//  uint16_t/int16_t	=>	16 bits
//  uint32_t/int32_t	=>	32 bits
//  float/double        => not supported
//	
//---------------------------------------
//---------------------------------------

//------------------------------
//	BOOLEAN TYPE DEFINITION
//------------------------------

typedef char bool; // it use only a byte. it can be optimizable using bitwise and masks
#define true 1
#define false 0

//----------------------------------------------------------
//
// RAM VARIABLES ADDRESS (STACK) 
//
//----------------------------------------------------------
//
//
//   |-----------------------------------------------------|
//   |                    |        |                       |
//   |     VARIABLES      |  SIZE  |        ADDRESSES      |
//   |                    |  bits  |                       |
//   |-----------------------------------------------------|
//   |                    |        |                       |
//   |     accelerometer  |    16  |     0x1ffb - 0x1ffa   |	<<--- FIRST VARIABLE IN THE STACK. FIRST DECLARED IN main().
//   |     data           |    16  |     0x1ff9 - 0x1ff8   |	
//   |     result         |    16  |     0x1ff7 - 0x1ff6   |	
//   |     data_rdy       |    8   |     0x1ff5            |
//   |     result_rdy     |    8   |     0x1ff4            |
//   |-----------------------------------------------------|
//


//--------------------------
// Main Routine
//--------------------------

int main(){

	int 	accelerometer;
	int 	data;
	int 	result;
	uint8_t data_rdy;
	uint8_t result_rdy;


	result_rdy= false;
	
	while(1){
			
		data = accelerometer;
		data_rdy = true;

		while(!result_rdy);		// blocked until new values from the Gain

		data_rdy 	= false;		// disabled by the memory
		result_rdy 	= false;

			
	}	

	
	return 0;
	
}
