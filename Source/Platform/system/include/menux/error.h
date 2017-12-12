/*!
********************************************************************************
* @file     : error.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_ERROR_H
#define INCLUDE_MENUX_ERROR_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#define E_OK					0
#define E_ERROR					1

#define E_EPERM               	1
#define E_EPERM_STR           	"Operation not permitted"
#define E_ENOENT              	2
#define E_ENOENT_STR          	"No such file or directory"
#define E_ESRCH               	3
#define E_ESRCH_STR           	"No such process"
#define E_EINTR               	4
#define E_EINTR_STR           	"Interrupted system call"
#define E_EIO                 	5
#define E_EIO_STR             	"I/O error"
#define E_ENXIO               	6
#define E_ENXIO_STR           	"No such device or address"
#define E_E2BIG               	7
#define E_E2BIG_STR           	"Arg list too long"
#define E_ENOEXEC             	8
#define E_ENOEXEC_STR         	"Exec format error"
#define E_EBADF               	9
#define E_EBADF_STR           	"Bad file number"
#define E_ECHILD              	10
#define E_ECHILD_STR          	"No child processes"
#define E_EAGAIN              	11
#define E_EWOULDBLOCK         	E_EAGAIN
#define E_EAGAIN_STR          	"Try again"
#define E_ENOMEM              	12
#define E_ENOMEM_STR          	"Out of memory"
#define E_EACCES              	13
#define E_EACCES_STR          	"Permission denied"
#define E_EFAULT              	14                         
#define E_EFAULT_STR          	"Bad address"
#define E_ENOTBLK             	15
#define E_ENOTBLK_STR         	"Block device required"
#define E_EBUSY               	16
#define E_EBUSY_STR           	"Device or resource busy"
#define E_EEXIST              	17
#define E_EEXIST_STR          	"File exists"
#define E_EXDEV               	18
#define E_EXDEV_STR           	"Cross-device link"
#define E_ENODEV              	19
#define E_ENODEV_STR          	"No such device"
#define E_ENOTDIR             	20
#define E_ENOTDIR_STR         	"Not a directory"
#define E_EISDIR              	21
#define E_EISDIR_STR          	"Is a directory"
#define E_EINVAL              	22
#define E_EINVAL_STR          	"Invalid argument"
#define E_ENFILE              	23
#define E_ENFILE_STR          	"File table overflow"
#define E_EMFILE             	24
#define E_EMFILE_STR          	"Too many open files"
#define E_ENOTTY              	25
#define E_ENOTTY_STR          	"Not a typewriter"
#define E_ETXTBSY             	26
#define E_ETXTBSY_STR         	"Text file busy"
#define E_EFBIG               	27
#define E_EFBIG_STR           	"File too large"
#define E_ENOSPC              	28
#define E_ENOSPC_STR          	"No space left on device"
#define E_ESPIPE              	29
#define E_ESPIPE_STR          	"Illegal seek"
#define E_EROFS               	30
#define E_EROFS_STR           	"Read-only file system"
#define E_EMLINK              	31
#define E_EMLINK_STR          	"Too many links"
#define E_EPIPE               	32
#define E_EPIPE_STR           	"Broken pipe"
#define E_EDOM                	33
#define E_EDOM_STR            	"Math argument out of domain of func"
#define E_ERANGE              	34
#define E_ERANGE_STR          	"Math result not representable"
#define E_ENOMSG              	35
#define E_ENOMSG_STR          	"No message of desired type"
#define E_EIDRM               	36
#define E_EIDRM_STR           	"Identifier removed"
#define E_ECHRNG              	37
#define E_ECHRNG_STR          	"Channel number out of range"
#define E_EL2NSYNC            	38
#define E_EL2NSYNC_STR        	"Level 2 not synchronized"
#define E_EL3HLT              	39
#define E_EL3HLT_STR          	"Level 3 halted"
#define E_EL3RST              	40
#define E_EL3RST_STR          	"Level 3 reset"
#define E_ELNRNG              	41                        
#define E_ELNRNG_STR          	"Link number out of range"
#define E_EUNATCH             	42                        
#define E_EUNATCH_STR         	"Protocol driver not attached"
#define E_ENOCSI              	43                      
#define E_ENOCSI_STR          	"No CSI structure available"
#define E_EL2HLT              	44                       
#define E_EL2HLT_STR          	"Level 2 halted"
#define E_EDEADLK             	45
#define E_EDEADLK_STR         	"Resource deadlock would occur"
#define E_ENOLCK              	46
#define E_ENOLCK_STR          	"No record locks available"

#define E_EBADE               	50                       
#define E_EBADE_STR           	"Invalid exchange"
#define E_EBADR               	51                        
#define E_EBADR_STR           	"Invalid request descriptor"
#define E_EXFULL              	52                        
#define E_EXFULL_STR          	"Exchange full"
#define E_ENOANO              	53                       
#define E_ENOANO_STR          	"No anode"
#define E_EBADRQC             	54                       
#define E_EBADRQC_STR         	"Invalid request code"
#define E_EBADSLT             	55                        
#define E_EBADSLT_STR         	"Invalid slot"
#define E_EDEADLOCK           	56                        
#define E_EDEADLOCK_STR       	"File locking deadlock error"
#define E_EBFONT              	57                       
#define E_EBFONT_STR          	"Bad font file format"

#define E_ENOSTR              	60
#define E_ENOSTR_STR          	"Device not a stream"
#define E_ENODATA             	61
#define E_ENODATA_STR         	"No data available"
#define E_ETIMEOUT              62
#define E_ETIMEOUT_STR          "Timer expired"
#define E_ENOSR               	63
#define E_ENOSR_STR           	"Out of streams resources"
#define E_ENONET              	64                       
#define E_ENONET_STR          	"Machine is not on the network"
#define E_ENOPKG              	65                       
#define E_ENOPKG_STR          	"Package not installed"
#define E_EREMOTE             	66                       
#define E_EREMOTE_STR         	"Object is remote"
#define E_ENOLINK             	67
#define E_ENOLINK_STR         	"Link has been severed"
#define E_EADV                	68                        
#define E_EADV_STR            	"Advertise error"
#define E_ESRMNT              	69                        
#define E_ESRMNT_STR          	"Srmount error"
#define E_ECOMM               	70                       
#define E_ECOMM_STR           	"Communication error on send"
#define E_EPROTO              	71
#define E_EPROTO_STR          	"Protocol error"

#define E_EMULTIHOP           	74
#define E_EMULTIHOP_STR       	"Multihop attempted"
#define E_ELBIN               	75                         
#define E_ELBIN_STR           	"Inode is remote"
#define E_EDOTDOT             	76                         
#define E_EDOTDOT_STR         	"RFS specific error"
#define E_EBADMSG             	77
#define E_EBADMSG_STR         	"Not a data message"

#define E_EFTYPE              	79
#define E_EFTYPE_STR          	"Inappropriate file type or format"
#define E_ENOTUNIQ            	80                         
#define E_ENOTUNIQ_STR        	"Name not unique on network"
#define E_EBADFD              	81                         
#define E_EBADFD_STR          	"File descriptor in bad state"
#define E_EREMCHG             	82                         
#define E_EREMCHG_STR         	"Remote address changed"
#define E_ELIBACC             	83                         
#define E_ELIBACC_STR         	"Can not access a needed shared library"
#define E_ELIBBAD             	84                         
#define E_ELIBBAD_STR         	"Accessing a corrupted shared library"
#define E_ELIBSCN             	85                         
#define E_ELIBSCN_STR         	".lib section in a.out corrupted"
#define E_ELIBMAX             	86                         
#define E_ELIBMAX_STR         	"Attempting to link in too many shared libraries"
#define E_ELIBEXEC            	87                         
#define E_ELIBEXEC_STR        	"Cannot exec a shared library directly"
#define E_ENOSYS              	88
#define E_ENOSYS_STR          	"Function not implemented"
#define E_ENMFILE             	89                         
#define E_ENMFILE_STR         	"No more files"
#define E_ENOTEMPTY           	90
#define E_ENOTEMPTY_STR       	"Directory not empty"
#define E_ENAMETOOLONG        	91
#define E_ENAMETOOLONG_STR    	"File name too long"
#define E_ELOOP               	92
#define E_ELOOP_STR           	"Too many symbolic links encountered"

#define E_EOPNOTSUPP          	95
#define E_EOPNOTSUPP_STR      	"Operation not supported on transport endpoint"
#define E_EPFNOSUPPORT        	96
#define E_EPFNOSUPPOE_STR    	"Protocol family not supported"

#define E_ECONNRESET          	104
#define E_ECONNRESET_STR      	"Connection reset by peer"
#define E_ENOBUFS             	105
#define E_ENOBUFS_STR         	"No buffer space available"
#define E_EAFNOSUPPORT        	106
#define E_EAFNOSUPPOE_STR    	"Address family not supported by protocol"
#define E_EPROTOTYPE          	107
#define E_EPROTOTYPE_STR      	"Protocol wrong type for socket"
#define E_ENOTSOCK            	108
#define E_ENOTSOCK_STR        	"Socket operation on non-socket"
#define E_ENOPROTOOPT         	109
#define E_ENOPROTOOPT_STR     	"Protocol not available"
#define E_ESHUTDOWN           	110                         
#define E_ESHUTDOWN_STR       	"Cannot send after transport endpoint shutdown"
#define E_ECONNREFUSED        	111
#define E_ECONNREFUSED_STR    	"Connection refused"
#define E_EADDRINUSE          	112
#define E_EADDRINUSE_STR      	"Address already in use"
#define E_ECONNABORTED        	113
#define E_ECONNABORTED_STR    	"Software caused connection abort"
#define E_ENETUNREACH         	114
#define E_ENETUNREACH_STR     	"Network is unreachable"
#define E_ENETDOWN            	115
#define E_ENETDOWN_STR        	"Network is down"
#define E_ETIMEDOUT           	116
#define E_ETIMEDOUT_STR       	"Connection timed out"
#define E_EHOSTDOWN           	117
#define E_EHOSTDOWN_STR       	"Host is down"
#define E_EHOSTUNREACH        	118
#define E_EHOSTUNREACH_STR    	"No route to host"
#define E_EINPROGRESS         	119
#define E_EINPROGRESS_STR     	"Operation now in progress"
#define E_EALREADY            	120
#define E_EALREADY_STR        	"Socket already connected"
#define E_EDESTADDRREQ        	121
#define E_EDESTADDRREQ_STR    	"Destination address required"
#define E_EMSGSIZE            	122
#define E_EMSGSIZE_STR        	"Message too long"
#define E_EPROTONOSUPPORT     	123
#define E_EPROTONOSUPPOE_STR 	"Protocol not supported"
#define E_ESOCKTNOSUPPORT     	124                         
#define E_ESOCKTNOSUPPOE_STR 	"Socket type not supported"
#define E_EADDRNOTAVAIL       	125
#define E_EADDRNOTAVAIL_STR   	"Cannot assign requested address"
#define E_ENETRESET           	126
#define E_ENETRESET_STR       	"Network dropped connection because of reset"
#define E_EISCONN             	127
#define E_EISCONN_STR         	"Transport endpoint is already connected"
#define E_ENOTCONN            	128
#define E_ENOTCONN_STR        	"Transport endpoint is not connected"
#define E_ETOOMANYREFS        	129
#define E_ETOOMANYREFS_STR    	"Too many references: cannot splice"
#define E_EPROCLIM            	130
#define E_EPROCLIM_STR        	"Limit would be exceeded by attempted fork"
#define E_EUSERS              	131
#define E_EUSERS_STR          	"Too many users"
#define E_EDQUOT              	132
#define E_EDQUOT_STR          	"Quota exceeded"
#define E_ESTALE              	133
#define E_ESTALE_STR          	"Stale NFS file handle"
#define E_ENOTSUP             	134
#define E_ENOTSUP_STR         	"Not supported"
#define E_ENOMEDIUM           	135                         
#define E_ENOMEDIUM_STR       	"No medium found"
#define E_ENOSHARE            	136                         
#define E_ENOSHARE_STR        	"No such host or network path"
#define E_ECASECLASH          	137                         
#define E_ECASECLASH_STR      	"Filename exists with different case"
#define E_EILSEQ              	138
#define E_EILSEQ_STR          	"Illegal byte sequence"
#define E_EOVERFLOW           	139
#define E_EOVERFLOW_STR       	"Value too large for defined data type"
#define E_ECANCELED           	140
#define E_ECANCELED_STR       	"Operation cancelled"
#define E_ENOTRECOVERABLE     	141
#define E_ENOTRECOVERABLE_STR 	"State not recoverable"
#define E_EOWNERDEAD          	142
#define E_EOWNERDEAD_STR      	"Previous owner died"
#define E_ESTRPIPE            	143                         
#define E_ESTRPIPE_STR        	"Streams pipe error"

#define __ELASTERROR        	2000			/* Users can add values starting here */

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
os_err_t  os_errno_get 		(void);
os_err_t *os_errno_get_ptr 	(void);
void 	  os_errno_set 		(os_err_t errcode);

#ifdef __cplusplus
}
#endif
#endif
