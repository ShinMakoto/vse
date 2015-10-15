#include <linux/if_ether.h>

struct vse_match
{
	__u32 wildcards;
	__u8 vm_id;
	__u8 dl_dst[ETH_ALEN];
	__u8 dl_src[ETH_ALEN];
	__u8 nw_src[16];
	__u8 nw_dst[16];
	__u8 nw_tos;
	__u8 nw_proto;
	__u32 vx_vni;
	__u8 dl_indst[ETH_ALEN];
	__u8 dl_insrc[ETH_ALEN];
};
