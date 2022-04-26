#Full directory; Or "." refers to the current directory of the Makefile
IDIR = C:\Users\Aleksandras\Desktop\VU\Procedurinis_Programavimas\C_programs\Char_Compress

#CC = gcc - creates a macro for gcc so we can compile files in C language
CC = gcc

#CFLAGS is the list of flags to pass for the compiler
# -I is a flag that makes gcc look in the directory for .h include file ...
# ... "." after -I would specify that gcc has to look in the current directory ...
# ... but $(IDIR) is used instead to tell the full path
# $(IDIR) tels the -I a specific directory to look in to
CFLAGS = -I $(IDIR)

#DEPS is a macro with a list of .h incude files on which .c files depend
DEPS = char_compress.h get_valid_int.h

#OBJ is a macro with a list of .o files on which .c files depend
OBJ = main.o char_compress.o get_valid_int.o


#Defines a rule for all .o files. The rule states that .o file depend on its .c version ...
#... and .h files in DEPS macro
# %.o: - file to create; %.c and $(DEPS) - file .c version and .h list dependancys
# TAB; $(CC) - using gcc; ...
# ... -o $@ - says that compilation output goes to file name on the left of ":" -> %.o"; ...
# ... -c is flag that compiles .c files to .o files; ...
# ... $< is the first dependacy file; $(CFLAGS) is a flag dependacy as defined above
%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)
	
#Defines a rule for making a file. 
#... and .h files in DEPS macro
# name: - file to create; $(OBJ) - dependancys
# TAB; $(CC) - using gcc; ...
# ... -o $@ - says that compilation output goes to file name on the left of ":" -> %.o"; ...
# ... $^ is files on the right of ":" -> (OBJ); $(CFLAGS) is a flag dependacy as defined above
test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

