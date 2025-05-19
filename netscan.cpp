// C++ implementaion netowrk scanner written by Nathan W Jone nat@davaosecurity.com
// g++ -o port_scanner port_scanner.cpp     ./port_scanner
// This code is a basic implementation and may not handle all edge cases or errors robustly.
// Scanning ports can be considered intrusive; ensure you have permission to scan the target IP address.
// The code is designed for Unix-like systems (Linux, macOS). If you're using Windows, you may need to adjust the socket functions and include the appropriate headers.

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void scanPort(const std::string& ip, int port, std::ofstream& htmlFile) {
    int sock;
    struct sockaddr_in server;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Could not create socket\n";
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Try to connect to the port
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
        htmlFile << "<tr><td>" << port << "</td><td>Open</td></tr>\n";
    } else {
        htmlFile << "<tr><td>" << port << "</td><td>Closed</td></tr>\n";
    }

    close(sock);
}

int main() {
    std::string ip;
    int startPort, endPort;

    std::cout << "Enter IP address to scan: ";
    std::cin >> ip;
    std::cout << "Enter start port: ";
    std::cin >> startPort;
    std::cout << "Enter end port: ";
    std::cin >> endPort;

    std::ofstream htmlFile("port_scan_results.html");
    if (!htmlFile) {
        std::cerr << "Error creating HTML file\n";
        return 1;
    }

    // Write HTML header
    htmlFile << "<html>\n<head><title>Port Scan Results</title></head>\n<body>\n";
    htmlFile << "<h1>Port Scan Results for " << ip << "</h1>\n";
    htmlFile << "<table border='1'>\n<tr><th>Port</th><th>Status</th></tr>\n";

    // Scan ports
    for (int port = startPort; port <= endPort; ++port) {
        scanPort(ip, port, htmlFile);
    }

    // Write HTML footer
    htmlFile << "</table>\n</body>\n</html>";
    htmlFile.close();

    std::cout << "Port scan completed. Results saved to port_scan_results.html\n";
    return 0;
}
