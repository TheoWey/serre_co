/**
 * @file circular_buffer.hh
 * @brief CircularBuffer class for managing a circular buffer of data.
 * This header defines the CircularBuffer class which provides methods to
 * add, retrieve, and manage data in a circular buffer structure.
 *
 * @authors ThéoWey, ThybaltCarratala
 * @date 2024-06-10
 * @version 1.0
 *
 */
#ifndef CIRCULAR_BUFFER_HH
#define CIRCULAR_BUFFER_HH

#include "stm32g0xx_hal.h"

namespace utils {
namespace circular_buffer {

/**
 * @class CircularBuffer
 * @brief A template class for managing a circular buffer.
 *
 * This class provides methods to add and retrieve data in a circular buffer
 * fashion. It supports fixed-size buffers and handles wrap-around
 * automatically.
 *
 * @tparam T The type of data to be stored in the buffer.
 * @tparam SIZE The maximum size of the circular buffer.
 */
template <typename T, size_t SIZE> class CircularBuffer {
  public:
    /**
     * @brief Constructor for CircularBuffer.
     */
    CircularBuffer() : head_(0), tail_(0), full_(false), size_(0), sum_(0) {
    }

    /**
     * @brief Add an item to the circular buffer.
     * @param item The item to add.
     */
    void add(const T &item) {
        // If buffer full, we will overwrite the oldest element at current head
        if (full_) {
            // Subtract the value being overwritten to keep a correct running
            // sum
            sum_ -= static_cast<uint32_t>(buffer_[head_]);
        } else {
            // Increase valid size until we reach capacity
            if (size_ < SIZE) {
                ++size_;
            }
        }

        buffer_[head_] = item;
        sum_ += static_cast<uint32_t>(item);

        if (full_) {
            tail_ = (tail_ + 1) % SIZE;
        }
        head_ = (head_ + 1) % SIZE;
        full_ = head_ == tail_;
    }

    /**
     * @brief Retrieve an item from the circular buffer.
     * @param item Reference to store the retrieved item.
     * @return true if an item was retrieved, false if the buffer is empty.
     */
    bool get(T &item) {
        if (isEmpty()) {
            return false;
        }
        item = buffer_[tail_];
        full_ = false;
        tail_ = (tail_ + 1) % SIZE;
        return true;
    }

    /**
     * @brief Check if the buffer is empty.
     * @return true if the buffer is empty, false otherwise.
     */
    bool isEmpty() const {
        return size_ == 0;
    }

    /**
     * @brief Check if the buffer is full.
     * @return true if the buffer is full, false otherwise.
     */
    bool isFull() const {
        return full_;
    }

    /**
     * @brief Get the current size of the buffer.
     * @return The number of items currently in the buffer.
     */
    size_t capacity() const {
        return SIZE;
    }

    /**
     * @brief Get the number of valid items currently stored.
     */
    size_t size() const {
        return size_;
    }

    /**
     * @brief Read the whole buffer content into an array.
     * @param array Pointer to the array to store the buffer content.
     */
    void readAll(T *array) const {
        if (array == nullptr) {
            return;
        }

        size_t index = 0;
        for (size_t i = tail_; i != head_; i = (i + 1) % SIZE) {
            array[index++] = buffer_[i];
        }
    }

    /**
     * @brief Calculate the average of the buffer content.
     * @return The average value of the items in the buffer.
     */
    float average() const {
        if (SIZE == 0 || size_ == 0) {
            return 0.0f;
        }
        // Use running sum for O(1) average; one float conversion at the end
        return static_cast<float>(sum_) / static_cast<float>(size_);
    }

  private:
    T buffer_[SIZE]; ///< The underlying buffer storage.
    size_t head_;    ///< Index of the next write position.
    size_t tail_;    ///< Index of the next read position.
    bool full_;      ///< Flag indicating if the buffer is full.
    size_t size_;    ///< Number of valid items stored (<= SIZE).
    uint32_t sum_;   ///< Running sum of items for O(1) average.
};

} // namespace circular_buffer
} // namespace utils
#endif // CIRCULAR_BUFFER_HH