# Maintainer: fuwa <main@fuwa.dev>
pkgname=i3-gulp
pkgver=0.0.2
pkgrel=1
url="https://github.com/ibuki2003/i3-gulp"
arch=('x86_64' 'i686')
license=('MIT')
makedepends=('git')
conflicts=()
replaces=()
backup=()
source=()
md5sums=()
depends=('libx11' 'i3-wm')

build() {
  cd ..
  make
}

package() {
  cd ..
  install -Dm755 gulp ${pkgdir}/usr/bin/gulp
}
