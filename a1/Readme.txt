V_number: V00946075
Section : CSC360
Name    : Shanhengchen

start:

1. make file:   simply enter make then make file create a pman 

2. run program: input ./pman to run the program 

functions:

3. fun_BG:  while enter bg ./test or bg ./(runable program) pman will create a child process and run test or (runable program)
    notice: -----------> if you just input bg, you will get a "Invalid input\n"
    notice: -----------> if you dont enter a compliable file, you will get a message Error on execvp and the program will end!

4. bglist: will print out current (runing/sleep/stop) backgound processes created by fun_BG
    notice: -----------> once the you call the (bgkill pid) then the pid process will no longer in bglist

5. func_BGkill: will send the TERM signal to the job with process ID pid and recycle the terminated process
    notice: -----------> you can input an unexist pid but nothing changed

6. func_BGstop: will send the STOP signal to the job pid
    notice: -----------> you can input an unexist pid but nothing changed

7. func_BGstart: will send the CONT signal to the job pid
    notice: -----------> you can input an unexist pid but nothing changed

8. func_pstat will display pids' comm,state,rss,utime,stime,voluntary_ctxt_switches and nonvoluntary_ctxt_switches