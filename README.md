# Mining Simulation
This purpose of this simulation is to manage and track the efficiency of mining trucks and unload stations during
a continuous 72-hour lunar Helium-3 space mining operation.

# Table of Contents

- [Mining Simulation](#mining-simulation)
- [Prerequisites](#prerequisites)
- [Installation Methods](#installation-methods)
  - [Clone repository](#clone-repository)
  - [Create Conda Environment from Environment.yml](#create-conda-environment-from-environmentyml)
  - [Install Python Dependencies with Requirements.txt](#install-python-dependencies-with-requirementstxt)
- [Build and Run Simulation](#build-and-run-simulation)
  - [Step 1: Building the Simulation](#step-1-building-the-simulation)
  - [Step 2: Run the Simulation](#step-2-run-the-simulation)
  - [Step 3: Clean Up the Simulation](#step-3-clean-up-the-simulation)
- [Visualize Results (Python Script)](#visualize-results-python-script)
- [Run Unit Tests](#run-unit-tests)

## Prerequisites

- **Conda**
- **C++ Compiler** (e.g., `g++` or `clang++`)
- **Make** (for building the C++ program)
- **Python 3.10**
- **Python Libraries**: `pandas`, `plotly`, `plotly.express`(for generating visualizations)
- **pip** or **conda** (package manager)

## Installation Methods

### Clone repository
You should clone the repository and update the submodules using the following commands in the terminal:
```bash
git clone https://github.com/ayo-gb/Mining_Simulation.git
git submodule update --init --recursive
```

After cloning the repository, you can install the dependencies using any of the following methods:

### Create Conda environment from Environment.yml
**Conda** is a package and environment management system. If you don't have it, you can install Miniconda or Anaconda.
  
  - You can download Miniconda from: [Miniconda Download](https://docs.conda.io/en/latest/miniconda.html)
  - Or Anaconda from: [Anaconda Download](https://www.anaconda.com/products/distribution)

Once Conda is installed, run the following command from the top level of the project directory to create a conda environment:
`conda env create -f environment.yml`

Run the following command to activate the conda environment:
`conda activate miningSimVisualizerEnv`

### Install Python Dependencies with Requirements.txt
Before you start, ensure that you have **Python** installed on your machine. If not, you can download and install it from [Python.org](https://www.python.org/downloads/).

*(Optional but Recommended)* Upon verifying python installation, create a Python virtual environment:

```bash
python -m venv venv
```

Install the required Python Libraries via `pip`:

```bash
pip install -r requirements.txt
```

## Build and Run Simulation

This section explains how to build and run the mining simulation using the provided Makefile.

### Step 1: Building the Simulation

The project includes a `Makefile` that automates the process of compiling the C++ code. To compile the program, follow these steps:

1. Open a terminal (or command prompt) and navigate to the simulation project repository where the `Makefile` is located.

2. Run the following command to build the program:

```bash
make
```

### Step 2: Run the Simulation
To run the simulation after building, run the following command in the terminal from the top level of the project repository,
```bash
./build/mining_simulation <number_of_mining_trucks> <number_of_unloading_stations>
```

CSV files containing the performance and efficiency results of the simulation will be saved to a `/results` directory in the repository. 

### Step 3: Clean Up the Simulation
To clean up the simulation and generated results files after running, run the following line in the terminal:

```bash
make clean
```

## Visualize Results (Python Script)

To generate plots to visualize the simulation results after running the simulation, run the following command in the terminal from the top level of the project repository, using the generated CSV files as input arguments:
```bash
python scripts/simResultsVisualizer.py <path/to/mining/trucks/performance/csv> <path/to/unloading/station/performance/csv>
```

## Run Unit Tests
To automatically build and run the unit tests for the simulation, run the following command in the terminal:
```bash
make test
```

If you recieve the following message `make: 'test' is up to date.`, run the following command to force the build and run unit tests:
```bash
make - B test
```