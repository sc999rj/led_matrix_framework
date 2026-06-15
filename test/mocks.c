#include <string.h>
#include <termios.h>
#include <unistd.h>

int mock_open_result = 10;
int mock_tcgetattr_result = 0;
int mock_tcsetattr_result = 0;
int mock_flush_result = 0;
int mock_close_called = 0;

ssize_t mock_write_result = 0;
ssize_t mock_read_result = 0;

unsigned char mock_read_buffer[32];
unsigned char last_write[64];
size_t last_write_size = 0;

int __wrap_open(const char *path, int flags)
{
    (void)path;
    (void)flags;
    return mock_open_result;
}

int __wrap_close(int fd)
{
    mock_close_called = fd;
    return 0;
}

int __wrap_tcgetattr(int fd, struct termios *t)
{
    (void)fd;
    memset(t, 0, sizeof(*t));
    return mock_tcgetattr_result;
}

int __wrap_tcsetattr(int fd, int action, const struct termios *t)
{
    (void)fd;
    (void)action;
    (void)t;
    return mock_tcsetattr_result;
}

int __wrap_tcflush(int fd, int queue)
{
    (void)fd;
    (void)queue;
    return mock_flush_result;
}

ssize_t __wrap_write(int fd, const void *buf, size_t count)
{
    (void)fd;

    memcpy(last_write, buf, count);
    last_write_size = count;

    return mock_write_result;
}

ssize_t __wrap_read(int fd, void *buf, size_t count)
{
    (void)fd;

    memcpy(buf, mock_read_buffer, count);

    return mock_read_result;
}