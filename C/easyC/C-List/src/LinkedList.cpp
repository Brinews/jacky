
template <class DT>
LinkedList<DT>::LinkedList()
{
	_info = NULL;
	_next = NULL;
}

template <class DT>
LinkedList<DT>::LinkedList(const DT& dt, LinkedList<DT>* next)
{
    // data copy
    _info = new DT(dt);
    if (_info == NULL) throw LinkedListMemory();

    _next = next;
}

template <class DT>
LinkedList<DT>::LinkedList(const LinkedList<DT>& ll)
{
	if (ll._info == NULL) info = NULL;
	else
	{
		_info = new DT(*(ll._info));
		if (_info == NULL) throw LinkedListMemory();
	}

	if (ll._next == NULL)
	{
		_next = NULL;
	}
	else
	{
        // deep copy
		_next = new LinkedList<DT>(*(ll._next));
		if (_next == NULL) throw LinkedListMemory();
	}
}

template <class DT>
LinkedList<DT>::~LinkedList()
{
	if (_info != NULL)
	{
		delete _info;
		_info = NULL;
	}
	if (_next != NULL)
	{
		delete _next;
		_next = NULL;
	}
}

template <class DT>
void LinkedList<DT>::copy(const LinkedList<DT>& ll)
{
	if (ll._info == NULL) _info = NULL;
	else
	{
		_info = new DT(*(ll._info));
		if (_info == NULL) throw LinkedListMemory();
	}
	if (ll._next == NULL) _next = NULL;
	else
	{
        // deep copy
		_next = new LinkedList<DT>(*(ll._next));
		if (_next == NULL) throw LinkedListMemory();
	}
}

template <class DT>
void LinkedList<DT>::operator=(const LinkedList<DT>& ll)
{
	if (_info != NULL) delete _info;
	if (_next != NULL) delete _next;

    // do a copy
	copy(ll);
}

template <class DT>
bool LinkedList<DT>::isEmpty()
{
	return (_info == NULL);
}

template <class DT>
DT& LinkedList<DT>::info()
{
	if (isEmpty()) throw LinkedListBounds();
	return *_info;
}

template <class DT>
int LinkedList<DT>::size()
{
	if (_next == NULL)
	{
		if (_info == NULL) return 0;
		else return 1;
	}
	else 
		return 1 + _next->size();
}

template <class DT>
DT& LinkedList<DT>::find(const DT& key)
{
	if (isEmpty())
	{
        throw LinkedListNotFound();
	}

	if (*_info == key )
	{
		return *_info;
	}

	if (_next == NULL)
	{
		throw LinkedListBounds();
	}

	return _next->find(key);
}

template <class DT>
DT& LinkedList<DT>::infoAt(int position)
{
	if (isEmpty()) throw LinkedListBounds();

	if (position == 0)
	{
		return *_info;
	}

	if (_next == NULL) throw LinkedListBounds();

	return _next->infoAt(position - 1);
}

template <class DT>
void LinkedList<DT>::add(const DT& object)
{
	if (_info == NULL)
	{
		_info = new DT(object);
	}
	else
	{
		LinkedList<DT>* newList = new LinkedList<DT>(*_info, _next);
		if (newList == NULL) throw LinkedListMemory();
		*_info = object; // new head
		_next = newList;
	}
}

template <class DT>
void LinkedList<DT>::insertAt(const DT& newObj, int position)
{
	if (position == 0) add(newObj);
	else
	{
		if (_next == NULL)
		{
			_next = new LinkedList(newObj, NULL);
			if (_next == NULL) throw LinkedListMemory();
		}
		else
		{
			_next->insertAt(newObj, position - 1);
		}
	}
}

template <class DT>
LinkedList<DT>* LinkedList<DT>::setNext(LinkedList<DT>* next)
{
	if (isEmpty()) throw LinkedListAttachToEmpty();

	LinkedList<DT>* temp = _next;
	_next = next;

	return temp;
}

template <class DT>
DT LinkedList<DT>::remove()
{
	if (isEmpty()) throw LinkedListBounds();

	DT temp = *_info;
	delete _info;

	if (_next == NULL)
	{
		_info = NULL;
	}
	else
	{
		LinkedList<DT>* oldnext = _next;

		_info = _next->_info;
		_next = _next->_next;

		oldnext->_info = NULL;
		oldnext->_next = NULL;
		delete oldnext;
	}

	return temp;
}

template <class DT>
DT LinkedList<DT>::removeAt(int position)
{
    if (isEmpty()) throw LinkedListBounds();
    if (position == 0) return remove();
    if (_next == NULL) throw LinkedListBounds();

    return _next->removeAt(position-1);
}

template <typename U>
ostream& operator<<(ostream& out, LinkedList<U>& ll)
{
    if (ll._info != NULL) {
        out << *(ll._info);
    }
    if (ll._next != NULL) {
        out << *(ll._next);
    }

    return out;
}
