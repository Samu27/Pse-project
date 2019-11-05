//-----------------------------------------------------------------
//Design: Memory for m6502 with Accelerometer and Gain Interface
//
//Author: Stefano Centomo
//
//Date: 16/09/2017
//
//-----------------------------------------------------------------

`timescale 1ns / 100ps

`define  RAM_SIZE  32768
`define  ROM_SIZE  8192

module memory 	(			
						clk,
						rst,
						addr,
						datai,
						datao,
						irq_n,
						nmi_n,
						sob_n,
						res_n,
						rdy,
						vpab,
						sync,
						we_n,
						oeb,
						accelerometer,
						data,
						data_rdy,
						result,
						result_rdy
				);

	// ------------------------------------------------------------------------
	// CPU INTERFACE
	// ------------------------------------------------------------------------
	
	input   			clk;
	input				rst;
	input  		[15:0] 	addr; 	// address bus
	output 		[7:0]	datai; 	// data bus
	input 		[7:0] 	datao; 	// data bus
	input 				oeb; 	// data bus negtive enable
	output reg  		irq_n; 	// interrupt request
	output reg  		nmi_n; 	// Non-makable interrupt request
	output reg  		res_n; 	// reset
	output reg  		rdy; 	// ready
	output reg  		sob_n; 	// Set Overflow bit
	input  				sync; 	// indicates an opcode fetch
	input  				vpab; 	// Vector Pull Address
	input  				we_n; 	// 1=read, 0=write

	// -----------------------------------------------------------------------
	// ACCELEROMETER INTERFACE
	// -----------------------------------------------------------------------

	input  		[15:0] 	accelerometer; 		// data from the accelerometer
	
	// -----------------------------------------------------------------------
	// MULTIPLIER INTERFACE
	// -----------------------------------------------------------------------

	output reg  [15:0] 	data; 				// data to the multiplier
	output reg  	 	data_rdy; 			// data flag to multiplier
	input  		[15:0] 	result; 			// result from the multiplier
	input  	 			result_rdy; 		// result flag from the multiplier
	
	// ------------------------------------------------------------------------
	//  MEMORY MAPPED IO
	// ------------------------------------------------------------------------

	localparam ACCELEROMETER_HIGH_ADDRESS		= 32'h00001FFB;
	localparam ACCELEROMETER_LOW_ADDRESS		= 32'h00001FFA;
	localparam DATA_HIGH_ADDRESS				= 32'h00001FF9;
	localparam DATA_LOW_ADDRESS					= 32'h00001FF8;
	localparam RESULT_HIGH_ADDRESS				= 32'h00001FF7;
	localparam RESULT_LOW_ADDRESS				= 32'h00001FF6;
	localparam DATA_RDY_ADDRESS					= 32'h00001FF5;
	localparam RESULT_RDY_ADDRESS				= 32'h00001FF4;


	// ------------------------------------------------------------------------
	//  STATES
	// ------------------------------------------------------------------------

	reg [7:0] state;

	localparam ST_START = 8'd0;
	localparam ST_1  	= 8'd1;
	localparam ST_2  	= 8'd2;
	localparam ST_3  	= 8'd3;
	localparam ST_4  	= 8'd4;
	localparam ST_5  	= 8'd5;
	localparam ST_6  	= 8'd6;
	localparam ST_7  	= 8'd7;
	localparam ST_8  	= 8'd8;
	localparam ST_9  	= 8'd9;
	localparam ST_10 	= 8'd10;
	localparam ST_11 	= 8'd11;
	localparam ST_12 	= 8'd12;
	localparam ST_13 	= 8'd13;
	localparam ST_14 	= 8'd14;
	localparam ST_15 	= 8'd15;

	// ------------------------------------------------------------------------
	// VARIABLES
	// ------------------------------------------------------------------------
	
	reg [7:0]	ram[(`RAM_SIZE-1):0];
	reg [7:0]	rom[(`ROM_SIZE-1):0];
	reg 		memory_addr_switch;
	reg [12:0]	memory_addr;

	initial begin
		$readmemh("./roms/rom.mem", rom);

		state 		= ST_START;
		datai		<= 8'h00;
		irq_n		<= 1'b0;
		nmi_n		<= 1'b0;
		res_n		<= 1'b0;
		rdy			<= 1'b0;
		sob_n		<= 1'b0;

		data_rdy 	<= 1'b0;
		data 	 	<= 16'h0000;
	
	end

	// DATA PATH
	always @(state) begin
		// --------------------------------------------------------------------
		//		MEMORY DATA PATH
		// --------------------------------------------------------------------
		case( state )
			
			ST_START : begin
				datai		<= 8'h00;
				irq_n		<= 1'b0;
				nmi_n		<= 1'b0;
				res_n		<= 1'b0;
				rdy			<= 1'b0;
				sob_n		<= 1'b0;
				data_rdy 	<= 1'b0;
				data 	 	<= 16'h0000;
			end

			ST_1: begin
				rdy 	<= 1'b1;
				sob_n	<= 1'b1;
				irq_n 	<= 1'b0;
				nmi_n 	<= 1'b0;
				res_n 	<= 1'b0;
			end
			
			ST_2: begin
				res_n <= 1'b0;
			end
			
			ST_3: begin
				// EMPTY
			end
			
			ST_4: begin
				// EMPTY
			end
			
			ST_5: begin
				nmi_n <= 1'b1;
			end
			
			ST_6: begin
				// EMPTY
			end
			
			ST_7: begin
				// EMPTY
			end
			
			ST_8: begin
				// EMPTY
			end
			
			ST_9: begin
				// EMPTY
			end
			
			ST_10: begin
				// EMPTY
			end
			
			ST_11: begin
				nmi_n <= 1'b0;
			end
			
			ST_12: begin
				// EMPTY
			end
			
			ST_13: begin
				res_n <= 1'b1;
			end
			
			ST_14: begin
				// EMPTY
			end
			
			ST_15: begin
				// EMPTY
			end

		endcase

	end


	always @ ( clk ) begin
		// POSEDGE RESET!
		if ( rst==1'b1 ) begin
			state = ST_START;
		end
		else begin
			case( state )
				ST_START: begin
					state = ST_1;
				end
				ST_1: begin
					state = ST_2;
				end
				ST_2: begin
					state = ST_3;
				end
				ST_3: begin
					state = ST_4;
				end
				ST_4: begin
					state = ST_5;
				end
				ST_5: begin
					state = ST_6;
				end
				ST_6: begin
					state = ST_7;
				end
				ST_7: begin
					state = ST_8;
				end
				ST_8: begin
					state = ST_9;
				end
				ST_9: begin
					state = ST_10;
				end
				ST_10: begin
					state = ST_11;
				end
				ST_11: begin
					state = ST_12;
				end
				ST_12: begin
					state = ST_13;
				end
				ST_13: begin
					state = ST_14;
				end
				ST_14: begin
					state = ST_15;
				end
				ST_15: begin
					state = ST_14;
				end
			endcase
		end
	end


	// ram process
	always @(  clk or addr ) begin
		memory_addr			= addr[12:0];
		memory_addr_switch	= addr[15];
		if (		~memory_addr_switch & ~we_n ) begin
			ram[memory_addr] = datao;
		end
		else if (	~memory_addr_switch &  we_n ) begin
			datai <= ram[memory_addr];
		end
		else if (	 memory_addr_switch &  we_n ) begin
			datai <= rom[memory_addr];
		end
	end

		// Update values between Memory and C code
	always @(clk) begin 
		
		ram[ACCELEROMETER_HIGH_ADDRESS] = accelerometer[15:8];
		ram[ACCELEROMETER_LOW_ADDRESS] 	= accelerometer[7:0];
		
		data_rdy 		<= ram[DATA_RDY_ADDRESS];	
		data[7:0] 		<= ram[DATA_LOW_ADDRESS];
		data[15:8] 		<= ram[DATA_HIGH_ADDRESS];

		ram[RESULT_RDY_ADDRESS]		= result_rdy;
		ram[RESULT_HIGH_ADDRESS] 	= result[15:8];
		ram[RESULT_LOW_ADDRESS] 	= result[7:0];


	end

	

endmodule
