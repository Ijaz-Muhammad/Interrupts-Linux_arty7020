#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

struct irq_data {
    int fd0;
    int fd1;
};

int TIMEcount;
int INTcount0;
int INTcount1;

void wait4INT0(int fd)
{
    unsigned int irqcount;
    unsigned int enable = 1;
    read(fd, &irqcount, sizeof(irqcount));
    INTcount0++;
    printf("Interrupt 0 fired %d time(GIC 61 -> Linux 29)\n",INTcount0);
    write(fd, &enable, sizeof(enable)); // re-arm
}
void wait4INT1(int fd)
{
    unsigned int irqcount;
    unsigned int enable = 1;
    read(fd, &irqcount, sizeof(irqcount));
    INTcount1++;
    printf("Interrupt 1 fired %d time(GIC 62 -> Linux 30)\n",INTcount1);
    write(fd, &enable, sizeof(enable)); // re-arm
}

void* irq_thread(void* arg) {

	struct irq_data *data = (struct irq_data*)arg;

    struct pollfd fds[2];
    fds[0].fd = data->fd0; fds[0].events = POLLIN;
    fds[1].fd = data->fd1; fds[1].events = POLLIN;

    while (1) {
        int ret = poll(fds, 2, -1);
        if (ret > 0) {
            if (fds[0].revents & POLLIN) {
            	wait4INT0(fds[0].fd);
            }
            if (fds[1].revents & POLLIN) {
            	wait4INT1(fds[1].fd);
            }
        }
    }
}

int main()
{
    printf("START interrupt software\n");

    struct irq_data data;

     data.fd0 = open("/dev/uio0", O_RDWR);
     data.fd1 = open("/dev/uio1", O_RDWR);
    if (data.fd0 < 0 || data.fd1 < 0) {
        perror("open");
        return -1;
    }

    printf("arm interrupts\n");
    uint32_t enable = 1;
    write(data.fd0, &enable, sizeof(enable));
    write(data.fd1, &enable, sizeof(enable));

    pthread_t irq_tid;
    pthread_create(&irq_tid, NULL, irq_thread, &data);

    TIMEcount = 0;
    INTcount0 = 0;
    INTcount1 = 0;

    printf("Wait for interrupts\n");
    while (1) {
    	sleep(1);
    	TIMEcount++;
    	printf("Count %d Seconds\n",TIMEcount);
    }


    close(data.fd0);
    close(data.fd1);
    return 0;
}
