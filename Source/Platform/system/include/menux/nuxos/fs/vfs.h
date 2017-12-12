/*!
********************************************************************************
* @file     : vfs.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_FS_VFS_H
#define INCLUDE_MENUX_FS_VFS_H
#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <menux/types.h>

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#define __FS_FLAG_EOF   	(1 << 0) /* EOF detected by a read operation */
#define __FS_FLAG_ERROR 	(1 << 1) /* Error detected by any operation */
#define __FS_FLAG_LBF   	(1 << 2) /* Line buffered */
#define __FS_FLAG_UBF   	(1 << 3) /* Buffer allocated by caller of setvbuf */

// INODE±êÖ¾
#define FSNODEFLAG_TYPE_MASK       	0x00000007 /* Isolates type field        */
#define FSNODEFLAG_TYPE_DRIVER   	0x00000000 /*   Character driver         */
#define FSNODEFLAG_TYPE_BLOCK    	0x00000001 /*   Block driver             */
#define FSNODEFLAG_TYPE_MOUNTPT  	0x00000002 /*   Mount point              */
#define FSNODEFLAG_TYPE_SPECIAL    	0x00000004 /* Special OS type            */
#define FSNODEFLAG_TYPE_NAMEDSEM 	0x00000004 /*   Named semaphore          */
#define FSNODEFLAG_TYPE_MQUEUE   	0x00000005 /*   Message Queue            */
#define FSNODEFLAG_TYPE_SHM      	0x00000006 /*   Shared memory region     */
#define FSNODEFLAG_TYPE_SOFTLINK 	0x00000007 /*   Soft link                */
#define FSNODEFLAG_DELETED         	0x00000008 /* Unlinked                   */

#define INODE_IS_TYPE(i,t) 			(((i)->i_flags & FSNODEFLAG_TYPE_MASK) == (t))
#define INODE_IS_SPECIAL(i) 		(((i)->i_flags & FSNODEFLAG_TYPE_SPECIAL) != 0)

#define INODE_IS_DRIVER(i)    		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_DRIVER)
#define INODE_IS_BLOCK(i)     		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_BLOCK)
#define INODE_IS_MOUNTPT(i)   		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_MOUNTPT)
#define INODE_IS_NAMEDSEM(i)  		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_NAMEDSEM)
#define INODE_IS_MQUEUE(i)    		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_MQUEUE)
#define INODE_IS_SHM(i)       		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_SHM)
#define INODE_IS_SOFTLINK(i)  		INODE_IS_TYPE(i,FSNODEFLAG_TYPE_SOFTLINK)

#define INODE_GET_TYPE(i)     		((i)->i_flags & FSNODEFLAG_TYPE_MASK)
#define INODE_SET_TYPE(i,t) \
	do 	\
    { 	\
		(i)->i_flags = ((i)->i_flags & ~FSNODEFLAG_TYPE_MASK) | (t); \
    } 	\
	while (0)

#define INODE_SET_DRIVER(i)   		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_DRIVER)
#define INODE_SET_BLOCK(i)    		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_BLOCK)
#define INODE_SET_MOUNTPT(i)  		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_MOUNTPT)
#define INODE_SET_NAMEDSEM(i) 		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_NAMEDSEM)
#define INODE_SET_MQUEUE(i)   		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_MQUEUE)
#define INODE_SET_SHM(i)      		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_SHM)
#define INODE_SET_SOFTLINK(i) 		INODE_SET_TYPE(i,FSNODEFLAG_TYPE_SOFTLINK)

/* Mountpoint fd_flags values */

#define DIRENTFLAGS_PSEUDONODE 1

#define DIRENT_SETPSEUDONODE(f) do (f) |= DIRENTFLAGS_PSEUDONODE; while (0)
#define DIRENT_ISPSEUDONODE(f) (((f) & DIRENTFLAGS_PSEUDONODE) != 0)

#define OPEN_MAGIC      0x4200
#define OPEN_MASK       0x00ff
#define OPEN_MAXFD      0x00ff

#define OPEN_ISFD(r)    (((r) & ~OPEN_MASK) == OPEN_MAGIC)
#define OPEN_SETFD(f)   ((f) | OPEN_MAGIC)
#define OPEN_GETFD(r)   ((r) & OPEN_MASK)

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/
struct rt_file_s;   /* Forward reference */
struct pollfd; /* Forward reference */
struct rt_inode_s;  /* Forward reference */
typedef struct rt_file_ops_s
{
	int     (*open)		(struct rt_file_s *);
	int     (*close)	(struct rt_file_s *);
	ssize_t (*read)		(struct rt_file_s *, char *, os_size_t);
	ssize_t (*write)	(struct rt_file_s *, const char *, os_size_t);
	off_t   (*seek)		(struct rt_file_s *, off_t, int);
	int     (*ioctl)	(struct rt_file_s *, int, unsigned long);
	int     (*poll)		(struct rt_file_s *, struct pollfd *, bool);
	int     (*unlink)	(struct rt_inode_s *);
} rt_file_ops_t;

/* This structure provides information about the state of a block driver */

#ifndef CONFIG_DISABLE_MOUNTPOINT
typedef struct rt_geometry_s
{
	bool   geo_available;    /* true: The device is available */
	bool   geo_mediachanged; /* true: The media has changed since last query */
	bool   geo_writeenabled; /* true: It is okay to write to this device */
	os_size_t geo_nsectors;     /* Number of sectors on the device */
	os_size_t geo_sectorsize;   /* Size of one sector */
} rt_geometry_t;

typedef struct rt_block_ops_s
{
	int     (*open)		(struct rt_inode_s *);
	int     (*close)	(struct rt_inode_s *);
	ssize_t (*read)		(struct rt_inode_s *, unsigned char *, os_size_t, unsigned int);
	ssize_t (*write)	(struct rt_inode_s *, const unsigned char *, os_size_t, unsigned int);
	int     (*geometry)	(struct rt_inode_s *, struct rt_geometry_s *);
	int     (*ioctl)	(struct rt_inode_s *, int, unsigned long);
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	int     (*unlink)	(struct rt_inode_s *);
#endif
} rt_block_ops_t;

struct fs_dirent_s;
struct stat;
struct statfs;
typedef struct rt_mountpt_ops_s
{
	int     (*open)		(struct rt_file_s *, const char *, int, mode_t);
	int     (*close)	(struct rt_file_s *);
	ssize_t (*read)		(struct rt_file_s *, char *, os_size_t);
	ssize_t (*write)	(struct rt_file_s *, const char *, os_size_t);
	off_t   (*seek)		(struct rt_file_s *, off_t, int );
	int     (*ioctl)	(struct rt_file_s *, int cmd, unsigned long );
	int     (*sync)		(struct rt_file_s *);
	int     (*dup)		(const struct rt_file_s *, struct rt_file_s *);
	int     (*fstat)	(const struct rt_file_s *, struct stat *);
	int     (*opendir)	(struct rt_inode_s *, const char *, struct fs_dirent_s *);
	int     (*closedir)	(struct rt_inode_s *, struct fs_dirent_s *);
	int     (*readdir)	(struct rt_inode_s *, struct fs_dirent_s *);
	int     (*rewinddir)(struct rt_inode_s *, struct fs_dirent_s *);
	int     (*bind)		(struct rt_inode_s *, const void *data, void **);
	int     (*unbind)	(void *, struct rt_inode_s **, unsigned int);
	int     (*statfs)	(struct rt_inode_s *, struct statfs *);
	int     (*unlink)	(struct rt_inode_s *, const char *);
	int     (*mkdir)	(struct rt_inode_s *, const char *, mode_t);
	int     (*rmdir)	(struct rt_inode_s *, const char *);
	int     (*rename)	(struct rt_inode_s *, const char *, const char *);
	int     (*stat)		(struct rt_inode_s *, const char *, struct stat *);
} rt_mountpt_ops_t;
#endif /* CONFIG_DISABLE_MOUNTPOINT */

typedef union inode_ops_u
{
	const rt_file_ops_t     *i_fops;    /* Driver operations for inode */
#ifndef CONFIG_DISABLE_MOUNTPOINT
	const rt_block_ops_t    *i_bops;   /* Block driver operations */
	const rt_mountpt_ops_t  *i_mops;   /* Operations on a mountpoint */
#endif
#ifdef CONFIG_FS_NAMED_SEMAPHORES
	struct nsem_inode_s     *i_nsem;   /* Named semaphore */
#endif
#ifndef CONFIG_DISABLE_MQUEUE
	struct mqueue_inode_s   *i_mqueue; /* POSIX message queue */
#endif
#ifdef CONFIG_PSEUDOFS_SOFTLINKS
	char                    *i_link;   /* Full path to link target */
#endif
} inode_ops_t;

/* This structure represents one inode in the Nuttx pseudo-rt_file_t system */

typedef struct rt_inode_s
{
	struct rt_inode_s *i_peer;     /* Link to same level inode */
	struct rt_inode_s *i_child;    /* Link to lower level inode */
	int16_t            i_crefs;    /* References to inode */
	uint16_t           i_flags;    /* Flags for inode */
	inode_ops_t        i_ops;		/* Inode operations */
#ifdef CONFIG_FILE_MODE
	mode_t             i_mode;     /* Access mode flags */
#endif
	void         	  *i_private;  /* Per inode driver private data */
	char               i_name[1];  /* Name of inode (variable) */
} rt_inode_t;

#define FSNODE_SIZE(n) (sizeof(struct rt_inode_s) + (n))

/* This is the underlying representation of an open rt_file_t.  A rt_file_t
 * descriptor is an index into an array of such types. The type associates
 * the rt_file_t descriptor to the rt_file_t state and to a set of inode operations.
 */

typedef struct rt_file_s
{
	int               f_oflags;   /* Open mode flags */
	off_t             f_pos;      /* File position */
	struct inode     *f_inode;    /* Driver or rt_file_t system interface */
	void             *f_priv;     /* Per rt_file_t driver private data */
} rt_file_t;

/* This defines a list of files indexed by the rt_file_t descriptor */

#if CONFIG_NFILE_DESCRIPTORS > 0
struct filelist
{
	sem_t   fl_sem;               /* Manage access to the rt_file_t list */
	struct rt_file_t fl_files[CONFIG_NFILE_DESCRIPTORS];
};
#endif

#if CONFIG_NFILE_STREAMS > 0
struct file_struct
{
	int                fs_fd;        /* File descriptor associated with stream */
#ifndef CONFIG_STDIO_DISABLE_BUFFERING
	sem_t              fs_sem;       /* For thread safety */
	pid_t              fs_holder;    /* Holder of sem */
	int                fs_counts;    /* Number of times sem is held */
	unsigned char *fs_bufstart;  /* Pointer to start of buffer */
	unsigned char *fs_bufend;    /* Pointer to 1 past end of buffer */
	unsigned char *fs_bufpos;    /* Current position in buffer */
	unsigned char *fs_bufread;   /* Pointer to 1 past last buffered read char. */
#endif
	uint16_t           fs_oflags;    /* Open mode flags */
	uint8_t            fs_flags;     /* Stream flags */
#if CONFIG_NUNGET_CHARS > 0
	uint8_t            fs_nungotten; /* The number of characters buffered for ungetc */
	unsigned char      fs_ungotten[CONFIG_NUNGET_CHARS];
#endif
};

struct streamlist
{
	sem_t               sl_sem;   /* For thread safety */
	struct file_struct sl_streams[CONFIG_NFILE_STREAMS];
};
#endif /* CONFIG_NFILE_STREAMS */

/* Callback used by foreach_mountpoints to traverse all mountpoints in the
 * pseudo-rt_file_t system.
 */

#ifndef CONFIG_DISABLE_MOUNTPOINT
struct statfs;                    /* Forward reference */
typedef int (*foreach_mountpoint_t)(const char *, struct statfs *, void *);
#endif

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif
