#ifndef __MATH_CCGEOMETRY_H__
#define __MATH_CCGEOMETRY_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Size {
public:
    float width;
    float height;
public:

    Size();

    Size(float width, float height);

    Size(const Size &other);

    explicit Size(const vec2 &point);

    Size &operator=(const Size &other);

    Size &operator=(const vec2 &point);

    Size operator+(const Size &right) const;

    /**
     * @js NA
     * @lua NA
     */
    Size operator-(const Size &right) const;

    /**
     * @js NA
     * @lua NA
     */
    Size operator*(float a) const;

    /**
     * @js NA
     * @lua NA
     */
    Size operator/(float a) const;

    /**
    Set the width and height of Size.
     * @js NA
     * @lua NA
     */
    void setSize(float width, float height);

    /**
    Check if two size is the same.
     * @js NA
     */
    bool equals(const Size &target) const;

    /**Size(0,0).*/
    static const Size ZERO;
};

/**Rectangle area.*/
class Rect {
public:
    vec2 origin;
    /**Width and height of the rect.*/
    Size size;

public:
    /**
    Constructor an empty Rect.
     * @js NA
     */
    Rect();

    /**
    Constructor a rect.
     * @js NA
     */
    Rect(float x, float y, float width, float height);

    /**
     Constructor a rect.
     * @js NA
     */
    Rect(const vec2 &pos, const Size &dimension);

    /**
    Copy constructor.
     * @js NA
     * @lua NA
     */
    Rect(const Rect &other);

    /**
     * @js NA
     * @lua NA
     */
    Rect &operator=(const Rect &other);

    /**
    Set the x, y, width and height of Rect.
     * @js NA
     * @lua NA
     */
    void setRect(float x, float y, float width, float height);

    /**
    Get the left of the rect.
     * @js NA
     */
    float getMinX() const; /// return the leftmost x-value of current rect
    /**
    Get the X coordinate of center point.
     * @js NA
     */
    float getMidX() const; /// return the midpoint x-value of current rect
    /**
    Get the right of rect.
     * @js NA
     */
    float getMaxX() const; /// return the rightmost x-value of current rect
    /**
    Get the bottom of rect.
     * @js NA
     */
    float getMinY() const; /// return the bottommost y-value of current rect
    /**
    Get the Y coordinate of center point.
     * @js NA
     */
    float getMidY() const; /// return the midpoint y-value of current rect
    /**
    Get top of rect.
     * @js NA
     */
    float getMaxY() const; /// return the topmost y-value of current rect
    /**
    Compare two rects.
     * @js NA
     */
    bool equals(const Rect &rect) const;

    /**
    Check if the points is contained in the rect.
     * @js NA
     */
    bool containsPoint(const vec2 &point) const;

    /**
    Check the intersect status of two rects.
     * @js NA
     */
    bool intersectsRect(const Rect &rect) const;

    /**
    Check the intersect status of the rect and a circle.
     * @js NA
     */
    bool intersectsCircle(const vec2 &center, float radius) const;

    /**
    Get the min rect which can contain this and rect.
     * @js NA
     * @lua NA
     */
    Rect unionWithRect(const Rect &rect) const;

    /**Compute the min rect which can contain this and rect, assign it to this.*/
    void merge(const Rect &rect);

    /**An empty Rect.*/
    static const Rect ZERO;
};

#endif // __MATH_CCGEOMETRY_H__
