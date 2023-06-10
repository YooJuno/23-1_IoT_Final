//g++ -std=c++11 Server.cpp -o Server -lboost_system -pthread
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

using namespace boost::asio;

class Server {
public:
    Server(io_service& ioService, short port) : acceptor_(ioService, ip::tcp::endpoint(ip::tcp::v4(), port)),
                                               socket_(ioService) {
        accept();
    }

private:
    void accept() {        
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!ec) {
                clients_.push_back(std::make_shared<ip::tcp::socket>(std::move(socket_)));      
                std::cout << "New Client Connected\n";
                          
                std::thread t(&Server::startReading, this, clients_.back());
                t.detach();

                
            }

            accept();
        });
    }

    
    
    void startReading(std::shared_ptr<ip::tcp::socket> clientSocket) {
	    std::shared_ptr<std::array<char, 2048>> buffer = std::make_shared<std::array<char, 2048>>();

	    clientSocket->async_read_some(
		boost::asio::buffer(*buffer),
		[clientSocket, buffer,this](const boost::system::error_code& ec, size_t bytesRead) {
		    if (ec == boost::asio::error::eof) {
		        // Client disconnected
		        handleDisconnect(clientSocket);
		    } else if (ec) {
		        std::cerr << "Error reading from client: " << ec.message() << std::endl;
		    } else {
		    
			if (clients_.size() == 3 && first_try == false) {
		            additionalAction();
		            loadingbroadcast("Play");
		            first_try = true;
		        }
		        // Process the received message
		        std::string message(buffer->data(), bytesRead);
		        
		        
		        std::vector<std::string> tokens;
		        std::stringstream ss(message);
		        std::string token;
		        
		        while (std::getline(ss, token, ':')) {
                            tokens.push_back(token);
                        }
                        
                        if (tokens.size() >= 2) {
		            name = tokens[0];
		            key = tokens[1];
		            
		            if(name == "username")
		            	userlist.push_back(tokens[1]);
		            	
		            std::cout << userlist.size() << std::endl;
	
		            std::cout << "[name]: [" << name << "]\n[key]: [" << key << "]" << std::endl;
		        }
		        
		        for(int i = 0 ; i < userlist.size() ; i++){
		            	if(userlist[i] == tokens[0]) {
		            		numbering = i;
		            		std::cout << userlist[i] << std::endl;
		            		break;
		            	}		
		            }
		      
		      	std::stringstream dd;
		      	dd << numbering;
		      	dd << ":";
		      	dd << key;
		      	std::string result = dd.str();
		      	
		      	if(key == "81" || key == "82" || key == "83" || key == "84") {
		      		broadcast(result, clientSocket);
		      	}

		        // Continue reading asynchronously
		        startReading(clientSocket);
		    }
		});
    }

    void additionalAction() {
        std::cout << "All 4 clients have connected.\n";
    }

    void handleDisconnect(std::shared_ptr<ip::tcp::socket> clientSocket) {
        auto it = std::find_if(clients_.begin(), clients_.end(), [clientSocket](const std::shared_ptr<ip::tcp::socket>& client) {
            return client.get() == clientSocket.get();
        });

        if (it != clients_.end()) {
            std::cout << "Client disconnected\n";
            clients_.erase(it);
        }
    }

    void broadcast(const std::string& message, std::shared_ptr<ip::tcp::socket> sender) {
        for (const auto& client : clients_) {
            boost::asio::write(*client, boost::asio::buffer(message));
            // 자기 자신에게 보낸 메시지는 제외하고 전송
            //if (client != sender) {
            //    boost::asio::write(*client, boost::asio::buffer(message));
            //}
        }
    }

    void loadingbroadcast(const std::string& message) {
        for (const auto& client : clients_) {
            boost::asio::write(*client, boost::asio::buffer(message));
        }
    }

    ip::tcp::acceptor acceptor_;
    ip::tcp::socket socket_;
    std::vector<std::shared_ptr<ip::tcp::socket>> clients_;
    std::string name;
    std::vector<std::string> userlist;
    int numbering;
    bool first_try = false;
    std::string key;
};

int main() {
    try {
        boost::asio::io_service ioService;
        Server server(ioService, 2222);

        std::thread t([&ioService]() {
            ioService.run();
        });
        t.join();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}

