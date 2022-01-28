# TODO: Precompile Lua scripts

import os
import platform
import shutil
import sys
from typing import Tuple
import zipfile


JS_SIZE_TPL = '[{"filename":"/package.dat","start":0,"end":0}],"remote_package_size":0'
JS_SIZE_REP = '[{"filename":"/package.dat","start":0,"end":$SIZE}],"remote_package_size":$SIZE'
PLIST_NAME = '$NAME'


def is_windows() -> bool:
    return platform.system() == 'Windows'


def is_macos() -> bool:
    return platform.system() == 'Darwin'


def runtime_path(out_dir: str, out_name: str) -> str:
    if is_macos():
        return os.path.join(out_dir, out_name + '.app', 'Contents', 'MacOS')
    else:
        return out_dir


def resources_path(out_dir: str, out_name: str) -> str:
    if is_macos():
        return os.path.join(out_dir, out_name + '.app', 'Contents', 'Resources')
    else:
        return out_dir


def plist_path(out_dir: str, out_name: str) -> str:
    if is_macos():
        return os.path.join(out_dir, out_name + '.app', 'Contents')
    else:
        return out_dir


def script_path() -> str:
    return os.path.dirname(os.path.abspath(__file__))


def copy_runtime(out_dir: str, out_name: str) -> None:
    runtime_file = 'coldsteel.exe' if is_windows() else 'coldsteel'
    out_name = (out_name + '.exe') if is_windows() else out_name
    source = os.path.join(script_path(), runtime_file)
    dest = os.path.join(runtime_path(out_dir, out_name), out_name)
    shutil.copy(source, dest)


def copy_icns(out_dir: str, out_name: str) -> None:
    source = os.path.join(script_path(), 'coldsteel.icns')
    dest = os.path.join(resources_path(out_dir, out_name), out_name + '.icns')
    shutil.copy(source, dest)


def write_plist(out_dir: str, out_name: str) -> None:
    with open(os.path.join(script_path(), 'Info.plist')) as file_:
        plist = file_.read()
    plist = plist.replace(PLIST_NAME, out_name)
    with open(os.path.join(plist_path(out_dir, out_name), 'Info.plist'), 'w') as file_:
        file_.write(plist)


def build() -> None:
    target, dir = parse_args()
    outfile = os.path.basename(dir)
    outdir = dir + '.build'
    os.makedirs(outdir, exist_ok=True)
    if target == 'desktop':
        if is_macos():
            os.makedirs(runtime_path(outdir, outfile), exist_ok=True)
            os.makedirs(resources_path(outdir, outfile), exist_ok=True)
            write_plist(outdir, outfile)
            copy_icns(outdir, outfile)
        write_zip(dir, runtime_path(outdir, outfile), 'package.dat')
        copy_runtime(outdir, outfile)
    elif target == 'html':
        pkg_size = write_zip(dir, outdir, outfile + '.data')
        write_js(outdir, outfile, pkg_size)
        write_wasm(outdir, outfile)
        write_html(outdir, outfile)


def parse_args() -> Tuple[str, str]:
    if len(sys.argv) != 3:
        print("Usage: csbuild target projectdir")
        print("Target is one of: desktop, html")
        sys.exit(-1)
    target = sys.argv[1]
    if target not in ('desktop', 'html'):
        raise ValueError(f'Invalid target {target}. Accepted values: desktop, html')
    path = sys.argv[2]
    path = path if os.path.isdir(path) else os.path.dirname(path)
    path = path if not (path.endswith('/') or path.endswith('\\')) else path[:-1]
    return (target, path)


def write_zip(path: str, out_dir: str, out_name: str) -> int:
    zip_name = os.path.join(out_dir, out_name)
    zip_ = zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED, compresslevel=9)
    prevcwd = os.getcwd()
    os.chdir(path)
    for root, dirs, files in os.walk('.'):
        for file_ in files:
            if not file_.startswith('.'):
                zip_.write(os.path.join(root, file_))
    os.chdir(prevcwd)
    os.chdir(script_path())
    for root, dirs, files in os.walk('media'):
        for file_ in files:
            if not file_.startswith('.'):
                zip_.write(os.path.join(root, file_))
    zip_.close()
    os.chdir(prevcwd)
    return os.path.getsize(zip_name)


def write_js(out_dir: str, out_name: str, pkg_size: int) -> None:
    with open(os.path.join(script_path(), 'coldsteel.js')) as file_:
        js = file_.read()
    js = js.replace(JS_SIZE_TPL, JS_SIZE_REP.replace('$SIZE', str(pkg_size)))
    js = js.replace('coldsteel', out_name)
    with open(os.path.join(out_dir, out_name + '.js'), 'w') as file_:
        file_.write(js)


def write_wasm(out_dir: str, out_name: str) -> None:
    with open(os.path.join(script_path(), 'coldsteel.wasm'), 'rb') as file_:
        wasm = file_.read()
    with open(os.path.join(out_dir, out_name + '.wasm'), 'wb') as file_:
        file_.write(wasm)


def write_html(out_dir: str, out_name: str) -> None:
    with open(os.path.join(script_path(), 'coldsteel.html')) as file_:
        html = file_.read()
    html = html.replace('coldsteel', out_name)
    html = html.replace('Emscripten-Generated Code', out_name)
    with open(os.path.join(out_dir, 'index.html'), 'w') as file_:
        file_.write(html)


if __name__ == '__main__':
    build()
