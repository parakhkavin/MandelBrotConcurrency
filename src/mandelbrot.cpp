/**
 @file
 @author Kavin Parakh <kkp56@drexel.edu>
 @date 10/25/2023
 @section Description
 
 This file contains the main function and algorithms to generate the Mandelbrot set.
 */

#include "bmp.h"
#include "pixel.h"
#include <complex>
#include <thread>
#include <iostream>
#include <vector>

/**
 @def WIDTH
 @brief Width of the image in pixels.
 */
const int WIDTH = 450;


/**
 @def MAX_ITERATIONS
 @brief Maximum number of iterations for Mandelbrot calculations.
 */
const int MAX_ITERATIONS = 100;

// Function declarations
pixel getPixelColor(int iterations);
int mandelbrot(long double x, long double y, int max_iterations);
void generateMandelbrotSection(int startY, int endY, long double x1, long double x2, long double y1, long double y2, pixel* image);
void generateMandelbrot(long double x1, long double x2, long double y1, long double y2, bitMap& bmp);

/**
 @brief Main function to generate a Mandelbrot set image.
 @param argc Number of command-line arguments.
 @param argv Command-line arguments.
 @return Exit status.
 */
int main(int argc, char **argv) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <x1> <x2> <y1> <y2> <filename>" << std::endl;
        return 1;
    }

    long double x1 = std::stold(argv[1]);
    long double x2 = std::stold(argv[2]);
    long double y1 = std::stold(argv[3]);
    long double y2 = std::stold(argv[4]);
    const char *filename = argv[5];

    if (x1 >= x2 || y1 >= y2) {
        std::cerr << "Invalid coordinates: x1 must be less than x2 and y1 must be less than y2" << std::endl;
        return 1;
    }

    int height = static_cast<int>(std::abs((y2 - y1) / (x2 - x1) * WIDTH));
    bitMap bmp(WIDTH, height, filename);

    generateMandelbrot(x1, x2, y1, y2, bmp);

    bmp.saveImage(filename);

    std::cout << "Mandelbrot set generated and saved to " << filename << std::endl;
    return 0;
}

/**
  @brief Calculates the Mandelbrot value for a specific point.
  @param x X-coordinate of the point.
  @param y Y-coordinate of the point.
  @param max_iterations Maximum number of iterations.
  @return The number of iterations before escape.
 */
int mandelbrot(long double x, long double y, int max_iterations) {
    std::complex<long double> c(x, y);
    std::complex<long double> current(0, 0);
    for (int i = 1; i < max_iterations; i++) {
        current = current * current + c;
        if (std::abs(current) > 4) {
            return i;
        }
    }
    return max_iterations;
}

/**
  @brief Generates a section of the Mandelbrot set.
  @param startY Starting Y-coordinate.
  @param endY Ending Y-coordinate.
  @param x1 Minimum X-value for the section.
  @param x2 Maximum X-value for the section.
  @param y1 Minimum Y-value for the section.
  @param y2 Maximum Y-value for the section.
  @param image Pointer to the image data.
 */
void generateMandelbrotSection(int startY, int endY, long double x1, long double x2, long double y1, long double y2, pixel* image) {
    
    int height = static_cast<int>(std::abs((y2 - y1) / (x2 - x1) * WIDTH));
    long double x_step = (x2 - x1) / WIDTH;

    // Soure: Was constanly facing an error, Joe De Mario helped me figure what could have been the potential error.
    long double y_step = (y2 - y1) / height;

    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            long double cx = x1 + x * x_step;
            long double cy = y1 + y * y_step;
            int iterations = mandelbrot(cx, cy, MAX_ITERATIONS);
            image[y * WIDTH + x] = getPixelColor(iterations);
        }
    }
}

/**
  @brief Generates the full Mandelbrot set image.
  @param x1 Minimum X-value for the image.
  @param x2 Maximum X-value for the image.
  @param y1 Minimum Y-value for the image.
  @param y2 Maximum Y-value for the image.
  @param bmp Reference to the bitmap image.
 */
void generateMandelbrot(long double x1, long double x2, long double y1, long double y2, bitMap& bmp) {
    pixel* image = bmp.getImageData();

    // Use hardware_concurrency to determine the number of threads
    // Source: https://stackoverflow.com/questions/57298045/maximum-number-of-threads-in-c
    const int thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int height = bmp.getHeight();
    int rows_per_thread = height / thread_count;
    for (int i = 0; i < thread_count; i++) {
        int startY = i * rows_per_thread;
        int endY;
        if (i == thread_count - 1) {
            endY = height;
        } else {
            endY = startY + rows_per_thread;
        }
        threads.emplace_back(generateMandelbrotSection, startY, endY, x1, x2, y1, y2, image);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

/**
  @brief Determines the pixel color based on the number of iterations.
  @param iterations Number of iterations.
  @return The pixel color.
 */
pixel getPixelColor(int iterations) {
    if (iterations == MAX_ITERATIONS) {
        return pixel(41, 24, 82);  // Black for points inside the Mandelbrot set
    }

    //Source: https://stackoverflow.com/questions/21835739/smooth-color-transition-algorithm
    int color = static_cast<int>(255 * std::sqrt(iterations / static_cast<float>(MAX_ITERATIONS)));
    return pixel(color, color, color);  // Shades of grey with smoother transition
}
