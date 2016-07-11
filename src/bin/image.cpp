#include "conwaygame_gui/image.h"
#include <conwaygame/memory.h>


namespace cg_gui {


image::image(QWidget* parent)
: QWidget{parent}, last_item_{},
  scene_{cg::make_unique<QGraphicsScene>(this)},
  view_{cg::make_unique<QGraphicsView>(scene_.get(), this)}
{
}

void image::update_image(const std::vector<bool>& data, int n, float scale) {
  const auto size = n * scale;
  view_->resize(size + 2, size + 2);

  if (last_item_) {
    scene_->removeItem(last_item_.get());
    last_item_.reset();
  }

  QImage image(n, n, QImage::Format_Mono);
  image.setColor(1, qRgb(255, 255, 255));
  image.setColor(0, qRgb(0, 0, 0));

  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      image.setPixel(i, j, data[i*n + j]);

  last_item_ = std::unique_ptr<QGraphicsPixmapItem>{
    scene_->addPixmap(QPixmap::fromImage(image.scaled(size, size)))
  };
}


}  // namespace cg_gui
