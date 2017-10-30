all:	no_show clr_cache_no_show

no_show:
	$(file >logs/log_no_imshow.txt,$^)
	./build.sh
	for n in 0 ; do \
	./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_no_imshow.txt $$n ; \
	done

clr_cache_no_show:
	$(file >logs/log_no_imshow_cache_clr.txt,$^)
	./build.sh
	for n in 0 ; do \
	sync; \
	sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a; \
	./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_no_imshow_cache_clr.txt $$n ; \
	done

show:
	$(file >logs/log_imshow.txt,$^)
	./build.sh ENABLE_IMSHOW
	for n in 5 10 20 50 100 200 ; do \
	./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_imshow.txt $$n ; \
	done

clr_cache_show:
	$(file >logs/log_imshow_cache_clr.txt,$^)
	./build.sh ENABLE_IMSHOW
	for n in 5 10 20 50 100 200 ; do \
	sync; \
	sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches' && swapoff -a && swapon -a; \
	./captureVid /home/rohit/Videos/train_vid/dashVid1.avi logs/log_imshow_cache_clr.txt $$n ; \
	done


