SHELL := /bin/bash

.PHONY: all

all:	no_show

no_show:	$(file >logs/log_no_imshow.txt,$^)
	./build.sh;
	for (( i=1; i<6; i=i+1 )); \
	do \
		echo "Iteration $$i"; \
		./captureVid /home/rohit/Videos/train_vid/dashVid1conv.avi logs/log_no_imshow.txt 0; \
	done

clr_cache_no_show:
	$(file >logs/log_no_imshow_cache_clr.txt,$^)
	./build.sh
	for (( i=1; i<6; i=i+1 )); \
	do \
		echo "Iteration $$i"; \
		sync; \
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a; \
		./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_no_imshow_cache_clr.txt 0 ; \
	done

show:
	i=1
	$(file >logs/log_imshow.txt,$^)
	./build.sh ENABLE_IMSHOW
	for (( i=1; i<20; i=i+1 )); \
	do \
		echo "Iteration $$i"; \
		./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_imshow.txt 0 ; \
	done

clr_cache_show:
	i=1
	$(file >logs/log_imshow_cache_clr.txt,$^)
	./build.sh ENABLE_IMSHOW
	for (( i=1; i<20; i=i+1 )); \
	do \
		echo "Iteration $$i"; \
		sync; \
		sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a; \
		./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_imshow_cache_clr.txt 0 ; \
	done

cache_clr:
	sync; \
	sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a; 

