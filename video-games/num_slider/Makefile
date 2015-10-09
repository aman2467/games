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
BASEDIR ="NUM_SLIDER"
CC =gcc
BIN_DIR=$(BASEDIR)/../bin
UTILS_DIR=$(BASEDIR)/utils
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
CFLAGS += -g -O3
CPPFLAGS += -I. \
		-I./inc/

.PHONY: clean all num_slider info

all: num_slider

num_slider:
	${VERBOSE}sed -i "s,PATH,$(UTILS_DIR),g" $(BASEDIR)/inc/num_slider.h
	${VERBOSE}gcc src/*.c ${CFLAGS} ${CPPFLAGS} -o ${BIN_DIR}/num_slider -lSDL
	${VERBOSE}sed -i "s,$(UTILS_DIR),"PATH",g" $(BASEDIR)/inc/num_slider.h

clean:
	${VERBOSE}rm -f $(BIN_DIR)/*
	${VERBOSE}echo "All Binaries ${RED}removed${NONE}"
