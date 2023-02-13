#include "stock_photo_webstore.hpp"

StockPhotoWebstore::StockPhotoWebstore() {}

StockPhotoWebstore &StockPhotoWebstore::addPhoto(const Photo &photo) {
    tpbst.insert(photo.getCategory(), photo.getName(), photo);

    return *this; // do not edit this line.
}

StockPhotoWebstore &StockPhotoWebstore::removePhoto(const std::string &category, const std::string &name) {
    /* IMPLEMENT THIS */

    return *this; // do not edit this line.
}

StockPhotoWebstore &StockPhotoWebstore::updatePrice(const std::string &category, const std::string &name, int newPrice) {
    /* IMPLEMENT THIS */

    return *this; // do not edit this line.
}

StockPhotoWebstore &StockPhotoWebstore::printAllPhotos() {
    /* IMPLEMENT THIS */

    return *this; // do not edit this line.
}

StockPhotoWebstore &StockPhotoWebstore::printAllPhotosInCategory(const std::string &category) {
    /* IMPLEMENT THIS */

    return *this; // do not edit this line.
}

StockPhotoWebstore &StockPhotoWebstore::printPhoto(const std::string &category, const std::string &name) {
    /* IMPLEMENT THIS */

    return *this; // do not edit this line.
}