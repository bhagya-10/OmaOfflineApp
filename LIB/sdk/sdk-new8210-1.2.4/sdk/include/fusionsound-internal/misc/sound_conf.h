/*
   (c) Copyright 2000-2002  convergence integrated media GmbH.
   (c) Copyright 2002-2005  convergence GmbH.
   
   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <sven@convergence.de> and
              Claudio Ciccani <klan@users.sf.net.

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

#ifndef __FS_CONFIG_H__
#define __FS_CONFIG_H__

#include <fusionsound.h>

typedef enum {
     FSRM_NONE = 0,
     FSRM_DPACK,
} FSRemoteCompression;

typedef struct {
     char               *driver;       /* Used driver, e.g. "oss" */
     char               *device;       /* Used device, e.g. "/dev/dsp" */
     
     FSSampleFormat      sampleformat; /* default sampleformat */
     FSChannelMode       channelmode;  /* default channelmode */
     int                 samplerate;   /* default samplerate */
     int                 buffertime;   /* default buffertime (in ms) */

     int                 session;      /* select multi app world */

     bool                banner;       /* startup banner */
     bool                wait;         /* wait slaves on exit */
     bool                deinit_check; /* enable deinit check at exit */
     
     bool                dither;       /* enable dithering */
    
     bool                dma;          /* use DMA */

     struct {
          char          *host;         /* Remote host in case of Voodoo Sound. */
          int            session;      /* Remote session number. */
     } remote;
     
     FSRemoteCompression remote_compression;
} FSConfig;

extern FSConfig *fs_config;

/*
 * Allocate Config struct, fill with defaults and parse command line options
 * for overrides. Options identified as FusionSound options are stripped out
 * of the array.
 */
DFBResult fs_config_init( int *argc, char **argv[] );

/*
 * Set indiviual option. Used by config_init(), and FusionSoundSetOption()
 */
DFBResult fs_config_set( const char *name, const char *value );

const char *fs_config_usage( void );

#endif /* __FS_CONFIG_H__ */
