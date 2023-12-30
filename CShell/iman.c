#include "iman.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void iman(int argc, char *command_name, int input_pipe, int output_pipe) {
    // Resolve DNS for man.he.net and retrieve the IP address
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Use IPv4
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo("man.he.net", "http", &hints, &res) != 0) {
        error("Error: Could not resolve host");
    }
    char url[] = "man.he.net";
    // Create a socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        error("Error: Could not create socket");
    }
    // Connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        error("Error: Could not connect to server");
    }
    // Send HTTP GET request for the specific page related to the command name
    char request[MAX_BUFFER_SIZE];
    sprintf(request, "GET /?topic=%s&section=all HTTP/1.1\r\nHost: %s\r\n\r\n", command_name, url);
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        return;
    }
    // Read and print the response
    char response[MAX_BUFFER_SIZE];
    int header_end = 0;
    int bytes_read;
    bool in_tag = false;
    bool in_desired_section = false;
    int flag = 0; // Flag to check if any content is printed
    while ((bytes_read = read(sockfd, response, sizeof(response))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (in_tag) {
                if (response[i] == '>') {
                    in_tag = false;
                }
            } else {
                if (response[i] == '<') {
                    in_tag = true;
                } else if (!in_tag) {
                    if (strncmp(response + i, "NAME", 4) == 0) {
                        in_desired_section = true;
                    } else if (strncmp(response + i, "SYNOPSIS", 8) == 0) {
                        in_desired_section = true;
                    } else if (strncmp(response + i, "DESCRIPTION", 11) == 0) {
                        in_desired_section = true;
                    }

                    if (in_desired_section) {
                        if (response[i] == '<') {
                            in_tag = true;
                        } else {
                        	flag = 1;
                            putchar(response[i]);
                        }
                    }
                }
            }
        }
    }
    // Close the socket
    close(sockfd);
    if (flag == 0) {
        printf("ERROR\n\t\tNo command found\n"); // Print "No command found" if nothing is printed
    }
    if (output_pipe != -1) {
        dup2(output_pipe, STDOUT_FILENO);
        close(output_pipe);
    }
    // Read from the input pipe if there is a valid input pipe
    if (input_pipe != -1) {
        dup2(input_pipe, STDIN_FILENO);
        close(input_pipe);
    }
}

