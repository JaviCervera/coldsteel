import os
import zipfile


def write_zip(path: str, zip_filename: str) -> None:
    zip_ = zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED)
    prevcwd = os.getcwd()
    os.chdir(path)
    for root, dirs, files in os.walk('.'):
        for file_ in files:
            if not file_.startswith('.'):
                zip_.write(os.path.join(root, file_))
    zip_.close()
    os.chdir(prevcwd)


if __name__ == '__main__':
  write_zip('_build', f'coldsteel-v{os.getenv("VERSION")}.zip')