#include <utility>
#include "subject.h"
#include "observer.h"
#include "cell.h"
#include "subscriptions.h"
#include "info.h"

Cell::Cell() {}

bool Cell::getState() const { return isOn; }

Info Cell::getInfo() const {
    Info inf;
    /* data field */
    inf.row = r;
    inf.col = c;
    inf.state = isOn;

    return inf;
}

void Cell::setOn() {
    isOn = true;
	notifyObservers(SubscriptionType::All);
}

void Cell::toggle() {
    isOn = !isOn;
	notifyObservers(SubscriptionType::All);
	notifyObservers(SubscriptionType::SwitchOnly);
}

void Cell::setCoords(int r, int c) { this->r = r; this->c = c; }

void Cell::notify(Subject &) {
	isOn = !isOn;
	notifyObservers(SubscriptionType::All);
}

SubscriptionType Cell::subType() const {
  return SubscriptionType::SwitchOnly;
}

