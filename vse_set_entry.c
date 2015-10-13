#include "vse_dev.h"
#include "vse_dev_entry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>

void vse_ioctl_init_wildcards(int fd)
{
	int ret_val;
	
	ret_val = ioctl(fd, VSE_IOCTL_INIT_WILDCARDS, NULL);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void vse_ioctl_set_vni(int fd, unsigned int *vni)
{
	int ret_val;
	
	ret_val = ioctl(fd, VSE_IOCTL_SET_VNI, vni);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void vse_ioctl_set_irq(int fd, unsigned int *irq)
{
	int ret_val;
	
	ret_val = ioctl(fd, VSE_IOCTL_SET_IRQ, irq);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void vse_ioctl_set_dl_indst(int fd, unsigned char *dl)
{
	int ret_val;
	
	ioctl(fd, VSE_IOCTL_SET_DL_INDST_1, &dl[0]);
	ioctl(fd, VSE_IOCTL_SET_DL_INDST_2, &dl[1]);
	ioctl(fd, VSE_IOCTL_SET_DL_INDST_3, &dl[2]);
	ioctl(fd, VSE_IOCTL_SET_DL_INDST_4, &dl[3]);
	ioctl(fd, VSE_IOCTL_SET_DL_INDST_5, &dl[4]);
	ret_val = ioctl(fd, VSE_IOCTL_SET_DL_INDST_6, &dl[5]);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void vse_ioctl_set_dl_dst(int fd, unsigned char *dl)
{
	int ret_val;
	
	ioctl(fd, VSE_IOCTL_SET_DL_DST_1, &dl[0]);
	ioctl(fd, VSE_IOCTL_SET_DL_DST_2, &dl[1]);
	ioctl(fd, VSE_IOCTL_SET_DL_DST_3, &dl[2]);
	ioctl(fd, VSE_IOCTL_SET_DL_DST_4, &dl[3]);
	ioctl(fd, VSE_IOCTL_SET_DL_DST_5, &dl[4]);
	ret_val = ioctl(fd, VSE_IOCTL_SET_DL_DST_6, &dl[5]);
	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
}

void vse_ioctl_set_entry(int fd)
{
	int ret_val;

	ret_val = ioctl(fd, VSE_IOCTL_SET_ENTRY, NULL);

}

int main(int argc, char *argv[])
{
	int fd, i;
	unsigned int vni;
	unsigned int irq;
	unsigned char dl[ETH_ALEN];
	
	if (argc < 2) {
	out_usage:
		fprintf(stderr, "Usage %s [--vni vni] [--dl_indst vm_dst_mac] "
			"[--irq CPU_core]\n"
			, argv[0]);
		exit(-1);
	}
	
	fd = open(DEVICE_FILE_NAME, 0);
	if (fd < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	vse_ioctl_init_wildcards(fd);

	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i], "--vni") == 0)
		{
			i++;
			vni = atoi(argv[i]);
			if (vni < 0) {
				fprintf(stderr, "Invalid number of vni: %d\n", vni);
				close(fd);
				goto out_usage;
			}
			vse_ioctl_set_vni(fd, &vni);
		}
		if (strcmp(argv[i], "--irq") == 0)
		{
			i++;
			irq = atoi(argv[i]);
			if (irq < 0) {
				fprintf(stderr, "Invalid number of irq: %d\n", vni);
				close(fd);
				goto out_usage;
			}
			vse_ioctl_set_irq(fd, &irq);
		}		
		if (strcmp(argv[i], "--dl_indst") == 0)
		{
			i++;
			sscanf(argv[i], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			       &dl[0], &dl[1], &dl[2], &dl[3], &dl[4], &dl[5]);
			printf("%x:%x:%x:%x:%x:%x\n", dl[0],dl[1], dl[2],dl[3],dl[4],dl[5]);
			vse_ioctl_set_dl_indst(fd, dl);
		}
		if (strcmp(argv[i], "--dl_dst") == 0)
		{
			i++;
			sscanf(argv[i], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
			       &dl[0], &dl[1], &dl[2], &dl[3], &dl[4], &dl[5]);
			printf("%x:%x:%x:%x:%x:%x\n", dl[0],dl[1], dl[2],dl[3],dl[4],dl[5]);
			vse_ioctl_set_dl_dst(fd, dl);
		}		
	}
	
	vse_ioctl_set_entry(fd);
	close(fd);
	return 0;
}
