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


static dirent_t* _initrd_readdir(struct _FS_NODE* node, unsigned int index) {
    if (node == initrd_root && index == 0) {
        strncpy(dirent.name, "dev\0", 4);
        dirent.inodeno = 0;
        return &dirent;
    }

    if (index >= nroot_nodes) {
        return 0;
    }

    strncpy(dirent.name, root_nodes[index - 1].name, strlen(root_nodes[index - 1].name + 1));
    dirent.name[strlen(root_nodes[index - 1].name)] = '\0';
    dirent.inodeno = root_nodes[index - 1].inode;
    return &dirent;
}


static struct _FS_NODE* initrd_finddir(struct _FS_NODE* node, char* name) {
}


struct _FS_NODE* init_initrd(unsigned int loc) {
    initrd_header = (initrd_header_t*)loc;
    file_headers = (initrd_file_header_t*)(loc + sizeof(initrd_header_t));
    initrd_root = (struct _FS_NODE*)kmalloc(sizeof(struct FS_NODE*));
    strncpy(initrd_root->name, "initrd\0", 7);
    initrd_root->mask = 0x0;
    initrd_root->uid = 0x0;
    initrd_root->gid = 0x0;
    initrd_root->inode = 0x0;
    initrd_root->length = 0x0;
    initrd_root->read = 0x0;
    initrd_root->write = 0x0;
    initrd_root->open = 0x0;
    initrd_root->close = 0x0;
    initrd_root->readdir = &_initrd_readdir;
    initrd_root->finddir = 0x0;
    initrd_root->ptr = 0x0;
    initrd_root->impl = 0x0;
    
    initrd_dev = (struct _FS_NODE*)kmalloc(sizeof(struct _FS_NODE*));
    strncpy(initrd_dev->name, "dev\0", 4);
    initrd_dev->mask = 0x0;
    initrd_dev->uid = 0x0;
    initrd_dev->gid = 0x0;
    initrd_dev->inode = 0x0;
    initrd_dev->length = 0x0;
    initrd_dev->flags = FS_DIRECTORY;
    initrd_dev->read = 0x0;
    initrd_dev->write = 0x0;
    initrd_dev->open = 0x0;
    initrd_dev->close = 0x0;
    initrd_dev->readdir = &_initrd_readdir;
    initrd_dev->finddir = 0x0;
    initrd_dev->ptr = 0x0;
    initrd_dev->impl = 0x0;

    root_nodes = (struct _FS_NODE*)kmalloc(sizeof(struct _FS_NODE) * initrd_header->nfiles);
    nroot_nodes = initrd_header->nfiles;
}
