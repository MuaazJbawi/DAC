# DAC Waveform Generator

This repository contains code for configuring and controlling a Digital to Analog Converter (DAC) to generate two types of waveforms: Triangle and Escalator. The code is designed to run on a microcontroller platform and uses the Hardware Abstraction Layer (HAL) library for hardware interaction.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [License](#license)

## Overview

The code is organized into a `DACController` class, which handles DAC and timer configuration. The DAC can be configured to generate either a Triangle waveform or an Escalator waveform based on user input from a button press.

- `Triangle waveform`: A triangular waveform with adjustable amplitude.
- `Escalator waveform`: An escalator waveform with 6 predefined levels.

## Prerequisites

Before using this code, make sure you have the following:

- A microcontroller platform supported by the Hardware Abstraction Layer (HAL) library.
- Development environment set up for the microcontroller platform.

## Getting Started

1. Clone this repository to your development environment.

    ```bash
    git clone https://github.com/your-username/dac-waveform-generator.git
    ```

2. Open the project in your preferred development environment.

3. Make sure to configure your development environment to target your specific microcontroller platform.

4. Build and flash the code to your microcontroller.

## Usage

1. Connect your microcontroller board to the required peripherals (DAC, GPIO, etc.).

2. Power up the board.

3. Press the user button on your board to toggle between Triangle and Escalator waveforms.

4. Observe the generated waveforms on the DAC output.

5. To stop the program, reset or power cycle the microcontroller board.


 
