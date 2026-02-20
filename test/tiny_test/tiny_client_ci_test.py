import subprocess
import sys
from pathlib import Path
import time
import argparse
import sys

parser = argparse.ArgumentParser(description='Run tiny client CI test')
parser.add_argument('--build-dir', type=str, help='Build directory', required=False, default=None)
args = parser.parse_args()

script_dir = Path(__file__).parent
repo_dir = script_dir.parent.parent.absolute()
examples_build_dir = args.build_dir
if examples_build_dir is None:
    examples_build_dir = (repo_dir / "build" / "examples").absolute()
examples_build_dir = str(examples_build_dir)
print(examples_build_dir)

py_dir = str((repo_dir / "examples" / "tiny_client").absolute())

service = subprocess.Popen([sys.executable, py_dir + "/tiny_client_service.py"], cwd=py_dir,
                           stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
time.sleep(2)

subprocess.check_call([examples_build_dir + "/robotraconteurlite_tiny_client"], cwd=examples_build_dir)
time.sleep(0.1)
subprocess.check_call([examples_build_dir + "/robotraconteurlite_tiny_client",
                      "rr+ws://127.0.0.1:22229?service=tiny_service"], cwd=examples_build_dir)

time.sleep(0.5)
if sys.platform == "win32":
    service.kill()
else:
    service.terminate()
service.wait()
