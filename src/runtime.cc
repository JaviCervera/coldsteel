#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#undef __STRICT_ANSI__
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#define _getcwd getcwd
#define _chdir chdir
#define _mkdir mkdir
#define _rmdir rmdir
#else
#include <direct.h>
#include "dirent.h"
#endif
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#define realpath(N, R) _fullpath((R), (N), MAX_PATH)
#undef CopyFile
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
/*
#if defined(_WIN32) && defined(LEGACY_IRRLICHT)
#define ZIP_ENABLE_SHARABLE_FILE_OPEN
#define fileno(__F) ((__F)->_file)
#define UINT64_MAX 0xffffffffui
#endif
*/
#include "../lib/zip/zip.h"
#include "core.h"
#include "dialogs.h"
#include "dir.h"
#include "internal/scripting.h"
#include "screen.h"
#include "string.h"
#include "xml.h"

static void Run();
static void Build(const stringc &dir, bool precompile);
static void BuildWeb(const stringc &dir, bool precompile);
static stringc RealDir(const stringc &dir);
static stringc BinDir();
static stringc RuntimeDir(const stringc &out_dir, const stringc &out_file);
static bool IsDir(const stringc &path);
static void CreateDir(const stringc &dir);
static void CopyFile(const stringc &src, const stringc &dst);

static void Error(const char *msg)
{
  if (_Device())
    _Device()->getLogger()->log(msg, ELL_ERROR);
  else
    puts(msg);
  Notify("Error", msg, true);
  exit(-1);
}

enum Mode
{
  MODE_RUN,
  MODE_BUILD,
  MODE_BUILD_WEB
};

struct Options
{
  Mode mode;
  stringc dir;
  bool precompile;

  Options(Mode mode, const stringc &dir, bool precompile)
      : mode(mode), dir(dir), precompile(precompile) {}

  static Options Parse(int argc, char *argv[])
  {
    if (argc != 1 && argc != 3)
      Error("Usage: coldsteel mode project_dir (mode=run,build,build_web)");
    const Mode mode = (argc > 1) ? ParseMode(argv[1]) : MODE_RUN;
    stringc dir = (argc > 2) ? argv[2] : "";
    if (dir != "")
      dir = RealDir(IsDir(dir) ? dir : ExtractDir(dir.c_str()));
    if (dir.findLast('\\') == dir.size() - 1 || dir.findLast('/') == dir.size() - 1)
      dir = dir.subString(0, dir.size() - 1);
#ifdef __APPLE__
    if (dir == "")
      dir = BinDir();
#endif
    return Options(mode, dir, true);
  }

private:
  static Mode ParseMode(const stringc &mode_name)
  {
    if (mode_name == "run")
      return MODE_RUN;
    if (mode_name == "build")
      return MODE_BUILD;
    if (mode_name == "build_web")
      return MODE_BUILD_WEB;
    Error((stringc("Invalid mode ") + mode_name + ". Accepted values: run, build, build_web").c_str());
    return MODE_RUN;
  }
};

int main(int argc, char *argv[])
{
  const Options opts = Options::Parse(argc, argv);
  _Init(opts.dir.c_str());
  switch (opts.mode)
  {
  case MODE_RUN:
    Run();
    break;
  case MODE_BUILD:
    Build(opts.dir, opts.precompile);
    break;
  case MODE_BUILD_WEB:
    BuildWeb(opts.dir, opts.precompile);
    break;
  }
  _Finish();
  return 0;
}

static void Run()
{
  if (!Scripting::Get().Load("main.lua"))
    Error(Scripting::Get().Error());
  RefreshScreen();
  CloseScreen();
}

static array<stringc> DirContentsArr(const stringc &path)
{
  array<stringc> arr;
  DIR *d = (DIR *)opendir(path.c_str());
  if (d == NULL)
    return arr;
  dirent *entry;
  while ((entry = (dirent *)readdir(d)))
  {
    if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
      arr.push_back(entry->d_name);
  }
  closedir(d);
  return arr;
}

static void AddZipFiles(zip_t *zip, const stringc &path, const stringc &root_dir, bool precompile)
{
  const array<stringc> contents = DirContentsArr(path);
  for (size_t i = 0; i < contents.size(); ++i)
  {
    if (contents[i].subString(0, 1) == ".")
      continue;
    const stringc filepath = path + "/" + contents[i];
    if (IsDir(filepath))
    {
      AddZipFiles(zip, filepath, root_dir, precompile);
      continue;
    }
    if (stringc(ExtractExt(filepath.c_str())) == "lua" && precompile)
    {
      const stringc rel_filepath = Replace(filepath.c_str(), (root_dir + "/").c_str(), "");
      printf("Adding precompiled '%s' as '%s' ...\n", filepath.c_str(), rel_filepath.c_str());
      FILE *file = Scripting::Get().CreateCompiledFile(filepath.c_str());
      if (file)
      {
        const size_t size = ftell(file);
        char *data = new char[size];
        rewind(file);
        fread(data, sizeof(char), size, file);
        fclose(file);
        zip_entry_open(zip, rel_filepath.c_str());
        zip_entry_write(zip, data, size);
        zip_entry_close(zip);
        delete[] data;
        continue;
      }
    }
    if (stringc(ExtractExt(filepath.c_str())) != "hx" && stringc(ExtractExt(filepath.c_str())) != "hxml")
    {
      const stringc rel_filepath = Replace(filepath.c_str(), (stringc(root_dir) + "/").c_str(), "");
      printf("Adding '%s' as '%s' ...\n", filepath.c_str(), rel_filepath.c_str());
      zip_entry_open(zip, rel_filepath.c_str());
      zip_entry_fwrite(zip, filepath.c_str());
      zip_entry_close(zip);
    }
  }
}

static size_t WriteZip(const stringc &path, const stringc &out_dir, const stringc &out_file, bool precompile)
{
  const stringc zip_path = out_dir + "/" + out_file;
  printf("Writing assets '%s' ...\n", zip_path.c_str());
  zip_t *zip = zip_open(zip_path.c_str(), 9, 'w');
  AddZipFiles(zip, path, path, precompile);
  AddZipFiles(zip, BinDir() + "/media", BinDir(), precompile);
  zip_close(zip);

  FILE *f = fopen(zip_path.c_str(), "rb");
  fseek(f, 0, SEEK_END);
  const size_t size = ftell(f);
  fclose(f);
  return size;
}

static void CopyRuntime(const stringc &out_dir, const stringc &out_file)
{
#ifdef _WIN32
  const stringc runtime_file = "coldsteel.exe";
  const stringc out_file_with_ext = out_file + ".exe";
#else
  const stringc runtime_file = "coldsteel";
  const stringc out_file_with_ext = out_file;
#endif
  const stringc src = BinDir() + "/" + runtime_file;
  const stringc dst = RuntimeDir(out_dir, out_file_with_ext) + "/" + out_file_with_ext;
  printf("Writing runtime '%s' ...\n", dst.c_str());
  CopyFile(src, dst);
}

static void Build(const stringc &dir, bool precompile)
{
  const stringc out_file = StripDir(dir.c_str());
  const stringc out_dir = dir + ".build";
  if (!IsDir(out_dir))
    CreateDir(out_dir);
#if defined(__APPLE__) && defined(__MACH__)
  CreateBundle(out_dir, out_file);
#endif
  WriteZip(dir, RuntimeDir(out_dir, out_file), "data.bin", precompile);
  CopyRuntime(out_dir, out_file);
}

#define JS_SIZE_TPL "[{filename:\"/data.bin\",start:0,end:0}],remote_package_size:0"
#define JS_SIZE_REP "[{filename:\"/data.bin\",start:0,end:#SIZE}],remote_package_size:#SIZE"

static void WriteJS(const stringc &out_dir, const stringc &out_file, size_t pkg_size)
{
  const stringc js = Replace(Replace(LoadString((BinDir() + "/coldsteel.js").c_str()), JS_SIZE_TPL, Replace(JS_SIZE_REP, "#SIZE", Str(pkg_size))), "coldsteel", out_file.c_str());
  const stringc dst = out_dir + "/" + out_file + ".js";
  printf("Writing js '%s' ...\n", dst.c_str());
  SaveString(js.c_str(), dst.c_str(), false);
}

static void WriteWasm(const stringc &out_dir, const stringc &out_file)
{
  const stringc src = BinDir() + "/coldsteel.wasm";
  const stringc dst = out_dir + "/" + out_file + ".wasm";
  printf("Writing wasm '%s' ...\n", dst.c_str());
  CopyFile(src, dst);
}

static void WriteHtml(const stringc &out_dir, const stringc &out_file)
{
  const stringc html = Replace(Replace(LoadString((BinDir() + "/coldsteel.html").c_str()), "coldsteel", out_file.c_str()), "Emscripten-Generated Code", out_file.c_str());
  const stringc dst = out_dir + "/index.html";
  printf("Writing html '%s' ...\n", dst.c_str());
  SaveString(html.c_str(), dst.c_str(), false);
}

static void BuildWeb(const stringc &dir, bool precompile)
{
  const stringc out_file = StripDir(dir.c_str());
  const stringc out_dir = dir + ".build";
  if (!IsDir(out_dir))
    CreateDir(out_dir);
  const size_t pkg_size = WriteZip(dir, out_dir, out_file + ".data", precompile);
  WriteJS(out_dir, out_file, pkg_size);
  WriteWasm(out_dir, out_file);
  WriteHtml(out_dir, out_file);
}

static stringc RealDir(const stringc &dir)
{
  char out_dir[FILENAME_MAX];
  realpath(dir.c_str(), out_dir);
  return out_dir;
}

static stringc BinDir()
{
  char path[FILENAME_MAX];
#if defined(_WIN32)
  path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__) && defined(__MACH__)
  unsigned int size = FILENAME_MAX;
  _NSGetExecutablePath(path, &size);
#else
  path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
  return ExtractDir(path);
}

static stringc RuntimeDir(const stringc &out_dir, const stringc &out_file)
{
#if defined(__APPLE__) && defined(__MACH__)
  return out_dir + "/" + out_file + ".app/Contents/MacOS";
#else
  return out_dir;
#endif
}

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

static bool IsDir(const stringc &path)
{
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) == -1)
    return false;
  return S_ISDIR(statbuf.st_mode);
}

void __mkdir(const char *path)
{
#ifdef _WIN32
  _mkdir(path);
#else
  _mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP);
#endif
}

static void CreateDir(const stringc &dir)
{
  char tmp[PATH_MAX];
  char *p = NULL;
  size_t len;

  snprintf(tmp, sizeof(tmp), "%s", dir.c_str());
  len = strlen(tmp);
  if (tmp[len - 1] == '/')
    tmp[len - 1] = 0;
  for (p = tmp + 1; *p; p++)
    if (*p == '/')
    {
      *p = 0;
      __mkdir(tmp);
      *p = '/';
    }
  __mkdir(tmp);
}

static void CopyFile(const stringc &src, const stringc &dst)
{
#ifdef _WIN32
  CopyFileA(src.c_str(), dst.c_str(), false);
#else
  system((stringc("cp \"") + src + "\" \"" + dst + "\"").c_str());
#endif
}

#if defined(__APPLE__) && defined(__MACH__)

static stringc ResourcesDir(const stringc &out_dir, const stringc &out_file)
{
#if defined(__APPLE__) && defined(__MACH__)
  return out_dir + "/" + out_file + ".app/Contents/Resources";
#else
  return out_dir;
#endif
}

static void WritePlist(const stringc &out_dir, const stringc &out_file)
{
  const stringc plist = Replace(LoadString((BinDir() + "/Info.plist").c_str()), "$NAME", out_file.c_str());
  const stringc dst = PlistPath(out_dir, out_file);
  printf("Writing plist '%s' ...\n", dst.c_str());
  SaveString(plist.c_str(), dst.c_str(), false);
}

static stringc PlistPath(const stringc &out_dir, const stringc &out_file)
{
  return out_dir + "/" + out_file + ".app/Contents/Info.plist";
}

void CopyIcns(const stringc &out_dir, const stringc &out_file)
{
  const stringc src = BinDir() + "/coldsteel.icns";
  const stringc dst = ResourcesDir(out_dir, out_file) + "/" + out_file + ".icns";
  printf("Writing icns '%s' ...\n", dst.c_str());
  CopyFile(src, dst);
}

static void CreateBundle(const stringc &out_dir, const stringc &out_file)
{
  CreateDir(RuntimeDir(out_dir, out_file));
  CreateDir(ResourcesDir(out_dir, out_file));
  WritePlist(out_dir, out_file);
  CopyIcns(out_dir, out_file);
}

#endif
