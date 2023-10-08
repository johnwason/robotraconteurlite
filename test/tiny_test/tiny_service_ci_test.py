import subprocess
import sys
from pathlib import Path
import time
import signal

script_dir = Path(__file__).parent
repo_dir = script_dir.parent.parent.absolute()
examples_build_dir = (repo_dir / "build" / "examples").absolute()
print(examples_build_dir)

py_dir = (repo_dir / "examples" / "tiny_service").absolute()

service = subprocess.Popen([examples_build_dir / "robotraconteurlite_tiny_service"], cwd=examples_build_dir, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
time.sleep(2)



subprocess.check_call([sys.executable, py_dir / "tiny_service_client.py"], cwd=py_dir)

time.sleep(0.5)
service.send_signal(signal.SIGINT)
service.wait()

# Check return code
if service.returncode != 0:
    print(f"Service return code: {service.returncode}")

