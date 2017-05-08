/*
 * Copyright 2014-2017, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * file.h -- internal definitions for file module
 */

#ifndef NVML_FILE_H
#define NVML_FILE_H 1
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef _WIN32
#define NAME_MAX MAX_PATH
#endif

struct file_info {
	char filename[NAME_MAX + 1];
	int is_dir;
};

struct dir_handle {
	const char *path;
#ifdef _WIN32
	HANDLE handle;
	char *_file;
#else
	DIR *dirp;
#endif
};

int util_file_dir_open(struct dir_handle *a, const char *path);
int util_file_dir_next(struct dir_handle *a, struct file_info *info);
int util_file_dir_close(struct dir_handle *a);
int util_file_dir_remove(const char *path);
int util_file_is_device_dax(const char *path);
int util_fd_is_device_dax(int fd);
ssize_t util_file_get_size(const char *path);
size_t util_file_device_dax_alignment(const char *path);
void *util_file_map_whole(const char *path);
int util_file_zero(const char *path, off_t off, size_t len);
ssize_t util_file_pread(const char *path, void *buffer, size_t size,
	off_t offset);
ssize_t util_file_pwrite(const char *path, const void *buffer, size_t size,
	off_t offset);

int util_tmpfile(const char *dir, const char *templ);
int util_is_absolute_path(const char *path);

int util_file_create(const char *path, size_t size, size_t minsize);
int util_file_open(const char *path, size_t *size, size_t minsize, int flags);
int util_unlink(const char *path);
int util_unlink_flock(const char *path);
int util_file_mkdir(const char *path, mode_t mode);

#ifndef _WIN32
#define util_read	read
#define util_write	write
#else
/* XXX - consider adding an assertion on (count <= UINT_MAX) */
#define util_read(fd, buf, count)	read(fd, buf, (unsigned)(count))
#define util_write(fd, buf, count)	write(fd, buf, (unsigned)(count))
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#endif
#ifdef __cplusplus
}
#endif
#endif
