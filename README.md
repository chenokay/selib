SUMMARY

this is a library which collect all kinds of useful source met in work. I am the coder for most of them, and 
some, very great code block, are coded by others-colleague, friends etc. which will be marked in the code.

CONTENT

util:

	bit_array.h : bit map, a great number of count but only two value in one position
 
	arrary_lock.h : amount of lock with bit flag
 
	lifo.h && lifo.hpp : a stack for thread safety under read and write
 
	autolock.h : a lock with RAII mechanism based on type
 
	singleton.h : process and thread singleton, especially thread singleton can manage, generate, get in thread field gobally and release thread data automatically
 
	auto_release.h : resouce release with RAII
 
	file_watcher.h : check whether is file updated
 
	hash_fun.h : fast hash function from facebook
 
	lock_free_atomic_ops.h : automic operations
 
	path.h : path join and split
 
	timer.h : time monitor
 

monitor:

	cpu_switch_monitor.h : monitor a cpu back time ine one thread with sleep, which can describe the cpu load to some degree
 

algorithm: computer algorithm test

	
container:

	chain_hash.h : a chain hash
 
	circle_cache.h : circle buffer cache
 ![image](https://github.com/chenokay/selib/assets/8099748/1d0c4d7a-f4e5-4802-b53b-e1ae8563efde)

 
	dict_container.h : two buffer dict container which can manage all kinds of dict
 
	pchain_map.h : a thread-safety hashmap
 
	pqueue.h : a light-weight thread-safety high performance queue based link
 
	resource_stack.h : a thread-safety stack
 
	sequence_recorder.h : record event and do sume simple summary
 

framework:

	hining_pool.h : a input queue and output queu with threads to process
 
	dict_manager.* : manager all kinds of dict
 

ml_util: machine learning util



algorighm: 

    tree.h:

    迭代方式打印二叉树核心思想：
    1）每一次循环是对二叉树根，左右三个节点处理切面
    2）确认满足需求打印节点思路，并在三个节点处理上体现这个思路
    3）思路包括：处理顺序，打印，入栈顺序 这几个核心处理要素
    4）充分利用栈后进先出特性和遍历顺序特性，这两个顺序组成需求顺序
