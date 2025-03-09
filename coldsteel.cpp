#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef LoadString
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <unistd.h>
#endif
#ifndef _MSC_VER
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#define _getcwd getcwd
#define _chdir chdir
#define _mkdir mkdir
#define _rmdir rmdir
#else
#include <direct.h>
#include "dirent.h"
// #undef CopyFile
// #undef DeleteFile
#endif
#include <iostream>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <vector>
extern "C"
{
#include "lib/zip/zip.c"
}

#ifdef _WIN32
#define realpath(N, R) _fullpath((R), (N), _MAX_PATH)
#endif

void Run(const std::string &dir);
void Build(const std::string &dir);
void BuildWeb(const std::string &dir);
void CreateBundle(const std::string &out_dir, const std::string &out_file);
void WritePlist(const std::string &out_dir, const std::string &out_file);
std::string PlistPath(const std::string &out_dir, const std::string &out_file);
void CopyIcns(const std::string &out_dir, const std::string &out_file);
void CopyRuntime(const std::string &out_dir, const std::string &out_file);
size_t WriteZip(const std::string &path, const std::string &out_dir, const std::string &out_file);
void AddZipFiles(zip_t *zip, const std::string &path, const std::string &root_dir);
void WriteJS(const std::string &out_dir, const std::string &out_file, size_t pkg_size);
void WriteWasm(const std::string &out_dir, const std::string &out_file);
void WriteHtml(const std::string &out_dir, const std::string &out_file);
std::string RealPath(const std::string &path);
bool IsDir(const std::string &path);
std::string ExtractDir(const std::string &path);
std::string StripDir(const std::string &filename);
std::string ExtractExt(const std::string &filename);
std::string ProgramDir();
std::string RuntimeDir(const std::string &out_dir, const std::string &out_file);
std::string ResourcesDir(const std::string &out_dir, const std::string &out_file);
void CreateDir(const std::string &path);
std::vector<std::string> DirContents(const std::string &path);
bool IsWindows();
bool IsMac();
std::string LoadString(const std::string &filename);
void SaveString(const std::string &str, const std::string &filename, bool append = true);
std::string Replace(const std::string &str, const std::string &find, const std::string &rep);
std::string Str(int value);
void CopyFile(const std::string &src, const std::string &dst);

enum Mode
{
  MODE_RUN,
  MODE_BUILD,
  MODE_BUILD_WEB
};

struct Options
{
  Mode mode;
  std::string dir;

  Options(Mode mode, const std::string &dir)
      : mode(mode), dir(dir) {}

  static Options Parse(int argc, char *argv[])
  {
    if (argc != 3)
      ThrowUsage();
    const Mode mode = ParseMode(argv[1]);
    std::string dir = RealPath(IsDir(argv[2]) ? argv[2] : ExtractDir(argv[2]));
    if (dir.rfind('\\') == dir.length() - 1 || dir.rfind('/') == dir.length() - 1)
      dir = dir.substr(0, dir.length() - 1);
    return Options(mode, dir);
  }

private:
  static void ThrowUsage()
  {
    throw std::runtime_error("Usage: coldsteel mode project_dir (mode=run,build,build_web)");
  }

  static Mode ParseMode(const std::string &mode_name)
  {
    if (mode_name == "run")
      return MODE_RUN;
    if (mode_name == "build")
      return MODE_BUILD;
    if (mode_name == "build_web")
      return MODE_BUILD_WEB;
    throw std::runtime_error("Invalid mode " + mode_name + ". Accepted values: run, build, build_web");
  }
};

int main(int argc, char *argv[])
{
  try
  {
    const Options opts = Options::Parse(argc, argv);
    switch (opts.mode)
    {
    case MODE_RUN:
      Run(opts.dir);
      break;
    case MODE_BUILD:
      Build(opts.dir);
      break;
    case MODE_BUILD_WEB:
      BuildWeb(opts.dir);
      break;
    }
    return 0;
  }
  catch (const std::exception &ex)
  {
    std::cout << ex.what() << std::endl;
    return -1;
  }
}

void Run(const std::string &dir)
{
  const std::string runtime_file = IsWindows() ? "csrun.exe" : "csrun";
  const std::string runtime_path = ProgramDir() + "/" + runtime_file;
  system((runtime_path + " " + dir).c_str());
}

void Build(const std::string &dir)
{
  const std::string out_file = StripDir(dir);
  const std::string out_dir = dir + ".build";
  if (!IsDir(out_dir))
    CreateDir(out_dir);
  if (IsMac())
    CreateBundle(out_dir, out_file);
  WriteZip(dir, RuntimeDir(out_dir, out_file), "data.bin");
  CopyRuntime(out_dir, out_file);
}

void BuildWeb(const std::string &dir)
{
  const std::string out_file = StripDir(dir);
  const std::string out_dir = dir + ".build";
  if (!IsDir(out_dir))
    CreateDir(out_dir);
  const size_t pkg_size = WriteZip(dir, out_dir, out_file + ".data");
  WriteJS(out_dir, out_file, pkg_size);
  WriteWasm(out_dir, out_file);
  WriteHtml(out_dir, out_file);
}

void CreateBundle(const std::string &out_dir, const std::string &out_file)
{
  CreateDir(RuntimeDir(out_dir, out_file));
  CreateDir(ResourcesDir(out_dir, out_file));
  WritePlist(out_dir, out_file);
  CopyIcns(out_dir, out_file);
}

void WritePlist(const std::string &out_dir, const std::string &out_file)
{
  const std::string plist = Replace(LoadString(ProgramDir() + "/Info.plist"), "$NAME", out_file);
  const std::string dst = PlistPath(out_dir, out_file);
  std::cout << "Writing plist '" << dst << "' ..." << std::endl;
  SaveString(plist, dst);
}

std::string PlistPath(const std::string &out_dir, const std::string &out_file)
{
  return out_dir + "/" + out_file + ".app/Contents/Info.plist";
}

void CopyIcns(const std::string &out_dir, const std::string &out_file)
{
  const std::string src = ProgramDir() + "/coldsteel.icns";
  const std::string dst = ResourcesDir(out_dir, out_file) + "/" + out_file + ".icns";
  std::cout << "Writing icns '" << dst << "' ..." << std::endl;
  CopyFile(src, dst);
}

void CopyRuntime(const std::string &out_dir, const std::string &out_file)
{
  const std::string runtime_file = IsWindows() ? "csrun.exe" : "csrun";
  const std::string out_file_with_ext = IsWindows() ? (out_file + ".exe") : out_file;
  const std::string src = ProgramDir() + "/" + runtime_file;
  const std::string dst = RuntimeDir(out_dir, out_file_with_ext) + "/" + out_file_with_ext;
  std::cout << "Writing runtime '" << dst << "' ..." << std::endl;
  CopyFile(src, dst);
}

size_t WriteZip(const std::string &path, const std::string &out_dir, const std::string &out_file)
{
  const std::string zip_path = out_dir + "/" + out_file;
  std::cout << "Writing assets '" << zip_path << "' ..." << std::endl;
  zip_t *zip = zip_open(zip_path.c_str(), 9, 'w');
  AddZipFiles(zip, path, path);
  AddZipFiles(zip, ProgramDir() + "/media", ProgramDir());
  zip_close(zip);

  FILE *f = fopen(zip_path.c_str(), "rb");
  fseek(f, 0, SEEK_END);
  const size_t size = ftell(f);
  fclose(f);
  return size;
}

void AddZipFiles(zip_t *zip, const std::string &path, const std::string &root_dir)
{
  const std::vector<std::string> contents = DirContents(path);
  for (size_t i = 0; i < contents.size(); ++i)
  {
    const std::string filepath = path + "/" + contents[i];
    if (IsDir(filepath))
    {
      AddZipFiles(zip, filepath, root_dir);
    }
    else if (ExtractExt(filepath) != "hx" && ExtractExt(filepath) != "hxml")
    {
      const std::string rel_filepath = Replace(filepath, root_dir + "/", "");
      std::cout << "Adding '" << filepath << "' as '" << rel_filepath << "' ..." << std::endl;
      zip_entry_open(zip, rel_filepath.c_str());
      zip_entry_fwrite(zip, filepath.c_str());
      zip_entry_close(zip);
    }
  }
}

#define JS_SIZE_TPL "[{filename:\"/data.bin\",start:0,end:0}],remote_package_size:0"
#define JS_SIZE_REP "[{filename:\"/data.bin\",start:0,end:#SIZE}],remote_package_size:#SIZE"

void WriteJS(const std::string &out_dir, const std::string &out_file, size_t pkg_size)
{
  const std::string js = Replace(Replace(LoadString(ProgramDir() + "/coldsteel.js"), JS_SIZE_TPL, Replace(JS_SIZE_REP, "#SIZE", Str(pkg_size))), "csrun", out_file);
  const std::string dst = out_dir + "/" + out_file + ".js";
  std::cout << "Writing js '" << dst << "' ..." << std::endl;
  SaveString(js, dst);
}

void WriteWasm(const std::string &out_dir, const std::string &out_file)
{
  const std::string src = ProgramDir() + "/coldsteel.wasm";
  const std::string dst = out_dir + "/" + out_file + ".wasm";
  std::cout << "Writing wasm '" << dst << "' ..." << std::endl;
  CopyFile(src, dst);
}

void WriteHtml(const std::string &out_dir, const std::string &out_file)
{
  const std::string html = Replace(Replace(LoadString(ProgramDir() + "/coldsteel.html"), "coldsteel", out_file), "Emscripten-Generated Code", out_file);
  const std::string dst = out_dir + "/index.html";
  std::cout << "Writing html '" << dst << "' ..." << std::endl;
  SaveString(html, dst);
}

std::string RealPath(const std::string &path)
{
  char out_path[FILENAME_MAX];
  realpath(path.c_str(), out_path);
  return out_path;
}

#if defined _WIN32 && !defined S_ISDIR
#define S_ISDIR(m) (((m) & _S_IFDIR) == _S_IFDIR)
#endif

bool IsDir(const std::string &path)
{
  struct stat statbuf;
  if (stat(path.c_str(), &statbuf) == -1)
    return false;
  return S_ISDIR(statbuf.st_mode);
}

std::string ExtractDir(const std::string &path)
{
  size_t find_pos = path.rfind('\\');
  if (find_pos == std::string::npos)
    find_pos = path.rfind('/');
  return path.substr(0, find_pos);
}

std::string StripDir(const std::string &filename)
{
  size_t find_pos = filename.rfind('\\');
  if (find_pos == std::string::npos)
    find_pos = filename.rfind('/');
  return (find_pos != std::string::npos)
             ? filename.substr(find_pos + 1, filename.length() - find_pos - 1)
             : filename;
}

std::string ExtractExt(const std::string &filename)
{
  const size_t find_pos = filename.rfind('.');
  return (find_pos != std::string::npos)
             ? filename.substr(find_pos + 1, filename.length() - find_pos - 1)
             : "";
}

std::string ProgramDir()
{
  char path[FILENAME_MAX];
#if defined(_WIN32)
  path[GetModuleFileNameA(NULL, path, FILENAME_MAX)] = 0;
#elif defined(__APPLE__)
  unsigned int size = FILENAME_MAX;
  _NSGetExecutablePath(path, &size);
#else
  path[readlink("/proc/self/exe", path, FILENAME_MAX)] = 0;
#endif
  return ExtractDir(path);
}

std::string RuntimeDir(const std::string &out_dir, const std::string &out_file)
{
  return IsMac() ? (out_dir + "/" + out_file + ".app/Contents/MacOS") : out_dir;
}

std::string ResourcesDir(const std::string &out_dir, const std::string &out_file)
{
  return IsMac() ? (out_dir + "/" + out_file + ".app/Contents/Resources") : out_dir;
}

void __mkdir(const char *path)
{
#ifdef _WIN32
  _mkdir(path);
#else
  _mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP);
#endif
}

void CreateDir(const std::string &dir)
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

std::vector<std::string> DirContents(const std::string &path)
{
  std::vector<std::string> arr;
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

bool IsWindows()
{
#ifdef _WIN32
  return true;
#else
  return false;
#endif
}

bool IsMac()
{
#if defined(__APPLE__) && defined(__MACH__)
  return true;
#else
  return false;
#endif
}

std::string LoadString(const std::string &filename)
{
  FILE *f = fopen(filename.c_str(), "rb");
  if (!f)
    return "";
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buf = (char *)malloc(size + 1);
  fread(buf, sizeof(char), size, f);
  buf[size] = '\0';
  std::string str(buf);
  free(buf);
  return str;
}

void SaveString(const std::string &str, const std::string &filename, bool append)
{
  FILE *f = fopen(filename.c_str(), append ? "ab" : "wb");
  if (!f)
    return;
  fwrite(str.c_str(), sizeof(char), str.length(), f);
  fclose(f);
}

std::string Replace(const std::string &str, const std::string &find, const std::string &rep)
{
  std::string strcopy = str;
  size_t find_pos = strcopy.find(find);
  while (find_pos != std::string::npos)
  {
    strcopy.replace(find_pos, find.length(), rep);
    find_pos = strcopy.find(find, find_pos + rep.length());
  }
  return strcopy;
}

std::string Str(int value)
{
  char buf[32];
  sprintf(buf, "%i", value);
  return std::string(buf);
}

void CopyFile(const std::string &src, const std::string &dst)
{
#ifdef _WIN32
  CopyFile(src.c_str(), dst.c_str(), false);
#else
  system(("cp \"" + src + "\" \"" + dst + "\"").c_str());
#endif
}
