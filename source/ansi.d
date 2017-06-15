
import std.process;
import std.stdio;
import std.file;

struct AnsiParser
{
    enum
    {
        TEXT,
        ESC,
        CSI
    };

    int pos = 0;
    ubyte[256 * 16] buf;
    int state = TEXT;

    struct Seq
    {
        this(string s)
        {
            isText = true;
            data = cast(ubyte[])s.dup;
        }

        this(ubyte[] s)
        {
            isText = false;
            data = s.dup;
        }

        string text()
        {
            return cast(string)data;
        }


        bool isText;
        ubyte[] data;
    }

    Seq[] seq;

    bool empty()
    {
        return seq.length == 0;
    }

    void popFront()
    {
        seq = seq[1 .. $];
    }

    Seq front()
    {
        return seq[0];
    }

    void put(ubyte[] data)
    {

        foreach (i, d; data)
        {
            buf[pos++] = d;
 
            if (d == 0x1b && state == TEXT)
            {
                seq ~= Seq(cast(string) buf[0 .. pos-1]);
                buf[0] = d;
                pos = 1;
                state = ESC;
            }
            else if (state == ESC)
            {
                if((d >= 0x40 && d <= 0x5f) || d == '(' || d == ')')
                {
                    state = CSI;
                } else {
                    seq ~= Seq(buf[0..pos]);
                    pos = 0;
                    state = TEXT;
                }
            }
            else if (state == CSI && (d >= 0x40 && d <= 0x7e))
            {
                seq ~= Seq(buf[0 .. pos]);
                pos = 0;
                state = TEXT;
            }
            else if (state == CSI)
            {
            }
            else if (state == TEXT)
            {
            }
        }
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
        //auto r = pipes.stdout.rawRead(b);
import core.sys.posix.termios;
extern(C) void cfmakeraw(termios *termios_p);

void setRaw(File f)
{
    termios  ostate;
    termios  nstate;

    tcgetattr(f.fileno, &nstate);
    cfmakeraw(&nstate);
    tcsetattr(f.fileno, TCSADRAIN, &nstate);

    //

//    tcsetattr(f.fileno, TCSADRAIN, &ostate);       // return to original mode
}

unittest {
	auto screen = new AnsiScreen(80, 25);

    import core.sys.posix.fcntl;
    import std.c.stdio;
	writeln("HEY");

	auto pipes = pipeProcess("/usr/local/bin/angband", Redirect.stdout | Redirect.stdin | Redirect.stderr);

    //setRaw(pipes.stdin);
    setvbuf(pipes.stdin.getFP(), null, _IONBF, 0U);

    int fd = pipes.stdout.fileno;
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    ubyte[] lf = [ 10, 13, 10, 13 ];
    int counter = 0;

	writeln("YOU");
	ubyte[] b = new ubyte[32];

    AnsiParser parser;

    while (true)
    {
        auto rc = fread(b.ptr, 1, b.length, pipes.stdout.getFP());
        if (rc > 0)
        {
            ubyte[] r = b[0 .. rc];
            parser.put(r);
            foreach (seq; parser)
            {
                //writefln("%s %d", seq.isText ? "TXT" : "ANS", seq.data.length);
                auto t = seq.data;
                if(seq.isText)
                    writeln(seq.text);
                else 
                    writeln("ANSI:" ~  cast(string)seq.data[1..$]);
               //for (int i = 0; i < t.length; i++)
                 //   writefln("%02x '%c'", t[i], t[i] >= 0x20 && t[i] < 0x7f ? cast(char) t[i] : '#');
            }
        }
    
        //writeln(pipes.stdout.eof);
    
        if (rc == 0)
        {
            counter++;
            if (counter == 10)
                pipes.stdin.rawWrite(lf);
        }
    }

}
