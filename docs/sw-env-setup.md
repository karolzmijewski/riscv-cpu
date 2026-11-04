# Environment Setup

This project uses AMD Vitis HLS (High Level Synthesis) to synthesize its C++ CPU model to RTL and Vivado.
Vivado is the FPGA design and implementation tool used by the HLS flow. The repository contains a Vitis HLS
component configuration under `hw/hls/riscv-ips/fde_ip`. It does not currently contain a complete Vivado
board project or generated bitstream.

These instructions use AMD's unified Linux installer. The examples use version 2026.1. Substitute the version
and installation path you actually selected.

- [Environment Setup](#environment-setup)
  - [1. Download](#1-download)
  - [2. Install](#2-install)
  - [3. Linux Dependencies](#3-linux-dependencies)
  - [4. Vivado Licensing on Linux](#4-vivado-licensing-on-linux)
  - [5. Launch Vivado and Vitis](#5-launch-vivado-and-vitis)
  - [6. Troubleshooting](#6-troubleshooting)
    - [Vitis GUI does not appear](#vitis-gui-does-not-appear)
    - [`installLibs.sh` fails](#installlibssh-fails)
    - [Vitis reports missing GLIBCXX versions](#vitis-reports-missing-glibcxx-versions)

## 1. Download

Sign in to the [AMD downloads page](https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html)
and download the Linux unified installer for Vivado and Vitis. The installer file is named using a release-specific
pattern such as `FPGAs_AdaptiveSoCs_Unified_SDI_<release>_<suffix>_Lin64.bin`. The release and suffix can vary
between downloads. AMD may require an account and acceptance of its license terms.

## 2. Install

Run the downloaded installer and follow its prompts. Accept the license terms, select both Vivado and Vitis,
and choose an installation directory (for instance: `/opt/xilinx/2026.1`).

After installation, note the destination directory. For the commands below, set the root to that directory.
For example:

```bash
export XILINX_ROOT=/opt/xilinx/2026.1
```

## 3. Linux Dependencies

Vitis and Vivado supports a variety of operating systems. For this project use 64-bit **Ubuntu 24.04** and make sure the
required disk space is available (both consumes a lot of memory). Run AMD's dependency script after installation:

```bash
sudo sed -i 's/\r$//' "$XILINX_ROOT/Vitis/scripts/installLibs.sh"
sudo "$XILINX_ROOT/Vitis/scripts/installLibs.sh"
```

The line-ending conversion is only needed if the script fails with a CRLF or `^M` interpreter error. On
**Ubuntu 24.04**, older package names such as `libtinfo5` and `libncurses5` may not be available. If the AMD
script reports missing libraries, install the available Ubuntu equivalents. Common GUI dependencies include:

```bash
sudo apt update
sudo apt install -y libasound2t64 libnss3 libgtk-3-0 libxss1 libsecret-1-0
```

Package names can differ across Ubuntu releases. Use `apt-cache search <name>` to find the package available
for your release instead of forcing an obsolete package.

Set up a UTF-8 locale if the installer or tools report locale errors:

```bash
sudo apt install -y locales
sudo locale-gen en_US.UTF-8
sudo update-locale LANG=en_US.UTF-8
locale -a | grep -i en_US
```

The output should include `en_US.utf8`. Open a new shell after `update-locale` if the current shell does not
pick up the change.

For the Classic Java-based Vitis IDE, install Java if it is not already present:

```bash
sudo apt install -y default-jre
```

## 4. Vivado Licensing on Linux

Some Vivado device and implementation features require a license. Check the current AMD licensing page for
the license tier that covers your device and features. For a node-locked license, use the Host ID reported
by Vivado on the Linux system where the tools will run. A virtual machine may report a virtual network
adapter, so generate the license for the actual target environment rather than copying an ID from another
machine.

Generate a Vivado license from the [AMD licensing portal](https://account.amd.com/en/forms/license/license-form.html),
selecting the appropriate tier and Node Locked type if that matches your intended use. Enter the Host ID
requested by the portal. Install the downloaded license for your Linux user:

```bash
mkdir -p "$HOME/.Xilinx"
cp /path/to/Xilinx.lic "$HOME/.Xilinx/Xilinx.lic"
sed -i 's/\r$//' "$HOME/.Xilinx/Xilinx.lic"
chmod 600 "$HOME/.Xilinx/Xilinx.lic"
```

Alternatively, load the license using Vivado License Manager (VLM) with **Load License** and **Copy License**.
If Vivado later reports no license, compare its detected Host ID with the one used to generate the license.

## 5. Launch Vivado and Vitis

Source each tool's settings script in the terminal where you want to use that tool:

```bash
source "$XILINX_ROOT/Vivado/settings64.sh"
vivado -version
vivado
```

For Vitis, source its settings and create a workspace:

```bash
source "$XILINX_ROOT/Vitis/settings64.sh"
mkdir -p "$HOME/vitis_ws"
vitis -w "$HOME/vitis_ws"
```

To launch the Vitis command-line interface instead of the GUI, use `vitis -i` or run a script with
`vitis -s script.py`. Avoid sourcing both settings scripts globally from `~/.bashrc` unless you want their
environment changes in every shell. Per-terminal setup makes conflicts easier to diagnose.

Open the HLS component in `hw/hls/riscv-ips/fde_ip` from the Vitis workspace. Its configuration selects
the Vivado flow target. Board/platform files and a complete Vivado implementation project must be provided
separately for FPGA implementation.

For the Z7-Nano 7020 (XC7Z020), a Vivado board definition is available in
[karolzmijewski/z7-nano-7020](https://github.com/karolzmijewski/z7-nano-7020). This provides the board definition,
not a complete Vivado implementation project. That project must still be supplied separately. A different target
board can also be used if a compatible board definition is available to Vivado.

## 6. Troubleshooting

### Vitis GUI does not appear

Check that a graphical display is available to the Linux session:

```bash
echo "$DISPLAY"
```

If `$DISPLAY` is empty, configure a desktop or X11/Wayland display for the system. If the display is
available but the Electron GUI still fails, try:

```bash
vitis -w "$HOME/vitis_ws" --no-sandbox --disable-gpu
```

Missing GTK/Electron libraries and display forwarding are common causes of a silent exit. Inspect the
terminal output first. If needed, use
`strace -f -o vitis.log "$XILINX_ROOT/Vitis/bin/vitis"` to identify a missing library.

### `installLibs.sh` fails

If the error mentions `^M` or a bad interpreter, normalize the script line endings as shown in section 3.
If apt cannot locate legacy `libtinfo5` or `libncurses5`, search for the current package names for your
Ubuntu release and install the remaining dependencies individually.

### Vitis reports missing GLIBCXX versions

Only use this workaround if the error specifically says a required `GLIBCXX` version is missing. Check the
system library first:

```bash
strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX
```

Install a compatible GCC runtime if needed. If Vitis still loads an incompatible bundled `libstdc++`, try a
one-command override rather than exporting these variables for every application:

```bash
env LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6 \
  LD_LIBRARY_PATH="/usr/lib/x86_64-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
  vitis -w "$HOME/vitis_ws"
```

Remove the override if it causes different library errors. Do not apply it to unrelated tools such as Simics.
