#ifndef MCP3008_H__
#define MCP3008_H__

#include <stdint.h>

// This driver is built to use only the first analog pin for the moment
uint8_t mcp3008_read_value(void);

#endif
