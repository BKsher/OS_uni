#include <boost/process.hpp>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

// Function that performs the binary operation on the given values
int perform_binary_operation(int f, int g) {
    // Perform binary operation
    return f * g;
}

int main() {
    // Create pipes for communication between processes
    HANDLE read_handle, write_handle;
    if (!CreatePipe(&read_handle, &write_handle, NULL, 0)) {
        std::cerr << "Error creating pipe" << std::endl;
        return 1;
    }

    // Start child process for computation f
    namespace bp = boost::process;
    bp::ipstream f_output;
    bp::child c_f("./f", bp::std_out > f_output);
    if (!c_f.valid()) {
        std::cerr << "Error starting child process for computation f" << std::endl;
        return 1;
    }

    // Start child process for computation g
    bp::ipstream g_output;
    bp::child c_g("./g", bp::std_out > g_output);
    if (!c_g.valid()) {
        std::cerr << "Error starting child process for computation g" << std::endl;
        return 1;
    }

    // Read values from pipes
    std::string f_str, g_str;
    f_output >> f_str;
    g_output >> g_str;

    // Convert values to integers
    int f = std::stoi(f_str);
    int g = std::stoi(g_str);

    // Perform binary operation
    int result = perform_binary_operation(f, g);

    // Handle soft fail and hard fail scenarios
    if (result == -1) {
        // Soft fail: try again until maximum amount of attempts is reached or value is obtained
        int max_attempts = 5;
        while (result == -1 && max_attempts > 0) {
            // Restart child processes
            c_f = bp::child("./f", bp::std_out > f_output);
            c_g = bp::child("./g", bp::std_out > g_output);

            char buffer[1024];
            DWORD bytes_read;

            // Read values from pipe
            if (!ReadFile(read_handle, buffer, 1024, &bytes_read, NULL)) {
                std::cerr << "Error reading from pipe" << std::endl;
                return 1;
            }

            // Parse values from buffer
            std::string str(buffer, bytes_read);
            std::vector<std::string> values;
            size_t pos = 0;
            while ((pos = str.find(' ')) != std::string::npos) {
                values.push_back(str.substr(0, pos));
                str.erase(0, pos + 1);
            }
            values.push_back(str);
            if (values.size() != 2) {
                std::cerr << "Error parsing values from buffer" << std::endl;
                return 1;
            }

            // Perform binary operation
            f = std::stoi(values[0]);
            g = std::stoi(values[1]);
            result = perform_binary_operation(f, g);

            // Decrement attempts counter
            --max_attempts;
        }

        // If result is still -1 after maximum amount of attempts, consider it a hard fail
        if (result == -1) {
            std::cerr << "Computation failed" << std::endl;
            return 1;
        }
    }

    // Print result to stdout
    std::cout << result << std::endl;

    // Wait for child processes to finish
    c_f.wait();
    c_g.wait();

    // Close handles
    CloseHandle(read_handle);
    CloseHandle(write_handle);

    return 0;

}