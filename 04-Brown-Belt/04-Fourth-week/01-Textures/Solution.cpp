#include "Common.h"

using namespace std;

class BasicShape : public IShape {
public:
    // Возвращает точную копию фигуры.
    // Если фигура содержит текстуру, то созданная копия содержит ту же самую
    // текстуру. Фигура и её копия совместно владеют этой текстурой.
    void SetPosition(Point p) override {
        position_ = p;
    }
    Point GetPosition() const override {
        return position_;
    }

    void SetSize(Size s) override {
        size_ = s;
    }
    Size GetSize() const override {
        return size_;
    }

    void SetTexture(std::shared_ptr<ITexture> t) override {
        tex_ = t;
    }
    ITexture* GetTexture() const override {
        return tex_.get();
    }

    // Рисует фигуру на указанном изображении
    void Draw(Image& _image) const override {
        const char default_symbol = '.';
        for (int h = 0; h < size_.height; ++h) {
            for (int w = 0; w < size_.width; ++w) {
                if ((h + position_.y < _image.size()) && (w + position_.x < _image[h + position_.y].size())) {
                    if (IsPointInShape({w, h})) {
                        _image[h + position_.y][w + position_.x] = tex_.get() && h < tex_->GetSize().height && w < tex_->GetSize().width ? tex_->GetImage()[h][w] : default_symbol;
                    }
                }
            }
        }
    }
protected:
    Size size_ = {0, 0};
    Point position_ = {0, 0};
    std::shared_ptr<ITexture> tex_;
    virtual bool IsPointInShape(Point p) const = 0;
};


// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Rectangle : public BasicShape {
public:
    // Возвращает точную копию фигуры.
    // Если фигура содержит текстуру, то созданная копия содержит ту же самую
    // текстуру. Фигура и её копия совместно владеют этой текстурой.
    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Rectangle>(*this);
    }
private:
    bool IsPointInShape(Point p) const override {
        return true;
    }
};

class Ellipse : public BasicShape {
public:
    std::unique_ptr<IShape> Clone() const override {
        return make_unique<Ellipse>(*this);
    }
private:
    bool IsPointInShape(Point p) const override {
        return IsPointInEllipse(p, size_);
    }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    if (shape_type == ShapeType::Rectangle) {
        return make_unique<Rectangle>();
    }
    return make_unique<Ellipse>();
}
