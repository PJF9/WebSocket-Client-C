#ifndef FILE_WRITER_H
#define FILE_WRITER_H


/**
 * @brief Structure to represent trade data.
 *
 * This structure holds the trade symbol and its associated price,
 * which are used for logging trade updates.
 */
typedef struct {
    char symbol[50];
    double price;
} trade_data_t;


/**
 * @brief Writes trade data to a file.
 *
 * This function takes a pointer to a `trade_data_t` structure and writes
 * its contents to a file in append mode. The function is designed to be
 * used as a thread entry point.
 *
 * @param arg A pointer to a `trade_data_t` structure containing the trade
 *            symbol and price data to be logged.
 * @return NULL Always returns NULL as required by the thread function signature.
 *
 * @note The memory for the `trade_data_t` structure must be dynamically allocated
 *       and will be freed by the function.
 */
void *write_to_file(void *arg);

#endif
