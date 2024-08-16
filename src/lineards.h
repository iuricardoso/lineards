/*
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Author: Iuri Sônego Cardoso
 * Date: 24 Jun 2024
 */

/**
 * @file lineards.h
 * @brief Header file defining functions and data types for linear data structures in C.
 *
 * This header file defines functions and data types for working with linear data structures,
 * using either arrays (vectors) or linked lists for storage and access of data elements.
 *
 * Supported operations include insertion, removal, retrieval, modification, iteration,
 * and access to specific data structure properties such as size, capacity, and type.
 *
 * @note This library provides functionalities for both stack and queue operations,
 * utilizing the underlying linear data structures.
 *
 * @author Iuri S&ocirc;nego Cardoso
 * @date June 24, 2024
 *
 * @remarks This library and its documentation were constructed with the assistance of AI.
 *
 * @example Vector A LINEAR_DS with data storage in a vector.
 * @code
 * #include <stdio.h>
 * #include <lineards.h>
 *
 * int main() {
 *     // Create a new vector with an initial capacity of 10 and element size of sizeof(int)
 *     LINEAR_DS* my_vector = lds_new_vector(10, sizeof(int));
 *
 *     // Check if vector creation was successful
 *     if (my_vector == NULL) {
 *         // Handle error
 *         return -1;
 *     }
 *
 *     // Insert elements into the vector
 *     int value = 42;
 *     lds_return_t result = lds_insert_last(my_vector, &value);
 *
 *     if (result != LDS_SUCCESS) {
 *         // Handle insertion error
 *     }
 *
 *     // Retrieve the element at position 0
 *     int retrieved_value;
 *     result = lds_get(my_vector, 0, &retrieved_value);
 *
 *     if (result == LDS_SUCCESS) {
 *         // Successfully retrieved value
 *         printf("Retrieved value: %d\n", retrieved_value);
 *     } else {
 *         // Handle retrieval error
 *     }
 *
 *     // Free the vector
 *     lds_free(my_vector);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @example Stack A LINEAR_DS with data storage in a linked list, operating as a stack.
 * @code
 * #include <stdio.h>
 * #include <lineards.h>
 *
 * int main() {
 *     // Create a new linked list with element size of sizeof(int)
 *     LINEAR_DS* my_stack = lds_new_list(sizeof(int));
 *
 *     // Check if list creation was successful
 *     if (my_stack == NULL) {
 *         // Handle error
 *         return -1;
 *     }
 *
 *     // Push three elements onto the stack
 *     int value1 = 10, value2 = 20, value3 = 30;
 *     lds_stack_push(my_stack, &value1);
 *     lds_stack_push(my_stack, &value2);
 *     lds_stack_push(my_stack, &value3);
 *
 *     // Retrieve the top element of the stack without removing it
 *     int top_value;
 *     lds_stack_top(my_stack, &top_value);
 *     printf("Top value: %d\n", top_value);
 *
 *     // Pop all elements from the stack using a loop
 *     int popped_value;
 *     while (lds_size(my_stack) > 0) {
 *         lds_stack_pop(my_stack, &popped_value);
 *         printf("Popped value: %d\n", popped_value);
 *     }
 *
 *     // Free the stack
 *     lds_free(my_stack);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @example Iterator
 * @code
 * #include <lineards.h>
 *
 * int main() {
 *     // Create a new linked list with element size of sizeof(int)
 *     LINEAR_DS* my_list = lds_new_list(sizeof(int));
 *
 *     // Check if list creation was successful
 *     if (my_list == NULL) {
 *         // Handle error
 *         return -1;
 *     }
 *
 *     // Insert three elements into the list
 *     int value1 = 10, value2 = 20, value3 = 30;
 *     lds_insert_last(my_list, &value1);
 *     lds_insert_last(my_list, &value2);
 *     lds_insert_last(my_list, &value3);
 *
 *     // Create an iterator for the list
 *     LDS_ITERATOR* iterator = lds_iterator(my_list);
 *
 *     // Reset the iterator to the beginning of the list
 *     lds_it_reset(iterator);
 *
 *     // Iterate over the list and print each element
 *     int element;
 *     for (lds_it_reset(iterator); lds_it_get(iterator, &element) == LDS_SUCCESS; lds_it_next(iterator)) {
 *         printf("Element: %d\n", element);
 *     }
 *
 *     // Free the list (iterator is freed automatically)
 *     lds_free(my_list);
 *
 *     return 0;
 * }
 * @endcode
 */


 /**
 * @mainpage Linear Data Structures Library Documentation
 *
 * @section intro_sec Introduction
 *
 * Welcome to the documentation for the Linear Data Structures Library in C.
 * This library provides implementations of various linear data structures,
 * including vectors and linked lists, designed for efficient storage and
 * manipulation of elements.
 *
 * @section features_sec Features
 *
 * - Support for both vector (array-based) and linked list data structures.
 * - Operations include insertion, removal, retrieval, and iteration.
 * - Stack and queue functionalities built on top of the underlying data structures.
 * - Iterators for traversing elements in the data structures.
 *
 * @section usage_sec Usage
 *
 * To use the library, include the header file `lineards.h` in your C source files:
 *
 * @code
 * #include <lineards.h>
 * @endcode
 *
 * Make sure to link your application with the library implementation when compiling.
 *
 * @section examples_sec Examples
 *
 * - Example 1: Creating and manipulating a vector:
 *   @code
 *   LINEAR_DS* vector = lds_new_vector(10, sizeof(int));
 *   lds_insert_last(vector, &some_value);
 *   // Further operations...
 *   lds_free(vector);
 *   @endcode
 *
 * - Example 2: Using a linked list as a stack:
 *   @code
 *   LINEAR_DS* stack = lds_new_list(sizeof(double));
 *   lds_stack_push(stack, &some_double_value);
 *   // Further operations...
 *   lds_free(stack);
 *   @endcode
 *
 * @section install How to install?
 *
 * To compile and install the library on the system, run the script setup.sh.
 * @code $ ./setup.sh @endcode
 *
 * The installation will be performed in the default directory: <code> /usr/local/ </code> .
 * If you want to install it in another location, pass the path as a parameter via the command line, as in the example below:
 * @code $ ./setup.sh /usr @endcode
 *
 * @section links_sec Additional Links
 *
 * - [GitHub Repository](https://github.com/iuricardoso/lineards)
 * - [Issue Tracker](https://github.com/iuricardoso/lineards/issues)
 *
 * @section author_sec Author
 *
 * This library and its documentation were created by:
 * - Name: Iuri S&ocirc;nego Cardoso
 * - Contact: iuri.cardoso@ifsc.edu.br
 *
 * @remarks This library and its documentation were constructed with the assistance of AI.
 *
 * @section license_sec License
 *
 * This library is licensed under the MIT License. See LICENSE file for more details.
 */
#ifndef LINEARDS_H
#define LINEARDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>

/**
 * @enum lds_return_t
 * @brief Indicates the success or failure of an operation involving the LINEAR_DS structure.
 *
 * Operations like insertion, deletion, querying, and value modification involving a LINEAR_DS structure
 * result in one of the values described by this enum.
 */
typedef enum {
    /**
     * @brief Indicates that the operation failed.
     * For insertion operations, it indicates that there is no more memory available.
     * For value modification operations, it indicates that the value was not changed because
     * the existing value is already the same.
     */
    LDS_FAIL=0,

    /**
     * @brief Indicates that the operation was successful.
     */
    LDS_SUCCESS=1,

    /**
     * @brief Indicates that the provided position is incorrect.
     * For insertion, deletion, querying, and data modification operations, it indicates that
     * the provided position parameter does not exist.
     */
    LDS_POS_ERR=2,

    /**
     * @brief Indicates that one or more mandatory parameters were passed as NULL.
     * Applies to all operations where a parameter of type LINEAR_DS* was passed as NULL.
     * For insertion, querying, and modification operations, it also occurs when the parameter
     * referring to the value to be inserted or modified, or the variable to receive the queried value,
     * is NULL.
     */
    LDS_NULL=3
} lds_return_t;

/**
 * @enum lds_type_t
 * @brief Supported types of linear data structures.
 */
typedef enum {
    /**
     * @brief Data structure stores elements in an array,
     * contiguous in memory.
     */
    LDS_VECTOR = 1,

    /**
     * @brief Data structure stores elements in a linked list,
     * non-contiguous.
     */
    LDS_LINKED_LIST = 2,

    /**
     * @brief Data structure type is unknown.
     */
    LDS_UNKNOWN = LDS_NULL
} lds_type_t;

/**
 * @typedef LINEAR_DS
 * @brief Definition of the opaque data structure for linear data structures.
 */
typedef struct LinearDS LINEAR_DS;

/**
 * @typedef LDS_ITERATOR
 * @brief Definition of the opaque iterator.
 */
typedef struct LDSIterator LDS_ITERATOR;


/* Funções de criação e destruição */

/**
 * @brief Creates a new linear data structure using an array (vector).
 *
 * This function allocates and initializes a new linear data structure that uses an array to store elements.
 *
 * @param initial_capacity Initial capacity of the array for storing elements.
 * @param data_size Size in bytes of each element to be stored in the structure.
 * @return A pointer to the newly created linear data structure (LINEAR_DS*).
 * @note The function dynamically allocates memory for the linear data structure. It is the caller's
 * responsibility to free this memory using lds_free() when it is no longer needed.
 * @see lds_free
 */
LINEAR_DS* lds_new_vector(size_t initial_capacity, size_t data_size);

/**
 * @brief Creates a new linear data structure using a linked list.
 *
 * This function allocates and initializes a new linear data structure that uses a linked list to store elements.
 *
 * @param data_size Size in bytes of each element to be stored in the structure.
 * @return A pointer to the newly created linear data structure (LINEAR_DS*).
 * @note The function dynamically allocates memory for the linear data structure. It is the caller's
 * responsibility to free this memory using lds_free() when it is no longer needed.
 */
LINEAR_DS* lds_new_list(size_t data_size);

/**
 * @brief Frees the memory allocated for a linear data structure.
 *
 * This function deallocates the memory allocated for a linear data structure.
 *
 * @param ds Pointer to the linear data structure to be freed.
 * @note The caller is responsible for ensuring that the pointer `ds` is valid and was previously
 * allocated by a function like lds_new_vector() or lds_new_list().
 */
void lds_free(LINEAR_DS *ds);


/* Funções principais de manipulação */
/**
 * @brief Inserts a value into the linear data structure at the specified position.
 *
 * This function inserts a value into the linear data structure at the specified position.
 *
 * @param ds Pointer to the linear data structure.
 * @param position Position where the value should be inserted.
 * @param value Pointer to the value to be inserted.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_insert(LINEAR_DS *ds, size_t position, void *value);

/**
 * @brief Inserts a value at the end of the linear data structure.
 *
 * This function inserts a value at the end of the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param value Pointer to the value to be inserted.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_insert_last(LINEAR_DS *ds, void *value);

/**
 * @brief Retrieves the value at the specified position in the linear data structure.
 *
 * This function retrieves the value at the specified position in the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param position Position from which to retrieve the value.
 * @param element Pointer to a memory location where the retrieved value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_get(LINEAR_DS *ds, size_t position, void *element);

/**
 * @brief Retrieves the last value from the linear data structure.
 *
 * This function retrieves the last value from the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param element Pointer to a memory location where the retrieved value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_get_last(LINEAR_DS *ds, void *element);

/**
 * @brief Sets the value at the specified position in the linear data structure.
 *
 * This function sets the value at the specified position in the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param position Position where the value should be set.
 * @param value Pointer to the value to be set.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_set(LINEAR_DS *ds, size_t position, void *value);

/**
 * @brief Removes the value at the specified position from the linear data structure.
 *
 * This function removes the value at the specified position from the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param position Position from which to remove the value.
 * @param removed_element Pointer to a memory location where the removed value will be stored.
 * if it is NULL, no value will stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_remove(LINEAR_DS *ds, size_t position, void *removed_element);

/**
 * @brief Removes the last value from the linear data structure.
 *
 * This function removes the last value from the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @param removed_element Pointer to a memory location where the removed value will be stored.
 * if it is NULL, no value will stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 * @note The caller is responsible for ensuring valid input parameters and proper memory management.
 */
lds_return_t lds_remove_last(LINEAR_DS *ds, void *removed_element);


/* Funções para consultar os campos da estrutura */
/**
 * @brief Returns the current number of elements in the linear data structure.
 *
 * This function retrieves the current number of elements stored in the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @return Current number of elements in the linear data structure. Zero if ds is NULL.
 */
size_t lds_size(LINEAR_DS *ds);

/**
 * @brief Returns the current capacity of the linear data structure.
 *
 * This function retrieves the current capacity of the linear data structure, which represents
 * the maximum number of elements that can be stored without reallocation.
 *
 * @param ds Pointer to the linear data structure.
 * @return Current capacity of the linear data structure. Zero if ds is NULL.
 * @see lds_new_vector
 * @see lds_new_list
 */
size_t lds_capacity(LINEAR_DS *ds);

/**
 * @brief Returns the size of each element stored in the linear data structure.
 *
 * This function retrieves the size, in bytes, of each element stored in the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @return Size of each element stored in the linear data structure. Zero if ds is NULL or
 * data strucuture type is LDS_LINKED_LIST.
 * @see lds_new_vector
 * @see lds_new_list
 * @see lds_type
 */
size_t lds_data_size(LINEAR_DS *ds);

/**
 * @brief Returns the type of the linear data structure.
 *
 * This function retrieves the type of the linear data structure, indicating whether it is
 * implemented using a vector, linked list, or is of unknown type.
 *
 * @param ds Pointer to the linear data structure.
 * @return Type of the linear data structure (lds_type_t).
 * @see lds_type_t
 * @see lds_new_vector
 * @see lds_new_list
 */
lds_type_t lds_type(LINEAR_DS *ds);



/* Funções de iterador */
/**
 * @brief Returns an iterator for traversing the elements of the linear data structure.
 *
 * This function returns an iterator that allows sequential access to the elements of the linear data structure.
 *
 * @param ds Pointer to the linear data structure.
 * @return An iterator (LDS_ITERATOR*) for the linear data structure.
 */
LDS_ITERATOR * lds_iterator(LINEAR_DS *ds);

/**
 * @brief Returns the current position of the iterator.
 *
 * This function retrieves the current position of the iterator within the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @return Current position of the iterator.
 */
size_t lds_it_position(LDS_ITERATOR *it);

lds_return_t lds_it_add(LDS_ITERATOR *it, void *value);/**
 * @brief Adds a new element at the current position of the iterator.
 *
 * This function adds a new element at the current position of the iterator in the linear data structure.
 * The new element is added on the current position. The old element from de current position is now located
 * on the next position.
 *
 * @param it Pointer to the iterator.
 * @param value Pointer to the value to be added.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_add(LDS_ITERATOR *it, void *value);

/**
 * @brief Moves the iterator to the next position in the linear data structure.
 *
 * This function moves the iterator to the next position in the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_next(LDS_ITERATOR *it);

/**
 * @brief Checks if there is a next element in the linear data structure.
 *
 * This function checks if there is a next element in the linear data structure that the iterator can move to.
 *
 * @param it Pointer to the iterator.
 * @return lds_return_t indicating whether there is a next element (LDS_SUCCESS) or not (LDS_FAIL).
 */
lds_return_t lds_it_has_next(LDS_ITERATOR *it);

/**
 * @brief Retrieves the element at the current position of the iterator.
 *
 * This function retrieves the element at the current position of the iterator in the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @param element Pointer to a memory location where the retrieved element will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_get(LDS_ITERATOR *it, void *element);

/**
 * @brief Sets the value of the element at the current position of the iterator.
 *
 * This function sets the value of the element at the current position of the iterator in the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @param value Pointer to the value to be set.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_set(LDS_ITERATOR *it, void *value);

/**
 * @brief Removes the element at the current position of the iterator.
 *
 * This function removes the element at the current position of the iterator from the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @param removed_element Pointer to a memory location where the removed element will be stored.
 * If it is NULL, no value is stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_remove(LDS_ITERATOR *it, void *removed_element);

/**
 * @brief Resets the iterator to the beginning of the linear data structure.
 *
 * This function resets the iterator to the beginning of the linear data structure, setting its position to the first element.
 *
 * @param it Pointer to the iterator.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_reset(LDS_ITERATOR *it);

/**
 * @brief Moves the iterator to a specific position in the linear data structure.
 *
 * This function moves the iterator to a specific position in the linear data structure.
 *
 * @param it Pointer to the iterator.
 * @param pos Position to which the iterator should be moved.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
lds_return_t lds_it_go(LDS_ITERATOR *it, size_t pos);


#ifndef __STDC_VERSION__
#define inline
#endif /* __STDC_VERSION__ */

#if __STDC_VERSION__ < 199901L
#define inline
#endif

/* Funções de pilha */
/**
 * @brief Pushes a value onto the stack.
 *
 * This function pushes a value onto the stack.
 *
 * @param ds Pointer to the linear data structure (stack).
 * @param value Pointer to the value to be pushed onto the stack.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_stack_push(LINEAR_DS * ds, void *value);

/**
 * @brief Pops the top value from the stack.
 *
 * This function removes and retrieves the top value from the stack.
 *
 * @param ds Pointer to the linear data structure (stack).
 * @param removed_element Pointer to a memory location where the removed value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_stack_pop(LINEAR_DS * ds, void *removed_element);

/**
 * @brief Retrieves the top value from the stack without removing it.
 *
 * This function retrieves the top value from the stack without removing it.
 *
 * @param ds Pointer to the linear data structure (stack).
 * @param top Pointer to a memory location where the top value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_stack_top(LINEAR_DS * ds, void *top);

/* Funções de fila */
/**
 * @brief Enqueues a value into the queue.
 *
 * This function enqueues a value into the queue.
 *
 * @param ds Pointer to the linear data structure (queue).
 * @param value Pointer to the value to be enqueued.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_enqueue(LINEAR_DS * ds, void *value);

/**
 * @brief Dequeues the front value from the queue.
 *
 * This function removes and retrieves the front value from the queue.
 *
 * @param ds Pointer to the linear data structure (queue).
 * @param removed_element Pointer to a memory location where the removed value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_dequeue(LINEAR_DS * ds, void *removed_element);

/**
 * @brief Retrieves the front value from the queue without removing it.
 *
 * This function retrieves the front value from the queue without removing it.
 *
 * @param ds Pointer to the linear data structure (queue).
 * @param front Pointer to a memory location where the front value will be stored.
 * @return lds_return_t indicating the result of the operation (LDS_SUCCESS or an error code).
 */
inline lds_return_t lds_queue_peek(LINEAR_DS * ds, void *front);

#ifdef NDEBUG
#define lds_debug(ds, out, debug_fmt)
#else
/* Funções para debug */
void lds_debug(LINEAR_DS * ds, FILE * out, void (*debug_fmt)(FILE * out, void *value));
#endif /* NDEBUG */


#ifdef __cplusplus
}
#endif


#endif /* LINEARDS_H */

