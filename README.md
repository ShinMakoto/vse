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


