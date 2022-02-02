#ifndef VFS_H
#define VFS_H

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08    // Is the file an active mountpoint?


typedef struct {
    char name[128];
    unsigned int inodeno;
} dirent_t;

struct _FS_NODE {
    char name[128];     // Filename.
    unsigned int mask;  // Perm mask.
    unsigned int uid;   // Owning user.
    unsigned int gid;   // Owning group.
    unsigned int flags; // Node type.
    unsigned int inode; // Gives a way for FS to identify files.
    unsigned int length; // Size of file in bytes.
    unsigned int impl;
    unsigned int(*read)(struct _FS_NODE*, unsigned int, unsigned int, unsigned char*);
    unsigned int(*write)(struct _FS_NODE*, unsigned int, unsigned int, unsigned char*);
    void(*open)(struct _FS_NODE*);
    void(*close)(struct _FS_NODE*);
    dirent_t*(*readdir)(struct _FS_NODE*, unsigned int);
    struct _FS_NODE*(*finddir)(struct _FS_NODE*, char* name);
    struct _FS_NODE* ptr;
};
  
unsigned int read_fs(struct _FS_NODE* node, unsigned int offset, unsigned int size, unsigned char* buffer);
unsigned int write_fs(struct _FS_NODE* node, unsigned int offset, unsigned int size, unsigned char* buffer);
void open_fs(struct _FS_NODE* node, unsigned char read, unsigned char write);
void close_fs(struct _FS_NODE* node);
dirent_t* readdir_fs(struct _FS_NODE* node, unsigned int idx);
struct _FS_NODE* finddir_fs(struct _FS_NODE* node, char* name);


#endif
