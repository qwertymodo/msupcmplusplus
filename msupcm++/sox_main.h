#ifndef SOX_MAIN_H
#define SOX_MAIN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "sox.h"
#include "util.h"
#include "unicode_support.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_GETTIMEOFDAY
#define TIME_FRAC 1e6
#else
#define timeval timeb
#define gettimeofday(a,b) ftime(a)
#define tv_sec time
#define tv_usec millitm
#define TIME_FRAC 1e3
#endif

extern lsx_getopt_t optstate;

/* argv[0] options */

extern char const * myname;
typedef enum _sox_mode { sox_sox, sox_play, sox_rec, sox_soxi } _sox_mode;
extern _sox_mode sox_mode;


/* gopts */

typedef enum sox_combine_method {
	sox_sequence, sox_concatenate, sox_mix, sox_mix_power,
	sox_merge, sox_multiply, sox_default
} sox_combine_method;
extern sox_combine_method combine_method;
typedef enum { sox_single, sox_multiple } sox_output_method;
extern sox_output_method output_method;
#define is_serial(m) ((m) <= sox_concatenate)
#define is_parallel(m) (!is_serial(m))
extern sox_bool no_clobber;
extern sox_bool interactive;
extern sox_bool uservolume;
typedef enum { RG_off, RG_track, RG_album, RG_default } rg_mode;
extern lsx_enum_item const rg_modes[];
extern rg_mode replay_gain_mode;
extern sox_option_t show_progress;


/* Input & output files */

typedef struct {
	char * filename;

	/* fopts */
	char const * filetype;
	sox_signalinfo_t signal;
	sox_encodinginfo_t encoding;
	double volume;
	double replay_gain;
	sox_oob_t oob;
	sox_bool no_glob;

	sox_format_t * ft;  /* libSoX file descriptor */
	uint64_t volume_clips;
	rg_mode replay_gain_mode;
} file_t;

extern file_t * * files; /* Array tracking input and output files */
#define ofile files[file_count - 1]
extern size_t file_count;
extern size_t input_count;
extern size_t output_count;

/* Effects */

/* We parse effects into a temporary effects table and then place into
* the real effects chain.  This allows scanning all effects to give
* hints to what input effect options should be as well as determining
* when mixer or resample effects need to be auto-inserted as well.
*/
extern sox_effect_t **user_efftab;
extern size_t user_efftab_size;
extern sox_effects_chain_t *effects_chain;
extern sox_effect_t *save_output_eff;

typedef struct user_effargs_t { char *name; int argc; char **argv; size_t argv_size; } user_effargs_t;
extern user_effargs_t **user_effargs;
extern size_t *user_effargs_size;  /* array: size of user_effargs for each chain */
								   /* Size of memory structures related to effects arguments (user_effargs[i],
								   * user_effargs[i][j].argv) to be extended in steps of EFFARGS_STEP */
#define EFFARGS_STEP 8
extern size_t *nuser_effects;  /* array: number of effects in each chain */
extern size_t current_eff_chain;
extern size_t eff_chain_count;
extern sox_bool very_first_effchain;
/* Indicates that not only the first effects chain is in effect (hrm), but
also that it has never been restarted. Only then we may use the
optimize_trim() hack. */
extern char *effects_filename;
extern char * play_rate_arg;
extern char *norm_level;

/* Flowing */

extern sox_signalinfo_t combiner_signal;
extern sox_signalinfo_t ofile_signal_options;
extern sox_encodinginfo_t combiner_encoding;
extern sox_encodinginfo_t ofile_encoding_options;
extern uint64_t mixing_clips;
extern size_t current_input;
extern uint64_t input_wide_samples;
extern uint64_t read_wide_samples;
extern uint64_t output_samples;
extern sox_bool input_eof;
extern sox_bool output_eof;
extern sox_bool user_abort;
extern sox_bool user_skip;
extern sox_bool user_restart_eff;
extern int success;
extern int cleanup_called;
extern sox_sample_t omax[2];
extern sox_sample_t omin[2];

#ifdef HAVE_TERMIOS_H
#include <termios.h>
extern struct termios original_termios;
extern sox_bool original_termios_saved;
#endif

extern sox_bool stdin_is_a_tty;
extern sox_bool is_player;
extern sox_bool is_guarded;
extern sox_bool do_guarded_norm;
extern sox_bool no_dither;
extern sox_bool reported_sox_opts;

extern struct timeval load_timeofday;

/* The input combiner: contains one sample buffer per input file, but only
* needed if is_parallel(combine_method) */
typedef struct {
	sox_sample_t * * ibuf;
	size_t *         ilen;
} input_combiner_t;

#define MIN_HEADROOM 6.
extern double min_headroom;

enum {
	encoding_signed_integer, encoding_unsigned_integer, encoding_floating_point,
	encoding_ms_adpcm, encoding_ima_adpcm, encoding_oki_adpcm,
	encoding_gsm_full_rate, encoding_u_law, encoding_a_law
};

extern lsx_enum_item const encodings[];

typedef enum {
	Full, Type, Rate, Channels, Samples, Duration, Duration_secs,
	Bits, Bitrate, Precision, Encoding, Annotation
} soxi_t;

extern char const * const getoptstr;

extern double soxi_total;
extern size_t soxi_file_count;


void add_eff_chain(void);
int add_effect(sox_effects_chain_t * chain, sox_effect_t * effp, sox_signalinfo_t * in, sox_signalinfo_t const * out, int * guard);
void add_effects(sox_effects_chain_t *chain);
int add_file(file_t const * const opts, char const * const filename);
void adjust_volume(int delta);
int advance_eff_chain(void);
void atexit_cleanup(void);
void auto_effect(sox_effects_chain_t *chain, char const *name, int argc, char *argv[], sox_signalinfo_t *signal, int * guard);
//void auto_effect(sox_effects_chain_t *, char const *, int, char **, sox_signalinfo_t *, int *);
void balance_input(sox_sample_t * buf, size_t ws, file_t * f);
void calculate_combiner_signal_parameters(void);
void calculate_output_signal_parameters(void);
sox_bool can_segue(size_t i);
void cleanup(void);
sox_bool cmp_comment_text(char const * c1, char const * c2);
int combiner_drain(sox_effect_t *effp, sox_sample_t * obuf, size_t * osamp);
int combiner_start(sox_effect_t *effp);
int combiner_stop(sox_effect_t *effp);
void create_user_effects(void);
void delete_eff_chains(void);
char const * device_name(char const * const type);
void display_file_info(sox_format_t * ft, file_t * f, sox_bool full);
void display_SoX_version(FILE * file);
void display_status(sox_bool all_done);
void display_supported_effects(void);
void display_supported_formats(void);
int enum_option(char const * arg, int option_index, lsx_enum_item const * items);
char *fndup_with_count(const char *filename, size_t count);
void free_eff_chain(void);
char * headroom(void);
void init_file(file_t * f);
sox_effect_handler_t const * input_combiner_effect_fn(void);
sox_bool is_pseudo_effect(const char *s);
#ifdef HAVE_TERMIOS_H
int kbhit(void);
#endif
void open_output_file(void);
int opt_index(int val);
void optimize_trim(void);
int ostart(sox_effect_t *effp);
sox_effect_handler_t const * output_effect_fn(void);
int output_flow(sox_effect_t *effp, sox_sample_t const * ibuf, sox_sample_t * obuf, size_t * isamp, size_t * osamp);
void output_message(unsigned level, const char *filename, const char *fmt, va_list ap);
sox_bool overwrite_permitted(char const * filename);
void parse_effects(int argc, char ** argv);
char parse_gopts_and_fopts(file_t * f);
void parse_options_and_filenames(int argc, char **argv);
void play_file_info(sox_format_t * ft, file_t * f, sox_bool full);
int process(void);
void progress_to_next_input_file(file_t * f, sox_effect_t * effp);
void read_comment_file(sox_comments_t * comments, char const * const filename);
void read_user_effects(char const *filename);
void report_file_info(file_t * f);
void set_combiner_and_output_encoding_parameters(void);
char const * set_default_device(file_t * f);
void set_replay_gain(sox_comments_t comments, file_t * f);
void sigint(int s);
sox_bool since(struct timeval * then, double secs, sox_bool always_reset);
char const * size_and_bitrate(sox_format_t * ft, char const * * text);
size_t sox_read_wide(sox_format_t * ft, sox_sample_t * buf, size_t max);
int soxi(int argc, char * const * argv);
void soxi_usage(int return_code);
int soxi1(soxi_t const * type, char const * filename);
int soxmain(int argc, char **argv);
char const * str_time(double seconds);
int strcmp_p(const void *p1, const void *p2);
char * * strtoargv(char * s, int * argc);
uint64_t total_clips(void);
char const * try_device(char const * name);
int update_status(sox_bool all_done, void * client_data);
void usage(char const * message);
void usage_effect(char const * name);
void usage_format(char const * name);
void usage_format1(sox_format_handler_t const * f);
char const * vu(unsigned channel);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // SOX_MAIN_H