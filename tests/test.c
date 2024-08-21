#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "../utimers.h"

#define ASSERT(cond)							\
	do {								\
		if (!(cond)) {						\
			printf("Condition " # cond " is not met\n");	\
			exit(EXIT_FAILURE);				\
		}							\
	} while (0);

static void test_timers_valid(void)
{
	struct snd_timer_uinfo *valid_timer;

	valid_timer = create_utimer(10000000ULL);
	ASSERT(valid_timer != NULL);
	ASSERT(valid_timer->fd >= 0);
	ASSERT(valid_timer->id >= 0);
	free_utimer(valid_timer);
}

static void test_timers_invalid(void)
{
	struct snd_timer_uinfo *utimer;

	utimer = create_utimer(0);
	ASSERT(utimer == NULL);
}

static void test_timers_from_pcm_info(void)
{
	struct snd_timer_uinfo *utimer;

	utimer = create_utimer_from_pcm_info(8000, 4410);
	ASSERT(utimer != NULL);
	ASSERT(utimer->resolution == 551250000);
	ASSERT(utimer->fd >= 0);
	ASSERT(utimer->id >= 0);
	free_utimer(utimer);
}

static const struct {
	uint64_t frame_rate;
	uint64_t period_size;
} invalid_data[] = {
	{ .frame_rate = 0, .period_size = 1000 },
	{ .frame_rate = 1000, .period_size = 0 },
	{ .frame_rate = 1000000001ULL, .period_size = 4410 },
	{ .frame_rate = 1, .period_size = UINT64_MAX / 1000000000ULL + 1 },
};

static void test_timers_from_pcm_info_invalid(void)
{
	struct snd_timer_uinfo *utimer;
	size_t i;

	for (i = 0; i < sizeof(invalid_data) / sizeof(invalid_data[0]); i++) {
		utimer = create_utimer_from_pcm_info(invalid_data[i].frame_rate,
						     invalid_data[i].period_size);
		ASSERT(utimer == NULL);
	}
}

int main(void)
{
	if (geteuid()) {
		printf("You should run this test as root\n");
		return EXIT_FAILURE;
	}

	test_timers_valid();
	test_timers_invalid();
	test_timers_from_pcm_info();
	test_timers_from_pcm_info_invalid();

	printf("All of the tests passed!\n");
	return EXIT_SUCCESS;
}
