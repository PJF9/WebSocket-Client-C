#include "file_writer.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Writes trade data to a file in an append mode.
 *
 * This function takes a pointer to a `trade_data_t` structure, which contains
 * a symbol and its associated price, and writes it to a file named
 * "trade_prices.txt" in append mode. Each entry is written in the format:
 * "Last trade update: <symbol> price = <price>".
 *
 * If the file cannot be opened, the function logs an error and frees the allocated
 * memory associated with the passed trade data.
 *
 * The allocated memory for the trade data is always freed at the end of the function
 * to prevent memory leaks, regardless of whether the file operation is successful or not.
 *
 * @param arg A pointer to a `trade_data_t` structure containing the trade symbol
 *            and price data to be written to the file. This memory is expected to
 *            be dynamically allocated by the caller.
 *
 * @return NULL Always returns NULL. This is primarily designed to be used as a thread
 *              function where the return value is ignored.
 *
 * @note It is the caller's responsibility to ensure that the passed pointer is valid
 *       and dynamically allocated, as this function will free it after use.
 */
void *write_to_file(void *arg) {
    trade_data_t *data = (trade_data_t *)arg;

    FILE *file = fopen("trade_prices.txt", "a");
    if (!file) {
        perror("Failed to open file");
        free(data);
        return NULL;
    }

    fprintf(file, "Last trade update: %s price = %.2f\n", data->symbol, data->price);
    fclose(file);

    free(data);
    return NULL;
}
