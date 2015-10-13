#ifndef __VSE_DEV_H_INCLUDED__
#define __VSE_DEV_H_INCLUDED__

#include <linux/ioctl.h>

#define MAJOR_NUM 60

#define VSE_IOCTL_SET_VM_ID _IOR(MAJOR_NUM, 0, char *)
#define VSE_IOCTL_SET_DL_INSRC _IOR(MAJOR_NUM, 1, char *)
#define VSE_IOCTL_SET_VNI _IOR(MAJOR_NUM, 3, char *)
#define VSE_IOCTL_SET_DL_SRC _IOR(MAJOR_NUM, 4, char *)
#define VSE_IOCTL_SET_DL_DST _IOR(MAJOR_NUM, 5, char *)
#define VSE_IOCTL_INIT_WILDCARDS _IOR(MAJOR_NUM, 6, char *)
#define VSE_IOCTL_SET_ENTRY _IOR(MAJOR_NUM, 7, char *)
#define VSE_IOCTL_SET_IRQ _IOR(MAJOR_NUM, 8, char *)
#define VSE_IOCTL_SET_DL_INDST_1 _IOR(MAJOR_NUM, 9, char *)
#define VSE_IOCTL_SET_DL_INDST_2 _IOR(MAJOR_NUM, 10, char *)
#define VSE_IOCTL_SET_DL_INDST_3 _IOR(MAJOR_NUM, 11, char *)
#define VSE_IOCTL_SET_DL_INDST_4 _IOR(MAJOR_NUM, 12, char *)
#define VSE_IOCTL_SET_DL_INDST_5 _IOR(MAJOR_NUM, 13, char *)
#define VSE_IOCTL_SET_DL_INDST_6 _IOR(MAJOR_NUM, 14, char *)
#define VSE_IOCTL_SET_DL_DST_1 _IOR(MAJOR_NUM, 15, char *)
#define VSE_IOCTL_SET_DL_DST_2 _IOR(MAJOR_NUM, 16, char *)
#define VSE_IOCTL_SET_DL_DST_3 _IOR(MAJOR_NUM, 17, char *)
#define VSE_IOCTL_SET_DL_DST_4 _IOR(MAJOR_NUM, 18, char *)
#define VSE_IOCTL_SET_DL_DST_5 _IOR(MAJOR_NUM, 19, char *)
#define VSE_IOCTL_SET_DL_DST_6 _IOR(MAJOR_NUM, 20, char *)

#define DEVICE_FILE_NAME "/dev/vse_dev"
#define DEVICE_NAME "vse_dev"

extern struct file_operations Fops;
extern char *message_ptr;
extern char message[100];
#endif /* __VSE_DEV_H_INCLUDED__ */
