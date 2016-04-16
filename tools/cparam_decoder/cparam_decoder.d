// Needs a rewrite. It works but is hard to understand. It should also find the starting address automatically.

import std.stdio : writef, writefln, writeln;
import std.file : read, write;
import std.string : fromStringz;

void main( string[] args )
{
	auto buf = cast(ubyte[]) read(args[1]);
	
	auto cparams = buf[0xE52C..$]; //Only works with LHTX firmware 244 (HTC Vive). Change this address for other firmware versions.
	decode( cparams, buf );
}

string[] types = ["void", "bool", "uint8", "uint16", "uint32", "int8", "int16", "int32", "float", "double", "string", "hex", "reserved", "enum"];

uint decode( ubyte[] src, ubyte[] buf )
{
	writeln("EEPROM Offset | Address | Type | Length | Name | Default Value | Description | Writable | Has Default | Handler Address");
	writeln("--------------|---------|------|--------|------|---------------|-------------|----------|-------------|----------------");
	
	char[][][string] enumTypes;
	
	uint count = 0;
	uint offset = 0;
	while( src.length>=32 && src[0]>0 && src[0]<=13 )
	{
		auto type = types[src[0]];
		auto address = *cast(uint*)src[4..8].ptr;
		auto name = fromStringz( cast(char*)&buf[*cast(uint*)src[8..0xC].ptr] );
		auto defaultValue = fromStringz( cast(char*)&buf[*cast(uint*)src[0xC..0x10].ptr] );
		auto length = src[0x10];
		auto bitfield = src[0x11];
		auto handler = *cast(uint*)src[0x14..0x18].ptr;
		auto description = fromStringz( cast(char*)&buf[*cast(uint*)src[0x18..0x1C].ptr] );
		auto enums = getEnums(buf, buf[(*cast(uint*)src[0x1C..0x20].ptr)..$]);
		writef("%.3X | 0x%.8X | %-8s | %.3d | %s | %s | %s | ", offset, address, type, length, name, defaultValue, description);
		writefln("%s | %s | 0x%.4X", bitfield&1?"":"x", bitfield&2?"":"x", handler);
		if(enums.length>0) enumTypes[name.idup] = enums;
		
		count++;
		offset += length;
		src = src[32..$];
	}
	
	writefln("\nName | Possible Enum Values\n-----|---------------------");
	foreach( string name, char[][] enumStr; enumTypes )
	{
		writefln("%s | %(%s, %)", name, enumStr);
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