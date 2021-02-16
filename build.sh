#!/bin/bash

make -C .. --quiet
clang -Wall -Wextra -g3 -I ../libft exec.c parser.c parser_utils.c debug_utils.c ../libft/libft.a