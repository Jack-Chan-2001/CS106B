/*
 * File: gevents.cpp
 * -----------------
 * This file implements the machine-independent functions for the classes
 * in the gevents.h interface.  The actual functions for receiving events
 * from the environment are implemented in the platform package.
 * 
 * @version 2016/11/26
 * - added WINDOW_CLOSING event
 * - added isCtrlOrCommandKeyDown
 * @version 2016/11/20
 * - added operator << for all event types
 * @version 2016/10/16
 * - added GEvent.isShiftKeyDown
 * - alphabetized methods
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

/*
 * Implementation notes:
 * ---------------------
 * The classes in this hierarchy are defined in an unusual way
 * for C++ in that all instance variables are part of the top-level
 * class.  The advantage of this design is that the GEvent class
 * then has all of the memory it needs to store an event of any of
 * its subclasses.
 */

#include <graphics/gevents.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <system/error.h>
#include "gtable.h"
#include <graphics/gtimer.h>
#include <graphics/gtypes.h>
#include <collections/map.h>
#include "private/platform.h"
#include "private/static.h"


GEvent::GEvent() {
    eventClass = NULL_EVENT;
    eventType = 0;
    valid = false;
    modifiers = 0;
    eventTime = 0.0;
    gwd = nullptr;
    source = nullptr;
    gtd = nullptr;
    x = 0;
    y = 0;
    keyChar = 0;
    keyCode = 0;
}

EventClassType GEvent::getEventClass() const {
    return eventClass;
}

double GEvent::getEventTime() const {
    return eventTime;
}

EventType GEvent::getEventType() const {
    return EventType(eventType);
}

int GEvent::getModifiers() const {
    return modifiers;
}

bool GEvent::isValid() {
    return valid;
}

bool GEvent::isAltKeyDown() const {
    return (modifiers & ALT_DOWN) != 0;
}

bool GEvent::isCtrlKeyDown() const {
    return (modifiers & CTRL_DOWN) != 0;
}

bool GEvent::isCtrlOrCommandKeyDown() const {
    return isCtrlKeyDown() || isMetaKeyDown();
}

bool GEvent::isMetaKeyDown() const {
    return (modifiers & META_DOWN) != 0;
}

bool GEvent::isShiftKeyDown() const {
    return (modifiers & SHIFT_DOWN) != 0;
}

void GEvent::setEventTime(double time) {
    eventTime = time;
}

void GEvent::setModifiers(int modifiers) {
    this->modifiers = modifiers;
}

std::string GEvent::toString() const {
    if (eventClass == 0) {
        return "GEvent(NULL)";
    } else if (eventClass == WINDOW_EVENT) {
        GWindowEvent windowEvent(*this);
        return (&windowEvent)->toString();
    } else if (eventClass == ACTION_EVENT) {
        GActionEvent actionEvent(*this);
        return (&actionEvent)->toString();
    } else if (eventClass == MOUSE_EVENT) {
        GMouseEvent mouseEvent(*this);
        return (&mouseEvent)->toString();
    } else if (eventClass == KEY_EVENT) {
        GKeyEvent keyEvent(*this);
        return (&keyEvent)->toString();
    } else if (eventClass == TIMER_EVENT) {
        GTimerEvent timerEvent(*this);
        return (&timerEvent)->toString();
    } else {
        return "GEvent(?)";
    }
}


GActionEvent::GActionEvent() {
    valid = false;
}

GActionEvent::GActionEvent(GEvent e) {
    valid = e.valid && e.eventClass == ACTION_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        source = e.source;
        actionCommand = e.actionCommand;
        gwd = e.gwd;
    }
}

GActionEvent::GActionEvent(EventType type, GObject* source,
                           std::string actionCommand) {
    this->eventClass = ACTION_EVENT;
    this->eventType = int(type);
    this->source = source;
    this->actionCommand = actionCommand;
    valid = true;
}

std::string GActionEvent::getActionCommand() const {
    if (!valid) {
        error("GActionEvent::getActionCommand: Event is not valid");
    }
    return actionCommand;
}

GObject* GActionEvent::getSource() const {
    return source;
}

std::string GActionEvent::toString() const {
    if (!valid) {
        return "GActionEvent(?)";
    }
    std::ostringstream os;
    os << "GActionEvent:ACTION_PERFORMED(" << actionCommand << ")";
    return os.str();
}


GKeyEvent::GKeyEvent() {
    valid = false;
}

GKeyEvent::GKeyEvent(GEvent e) {
    this->eventClass = KEY_EVENT;
    valid = e.valid && e.eventClass == KEY_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        keyChar = e.keyChar;
        keyCode = e.keyCode;
        gwd = e.gwd;
    }
}

GKeyEvent::GKeyEvent(EventType type, const GWindow& gw,
                     int keyChar, int keyCode) {
    this->eventClass = KEY_EVENT;
    this->eventType = int(type);
    this->gwd = gw.gwd;
    this->keyChar = keyChar;
    this->keyCode = keyCode;
    valid = true;
}

GWindow GKeyEvent::getGWindow() const {
    return GWindow(gwd);
}

char GKeyEvent::getKeyChar() const {
    if (!valid) {
        error("GKeyEvent::getKey: Event is not valid");
    }
    return char(keyChar);
    // Think about wide characters at some point
}

int GKeyEvent::getKeyCode() const {
    if (!valid) {
        error("GKeyEvent::getKey: Event is not valid");
    }
    return keyCode;
}

std::string GKeyEvent::toString() const {
    if (!valid) return "GKeyEvent(?)";
    std::ostringstream os;
    os << "GKeyEvent:";
    int ch = '\0';
    switch (eventType) {
    case KEY_PRESSED:  os << "KEY_PRESSED";   ch = keyCode; break;
    case KEY_RELEASED: os << "KEY_RELEASED";  ch = keyCode; break;
    case KEY_TYPED:    os << "KEY_TYPED";     ch = keyChar; break;
    }
    if (isprint(ch)) {
        os << "('" << char(ch) << "')";
    } else {
        os << std::oct << "('\\" << ch << "')";
    }
    return os.str();
}


GMouseEvent::GMouseEvent() {
    valid = false;
}

GMouseEvent::GMouseEvent(GEvent e) {
    valid = e.valid && e.eventClass == MOUSE_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        x = e.x;
        y = e.y;
        gwd = e.gwd;   // JL
    }
}

GMouseEvent::GMouseEvent(EventType type, const GWindow& gw,
                         double x, double y) {
    this->eventClass = MOUSE_EVENT;
    this->eventType = int(type);
    this->gwd = gw.gwd;
    this->x = x;
    this->y = y;
    valid = true;
}

GWindow GMouseEvent::getGWindow() const {
    return GWindow(gwd);
}

double GMouseEvent::getX() const {
    if (!valid) {
        error("GMouseEvent::getX: Event is not valid");
    }
    return x;
}

double GMouseEvent::getY() const {
    if (!valid) {
        error("GMouseEvent::getY: Event is not valid");
    }
    return y;
}

bool GMouseEvent::isLeftClick() const {
    return !isMiddleClick() && !isRightClick();
}

bool GMouseEvent::isMiddleClick() const {
    return (getModifiers() & BUTTON3_DOWN) != 0;
}

bool GMouseEvent::isRightClick() const {
    return (getModifiers() & BUTTON2_DOWN) != 0;
}

std::string GMouseEvent::toString() const {
    if (!valid) return "GMouseEvent(?)";
    std::ostringstream os;
    os << "GMouseEvent:";
    switch (eventType) {
    case MOUSE_PRESSED:  os << "MOUSE_PRESSED";   break;
    case MOUSE_RELEASED: os << "MOUSE_RELEASED";  break;
    case MOUSE_CLICKED:  os << "MOUSE_CLICKED";   break;
    case MOUSE_MOVED:    os << "MOUSE_MOVED";     break;
    case MOUSE_DRAGGED:  os << "MOUSE_DRAGGED";   break;
    }
    os << "(" << x << ", " << y << ")";
    return os.str();
}


GServerEvent::GServerEvent(EventType type, int requestID, const std::string& requestUrl) {
    this->eventClass = SERVER_EVENT;
    this->eventType = int(type);
    this->requestID = requestID;
    this->requestUrl = requestUrl;
    valid = true;
}

int GServerEvent::getRequestID() const {
    return requestID;
}

std::string GServerEvent::getRequestURL() const {
    return requestUrl;
}

std::string GServerEvent::toString() const {
    if (!valid) return "GServerEvent(?)";
    std::ostringstream out;
    if (eventType == SERVER_REQUEST) {
        out << "GServerEvent:SERVER_REQUEST(id="
            << requestID << " url=\"" << requestUrl << "\")";
    }
    return out.str();
}

GServerEvent::GServerEvent() {
    valid = false;
}

GServerEvent::GServerEvent(GEvent e) {
    valid = e.valid && e.eventClass == SERVER_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        requestID = e.requestID;
        requestUrl = e.requestUrl;
        value = e.value;
        // gwd = e.gwd;
    }
}


GTableEvent::GTableEvent(GEvent e) {
    valid = e.valid && e.eventClass == TABLE_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        row = e.row;
        column = e.column;
        value = e.value;
        gwd = e.gwd;
    }
}

GTableEvent::GTableEvent(EventType type) {
    this->eventClass = TABLE_EVENT;
    this->eventType = int(type);
    valid = true;
}

int GTableEvent::getColumn() const {
    return column;
}

int GTableEvent::getRow() const {
    return row;
}

std::string GTableEvent::getValue() const {
    return value;
}

void GTableEvent::setLocation(int row, int column) {
    this->row = row;
    this->column = column;
}

void GTableEvent::setValue(std::string value) {
    this->value = value;
}

std::string GTableEvent::toString() const {
    if (!valid) {
        return "GTableEvent(?)";
    }
    std::ostringstream out;
    out << "GTableEvent:";
    switch (eventType) {
    case TABLE_COPY:          out << "TABLE_COPY";          break;
    case TABLE_CUT:           out << "TABLE_CUT";           break;
    case TABLE_EDIT_BEGIN:    out << "TABLE_EDIT_BEGIN";    break;
    case TABLE_PASTE:         out << "TABLE_PASTE";         break;
    case TABLE_REPLACE_BEGIN: out << "TABLE_REPLACE_BEGIN"; break;
    case TABLE_SELECTED:      out << "TABLE_SELECTED";      break;
    case TABLE_UPDATED:       out << "TABLE_UPDATED";       break;
    }
    out << "(r" << row << "c" << column;
    if (eventType == TABLE_UPDATED) {
        out << " \"" << value << "\"";
    }
    out << ")";
    return out.str();
}


GTimerEvent::GTimerEvent() {
    valid = false;
}

GTimerEvent::GTimerEvent(GEvent e) {
    valid = e.valid && e.eventClass == TIMER_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        gtd = e.gtd;
        // gwd = e.gwd;
    }
}

GTimerEvent::GTimerEvent(EventType type, const GTimer& timer) {
    this->eventClass = TIMER_EVENT;
    this->eventType = int(type);
    this->gtd = timer.gtd;
    valid = true;
}

GTimer GTimerEvent::getGTimer() const {
    return GTimer(gtd);
}

std::string GTimerEvent::toString() const {
    if (!valid) {
        return "GTimerEvent(?)";
    }
    return "GTimerEvent:TIMER_TICKED()";
}


GTableEvent::GTableEvent() {
    valid = false;
}


GWindowEvent::GWindowEvent() {
    valid = false;
}

GWindowEvent::GWindowEvent(GEvent e) {
    valid = e.valid && e.eventClass == WINDOW_EVENT;
    if (valid) {
        eventClass = e.eventClass;
        eventType = e.eventType;
        modifiers = e.modifiers;
        eventTime = e.eventTime;
        gwd = e.gwd;
    }
}

GWindowEvent::GWindowEvent(EventType type, const GWindow& gw) {
    this->eventClass = WINDOW_EVENT;
    this->eventType = int(type);
    this->gwd = gw.gwd;
    valid = true;
}

GWindow GWindowEvent::getGWindow() const {
    return GWindow(gwd);
}

std::string GWindowEvent::toString() const {
    if (!valid) {
        return "GWindowEvent(?)";
    }
    std::ostringstream os;
    os << "GWindowEvent:";
    switch (eventType) {
    case WINDOW_CLOSED:      os << "WINDOW_CLOSED";       break;
    case WINDOW_CLOSING:     os << "WINDOW_CLOSING";      break;
    case WINDOW_RESIZED:     os << "WINDOW_RESIZED";      break;
    }
    os << "()";
    return os.str();
}


std::ostream& operator <<(std::ostream& out, const GEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GActionEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GKeyEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GMouseEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GServerEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GTableEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GTimerEvent& event) {
    return out << event.toString();
}

std::ostream& operator <<(std::ostream& out, const GWindowEvent& event) {
    return out << event.toString();
}


// free functions

GMouseEvent waitForClick() {
    GEvent event = waitForEvent(CLICK_EVENT);
    GMouseEvent mouseEvent(event);
    return mouseEvent;
}

GEvent waitForEvent(int mask) {
    return stanfordcpplib::getPlatform()->gevent_waitForEvent(mask);
}

GEvent getNextEvent(int mask) {
    return stanfordcpplib::getPlatform()->gevent_getNextEvent(mask);
}
