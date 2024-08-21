#ifndef __UTIMERS_HDR
#define __UTIMERS_HDR
#include <sound/asound.h>
#include <stdint.h>

struct snd_timer_uinfo *create_utimer(uint64_t resolution);
struct snd_timer_uinfo *create_utimer_from_pcm_info(uint64_t frame_rate, uint64_t period_size);
void free_utimer(struct snd_timer_uinfo *utimer);
void trigger_utimer(struct snd_timer_uinfo *utimer);
#endif
