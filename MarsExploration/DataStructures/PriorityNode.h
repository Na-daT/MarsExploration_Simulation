#ifndef P_NODE
#define P_NODE
template < typename T>
class PriorityNode
{
private:
	T item; // A data item
	PriorityNode<T>* next; // Pointer to next node
	int Priority;
public:
	PriorityNode();
	PriorityNode(const T& r_Item, int pr);
	PriorityNode(const T& r_Item, PriorityNode<T>* nextNodePtr, int pr);
	void setItem(const T& r_Item);
	void setNext(PriorityNode<T>* nextNodePtr);
	void setPriority(int pr);
	int getPriority() const;
	T getItem() const;
	PriorityNode<T>* getNext() const;
}; // end Node


template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
	Priority = 0;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, int pr)
{
	item = r_Item;
	Priority = pr;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, PriorityNode<T>* nextNodePtr, int pr)
{
	item = r_Item;
	Priority = pr;
	next = nextNodePtr;
}
template<typename T>
inline void PriorityNode<T>::setPriority(int pr)
{
	if (pr < 0)
		return;

	Priority = pr;
}
template<typename T>
inline int PriorityNode<T>::getPriority() const
{
	return Priority;
}
template < typename T>
void PriorityNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T PriorityNode<T>::getItem() const
{
	return item;
}

template < typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}
#endif