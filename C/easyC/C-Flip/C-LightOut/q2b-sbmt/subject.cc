#include "subject.h"
#include "observer.h"
#include "subscriptions.h"

void Subject::attach(Observer *o) { 
	observers.push_back(o);
}

void Subject::notifyObservers(SubscriptionType t) {
	for (unsigned int i = 0; i < observers.size(); i++) {
		if (t == observers[i]->subType())
			observers[i]->notify(*this);
	}
}

Subject::~Subject() {}
