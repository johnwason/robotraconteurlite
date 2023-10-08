import subprocess
import sys
from pathlib import Path
import time

script_dir = Path(__file__).parent
repo_dir = script_dir.parent.parent
examples_build_dir = (repo_dir / "build" / "examples").absolute()
print(examples_build_dir)

py_dir = (repo_dir / "examples" / "tiny_client").absolute()

service = subprocess.Popen([sys.executable, py_dir / "tiny_client_service.py"], cwd=py_dir, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
time.sleep(2)

subprocess.check_call([examples_build_dir / "robotraconteurlite_tiny_client"], cwd=examples_build_dir)

time.sleep(0.5)
service.terminate()
service.wait()
