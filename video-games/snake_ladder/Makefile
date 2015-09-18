# ========================================================================================
#  @file    : Makefile
#
#  @description : Makefile for all games.
#
#  @author  : Aman Kumar (2015)
#
#  @copyright   : The code contained herein is licensed under the GNU General Public License.
#               You may obtain a copy of the GNU General Public License Version 2 or later
#               at the following locations:
#               http://www.opensource.org/licenses/gpl-license.html
#               http://www.gnu.org/copyleft/gpl.html
# =========================================================================================*/

VERBOSE = @
BASEDIR = "SL_BOARD"
CC =gcc
NONE=\033[0m
GREEN=\033[01;32m
RED=\033[01;31m
YELLOW=\033[01;33m
CYAN=\033[01;36m
BIN_DIR=$(BASEDIR)/../bin
UTILS_DIR=$(BASEDIR)/utils
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
CFLAGS += -g -O3
CPPFLAGS += -I. \
		-I./inc/

.PHONY: clean all snake_ladder info

all: snake_ladder

snake_ladder:
	${VERBOSE}sed -i "s,PATH,$(UTILS_DIR),g" $(BASEDIR)/inc/sl_board.h
	${VERBOSE}gcc src/*.c ${CFLAGS} ${CPPFLAGS} -o ${BIN_DIR}/snake_ladder -lSDL
	${VERBOSE}sed -i "s,$(UTILS_DIR),"PATH",g" $(BASEDIR)/inc/sl_board.h

clean:
	${VERBOSE}rm -f $(BIN_DIR)/*
	${VERBOSE}echo "All Binaries ${RED}removed${NONE}"
