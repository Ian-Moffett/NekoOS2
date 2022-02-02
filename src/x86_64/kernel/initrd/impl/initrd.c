#include "../initrd.h"

static initrd_header_t* initrd_header;
static initrd_file_header_t* file_headers;
struct _FS_NODE* initrd_root;
struct _FS_NODE* initrd_dev;
struct _FS_NODE* root_nodes;

static int nroot_nodes;  // Number of file nodes.

static dirent_t dirent;

static unsigned int _initrd_read(struct _FS_NODE* node, unsigned int offset, unsigned int size, unsigned char* buffer) {
    initrd_file_header_t header = file_headers[node->inode];

    if (offset > header.length) {
        return 0;
    }

    if (offset + size > header.length) {
        size = header.length - offset;
    }

    for (int i = 0; i < size; ++i) {
        char* addr = (char*)(header.offset + offset);
        buffer[i] = addr[i];
    }

    return size;
}
