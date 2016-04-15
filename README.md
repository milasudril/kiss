# kiss
Usage:
    kiss dictionary ... < file
or
    command | kiss dictionary ...

Kiss Is a Simple Substitution filter that follows the KISS princlple. `kiss`
replaces variables in found in standard input with values from loaded
dictionaries. The result is written to standard output.

Dictionary entries take the form

    variable:value

and are delimited by newline

Variables in the input file are delmited by `$` and any whitespace or
punctuation character.

In both input stream and dictionaries, `_` acts as escape character.
