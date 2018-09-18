/* libSoX skeleton file format handler.
 *
 * Copyright 1999 Chris Bagwell And Sundry Contributors
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sox_i.h"

#include <string.h>

/* Private data for PCM file */
typedef struct {
  size_t loop_point;
  size_t remaining_samples;
} priv_t;

/* Note that if any of your methods doesn't need to do anything, you
   can instead use the relevant sox_*_nothing* method */

/*
 * Do anything required before you start reading samples.
 * Read file header.
 *      Find out sampling rate,
 *      size and encoding of samples,
 *      mono/stereo/quad.
 */
static int startread(sox_format_t * ft)
{
  priv_t * msu = (priv_t *)ft->priv;
  size_t samples_in_file;
  char magic[5];

  /* If you need to seek around the input file. */
  if (!ft->seekable) {
    lsx_fail_errno(ft, SOX_EOF, "msupcm inputfile must be a file");
    return SOX_EOF;
  }

  /* Check for valid file header */
  if (lsx_reads(ft, magic, (size_t)4) == SOX_EOF || strncmp("MSU1", magic, (size_t)4) != 0)
  {
	  lsx_fail_errno(ft, SOX_EHDR, "MSU-1 header not found");
	  return SOX_EOF;
  }

  if (lsx_readdw(ft, (uint32_t*)&msu->loop_point) == SOX_EOF)
  {
	  lsx_fail_errno(ft, SOX_EHDR, "Error reading loop point");
	  return SOX_EOF;
  }

  ft->encoding.reverse_bytes = MACHINE_IS_BIGENDIAN;

  /*
   * If your format is headerless and has fixed values for
   * the following items, you can hard code them here (see cdr.c).
   * If your format contains a header with format information
   * then you should set it here.
   */
  ft->signal.rate = 44100; /* or 8000, 16000, 32000, 48000, ... */
  ft->signal.channels = 2; /* or 2 or 3 ... */
  ft->encoding.bits_per_sample = 16; /* or 16 ... */
  ft->encoding.encoding = SOX_ENCODING_SIGN2; /* or SIGN2 ... */

  /* If your format doesn't have a header then samples_in_file
   * can be determined by the file size.
   */
  samples_in_file = (lsx_filelength(ft) - 8) / (ft->encoding.bits_per_sample >> 3);

  /* If you can detect the length of your file, record it here. */
  ft->signal.length = samples_in_file;
  msu->remaining_samples = samples_in_file;

  return SOX_SUCCESS;
}

/*
 * Read up to len samples of type sox_sample_t from file into buf[].
 * Return number of samples read, or 0 if at end of file.
 */
static size_t read_samples(sox_format_t * ft, sox_sample_t *buf, size_t len)
{
  priv_t * UNUSED msu = (priv_t *)ft->priv;
  size_t done;
  uint16_t sample;

  for (done = 0; done < len; done++) {
    if (lsx_eof(ft) || lsx_readw(ft, &sample)) /* no more samples */
      break;
    switch (ft->encoding.bits_per_sample) {
    case 16:
      switch (ft->encoding.encoding) {
      case SOX_ENCODING_SIGN2:
        *buf++ = SOX_SIGNED_16BIT_TO_SAMPLE(sample,);
        break;
      default:
        lsx_fail("Undetected sample encoding in read!");
        return 0;
      }
      break;
    default:
      lsx_fail("Undetected bad sample size in read!");
      return 0;
    }
  }

  return done;
}

/*
 * Do anything required when you stop reading samples.
 * Don't close input file!
 */
static int stopread(sox_format_t UNUSED * ft)
{
  return SOX_SUCCESS;
}

static int startwrite(sox_format_t * ft)
{
  priv_t UNUSED * msu = (priv_t *)ft->priv;

  /* If you have to seek around the output file. */
  /* If header contains a length value then seeking will be
   * required.  Instead of failing, it's sometimes nice to
   * just set the length to max value and not fail.
   */
  if (!ft->seekable) {
    lsx_fail("Output .pcm file must be a file, not a pipe");
    return SOX_EOF;
  }

  if (ft->signal.rate != 44100)
  {
	lsx_fail("Output .pcm file must have a sample rate of 44100Hz");
	return SOX_EOF;
  }

  if (ft->encoding.bits_per_sample == 0) {
    lsx_fail("Did not specify a size for .pcm output file");
    return SOX_EOF;
  }

  /* error check ft->encoding.encoding */
  /* error check ft->signal.channels */

  /* Write file header, if any */
  lsx_writeb(ft, 'M');
  lsx_writeb(ft, 'S');
  lsx_writeb(ft, 'U');
  lsx_writeb(ft, '1');
  lsx_writedw(ft, 0);

  /* Write comment field, if any */

  return SOX_SUCCESS;

}

/*
 * Write len samples of type sox_sample_t from buf[] to file.
 * Return number of samples written.
 */
static size_t write_samples(sox_format_t * ft, const sox_sample_t *buf, size_t len)
{
  priv_t * msu = (priv_t *)ft->priv;
  size_t done = 0;

  (void)msu;
  switch (ft->encoding.bits_per_sample) {
  case 16:
    switch (ft->encoding.encoding) {
    SOX_SAMPLE_LOCALS;
    case SOX_ENCODING_SIGN2:
      while (done < len && lsx_writew(ft, (unsigned)SOX_SAMPLE_TO_SIGNED_16BIT(*buf++, ft->clips)) == SOX_SUCCESS)
        ++done;
      break;
    default:
      lsx_fail("Undetected bad sample encoding in write!");
      return 0;
    }
    break;
  default:
    lsx_fail("Undetected bad sample size in write!");
    return 0;
  }
  return done;
}

static int stopwrite(sox_format_t * ft)
{
  priv_t * msu = (priv_t *)ft->priv;

  /* All samples are already written out. */
  /* If file header needs fixing up, for example it needs the number
     of samples in a field, seek back and write them here. */
  lsx_seeki(ft, (off_t)(4), SEEK_SET);
  lsx_writedw(ft, (unsigned)msu->loop_point);

  return SOX_SUCCESS;
}

static int seek(sox_format_t * ft, uint64_t offset)
{
  /* Seek relative to current position. */
  ft->sox_errno = lsx_seeki(ft, (off_t)(offset + 8), SEEK_SET);

  return ft->sox_errno;
}

LSX_FORMAT_HANDLER(pcm)
{
  /* Format file suffixes */
  static const char *names[] = {"pcm",NULL };

  /* Encoding types and sizes that this handler can write */
  static const unsigned encodings[] = {
    SOX_ENCODING_SIGN2, 16, 0,
    0};

  /* Format descriptor
   * If no specific processing is needed for any of
   * the 7 functions, then the function above can be deleted
   * and NULL used in place of the its name below.
   */
  static sox_format_handler_t handler = {
    SOX_LIB_VERSION_CODE,
    "MSU-1 PCM audio format",
    names, 0,
    startread, read_samples, stopread,
    startwrite, write_samples, stopwrite,
    seek, encodings, NULL, sizeof(priv_t)
  };

  return &handler;
}
