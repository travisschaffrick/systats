pkgname=systats
pkgver=1.0.0
pkgrel=1
pkgdesc="Simple ncurses-based system monitor"
arch=('x86_64')
url="https://github.com/travisschaffrick/systats"
license=('MIT')
depends=('ncurses')
makedepends=('gcc' 'make')
source=("$pkgname-$pkgver.tar.gz::$url/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('cdc0ab03504253b089d94f69cc1de9997ff5c717a0424d58c0917fcf36f24461')

build() {
    cd "$pkgname-$pkgver"
    make
}

package() {
    cd "$pkgname-$pkgver"
    install -Dm755 systats "$pkgdir/usr/bin/systats"
}
