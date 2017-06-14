#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libopenmpt/libopenmpt.h"
#include "libopenmpt/libopenmpt_stream_callbacks_file.h"

void* error(const char *code)
{
	return NULL;
}

extern "C" void* load_ptr(void *data, int size) {

	uint8_t *ptr = (uint8_t*)data;
	if(memcmp(ptr + 1080, "FLT", 3) == 0 ||
	   memcmp(ptr + 1080, "EXO", 3) == 0)
		return error("Can not play Startrekker module");

	openmpt_module* mod = openmpt_module_create_from_memory(ptr, size, NULL, NULL, NULL);

	if(!mod)
		 return error("Could not load module");

//if(loopmode)
	openmpt_module_set_repeat_count(mod, 99);
/*
	double length = openmpt_module_get_duration_seconds(mod);
	int songs = openmpt_module_get_num_subsongs(mod);

	const char* keys = openmpt_module_get_metadata_keys(mod);

	const char* title = openmpt_module_get_metadata(mod, "title");
	const char* artist = openmpt_module_get_metadata(mod, "artist");
	const char* tracker = openmpt_module_get_metadata(mod, "tracker");
	const char* message = openmpt_module_get_metadata(mod, "message");
	const char* type_long = openmpt_module_get_metadata(mod, "type_long");
*/
	const char* type = openmpt_module_get_metadata(mod, "type");
	if(strcmp(type, "mod") == 0)
		openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, 1);
	else
		openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, 0);

	double separation = 100.0;
	openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT, separation);

	return (void*)mod;
}

extern "C" void unload(openmpt_module* mod) {
		openmpt_module_destroy(mod);
}

extern "C" int getSamples(openmpt_module* mod, int16_t *target, int noSamples) {
	int len = openmpt_module_read_interleaved_stereo(mod, 44100, noSamples/2, target);
	return len*2;
}

extern "C" int seekTo(openmpt_module* mod, int song, int seconds) {
	if(mod) {
		if(song >= 0)
			openmpt_module_select_subsong(mod, song);
		else
			openmpt_module_set_position_seconds(mod, seconds);
		return true;
	}
	return false;
}

