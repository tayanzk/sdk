#define _GNU_SOURCE
#include <sdk/fs.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>

typedef struct fs_base_t
{
  fs_item_t item;
  fs_item_t iter;

  DIR *dir;
} fs_base_t;

static char _DirectoryName[FS_PATH_MAX] = { 0 };

static void fs_absolute(const char *path, char absolute[FS_PATH_MAX])
{
  static char _Temp[FS_PATH_MAX] = { 0 };
  int length;

  if (!realpath(path, _Temp))
    return;

  length = strnlen(_Temp, FS_PATH_MAX);
  assert(length > 0);

  if (length <= 0)
    return;

  assert(absolute != NULL);

  if (!absolute)
    return;

  memcpy(absolute, _Temp, length);
}

const char *fs_path()
{
  uint length = readlink("/proc/self/exe", _DirectoryName, FS_PATH_MAX);
  _DirectoryName[length] = 0;

  void *last = memrchr(_DirectoryName, '/', length - 2);
  _DirectoryName[(char *) last - _DirectoryName] = 0;

  return _DirectoryName;
}

fs_item_kind fs_kind(const char *path)
{
  struct stat s;

  if (stat(path, &s) != -1)
  {
    if (s.st_mode & S_IFDIR)
    {
      return FS_ITEM_DIRECTORY;
    }

    if (s.st_mode & S_IFREG)
    {
      return FS_ITEM_FILE;
    }

    return FS_ITEM_UNKNOWN;
  }

  return FS_ITEM_NONE;
}

const char *fs_name(const char *path)
{
  return basename((char *) path);
}

static fs_item_t *fs_fill_item(fs_item_t *fs, fs_item_kind kind, const char *path, struct stat *s)
{
  fs->kind = kind;
  fs->path = path;
  fs->name = fs_name(fs->path);
  fs->size = s->st_size;

  return fs;
}

static void fs_scan(fs_item_t *it, const char *path, struct stat *s, bool base)
{
  // NOTE: Only works for directories and files
  // TODO: Support links
    
  // Directory
  if (s->st_mode & S_IFDIR)
  {
    fs_base_t *fs = (fs_base_t *) fs_fill_item(it, FS_ITEM_DIRECTORY, path, s);
    
    if (base)
    {
      fs->dir  = opendir(fs->item.path);
      fs->iter = (fs_item_t) { 0 };
    }
  }
  // File
  else if (s->st_mode & S_IFREG)
  {
    fs_fill_item(it, FS_ITEM_FILE, path, s);
  }
  else
  {
    fs_fill_item(it, FS_ITEM_UNKNOWN, path, s);
  }
}

bool fs_find(const char *item, fs_path_t paths, char path[FS_PATH_MAX])
{
  char item_path[FS_PATH_MAX];

  for (int i = 0; i < paths.count; i++)
  {
    char *path = paths.paths[i];
    char absolute[FS_PATH_MAX];

    fs_absolute(path, absolute);

    snprintf(item_path, FS_PATH_MAX, "%s/%s", absolute, item);

    if (fs_kind(item_path))
    {
      memcpy(path, item_path, FS_PATH_MAX);

      return true;
    }
  }

  return false;
}

fs_item_t *fs_open(const char *path)
{
  struct stat s;
  
  char *absolute = malloc(FS_PATH_MAX);
  fs_absolute(path, absolute);

  fs_base_t *fs = malloc(sizeof (fs_base_t));
  assert(fs != NULL);

  if (stat(absolute, &s) != -1)
  {
    fs_scan((fs_item_t *) fs, absolute, &s, true);
  }
  else
  {
    free(absolute);
    free(fs);

    return NULL;
  }

  return (fs_item_t *) fs;
}

void fs_close(fs_item_t *item)
{
  fs_base_t *fs = (fs_base_t *) item; 

  assert(item       != NULL);
  assert(item->path != NULL);

  if (item->kind == FS_ITEM_DIRECTORY)
  {
    closedir(fs->dir);
  }

  if (fs->iter.path)
    free((char *) fs->iter.path);
  
  free((char *) item->path);
  free(item);
}

uint fs_read(fs_item_t *item, char **data, uint bytes)
{
  assert(item       != NULL);
  assert(item->path != NULL);
  assert(item->kind == FS_ITEM_FILE);

  FILE *file = fopen(item->path, "r");
  uint read;

  assert(file != NULL);
  assert(data != NULL);

  if (bytes == 0)
  {
    fseek(file, 0, SEEK_END);
    bytes = ftell(file);
    rewind(file);

    assert(bytes != 0);
    *data = malloc(bytes + 1);
    assert(*data != NULL);
  }

  read = fread(*data, 1, bytes, file);
  fclose(file);

  return read;
}

uint fs_write(fs_item_t *item, const char *data, uint bytes)
{
  assert(item != NULL);
  assert(item->path != NULL);
  assert(item->kind == FS_ITEM_FILE);

  FILE *file = fopen(item->path, "w");

  assert(file != NULL);
  assert(data != NULL);

  if (bytes == 0)
    return 0;

  uint write = fwrite(data, 1, bytes, file);
  fclose(file);

  return write;
}

fs_item_t *fs_iter(fs_item_t *directory, fs_item_t **iter)
{
  fs_base_t *fs = (fs_base_t *) directory;
  struct dirent *entry;

  assert(directory       != NULL);
  assert(directory->path != NULL);
  assert(directory->kind == FS_ITEM_DIRECTORY);

  if (*iter == NULL)
  {
    *iter = &fs->iter;
  }
  else
  {
    free((char *) fs->iter.path);
    fs->iter = (fs_item_t) { 0 };
  }

  if ((entry = readdir(fs->dir)) != NULL)
  {
    char Path[FS_PATH_MAX] = { 0 };

    struct stat s;
    char *path;

    if (entry->d_name[0] == '.' || (entry->d_name[0] == '.' && entry->d_name[1] == '.'))
      return fs_iter(directory, iter);

    strncpy(Path, directory->path, FS_PATH_MAX - 1);
    strncat(Path, "/", FS_PATH_MAX - 1);
    strncat(Path, entry->d_name, FS_PATH_MAX - 1);

    path = calloc(1, FS_PATH_MAX);
    memcpy(path, Path, FS_PATH_MAX);

    if (stat(path, &s) != -1)
    {
      fs_scan(&fs->iter, path, &s, false);

      return &fs->iter;
    }
  }

  return NULL;
}
