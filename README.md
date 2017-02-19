## Introduce

A simple C++ template library modified STL. Some of the parts I'm not intend to include：
**thread**,**chorno**： need to apdapt for different OS

**type_traits**,**iterator**：have to know lot about type cast rules and C++ template metaprogramming

**allocator**：[No need for it.](http://blog.csdn.net/Solstice/article/details/4401382)

**stack**：Just vector is enough

**execption**：google's and llvm's code standard both forbid execptions


different with STL：the assignment and compound assignment operators return void type instead of a refrence to this. 

### vector
Each time expand memory for 1.5time as before（similar to VC++ STL，not like libc++ for gcc which is twice）,so that the memory deallocated before may be used when continue calling push_back(). Same as STL, clear() will not deallocate the memory, only destruct the elements(so as deque/queue/string).

### deque/queue

Allocate a bigger size of memory when the elements touch the end instead of using circle array.

### utility
Use some template metaprogramming in swap() function, not the same way as STL to specialize for each swappable class.






