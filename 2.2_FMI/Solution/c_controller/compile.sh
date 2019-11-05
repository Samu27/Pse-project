
cl65 --verbose  -t sim6502 --cpu 6502 --asm-include-dir $CC65_HOME/asminc -C $CC65_HOME/mycfg.cfg  -I $CC65_HOME/include -L $CC65_HOME/lib -l listing.lst -m $CC65_HOME/map -T $1
string=$1
filename="${string%.*}"
xxd -g 1 $filename | cut -c11-57 > $2 
sed -i '513d' $2
sed -i '512c\ff ff ff ff ff ff ff ff ff ff 01 e0 00 00 00 00' $2
