/*
   (c) Copyright 2001-2007  The DirectFB Organization (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <sven@convergence.de> and
              Claudio Ciccani <klan@users.sf.net>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __FUSIONSOUND_H__
#define __FUSIONSOUND_H__

#include <directfb.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Version handling.
 */
extern const unsigned int fusionsound_major_version;
extern const unsigned int fusionsound_minor_version;
extern const unsigned int fusionsound_micro_version;
extern const unsigned int fusionsound_binary_age;
extern const unsigned int fusionsound_interface_age;

/*
 * Check for a certain FusionSound version.
 * In case of an error a message is returned describing the mismatch.
 */
const char * FusionSoundCheckVersion( unsigned int required_major,
                                      unsigned int required_minor,
                                      unsigned int required_micro );

/*
 * Main FusionSound interface.
 */
DECLARE_INTERFACE( IFusionSound )

/*
 * Static sound buffer for playback of smaller samples.
 */
DECLARE_INTERFACE( IFusionSoundBuffer )

/*
 * Streaming sound buffer for playback of large files or real time data.
 */
DECLARE_INTERFACE( IFusionSoundStream )

/*
 * Advanced playback control for static sound buffers.
 */
DECLARE_INTERFACE( IFusionSoundPlayback )

/*
 * Rendering music data into a stream.
 */
DECLARE_INTERFACE( IFusionSoundMusicProvider )

/*
 * Parses the command-line and initializes some variables. You absolutely need to
 * call this before doing anything else. Removes all options used by FusionSound from argv.
 */
DFBResult FusionSoundInit(
                           int    *argc,   /* pointer to main()'s argc */
                           char *(*argv[]) /* pointer to main()'s argv */
                         );

/*
 * Sets configuration parameters supported on command line and in config file.
 * Can only be called before FusionSoundCreate but after FusionSoundInit.
 */
DFBResult FusionSoundSetOption(
                                const char *name,
                                const char *value
                              );

/*
 * Creates the super interface.
 */
DFBResult FusionSoundCreate(
                             IFusionSound **ret_interface  /* pointer to the created interface */
                           );

/*
 * Print a description of the result code along with an
 * optional message that is put in front with a colon.
 */
DFBResult FusionSoundError(
                            const char *msg,    /* optional message */
                            DFBResult   result  /* result code to interpret */
                          );
                          
/*
 * Behaves like FusionSoundError, but shuts down the calling application.
 */
DFBResult FusionSoundErrorFatal(
                            const char *msg,    /* optional message */
                            DFBResult   result  /* result code to interpret */
                          );

/*
 * Returns a string describing 'result'.
 */
const char *FusionSoundErrorString(
                                    DFBResult result
                                  );
                                  
/*
 * Retrieves information about supported command-line flags in the
 * form of a user-readable string formatted suitable to be printed
 * as usage information.
 */
const char *FusionSoundUsageString( void );


#define FS_SOUND_DRIVER_INFO_NAME_LENGTH     40
#define FS_SOUND_DRIVER_INFO_VENDOR_LENGTH   60
#define FS_SOUND_DRIVER_INFO_URL_LENGTH     100
#define FS_SOUND_DRIVER_INFO_LICENSE_LENGTH  40

/*
 * Description of the sound driver.
 */
typedef struct {
     int  major;                                        /* Major version. */
     int  minor;                                        /* Minor version. */
     
     char name[FS_SOUND_DRIVER_INFO_NAME_LENGTH];       /* Driver name. */
     char vendor[FS_SOUND_DRIVER_INFO_VENDOR_LENGTH];   /* Driver vendor. */
     char url[FS_SOUND_DRIVER_INFO_URL_LENGTH];         /* Driver URL. */
     char license[FS_SOUND_DRIVER_INFO_LICENSE_LENGTH]; /* Driver license. */
} FSSoundDriverInfo;

#define FS_SOUND_DEVICE_INFO_NAME_LENGTH    96

/*
 * Description of the sound device.
 */
typedef struct {
     char name[FS_SOUND_DEVICE_INFO_NAME_LENGTH];       /* Device name. */
     
     FSSoundDriverInfo driver;
} FSDeviceDescription;

/*
 * @internal
 *
 * Encodes format constants in the following way (bit 31 - 0):
 *
 * 0000:0000 | 0000:0dcc | cccc:cbbb | bbbb:aaaa
 *
 * a) sampleformat index<br>
 * b) total bits per sample<br>
 * c) effective sound bits per sample (i.e. depth)<br>
 * d) signed sample format
 */
#define FS_SAMPLEFORMAT( index, bits, depth, sgned ) \
     ( ((index & 0x0F)      ) |                      \
       ((bits  & 0x7F) <<  4) |                      \
       ((depth & 0x7F) << 11) |                      \
       ((sgned & 0x01) << 18) )

/*
 * The sample format is the way of storing audible information.
 *
 * 16, 24 and 32 bit samples are always stored in <b>native endian</b>.
 * This keeps the library and applications simple and clean. 
 * Always access sample buffers like arrays of 8, 16 or 32 bit integers
 * depending on the sample format, unless data is written with endianness
 * being taken care of. This does not excuse from endian conversion
 * that might be necessary when reading data from files.
 */
typedef enum {
     FSSF_UNKNOWN        = 0x00000000, /* Unknown or invalid format. */
     
     /* Unsigned 8 bit. */ 
     FSSF_U8             = FS_SAMPLEFORMAT( 0,  8,  8, 0 ),
     
     /* Signed 16 bit (native endian). */
     FSSF_S16            = FS_SAMPLEFORMAT( 1, 16, 16, 1 ),

     /* Signed 24 bit (native endian). */
     FSSF_S24            = FS_SAMPLEFORMAT( 2, 24, 24, 1 ),

     /* Signed 32 bit (native endian). */
     FSSF_S32            = FS_SAMPLEFORMAT( 3, 32, 32, 1 ),
     
     /* Floating-point 32 bit (native endian). */
     FSSF_FLOAT          = FS_SAMPLEFORMAT( 4, 32, 32, 1 ),
} FSSampleFormat;

/* Number of defined sample formats. */
#define FS_NUM_SAMPLEFORMATS  5

/* These macros extract information about the sample format. */
#define FS_SAMPLEFORMAT_INDEX( fmt )  (((fmt) & 0x0000000F)      )

#define FS_BITS_PER_SAMPLE( fmt )     (((fmt) & 0x000007F0) >>  4)

#define FS_BYTES_PER_SAMPLE( fmt )    (((fmt) & 0x000007F0) >>  7)

#define FS_SAMPLEFORMAT_DEPTH( fmt )  (((fmt) & 0x0003f800) >> 11)

#define FS_SIGNED_SAMPLEFORMAT( fmt ) (((fmt) & 0x00040000) !=  0)


/*
 * @internal
 *
 * Encodes channelmode constants in the following way (bit 31 - 0):
 *
 * 0000:0000 | 0000:0000 | 0000:0000 | dccb:aaaa
 *
 * a) number of channels per frame<br>
 * b) has a center channel<br>
 * c) number of rear channels<br>
 * d) has a LFE channel<br>
 */
#define FS_CHANNELMODE( num, center, rears, lfe )  \
     ( ((num    & 0x3F)      ) |                   \
       ((center & 0x01) <<  4) |                   \
       ((rears  & 0x03) <<  5) |                   \
       ((lfe    & 0x01) <<  7) )                   \

/*
 * The channel mode provides information about the channels configuration.
 *
 * The channel mode instructs FusionSound about the channels configuration.
 * Particulary usefull when you have a stream with 3, 4 or 5 channels; in 
 * these cases, if unspecified, FusionSound assumes STEREO30 for 3 channels,
 * SURROUND40_2F2R for 4 and SURROUND50 for 5.
 */
typedef enum {
     FSCM_UNKNOWN         = 0x00000000, /* Unknown or invalid mode. */
     
     /* 1 Channel (Mono). */ 
     FSCM_MONO            = FS_CHANNELMODE( 1, 0, 0, 0 ),
     
     /* 2 Channels (Left Right). */
     FSCM_STEREO          = FS_CHANNELMODE( 2, 0, 0, 0 ),
     
     /* 3 Channels (Left Right Subwoofer). */
     FSCM_STEREO21        = FS_CHANNELMODE( 3, 0, 0, 1 ),

     /* 3 Channels (Left Center Right). */
     FSCM_STEREO30        = FS_CHANNELMODE( 3, 1, 0, 0 ),
     
     /* 4 Channels (Left Center Right Subwoofer). */
     FSCM_STEREO31        = FS_CHANNELMODE( 3, 1, 0, 1 ),
     
     /* 3 Channels (Left Right Rear). */
     FSCM_SURROUND30      = FS_CHANNELMODE( 3, 0, 1, 0 ),
     
     /* 4 Channels (Left Right Rear Subwoofer). */
     FSCM_SURROUND31      = FS_CHANNELMODE( 4, 0, 1, 1 ),
    
     /* 4 Channels (Left Right RearLeft RearRight). */
     FSCM_SURROUND40_2F2R = FS_CHANNELMODE( 4, 0, 2, 0 ),
     
     /* 5 Channels (Left Right RearLeft RearRight Subwoofer). */
     FSCM_SURROUND41_2F2R = FS_CHANNELMODE( 5, 0, 2, 1 ),
     
     /* 4 Channels (Left Center Right Rear). */
     FSCM_SURROUND40_3F1R = FS_CHANNELMODE( 4, 1, 1, 0 ),
     
     /* 5 Channels (Left Center Right Rear Subwoofer). */
     FSCM_SURROUND41_3F1R = FS_CHANNELMODE( 5, 1, 1, 1 ),
     
     /* 5 Channels (Left Center Right RearLeft RearRight). */
     FSCM_SURROUND50      = FS_CHANNELMODE( 5, 1, 2, 0 ),
     
     /* 6 Channels (Left Center Right RearLeft RearRight Subwoofer). */
     FSCM_SURROUND51      = FS_CHANNELMODE( 6, 1, 2, 1 ),
} FSChannelMode;

/* Number of defined channel modes. */
#define FS_NUM_CHANNELMODES  13

/* These macros extract information about the channel mode. */
#define FS_CHANNELS_FOR_MODE( mode )  (((mode) & 0x0000000F)     )

#define FS_MODE_HAS_CENTER( mode )    (((mode) & 0x00000010) != 0)

#define FS_MODE_NUM_REARS( mode )     (((mode) & 0x00000060) >> 5)

#define FS_MODE_HAS_LFE( mode )       (((mode) & 0x00000080) != 0)


/*
 * Each buffer description flag validates one field of the buffer description.
 */
typedef enum {
     FSBDF_NONE          = 0x00000000,      /* None of these. */
     FSBDF_LENGTH        = 0x00000001,      /* Buffer length is set. */
     FSBDF_CHANNELS      = 0x00000002,      /* Number of channels is set. */
     FSBDF_SAMPLEFORMAT  = 0x00000004,      /* Sample format is set. */
     FSBDF_SAMPLERATE    = 0x00000008,      /* Sample rate is set. */
     FSBDF_CHANNELMODE   = 0x00000010,      /* Channel mode is set. */
     FSBDF_ALL           = 0x0000001F       /* All of these. */
} FSBufferDescriptionFlags;

/*
 * The buffer description is used to create static sound buffers.
 */
typedef struct {
     FSBufferDescriptionFlags flags;        /* Defines which fields are set. */

     int                      length;       /* Buffer length specified as
                                               number of samples per channel. */
     int                      channels;     /* Number of channels. */
     FSSampleFormat           sampleformat; /* Format of each sample. */
     int                      samplerate;   /* Number of samples per second. */
     FSChannelMode            channelmode;  /* Channel mode (overrides channels). */
} FSBufferDescription;

/*
 * Each stream description flag validates one field of the stream description.
 */
typedef enum {
     FSSDF_NONE          = 0x00000000,      /* None of these. */
     FSSDF_BUFFERSIZE    = 0x00000001,      /* Ring buffer size is set. */
     FSSDF_CHANNELS      = 0x00000002,      /* Number of channels is set. */
     FSSDF_SAMPLEFORMAT  = 0x00000004,      /* Sample format is set. */
     FSSDF_SAMPLERATE    = 0x00000008,      /* Sample rate is set. */
     FSSDF_PREBUFFER     = 0x00000010,      /* Prebuffer amount is set. */
     FSSDF_CHANNELMODE   = 0x00000020,      /* Channel mode is set. */
     FSSDF_ALL           = 0x0000003F       /* All of these. */
} FSStreamDescriptionFlags;

/*
 * The stream description is used to create streaming sound buffers.
 */
typedef struct {
     FSStreamDescriptionFlags flags;        /* Defines which fields are set. */

     int                      buffersize;   /* Ring buffer size specified as
                                               a number of samples (per channel). */
     int                      channels;     /* Number of channels. */
     FSSampleFormat           sampleformat; /* Format of each sample. */
     int                      samplerate;   /* Number of samples per second (per channel). */
     int                      prebuffer;    /* Samples to buffer before starting the playback.
                                               A negative value disables auto start of playback. */
     FSChannelMode            channelmode;  /* Channel mode (overrides channels). */
} FSStreamDescription;

/*
 * Information about an IFusionSoundMusicProvider.
 */
typedef enum {
     FMCAPS_BASIC      = 0x00000000,  /* Basic ops (PlayTo, Stop).                */
     FMCAPS_SEEK       = 0x00000001,  /* Supports SeekTo.                         */
     FMCAPS_RESAMPLE   = 0x00000002,  /* Supports audio resampling.               */
     FMCAPS_HALFRATE   = 0x00000004,  /* Supports decoding at half original rate. */
} FSMusicProviderCapabilities;

/*
 * Information about the status of a music provider.
 */
typedef enum {
     FMSTATE_UNKNOWN   = 0x00000000, /* Unknown status.       */
     FMSTATE_PLAY      = 0x00000001, /* Provider is playing.  */
     FMSTATE_STOP      = 0x00000002, /* Playback was stopped. */
     FMSTATE_FINISHED  = 0x00000004, /* Playback is finished. */
     FMSTATE_ALL       = 0x00000007
} FSMusicProviderStatus;

/*
 * Flags controlling playback of a music provider.
 */
typedef enum {
     FMPLAY_NOFX       = 0x000000000, /* Normal playback.                            */
     FMPLAY_LOOPING    = 0x000000001, /* Automatically restart playback as soon
                                         as the end of the current track is reached. */
} FSMusicProviderPlaybackFlags;

/*
 * Track ID.
 */
typedef unsigned int FSTrackID;

#define FS_TRACK_DESC_ARTIST_LENGTH   32
#define FS_TRACK_DESC_TITLE_LENGTH    125
#define FS_TRACK_DESC_ALBUM_LENGTH    125
#define FS_TRACK_DESC_GENRE_LENGTH    32
#define FS_TRACK_DESC_ENCODING_LENGTH 32

/*
 * Description of a track provided by a music provider.
 */
typedef struct {
     char  artist[FS_TRACK_DESC_ARTIST_LENGTH];     /* Artist. */
     char  title[FS_TRACK_DESC_TITLE_LENGTH];       /* Title. */
     char  album[FS_TRACK_DESC_ALBUM_LENGTH];       /* Album. */
     short year;                                    /* Year. */
     char  genre[FS_TRACK_DESC_GENRE_LENGTH];       /* Genre. */
     char  encoding[FS_TRACK_DESC_ENCODING_LENGTH]; /* Encoding (for example: MPEG Layer-1). */
     int   bitrate;                                 /* Bitrate in bits/s. */
     float replaygain;                              /* ReplayGain level (1.0 by default). */
     float replaygain_album;                        /* Album ReplayGain level. */
} FSTrackDescription;

/*
 * Called for each track provided by a music provider.
 */
typedef DFBEnumerationResult (*FSTrackCallback) (
     FSTrackID                track_id,
     FSTrackDescription       desc,
     void                    *callbackdata
);

/*
 * <i><b>IFusionSound</b></i> is the main FusionSound interface. 
 * It can be retrieved either by calling <i>IDirectFB::GetInterface()</i> or
 * by calling <i>FusionSoundCreate()</i> (after <i>FusionSoundInit()</i>).
 *
 * <b>Static sound buffers</b> for smaller samples like sound effects in
 * games or audible feedback in user interfaces are created by calling
 * <i>CreateBuffer()</i>. They can be played several times with an unlimited
 * number of <b>concurrent playbacks</b>. Playback can be started in
 * <b>looping</b> mode. Other per-playback control includes <b>pan value</b>,
 * <b>volume level</b> and <b>pitch</b>.
 *
 * <b>Streaming sound buffers</b> for large or compressed files and for
 * streaming of real time sound data are created by calling
 * <i>CreateStream()</i>. There's only one <b>single playback</b> that
 * automatically starts when data is written to the <b>ring buffer</b> for the
 * first time. If the buffer underruns, the playback automatically stops and
 * continues when the ring buffer is written to again.
 *
 */
DEFINE_INTERFACE( IFusionSound,

   /** Information **/
   
     /* 
      * Get a description of the sound device.
      */
     DFBResult (*GetDeviceDescription) (
          IFusionSound           *thiz,
          FSDeviceDescription    *ret_desc
     );

   /** Buffers **/

     /*
      * Create a static sound buffer.
      *
      * This requires a <b>desc</b> with at least the length being set.
      *
      * Default values for sample rate, sample format and number of channels
      * depend on device configuration.
      */
     DFBResult (*CreateBuffer) (
          IFusionSound               *thiz,
          const FSBufferDescription  *desc,
          IFusionSoundBuffer        **interface
     );

     /*
      * Create a streaming sound buffer.
      *
      * If <b>desc</b> is NULL, all default values will be used.
      *
      * Default values for sample rate, sample format and number of channels
      * depend on device configuration, while ring buffer length defaults to 1/5 seconds.
      */
     DFBResult (*CreateStream) (
          IFusionSound               *thiz,
          const FSStreamDescription  *desc,
          IFusionSoundStream        **interface
     );

     /*
      * Create a music provider.
      */
     DFBResult (*CreateMusicProvider) (
          IFusionSound               *thiz,
          const char                 *filename,
          IFusionSoundMusicProvider **interface
     );
     
     
   /** Volume Control **/
     
     /*
      * Get master volume level.
      *
      * Get the master volume level (i.e. that applies to all playbacks).
      *
      * See also <i>SetMasterVolume()</i>.
      */
     DFBResult (*GetMasterVolume) (
          IFusionSound               *thiz,
          float                      *level
     );
     
     /*
      * Set master volume level.
      *
      * Set the master volume level (i.e. that applies to all playbacks).<br>
      * The <b>level</b> is a linear factor ranging from 0.0f to 1.0f.
      *
      * See also <i>GetMasterVolume()</i>.
      */
     DFBResult (*SetMasterVolume) (
          IFusionSound               *thiz,
          float                       level
     );
     
     /*
      * Get local volume level.
      *
      * Get the local volume level (i.e. that applies to the playbacks
      * created by the current process).
      *
      * See also <i>SetLocalVolume()</i>.
      */
     DFBResult (*GetLocalVolume) (
          IFusionSound               *thiz,
          float                      *level
     );
     
     /*
      * Set local volume level.
      *
      * Set the local volume level (i.e. that applies to the playbacks
      * created by the current process).<br>
      * The <b>level</b> is a linear factor ranging from 0.0f to 1.0f.
      *
      * See also <i>GetLocalVolume()</i>.
      */
     DFBResult (*SetLocalVolume) (
          IFusionSound               *thiz,
          float                       level
     );
     
     
   /** Misc **/
   
     /*
      * Suspend FusionSound.
      *
      * No other calls to FusionSound are allowed until <i>Resume()</i>
      * has been called.
      */
     DFBResult (*Suspend) (
          IFusionSound               *thiz
     );
     
     /*
      * Resume FusionSound.
      *
      * Only to be called after <i>Suspend()</i>.
      */
     DFBResult (*Resume) (
          IFusionSound               *thiz
     );
)

/*
 * Flags for simple playback using <i>IFusionSoundBuffer::Play()</i>.
 */
typedef enum {
     FSPLAY_NOFX         = 0x00000000,  /* No effects are applied. */
     FSPLAY_LOOPING      = 0x00000001,  /* Playback will continue at the
                                           beginning of the buffer as soon as
                                           the end is reached. There's no gap
                                           produced by concatenation. Only one
                                           looping playback at a time is
                                           supported by the simple playback.
                                           See also <i>CreatePlayback()</i>. */
     FSPLAY_CYCLE        = 0x00000002,  /* Play the whole buffer for one cycle, 
                                           wrapping at the end. */
     FSPLAY_REWIND       = 0x00000004,  /* Play reversing sample order. */
     FSPLAY_ALL          = 0x00000007   /* All of these. */
} FSBufferPlayFlags;


/*
 * <i><b>IFusionSoundBuffer</b></i> represents a static block of sample data.
 *
 * <b>Data access</b> is simply provided by <i>Lock()</i> and <i>Unlock()</i>.
 *
 * There are <b>two ways of playback</b>.
 *
 * <b>Simple playback</b> is provided by this interface. It includes an
 * unlimited number of non-looping playbacks plus one looping playback at a
 * time. To start the <b>looping</b> playback with <i>Play()</i> use the 
 * FSPLAY_LOOPING playback flag. It will <b>stop</b> when the interface is
 * destroyed or <i>Stop()</i> is called.
 *
 * <b>Advanced playback</b> is provided by an extra interface called
 * <i>IFusionSoundPlayback</i> which is created by <i>CreatePlayback()</i>.
 * It includes <b>live</b> control over <b>pan</b>, <b>volume</b>, <b>pitch</b>
 * and provides <b>versatile playback commands</b>.
 */
DEFINE_INTERFACE( IFusionSoundBuffer,

   /** Information **/

     /*
      * Get a description of the buffer.
      */
     DFBResult (*GetDescription) (
          IFusionSoundBuffer       *thiz,
          FSBufferDescription      *ret_desc
     );


   /** Positioning **/
   
     /*
      * Set the buffer position indicator.
      *
      * Set the buffer position indicator (in frames)
      * affecting subsequent playback and lock for access.
      */
     DFBResult (*SetPosition) (
          IFusionSoundBuffer       *thiz,
          int                       position
     );
     
     
   /** Access **/

     /*
      * Lock a buffer to access its data.
      *
      * Optionally returns the amount of available frames or bytes
      * at the current position.
      *
      * See also <i>SetPosition()</i>.
      */
     DFBResult (*Lock) (
          IFusionSoundBuffer       *thiz,
          void                    **ret_data,
          int                      *ret_frames,
          int                      *ret_bytes
     );

     /*
      * Unlock a buffer.
      */
     DFBResult (*Unlock) (
          IFusionSoundBuffer       *thiz
     );


   /** Simple playback **/

     /*
      * Start playing the buffer.
      *
      * Start playing the buffer at the specified position 
      * (see <i>SetPosition()</i>).
      * There's no limited number of concurrent playbacks, but the simple
      * playback only provides one looping playback at a time.
      *
      * See also <i>CreatePlayback()</i>.
      */
     DFBResult (*Play) (
          IFusionSoundBuffer       *thiz,
          FSBufferPlayFlags         flags
     );

     /*
      * Stop looping playback.
      *
      * This method is for the one concurrently looping playback that is
      * provided by the simple playback.
      *
      * See also <i>CreatePlayback()</i>.
      */
     DFBResult (*Stop) (
          IFusionSoundBuffer       *thiz
     );


   /** Advanced playback **/

     /*
      * Retrieve advanced playback control interface.
      *
      * Each playback instance represents one concurrent playback of the buffer.
      */
     DFBResult (*CreatePlayback) (
          IFusionSoundBuffer       *thiz,
          IFusionSoundPlayback    **interface
     );
)

/*
 * <i><b>IFusionSoundStream</b></i> represents a ring buffer for streamed
 * playback which fairly maps to writing to a sound device. Use it for easy
 * porting of applications that use exclusive access to a sound device.
 *
 * <b>Writing</b> to the ring buffer <b>triggers the playback</b> if it's not
 * already running. The method <i>Write()</i> can be called with an <b>arbitrary
 * number of samples</b>. It returns after all samples have been written to the
 * ring buffer and <b>sleeps</b> while the ring buffer is full.
 * Blocking writes are perfect for accurate filling of the buffer,
 * which keeps the ring buffer as full as possible using a very small block
 * size (depending on sample rate, playback pitch and the underlying hardware).
 *
 * <b>Waiting</b> for a specific amount of <b>free space</b> in the ring buffer
 * is provided by <i>Wait()</i>. It can be used to <b>avoid blocking</b> of
 * <i>Write()</i> or to <b>finish playback</b> before destroying the interface.
 *
 * <b>Status information</b> includes the amount of <b>filled</b> and
 * <b>total</b> space in the ring buffer, along with the current <b>read</b> and
 * <b>write position</b>. It can be retrieved by calling <i>GetStatus()</i> at
 * any time without blocking.
 */
DEFINE_INTERFACE( IFusionSoundStream,

   /** Information **/

     /*
      * Get a description of the stream.
      */
     DFBResult (*GetDescription) (
          IFusionSoundStream       *thiz,
          FSStreamDescription      *ret_desc
     );


   /** Ring buffer **/

     /*
      * Fill the ring buffer with data.
      *
      * Writes the sample <b>data</b> into the ring buffer.
      * The <b>length</b> specifies the number of samples per channel.
      *
      * If the ring buffer gets full, the method blocks until it can write more
      * data.
      *
      * If this method returns successfully, all data has been written.
      */
     DFBResult (*Write) (
          IFusionSoundStream       *thiz,
          const void               *data,
          int                       length
     );

     /*
      * Wait for a specified amount of free ring buffer space.
      *
      * This method blocks until there's free space of at least the specified
      * <b>length</b> (number of samples per channel).
      *
      * Specifying a <b>length</b> of zero waits until playback has finished.
      */
     DFBResult (*Wait) (
          IFusionSoundStream       *thiz,
          int                       length
     );

     /*
      * Query ring buffer status.
      *
      * Returns the number of samples the ring buffer is <b>filled</b> with,
      * the <b>total</b> number of samples that can be stored (buffer size),
      * current <b>read_position</b> and <b>write_position</b> and if the stream
      * is <b>playing</b>.
      *
      * Simply pass NULL for values that are not of interest.
      */
     DFBResult (*GetStatus) (
          IFusionSoundStream       *thiz,
          int                      *filled,
          int                      *total,
          int                      *read_position,
          int                      *write_position,
          DFBBoolean               *playing
     );

     /*
      * Flush the ring buffer.
      *
      * This method stops the playback immediately and 
      * discards any buffered data.
      */
     DFBResult (*Flush) (
          IFusionSoundStream       *thiz
     );

     /*
      * Drop pending data.
      *
      * This method discards any pending input data,
      * making <i>Write()</i> return as soon as possible.
      */
     DFBResult (*Drop) (
          IFusionSoundStream       *thiz
     );


   /** Timing **/

     /*
      * Query the presentation delay.
      *
      * Returns the amount of time in milli seconds that passes
      * until the last sample stored in the buffer is audible.
      * This includes any buffered data (by hardware or driver)
      * as well as the ring buffer status of the stream.
      *
      * Even if the stream is not playing, e.g. due to pre-buffering,
      * the method behaves as if the playback has just been started.
      */
     DFBResult (*GetPresentationDelay) (
          IFusionSoundStream       *thiz,
          int                      *delay
     );


   /** Playback control **/

     /*
      * Retrieve advanced playback control interface.
      *
      * The returned <b>interface</b> provides advanced control over the playback of the stream.
      * This includes volume, pitch and pan settings as well as manual starting, pausing or
      * stopping of the playback.
      */
     DFBResult (*GetPlayback) (
          IFusionSoundStream       *thiz,
          IFusionSoundPlayback    **interface
     );


   /** Direct memory access **/
   
     /*
      * Access the ring buffer to fill it with data.
      *
      * The method returns a pointer to the current write position 
      * and the amount of available space in frames.
      *
      * If the ring buffer is full, the method blocks until there is 
      * space available.
      *
      * After filling the ring buffer, call <i>Commit()</i> to submit
      * the samples to the stream.
      */
     DFBResult (*Access) (
          IFusionSoundStream       *thiz,
          void                    **ret_data,
          int                      *ret_avail
     );
     
     /*
      * Commit written data to the stream.
      *
      * Commit <b>length</b> frames of data written upon previous <i>Access()</i>.
      */
     DFBResult (*Commit) (
          IFusionSoundStream       *thiz,
          int                       length
     );
)

/*
 * Direction of a playback.
 */
typedef enum {
     FSPD_FORWARD        = +1, /* Forward.  */
     FSPD_BACKWARD       = -1, /* Backward. */
} FSPlaybackDirection;

/*
 * <i><b>IFusionSoundPlayback</b></i> represents one concurrent playback and
 * provides full control over the internal processing of samples.
 *
 * <b>Commands</b> control the playback as in terms of tape transportation.
 * This includes <b>starting</b> the playback at <b>any position</b> with an
 * optional <b>stop position</b>. The default value of <b>zero</b> causes the
 * playback to stop at the <b>end</b>. A <b>negative</b> value puts the playback
 * in <b>looping</b> mode. <i>Start()</i> does <b>seeking</b> if the playback is
 * already running and updates the stop position. Other methods provide
 * <b>pausing</b>, <b>stopping</b> and <b>waiting</b> for the playback to end.
 *
 * <b>Information</b> provided by <i>GetStatus()</i> includes the current
 * <b>position</b> and whether the playback is <b>running</b>.
 *
 * <b>Parameters</b> provide <b>live</b> control over <b>volume</b>, <b>pan</b>,
 * <b>pitch</b> (speed factor) and <b>direction</b> of the playback.
 */
DEFINE_INTERFACE( IFusionSoundPlayback,

   /** Commands **/

     /*
      * Start playback of the buffer.
      *
      * This method is only supported for playback of a buffer.
      * For stream playbacks use <i>Continue()</i>.
      *
      * The <b>start</b> position specifies the sample at which the playback
      * is going to start.
      *
      * The <b>stop</b> position specifies the sample after the last sample
      * being played. The default value of zero causes the playback to stop
      * after the last sample in the buffer, i.e. upon completion. A negative
      * value means unlimited playback (looping).
      *
      * This method can be used for seeking if the playback is already running.
      */
     DFBResult (*Start) (
          IFusionSoundPlayback     *thiz,
          int                       start,
          int                       stop
     );

     /*
      * Stop playback of the buffer.
      *
      * This method stops a running playback. The playback can be continued
      * by calling <i>Continue()</i> or restarted using <i>Start()</i>.
      */
     DFBResult (*Stop) (
          IFusionSoundPlayback     *thiz
     );

     /*
      * Continue playback of the buffer or start playback of a stream.
      *
      * This method is used to continue a playback that isn't running (anymore).
      *
      * The playback will begin at the position where it stopped, either
      * explicitly by <i>Stop()</i> or by reaching the stop position.
      *
      * If the playback has never been started, it uses the default start and
      * stop position which means non-looping playback from the beginning
      * to the end.
      *
      * It returns WITHOUT an error if the playback is running.
      *
      * This method can be used to trigger one-at-a-time playback without having
      * to check if it's already running. It's similar to simple playback via
      * <i>IFusionSoundBuffer::Play()</i>, but rejects multiple concurrent
      * playbacks.
      */
     DFBResult (*Continue) (
          IFusionSoundPlayback     *thiz
     );

     /*
      * Wait until playback of the buffer has finished.
      *
      * This method will block as long as the playback is running.
      *
      * If the playback is in looping mode the method returns immediately
      * with an error.
      */
     DFBResult (*Wait) (
          IFusionSoundPlayback     *thiz
     );


   /** Information **/

     /*
      * Get the current playback status.
      *
      * This method can be used to check if the playback is <b>running</b>.
      *
      * It also returns the current playback <b>position</b> or the position
      * where <i>Continue()</i> would start to play.
      */
     DFBResult (*GetStatus) (
          IFusionSoundPlayback     *thiz,
          DFBBoolean               *running,
          int                      *position
     );


   /** Parameters **/

     /*
      * Set volume level.
      *
      * The <b>level</b> is a linear factor being 1.0f by default, currently
      * ranges from 0.0f to 64.0f due to internal mixing limitations.
      */
     DFBResult (*SetVolume) (
          IFusionSoundPlayback     *thiz,
          float                     level
     );

     /*
      * Set panning value.
      *
      * The <b>value</b> ranges from -1.0f (left) to 1.0f (right).
      */
     DFBResult (*SetPan) (
          IFusionSoundPlayback     *thiz,
          float                     value
     );

     /*
      * Set pitch value.
      *
      * The <b>value</b> is a linear factor being 1.0f by default, currently
      * ranges from 0.0f to 64.0f due to internal mixing limitations.
      */
     DFBResult (*SetPitch) (
          IFusionSoundPlayback     *thiz,
          float                     value
     );
     
     /*
      * Set the direction of the playback.
      */
     DFBResult (*SetDirection) (
          IFusionSoundPlayback     *thiz,
          FSPlaybackDirection       direction
     );
     
     /*
      * Set the volume levels for downmixing.
      *
      * Set the levels used for downmixing center and rear channels
      * of a multichannel buffer (more than 2 channels).<br>
      * Levels are linear factors ranging from 0.0f to 1.0f and
      * being 0.707f (-3dB) by default.
      */
     DFBResult (*SetDownmixLevels) (
          IFusionSoundPlayback     *thiz,
          float                     center,
          float                     rear
     );
)

/*
 * Result of a FMBufferCallback.
 */
typedef enum {
     FMBCR_OK            = 0x00000000, /* Continue. */
     FMBCR_BREAK         = 0x00000001, /* Stop loading. */
} FMBufferCallbackResult;

/*
 * Called after each buffer write.
 */
typedef FMBufferCallbackResult (*FMBufferCallback)( int length, void *ctx );

/*
 * <i>No summary yet...</i>
 */
DEFINE_INTERFACE(   IFusionSoundMusicProvider,
   /** Retrieving information **/

     /*
      * Retrieve information about the music provider's
      * capabilities.
      */
     DFBResult (*GetCapabilities) (
          IFusionSoundMusicProvider   *thiz,
          FSMusicProviderCapabilities *ret_caps
     );

     /*
      * Enumerate all tracks contained in the file.
      *
      * Calls the given callback for all available tracks.<br>
      * The callback is passed the track id that can be
      * used to select a track for playback using 
      * IFusionSoundMusicProvider::SelectTrack().
      */
     DFBResult (*EnumTracks) (
          IFusionSoundMusicProvider *thiz,
          FSTrackCallback            callback,
          void                      *callbackdata
     );

     /*
      * Get the unique ID of the current track.
      */
     DFBResult (*GetTrackID) (
          IFusionSoundMusicProvider *thiz,
          FSTrackID                 *ret_track_id
     );

     /*
      * Get a description of the current track.
      */
     DFBResult (*GetTrackDescription) (
          IFusionSoundMusicProvider *thiz,
          FSTrackDescription        *ret_desc
     );

     /*
      * Get a stream description that best matches the music
      * contained in the file.
      */
     DFBResult (*GetStreamDescription) (
          IFusionSoundMusicProvider *thiz,
          FSStreamDescription       *ret_desc
     );
     
     /*
      * Get a buffer description that best matches the music
      * contained in the file.
      *
      * The music provider is responsible of returning a 
      * buffer description suitable for holding the whole track.
      */
     DFBResult (*GetBufferDescription) (
          IFusionSoundMusicProvider *thiz,
          FSBufferDescription       *ret_desc
     );

   /** Playback **/

     /*
      * Select a track by its unique ID.
      */
     DFBResult (*SelectTrack) (
          IFusionSoundMusicProvider *thiz,
          FSTrackID                  track_id 
     );
     
     /*
      * Play selected track rendering it into
      * the destination stream.
      */
     DFBResult (*PlayToStream) (
          IFusionSoundMusicProvider *thiz,
          IFusionSoundStream        *destination
     );

     /*
      * Play selected track rendering it into
      * the destination buffer.
      *
      * Optionally a callback can be registered
      * that is called after each buffer write.<br>
      * The callback is passed the number of 
      * samples per channels actually written
      * to the destination buffer.
      */
     DFBResult (*PlayToBuffer) (
          IFusionSoundMusicProvider *thiz,
          IFusionSoundBuffer        *destination,
          FMBufferCallback           callback,
          void                      *callbackdata
     );

     /*
      * Stop playback.
      */
     DFBResult (*Stop) (
          IFusionSoundMusicProvider *thiz
     );
     
     /*
      * Get playback status.
      */
     DFBResult (*GetStatus) (
          IFusionSoundMusicProvider *thiz,
          FSMusicProviderStatus     *ret_status
     );

   /** Media Control **/

     /*
      * Seeks to a position within the current track.
      */
     DFBResult (*SeekTo) (
          IFusionSoundMusicProvider *thiz,
          double                     seconds
     );

     /*
      * Gets current position within the current track.
      */
     DFBResult (*GetPos) (
          IFusionSoundMusicProvider *thiz,
          double                    *ret_seconds
     );

     /*
      * Gets the length of the current track.
      */
     DFBResult (*GetLength) (
          IFusionSoundMusicProvider *thiz,
          double                    *ret_seconds
     );
     
   /** Advanced Playback **/
     
     /*
      * Set the flags controlling playback.
      */
     DFBResult (*SetPlaybackFlags) (
          IFusionSoundMusicProvider    *thiz,
          FSMusicProviderPlaybackFlags  flags
     );
     
     /*
      * Wait for playback status.
      *
      * This method blocks until playback reaches
      * one of the states specified in <b>mask</b> or,
      * if the specified <b>timeout</b> in milliseconds
      * is non-zero, until timeout expires.
      */
     DFBResult (*WaitStatus) (
          IFusionSoundMusicProvider *thiz,
          FSMusicProviderStatus      mask,
          unsigned int               timeout
     );
)

#ifdef __cplusplus
}
#endif

#endif

