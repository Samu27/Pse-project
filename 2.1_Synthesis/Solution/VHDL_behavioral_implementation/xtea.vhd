library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package xtea_pack is
  constant DELTA     : std_logic_vector (31 downto 0) := X"9E3779B9";
  constant N_CYCLES  : unsigned (7 downto 0)  := "00011111"; 
end xtea_pack;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.xtea_pack.all;
 
entity xtea is
    port (
        clk             : in    bit;
        rst             : in    bit;
        mode            : in    bit;
        input_ready     : in    bit;
        output_ready    : out   bit;
        key0            : in    unsigned (31 downto 0);
        key1            : in    unsigned (31 downto 0);
        key2            : in    unsigned (31 downto 0);
        key3            : in    unsigned (31 downto 0);
        word0           : in    unsigned (31 downto 0);
        word1           : in    unsigned (31 downto 0);
        output0         : out   unsigned (31 downto 0);
        output1         : out   unsigned (31 downto 0));
end xtea;

architecture xtea_arc of xtea is

    type state         is (START, INITIAL, ASSIGN, ENCRYPT_1, ENCRYPT_2, DECRYPT_1, DECRYPT_2, TERM);
    subtype int        is integer range 0 to 65;
    
    signal STATUS      : state;
    signal NEXT_STATUS : state;
    signal k           : unsigned (1 downto 0);
    signal w0          : unsigned (31 downto 0);
    signal w1          : unsigned (31 downto 0);
    signal sum         : unsigned (31 downto 0);
    signal counter     : int := 0;

begin

    -- FSM
    fsm: process (STATUS, input_ready)
    begin
        NEXT_STATUS <= STATUS;

        case STATUS is
            when START =>
                NEXT_STATUS <= INITIAL;
            
            when INITIAL =>
                if input_ready = '1' then
                    NEXT_STATUS <= ASSIGN;
                else
                    NEXT_STATUS <= INITIAL;
                end if;
            
            when ASSIGN =>
                if mode = '0' then
                    NEXT_STATUS <= ENCRYPT_1;
                else
                    NEXT_STATUS <= DECRYPT_1;
                end if;
            
            when ENCRYPT_1 =>
                NEXT_STATUS <= ENCRYPT_2;
            
            when ENCRYPT_2 =>
                if counter = N_CYCLES then
                    NEXT_STATUS <= TERM;
                else
                    NEXT_STATUS <= ENCRYPT_1;
                end if;
            
            when DECRYPT_1 =>
                NEXT_STATUS <= DECRYPT_2;
            
            when DECRYPT_2 =>
                if counter = N_CYCLES then
                    NEXT_STATUS <= TERM;
                else
                    NEXT_STATUS <= DECRYPT_1;
                end if;

            when TERM =>
                NEXT_STATUS <= INITIAL;
            
            when others => 
                NEXT_STATUS <= STATUS;
        end case;
    end process;


    -- DATAPATH
    datapath: process (clk, rst)
    begin
        if rst /= '0' then
            STATUS <= START;
        else 
            if clk'event and clk = '1' then
                STATUS <= NEXT_STATUS;

                case STATUS is
                    when START => 
                        output_ready <= '0';
                        output0 <= (others => '0');
                        output1 <= (others => '0');
                        k <= "00";
                    
                    when INITIAL => 
                        output_ready <= '0';
                        counter <= 0;
                        sum <= (others => '0');
                        w0 <= (others => '0');
                        w1 <= (others => '0');
                        k <= "00";

                    when assign => 
                        w0 <= word0;
                        w1 <= word1;
                        if mode = '1' then 
                            sum <= "11000110111011110011011100100000"; --delta multiply by 32 
                        end if;

                    when ENCRYPT_1 =>
                        case sum(1 downto 0) and "11" is
                            when "00" => w0 <= w0 + ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key0)); 
                            when "01" => w0 <= w0 + ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key1));
                            when "10" => w0 <= w0 + ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key2));
                            when "11" => w0 <= w0 + ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key3));
                            when others => null;
                        end case;
                        sum <= sum + unsigned(delta);

                    when ENCRYPT_2 => 
                        case sum(12 downto 11) and "11" is
                            when "00" => w1 <= w1 + ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key0));
                            when "01" => w1 <= w1 + ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key1));
                            when "10" => w1 <= w1 + ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key2));
                            when "11" => w1 <= w1 + ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key3));
                            when others => null;
                        end case;
                        counter <= counter + 1;

                    when DECRYPT_1 => 
                        case sum(12 downto 11) and "11" is
                            when "00" => w1 <= w1 - ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key0));
                            when "01" => w1 <= w1 - ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key1));
                            when "10" => w1 <= w1 - ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key2));
                            when "11" => w1 <= w1 - ((((w0 sll 4) xor (w0 srl 5)) + w0) xor (sum + key3));
                        when others => null;
                        end case;
                        sum <= sum - unsigned(delta);

                    when DECRYPT_2 => 
                        case sum(1 downto 0) and "11" is
                            when "00" => w0 <= w0 - ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key0));
                            when "01" => w0 <= w0 - ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key1));
                            when "10" => w0 <= w0 - ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key2));
                            when "11" => w0 <= w0 - ((((w1 sll 4) xor (w1 srl 5)) + w1) xor (sum + key3));
                            when others => null;
                        end case;
                        counter <= counter + 1;
                    
                    when TERM => 
                        output0 <= w0;
                        output1 <= w1;
                        output_ready <= '1';
                    
                    when others => 
                        output_ready <= '0';
                end case;
            end if;
        end if;
    end process;
end xtea_arc;