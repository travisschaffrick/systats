pkgname=sysinfo
pkgver=1.0.0
pkgrel=1
pkgdesc="Simple ncurses-based system monitor"
arch=('x86_64')
url="https://github.com/travisschaffrick/sysinfo"
license=('MIT')
depends=('ncurses')
makedepends=('gcc' 'make')
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('55a36887cbba17a11d721bc2b274ce5c7159621eafd5625a4e9df0970e7c83b9')

build() {
    cd "$pkgname-$pkgver"
    make
}

package() {
    cd "$pkgname-$pkgver"
    install -Dm755 sysinfo "$pkgdir/usr/bin/sysinfo"
}
