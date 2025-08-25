# Environment Setup

This project utilizes **High-Level Synthesis (HLS)** for targeting a **Xilinx FPGA** board. Development, testing, and
compilation are done using the **Vitis IDE** provided by **AMD/Xilinx**.

Follow the steps below to configure your development environment.

- [1. Download Vivado and Vitis](#1-download-vivado-and-vitis)
- [2. Install Vitis](#2-install-vitis)
- [3. Install Required Dependencies](#3-install-required-dependencies)
- [4. Install Java Runtime Environment (for Classic Vitis IDE)](#4-install-java-runtime-environment-for-classic-vitis-ide)
- [5. Fixing GCC Compatibility Issues (for Electron-based Vitis IDE)](#5-fixing-gcc-compatibility-issues-for-electron-based-vitis-ide)
  - [Try launching Vitis:](#try-launching-vitis)
  - [Install the Correct GCC Version](#install-the-correct-gcc-version)
  - [Override the Bundled GCC Library](#override-the-bundled-gcc-library)

## 1. Download Vivado and Vitis

Vitis is bundled with Vivado in the unified installer. Download it from the official Xilinx website:

```bash
wget "https://www.xilinx.com/member/forms/download/xef.html?filename=FPGAs_AdaptiveSoCs_Unified_SDI_2025.1_0530_0145_Lin64.bin" \
     -O FPGAs_AdaptiveSoCs_Unified_SDI_2025.1_0530_0145_Lin64.bin
```


## 2. Install Vitis

```bash
cd $HOME/Downloads
chmod u+x FPGAs_AdaptiveSoCs_Unified_SDI_2025.1_0530_0145_Lin64.bin
sudo ./FPGAs_AdaptiveSoCs_Unified_SDI_2025.1_0530_0145_Lin64.bin
```

## 3. Install Required Dependencies

Run the Vitis-provided script to install necessary system libraries:

```bash
/opt/Xilinx/2025.1/Vitis/scripts/installLibs.sh
```


## 4. Install Java Runtime Environment (for Classic Vitis IDE)

The classic (non-Electron) version of Vitis requires the Java Runtime Environment:

```bash
sudo apt update
sudo apt install default-jre
```


## 5. Fixing GCC Compatibility Issues (for Electron-based Vitis IDE)

The newer Vitis IDE (Electron-based) may require `libstdc++` versions that differ from those
bundled with the installer. These may be incompatible with libraries already present in your OS
(especially on Ubuntu 24.04 or newer).

### Try launching Vitis:

```bash
source /opt/Xilinx/2025.1/Vitis/settings64.sh
vitis
```

If the GUI does not appear, check for GCC-related issues:

```bash
strace -f -o vitis.log /opt/Xilinx/2025.1/Vitis/bin/vitis
cat vitis.log
```

If the log contains errors such as:

```
/opt/Xilinx/.../libstdc++.so.6: version `GLIBCXX_3.4.26' not found
/opt/Xilinx/.../libstdc++.so.6: version `GLIBCXX_3.4.29' not found
...
```

…it indicates that the bundled `libstdc++` is outdated and incompatible with your system libraries.


### Install the Correct GCC Version

To fix this, install a newer version of GCC and G++:

```bash
sudo apt update
sudo apt install gcc-12 g++-12
```

Then verify that the required GLIBCXX versions are present:

```bash
strings /usr/lib/x86_64-linux-gnu/libstdc++.so.6 | grep GLIBCXX
```

Look for versions such as `GLIBCXX_3.4.26`, `GLIBCXX_3.4.29`, `GLIBCXX_3.4.30`, etc.


### Override the Bundled GCC Library

Force Vitis to use the system version of `libstdc++` instead of the outdated bundled one:

```bash
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
vitis
```

✅ If everything is set up correctly, the Vitis IDE should now launch and function as expected.
