#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
using namespace boost::asio;
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
                  , boost::noncopyable {
    typedef talk_to_svr self_type;
    talk_to_svr() 
      : sock_(service), started_(true) {}
    void start1(ip::tcp::endpoint ep) {
        sock_.async_connect(ep, MEM_FN1(on_connect,_1));
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_svr> ptr;
  
  static ptr start2(ip::tcp::endpoint ep) {
    ptr new_(new talk_to_svr());
        new_->start1(ep);
        return new_;
    }
  
    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }
    bool started() { return started_; }
private:
    void on_connect(const error_code & err) {
        if ( !err)      do_write();
        else            stop();
    }
   
    void on_write(const error_code & err, size_t bytes) {
      // do_read();
    }
   
    void do_write() {
        if ( !started() ) return;
	std::string msg="hello";
	std::string msg2="sorry";
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some( buffer(write_buffer_, msg.size()), 
                                MEM_FN2(on_write,_1,_2));
	std::copy(msg2.begin(), msg2.end(), write_buffer_);
	sock_.async_write_some( buffer(write_buffer_, msg.size()), 
                                MEM_FN2(on_write,_1,_2));
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string message_;
};

  int main(int argc, char* argv[]) {
    // connect several clients
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
   
    talk_to_svr::start2(ep);
       
    service.run();
  }
