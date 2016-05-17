/*
 * micro_config.h
 *
 * Created: 5/14/2016 4:37:52 PM
 * Author: Gamal Mohamed
 */ 

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_


#ifndef F_CPU
#define F_CPU 1000000UL //1MHz Clock frequency
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#ifndef NOTHING
#define NOTHING 99
#endif

#endif /* MICRO_CONFIG_H_ */
