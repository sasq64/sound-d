/* Converted to D from hvl_replay.h by htod */
module hvl_replay;

//C     typedef signed char int8;
extern (C):
alias byte int8;
//C     typedef unsigned char uint8;
alias ubyte uint8;
//C     typedef short int16;
alias short int16;
//C     typedef unsigned short uint16;
alias ushort uint16;
//C     typedef int int32;
alias int int32;
//C     typedef unsigned int uint32;
alias uint uint32;
//C     typedef double float64;
alias double float64;
//C     typedef char TEXT;
alias char TEXT;
//C     typedef int BOOL;
alias int BOOL;

//C     #define TRUE 1
//C     #define FALSE 0
const TRUE = 1;

const FALSE = 0;
// Woohoo!
//C     #define MAX_CHANNELS 16

const MAX_CHANNELS = 16;
//C     #define Period2Freq(period) ((3546897.f * 65536.f) / (period)) 

//C     struct hvl_envelope
//C     {
//C       int16 aFrames, aVolume;
//C       int16 dFrames, dVolume;
//C       int16 sFrames;
//C       int16 rFrames, rVolume;
//C       int16 pad;
//C     };
struct hvl_envelope
{
    int16 aFrames;
    int16 aVolume;
    int16 dFrames;
    int16 dVolume;
    int16 sFrames;
    int16 rFrames;
    int16 rVolume;
    int16 pad;
}

//C     struct hvl_plsentry
//C     {
//C       uint8 ple_Note;
//C       uint8 ple_Waveform;
//C       int16 ple_Fixed;
//C       int8  ple_FX[2];
//C       int8  ple_FXParam[2];
//C     };
struct hvl_plsentry
{
    uint8 ple_Note;
    uint8 ple_Waveform;
    int16 ple_Fixed;
    int8 [2]ple_FX;
    int8 [2]ple_FXParam;
}

//C     struct hvl_plist
//C     {
//C       int16                pls_Speed;
//C       int16                pls_Length;
//C       struct hvl_plsentry *pls_Entries;
//C     };
struct hvl_plist
{
    int16 pls_Speed;
    int16 pls_Length;
    hvl_plsentry *pls_Entries;
}

//C     struct hvl_instrument
//C     {
//C       TEXT                ins_Name[128];
//C       uint8               ins_Volume;
//C       uint8               ins_WaveLength;
//C       uint8               ins_FilterLowerLimit;
//C       uint8               ins_FilterUpperLimit;
//C       uint8               ins_FilterSpeed;
//C       uint8               ins_SquareLowerLimit;
//C       uint8               ins_SquareUpperLimit;
//C       uint8               ins_SquareSpeed;
//C       uint8               ins_VibratoDelay;
//C       uint8               ins_VibratoSpeed;
//C       uint8               ins_VibratoDepth;
//C       uint8               ins_HardCutRelease;
//C       uint8               ins_HardCutReleaseFrames;
//C       struct hvl_envelope ins_Envelope;
//C       struct hvl_plist    ins_PList;
//C     };
struct hvl_instrument
{
    TEXT [128]ins_Name;
    uint8 ins_Volume;
    uint8 ins_WaveLength;
    uint8 ins_FilterLowerLimit;
    uint8 ins_FilterUpperLimit;
    uint8 ins_FilterSpeed;
    uint8 ins_SquareLowerLimit;
    uint8 ins_SquareUpperLimit;
    uint8 ins_SquareSpeed;
    uint8 ins_VibratoDelay;
    uint8 ins_VibratoSpeed;
    uint8 ins_VibratoDepth;
    uint8 ins_HardCutRelease;
    uint8 ins_HardCutReleaseFrames;
    hvl_envelope ins_Envelope;
    hvl_plist ins_PList;
}

//C     struct hvl_position
//C     {
//C       uint8 pos_Track[MAX_CHANNELS];
//C       int8  pos_Transpose[MAX_CHANNELS];
//C     };
struct hvl_position
{
    uint8 [16]pos_Track;
    int8 [16]pos_Transpose;
}

//C     struct hvl_step
//C     {
//C       uint8 stp_Note;
//C       uint8 stp_Instrument;
//C       uint8 stp_FX;
//C       uint8 stp_FXParam;
//C       uint8 stp_FXb;
//C       uint8 stp_FXbParam;
//C     };
struct hvl_step
{
    uint8 stp_Note;
    uint8 stp_Instrument;
    uint8 stp_FX;
    uint8 stp_FXParam;
    uint8 stp_FXb;
    uint8 stp_FXbParam;
}

//C     struct hvl_voice
//C     {
//C       int16                  vc_Track;
//C       int16                  vc_NextTrack;
//C       int16                  vc_Transpose;
//C       int16                  vc_NextTranspose;
//C       int16                  vc_OverrideTranspose; // 1.5
//C       int32                  vc_ADSRVolume;
//C       struct hvl_envelope    vc_ADSR;
//C       struct hvl_instrument *vc_Instrument;
//C       uint32                 vc_SamplePos;
//C       uint32                 vc_Delta;
//C       uint16                 vc_InstrPeriod;
//C       uint16                 vc_TrackPeriod;
//C       uint16                 vc_VibratoPeriod;
//C       uint16                 vc_WaveLength;
//C       int16                  vc_NoteMaxVolume;
//C       uint16                 vc_PerfSubVolume;
//C       uint8                  vc_NewWaveform;
//C       uint8                  vc_Waveform;
//C       uint8                  vc_PlantPeriod;
//C       uint8                  vc_VoiceVolume;
//C       uint8                  vc_PlantSquare;
//C       uint8                  vc_IgnoreSquare;
//C       uint8                  vc_FixedNote;
//C       int16                  vc_VolumeSlideUp;
//C       int16                  vc_VolumeSlideDown;
//C       int16                  vc_HardCut;
//C       uint8                  vc_HardCutRelease;
//C       int16                  vc_HardCutReleaseF;
//C       uint8                  vc_PeriodSlideOn;
//C       int16                  vc_PeriodSlideSpeed;
//C       int16                  vc_PeriodSlidePeriod;
//C       int16                  vc_PeriodSlideLimit;
//C       int16                  vc_PeriodSlideWithLimit;
//C       int16                  vc_PeriodPerfSlideSpeed;
//C       int16                  vc_PeriodPerfSlidePeriod;
//C       uint8                  vc_PeriodPerfSlideOn;
//C       int16                  vc_VibratoDelay;
//C       int16                  vc_VibratoSpeed;
//C       int16                  vc_VibratoCurrent;
//C       int16                  vc_VibratoDepth;
//C       int16                  vc_SquareOn;
//C       int16                  vc_SquareInit;
//C       int16                  vc_SquareWait;
//C       int16                  vc_SquareLowerLimit;
//C       int16                  vc_SquareUpperLimit;
//C       int16                  vc_SquarePos;
//C       int16                  vc_SquareSign;
//C       int16                  vc_SquareSlidingIn;
//C       int16                  vc_SquareReverse;
//C       uint8                  vc_FilterOn;
//C       uint8                  vc_FilterInit;
//C       int16                  vc_FilterWait;
//C       int16                  vc_FilterSpeed;
//C       int16                  vc_FilterUpperLimit;
//C       int16                  vc_FilterLowerLimit;
//C       int16                  vc_FilterPos;
//C       int16                  vc_FilterSign;
//C       int16                  vc_FilterSlidingIn;
//C       int16                  vc_IgnoreFilter;
//C       int16                  vc_PerfCurrent;
//C       int16                  vc_PerfSpeed;
//C       int16                  vc_PerfWait;
//C       struct hvl_plist      *vc_PerfList;
//C       int8                  *vc_AudioPointer;
//C       int8                  *vc_AudioSource;
//C       uint8                  vc_NoteDelayOn;
//C       uint8                  vc_NoteCutOn;
//C       int16                  vc_NoteDelayWait;
//C       int16                  vc_NoteCutWait;
//C       int16                  vc_AudioPeriod;
//C       int16                  vc_AudioVolume;
//C       int32                  vc_WNRandom;
//C       int8                  *vc_MixSource;
//C       int8                   vc_SquareTempBuffer[0x80];
//C       int8                   vc_VoiceBuffer[0x282*4];
//C       uint8                  vc_VoiceNum;
//C       uint8                  vc_TrackMasterVolume;
//C       uint8                  vc_TrackOn;
//C       int16                  vc_VoicePeriod;
//C       uint32                 vc_Pan;
//C       uint32                 vc_SetPan;   // New for 1.4
//C       uint32                 vc_PanMultLeft;
//C       uint32                 vc_PanMultRight;
//C       uint32                 vc_RingSamplePos;
//C       uint32                 vc_RingDelta;
//C       int8                  *vc_RingMixSource;
//C       uint8                  vc_RingPlantPeriod;
//C       int16                  vc_RingInstrPeriod;
//C       int16                  vc_RingBasePeriod;
//C       int16                  vc_RingAudioPeriod;
//C       int8                  *vc_RingAudioSource;
//C       uint8                  vc_RingNewWaveform;
//C       uint8                  vc_RingWaveform;
//C       uint8                  vc_RingFixedPeriod;
//C       int8                   vc_RingVoiceBuffer[0x282*4];
//C     };
struct hvl_voice
{
    int16 vc_Track;
    int16 vc_NextTrack;
    int16 vc_Transpose;
    int16 vc_NextTranspose;
    int16 vc_OverrideTranspose;
    int32 vc_ADSRVolume;
    hvl_envelope vc_ADSR;
    hvl_instrument *vc_Instrument;
    uint32 vc_SamplePos;
    uint32 vc_Delta;
    uint16 vc_InstrPeriod;
    uint16 vc_TrackPeriod;
    uint16 vc_VibratoPeriod;
    uint16 vc_WaveLength;
    int16 vc_NoteMaxVolume;
    uint16 vc_PerfSubVolume;
    uint8 vc_NewWaveform;
    uint8 vc_Waveform;
    uint8 vc_PlantPeriod;
    uint8 vc_VoiceVolume;
    uint8 vc_PlantSquare;
    uint8 vc_IgnoreSquare;
    uint8 vc_FixedNote;
    int16 vc_VolumeSlideUp;
    int16 vc_VolumeSlideDown;
    int16 vc_HardCut;
    uint8 vc_HardCutRelease;
    int16 vc_HardCutReleaseF;
    uint8 vc_PeriodSlideOn;
    int16 vc_PeriodSlideSpeed;
    int16 vc_PeriodSlidePeriod;
    int16 vc_PeriodSlideLimit;
    int16 vc_PeriodSlideWithLimit;
    int16 vc_PeriodPerfSlideSpeed;
    int16 vc_PeriodPerfSlidePeriod;
    uint8 vc_PeriodPerfSlideOn;
    int16 vc_VibratoDelay;
    int16 vc_VibratoSpeed;
    int16 vc_VibratoCurrent;
    int16 vc_VibratoDepth;
    int16 vc_SquareOn;
    int16 vc_SquareInit;
    int16 vc_SquareWait;
    int16 vc_SquareLowerLimit;
    int16 vc_SquareUpperLimit;
    int16 vc_SquarePos;
    int16 vc_SquareSign;
    int16 vc_SquareSlidingIn;
    int16 vc_SquareReverse;
    uint8 vc_FilterOn;
    uint8 vc_FilterInit;
    int16 vc_FilterWait;
    int16 vc_FilterSpeed;
    int16 vc_FilterUpperLimit;
    int16 vc_FilterLowerLimit;
    int16 vc_FilterPos;
    int16 vc_FilterSign;
    int16 vc_FilterSlidingIn;
    int16 vc_IgnoreFilter;
    int16 vc_PerfCurrent;
    int16 vc_PerfSpeed;
    int16 vc_PerfWait;
    hvl_plist *vc_PerfList;
    int8 *vc_AudioPointer;
    int8 *vc_AudioSource;
    uint8 vc_NoteDelayOn;
    uint8 vc_NoteCutOn;
    int16 vc_NoteDelayWait;
    int16 vc_NoteCutWait;
    int16 vc_AudioPeriod;
    int16 vc_AudioVolume;
    int32 vc_WNRandom;
    int8 *vc_MixSource;
    int8 [128]vc_SquareTempBuffer;
    int8 [2568]vc_VoiceBuffer;
    uint8 vc_VoiceNum;
    uint8 vc_TrackMasterVolume;
    uint8 vc_TrackOn;
    int16 vc_VoicePeriod;
    uint32 vc_Pan;
    uint32 vc_SetPan;
    uint32 vc_PanMultLeft;
    uint32 vc_PanMultRight;
    uint32 vc_RingSamplePos;
    uint32 vc_RingDelta;
    int8 *vc_RingMixSource;
    uint8 vc_RingPlantPeriod;
    int16 vc_RingInstrPeriod;
    int16 vc_RingBasePeriod;
    int16 vc_RingAudioPeriod;
    int8 *vc_RingAudioSource;
    uint8 vc_RingNewWaveform;
    uint8 vc_RingWaveform;
    uint8 vc_RingFixedPeriod;
    int8 [2568]vc_RingVoiceBuffer;
}

//C     struct hvl_tune
//C     {
//C       TEXT                   ht_Name[128];
//C       uint16                 ht_SongNum;
//C       uint32                 ht_Frequency;
//C       float64                ht_FreqF;
//C       int8                  *ht_WaveformTab[MAX_CHANNELS];
//C       uint16                 ht_Restart;
//C       uint16                 ht_PositionNr;
//C       uint8                  ht_SpeedMultiplier;
//C       uint8                  ht_TrackLength;
//C       uint8                  ht_TrackNr;
//C       uint8                  ht_InstrumentNr;
//C       uint8                  ht_SubsongNr;
//C       uint16                 ht_PosJump;
//C       uint32                 ht_PlayingTime;
//C       int16                  ht_Tempo;
//C       int16                  ht_PosNr;
//C       int16                  ht_StepWaitFrames;
//C       int16                  ht_NoteNr;
//C       uint16                 ht_PosJumpNote;
//C       uint8                  ht_GetNewPosition;
//C       uint8                  ht_PatternBreak;
//C       uint8                  ht_SongEndReached;
//C       uint8                  ht_Stereo;
//C       uint16                *ht_Subsongs;
//C       uint16                 ht_Channels;
//C       struct hvl_position   *ht_Positions;
//C       struct hvl_step        ht_Tracks[256][64];
//C       struct hvl_instrument *ht_Instruments;
//C       struct hvl_voice       ht_Voices[MAX_CHANNELS];
//C       int32                  ht_defstereo;
//C       int32                  ht_defpanleft;
//C       int32                  ht_defpanright;
//C       int32                  ht_mixgain;
//C       uint8                  ht_Version;
//C     };
struct hvl_tune
{
    TEXT [128]ht_Name;
    uint16 ht_SongNum;
    uint32 ht_Frequency;
    float64 ht_FreqF;
    int8 *[16]ht_WaveformTab;
    uint16 ht_Restart;
    uint16 ht_PositionNr;
    uint8 ht_SpeedMultiplier;
    uint8 ht_TrackLength;
    uint8 ht_TrackNr;
    uint8 ht_InstrumentNr;
    uint8 ht_SubsongNr;
    uint16 ht_PosJump;
    uint32 ht_PlayingTime;
    int16 ht_Tempo;
    int16 ht_PosNr;
    int16 ht_StepWaitFrames;
    int16 ht_NoteNr;
    uint16 ht_PosJumpNote;
    uint8 ht_GetNewPosition;
    uint8 ht_PatternBreak;
    uint8 ht_SongEndReached;
    uint8 ht_Stereo;
    uint16 *ht_Subsongs;
    uint16 ht_Channels;
    hvl_position *ht_Positions;
    hvl_step [64][256]ht_Tracks;
    hvl_instrument *ht_Instruments;
    hvl_voice [16]ht_Voices;
    int32 ht_defstereo;
    int32 ht_defpanleft;
    int32 ht_defpanright;
    int32 ht_mixgain;
    uint8 ht_Version;
}

//C     void hvl_DecodeFrame( struct hvl_tune *ht, int8 *buf1, int8 *buf2, int32 bufmod );
void  hvl_DecodeFrame(hvl_tune *ht, int8 *buf1, int8 *buf2, int32 bufmod);
//C     void hvl_InitReplayer( void );
void  hvl_InitReplayer();
//C     BOOL hvl_InitSubsong( struct hvl_tune *ht, uint32 nr );
BOOL  hvl_InitSubsong(hvl_tune *ht, uint32 nr);
//C     struct hvl_tune *hvl_LoadTune( const TEXT *name, uint32 freq, uint32 defstereo );
hvl_tune * hvl_LoadTune(const TEXT *name, uint32 freq, uint32 defstereo);
//C     struct hvl_tune *hvl_LoadTuneMemory( uint8 *buf, int buflen, uint32 freq, uint32 defstereo );
hvl_tune * hvl_LoadTuneMemory(uint8 *buf, int buflen, uint32 freq, uint32 defstereo);

//C     void hvl_FreeTune( struct hvl_tune *ht );
void  hvl_FreeTune(hvl_tune *ht);

