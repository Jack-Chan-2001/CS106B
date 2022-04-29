/*
 * File: gtypes.cpp
 * ----------------
 * This file implements the classes in the gtypes.h interface.
 * 
 * @version 2017/09/29
 * - updated to use composite hashCode function
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include <graphics/gtypes.h>
#include <cmath>
#include <string>
#include <system/error.h>
#include <util/gmath.h>
#include <collections/hashcode.h>
#include <util/strlib.h>

/*
 * Implementation notes: GPoint class
 * ----------------------------------
 * The GPoint class itself is entirely straightforward.  The relational
 * operators compare the x components first, followed by the y component.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GPoint::GPoint() {
    x = 0;
    y = 0;
}

GPoint::GPoint(double x, double y) {
    this->x = x;
    this->y = y;
}

double GPoint::getX() const {
    return x;
}

double GPoint::getY() const {
    return y;
}

std::string GPoint::toString() const {
    return "(" + realToString(x) + ", " + realToString(y) + ")";
}

std::ostream& operator <<(std::ostream& os, const GPoint& pt) {
    return os << pt.toString();
}

bool operator ==(const GPoint& p1, const GPoint& p2) {
    return floatingPointEqual(p1.x, p2.x)
            && floatingPointEqual(p1.y, p2.y);
}

bool operator !=(const GPoint& p1, const GPoint& p2) {
    return !(p1 == p2);
}

int hashCode(const GPoint& pt) {
    return hashCode2(pt.x, pt.y);
}

/*
 * Implementation notes: GDimension class
 * --------------------------------------
 * The GDimension class itself is entirely straightforward.  The
 * relational operators compare the width first, followed by the height.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GDimension::GDimension() {
    width = 0;
    height = 0;
}

GDimension::GDimension(double width, double height) {
    this->width = width;
    this->height = height;
}

double GDimension::getWidth() const {
    return width;
}

double GDimension::getHeight() const {
    return height;
}

std::string GDimension::toString() const {
    return "(" + realToString(width) + ", " + realToString(height) + ")";
}

std::ostream& operator <<(std::ostream& os, const GDimension& dim) {
    return os << dim.toString();
}

bool operator ==(const GDimension& d1, const GDimension& d2) {
    return floatingPointEqual(d1.width, d2.width)
            && floatingPointEqual(d1.height, d2.height);
}

bool operator !=(const GDimension& d1, const GDimension& d2) {
    return !(d1 == d2);
}

int hashCode(const GDimension& dim) {
    return hashCode2(dim.width, dim.height);
}

/*
 * Implementation notes: GRectangle class
 * --------------------------------------
 * The GRectangle class itself is entirely straightforward.  The
 * relational operators compare the components in the following order:
 * x, y, width, height.  The hashCode function computes the exclusive-or
 * of the individual words.
 */

GRectangle::GRectangle() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

GRectangle::GRectangle(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

double GRectangle::getX() const {
    return x;
}

double GRectangle::getY() const {
    return y;
}

double GRectangle::getWidth() const {
    return width;
}

double GRectangle::getHeight() const {
    return height;
}

bool GRectangle::isEmpty() const {
    return width <= 0 || height <= 0;
}

bool GRectangle::contains(double x, double y) const {
    return x >= this->x && y >= this->y
            && x < this->x + width
            && y < this->y + height;
}

bool GRectangle::contains(GPoint pt) const {
    return contains(pt.getX(), pt.getY());
}

std::string GRectangle::toString() const {
    return "(" + realToString(x) + ", " + realToString(y) + ", "
            + realToString(width) + ", " + realToString(height) + ")";
}

std::ostream& operator <<(std::ostream& os, const GRectangle& rect) {
    return os << rect.toString();
}

bool operator ==(const GRectangle& r1, const GRectangle& r2) {
    return floatingPointEqual(r1.x, r2.x)
            && floatingPointEqual(r1.y, r2.y)
            && floatingPointEqual(r1.width, r2.width)
            && floatingPointEqual(r1.height, r2.height);
}

bool operator !=(const GRectangle& r1, const GRectangle& r2) {
    return !(r1 == r2);
}

int hashCode(const GRectangle& r) {
    return hashCode4(r.x, r.y, r.width, r.height);
}
