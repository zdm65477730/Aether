#include "Container.hpp"

namespace Aether {
    Container::Container(int x, int y, int w, int h) : Element(x, y, w, h) {
        this->focussed = nullptr;
    }

    void Container::addElement(Element * e) {
        e->setInactive();
        if ((e->selectable() || e->hasSelectable()) && this->focussed == nullptr) {
            this->setFocussed(e);
        }
        Element::addElement(e);
    }

    bool Container::handleEvent(InputEvent * e) {
        // Don't handle event if hidden!
        if (this->hidden() || this->focussed == nullptr) {
            return false;
        }

        // Default behaviour is to pass to focussed
        if (this->focussed->handleEvent(e)) {
            return true;
        }

        // If children didn't handle it, shift focus between them
        switch (e->type()) {
            case EventType::ButtonPressed:
                switch (e->button()) {
                    // Cur: current element that's focussed
                    // Pot: potential element to move to
                    case Button::DPAD_RIGHT:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->x() >= cur->x() + cur->w());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow(pot->x() - (cur->x() + cur->w()), 2) + pow((pot->y() + pot->h()/2) - (cur->y() + cur->h()/2), 2));
                        });
                        break;

                    case Button::DPAD_LEFT:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->x() + pot->w() <= cur->x());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()) - cur->x(), 2) + pow((pot->y() + pot->h()/2) - (cur->y() + cur->h()/2), 2));
                        });
                        break;

                    case Button::DPAD_UP:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->y() + pot->h() <= cur->y());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()/2) - (cur->x() + cur->w()/2), 2) + pow((pot->y() + pot->h()) - cur->y(), 2));
                        });
                        break;

                    case Button::DPAD_DOWN:
                        return moveHighlight(this, [](Element * cur, Element * pot) {
                            return (pot->y() >= cur->y() + cur->h());
                        },
                        [](Element * cur, Element * pot){
                            return sqrt(pow((pot->x() + pot->w()/2) - (cur->x() + cur->w()/2), 2) + pow(pot->y() - (cur->y() + cur->h()), 2));
                        });
                        break;
                }
                break;
        }

        return false;
    }

    void Container::setActive() {
        if (this->focussed != nullptr) {
            this->focussed->setActive();
        }
    }

    void Container::setInactive() {
        if (this->focussed != nullptr) {
            this->focussed->setInactive();
        }
    }

    void Container::setFocussed(Element * e) {
        if (this->focussed != nullptr) {
            this->focussed->setInactive();
        }
        this->focussed = e;
        e->setActive();
    }

    bool moveHighlight(Container * parent, std::function<bool(Element *, Element*)> check, std::function<int(Element *, Element *)> dist) {
        int minDist = std::numeric_limits<int>::max();
        Element * mv = nullptr;

        // Iterate over all children and keep valid one with smallest distance
        for (size_t i = 0; i < parent->children.size(); i++) {
            if (parent->children[i] == parent->focussed || !(parent->children[i]->selectable() || parent->children[i]->hasSelectable())) {
                continue;
            }

            if (check(parent->focussed, parent->children[i])) {
                int tmpDist = dist(parent->focussed, parent->children[i]);
                if (tmpDist < minDist) {
                    minDist = tmpDist;
                    mv = parent->children[i];
                }
            }
        }

        // If one found change focus
        if (mv != nullptr) {
            parent->focussed->setInactive();
            parent->focussed = mv;
            parent->focussed->setActive();
            return true;
        }
        return false;
    }
};