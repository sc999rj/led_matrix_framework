# C API for the framework 16 LED matrix

This library has been created by analyzing the CLI tool written in rust provided by the official github repo for framework 16 input modules as reference. Link: https://github.com/FrameworkComputer/InputModules

## How it works:

It sends messages and reads messages over a serial protocol using the packet format [0x32][0xAC][COMMAND][DATA].
The INIT fuction sets up the connection and stores it in the LEDMatrix struct. The communicator member variable of the struct sets up the connection settings. The device path is used to open file_descriptor (open a file to write and read from) to commuinicate with the device.

### Serial Communication Settings

- **Baud rate:** 115200 bps
  - Input speed: `B115200`
  - Output speed: `B115200`

- **Data bits:** 8
  - Configured with `CS8`

- **Parity:** None
  - `PARENB` cleared

- **Stop bits:** 1
  - `CSTOPB` cleared

- **Mode:** Raw
  - Configured with `cfmakeraw()`
  - No line processing, echo, signal generation, or character translation

- **Receiver enabled:** Yes
  - `CREAD` set

- **Ignore modem control lines:** Yes
  - `CLOCAL` set

- **Read behavior:**
  - `VMIN = 0`
  - `VTIME = 1`
  - Read returns immediately if data is available.
  - Otherwise waits up to 0.1 seconds (1 decisecond) before timing out.

- **Buffer handling:**
  - Input and output buffers are flushed after configuration using:
    ```c
    tcflush(fd, TCIOFLUSH);
    ```

- **Terminal configuration applied immediately:**
  - `tcsetattr(fd, TCSANOW, &communicator)`

## How to use

The first thing to do is create a LEDMatrix type variable and initialize the connection using the **LEDMatrix_INIT** function. After initialization you can then use the **LEDMatrix_send_cmd** and **LEDMatrix_read** functions. 

> [NOTE]
> Using the functions **LEDMatrix_send_cmd**, **LEDMatrix_read** and **LEDMatrix_CLOSE** before initializing the connection with **LEDMatrix_INIT** will result in undefined behaviour

When the connection is no longer needed use **LEDMatrix_CLOSE** to close the connection.

If look into the test folder for some examples or the doxygen docs: https://sc999rj.github.io/led_matrix_framework/ [![Documentation](https://github.com/sc999rj/led_matrix_framework/actions/workflows/docs.yml/badge.svg)](https://github.com/sc999rj/led_matrix_framework/actions/workflows/docs.yml).