/*
cmdparser.c - Parser de command line para paso de argumentos, detección de errores de sintaxis y comprobación de comandos por callback
Creado por Juan Ignacio Causse (61105)
Instituto Tecnológico de Buenos Aires
Estructuras de Datos y Algoritmos 1C2021
*/

#include "cmdparser.h"

//Error codes
#define ERROR_OPTION_NO_KEY -1
#define ERROR_LAST_OPTION_NO_VALUE -2
#define ERROR_ARG_IS_NOT_VALID -3
#define ERROR_ARG_OPT_NUM_NOT_MATCH -4
#define ERROR_ARG_NUM_NOT_MATCH -5
#define ERROR_OPT_NUM_NOT_MATCH -6
#define ERROR_BOTH_ARG_AND_OPT_NUM_NOT_MATCH -7

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData, unsigned short expectedArgs, unsigned short expectedOptions){
    unsigned short index = 1, readTotal = 0, argsCounter = 0, optionsCounter = 0;
    short error = 0;
    bool exit = ((argc > 1) ? false : true), enoughArgsForOption = true, inputIsValid, ignoreOptArgNumVerification = false;
    char *arg1, *arg2;
    //INITIAL VERIFICATIONS
    if ((expectedArgs == 0) && (expectedOptions == 0))
        ignoreOptArgNumVerification = true;
    if (!ignoreOptArgNumVerification) {
        if (argc != (expectedArgs + 2 * expectedOptions + 1)) {  //Options contain two arguments. + 1 to exclude executable path
            error = ERROR_ARG_OPT_NUM_NOT_MATCH;
            exit = true;
        }
    }
    //MAIN PARSER LOOP
    while (!exit){
        //Check for non-read args
        if (readTotal == argc - 1){          //None left (ignoring executable file path)
            exit = true;
            continue;
        }
        else if (readTotal == argc - 2){          //1 arg left. It cannot be an option since options need two args
            enoughArgsForOption = false;
        }
        //Get args if available
        arg1 = argv[index++];
        readTotal++;
        if ((*arg1) == '-'){                    //Options (key starting in '-' + value)
            if (*(arg1+1) == '\0'){             //All argv arguments are null terminated
                error = ERROR_OPTION_NO_KEY;
                exit = true;
                continue;
            }
            else if (!enoughArgsForOption){     //Last argument
                error = ERROR_LAST_OPTION_NO_VALUE;
                exit = true;
                continue;
            }
            arg2 = argv[index++];
            readTotal++;
            optionsCounter++;   //This is needed to count key + value (option) as 1 argument
            inputIsValid = (bool) p(arg1, arg2, userData);   //userData is unmodified
        }
        else{                                   //Parameters (not starting in '-')
            argsCounter++;
            inputIsValid = (bool) p(NULL, arg1, userData);   //userData is unmodified
        }
        if (!inputIsValid){
            error = ERROR_ARG_IS_NOT_VALID;
            exit = true;
            continue;
        }
    }
    //FINAL VERIFICATION
    if (!ignoreOptArgNumVerification) {
        if (expectedArgs != argsCounter) {
            if (expectedOptions != optionsCounter) {
                error = ERROR_BOTH_ARG_AND_OPT_NUM_NOT_MATCH;
            }
            else
                error = ERROR_ARG_NUM_NOT_MATCH;
        }
        else if (expectedOptions != optionsCounter)
            error = ERROR_OPT_NUM_NOT_MATCH;
    }
    return ((error == 0) ? (readTotal - optionsCounter) : error);
}