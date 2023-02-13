#include <iostream>
#include <vector>
#include "parser.h"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>

///FILE OPENLARDA SORUN VAR, ONLARI HALLET SONRA PIPE'İ HALLET EASY


using namespace std;

int main() {
    vector< string > bundles;
    vector < vector<parsed_input> > bundle_process;
    string inp;
    int is_bundle_creation = 0;
    int end_of_bundle = 0;
    while (true) {
        parsed_input parsed_inp;
        getline(cin, inp);
        inp = inp + "\n";
        char* line = strdup(inp.c_str());
        end_of_bundle = parse(line, is_bundle_creation, &parsed_inp);
        if (end_of_bundle == 0)
        {
            if (parsed_inp.command.type == command_type::PROCESS_BUNDLE_CREATE)
            {
                //This will handle Create Requests
                string name(parsed_inp.command.bundle_name);
                bundles.push_back(name);
                vector< parsed_input > process;
                bundle_process.push_back(process);
                is_bundle_creation = 1;
            }

            else if (parsed_inp.command.type == command_type::PROCESS_BUNDLE_EXECUTION)
            {
                //This will handle the execution
                string name(parsed_inp.command.bundles->name);
                int i = 0;
                for (const auto& elem : bundles)
                {
                    if (elem == name)
                    {
                        //This only redirects inpt output
                        int input = 0; int output = 0;
                        vector< parsed_input > curr_bundle =  bundle_process[i] ;
                        if (parsed_inp.command.bundles->input)
                        {
                            input = open(parsed_inp.command.bundles->input, O_RDONLY);
                        }
                        if (parsed_inp.command.bundles->output)
                        {
                            //char* out_line = const_cast<char*>(parsed_inp.command.bundles->output);
                            output = open(parsed_inp.command.bundles->output, O_WRONLY | O_CREAT | O_APPEND, 0777);
                        }



                        pid_t pids[curr_bundle.size()];
                        int j = 0;

                        for (auto process : curr_bundle)
                        {
                            if ((pids[j]=fork()) == 0)
                            {
                                if (input)
                                {
                                    dup2(input, 0);
                                    close(input);
                                }
                                if (output)
                                {
                                    dup2(output, 1);
                                    close(output);
                                }
                                execvp(process.argv[0], process.argv);
                                perror("execvp error happened.");
                                _exit(1);
                            }
                        }
                        for (int k = 0; k < curr_bundle.size(); k++) {
                            if (pids[k]) {
                                int status;
                                waitpid(pids[i], &status, 0);
                            }
                        }
                    }
                    else
                    {
                        i++;
                    }
                }
            }

             else if (parsed_inp.command.type == command_type::QUIT)
            {
                //This will handle STOP and reap forks and others
                exit(0);

            }

             else if (parsed_inp.command.type == command_type::PROCESS_BUNDLE_STOP)
            {
                //This will handle bundle creation ending
                 is_bundle_creation = 0;
            }

            else
            {
                //push the process in the according bundle package in array
                bundle_process[bundle_process.size() - 1].push_back(parsed_inp);
            }

        }

        else
        {
            is_bundle_creation = 0;
            //This will handle bundle creation ending
        }
    }

    //
    //cout << line;
    return 0;
}
