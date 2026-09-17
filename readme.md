<div align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Press+Start+2P&size=42&pause=1000&color=00F0FF&multiline=false&width=500&height=100&lines=vmlite&center=true&vCenter=true&gradient=00F0FF%2C8B5CF6%2CFF007F" alt="vmlite" />
</div>

<div align="center">

  ***vmlite** is a lightweight, real-time system audio amplifier for Linux written in C++17. It decouples audio streams via a virtual PipeWire/PulseAudio null-sink and applies clean DSP gain without system-wide feedback loops or latency.*

  [![Version](https://img.shields.io/badge/v2.0.2-10B981?logo=git&logoColor=black&label=version&labelColor=ffffff&color=10B981)](https://github.com/fxhxyz4/vmlite/releases)
  [![Language](https://img.shields.io/badge/C%2B%2B17-00D2FF?logo=cplusplus&logoColor=black&label=language&labelColor=ffffff&color=00BAE2)](https://isocpp.org/)
  [![Platform](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black&label=platform&labelColor=ffffff&color=FCC624)](https://www.kernel.org/)
  [![Audio](https://img.shields.io/badge/PipeWire%20%2F%20PulseAudio-8B5CF6?logo=pipewire&logoColor=white&label=audio&labelColor=ffffff&color=8B5CF6)](https://pipewire.org/)
  [![miniaudio](https://img.shields.io/badge/miniaudio-FF69B4?logo=cplusplus&logoColor=black&label=backend&labelColor=ffffff&color=FF69B4)](https://github.com/mackron/miniaudio)
  [![License](https://img.shields.io/badge/MIT-10B981?logo=open-source-initiative&logoColor=black&label=license&labelColor=ffffff&color=10B981)](LICENSE)

</div>

---

### Features

* **Zero-Latency DSP Processing:** Operates directly on `float32` sample buffers with minimal buffer overhead.
* **No Loopback / Feedback Artifacts:** Isolated capture and playback devices decoupled via a virtual `vmlite_sink`.
* **Smooth Soft-Limiting:** Built-in `tanh` wave-shaper prevents harsh digital clipping and crackling at high amplification levels.

---

### System Requirements
* **Arch based:**
```bash
sudo pacman -S gcc make pipewire-pulse
```

* **Debian based:**
```bash
sudo apt install build-essential pipewire-pulse pulseaudio-utils
```

* **Fedora based:**
```bash
sudo dnf install gcc-c++ make pipewire-pulseaudio pulseaudio-utils
```

---

### Installation & Usage

#### Option 1: Building from Source (Recommended)
**Clone the repository:**
```bash
git clone https://github.com/fxhxyz4/vmlite.git
cd ./vmlite && make
chmod +x ./run.sh && ./run.sh 200
```

#### Option 2: Download Pre-built Binary

Download the latest compiled release from the **[Releases](https://github.com/fxhxyz4/vmlite/releases)** page, extract it, and make sure `run.sh` is executable:

```bash
chmod +x ./run.sh
```

---

#### Running vmlite
```bash
# Run launcher script with 200% amplification
./run.sh 200

# Or run without bash script
./build/vmlite -g 200
```

### Help & Command-Line Arguments
To view all available options directly from the C++ binary:

```bash
./build/vmlite --help
```

#### Output & Options

```text
Usage: vmlite [options]

Options:
  -g, --gain <percent>    Set gain in percentage (e.g., 300). Default: 100%
  -h, --help              Display this help message and exit
  -v, --version           Print version information
```

> **Note:** To stop vmlite and restore original audio routes, press `Ctrl+C`.


---

### Running as a Systemd Service
___If you want `vmlite` to run automatically as a background user service, you can use the provided systemd configuration file.___
__Create the systemd user directory if it doesn't exist:__
  ```bash
  mkdir -p ~/.config/systemd/user/
  cp vmlite.service ~/.config/systemd/user/

  # copy bin & run.sh file for daemon from tar.gz or source code
  mkdir -p ~/.local/bin/build
  cp ./build/vmlite ~/.local/bin/build/
  cp ./run.sh ~/.local/bin/

  chmod +x ~/.local/bin/build/vmlite
  chmod +x ~/.local/bin/run.sh

  systemctl --user daemon-reload
  systemctl --user enable --now vmlite

  systemctl --user restart vmlite

  systemctl --user status vmlite
  journalctl --user -u vmlite -f
  ```
---

### Acknowledgments & Credits

* **[miniaudio](https://github.com/mackron/miniaudio)** — Single-header audio playback and capture library written in C by David Reid. Used as the core audio backend engine for cross-device I/O.

---

### License

[**MIT License**](./license.md)
