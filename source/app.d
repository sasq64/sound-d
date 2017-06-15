import std.stdio;
import deimos.portaudio;
//import hvl_replay;
import std.file;
import std.stdio;
import core.thread;
import std.traits;
import std.string;
import core.sys.posix.dlfcn;
import std.exception : enforce;

pragma(lib, "portaudio");

class audio_exception : Exception {
	this(string msg, string file = __FILE__, size_t line = __LINE__) { super(msg, file, line); }
};


alias toz = std.string.toStringz;


struct Plugin
{
	extern(C) void function() init;
	extern(C) void* function(const char* name) load;
	extern(C) void* function(void* data, int size) load_ptr;
	extern(C) int function(void* song, short* target, int noSamples) getSamples;
}


T DynLoad(T)(string dllname) {
	auto h = dlopen(toz(dllname), RTLD_LAZY | RTLD_LOCAL);
	if(!h)
		throw new Exception("Could not load " ~ dllname);
	T t;
	foreach(N ; FieldNameTuple!T) {
		__traits(getMember, t, N) = cast(typeof(__traits(getMember, T, N)))dlsym(h, toz(N));
	}
	return t;
}





class Audio
{
	this(int delegate(short[]) cb, int sampleRate = 44100)
	{
		audioCb = cb;
		PaError err;
		err = Pa_Initialize();
		if(err != paNoError)
			throw new audio_exception("");
    	err = Pa_OpenDefaultStream(&stream, 0, 2, paInt16, sampleRate, 4096, &callback, cast(void*)this);
		if(err != paNoError)
			throw new audio_exception("");
		err = Pa_StartStream(stream);
		if(err != paNoError)
			throw new audio_exception("");
	}

    PaStream* stream;
	int delegate(short[]) audioCb;

	extern(C) static int callback(const(void)* input, void* output, size_t size,
                             const(PaStreamCallbackTimeInfo)* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData)
	{
		auto audio = cast(Audio)userData;
		short* ptr = cast(short*)output;
		return audio.audioCb(ptr[0..size*2]);
	}

}

struct Fifo(T, int SIZE)
{
	@nogc void put(T[] source)
	{
		if(writePos + source.length - readPos >= SIZE)
			return;
		puts("PUT");

		auto l = source.length;
		auto wp = writePos % SIZE;
		if(wp + l > SIZE) {
			auto left = SIZE - wp;
			data[wp..$] = source[0..left];
			data[0..(l-left)] = source[left..$];
		} else {
			data[wp..wp+l] = source[0..l];
		}
		writePos += l;
	}
	
	@nogc void get(T[] target)
	{
		if(readPos + target.length > writePos)
			return;

		auto l = target.length;
		auto rp = readPos % SIZE;
		if(rp + l > SIZE) {
			auto left = SIZE - rp;
			target[0..left] = data[rp..SIZE];
			target[left..$] = data[0..(l-left)];
		} else {
			target[0..l] = data[rp..rp+l];
		}
		readPos += l;
	}

	ulong canPut() { return readPos - writePos + SIZE; }
	ulong canGet() { return writePos - readPos; }

	ulong writePos = 0;
	ulong readPos = 0;

	T[SIZE] data;

}

unittest {

	Fifo!(short, 16384) f;

	short[] wr = new short [3000];
	short[] re = new short [2000];

	f.put(wr);
	f.get(re);
	assert(f.canGet() == 1000);
	assert(f.canPut() == 4096 - 1000);
	f.put(wr);

}

void main(string[] args)
{
	if(args.length < 2)
		return;

	//auto plugin = DynLoad!Plugin("openmptplugin/build/libopenmptplugin.dylib");
	auto plugin = DynLoad!Plugin("viceplugin/build/libviceplugin.so");

	if(plugin.init)
		plugin.init();

	auto name = toz(args[1]);

	void* tune = void;
	if(plugin.load)	
		tune = plugin.load(name);
	else {
		auto data = read(args[1]);
		auto ptr = cast(void[])data;
		writefln("%d bytes", ptr.length);
		tune = plugin.load_ptr(ptr.ptr, cast(int)ptr.length);
	}

	auto audio = new Audio((short[] target) {
		static Fifo!(short, 16*1024) fifo;
		static short[2048] frame;

		while(fifo.canGet() < target.length) {
			int len = plugin.getSamples(tune, frame.ptr, frame.length);
			fifo.put(frame[0..len]);
		}
		fifo.get(target);

		return 0;
	});

	while(true)
		Thread.sleep(dur!("seconds")(1));

}
