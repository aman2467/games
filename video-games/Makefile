# =========================================================================================
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
BASEDIR = $(PWD)
CC =gcc
NONE=\033[0m
GREEN=\033[01;32m
RED=\033[01;31m
YELLOW=\033[01;33m
CYAN=\033[01;36m
BIN_DIR=$(BASEDIR)/bin
UTILS_DIR=$(BASEDIR)/pong/utils
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
CFLAGS += -g -O3
CPPFLAGS += -I. \
		-I./inc/ \
		-I./pong/inc

.PHONY: clean all num_slider pong snake_ladder info install

all: num_slider pong snake_ladder info

num_slider:
	${VERBOSE}sed -i "s,NUM_SLIDER,$(BASEDIR)/num_slider,g" $(BASEDIR)/num_slider/Makefile
	${VERBOSE}make -s -C num_slider/
	${VERBOSE}sed -i "s,$(BASEDIR)/num_slider,"NUM_SLIDER",g" $(BASEDIR)/num_slider/Makefile

pong:
	${VERBOSE}sed -i "s,PATH,$(UTILS_DIR),g" $(BASEDIR)/pong/inc/pong.h
	${VERBOSE}gcc pong/src/*.c ${CFLAGS} ${CPPFLAGS} -o ${BIN_DIR}/pong -lSDL -lSDL_mixer
	${VERBOSE}sed -i "s,$(UTILS_DIR),"PATH",g" $(BASEDIR)/pong/inc/pong.h

snake_ladder:
	${VERBOSE}sed -i "s,SL_BOARD,$(BASEDIR)/snake_ladder,g" $(BASEDIR)/snake_ladder/Makefile
	${VERBOSE}make -s -C snake_ladder/
	${VERBOSE}sed -i "s,$(BASEDIR)/snake_ladder,"SL_BOARD",g" $(BASEDIR)/snake_ladder/Makefile

clean:
	${VERBOSE}rm -f $(BIN_DIR)/*
	${VERBOSE}echo "All Binaries ${RED}removed${NONE}"

info:
	${VERBOSE}echo "                                         ${YELLOW}<<#|#>> "
	${VERBOSE}echo "                                         -(${RED}o o${YELLOW})-${NONE}"
	${VERBOSE}echo "${CYAN}======================================${YELLOW}oOO${CYAN}==${YELLOW}(${RED}^${YELLOW})${CYAN}==${YELLOW}OOo${CYAN}=====================================${NONE}"
	${VERBOSE}echo " "
	${VERBOSE}echo "${YELLOW}Binary Path :" 
	${VERBOSE}echo "        ${GREEN}${BIN_DIR}/${NONE}"
	${VERBOSE}echo "${YELLOW}Binaries :"
	${VERBOSE}echo "        ${GREEN}1. num_slider${NONE}"
	${VERBOSE}echo "        ${GREEN}2. pong${NONE}"
	${VERBOSE}echo "        ${GREEN}3. snake_ladder${NONE}"
	${VERBOSE}echo " "
	${VERBOSE}echo "${CYAN}========================================================================================"
	${VERBOSE}echo " "

install:
	${VERBOSE}sudo mkdir -p /usr/share/pong
	${VERBOSE}sudo cp ${UTILS_DIR}/pong.png /usr/share/pong/
	${VERBOSE}sudo cp ${BIN_DIR}/pong /usr/local/bin/
	${VERBOSE}sudo cp ${UTILS_DIR}/pong.desktop /usr/share/applications/
