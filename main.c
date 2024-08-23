#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define KEYBOARD "/dev/input/event2"
#define FILE_PATH "death_counter.txt"

void update_file(int counter)
{
	FILE *file = fopen(FILE_PATH, "w");
	printf("Counter: %d.\n", counter);
	fprintf(file, "%d", counter);
	fclose(file);
}

int main(void)
{
	uid_t ruid = getuid();
	uid_t rgid = getgid();

	const char *dev = KEYBOARD;
	struct input_event ev;
	ssize_t n;
	int fd;
	fd = open(dev, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
		return EXIT_FAILURE;
	}

	if (seteuid(ruid) < 0) {
		fprintf(stderr, 
				"WARNING: Could not set Effective UID: %s\n", 
				strerror(errno));
	}

	if (setegid(rgid) < 0) {
		fprintf(stderr, 
				"WARNING: Could not set Effective GID: %s\n", 
				strerror(errno));
	}

	char score[7];
	int counter;

	FILE *file = fopen(FILE_PATH, "r");
	if (file == NULL) {
		counter = 0;
		update_file(counter);
	} else {
		fread(score, sizeof(counter), 1, file);
		counter = atoi(score);
		printf("Counter: %d.\n", counter);
		fclose(file);
	}

	while (1) {
		n = read(fd, &ev, sizeof ev);
		if (n == (ssize_t)-1) {
			if (errno == EINTR)
				continue;
			else
				break;
		} else
			if (n != sizeof ev) {
				errno = EIO;
				break;
			}

		if (ev.type == EV_KEY && ev.value == 0) {
			if (ev.code == 78) {
				++counter;
				update_file(counter);
			} else if (ev.code == 74) {
				--counter;
				update_file(counter);
			}
		}
	}

	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));
	return EXIT_FAILURE;
}
