/* function exec
in: command to execute in shell
out: executes entered command in shell */
std::string exec(const char* cmd)
{
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");

    while (fgets(buffer, sizeof buffer, pipe) != NULL)
    {
    	result += buffer;
    }
            
    pclose(pipe);
    return result;
}
