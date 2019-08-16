//
// Created by xyk on 19-7-3.
//

#include "Server.h"

static const int BUFFER_ALIGNMENT = 16;
static const double BUFFER_EXPANSION_MULTIPLIER = 1.3;

queue<TransportData> Server_Client::m_receiveQueue;
boost::mutex Server_Client::m_receiveQueueMutex;

queue<TransportData> Server_Client::m_sendQueue;
boost::mutex Server_Client::m_sendQueueMutex;

Server_Client::Server_Client(size_t initialBufferSize, bool tag) {
    m_isClosing = false;
    m_tag = tag;
    m_bufferSize = initialBufferSize;
    m_receivedBytes = 0;
}

int Server_Client::getListeningPort(){
    int port = 0;
    if(m_acceptor)
        port = m_acceptor->local_endpoint().port();

    return port;
}

void Server_Client::listento(int minPort, int maxPort) {
    if (sock)cout << "TCP sock existed, ignoring the listening request for minPort: " << minPort<<",maxPort: "<<maxPort << endl;
    else {
        uint32_t portRange = maxPort - minPort + 1;
        uint32_t port = rand() % portRange + minPort;
        sock.reset(new tcp::socket(m_io));
        m_acceptor.reset(new tcp::acceptor(m_io, tcp::endpoint(tcp::v4(), port)));
        m_acceptor->async_accept(*sock, std::bind(&Server_Client::accept_handler,
                                                  this, std::placeholders::_1, sock));
        cout << "TCP listening on " << m_acceptor->local_endpoint().address().to_string() << ":" << port << endl;
    }
    m_io.run();
}

void Server_Client::accept_handler(const boost::system::error_code &ec, Server_Client::sock_ptr sock) {
    if (ec) { return; }
    std::cout << "client:";
    std::cout << sock->remote_endpoint().address() << std::endl;
    receiveData();
}

void Server_Client::readPacketHandler(const boost::system::error_code &ec, std::size_t bytes) {
    if (m_isClosing)
        return;

    printf("Port#%d recieved data(%zu)", m_acceptor->local_endpoint().port(), bytes);
    uint32_t expectedLen = ntohl(*(reinterpret_cast<uint32_t *>(m_readHeader)));
    if (!ec || ec == boost::asio::error::message_size) {
        m_receivedBytes += bytes;
        if (expectedLen > m_receivedBytes) {
            printf("Expect to receive %u bytes, but actually received %zu bytes.", expectedLen, bytes);
            printf("Continue receiving %u bytes.", expectedLen - m_receivedBytes);
            sock->async_read_some(
                    boost::asio::buffer(m_receiveData.buffer.get() + m_receivedBytes, expectedLen - m_receivedBytes),
                    boost::bind(&Server_Client::readPacketHandler, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        } else {
            m_receivedBytes = 0;
//            cout<<m_receiveData.buffer.get()<<endl;
            boost::lock_guard<boost::mutex>lock(m_receiveQueueMutex);
            m_receiveQueue.push(m_receiveData);
            receiveData();
        }
    } else {
        printf("Error receiving data: %s", ec.message().c_str());
    }
}

void Server_Client::readHandler(const boost::system::error_code &ec, std::size_t bytes) {
    if (m_isClosing)return;

    if (!ec || ec == boost::asio::error::message_size) {
        if (!m_tag) {
//            cout << m_receiveData.buffer.get() << endl;
//            if(m_receiveQueue.size()>10)sleep(1);
            boost::lock_guard<boost::mutex>lock(m_receiveQueueMutex);
            m_receiveQueue.push(m_receiveData);
            receiveData();
            return;
        }
        uint32_t payloadlen = 0;
        assert(sock);
        m_receivedBytes += bytes;
        if (m_receivedBytes < 4) {
            printf("Incomplete header, continue receiving %u bytes\n", 4 - m_receivedBytes);
            sock->async_read_some(boost::asio::buffer(m_readHeader + m_receivedBytes, 4 - m_receivedBytes),
                                  boost::bind(&Server_Client::readHandler, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
        } else {
            payloadlen = ntohl(*(reinterpret_cast<uint32_t *>(m_readHeader)));
//            cout<<"payloadlen: "<<payloadlen<<endl;
            if (payloadlen > m_bufferSize) {
                m_bufferSize = ((payloadlen * BUFFER_EXPANSION_MULTIPLIER + BUFFER_ALIGNMENT - 1) / BUFFER_ALIGNMENT) *
                               BUFFER_ALIGNMENT;
                printf("Increasing the buffer size: %zu\n", m_bufferSize);
                m_receiveData.buffer.reset(new char[m_bufferSize]);
            }
            printf("readHandler(%zu):[%x,%x,%x,%x], payloadlen:%u", bytes, m_readHeader[0], m_readHeader[1],
                   (unsigned char) m_readHeader[2], (unsigned char) m_readHeader[3], payloadlen);

            m_receivedBytes = 0;
            sock->async_read_some(boost::asio::buffer(m_receiveData.buffer.get(), payloadlen),
                                  boost::bind(&Server_Client::readPacketHandler, this,
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
        }
    }
}

void Server_Client::receiveData() {
    if (!m_receiveData.buffer)
        m_receiveData.buffer.reset(new char[m_bufferSize]);

    assert(sock);
    if (m_tag) {
        sock->async_read_some(boost::asio::buffer(m_readHeader,4),
                boost::bind(&Server_Client::readHandler,this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else {
        sock->async_read_some(boost::asio::buffer(m_receiveData.buffer.get(), m_bufferSize),
                              boost::bind(&Server_Client::readHandler, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
    }
}

void Server_Client::connect(const std::string &ip, int port) {
    if (sock)cout << "TCP sock is existed" << endl;
    else {
        sock.reset(new tcp::socket(m_io));

        tcp::resolver resolver(m_io);
        tcp::resolver::query query(ip.c_str(), boost::to_string(port).c_str());
        tcp::resolver::iterator iterator = resolver.resolve(query);
        // TODO: Accept IPv6.
        sock->open(boost::asio::ip::tcp::v4());
        sock->async_connect(*iterator,
                            boost::bind(&Server_Client::connect_handler, this,
                                        boost::asio::placeholders::error));

       // tcp::endpoint m_ep(boost::asio::ip::address::from_string(ip), port);
       // sock->async_connect(m_ep, std::bind(&Server_Client::connect_handler, this, std::placeholders::_1));
       // m_io.run();
        if (m_workThread.get_id() == boost::thread::id()) {
            m_workThread = boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
        }
    }
}

void Server_Client::connect_handler(const boost::system::error_code &ec) {
    if (ec) {
        cout << "error:" << ec.message() << endl;
        return;
    }
    cout << "connected success" << endl;
}

void Server_Client::sync_write(char* buf,int len){
    assert(sock);
    boost::system::error_code ec;
    sock->write_some(boost::asio::buffer(buf,len),ec);
    if(ec) 
    {
        std::cout<<boost::system::system_error(ec).what()<<std::endl;
    }
}

void Server_Client::sendData() {
    // sock->async_write_some(boost::asio::buffer(buf,len),
    //                        std::bind(&Server_Client::write_handler, this, std::placeholders::_1));
    // TransportData data;
    // if(m_tag){
    //     data.buffer.reset(new char[len+4]);
    //     *(reinterpret_cast<uint32_t*>(data.buffer.get())) = htonl(len);
    //     memcpy(data.buffer.get()+4,buf,len);
    //     data.length = len + 4;
    // } else {
    //     data.buffer.reset(new char[len]);
    //     memcpy(data.buffer.get(),buf,len);
    //     data.length = len;
    // }
    boost::lock_guard<boost::mutex> lock(m_sendQueueMutex);
    // m_sendQueue.push(data);
    if (m_sendQueue.size() != 0)
        doSend();
}

void Server_Client::storeData(char* buf,int len){
    TransportData data;
    if(m_tag){
        data.buffer.reset(new char[len+4]);
        *(reinterpret_cast<uint32_t*>(data.buffer.get())) = htonl(len);
        memcpy(data.buffer.get()+4,buf,len);
        data.length = len + 4;
    } else {
        data.buffer.reset(new char[len]);
        memcpy(data.buffer.get(),buf,len);
        data.length = len;
    }
    boost::lock_guard<boost::mutex> lock(m_sendQueueMutex);
    m_sendQueue.push(data);
}

void Server_Client::doSend(){
    // if(!m_sendQueue.empty()){
    TransportData& data = m_sendQueue.front();
    assert(sock);
    sock->async_write_some(boost::asio::buffer(data.buffer.get(),data.length),
                           std::bind(&Server_Client::write_handler,this,std::placeholders::_1)); 
    // }    
}   

void Server_Client::write_handler(const boost::system::error_code &ec){
    printf("write complete\n");
    if(ec){
        printf("wrote data error: %s\n",ec.message().c_str());
    }
    boost::lock_guard<boost::mutex> lock(m_sendQueueMutex);
    assert(m_sendQueue.size() > 0);
    m_sendQueue.pop();
    if(m_sendQueue.size()>0)
        doSend();
}











