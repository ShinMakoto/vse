### VSE module

### Overview

VSE (Virtual Switch Extension) is a novel software that adaptively determines CPU core assignment for SoftIRQ to prevent performance degradation.
VSE supports two types of SoftIRQ core selection mechanisms, on-the-fly or predetermined.
In the on-the-fly mode, VSE selects a SoftIRQ core based on current CPU load to exploit low-loaded CPU resources.
In the predetermined mode, SoftIRQ cores are assigned in advance to differentiate the performance of priority flows.
In the current version, VSE supports the predetermined mode.

=
### Contents
* vse_main.c: Module's main functions
* vse_flow_table.c: OpenFlow based flow table functions
* vse_rps.c: RPS fucntions
* vse_core_table.c: VSE's core table (under construction)
* vse_dev.c: API for set flow entries

=
### Supported distributions
Currently, the VSE module has been tested on the following distribution.
* Redhat Enterprise Linux 6.6, 6.7

=
### Supported NIC drivers
Currently, the VSE module has been implemented on the following NIC driver.
* Mellanox Connect-X3 (mlxn-en)


=
### Install

### 1. Getting source codes of mlxn-en driver and VSE

You can download the code from the GitHub repository.

https://github.com/sdnnit/mlxn-en-vse

Then, you download the code of VSE into mlxn-en/mellanox/mlx4/

```sh
$ git clone https://github.com/sdnnit/vse mlxn-en-vse/mellanox/mlx4/

```
### 2. Build the VSE module
To build the module, you can simply run the script.

```sh
$ sudo ./setup.sh

$ sudo ./install.sh

```
If the building process succeeds, files of mlxn-en and VSE are created in '/usr/src/mlxn-en-3.0'

Then, you build the mlxn-en module for VSE.

```sh
$ cd /usr/src/mlxn-en-3.0

$ sudo make

```

### 3. Installing the VSE module
```sh
$ sudo rmmod mlxn_en

$ sudo insmod driver/net/ethernet/mellanox/mlx4/mlx4_en.ko

$ sudo insmod driver/net/ethernet/mellanox/mlx4/vse.ko

```

- Node: 
