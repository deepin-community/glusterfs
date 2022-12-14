/*
  Copyright (c) 2008-2012 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

#ifndef __QUICK_READ_H
#define __QUICK_READ_H

#include <glusterfs/glusterfs.h>
#include <glusterfs/logging.h>
#include <glusterfs/dict.h>
#include <glusterfs/xlator.h>
#include <glusterfs/list.h>
#include <glusterfs/compat.h>
#include <glusterfs/compat-errno.h>
#include <glusterfs/common-utils.h>
#include <glusterfs/call-stub.h>
#include <glusterfs/defaults.h>
#include <libgen.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
#include "quick-read-mem-types.h"

struct qr_inode {
    void *data;
    size_t size;
    int priority;
    uint32_t ia_mtime;
    uint32_t ia_mtime_nsec;
    uint32_t ia_ctime;
    uint32_t ia_ctime_nsec;
    uint32_t gen_rollover;
    struct iatt buf;
    time_t last_refresh;
    struct list_head lru;
    uint64_t gen;
    time_t invalidation_time;
};
typedef struct qr_inode qr_inode_t;

struct qr_priority {
    char *pattern;
    int32_t priority;
    struct list_head list;
};
typedef struct qr_priority qr_priority_t;

struct qr_conf {
    uint64_t max_file_size;
    time_t cache_timeout;
    uint64_t cache_size;
    int max_pri;
    gf_boolean_t qr_invalidation;
    gf_boolean_t ctime_invalidation;
    struct list_head priority_list;
};
typedef struct qr_conf qr_conf_t;

struct qr_inode_table {
    uint64_t cache_used;
    struct list_head *lru;
    gf_lock_t lock;
};
typedef struct qr_inode_table qr_inode_table_t;

struct qr_statistics {
    gf_atomic_t cache_hit;
    gf_atomic_t cache_miss;
    gf_atomic_t file_data_invals; /* No. of invalidates received from upcall */
    gf_atomic_t files_cached;
};

struct qr_private {
    qr_conf_t conf;
    qr_inode_table_t table;
    time_t last_child_down;
    gf_lock_t lock;
    struct qr_statistics qr_counter;
    gf_atomic_int32_t generation;
};
typedef struct qr_private qr_private_t;

#endif /* #ifndef __QUICK_READ_H */
