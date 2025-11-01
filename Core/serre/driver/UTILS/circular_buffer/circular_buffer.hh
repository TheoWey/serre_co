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
    CircularBuffer() : head_(0), tail_(0), full_(false) {
    }

    /**
     * @brief Add an item to the circular buffer.
     * @param item The item to add.
     */
    void add(const T &item) {
        buffer_[head_] = item;
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
        return (!full_ && (head_ == tail_));
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

  private:
    T buffer_[SIZE]; ///< The underlying buffer storage.
    size_t head_;    ///< Index of the next write position.
    size_t tail_;    ///< Index of the next read position.
    bool full_;      ///< Flag indicating if the buffer is full.
};

} // namespace utils
#endif // CIRCULAR_BUFFER_HH