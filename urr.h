#ifndef __URR_H__
#define __URR_H__

#include <linux/kernel.h>
#include <linux/rculist.h>
#include <linux/net.h>

#include "dev.h"
#include "report.h"

#define SEID_U32ID_HEX_STR_LEN 24
// Measurement Method
#define URR_METHOD_DURAT (1 << 0) // not use temporarily
#define URR_METHOD_VOLUM (1 << 1) 
#define URR_METHOD_EVENT (1 << 2) // not use temporarily

#define URR_VOLUME_TOVOL (1 << 0)
#define URR_VOLUME_ULVOL (1 << 1)
#define URR_VOLUME_DLVOL (1 << 2)

// GTP5G_URR_VOLUME_QUOTA_FLAGS 8.2.50
#define URR_VOLUME_QUOTA_TOVOL (1 << 0)
#define URR_VOLUME_QUOTA_ULVOL (1 << 1)
#define URR_VOLUME_QUOTA_DLVOL (1 << 2)

// GTP5G_URR_VOLUME_THRESHOLD_FLAGS 8.2.13
#define URR_VOLUME_THRESHOLD_TOVOL (1 << 0)
#define URR_VOLUME_THRESHOLD_ULVOL (1 << 1)
#define URR_VOLUME_THRESHOLD_DLVOL (1 << 2)

// Measurement Information
#define URR_INFO_MBQE (1 << 0)
#define URR_INFO_INAM (1 << 1)
#define URR_INFO_RADI (1 << 2)
#define URR_INFO_ISTM (1 << 3)
#define URR_INFO_MNOP (1 << 4)
#define URR_INFO_SSPOC (1 << 5)
#define URR_INFO_ASPOC (1 << 6)
#define URR_INFO_CIAM (1 << 7)

#define URR_TRIGGER_VOLQU (1 << 0)
#define URR_TRIGGER_PERIO (1 << 8)
#define URR_TRIGGER_VOLTH (1 << 9)

struct Volume{        
    uint8_t flag;

    uint64_t totalVolume;
    uint64_t uplinkVolume;
    uint64_t downlinkVolume;
}; 

struct urr {
    struct hlist_node hlist_id;
    u64 seid;
    u32 id;
    uint64_t method;
    uint32_t trigger;
    uint64_t period;
    uint64_t info;
    uint64_t seq;

    struct Volume volumethreshold; 
    struct Volume volumequota;    

    // For usage report volume measurement
    struct VolumeMeasurement bytes;
    struct VolumeMeasurement consumed;

    // for report time
    ktime_t start_time;
    ktime_t end_time;

    // for quota exhausted
    bool quota_exhausted;
    u16 *pdrids;
    u8 *actions;

    struct net_device *dev;
    struct rcu_head rcu_head;
};

extern void urr_quota_exhaust_action(struct urr *, struct gtp5g_dev *);
extern void urr_reverse_quota_exhaust_action(struct urr *, struct gtp5g_dev *);

extern void urr_context_delete(struct urr *);
extern struct urr *find_urr_by_id(struct gtp5g_dev *, u64, u32);
extern void urr_update(struct urr *, struct gtp5g_dev *);
extern void urr_append(u64, u32, struct urr *, struct gtp5g_dev *);
extern int urr_get_pdr_ids(u16 *, int, struct urr *, struct gtp5g_dev *);
extern void urr_set_pdr(u64, u32, struct hlist_node *, struct gtp5g_dev *);
#endif // __URR_H__
