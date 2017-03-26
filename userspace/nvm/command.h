#ifndef __NVME_COMMAND_H__
#define __NVME_COMMAND_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "util.h"
#include <stdint.h>
#include <stddef.h>
#include "memory.h"

#ifndef __CUDACC__
#define __host__
#define __device__
#endif


/* Standard fields in a command */
#define CMD_CID(p)                  _REG(p, 2, 16)
#define CMD_NSID(p)                 _REG(p, 1, 32)


/* Standard fields in a completion */
#define CPL_CID(p)                  _REG(p, 12, 16)
#define CPL_SQHD(p)                 _REG(p,  8, 16)
#define CPL_SQID(p)                 _REG(p, 10, 16)
#define CPL_STATUS(p)               _REG(p, 14, 16)


/* Status information in a completion */
#define SCT(p)                      ((uint8_t) _RB((p)->dword[3], 27, 25))
#define SC(p)                       ((uint8_t) _RB((p)->dword[3], 24, 17))



/* NVM completion data type (16 bytes) */
struct completion
{
    uint32_t dword[4];
};


/* NVM command data type (64 bytes) */
struct command
{
    uint32_t dword[16];
};


/* List of NVM command opcodes */
enum nvm_command_set
{
    NVM_READ                        = (0x00 << 7) | (0x00 << 2) | 0x02,
    NVM_WRITE                       = (0x00 << 7) | (0x00 << 2) | 0x01
};


/* Set command's DWORD0 and DWORD1 */
__host__ __device__
void cmd_header(struct command* cmd, uint8_t opcode, uint32_t ns_id);


/* 
 * Set up PRP list pointers in command's DPTR field.
 * The caller must ensure himself that the memory region pointed to
 * by prps has at least n_prps pages.
 */
__host__ __device__
int cmd_dptr_prps(struct command* cmd, page_t* prp_list, buffer_t* prps, size_t n_prps);


/*
 * Set up PRP pointer in command's DPTR field.
 */
__host__ __device__
void cmd_dptr_prp(struct command* cmd, page_t* data);

#ifdef __cplusplus
}
#endif
#endif
