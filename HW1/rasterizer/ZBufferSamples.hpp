// ZBufferSamples.hpp

#ifndef ZBUFFERSAMPLES_HPP
#define ZBUFFERSAMPLES_HPP

#include <vector>
#include <limits>
#include <cstdint>

class ZBufferSamples {
public:
    // Constructor: Initializes the buffer with default depth values
    ZBufferSamples(uint32_t width, uint32_t height, uint32_t samplesPerPixel)
        : width(width), height(height), spp(samplesPerPixel) {
        buffer.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * spp, std::numeric_limits<float>::infinity());
    }

    // Get the depth value at a specific pixel and sample index
    float Get(uint32_t x, uint32_t y, uint32_t sampleIndex) const {
        return buffer[(static_cast<size_t>(y) * width + x) * spp + sampleIndex];
    }

    // Set the depth value at a specific pixel and sample index
    void Set(uint32_t x, uint32_t y, uint32_t sampleIndex, float depth) {
        buffer[(static_cast<size_t>(y) * width + x) * spp + sampleIndex] = depth;
    }

    // Reset the buffer to the default depth values
    void Clear(float depth = std::numeric_limits<float>::infinity()) {
        std::fill(buffer.begin(), buffer.end(), depth);
    }

    // Get the number of samples per pixel
    uint32_t GetSampleCount() const {
        return spp;
    }

private:
    uint32_t width;
    uint32_t height;
    uint32_t spp; // Samples per pixel
    std::vector<float> buffer;
};

#endif // ZBUFFERSAMPLES_HPP
