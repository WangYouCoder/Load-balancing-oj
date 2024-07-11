oj_server:oj_server.cc
	g++ -o $@ $^ -L/user/lib/mysql -lmysqlclient -lpthread -lctemplate -ljsoncpp
.PHONY:clean
clean:
	rm -rf oj_server