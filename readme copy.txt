Luis Duque and Oscar Hedblad
System Software HW 4

Steps to Compile and Run:

1. Transfer "Driver.c", "Scanner.c", "Parser.c", "VM.c", "Header.h" and whatever input file that is to be used into Eustis account using either Filezilla or PSFTP through Putty (I used Filezilla). Connect to "eustis.eecs.ucf.edu" using NID and password, then transfer necessary files.

2. Open up command-line such as terminal or you can use putty, and ssh into "eustis.eecs.ucf.edu" using NID and password again.

3. When connected, type in "bash" to be able to use keys such as "backspace" and the arrow keys.

4. Type in "gcc -o Project4 Driver.c Scanner.c Parser.c VM.c". This step creates the executable file called "Project4".

5. Type in "./Project4" followed by desired arguments to run the executable file.

6. When run, the program will prompt you to enter the name of the input file. Type in the name of the file and hit enter and the output will appear.