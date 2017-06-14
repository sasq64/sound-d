
import std.process;


	enum  WHAT { TEXT, CTL};
struct AnsiSeq {
   WHAT	what;
   ubyte[] data;

};


struct AnsiParser
{

void put(ubyte[] data)
{
	int csiPos = 0;
	byte[256] csi;

	foreach(i,d ; data) {
		if(d == 0x1b && state == TEXT) {
			state = FOUND_ESC;
		} else if(state == FOUND_ESC && d >= 0x40 && d <= 0x5f) {
			state = CSI;
		} else if(state == CSI && (d >= 0x40 || d <= 0x7e))
			state = TEXT;
		else if(state == CSI)
			csi[c++] = d;
	}
}
/*
	int i = 0;
	if(data[i] == 0x1b && data[i+1] >= 0x40 && data[i+1] <= 0x5f) {
		// CSI
		i+= 2;
		while(data[i] < 0x40 || data[i] > 0x7E)
			i++;
		return AnsiSeq(WHAT.CTL, data[0..i]);
	} else {
		while(data[i] != 0x1b) i++;
		return AnsiSeq(WHAT.TEXT, data[0..i]);
	}
}
*/

class AnsiScreen
{
	struct AnsiChar
	{
		int flags;
		char c;
		alias c this;
	}

	int width;
	int height;

	AnsiChar[] data;

	this(int w, int h)
	{
		width = w;
		height = h;
		data = new AnsiChar[w*h];
	}
}

unittest {
	import std.stdio;
	import std.file;
	auto screen = new AnsiScreen(80, 25);


	writeln("HEY");

	auto pipes = pipeProcess("/usr/local/bin/angband", Redirect.stdout | Redirect.stdin | Redirect.stderr);

	writeln("YOU");
	ubyte[] b = new ubyte[1024];

	auto r = pipes.stdout.rawRead(b);
	writeln("MAN");
	//writeln(r.length);
	for(int i=0; i < r.length; i++)
		writefln("%02x '%c'", r[i], r[i] >= 0x20 && r[i] < 0x7f ? cast(char)r[i] : '#') ;


}
