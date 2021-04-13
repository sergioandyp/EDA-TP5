/*
cmdparser.h - Parser de command line para paso de argumentos, detección de errores de sintaxis y comprobación de comandos por callback
Creado por Juan Ignacio Causse (61105)
Instituto Tecnológico de Buenos Aires
Estructuras de Datos y Algoritmos 1C2021
*/

#ifndef PARSER_H
#define PARSER_H

#include <iostream>

typedef int (*pCallback) (char *key, char *value, void *userData);          
/*
User-defined Callback for data processing (key, value, command line value for key).

* Key and value are unmodified
* Parser user must define Callback function "pCallback" with given prototype
* The function must return 0 if an argument error occurred, and 1 if there was no error
*/

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData, unsigned short expectedArgs, unsigned short expectedOptions);
/*
Command line parser.

parseCmdLine receives:
int argc => a number containing the number of arguments, including the executable path. It is highly recommended to use standard argc.
char *argv[] => an array of null-terminated strings containing the arguments. It is highly recommended to use standard argv.
pCallback p => a pCallback type pointer, pointing to a user-defined function which provides argument processing criteria.
void *userdata => void pointer for data storing. IT IS NOT USED OR MODIFIED by parseCmdLine.
unsigned short expectedArgs => Number of non-option arguments expected.
unsigned short expectedOptions => Number of options expected (key + value = option).
	expectedArgs and expectedOptions allow the parser to be used for specific applications where the number of args and
	options expected are known at compile-time (e.g. data input, programs needing a file path input).
	If expectedArgs = 0 and expectedOptions = 0, the parser will ignore this verification.

parseCmdLine returns:
-1 if there is an option without key
-2 if last option has no value
-3 if callback returned error
-4 if the number of options and arguments does not match the expected value.
	(The parser returns this error if argc is different than (expectedArgs + 2 * expectedOptions + 1)).
-5 if the number of arguments found does not match the expected value (expectedArgs).
-6 if the number of options found does not match the expected value (expectedOptions).
-7 if the two previous errors happen at the same time
else: number of options (key + value) + number of args
*/

#endif /*PARSER_H*/