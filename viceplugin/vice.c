#include "archdep.h"
#include "drive.h"
#include "gfxoutput.h"
#include "init.h"
#include "initcmdline.h"
#include "lib.h"
#include "machine.h"
#include "maincpu.h"
#include "psid.h"
#include "resources.h"
#include "sound.h"
#include "sysfile.h"
#include "sid/sid.h"

void psid_play(short *buf, int size);
const char *psid_get_name();
const char *psid_get_author();
const char *psid_get_copyright();
int psid_tunes(int* default_tune);

int console_mode = 1;
int vsid_mode = 1;
int video_disabled_mode = 1;

#define false 0
#define true 1

static int videomode_is_ntsc = false;
static int videomode_is_forced = false;
static int sid = 3;//SID_MODEL_DEFAULT;// SID_MODEL_8580;
static int sid_is_forced = true;

int c64_init(const char* c64Dir) {
	maincpu_early_init();
	machine_setup_context();
	drive_setup_context();
	machine_early_init();
	sysfile_init("C64");
	gfxoutput_early_init();
	if(init_resources() < 0) {
		archdep_startup_log_error("Failed to init resources");
		return false;
	}

	if(resources_set_defaults() < 0) {
		archdep_startup_log_error("Cannot set defaults.\n");
		return false;
	}

	resources_set_int("SidResidSampling", 0);
	resources_set_int("VICIIVideoCache", 0);
	resources_set_string("Directory", c64Dir);
	if(init_main() < 0) {
		archdep_startup_log_error("Failed to init main");
		return false;
	}

	return true;
}

void c64_exit()
{
	machine_shutdown();
}

void c64_song_init()
{
	/* Set default, potentially overridden by reset. */
	resources_set_int("MachineVideoStandard", videomode_is_ntsc ? MACHINE_SYNC_NTSC : MACHINE_SYNC_PAL);
	
	/* Default to 6581 in case tune doesn't specify. */
	resources_set_int("SidModel", sid);

	/* Reset C64, which also initializes PSID for us. */
	machine_trigger_reset(MACHINE_RESET_MODE_SOFT);

	/* Now force video mode if we are asked to. */
	if (videomode_is_forced)
	{
		resources_set_int("MachineVideoStandard", videomode_is_ntsc ? MACHINE_SYNC_NTSC : MACHINE_SYNC_PAL);
	}
	
	/* Force the SID model if told to in the settings */
	if (sid_is_forced)
	{
		resources_set_int("SidModel", sid);
	}

}


