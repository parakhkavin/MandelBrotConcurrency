#Mandelbrot Set Image Generator

This project generates visual representations of the Mandelbrot Set using concurrency in C++. The application computes the Mandelbrot Set and saves the output as a BMP image file. Concurrency techniques are employed to optimize performance for complex calculations.

Featureshttps://github.com/parakhkavin/Mandelbrot-Concurrency/blob/main/README.md

High-performance computation: Utilizes concurrency to divide and manage workloads efficiently.

Custom image output: Generates high-quality BMP images.

Modular design: Organized codebase with separate modules for image handling and fractal generation.

Project Structure

kkp56_asn4/
|-- makefile           # Build instructions
|-- docconfig          # Documentation configuration (if applicable)
|-- src/               # Source code
    |-- bmp.cpp        # BMP file handling implementation
    |-- bmp.h          # BMP file handling header
    |-- mandelbrot.cpp # Mandelbrot Set generation logic
    |-- pixel.h        # Pixel structure and utilities

Prerequisites

Compiler: GCC or any C++17 compatible compiler.

Build Tools: Make utility.

Installation

Clone this repository:

git clone https://github.com/yourusername/mandelbrot-concurrency.git

Navigate to the project directory:

cd mandelbrot-concurrency

Usage

Build the Project

Run the following command in the project directory:

make

This will compile the source files and generate an executable named mandelbrot.

Run the Program

Execute the program to generate a Mandelbrot Set image:

./mandelbrot

The output image will be saved in the same directory as a .bmp file.

Customization

You can customize the output by modifying parameters in the mandelbrot.cpp file, such as:

Image dimensions

Iteration limits

Zoom level and region of focus

Key Files

mandelbrot.cpp: Implements the logic for generating the Mandelbrot Set and coordinates multi-threaded computations.

bmp.cpp & bmp.h: Provide functions for creating and saving BMP files.

pixel.h: Defines structures and utilities for handling image pixels.

Learning Outcomes

Mastery of concurrency in C++.

Hands-on experience with image processing and file handling.

Insights into fractal mathematics and visualization techniques.

Author

Kavin Parakh

Feel free to contribute or reach out for collaboration opportunities!
