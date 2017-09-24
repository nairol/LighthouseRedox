// Needs a rewrite. It works but is hard to understand. It should also find the starting address automatically.

import std.stdio : writef, writefln, writeln;
import std.file : read, write;
import std.string : fromStringz;
import std.algorithm : sort;

void main( string[] args )
{
	auto buf = cast(ubyte[]) read(args[1]);
	
	auto cparams = buf[0xECF0..$]; //Only works with LHTX firmware 436 (HTC Vive). Change this address for other firmware versions.
	decode( cparams, buf );
}

string[] types = ["void", "bool", "uint8", "uint16", "uint32", "int8", "int16", "int32", "float", "double", "string", "hex", "reserved", "enum"];

uint decode( ubyte[] src, ubyte[] buf )
{
	writeln("EEPROM Offset | RAM Address | Type | Length | Name | Default Value | Description | In EEPROM | Has Default | Handler Address");
	writeln("--------------|-------------|------|--------|------|---------------|-------------|-----------|-------------|----------------");
	writeln("000 | | uint32 | 4 | Partition Signature | 0x4D525043 (\"CPRM\") | Marks the beginning of a CParam partition | x | x | ");
	writeln("004 | | uint32? | 4 | Write Counter? | | [Unconfirmed/Guess] Counts how many times the partition has been overwritten | x | | ");
	writeln("008 | | uint32 | 4 | CRC32 of CParam data | | CRC32 of all following bytes that contain CParam data | x | | ");
	
	char[][][string] enumTypes;
	
	uint count = 0;
	uint offset = 0x0C;
	while( src.length>=32 && src[0] > 0 && src[0]<=13 )
	{
		auto type = types[src[0]];
		auto address = *cast(uint*)src[4..8].ptr;
		auto name = fromStringz( cast(char*)&buf[*cast(uint*)src[8..0xC].ptr] );
		auto defaultValue = fromStringz( cast(char*)&buf[*cast(uint*)src[0xC..0x10].ptr] );
		auto length = src[0x10];
		auto bitfield = src[0x11];
		auto inEEPROM = !(bitfield&1);
		auto hasDefault = !(bitfield&2);
		auto handler = *cast(uint*)src[0x14..0x18].ptr;
		auto description = fromStringz( cast(char*)&buf[*cast(uint*)src[0x18..0x1C].ptr] );
		auto enums = getEnums(buf, buf[(*cast(uint*)src[0x1C..0x20].ptr)..$]);
		
		
		if(inEEPROM) { writef("%.3X", offset); }
		else { writef("   "); }
		
		writef(" | 0x%.8X | %-8s | %.3d | %s | %s | %s | ", address, type, length, name, defaultValue, description);
		writef("%s | %s", inEEPROM?"x":"", hasDefault?"x":"");
		if( handler != 0 ) { writefln(" | 0x%.4X", handler); }
		else { writefln(" | "); }
		
		if(enums.length>0) { enumTypes[name.idup] = enums; }
		
		count++;
		offset += inEEPROM?length:0;
		src = src[32..$];
	}
	
	writefln("\nName | Possible Enum Values\n-----|---------------------");
	foreach( string name; enumTypes.keys.dup.sort() )
	{
		writefln("%s | %(%s, %)", name, enumTypes[name]);
	}
	
	return count;
}

char[][] getEnums( ubyte[] buf, ubyte[] slice )
{
	char[][] result;
	
	if( buf is slice ) return result;
	
	while( true )
	{
		uint current = *cast(uint*)slice[0..4].ptr;
		if( current == 0 ) break;
		result.length = result.length + 1;
		result[$-1] = fromStringz( cast(char*)&buf[current] );
		slice = slice[4..$];
	}
	return result;
}