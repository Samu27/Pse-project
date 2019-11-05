# Simulation script

restart -f

add wave clk
add wave rst
add wave mode
add wave input_ready
add wave output_ready
add wave -radix binary key0
add wave -radix binary key1
add wave -radix binary key2
add wave -radix binary key3
add wave -radix decimal -color purple k
add wave -color purple w0
add wave -color purple w1
add wave -radix decimal -color purple counter
add wave -color purple sum
add wave -color yellow output0
add wave -color yellow output1
add wave status
add wave next_status


# SETUP
force clk 0 0 ns, 1 1 ns -repeat 2
force rst 1 0 ns, 0 5 ns

force key0 2#01101010000111010111100011001000 0 ns
force key1 2#10001100100001101101011001111111 0 ns
force key2 2#00101010011001011011111110111110 0 ns
force key3 2#10110100101111010110111001000110 0 ns


# ENCRYPTION
force mode 0 0 ns
force input_ready 0 0 ns, 1 5 ns, 0 20 ns

force word0 16#12345678 0 ns
force word1 16#9abcdeff 0 ns
run 145 ns


# DECRYPTION
force mode 1 0 ns
force input_ready 1 5 ns, 0 20 ns

force word0 16#99bbb92b 0 ns  
force word1 16#3ebd1644 0 ns
run 145 ns