#include "Textures.h"

#include <cassert>

using namespace std;

// Класс текстуры
// Обертка над вектором строк
class Texture : public ITexture {
public:
    Texture(Image image) : image_(move(image)) {
        // Проверяем, что все строки одинаковой длины
        // В релизном режиме эта строка пропадет
        for (const auto& line : image_) {
            assert(line.size() == image_[0].size());
        }
    }

    Size GetSize() const override {
        auto width = static_cast<int>(image_.empty() ? 0 : image_[0].size());
        auto height = static_cast<int>(image_.size());
        return {width, height};
    }

    const Image& GetImage() const override {
        return image_;
    }

private:
    Image image_;
};

// Создать прямоугольник из одинаковых символов
std::unique_ptr<ITexture> MakeTextureSolid(Size size, char pixel) {
  Image image(size.height, string(size.width, pixel));
  return make_unique<Texture>(move(image));
}

// Создать эллипс из одинаковых символов
std::unique_ptr<ITexture> MakeTextureCheckers(Size size, char pixel1,
                                              char pixel2) {
  Image image(size.height, string(size.width, pixel1));

  for (int i = 0; i < size.height; ++i) {
    for (int j = 0; j < size.width; ++j) {
      if ((i + j) % 2 != 0) {
        image[i][j] = pixel2;
      }
    }
  }

  return make_unique<Texture>(move(image));
}

std::unique_ptr<ITexture> MakeTextureCow() {
  Image image = {R"(^__^            )", 
                 R"((oo)\_______    )",
                 R"((__)\       )\/\)",
                 R"(    ||----w |   )",
                 R"(    ||     ||   )"};
  return make_unique<Texture>(move(image));
}
