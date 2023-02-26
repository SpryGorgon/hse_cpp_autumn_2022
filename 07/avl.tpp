#pragma once
template class Node<int, int>;
template class CustomAllocator<std::pair<int, int>>;
template class avl<int,int>;
//template class avl<int,int, std::less<int>, CustomAllocator<std::pair<int,int>>>;