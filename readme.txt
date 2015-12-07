開発環境：
VM ware workstation
LUbuntu

コンパイラ：
g++(gcc)

コマンド：
g++ server.cpp -o server -lboost_system -lboost_thread -lpthread
g++ client.cpp -o client -lboost_thread -lboost_system -lpthread