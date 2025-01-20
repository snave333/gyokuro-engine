#ifndef I_DRAWABLE_H
#define I_DRAWABLE_H

struct IDrawable {
    virtual ~IDrawable() {}

    virtual void Draw() = 0;
};

#endif // I_DRAWABLE_H