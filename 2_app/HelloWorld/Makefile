CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-

objs += hello.o

hello : $(objs)
	$(CROSS_COMPILE)g++ -o $@ $^
	
%.o : %.cpp
	$(CROSS_COMPILE)g++ -c $<

clean:
	rm -rf *.o hello	