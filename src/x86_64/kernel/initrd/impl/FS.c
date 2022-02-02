#include "../FS.h"

unsigned int read_fs(struct _FS_NODE* node, unsigned int offset, unsigned int size, unsigned char* buffer) {
    if (node->read) {
        return node->read(node, offset, size, buffer);
    }

    return 0;
}


unsigned int write_fs(struct _FS_NODE* node, unsigned int offset, unsigned int size, unsigned char* buffer) {
    if (node->write) {
        return node->write(node, offset, size, buffer);
    }

    return 0;
}


struct _FS_NODE* finddir_fs(struct _FS_NODE* node, char* name) {
    if (node->finddir && node->flags & 0x7 == FS_DIRECTORY) {
        return node->finddir(node, name);
    }

    return 0;
}

dirent_t* readdir(struct _FS_NODE* node, unsigned int idx) {
    if (node->finddir && node->flags & 0x7 == FS_DIRECTORY) {
        return node->readdir(node, idx);
    }

    return 0;
}



