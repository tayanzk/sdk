#ifndef _FS_h
#define _FS_h 1

#include <sdk/sdk.h>

#define FS_NAME_MAX 256
#define FS_PATH_MAX 4096

typedef enum fs_item_kind
{
  FS_ITEM_NONE,
  FS_ITEM_UNKNOWN,

  FS_ITEM_DIRECTORY,
  FS_ITEM_FILE
} fs_item_kind;

typedef struct fs_item_t
{
  fs_item_kind kind;

  char *name;
  char *path;
  int   size;
} fs_item_t;

typedef struct fs_path_t
{
  char **paths;
  int count;
} fs_path_t;

const char  *fs_path(void); // Executable path
const char  *fs_name(const char *path); // Basename of path
fs_item_kind fs_kind(const char *path); // Kind of item the path is
const char  *fs_find(const char *item, fs_path_t paths); // Returns allocated string path of item found in one of the paths

fs_item_t *fs_open(const char *path);
void       fs_close(fs_item_t *item);

uint fs_read(fs_item_t *item, char **data, uint bytes);
uint fs_write(fs_item_t *item, const char *data, uint bytes);

fs_item_t *fs_iter(fs_item_t *directory, fs_item_t **iter);

#endif