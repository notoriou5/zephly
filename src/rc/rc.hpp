#ifndef RC_HPP
#define RC_HPP

#include <msgs.hpp>
#include <stdint.h>

/**
 * @brief Call during initialization of system
 *
 * @details Initializes parser state machine and
 * variables. Also checks whether underlying uart
 * is available and enable interrupts.
 * 
 * @return int 0 or -error
 */
int rc_init();

/**
 * @brief Update state machine and parse new data
 * 
 * @param dt time in ms passed since last call
 * @return int 0 or -error
 */
int rc_update(uint32_t dt);

/**
 * @brief Start timer calling `rc_update` regularly
 */
void rc_run();

/**
 * @brief Get the latest rc commands
 * 
 * @return struct RCInput 
 */
Command rc_get();

#endif // RC_HPP
