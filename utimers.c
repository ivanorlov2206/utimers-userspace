#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "utimers.h"

#define NANO 1000000000ULL

#ifdef DEBUG
#define LOG(fmt, ...)								\
	do {									\
		printf("Userspace-driven timers: " fmt "\n", ##__VA_ARGS__);	\
	} while (0)
#else
#define LOG(fmt, ...)
#endif

static int open_timer_device(void)
{
	int timer_fd;

	timer_fd = open("/dev/snd/timer", O_RDWR, O_CLOEXEC);
	if (timer_fd < 0) {
		LOG("Can't open the timer device (/dev/snd/timer).");
		return -1;
	}

	return timer_fd;
}

struct snd_timer_uinfo *create_utimer(uint64_t resolution)
{
	int timer_fd;
	struct snd_timer_uinfo *utimer_info;

	if (resolution == 0) {
		LOG("Resolution can't be 0!");
		return NULL;
	}

	utimer_info = calloc(1, sizeof(*utimer_info));
	if (!utimer_info) {
		LOG("Can't allocate utimer_info");
		return NULL;
	}

	utimer_info->resolution = resolution;

	timer_fd = open_timer_device();
	if (timer_fd < 0)
		return NULL;

	if (ioctl(timer_fd, SNDRV_TIMER_IOCTL_CREATE, utimer_info)) {
		LOG("Can't create the timer");
		close(timer_fd);
		return NULL;
	}

	close(timer_fd);

	return utimer_info;
}

struct snd_timer_uinfo *create_utimer_from_pcm_info(uint64_t frame_rate, uint64_t period_size)
{
	uint64_t resolution;

	if (frame_rate == 0) {
		LOG("Frame rate can't be 0");
		return NULL;
	}
	
	if (period_size == 0) {
		LOG("Period size can't be 0");
		return NULL;
	}

	if (NANO / frame_rate > UINT64_MAX / period_size) {
		LOG("Resolution will overflow, aborting");
		return NULL;
	}

	resolution = NANO / frame_rate * period_size;
	return create_utimer(resolution);
}

void trigger_utimer(struct snd_timer_uinfo *utimer)
{
	if (utimer == NULL) {
		LOG("utimer is null!");
		return;
	}
	ioctl(utimer->fd, SNDRV_TIMER_IOCTL_TRIGGER, NULL);
}

void free_utimer(struct snd_timer_uinfo *utimer)
{
	if (utimer == NULL)
		return;

	close(utimer->fd);
	free(utimer);
}
