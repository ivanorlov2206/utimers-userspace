#include <stdio.h>
#include <unistd.h>
#include "../utimers.h"

int main(void)
{
	struct snd_timer_uinfo *utimer;
	size_t i;

	utimer = create_utimer_from_pcm_info(8000, 4410);
	if (!utimer) {
		perror("Can't create utimer");
		exit(EXIT_FAILURE);
	}

	printf("Timer id: %d\n", utimer->id);
	printf("Timer file descriptor: %d\n", utimer->fd);
	printf("Timer id for snd-aloop: -1.%d.%d\n", SNDRV_TIMER_GLOBAL_UDRIVEN, utimer->id);
	printf("Let's tick 10 times\n");

	for (i = 0; i < 10; i++) {
		printf(i % 2 ? "Tock\n" : "Tick\n");
		trigger_utimer(utimer);
		sleep(1);
	}

	free_utimer(utimer);

	return EXIT_SUCCESS;
}
