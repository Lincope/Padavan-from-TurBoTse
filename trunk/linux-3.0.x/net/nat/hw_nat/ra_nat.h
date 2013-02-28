/*
    Module Name:
    ra_nat.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Steven Liu  2006-10-06      Initial version
*/

#ifndef _RA_NAT_WANTED
#define _RA_NAT_WANTED

#include "foe_fdb.h"
#include <linux/ip.h>
#include <linux/ipv6.h>

#define HW_NAT_MODULE_VER "v2.50.4"

/*
 * TYPEDEFS AND STRUCTURES
 */
enum DstPort {
	DP_RA0 = 11,
#if defined (CONFIG_RT2860V2_AP_MBSS)
	DP_RA1 = 12,
	DP_RA2 = 13,
	DP_RA3 = 14,
	DP_RA4 = 15,
	DP_RA5 = 16,
	DP_RA6 = 17,
	DP_RA7 = 18,
	DP_RA8 = 19,
	DP_RA9 = 20,
	DP_RA10 = 21,
	DP_RA11 = 22,
	DP_RA12 = 23,
	DP_RA13 = 24,
	DP_RA14 = 25,
	DP_RA15 = 26,
#endif // CONFIG_RT2860V2_AP_MBSS //
#if defined (CONFIG_RT2860V2_AP_WDS)
	DP_WDS0 = 27,
	DP_WDS1 = 28,
	DP_WDS2 = 29,
	DP_WDS3 = 30,
#endif // CONFIG_RT2860V2_AP_WDS //
#if defined (CONFIG_RT2860V2_AP_APCLI)
	DP_APCLI0 = 31,
#endif // CONFIG_RT2860V2_AP_APCLI //
#if defined (CONFIG_RT2860V2_AP_MESH)
	DP_MESH0 = 32,
#endif // CONFIG_RT2860V2_AP_MESH //
	DP_RAI0 = 33,
#if defined (CONFIG_RT3090_AP_MBSS) || defined (CONFIG_RT5392_AP_MBSS) || \
    defined (CONFIG_RT3572_AP_MBSS) || defined (CONFIG_RT5572_AP_MBSS) || \
    defined (CONFIG_RT5592_AP_MBSS) || defined (CONFIG_RT3593_AP_MBSS)
	DP_RAI1 = 34,
	DP_RAI2 = 35,
	DP_RAI3 = 36,
	DP_RAI4 = 37,
	DP_RAI5 = 38,
	DP_RAI6 = 39,
	DP_RAI7 = 40,
	DP_RAI8 = 41,
	DP_RAI9 = 42,
	DP_RAI10 = 43,
	DP_RAI11 = 44,
	DP_RAI12 = 45,
	DP_RAI13 = 46,
	DP_RAI14 = 47,
	DP_RAI15 = 48,
#endif // CONFIG_RTDEV_AP_MBSS //
#if defined (CONFIG_RT3090_AP_WDS) || defined (CONFIG_RT5392_AP_WDS) || \
    defined (CONFIG_RT3572_AP_WDS) || defined (CONFIG_RT5572_AP_WDS) || \
    defined (CONFIG_RT5592_AP_WDS) || defined (CONFIG_RT3593_AP_WDS)
	DP_WDSI0 = 49,
	DP_WDSI1 = 50,
	DP_WDSI2 = 51,
	DP_WDSI3 = 52,
#endif // CONFIG_RTDEV_AP_WDS //
#if defined (CONFIG_RT3090_AP_APCLI) || defined (CONFIG_RT5392_AP_APCLI) || \
    defined (CONFIG_RT3572_AP_APCLI) || defined (CONFIG_RT5572_AP_APCLI) || \
    defined (CONFIG_RT5592_AP_APCLI) || defined (CONFIG_RT3593_AP_APCLI)
	DP_APCLII0 = 53,
#endif // CONFIG_RTDEV_AP_APCLI //
#if defined (CONFIG_RT3090_AP_MESH) || defined (CONFIG_RT5392_AP_MESH) || \
    defined (CONFIG_RT3572_AP_MESH) || defined (CONFIG_RT5572_AP_MESH) || \
    defined (CONFIG_RT5592_AP_MESH) || defined (CONFIG_RT3593_AP_MESH)
	DP_MESHI0 = 54,
#endif // CONFIG_RTDEV_AP_MESH //
	MAX_WIFI_IF_NUM = 60,
	DP_GMAC1 = 61,
	DP_GMAC2 = 62,
	DP_PCI = 63,
	MAX_IF_NUM // MAX_IF_NUM = 64 entries (act_dp length 6bits)
};

typedef struct {
	uint16_t MAGIC_TAG;
	uint32_t FOE_Entry:14;
	uint32_t FVLD:1;
	uint32_t ALG:1;
	uint32_t AI:8;
	uint32_t SP:3;
	uint32_t AIS:1;
	uint32_t RESV2:4;
}  __attribute__ ((packed)) PdmaRxDescInfo4;

typedef struct {
	//layer2 header
	uint8_t dmac[6];
	uint8_t smac[6];

	//vlan header 
	uint16_t vlan_tag;
	uint16_t vlan1_gap;
	uint16_t vlan1;
	uint16_t vlan2_gap;
	uint16_t vlan2;
	uint16_t vlan_layer;

	//pppoe header
	uint32_t pppoe_gap;
	uint16_t ppp_tag;
	uint16_t pppoe_sid;

	//layer3 header
	uint16_t eth_type;
	struct iphdr iph;
	struct ipv6hdr ip6h;

	//layer4 header
	struct tcphdr th;
	struct udphdr uh;

	uint32_t pkt_type;
	uint8_t is_mcast;

} PktParseResult;


/*
 * DEFINITIONS AND MACROS
 */
#ifndef NEXTHDR_IPIP
#define NEXTHDR_IPIP 4
#endif

/*
 *    2bytes	    4bytes 
 * +-----------+-------------------+
 * | Magic Tag | RX/TX Desc info4  |
 * +-----------+-------------------+
 * |<------FOE Flow Info---------->|
 */
#define FOE_INFO_LEN		    6
#define FOE_MAGIC_PCI		    0x7273
#define FOE_MAGIC_WLAN		    0x7274
#define FOE_MAGIC_GE		    0x7275
#define FOE_MAGIC_PPE		    0x7276

/* choose one of them to keep HNAT related information in somewhere. */
#define HNAT_USE_HEADROOM
//#define HNAT_USE_TAILROOM
//#define HNAT_USE_SKB_CB

#if defined (HNAT_USE_HEADROOM)
#define IS_SPACE_AVAILABLED(skb)    ((skb_headroom(skb) >= FOE_INFO_LEN) ? 1 : 0)
#define FOE_INFO_START_ADDR(skb)    (skb->head)

#define FOE_MAGIC_TAG(skb)	    ((PdmaRxDescInfo4 *)((skb)->head))->MAGIC_TAG
#define FOE_ENTRY_NUM(skb)	    ((PdmaRxDescInfo4 *)((skb)->head))->FOE_Entry
#define FOE_ENTRY_VALID(skb)	    ((PdmaRxDescInfo4 *)((skb)->head))->FVLD
#define FOE_ALG(skb)		    ((PdmaRxDescInfo4 *)((skb)->head))->ALG
#define FOE_AI(skb)		    ((PdmaRxDescInfo4 *)((skb)->head))->AI
#define FOE_SP(skb)		    ((PdmaRxDescInfo4 *)((skb)->head))->SP	//src_port or user priority

#elif defined (HNAT_USE_TAILROOM)

#define IS_SPACE_AVAILABLED(skb)    ((skb_tailroom(skb) >= FOE_INFO_LEN) ? 1 : 0)
#define FOE_INFO_START_ADDR(skb)    (skb->end - FOE_INFO_LEN)

#define FOE_MAGIC_TAG(skb)	    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->MAGIC_TAG
#define FOE_ENTRY_NUM(skb)	    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->FOE_Entry
#define FOE_ENTRY_VALID(skb)	    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->FVLD
#define FOE_ALG(skb)		    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->ALG
#define FOE_AI(skb)		    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->AI
#define FOE_SP(skb)		    ((PdmaRxDescInfo4 *)((skb)->end-FOE_INFO_LEN))->SP	//src_port or user priority

#elif defined (HNAT_USE_SKB_CB)
//change the position of skb_CB if necessary
#define CB_OFFSET		    32
#define IS_SPACE_AVAILABLED(skb)    1
#define FOE_INFO_START_ADDR(skb)    (skb->cb +  CB_OFFSET)

#define FOE_MAGIC_TAG(skb)	    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->MAGIC_TAG
#define FOE_ENTRY_NUM(skb)	    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->FOE_Entry
#define FOE_ENTRY_VALID(skb)	    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->FVLD
#define FOE_ALG(skb)		    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->ALG
#define FOE_AI(skb)		    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->AI
#define FOE_SP(skb)		    ((PdmaRxDescInfo4 *)((skb)->cb + CB_OFFSET))->SP	//src_port or user priority
#endif

// fast fill FoE desc field
#define DO_FILL_FOE_DESC(skb,desc)  (*(uint32_t *)(FOE_INFO_START_ADDR(skb)+2) = (uint32_t)desc)

// fast clear FoE Info (magic_tag,entry_num)
#define DO_FAST_CLEAR_FOE(skb)	    (*(uint32_t *)(FOE_INFO_START_ADDR(skb)) = 0UL)

// full clear FoE Info
#define DO_FULL_CLEAR_FOE(skb)	    (memset(FOE_INFO_START_ADDR(skb), 0, FOE_INFO_LEN))

#define IS_MAGIC_TAG_VALID(skb)	    ((FOE_MAGIC_TAG(skb) == FOE_MAGIC_PCI) || \
				     (FOE_MAGIC_TAG(skb) == FOE_MAGIC_GE)   || \
				     (FOE_MAGIC_TAG(skb) == FOE_MAGIC_WLAN))

#define IS_DPORT_PPE_VALID(skb)	    ((FOE_MAGIC_TAG(skb) == FOE_MAGIC_PPE) && \
				     (FOE_ENTRY_NUM(skb) == 0) && \
				     (FOE_AI(skb) == 0))


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,21)
#define LAYER2_HEADER(skb)		(skb)->mac_header
#else
#define LAYER2_HEADER(skb)		(skb)->mac.raw
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,21)
#define LAYER3_HEADER(skb)		(skb)->network_header
#else
#define LAYER3_HEADER(skb)		(skb)->nh.raw
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,21)
#define LAYER4_HEADER(skb)		(skb)->transport_header
#else
#define LAYER4_HEADER(skb)		(skb)->h.raw
#endif


/*
 * EXPORT FUNCTION
 */
int32_t GetPppoeSid(struct sk_buff *skb, uint32_t vlan_gap, uint16_t * sid, uint16_t * ppp_tag);

int PpeSetBindThreshold(uint32_t threshold);
int PpeSetMaxEntryLimit(uint32_t full, uint32_t half, uint32_t qurt);
int PpeSetRuleSize(uint16_t pre_acl, uint16_t pre_meter, uint16_t pre_ac,
		   uint16_t post_meter, uint16_t post_ac);

int PpeSetKaInterval(uint8_t tcp_ka, uint8_t udp_ka);
int PpeSetUnbindLifeTime(uint8_t lifetime);
int PpeSetBindLifetime(uint16_t tcp_fin, uint16_t udp_life, uint16_t fin_life);
int PpeSetAllowIPv6(uint8_t allow_ipv6);

extern struct FoeEntry *get_foe_table(dma_addr_t *dma_handle, uint32_t *FoeTblSize);

#endif
