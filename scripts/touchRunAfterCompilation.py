import os
import glob

# Najde zkompilovany objekt v build slozce (cesta se lisi podle OS a nastaveni)
# Obvykle .pio/build/<env_name>/src/core/run_after_upload.cpp.o
obj_files = glob.glob(".pio/build/**/run_after_upload.cpp.o", recursive=True)

for f in obj_files:
    os.remove(f)