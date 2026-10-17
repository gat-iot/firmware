"""Compile the actual scanner dispatch with deterministic register responses."""
import argparse
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument("compiler", help="Host C++ compiler or Zig executable")
args = parser.parse_args()
here = Path(__file__).resolve().parent
root = here.parents[1]
source = (root / "src/detect/ScanI2CTwoWire.cpp").read_text(encoding="utf-8")
start = source.index("            case BME_ADDR:")
end = source.index("#ifndef HAS_NCP5623", start)
with tempfile.TemporaryDirectory(prefix="gat562-barometer-") as temporary:
    directory = Path(temporary)
    (directory / "sensor_probe.inc").write_text(source[start:end], encoding="utf-8")
    compiler = [args.compiler]
    if Path(args.compiler).stem == "zig":
        compiler.append("c++")
    for mode in ("gat562", "generic"):
        executable = directory / (mode + ".exe")
        flags = ["-DGAT562_BME280_ADDRESS=0x77"] if mode == "gat562" else []
        subprocess.run(compiler + ["-std=c++17", *flags, "-I", str(directory),
                                  str(here / "test.cpp"), "-o", str(executable)], check=True)
        subprocess.run([str(executable)], check=True)
