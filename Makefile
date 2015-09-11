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

.PHONY: clean all pong screen_saver snake_ladder tic_tac_toe info install

all: pong screen_saver snake_ladder tic_tac_toe info

pong:
	${VERBOSE}sed -i "s,PATH,$(UTILS_DIR),g" $(BASEDIR)/pong/inc/pong.h
	${VERBOSE}gcc pong/src/*.c ${CFLAGS} ${CPPFLAGS} -o ${BIN_DIR}/pong `sdl-config --cflags --libs`
	${VERBOSE}sed -i "s,$(UTILS_DIR),"PATH",g" $(BASEDIR)/pong/inc/pong.h

screen_saver:
	${VERBOSE}gcc screen_saver/*.c ${CFLAGS} -o ${BIN_DIR}/screen_saver `sdl-config --cflags --libs`

snake_ladder:
	${VERBOSE}gcc snake-ladder/*.c ${CFLAGS} -o ${BIN_DIR}/snake_ladder

tic_tac_toe:
	${VERBOSE}gcc tic-tac-toe/*.c ${CFLAGS} -o ${BIN_DIR}/tic_tac_toe

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
	${VERBOSE}echo "        ${GREEN}1. pong${NONE}"
	${VERBOSE}echo "        ${GREEN}2. screen_saver${NONE}"
	${VERBOSE}echo "        ${GREEN}3. snake_ladder${NONE}"
	${VERBOSE}echo "        ${GREEN}4. tic_tac_toe${NONE}"
	${VERBOSE}echo " "
	${VERBOSE}echo "${CYAN}========================================================================================"
	${VERBOSE}echo " "

install:
	${VERBOSE}sudo mkdir -p /usr/share/pong
	${VERBOSE}sudo cp ${UTILS_DIR}/pong.png /usr/share/pong/
	${VERBOSE}sudo cp ${BIN_DIR}/pong /usr/local/bin/
	${VERBOSE}sudo cp ${UTILS_DIR}/pong.desktop /usr/share/applications/
