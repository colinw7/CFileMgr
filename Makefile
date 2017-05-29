all:
	cd src; make

clean:
	cd src; make clean
	rm -f lib/*.a
	rm -f bin/CQFileMgrTest
