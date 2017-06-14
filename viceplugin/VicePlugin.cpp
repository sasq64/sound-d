
extern "C" {
#include "psid.h"

void psid_play(short *buf, int size);
const char *psid_get_name();
const char *psid_get_author();
const char *psid_get_copyright();
int psid_tunes(int* default_tune);

int c64_init(const char* c64Dir);
void c64_song_init();
void c64_exit();
}

#include <coreutils/utils.h>
#include <coreutils/file.h>
#include <crypto/md5.h>
#include <coreutils/log.h>

#include <set>
#include <algorithm>
#include <unordered_map>

using namespace std;
using namespace utils;

struct __attribute__((packed)) LengthEntry {
	LengthEntry() {}
	LengthEntry(uint64_t h, uint16_t l) : hash(h), length(l) {}
	uint64_t hash;
	uint16_t length;
	//bool operator>(const LengthEntry& other) { return hash > other.hash; }
	bool operator<(const LengthEntry& other) const { return hash < other.hash; }
	bool operator<(uint64_t other) const { return hash < other; }
};

struct STIL {
	int subsong;
	int seconds;
	std::string title;
	std::string name;
	std::string artist;
	std::string author;
	std::string comment;
};

struct STILSong {
	STILSong() {}
	STILSong(const std::vector<STIL> songs, const std::string &c) : songs(songs), comment(c) {}
	std::vector<STIL> songs;
	std::string comment;
};

uint32_t currentLength;
uint32_t currentPos;
uint32_t nextCheckPos;
int currentInfo;
int currentSong;
std::vector<uint16_t> songLengths;

STILSong currentStil;
vector<LengthEntry> mainHash;
vector<uint16_t> extraLengths;
unordered_map<string, STILSong> stilSongs;
std::string dataDir = ".";

constexpr uint16_t a2h(char c) {
	return c <= '9' ? c - '0' : (tolower(c) - 'a' + 10);
}

template <typename T> T from_hex(const std::string &s) {
	T t = 0;
	auto *ptr = s.c_str();
	while(*ptr) {
		t = (t<<4) | a2h(*ptr++);
	}
	return t;
}

void readSTIL() {

	STIL current;
	vector<STIL> songs;
	File f = File(dataDir + "/data/STIL.txt");
	//int subsong = -1;
	string path;
	string what;
	string content;
	string songComment;
	bool currentSet = false;
	//int seconds = 0;
	//int count = 0;
	for(auto l : f.getLines()) {
		//LOGD("'%c' : %s", l[0], l);
		if(l == "" || l[0] == '#')
			continue;
		//if(count++ == 300) break;
		if(l.length() > 4 && l[4] == ' ' && what != "") {
			content = content + " " + lstrip(l);
		} else {
			if(what != "" && content != "") {
				if(songComment == "" && what == "COMMENT" && songs.size() == 0 && current.title == "" && current.name == "") {
					songComment = content;
				} else {
					//LOGD("WHAT:%s = '%s'", what, content);
					if(what == "TITLE")
						current.title = content;
					else if(what == "COMMENT")
						current.comment = content;
					else if(what == "AUTHOR")
						current.author = content;
					else if(what == "ARTIST")
						current.artist = content;
					else if(what == "NAME")
						current.name = content;
					currentSet = true;
				}
				what = "";
				content = "";
			}


			if(l[0] == '/') {
				if(currentSet) {
					songs.push_back(current);
					current = STIL();
					currentSet = false;
					//LOGD("PATH:%s", path);
					//LOGD("========================================");
					//for(auto &s : songs) {
					//	LOGD(" (#%d) T:%s BY:%s A:%s C:%s SEC:%d", s.subsong, s.title, s.artist, s.author, s.comment, s.seconds);
					//}
				}
				//LOGD("Adding '%s'", path);
				stilSongs[path] = STILSong(songs, songComment);
				songComment = "";
				songs.clear();
				path = l;
				current.subsong = 1;
				current.seconds = 0;
				what = "";
				content = "";
			} else if(l[0] == '(') {

				if(currentSet) {
					if(songComment == "" && current.comment != "" && songs.size() == 0 && current.title == "" && current.name == "") {
						songComment = content;
					} else {
						songs.push_back(current);
					}
					current = STIL();
					currentSet = false;
				}
				current.subsong = atoi(l.substr(2).c_str());
				//LOGD("SUBSONG %d", current.subsong);
				current.seconds = 0;
				content = "";
				what = "";
			} else {
				auto colon = l.find(":");
				if(colon != string::npos) {
					what = lstrip(l.substr(0,colon));
					content = l.substr(colon+1);
					if(what == "TITLE") {
						if(currentSet && current.title != "") {
							songs.push_back(current);
							auto s = current.subsong;
							current = STIL();
							current.subsong = s;
							currentSet = false;
						}
						if(content[content.size()-1] == ')') {
							auto pos = content.rfind("(");
							auto secs = split(content.substr(pos+1), ":");
							if(secs.size() >= 2) {
								int m = atoi(secs[0].c_str());
								int s = atoi(secs[1].c_str());
								current.seconds = s + m * 60;
							}					
						}
					}
				}
			}
		}
	}
}

void readLengths() {

	File fp{"data/Songlengths.txt"};
	string secs, mins;
	vector<uint16_t> lengths;
	for(const auto &l : fp.getLines()) {
		//puts(l.c_str());
		if(l[0] != ';' && l[0] != '[') {
			auto key = from_hex<uint64_t>(l.substr(0,16));
			//printf("\nHASH %llx\n", key);
			lengths.clear();
			for(const auto &sl : split(l.substr(33), " ")) {
				tie(mins,secs) = splitn<2>(sl, ":");
				lengths.push_back(stol(mins)*60 + stol(secs));
			}
			int ll = 0;
			if(lengths.size() > 1) {
				ll = extraLengths.size() | 0x8000;
				extraLengths.insert(extraLengths.end(), lengths.begin(), lengths.end());
				extraLengths.back() |= 0x8000;
			} else {
				ll = lengths[0];
			}

			LengthEntry le(key, ll);

			mainHash.insert(upper_bound(mainHash.begin(), mainHash.end(), le), le);

		}
	}

}

vector<uint16_t> findLengths(uint64_t key) {

	vector<uint16_t> songLengths;

	auto it = lower_bound(mainHash.begin(), mainHash.end(), key);
	if(it != mainHash.end()) {
		uint16_t len = it->length;
		LOGD("%llx %llx LEN: %x", key, it->hash, len);
		if((len & 0x8000) != 0) {
			len &= 0x7fff;
			int xl = 0;
			while((xl & 0x8000) == 0) {
				xl = extraLengths[len++];
				songLengths.push_back(xl & 0x7fff);
			}				
		} else
			songLengths.push_back(len);
	}
	return songLengths;
}


template <typename T> const T get(const vector<uint8_t> &v, int offset) {}

template <> const uint16_t get(const vector<uint8_t> &v, int offset) {
	return (v[offset] <<8) | v[offset+1];
}

template <> const uint32_t get(const vector<uint8_t> &v, int offset) {
	return (v[offset] <<24) | (v[offset+1]<<16) | (v[offset+2] <<8) | v[offset+3];
}

template <> const uint64_t get(const vector<uint8_t> &v, int offset) {
	return ((uint64_t)get<uint32_t>(v, offset)<<32) | get<uint32_t>(v, offset+4);
}

enum {
	MAGICID = 0,
	PSID_VERSION = 4,
	DATA_OFFSET = 6,
	LOAD_ADDRESS  = 8,
	INIT_ADDRESS = 0xA,
	PLAY_ADDRESS = 0xC,
	SONGS = 0xE,
	START_SONG = 0x10,
	SPEED = 0x12,
	FLAGS = 0x76
};

static vector<uint8_t> calculateMD5(vector<uint8_t> data) {
	
	uint8_t speed = (data[0] == 'R') ? 60 : 0;
	uint16_t version = get<uint16_t>(data, PSID_VERSION);

	uint16_t initAdr = get<uint16_t>(data, INIT_ADDRESS);
	uint16_t playAdr = get<uint16_t>(data, PLAY_ADDRESS);
	uint16_t songs = get<uint16_t>(data, SONGS);

	uint32_t speedBits = get<uint32_t>(data, SPEED);
	uint16_t flags = get<uint16_t>(data, FLAGS);

	MD5 md5;

	auto offset = (version == 2) ? 126 : 120;

	md5.add(data, offset);

	md5.add(initAdr);
	md5.add(playAdr);
	md5.add(songs);	

	for(int i=0; i<songs; i++) {
		if((speedBits & (1 << i)) != 0) {
			md5.add((uint8_t)60);
		} else {
			md5.add(speed);
		}
	}

	if((flags & 0x8) != 0) {
		md5.add((uint8_t)2);
	}

	return md5.get();
}

enum { TEXT, INT, FLOAT, SPECIAL };

struct meta_data
{
	const char *what;
	void* data;
};

meta_data meta[100];
int meta_count = 0;
vector<string> changedMeta;
unordered_map<string, string> metaData;

void (*meta_callback)(int count, struct meta_data*);

//template <typename... A> void setMeta(A...);

void setMeta() 
{
	if(meta_callback) {
		int i = 0;
		for(auto m : changedMeta) {
			meta[i].what = m.c_str();
			meta[i++].data = (void*)metaData[m].c_str();
		}
		meta_callback(i, meta);
	}
	changedMeta.clear();
	LOGV("Meta Done");
}

template <typename... A> void setMeta(const char* what, const char *value, const A& ...args);
template <typename... A> void setMeta(const char* what, const std::string &value, const A& ...args);

template <typename... A> void setMeta(const char* what, int value, const A& ...args)
{
	metaData[what] = std::to_string(value);
	LOGV("Meta %s=%s", what, metaData[what]);
	changedMeta.push_back(what);
	setMeta(args...);
}

template <typename... A> void setMeta(const char* what, const std::string &value, const A& ...args)
{
	metaData[what] = value;
	LOGV("Meta %s=%s", what, metaData[what]);
	changedMeta.push_back(what);
	setMeta(args...);
}

template <typename... A> void setMeta(const char* what, const char *value, const A& ...args)
{
	metaData[what] = std::string(value);
	LOGV("Meta %s=%s", what, metaData[what]);
	changedMeta.push_back(what);
	setMeta(args...);
}

static const set<string> ext { ".sid", ".psid", ".rsid" , ".2sid", ".mus" };

static bool canHandle(const std::string &name) {
	for(string x : ext) {
		if(utils::endsWith(name, x))
			return true;
	}
	return false;
}

void* error(const char *err)
{
	return NULL;
}

extern "C" void* load(const char* sidFile)
{
	int ret = psid_load_file(sidFile);
	LOGD("Loaded %s -> %d", sidFile, ret);
	if (ret != 0)
		return error("Not a sid file");

	File f { sidFile };
	auto data = f.readAll();
	auto md5 = calculateMD5(data);
	auto key = get<uint64_t>(md5, 0);
	LOGD("MD5: [%02x] %08x", md5, key);
	songLengths = findLengths(key);

	string realPath = sidFile;
	if(realPath.find("C64Music%2f") != string::npos) {
		realPath = utils::urldecode(sidFile, ":/\\?;");
	}

	int defaultSong;
	int songs = psid_tunes(&defaultSong);
	defaultSong--;
	currentSong = defaultSong;
	LOGD("DEFSONG: %d", defaultSong);
	currentLength = 0;
	currentPos = 0;
	nextCheckPos = currentPos + 44100;
	if((int)songLengths.size() > defaultSong) {
		currentLength = songLengths[defaultSong];
	}
	LOGD("Length:%d", currentLength);
	string msg = "NO STIL INFO";
	string sub_title;
	auto pos = realPath.find("C64Music/");
	currentInfo = 0;
	if(pos != string::npos) {
		auto p = realPath.substr(pos+8);
		LOGD("SIDFILE:%s", p);
		if(stilSongs.count(p)) {
			currentStil = stilSongs[p];
			msg = currentStil.comment;

			for(int i=0; i<(int)currentStil.songs.size(); i++) {
				auto &s = currentStil.songs[i];
				LOGD("#%d: %s", s.subsong, s.title);
				if(s.subsong == defaultSong+1) {
					currentInfo = i;
					sub_title = s.title;//sub_title + s.title + " ";
					if(sub_title == "") sub_title = s.name;

					if(msg == "") msg = s.comment;
					break;
				}
			}
		}
	}
	setMeta(
		"title", utf8_encode(psid_get_name()),
		"composer", utf8_encode(psid_get_author()),
		"copyright", utf8_encode(psid_get_copyright()),
		"format", "C64 Sid",
		"songs", songs,
		"message", utf8_encode(msg),
		"sub_title", utf8_encode(sub_title),
		"length", currentLength,
		"startSong", defaultSong
	); 

	c64_song_init();
	return (void*)1;
}

extern "C" void set_meta_callback(void (*fn)(int n, struct meta_data *))
{
	meta_callback = fn;
}

extern "C" bool seekTo(void*, int song, int seconds = -1) {
	if(song >= 0) {
		currentSong = song;
		psid_set_tune(song+1);
		c64_song_init();
		currentLength = 0;
		currentPos = 0;
		if((int)songLengths.size() > song) {
			currentLength = songLengths[song];
		}

		LOGD("Length:%d, SONG %d", currentLength, song);
		string sub_title;
		string msg = currentStil.comment;
		for(int i=0; i<(int)currentStil.songs.size(); i++) {
			auto &s = currentStil.songs[i];
			LOGD("#%d: %s", s.subsong, s.title);
			if(s.subsong == song+1) {
				currentInfo = i;
				sub_title = s.title; //sub_title + s.title + " ";
				if(sub_title == "") sub_title = s.name;
				if(s.comment != "") msg = s.comment;
				break;
			}
		}

		setMeta("length", currentLength, "dsada", "");//, "sub_title", utf8_encode(sub_title), "message", utf8_encode(msg));
		return true;
	}
	return false;
}

extern "C" int getSamples(void*, int16_t *target, int size) {
	currentPos += (size/2);

	if(currentPos > nextCheckPos) {
		int sec = currentPos / 44100;
		nextCheckPos = currentPos + 44100;
		for(int i=currentInfo+1; i<(int)currentStil.songs.size(); i++) {
			auto &s = currentStil.songs[i];
			if(s.subsong == currentSong+1) {
				if(s.seconds > 0 && sec >= s.seconds) {
					LOGD("Found new info");
					currentInfo = i;
					if(s.comment != "")
						setMeta("sub_title", utf8_encode(s.title), "message", utf8_encode(s.comment));
					else
						setMeta("sub_title", utf8_encode(s.title));
					break;
				}
			}


		}
	}

	//LOGD("%d vs %d", currentPos, currentLength*44100);
	//if(currentLength > 0 && currentPos > currentLength*44100)
	//	return -1;
	psid_play(target, size);
	return size;
}

extern "C" void init()
{
	c64_init((dataDir + "/data/c64").c_str());
	readLengths();
	readSTIL();
}
/*
VicePlugin(const unsigned char *data) {
	utils::makedir("c64");

	FILE *fp;
	fp = fopen("c64/basic", "wb");
	fwrite(&data[0], 1, 8192, fp);
	fclose(fp);

	fp = fopen("c64/chargen", "wb");
	fwrite(&data[8192], 1, 4096, fp);
	fclose(fp);

	fp = fopen("c64/kernal", "wb");
	fwrite(&data[8192+4096], 1, 8192, fp);
	fclose(fp);
	VicePlayer::init("c64");

	readLengths();
}
*/

//static File find_file(const std::string &name) {
//	return File::findFile(current_exe_path() + ":" + File::getAppDir(), name);
//}
extern "C" void uninit() {
	LOGD("VicePlugin destroy\n");
	c64_exit();
}
extern "C" int can_handle(const char *name)
{
	return canHandle(name);
}

