# Font Generation

The `-Layout.csv` files were generated using [msdf-atlas-gen](https://github.com/Chlumsky/msdf-atlas-gen), and Adobe's [Source Code Pro](https://fonts.adobe.com/fonts/source-code-pro) font. These files are used to render text using the multi-channel signed distance field (MSDF) technique.

The corresponding `-Atlas.png` files are located in the `resources/textures` directory.

The following command was run in the msdf-atlas-gen repo to generate the files (after building with cmake).

```zsh
build/bin/msdf-atlas-gen \
    -font "SourceCodePro-Regular.ttf" \
    -chars "[32, 126]" \
    -type mtsdf \
    -format png \
    -pots \
    -imageout "SourceCodePro-Regular-Atlas.png" \
    -csv "SourceCodePro-Regular-Layout.csv" \
    -pxrange 6
```

> Note that the command output indicated a resulting glyph size of `33.9375 pixels/em`.
