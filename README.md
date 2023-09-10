# gdk_pixbuf_convert.c

Short script for converting images, a fine alternative to ImageMagick for icon resizing in flatpak builds.

```sh
gcc -o gdk_pixbuf_convert $(pkg-config gdk-pixbuf-2.0 gio-2.0 --cflags --libs) ./gdk_pixbuf_convert.c
./gdk_pixbuf_convert -o icon_64.png -w 64 -r icon.ico
```

```yaml
modules:
  - name: example-app
    post-install:
      - |
        gcc -o gdk_pixbuf_convert $(pkg-config gdk-pixbuf-2.0 gio-2.0 --cflags --libs) gdk_pixbuf_convert.c
        for s in 16 32 48 64 128 256 512; do
          ./gdk_pixbuf_convert -w "${s}" -r -o "example.png" "example.ico"
          install -Dm644 example.png /app/share/icons/hicolor/${s}x${s}/apps/example.png"
        done
    sources:
      - type: archive
        url: https://example.com/example.tar.gz
      - type: file
        path: gdk_pixbuf_convert.c
    
```
