#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define KEYBOARD "/dev/input/by-path/pci-0000:00:14.0-usb-0:7:1.0-event-kbd"
#define FILE_PATH "death_counter"

static const char *const evval[3] = {
	"RELEASED",
	"PRESSED ",
	"REPEATED"
};

int main(void)
{
	int counter;

	FILE *file = fopen(FILE_PATH, "r");
	if (file == NULL) {
		// perror("fopen");
		counter = 0;
	} else {
       fread(&counter, sizeof(counter), 1, file);
	   fclose(file);
	}

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

		if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
			printf("%s 0x%04x (%d)\n", evval[ev.value], 
					(int)ev.code, (int)ev.code);
	}

	fflush(stdout);
	fprintf(stderr, "%s.\n", strerror(errno));
	return EXIT_FAILURE;
}
