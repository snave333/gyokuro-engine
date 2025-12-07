#ifndef I_DRAWABLE_H
#define I_DRAWABLE_H

namespace gyo {

struct IDrawable {
    virtual ~IDrawable() {}

    virtual void Draw() const = 0;
};

} // namespace gyo

#endif // I_DRAWABLE_H
