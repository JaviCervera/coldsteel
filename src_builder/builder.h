#pragma once

extern "C"
{
#include "../../lib/lua/lstate.h"
#include "../../lib/lua/lua.h"
#include "../../lib/lua/lualib.h"
#include "../../lib/lua/lauxlib.h"
#include "../../lib/lua/lundump.h"
}
#include "../lib/zip/zip.h"
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#define __DID_UNDEF__STRICT_ANSI__
#endif
#include "dir.hh"
#include "file.hh"
#include "strmanip.hh"
#ifdef __DID_UNDEF__STRICT_ANSI__
#define __STRICT_ANSI__
#endif

#define MSG_LEN 1024
#define JS_SIZE_TPL_QUOTED "{\"files\":[{\"filename\":\"/data.bin\",\"start\":0,\"end\":0}],\"remote_package_size\":0"
#define JS_SIZE_REP_QUOTED "{\"files\":[{\"filename\":\"/data.bin\",\"start\":0,\"end\":#SIZE}],\"remote_package_size\":#SIZE"
#define JS_SIZE_TPL_UNQUOTED "{files:[{filename:\"/data.bin\",start:0,end:0}],remote_package_size:0"
#define JS_SIZE_REP_UNQUOTED "{files:[{filename:\"/data.bin\",start:0,end:#SIZE}],remote_package_size:#SIZE"

struct Builder
{
  typedef void (*PrintCallback)(const char *);

  Builder(PrintCallback cb)
      : m_print(cb)
  {
  }

  void Build(const char *dir, bool precompile) const
  {
    const irr::core::stringc out_file = strmanip::stripdir(dir);
    const irr::core::stringc out_dir = irr::core::stringc(dir) + ".build";
    if (filetype(out_dir.c_str()) != fdir)
      dir::create(out_dir);
#if defined(__APPLE__) && defined(__MACH__)
    CreateBundle(out_dir, out_file);
#endif
    WriteZip(dir, RuntimeDir(out_dir, out_file), "data.bin", precompile);
    CopyRuntime(out_dir, out_file);
  }

  void BuildWeb(const char *dir, bool precompile) const
  {
    const irr::core::stringc out_file = strmanip::stripdir(dir);
    const irr::core::stringc out_dir = irr::core::stringc(dir) + ".build";
    if (filetype(out_dir.c_str()) != fdir)
      dir::create(out_dir);
    const size_t pkg_size = WriteZip(dir, out_dir, out_file + ".data", precompile);
    WriteJS(out_dir, out_file, pkg_size);
    WriteWasm(out_dir, out_file);
    WriteHtml(out_dir, out_file);
  }

private:
  PrintCallback m_print;

  size_t WriteZip(const irr::core::stringc &path, const irr::core::stringc &out_dir, const irr::core::stringc &out_file, bool precompile) const
  {
    char msg[MSG_LEN];
    const irr::core::stringc zip_path = out_dir + "/" + out_file;
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing assets '%s' ...", zip_path.c_str());
      m_print(msg);
    }
    zip_t *zip = zip_open(zip_path.c_str(), 9, 'w');
    AddZipFiles(zip, path, path, precompile);
    AddZipFiles(zip, BinDir() + "/media", BinDir(), precompile);
    zip_close(zip);

    FILE *f = fopen(zip_path.c_str(), "rb");
    if (!f)
      return 0;
    fseek(f, 0, SEEK_END);
    const size_t size = ftell(f);
    fclose(f);
    return size;
  }

  void AddZipFiles(zip_t *zip, const irr::core::stringc &path, const irr::core::stringc &root_dir, bool precompile) const
  {
    const irr::core::array<irr::core::stringc> contents = dir::contents(path);
    for (irr::u32 i = 0; i < contents.size(); ++i)
    {
      if (contents[i].subString(0, 1) == ".")
        continue;
      const irr::core::stringc filepath = path + "/" + contents[i];
      if (filetype(filepath.c_str()) == fdir)
      {
        AddZipFiles(zip, filepath, root_dir, precompile);
        continue;
      }
      if (strmanip::extractext(filepath) == "lua" && precompile)
      {
        const irr::core::stringc rel_filepath = strmanip::replaceall(filepath, root_dir + "/", "");
        if (m_print)
        {
          char msg[MSG_LEN];
          sprintf(msg, "Adding precompiled '%s' as '%s' ...", filepath.c_str(), rel_filepath.c_str());
          m_print(msg);
        }
        FILE *file = CreateCompiledFile(filepath);
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
      if (strmanip::extractext(filepath) != "hx" && strmanip::extractext(filepath) != "hxml")
      {
        const irr::core::stringc rel_filepath = strmanip::replaceall(filepath, root_dir + "/", "");
        if (m_print)
        {
          char msg[MSG_LEN];
          sprintf(msg, "Adding '%s' as '%s' ...", filepath.c_str(), rel_filepath.c_str());
          m_print(msg);
        }
        zip_entry_open(zip, rel_filepath.c_str());
        zip_entry_fwrite(zip, filepath.c_str());
        zip_entry_close(zip);
      }
    }
  }

  static FILE *CreateCompiledFile(const irr::core::stringc &file)
  {
    lua_State *L = lua_open();
    if (luaL_loadfile(L, file.c_str()))
      return NULL;
    FILE *f = tmpfile();
    luaU_dump(L, clvalue(L->top - 1)->l.p, writer, f, true);
    lua_close(L);
    return f;
  }

  static int writer(lua_State *, const void *p, size_t size, void *u)
  {
    return (fwrite(p, size, 1, (FILE *)u) != 1) && (size != 0);
  }

  void WriteJS(const irr::core::stringc &out_dir, const irr::core::stringc &out_file, size_t pkg_size) const
  {
    const irr::core::stringc size_str = strmanip::fromnumber(pkg_size, "%li");
    irr::core::stringc js = strmanip::read(BinDir() + "/coldsteel.js");
    js = strmanip::replaceall(js, JS_SIZE_TPL_QUOTED,
        strmanip::replaceall(JS_SIZE_REP_QUOTED, "#SIZE", size_str));
    js = strmanip::replaceall(js, JS_SIZE_TPL_UNQUOTED,
        strmanip::replaceall(JS_SIZE_REP_UNQUOTED, "#SIZE", size_str));
    js = strmanip::replaceall(js, "coldsteel", out_file);
    const irr::core::stringc dst = out_dir + "/" + out_file + ".js";
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing js '%s' ...", dst.c_str());
      m_print(msg);
    }
    strmanip::write(js, dst, false);
  }

  void WriteWasm(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
    const irr::core::stringc src = BinDir() + "/coldsteel.wasm";
    const irr::core::stringc dst = out_dir + "/" + out_file + ".wasm";
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing wasm '%s' ...", dst.c_str());
      m_print(msg);
    }
    filecopy(src.c_str(), dst.c_str());
  }

  void WriteHtml(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
    const irr::core::stringc html = strmanip::replaceall(
        strmanip::replaceall(
            strmanip::read(BinDir() + "/coldsteel.html"),
            "coldsteel",
            out_file),
        "Emscripten-Generated Code",
        out_file);
    const irr::core::stringc dst = out_dir + "/index.html";
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing html '%s' ...", dst.c_str());
      m_print(msg);
    }
    strmanip::write(html, dst, false);
  }

  void CopyRuntime(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
#ifdef _WIN32
    const irr::core::stringc runtime_file = "coldsteel.exe";
    const irr::core::stringc out_file_with_ext = out_file + ".exe";
#else
    const irr::core::stringc runtime_file = "coldsteel";
    const irr::core::stringc out_file_with_ext = out_file;
#endif
    const irr::core::stringc src = BinDir() + "/" + runtime_file;
    const irr::core::stringc dst = RuntimeDir(out_dir, out_file_with_ext) + "/" + out_file_with_ext;
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing runtime '%s' ...", dst.c_str());
      m_print(msg);
    }
    filecopy(src.c_str(), dst.c_str());
  }

  static irr::core::stringc BinDir()
  {
    return strmanip::extractdir(dir::bin_filename());
  }

  static irr::core::stringc RuntimeDir(const irr::core::stringc &out_dir, const irr::core::stringc &out_file)
  {
#if defined(__APPLE__) && defined(__MACH__)
    return out_dir + "/" + out_file + ".app/Contents/MacOS";
#else
    return out_dir;
#endif
  }

#if defined(__APPLE__) && defined(__MACH__)
  void CreateBundle(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
    dir::create(RuntimeDir(out_dir, out_file));
    dir::create(ResourcesDir(out_dir, out_file));
    WritePlist(out_dir, out_file);
    CopyIcns(out_dir, out_file);
  }

  static irr::core::stringc ResourcesDir(const irr::core::stringc &out_dir, const irr::core::stringc &out_file)
  {
    return out_dir + "/" + out_file + ".app/Contents/Resources";
  }

  void WritePlist(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
    const irr::core::stringc plist = strmanip::replaceall(strmanip::read(BinDir() + "/Info.plist"), "$NAME", out_file);
    const irr::core::stringc dst = PlistPath(out_dir, out_file);
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing plist '%s' ...", dst.c_str());
      m_print(msg);
    }
    strmanip::write(plist, dst, false);
  }

  static irr::core::stringc PlistPath(const irr::core::stringc &out_dir, const irr::core::stringc &out_file)
  {
    return out_dir + "/" + out_file + ".app/Contents/Info.plist";
  }

  void CopyIcns(const irr::core::stringc &out_dir, const irr::core::stringc &out_file) const
  {
    const irr::core::stringc src = BinDir() + "/coldsteel.icns";
    const irr::core::stringc dst = ResourcesDir(out_dir, out_file) + "/" + out_file + ".icns";
    if (m_print)
    {
      char msg[MSG_LEN];
      sprintf(msg, "Writing icns '%s' ...", dst.c_str());
      m_print(msg);
    }
    filecopy(src.c_str(), dst.c_str());
  }
#endif
};
