#ifndef _ENBD_H
#define _ENBD_H

#include <linux/types.h>    // For __u32, __u64, etc. 
#include <linux/list.h>     // For list_head
#include <linux/atomic.h>   // For atomic_t
#include <linux/spinlock.h> // For spinlock_t
#include <linux/socket.h>   // For socket
#include <linux/sockaddr.h> // For sockaddr_in

#define ENBD_MAJOR_NUM 43   // Major number for the device
#define ENBD_MINOR_NUM 0
#define ENBD_MAX_DEVICES 4
#define ENBD_SECTOR_SIZE 512
#define ENBD_CACHE_SIZE (32 * 1024 * 1024) // 32MB cache

// Protocal commands
enum enbd_cmd {
    ENBD_CMD_READ = 1,
    ENBD_CMD_WRITE,
    ENBD_CMD_FLUSH,
    ENBD_CMD_QUERY
};

// Network message structure
struct enbd_request {
    __u32 magic;
    __u32 type;
    __u64 handle;
    __u64 sector;
    __u32 len;
    char data[0];
} __packed;

// Cache entry structure
struct enbd_cache_entry {
    struct list_head list;
    sector_t sector;
    unsigned int size;
    void *data;
    bool dirty;
    atomic_t refcount;
};

// Device structure
struct enbd_device {
    int index;
    spinlock_t lock;
    struct socket *sock;
    struct sockaddr_in remote_addr;
};


#endif // _ENBD_H
