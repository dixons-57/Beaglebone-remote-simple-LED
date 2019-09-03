#projects src files are in ./src
SRC_DIR := ./src

#object files will be placed in ./obj
OBJ_DIR := ./obj

#the final name of the executable
EXE_NAME := main

#this finds all source files ending in .cpp
SRC_FILES := $(filter-out $(SRC_DIR)/test_main.cpp, $(wildcard $(SRC_DIR)/*.cpp))

#this lists all object files by substituting the extension in all the source files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))


#give the ip address and user account of the beaglebone black for ssh purposes - typical defaults are given here
SSHIP := 192.168.7.2
USER := debian


#point to the desired target location for the program on the beaglebone (note that ~ does NOT refer to local home directory, but home directory of user specified above in USER variable)
TARGETLOC = ~/projects/LEDflashproject


#point to the g++ linaro cross-compiler here, first g++, then the includes folder
COMPILER = ${HOME}/crosscomp/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
INCLUDES = ${HOME}/crosscomp/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/include


#build rules

all: build send run #default target builds main exe then runs remotely over ssh

build: $(EXE_NAME) #build the executable

$(EXE_NAME): $(OBJ_FILES) #once all object files are up-to-date
	${COMPILER} -g -o $@ $^ #link them all using g++ linaro into the executable
	
send: #comms commands
	-ssh -q $(USER)@$(SSHIP) "rm -rf ${TARGETLOC}"  #delete the target folder on the beaglebone, ignores if error
	ssh -q $(USER)@$(SSHIP) "mkdir ${TARGETLOC}"  #recreate the target folder
	scp "./$(EXE_NAME)" "$(USER)@$(SSHIP):${TARGETLOC}" #secure copy the executable over


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp #builds the given object file if the corresponding source file has changed
	${COMPILER} -I"${INCLUDES}" -g -c -o $@ $< #-g gives gdb support

run: #runs the program remotely using ssh
	ssh $(USER)@$(SSHIP) "${TARGETLOC}/${EXE_NAME}"


#clean stuff

clean: #just wipes exe and object folder, ignores any errors
	-rm "./$(EXE_NAME)"
	-rm -rf "./$(OBJ_DIR)/*"
