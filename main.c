#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define KEYBOARD "/dev/input/by-path/pci-0000:00:14.0-usb-0:7:1.0-event-kbd"
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
	char score[7];
	int counter;

	FILE *file = fopen(FILE_PATH, "r");
	if (file == NULL) {
		// perror("fopen");
		counter = 0;
	} else {
       fread(score, sizeof(counter), 1, file);
	   counter = atoi(score);
	   fclose(file);
	}

	printf("Current count: %d.\n", counter);

	const char *dev = KEYBOARD;
	struct input_event ev;
	ssize_t n;
	int fd;
	fd = open(dev, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
		return EXIT_FAILURE;
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
