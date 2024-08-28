#define _GNU_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define KEYBOARD "/dev/input/event2"

#define USAGE_MESSAGE "Usage: %s <counter_file.txt>\n" \
	"AUGHTUNG!! txt - suffix is reuired for OBS!\n"

void update_file(int counter, char *argv)
{
	FILE *file = fopen(argv, "w");
	printf("Counter: %d.\n", counter);
	fprintf(file, "%d", counter);
	fclose(file);
}

void check_file(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, USAGE_MESSAGE, argv[0]);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	check_file(argc, argv);

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

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		counter = 0;
		update_file(counter, argv[1]);
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
				update_file(counter, argv[1]);
			} else if (ev.code == 74) {
				--counter;
				update_file(counter, argv[1]);
			}
		}
	}

	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));
	return EXIT_FAILURE;
}
