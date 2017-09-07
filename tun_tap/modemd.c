#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <math.h>

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/ioctl.h>
#include <net/route.h>
#include <netinet/in.h>
#include <pthread.h>
#include "mac.h"

#define DEBUG 1

#define IP(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | d)

#define INTERFACE_NAME "adi_radio"
#define INTERFACE_ADDRESS htonl(IP(192, 168, 23, 1))
#define INTERFACE_NETMASK htonl(IP(255, 255, 255, 0))

#define MTU                             1500
#define HEADER_DATA_SIZE        8
#define HEADER_FRAME_SIZE       16
#define CRC_SIZE                        8
#define PADDING_SIZE            12
#define TX_BUF_SIZE                     HEADER_DATA_SIZE + HEADER_FRAME_SIZE + MTU + PADDING_SIZE
#define RX_BUF_SIZE                     HEADER_FRAME_SIZE + MTU + PADDING_SIZE + CRC_SIZE

static char     tx_buffer[TX_BUF_SIZE];
static char     rx_buffer[RX_BUF_SIZE];
static struct   pollfd pfd[2];

static int tun_alloc(const char *name, int flags)
{
        static const char *clonedev = "/dev/net/tap";
        struct ifreq ifr;
        int fd, ret;

        fd = open(clonedev, O_RDWR);
        if (fd < 0)
                return -errno;

        memset(&ifr, 0, sizeof(ifr));

        ifr.ifr_flags = flags;
        strncpy(ifr.ifr_name, name, IFNAMSIZ);

        ret = ioctl(fd, TUNSETIFF, &ifr);
        if (ret < 0) {
                close(fd);
                return -errno;
        }

        return fd;
}

static int set_ip(const char *name, in_addr_t addr)
{
        struct ifreq ifr;
        struct sockaddr_in sin;
        int ret, fd;

        memset(&ifr, 0, sizeof(struct ifreq));
        memset(&sin, 0, sizeof(struct sockaddr_in));
        strncpy(ifr.ifr_name, name, IFNAMSIZ);
        sin.sin_addr.s_addr = addr;
        sin.sin_family = AF_INET;
        memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr_in));

        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
                return -errno;
        ret = ioctl(fd, SIOCSIFADDR, &ifr);
        if (ret < 0) {
                ret = -errno;
                goto err;
        }

        ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
        if (ret < 0) {
                ret = -errno;
                goto err;
        }
        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        ioctl(fd, SIOCSIFFLAGS, &ifr);
        if (ret < 0)
                ret = -errno;
err:
        close(fd);

        if (ret < 0)
                return ret;

        return 0;
}

static int add_route(const char *dev, in_addr_t host_addr, in_addr_t mask)
{
        struct sockaddr_in *addr;
        struct rtentry route;
        int fd, ret;

        memset(&route, 0, sizeof(route));

        /* gateway IP */
        addr = (struct sockaddr_in *)&route.rt_gateway;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = INADDR_ANY;

        /* target IP */
        addr = (struct sockaddr_in *)&route.rt_dst;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = host_addr & mask;

        /* subnet mask */
        addr = (struct sockaddr_in *)&route.rt_genmask;
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = mask;

        route.rt_flags = RTF_UP;
        route.rt_metric = 0;
        route.rt_dev = (char *)dev;

        fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (fd < 0)
                return -errno;
        ret = ioctl(fd, SIOCADDRT, &route);
        if (ret < 0)
                ret = -errno;
        close(fd);

        if (ret < 0)
                return ret;

        return 0;
}

static int setup_signal_handler(void)
{
        sigset_t mask;
        int ret;

        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigaddset(&mask, SIGPIPE);
        sigaddset(&mask, SIGHUP);
        sigaddset(&mask, SIGTERM);

        ret = sigprocmask(SIG_BLOCK, &mask, NULL);
        if (ret) {
                perror("Failed to setup signal mask");
                return -errno;
        }

        ret = signalfd(-1, &mask, 0);
        if (ret < 0) {
                perror("Failed to create signalfd");
                return -errno;
        }

        return ret;
}

static int receive_data(int fd)
{
        int ret;

        do {
                ret = read(fd, &tx_buffer[HEADER_DATA_SIZE + HEADER_FRAME_SIZE], MTU);
        } while (ret == -1 && errno == EAGAIN);

        if (ret < 0) {
                perror("Failed to receive data");
                return -errno;
        }

#if(DEBUG >= 1)
        printf("ETH: Received %d bytes of data....\n", ret);
#endif

#if(DEBUG >= 2)
        for(int i = 16; i < ret + 16; i++)
                printf("%x, ", tx_buffer[i]);
        printf("\n");
#endif

        *(uint64_t*)(&tx_buffer[0]) = MTU + PADDING_SIZE + HEADER_FRAME_SIZE;
        *(uint64_t*)(&tx_buffer[HEADER_DATA_SIZE]) = ret;
        *(uint64_t*)(&tx_buffer[HEADER_FRAME_SIZE]) = ret;
        tx_write((uint64_t*)tx_buffer, TX_BUF_SIZE, 0);

#if(DEBUG >= 2)
        for(int i = 0; i < ret + HEADER_FRAME_SIZE + HEADER_DATA_SIZE; i++)
                printf("%x, ", tx_buffer[i]);
        printf("\n");
#endif

#if(DEBUG >= 1)
        printf("ETH: Received Done\n");
#endif

        return 0;
}

static int send_data(int fd, unsigned char* buf, size_t len)
{
        int ret;
        int i;

#if(DEBUG >= 1)
        printf("RADIO: Sent %d bytes of data\n", len);
#endif

#if(DEBUG >= 2)
        for(i = 0; i < len; i++)
                printf("%x, ", buf[i]);
        printf("\n");
#endif

        do {
                ret = write(fd, buf, len);
        } while (ret == -1 && errno == EAGAIN);

        if (ret < 0) {
                perror("RADIO: Failed to send data");
                return -errno;
        }

#if(DEBUG >= 1)
        printf("RADIO: Sent Done\n");
#endif

        return 0;
}

void *rx_thread_fnc(void* ptr)
{
        int i = 0;

#if(DEBUG >= 1)
        printf("Running Rx thread\n");
#endif

        while(1)
        {
                rx_read((uint64_t*)rx_buffer, RX_BUF_SIZE);

        #if(DEBUG >= 2)
                for(i = 0; i < HEADER_FRAME_SIZE; i++)
                        printf("%x, ", rx_buffer[i]);
                printf("\n");
        #endif
                send_data(pfd[1].fd, (unsigned char*)&rx_buffer[HEADER_FRAME_SIZE], *(uint64_t*)&rx_buffer[HEADER_FRAME_SIZE/2]);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        int ret;
        pthread_t rx_thread;

        printf("Running TUN/TAP daemon...\n");

        ret = setup_signal_handler();
        if (ret < 0)
                return 1;

        pfd[0].fd = ret;
        pfd[0].events = POLLIN;

        ret = tun_alloc(INTERFACE_NAME, IFF_TAP | IFF_NO_PI);
        if (ret < 0) {
                perror("Failed to create TUN device");
                return 1;
        }

        pfd[1].fd = ret;
        pfd[1].events = POLLIN;

        ret = set_ip(INTERFACE_NAME, INTERFACE_ADDRESS);
        if (ret < 0) {
                perror("Failed to set IP address on TUN device");
                return 1;
        }

        ret = add_route(INTERFACE_NAME, INTERFACE_ADDRESS, INTERFACE_NETMASK);
        if (ret < 0) {
                perror("Failed to create route");
                return 1;
        }

        ret = modem_reset();
        if(ret)
    {
        perror("Failed to reset Modem");
        return 1;
    }

        ret = tx_setup();
        if(ret)
    {
        perror("Failed to setup Tx");
        return 1;
    }

        ret = rx_setup();
        if(ret)
    {
        perror("Failed to setup Rx");
        return 1;
    }

        ret = pthread_create(&rx_thread, NULL, rx_thread_fnc, NULL);
    if(ret)
    {
        perror("Error - pthread_create");
        return 1;
    }

        while (1) {
                do {
                        ret = poll(pfd, 2, -1);
                } while (ret == -1 && errno == EINTR);

                /* If any signals are pending cleanup and exit */
                if (pfd[0].revents & POLLIN)
                        break;

                if (pfd[1].revents & POLLIN) {
                        ret = receive_data(pfd[1].fd);
                        if (ret < 0)
                                break;
                        usleep(10000);
                }
        }

        pthread_join(rx_thread, NULL);

        close(pfd[1].fd);
        close(pfd[0].fd);

        rx_close();
        tx_close();

        printf("Exiting TUN/TAP daemon\n");

        return 0;
}
