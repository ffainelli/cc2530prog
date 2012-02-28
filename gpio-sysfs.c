/*
 * Linux GPIO backend using sysfs
 *
 * Copyright (C) 2010, Florian Fainelli <f.fainelli@gmail.com>
 *
 * This file is part of "cc2530prog", this file is distributed under
 * a 2-clause BSD license, see LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "gpio.h"

#define SYSFS_GPIO	"/sys/class/gpio"

int read_file(const char *path, char *str, size_t size)
{
	int fd;
	int ret;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror(path);
		return -1;
	}

	ret = read(fd, str, size - 1);
	if (ret < 0) {
		perror("read");
		close(fd);
		return -1;
	}

	close(fd);
	str[ret] = '\0';

	return 0;
}

int write_file(const char *path, const char *str)
{
	int fd;
	int ret;

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		perror(path);
		return -1;
	}

	ret = write(fd, str, strlen(str));
	if (ret < 0) {
		if (errno == EBUSY)
			ret = 0;
		else
			perror("write");
	}

	close(fd);

	return ret < 0 ? -1 : 0;
}


int
gpio_export(int n)
{
	char buf[16];

	snprintf(buf, sizeof (buf), "%d", n);

	return write_file(SYSFS_GPIO "/export", buf);
}

int gpio_unexport(int n)
{
	char buf[16];

	snprintf(buf, sizeof(buf), "%d", n);

	return write_file(SYSFS_GPIO "/unexport", buf);
}

int
gpio_set_direction(int n, enum gpio_direction direction)
{
	static const char *str[] = {
		[GPIO_DIRECTION_IN]	= "in",
		[GPIO_DIRECTION_OUT]	= "out",
		[GPIO_DIRECTION_HIGH]	= "high",
	};
	char path[128];

	snprintf(path, sizeof (path), SYSFS_GPIO "/gpio%d/direction", n);

	return write_file(path, str[direction]);
}

int
gpio_get_value(int n, bool *value)
{
	char buf[128];

	snprintf(buf, sizeof (buf), SYSFS_GPIO "/gpio%d/value", n);

	if (read_file(buf, buf, sizeof (buf)) < 0)
		return -1;

	*value = (*buf != '0');

	return 0;
}

int
gpio_set_value(int n, bool value)
{
	char path[128];

	snprintf(path, sizeof (path), SYSFS_GPIO "/gpio%d/value", n);

	return write_file(path, value ? "1" : "0");
}
