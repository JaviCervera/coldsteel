# TODO: Precompile Lua scripts

import os
import sys
import zipfile


JS_SIZE_TPL = '[{"filename":"/package.dat","start":0,"end":0}],"remote_package_size":0'
JS_SIZE_REP = '[{"filename":"/package.dat","start":0,"end":$SIZE}],"remote_package_size":$SIZE'


def main() -> None:
    dir = parse_args()
    outfile = os.path.basename(dir)
    outdir = dir + '.build'
    os.makedirs(outdir, exist_ok=True)
    pkg_size = write_zip(dir, outdir, outfile)
    write_js(outdir, outfile, pkg_size)
    write_wasm(outdir, outfile)
    write_html(outdir, outfile)


def parse_args() -> str:
    if len(sys.argv) != 2:
        print("Usage: csbuild projectdir")
        sys.exit(-1)
    path = sys.argv[1]
    path = path if os.path.isdir(path) else os.path.dirname(path)
    path = path if not (path.endswith('/') or path.endswith('\\')) else path[:-1]
    return path


def write_zip(path: str, out_dir: str, out_name: str) -> int:
    zip_name = os.path.join(out_dir, out_name + '.data')
    zip_ = zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED)
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


def script_path() -> str:
    return os.path.dirname(os.path.abspath(__file__))


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
    main()
