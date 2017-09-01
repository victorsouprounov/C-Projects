#COMPILE
	To compile run: make
    ./make
	To clear
	./make clear 
	(deletes mydiz and mydiz.o)

#RUN
	Command line example:
	./mydiz -c this filea fileb
	
	this: will create this.di file
	filea: a filea we want to store in this.di
	fileb: a fileb we want to store in this.di
	
	
Invocation:
	user can use mydiz with -c , -j , -m , -x , -p and -q.
	[! -j comes only with -c !]
		
	
# -c
	if it is a directory we store as name the path of the file this dir contains.
	
# -x
	we exctract from .di file all the containing files. if it is zipped (zip==1) we unzip it. then we chmod to the original
	rights and we change the timestamp.
	
# -j
	our program calls with execlp gzip and creates the .gz's after that we store them in 
	the .di file. in the end we unzip the zipped files and return to their starting status.(chmod , utime)
# -m
	prints out to the screen the metadata struct we used(dictionary). even if its zipped or not.
# -p
	prints out the hierachy of the files. checking every path that is stored in .di file.
# -q
	returns if the file/list given exists or not in the .di file. checks all the paths stored in the .di


	


	