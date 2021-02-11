#!/bin/bash

make -C .. --quiet
clang -Wall -Wextra -g3 -I ../libft $1 ../libft/libft.a