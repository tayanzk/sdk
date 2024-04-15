#ifndef _FS_h
#define _FS_h 1

#include <sdk/sdk.h>

#define FS_NAME_MAX 256
#define FS_PATH_MAX 4096

typedef enum fs_item
{
  FS_NONE,
  FS_UNKNOWN,

  FS_DIRECTORY,
  FS_FILE
} fs_item;

// TODO: Use allocator_t
typedef struct fs_item_t
{
  fs_item kind;

  // TODO: Reduce allocations
  const char *name; // char name[FS_NAME_MAX]
  const char *path; // char path[FS_PATH_MAX]
  int         size;
} fs_item_t;

typedef struct fs_path_t
{
  char **paths;
  int length;
} fs_path_t;

cstr fs_path(void); // Executable path
cstr fs_name(cstr path); // Basename of path
fs_item fs_kind(cstr path); // Kind of item the path is
bool    fs_find(cstr item, fs_path_t paths, char path[FS_PATH_MAX]);

fs_item_t *fs_open(cstr path);
void       fs_close(fs_item_t *item);

uint fs_read(fs_item_t *item, char **data, uint bytes);
uint fs_write(fs_item_t *item, const char **data, uint bytes);

fs_item_t *fs_iter(fs_item_t *directory, fs_item_t **iter);

#endif
